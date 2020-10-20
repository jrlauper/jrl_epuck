/* Jean-Roch LAUPER, unifr.ch, 2019/2020
 * 
 * >>> More details in the header file 
 * 
 */


/* Program Sections:
 * ----------------
 * - Functions Lists
 * - FFT functions
 * - Maths Functions
 * - Utilities
 * - Behaviors
 * - States
 */


/* Implementation Notes:
 * ---------------------
 * 
 * #Note (1):
 *  
 * Delicate balance to find between 
 *  - the length of the emitted BEEP and
 *  - TIC
 *  => what is at stake : epsilon-increase (charge increase)
 * The epsilon increase must happen 
 * once and not several time when the robot recognize one beep.
 * 
 * TIC time during which the robot checks for frequency
 * (it is listening to "see" if it hears a beep)
 *      => if TIC too short, 
 *          risk of recognizing two beeps,
 *          when in fact it hears only 1.
 *      => if TIC too long, 
 *              risk of making the robot too slow in its reactions
 * Emitted BEEP : 
 *      - if too short -> risk of none recognition
 *       - if too long =>  will be recognized several times
 *          and triggers several epsilon-increase
 * 
 * 
 * #Note (2) : 
 * Implementation decision : 
 * small waiting time : always AFTER changing the value of t
 */

#include "run_chirping.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_ad_conv.h"
#include "e_agenda.h"
#include "e_uart_char.h"
#include "btcom.h"

#include "e_led.h"
#include "e_motors.h"
#include "e_prox.h"

#include "e_sound.h"
#include "e_micro.h"

#include "e_fft.h"
#include "e_fft_utilities.h"

// JRL Libraries
#include "e_freq_recognition.h"
#include "e_freq_sound.h"
#include "utility_mod3.h"


//#define VERBOSE (true)
#define VERBOSE (false)


#define WITH_SOUND true
#define WITHOUT_SOUND false


#define PI 3.14159265358979


// BEEP volume setting
// the speaker of e-puck HWrev 1.1. is louder
#define BEEP_EMIT_HWRev1_3 80
#define BEEP_EMIT_HWrev1_1 60

// Sound detection 
int vol_freq_thresh = 50; 
//int vol_freq_thresh = 30; 
/* empirical measurements
* 25 should allow recognition to a long distance (around 100cm) */


#define TIC 1
//#define TIC 7
/* Sensiblity Adjustement 
 inversely proportional to T:
 e.g. TIC 10/ T 50 -> gives roughly the same total cycle time as TIC 1 / T 500 */
/* for the right sensibility -> really to try experimentally*/

/* duration between timer incrementation
 *  - 100 during test to see what happens
 *      without fft recognition 1000 -> gave roughly 1 second
 *  - with fft : dividing by 10 at least
 *  Note: wait_jr_with_beep_check(int x) check fft every 1 TIC
 *  but the process take time.*/


#define T 300
//#define T 50
//#define T 50
// Cycle period
/* chose 75 for ~10 second / cycle -> if you want to have more time to see what happens*/


#define B 8.
/* dissipation factor (see paper); must be >0
* higher -> faster convergence
* 8 a bit too slow with the new EPSILON */


#define EPSILON 1./T
// pulse strength; must be in [0,1] 
// what is added to f(phi(t)) when hearing another oscillator firing



/* ******************** Global variables ************************************ */

/* careful!!! for readability : I used just "t" for a global variable
But do not use t for other var */
int t = 0; // time - internal timer of the oscillator - in [0;T]

static char chirping_buffer[100]; // buffer used for uart comm - 

static bool beep_detected = false;



/* #################### Functions LIST ###################################### */

// fft functions
static bool beep_check();

// math functions used
static double phi_(int t);
static double f_(double phi);
static double g_ (double u);

// utilities
int get_chirping_cycle_length();
static void wait_jr_with_beep_check(int x);
static void status_print();


// behaviors (firing behaviors)
static void beep_emit();
static void move();
static void cross_led();

// states
static void ordinary_step();
static void firing(bool sound);
static void epsilon_increase();
static bool front_ir_ok();
static bool rear_ir_ok();


// main
static void very_first_run();
void run_chirping(); 

/* ########################################################################## */




/* ******************** FFT Functions ***************************** */

// Frequencies to recognize

#define COUNT_BEEP_FREQU_NB 12
static int beep_frequ[COUNT_BEEP_FREQU_NB] =
    {2191, 2320, 2449, 7089, 
	7218, 7347, 8378, 10441,
	10570, 11859, 11730, 15339};

/* Frequencies recognized by the fft module
 * when another e-puck emits a beep at 3072Hz with freq_beep1_3072
 * with e_init_ad_scan(ALL_ADC)
 * See the project report for the importance of ALL_ADC and about the 3072 mistake) */


static bool beep_check(){ 
    /* returns true if a beep of 
     * - the correct frequency and of
     * - the correct volume (louder enough)
     * has been detected */    
    
//    int volume = 0;	
	int frequency;    
    int i;    
    
    bool result = false;
    
    fft_set_MIN_volume_threshold(vol_freq_thresh);
    // the value of the max threshold is set to 1000 by default
    // => ]MIN, max[ gives a volume interval within which detecting the frequency
    
    frequency = fft_get_frequency();
    // reminder : fft_get_frequency -> get the frequency
    // between a min and a max volume 

    /* I check every beep_frequencies possible in any case 
     * (no use of break in the for loop)
     * for synchronicity reason */    
    for (i = 0; i<COUNT_BEEP_FREQU_NB; i++){
        if (frequency == beep_frequ[i]) {                                
            result = true;
        }
    }            
            
    return result;
}



/* ******************** Maths Functions ************************************* */
/* see MIROLLO & STROGATZ (1990), esp. p.1650, fla 2.14 & 2.15 */

// phi is in [0,1]
// return the phase of the oscillator given its internal timer
static double phi_(int t){
    return ((double)t)/T;    
}


// f(phi(t)) return the state of the oscillator given its current phase phi
// several functions are possible, I take 2.15 in MIROLLO & STROGATZ
static double f_(double phi) {
    double result;
    result = exp(B)-1;
    result *= phi;
    result += 1;
    result = log(result);
    result /= B;        
    return result;
}


static double g_ (double u){
    double result;    
    result = exp(B*u)-1;
    result /= exp(B)-1;
    return result;
}



/* ******************** UTILITIES ******************************************* */


int get_chirping_cycle_length(){
    return T*TIC;
}


#define DELAY1 3
#define DELAY2 64
// makes roughly 1ms together

static void wait_jr_with_beep_check(int x) {    
    int k ;        
    
    /* # Implementation note
     * ----------------------
     * 
     * At start: thought a good idea to fft check only punctually
     * - I had a structure similar to wait_jr : 
     * for i = 0 to DELAY2 /
     *  for j = 0 to DELAY 1
     *      for k = 0 to x
     *          check
     * => but not enough
     * 
     * Finally : this time without fft detection has only cons and no pros
     * cons: beep sometimes missed...
     * => the computation work is not too heavy
     */
    
    /* # note : 
     * I keep on checking (beep_check()) even if tone recognized
     * in order to have always a similar time for wait_jr_with_beep_check(x) 
     * and no random variation depending on when during the k loop it recognizes 
     * the right tone*/
    
    for (k = 0; k < x; k++) {                                          
        if (beep_check() == true ) {
            beep_detected = true;           
        }             
    }
    if(beep_detected && VERBOSE) {
        btcomSendString(YEL "Frequ Recognized\r\n" COL_RESET);                            
    }
    
    e_set_body_led(beep_detected);     
    /* if the frequency is detected : 
     * - robot's body in green
     * - otherwise: switched off  */    

}


// returns the key value of the inner working of the robot
static void status_print(){     
    if (VERBOSE) {
        sprintf(chirping_buffer, "t: %d \tphi: %.3f \tf: %.3f \r\n", 
                t, phi_(t), f_(phi_(t)));    
        btcomSendString(chirping_buffer);
    }
}



/* ******************** BEHAVIORS ******************************************* */


static void beep_emit() {        
    if (isEpuckVersion1_3())
        set_volume(BEEP_EMIT_HWRev1_3);
    else
        set_volume(BEEP_EMIT_HWrev1_1);            
    
    freq_beep1_3072(0.4);    
    
    if (VERBOSE) {
        sprintf(chirping_buffer, YEL "########### BEEP! ########## \r\n" COL_RESET);    
        btcomSendString(chirping_buffer);                
    }    
    
}


static void move(){     
    int target_speed = 400;
    int speed;
    
    int move_duration = 30;        
    int x = move_duration;    
    int i, j, k ;
    
//    e_init_ad_scan(ALL_ADC);
    
    // move forwards
    /* wait_jr() adapted for the current purpose
     * i.e. in the current case, the robot must not wait doing nothing
     * but must check sth while "waiting" and moving
    */
    for (k = 0; k < x; k++) {
        for (i = 0; i < DELAY2; i++){
            for (j = 0; j < DELAY1; j++) ;
        }
        
        if(VERBOSE) status_print();                
        
        if (front_ir_ok()) speed = target_speed; 
        else speed = 0;
        
        e_set_speed_left(speed);
        e_set_speed_right(speed);                    
    }

    // move backwards    
    for (k = 0; k < x; k++) {
        for (i = 0; i < DELAY2; i++){
            for (j = 0; j < DELAY1; j++) ;
        }
        
        if(VERBOSE) status_print(); 
        
        if (rear_ir_ok()) speed = -target_speed; 
        else speed = 0;
        
        e_set_speed_left(speed);
        e_set_speed_right(speed);
    }
        
    speed = 0;
    e_set_speed_left(speed);
    e_set_speed_right(speed); 
    
}


static void cross_led(){
    int i; 
    for (i=1;i<8;i+=2){ 
        e_set_led(i,1);
    }
}



/* ******************** STATES ********************************************** */


//#define IR_THRESHOLD 200 
#define IR_THRESHOLD 500 
// with reflecting tape, 500 ok


static bool front_ir_ok() {
    // to avoid collision between robot during the firing move
    
    bool ok = true;
    
    if(e_get_calibrated_prox(6) > IR_THRESHOLD && ok) ok = false;
    if(e_get_calibrated_prox(7) > IR_THRESHOLD && ok) ok = false;
    if(e_get_calibrated_prox(0) > IR_THRESHOLD && ok) ok = false;
    if(e_get_calibrated_prox(1) > IR_THRESHOLD && ok) ok = false;
        
    if (VERBOSE) {
        sprintf(chirping_buffer, MAG
            "[wide front] (L) IR6=%d, IR7=%d | (R) IR0=%d, IR1=%d \n\r" COL_RESET,
                e_get_calibrated_prox(6),
                e_get_calibrated_prox(7), 
                e_get_calibrated_prox(0),
                e_get_calibrated_prox(1));   
        btcomSendString(chirping_buffer);
    } else {
        wait_jr(2); // put in comments if using bt comm for front_ir_ok    
    }
        
    return ok;
}


static bool rear_ir_ok() {
    
    bool ok = true;
    
    if(e_get_calibrated_prox(4) > IR_THRESHOLD && ok) ok = false;
    if(e_get_calibrated_prox(3) > IR_THRESHOLD && ok) ok = false;    

    if (VERBOSE) {
        sprintf(chirping_buffer, MAG
            "[rear] (L) IR4=%d | (R) IR3=%d \n\r" COL_RESET,
                e_get_calibrated_prox(4),
                e_get_calibrated_prox(3));             
        btcomSendString(chirping_buffer);
    } else {    
        wait_jr(2); // put in comments if using bt comm for front_ir_ok
    }
      
    return ok;
}


static void ordinary_step() {        
    // key function of the algo
   
    if (phi_(t) < 1) {
        t++;        

        if (VERBOSE) {
            sprintf(chirping_buffer, "[ordinary_step]\r\n");
            btcomSendString(chirping_buffer);
            status_print();    
        }                

        wait_jr_with_beep_check(TIC);    
        
        if (beep_detected) { // beep_dected: global bool var
            epsilon_increase();
        } else if (phi_(t)>=1) {        
            firing(WITH_SOUND);
        }

    }
}


static void firing(bool sound) {        
    
    if (VERBOSE) {
        sprintf(chirping_buffer, "[firing]\r\n");
        btcomSendString(chirping_buffer);    
//        status_print();
    }
    
    e_set_led(8,1); // 8 -> all                
    if (sound) beep_emit();           
    move();  
    e_led_clear();
    t= 0;        
}


static void epsilon_increase() {
    //beep_detected = true -> triggered epsilon_increase())
    
    double phi;         
    
    if (phi_(t) < 1) {            
        
        if(VERBOSE) {
            sprintf(chirping_buffer, RED "[epsilon_increase]\r\n");
            btcomSendString(chirping_buffer);
            sprintf(chirping_buffer, "t: %d \tphi: %.3f \tf: %.3f \tg: %.3f\r\n" COL_RESET, 
                    t, phi_(t), f_(phi_(t)), g_(f_(phi_(t))+EPSILON));
            btcomSendString(chirping_buffer);
        }

        phi = g_(f_(phi_(t)) + EPSILON);
        t = phi*T; // reminder: t is global
        
        if (VERBOSE) 
            status_print();
    
        beep_detected = false;        
     
        if (phi >= 1) {        
            firing(WITHOUT_SOUND);
        } 
        // otherwise -> ordinary step called by the loop again
        
    } else //phi >= 1
        firing(WITHOUT_SOUND);
}



/* ********** MAIN ********************************************************** */


static bool first_run = true; // global var


static void very_first_run(){
    int random;
    
    e_led_clear();      
    if (VERBOSE)
        btcomSendString("Run_chirping_offset_wait\r\n");    

    random = jr_random(get_chirping_cycle_length()/TIC);  
    if (VERBOSE) {
        sprintf(chirping_buffer, "random_chirping: %d \r\n", random);
        btcomSendString(chirping_buffer);
    }

    wait_jr_with_beep_check(random); 
        /* beep_check is not really used -> this is just to get really a time
         * between 0 and cycle_length */
    first_run = false;

    firing(WITH_SOUND);  
    first_run = false;      
}


void run_chirping(){  
   
    if (VERBOSE) {   
       btcomSendString(GRN "\r\nrun_chirping - Start\r\n");
       btcomSendString("--------------------\r\n" COL_RESET);
    }
    
    if (first_run) {
        very_first_run();        
    }

    // important that the ordinary_step is in the main loop of the main.c file
    // reason 1: otherwise : strange reset after 5 or 6 cycles (sth known with dsPIC)
    // reason 2: if we use remote control : to be able to stop whenever we want
    ordinary_step();
    
}

