/******************************************************************************
 *        "Chirping" - Synchronization between Robots through Beeps           *
 ******************************************************************************
 *                                                                            *
 * Jean-Roch LAUPER, unifr.ch, Spring 2019                                    *
 * Video: https://youtu.be/VFgOjUQfNQg                                        *
 *                                                                            *
 * Theoretical Source:                                                        *
 *      MIROLLO, STROGATZ (1990):                                             *
 *      "Synchronization of Pulse-Coupled Biological Oscillators"             *
 *                                                                            *
 * ************************************************************************** */


/* If finally I don't use SYNC -> clean the code of all the remainings
 from chirping*/

/* BEACUOP DE RESTE DE CHIRPING ? ENLEVER SI JE CONTINUE
 * ? NE PAS UTILISER SYNC FOR ALPHA-ALGO
 
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "e_epuck_ports.h"
#include "e_init_port.h"
#include "e_led.h"
#include "e_ad_conv.h"
#include "e_uart_char.h"
#include "btcom.h"
#include "utility_mod.h"

#include "e_sound.h"
#include "e_agenda.h"


#include "e_fft.h"
#include "e_fft_utilities.h"
#include "e_micro.h"

#include "e_motors.h"
#include "e_prox.h"


#include "run_alpha_algo.h"
#include "runbreitenberg_mod3.h"
#include "e_freq_sound.h"
//for light_lover_addition

#define ON 1
#define OFF 0


/* ********** UART Text Color ********** */
/* works with Putty :-) */

#define RED   "\033[1;31m"
#define GRN   "\033[1;32m"
#define YEL   "\033[1;33m"
#define BLU   "\033[1;34m"
#define MAG   "\033[1;35m"
#define CYN   "\033[1;36m"
#define WHT   "\033[1;37m"
#define COL_RESET "\033[1;0m"

/* ex printf(RED "red\n" COL_RESET); */

/* ************************************* */


#define BASIC_SPEED 200

#define TIC 1
/* settle  the sensiblity of adjustement 
 and is inversely proportional to T:
 e.g. TIC 10/ T 50 -> gives roughly the same total cycle time as
 TIC 1 / T 500 */
/* for the right sensibility -> really to try experimentally*/

/* duration between timer incrementation
 *  - 100 during test to see what happens
 *      without fft recognition 1000 -> gave roughly 1 second
 *  - with fft : dividing by 10 at least

 *  Note: wait_jr_with_beeps_and_light_check(int x) check fft every 1 TIC
 *  but the process take time.*/


#define TIME_FOR_PROX 100
/* Empirical Tests: 
 * 100 ok
 * 75 as soon as carton darker... goes straight
 * 50, 10 too low : the e-puck goes into the wall
 */

/* used with wait and not wait_jr
 * roughly : 2000 = 1ms
 */

/* the time during each TIC devoted to prox detection
 * if none ->  robots are not enough reactive
 * each TIC a time devoted to fft a time devoted to prox
 * A balance to find : 
 * TIME_FOR_POX as low as possible
 * while e-pucks reactive enough
 *  */

#define T 100
/* when prox and micro -> need of swith 
 * - e_init_ad_scan(MICRO_ONLY)
 * - e_init_ad_scan(ALL_ADC)
 * takes time -> need to decreased
 * without switch: 250
 * with switch : 50 */

/* T = 100 verbose off -> 1 beep every 1.1 sec*/


// must be < 270 -> otherwise a timer go back to 0 and reset
// Cycle period
/* for alpha-algo I want 100
 * Indeed: for having count_beep at different time for each
 * robot -> I will make them emit with an offset corresponding
 * to the last 2 number of them pin (no one has the same) */

#define TIC_BETWEEN_BEEPS 3
// the minimum number of beep between beep offset -> to avoid beep juste one after another
// You have to take into account T... to have enough possible offsets



// Sound detection 
#define MIC_USED 0
static int vol_freq_MIN_thresh = 270; //40, 50, 300 325
// 400 is ok when just two or three robots, but not beyond...
// 350
static int vol_freq_MAX_thresh = 1000; // beyond, robot hears its own beep

// note : for 0.5 duration -> duration has an impact on the volume!

// empirical measurements
// 25 should allow recognition to a long distance (around 100cm)
//int vol_freq_MIN_thresh3 = 30;
//#define COUNT_BEEP_FREQU_NB 7
//int count_beep_frequ[7] = {2835, 5671, 8507, 11343, 13406, 13535,14179}; 
/* value for 2816 Hz emitted by a robot
 * at low volume 2835 sounded enough
 * at higher volumes: {2835,8507,14179}
 * but finally : need all
 * see previous tests for music_comm */
/* later: need to add other frequencies : coming from empirical tests:
 13406, 13535
 => especially important not to count too many times a same beep */

/* CHANGES - at start I used different beep than sync and different volume
 * because I'd think to use two beeps
 * But now (25.07.2019) that I use one beep
 * => I prefer using the one from the sync
 */

//#define COUNT_BEEP_FREQU_NB 3
//int count_beep_frequ[3] = {3093,9281,15468};
//// values for 3072hz emitted by a robot
//// see previous tests for music_com

//#define COUNT_BEEP_FREQU_NB 7
//static int count_beep_frequ[COUNT_BEEP_FREQU_NB] = 
//    {2191, 2320, 2449, 2578,
//    7089, 11859, 11730};
// values for 3072hz emitted by a robot / BUT with ALL_ADC
// change value -> new measurement

//// new version with e_freq_sound for beep emit and all_adc
#define COUNT_BEEP_FREQU_NB 4
static int count_beep_frequ[COUNT_BEEP_FREQU_NB] =
    {10570,  8378,  15339,  10441};
//(by order of occurrences number)


/* ******************** Global variables ************************************ */

/* carefull!!! for readibility : I used just "t" for a global variable
But do not use t!!! for other */
static int t = 0; // time - internal timer of the oscillator - in [0;T]
/* first I try with a simple incremented variable
 * too see if precise enough or if I'd better to add a real timer */

static bool verbose = false; // default

static char alpha_buffer[100]; // alpha_buffer used for uart comm - 

static bool count_beep_detected = false;
static bool prev_count_beep_detected = false;
static bool prev_prev_count_beep_detected = false;

static int count_beep_offset = 50;
static int total_count_beep = 0; // -1 as each robot detects its own beep
static int goal_total_count_beep;


#define ALPHA_CLOSE_BEHAVIOR_ 0
#define ALPHA_TOO_FAR_BEHAVIOR_ 1

static int current_behavior = 0;
static int light_direction = -1;

// these two variable are updated via beep_check;

/* ********* FFT related functions and var ********** */

/* Extern definitions */
/* D?finitions externes des variables globales, des diff?rents tableaux ou seront stock?s les signaux des diff?rents micros et la FFT du micro choisi, et
d?finitions de fonctions utiles de moyennage et de copie d'un buffer ? l'autre*/ 
/* Typically, the input signal to an FFT routine is a complex array containing samples of an input signal. */
/* For this example, we will provide the input signal in an array declared in Y-data space. */
extern fractcomplex sigCmpx[FFT_BLOCK_LENGTH] __attribute__ ((section (".ydata, data, ymemory"),aligned (FFT_BLOCK_LENGTH * 2 *2)));      		
/* Access to the mic. samples */
extern int e_mic_scan[3][FFT_BLOCK_LENGTH];


/*! \brief Localize the bigger pic of the array
 * \param spectre The array in which the FFT was made
 * \param spectre_length The length of the scan in the array
 * \return The index of the bigger pic detected
 */
static int localise_pic_max(fractcomplex *spectre, int spectre_length)
{			
	int i = 0 ;
	long ampl_max= 0 ;	// Initialisation de l'amplitude maximale
	long ampl_courante = 0 ;  //Initialisation de l'amplitude courante
	int pic_max;
	for (i = 0; i < spectre_length/2; i++)	
	{
		ampl_courante = spectre[i].real*spectre[i].real+spectre[i].imag*spectre[i].imag ; // Calcul de l'amplitude de la FFT ? la position i courante
		
		if (ampl_courante>ampl_max)						// Si l'amplitude courante est plus grande que l'amplitude maximale m?moris?e jusqu'ici...
			{
				pic_max = i ;							// La position du Maxima est m?moris?e dans k_max_1
				ampl_max = ampl_courante ;				// La valeur de l'amplitude maximum est remplac?e par la valeur courante
			}
	}
	return pic_max;
}

/*! \brief Get the max volume of the sound detected
 * \param spectre The array in which the FFT was made
 * \param pic_pos The index of the louder frequency
 * \return The amplitude of the louder frequency
 */
static int get_volume(fractcomplex *spectre, int pic_pos)
{
	if(pic_pos < 0 || pic_pos >= FFT_BLOCK_LENGTH/2)
		return 0;
	return spectre[pic_pos].real*spectre[pic_pos].real+spectre[pic_pos].imag*spectre[pic_pos].imag;
}




/* #################### Functions LIST ###################################### */

// fft functions
static int calcul_frequence(int pic_pos);
static void beeps_check();


// utilities
static void wait_jr_with_beeps_and_light_check(int x);
static void status_print();
static void total_count_display();


// behaviors
//static void sync_beep_emit();
static void count_beep_emit();
//static void cross_led();

static void alpha_close_behavior();
    // basically breitenberg explorer when they are close enough
static void alpha_too_far_behavior();

// states

static void start();
static void ordinary_step();


/* ########################################################################## */


/* ******************** FFT Functions ***************************** */

// test-fake version
//bool beeps_check(){    
//    // for simulation : beep emitet in random way from time to time
//    if (rand() % (10) == 1) return true;
//    else return false;    
//    return false;
//}


/*! \brief Calcul the corresponding frequency of the bigger pic detected
 * \param pic_pos The index of the bigger pic detected
 * \return The corresponding frequency
 */
static int calcul_frequence(int pic_pos)
{
	return (pic_pos*33000)/FFT_BLOCK_LENGTH;
}

/* not very clean: but I use two global variables to return the check
 * bool sync_beep_check
 * bool count_beep_check
 */

static void beeps_check(){     
    
    int volume;
	int pos_pic;
	int frequency;    
    int i;   
    
    count_beep_detected = false;
    
//     e_init_ad_scan(MICRO_ONLY);
    
    
    /* I put it at the end of front_ir_ok 
     * since we need to switch for e_inid_ad_scan(ALL_DC) only for IR_prox
     */    
    
    e_ad_scan_on(); // micro scan
    while(!e_ad_is_array_filled()); //wait for values acquisition
    e_ad_scan_off();
//    e_init_ad_scan(ALL_ADC);
    volume = e_get_micro_volume(MIC_USED); // mic 0

    // Signal centering around zero (average = 0))
    e_subtract_mean(e_mic_scan[MIC_USED], FFT_BLOCK_LENGTH, LOG2_BLOCK_LENGTH);

    // Copie le signal du micro zero dans le buffer destin? ? effectuer la FFT
    // Affecte les valeurs ? sigCmpx.real (r?els) et 0 ? sigCmpx.imag (imaginaires)
    e_fast_copy(e_mic_scan[MIC_USED], (int*)sigCmpx, FFT_BLOCK_LENGTH);

    // Le r?sultat est sauvegard? => On d?marre une nouvelle acquisition
    e_ad_scan_on();
    // JRL seems not necessarry? or AM I wrong / necessary
    

    // Execution de la FFT sur le buffer
    e_doFFT_asm(sigCmpx);

    // Recherche de la position K des deux fr?quences maximum 				
    pos_pic = localise_pic_max(sigCmpx, FFT_BLOCK_LENGTH);
    

//    e_init_ad_scan(ALL_ADC); // to make prox working again

//    frequency = calcul_frequence(pos_pic);                                        
    /* put outside the below if for the cycle being of a same length
     * for every robot whatever the number of frequ above the volume
     * otherwise: the cycles start not being the same length for
     * every robot / and like with chirping... overlap  */
    
    if(volume > vol_freq_MIN_thresh 
            && volume < vol_freq_MAX_thresh)
    {
        frequency = calcul_frequence(pos_pic);                                        
        
        if(verbose) {
            sprintf(alpha_buffer, BLU "vol: %d\t freq:%d \n\r" COL_RESET,
                    volume, frequency);
            btcomSendString(alpha_buffer); 
        }

        
        for (i = 0; i<COUNT_BEEP_FREQU_NB; i++){
            if (frequency == count_beep_frequ[i]) {
                count_beep_detected = true;
//                e_set_led(7,1);
//                e_set_led(1,1);
            }
        }        
    } 
    
}



/* ******************** UTILITIES ************************** */

#define DELAY1 3
#define DELAY2 64
// makes roughly 1ms together

static void wait_jr_with_beeps_and_light_check(int x) {    
    int k ;  
//    bool prev_sync_beep_detected = sync_beep_detected;
    prev_prev_count_beep_detected = prev_count_beep_detected;
    prev_count_beep_detected = count_beep_detected;
    
    /* At start: thought a good idea to fft check only punctually
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
    
    /* note : I keep on check (beeps_check()) even if tone recognized
     in order to have always a similar time for wait_jr_with_beeps_and_light_check(x) 
     and no random variation depending on when during the k loop it recognizes
     the right tone*/
    
    for (k = 0; k < x; k++) {                                          
        beeps_check();
        // light_lover_addition
        light_direction = light_direction_neuron();
        wait(TIME_FOR_PROX); // to let the prox work enough
        /* update the two global var
         *  sync_beep_detected
         *  count_beep_detected*/
//        if (beeps_check() == true ) {
//            sync_beep_detected = true;           
//        }             
    }
    /* (sync_beep_detected && sync_beep_detected != prev_sync_beep_detected
     * => to avoid the fact that a same frequ can be detected several times
     * especially annoying for count_beep_detected
     * especially because the number of times a same beep can be counted
     * varies with distance (in other words with volume)
     */ 
    
    if(verbose) {
        sprintf(alpha_buffer, "count_beep_detected: %d \tprev_count_beep_detected: %d \r\n",
                count_beep_detected, prev_count_beep_detected);
        btcomSendString(alpha_buffer);
    }

    
    // need of the two prev_ prev_prev_ sometimes just a jump and
    // recognized two many times even with prev_
    if(count_beep_detected 
            && prev_count_beep_detected == false
                && prev_prev_count_beep_detected == false) 
    {
        total_count_beep++;
//        e_set_led(0,1);
//        cross_led(ON);
        if(verbose) {
            sprintf(alpha_buffer, 
                    CYN "Count Frequ Recognized. Total: %d \r\n" COL_RESET,
                    total_count_beep);
            btcomSendString(alpha_buffer);                            
        }
//        cross_led(OFF);
//        e_set_led(0,0);
    }
    
}

static void status_print(){
    if(verbose) {
        sprintf(alpha_buffer, "t: %d \t count_beep_offset: %d\t current_behav: %d \r\n", 
                t, count_beep_offset, current_behavior);
        btcomSendString(alpha_buffer);
//    sprintf(alpha_buffer, "TMR1: %d \tTMR2: %d \tTMR3: %d \tTMR4: %d \tTMR5: %d \r\n", 
//            TMR1, TMR2, TMR3, TMR4, TMR5);
//    btcomSendString(alpha_buffer);
    }
}

static void total_count_display(){    
    if (total_count_beep >= 1) e_set_led(2,1);
    if (total_count_beep >= 2) e_set_led(3,1);
    if (total_count_beep >= 3) e_set_led(4,1); 
    if (total_count_beep >= 4) e_set_led(5,1);
    if (total_count_beep >= 5) e_set_led(6,1);
    if (total_count_beep >= 6) e_set_led(7,1);
    if (total_count_beep >= 7) e_set_led(0,1);
    if (total_count_beep >= 8) e_set_led(1,1);        
}


/* ******************** BEHAVIORS *************************** */

//Duration for sound sound playing
#define STD_TONES_DURATION 1440
#define STD_WAITING_DURATION 150
// for duration for sound (to have the time to  be emitted)
// used in a wait_jr ~150 ms

static void count_beep_emit() {    
    float std_percent = 0.5;
    /* e_const_sound.s */
    
//    e_set_led(0,1);
//    ao_beep(880,0.1);
//    e_init_sound(); 
    /* it is strange to place e_init_sound() here and not one for all
     at the start of run_chirping() 
     However, if I did it in this "usual" way -> epuck HWRev 1.3
     emits a sound then switch off... hence its presence here
     please see my comment in run_chirping() for more details */    
    
    /* 1 beep:
     - enough for recognition by epuck HWRev 1.3
     - but not for epuck HWRev1.1
     2 beeps : recognized by both...
     but too much : overlaps all the time...
     => attempts with two beep with smaller STD_WAITING_DURATION
     if not work*/
    
//    e_play_sound(0, (int)(std_percent*STD_TONES_DURATION));
//    wait_jr((int)(std_percent*STD_WAITING_DURATION));
    
    // new version with e_freq_sound
    freq_beep1_3072(std_percent);

    if(verbose) {
        sprintf(alpha_buffer, MAG "########### COUNT BEEP! ########## \r\n" COL_RESET);    
        btcomSendString(alpha_buffer);                
    }
    
//    e_close_sound();
//    e_set_led(0,0);
}



//static void cross_led(int on_var){
//    int i; 
//    for (i=0;i<8;i+=2){ 
//        e_set_led(i,on_var);
//    }
//}

static void stop_behavior() {
    e_set_speed_left(0);
    e_set_speed_right(0);
}  

/* basis for half_turn behavior : 
 * demoEPuck (Besuchet)
 * runlocatesound.c
 */
//#define TURN_SPEED 1000
#define TURN_SPEED 1000
#define STEPS_FOR_2PI 1250
// initial value 1300 too-much by experiences
static void left_half_turn_behavior() {
    int end_turn;
    e_set_steps_right(0); //Set the number of left motor steps
    e_set_speed_left(-TURN_SPEED);
    e_set_speed_right(TURN_SPEED);
    end_turn = (int)(STEPS_FOR_2PI*0.5);
    while(e_get_steps_right() < end_turn) {  // turn until done
        if (t == count_beep_offset)     
           count_beep_emit();    
        t++;
        wait_jr_with_beeps_and_light_check(TIC);
    }    
}

// when robot hear enough other robots
static void alpha_close_behavior() {   
////    e_pause_agenda(left_half_turn_behavior);
//    e_pause_agenda(snake_led);
	e_restart_agenda(alpha_explorer_neuron);    
    
//    wait_jr_with_beeps_and_light_check(0*TIC); 
    // to compensate partially the time taken in case of alpha_too_far_behavior();
    // value find through tests 
    // => surprisingly close without doing anything :-)
}

/* happens at the start of the ordinary step if 
 * at the end of the previous cycle, not enough beeps
 * So we have to keep counting the t is we want to keep the offset
 * This is very roughly done.
 * */
//#define TURN_TIME 20
// in TIC
static void alpha_too_far_behavior() {

    e_pause_agenda(alpha_explorer_neuron); 
//    e_set_led(8,1);

    left_half_turn_behavior();
    
//    e_led_clear();
    e_restart_agenda(alpha_explorer_neuron);     
    current_behavior = ALPHA_CLOSE_BEHAVIOR_;
}

 

/* ******************** STATES ***************************** */

/* decision : working with time t instead of phi */
/* see scan of the "automaton" (not a real NFA) */



static void start() {    
//    e_start_agendas_processing(); 
    // reseting TMR2 and connected objects
    // goal -> avoiding reset by going too high with TMR2
    // to see if necessary
    
//    e_set_led(4,1);
    t = 0;
    total_count_beep = 0; // -1 as each robot detects its own beep
    if (verbose) {
        sprintf(alpha_buffer, GRN "[Cycle Start]\r\n" COL_RESET);
        btcomSendString(alpha_buffer);
        status_print();
    }    
    
    e_activate_agenda(alpha_explorer_neuron, 650);
    e_led_clear();
    wait_jr_with_beeps_and_light_check(TIC);
    
    ordinary_step();
    
 }


static void ordinary_step() {        
//    front_ir_ok(); // this was just for test purpose (with e_init_ad_scan)
//    rear_ir_ok();
//    total_count_display();
     
    if (light_direction > -1) {
            e_set_body_led(1);
            e_set_led(8,1);
            // goal : the robot seeing light becomes itself a beacon            
        } else {
            e_set_body_led(0);
            e_set_led(8,0);
            e_set_led(1,1);
            e_set_led(7,1);
        }
    
    if (current_behavior == ALPHA_CLOSE_BEHAVIOR_) 
        alpha_close_behavior();
    else
        alpha_too_far_behavior();
    
    if (t == count_beep_offset) {        
        count_beep_emit();
    }

    //for light_lover_addition
    // once in the middle of the cycle
    if (t == 2*T/3) {
        
        if (light_direction == 0 ||
                light_direction == 7 ||
                    light_direction == -1) {
            wait_jr_with_beeps_and_light_check(20*TIC);
            /* to compensate the fact that the robot does nothing in this case
             * in comparison with the other case in which it take time to turn to light             
             the number of TIC is found empirically and is rough */            
        } else {
            e_pause_agenda(alpha_explorer_neuron);
        }
        if (light_direction > -1) {
            e_set_body_led(1);
            e_set_led(8,1);
            // goal : the robot seeing light becomes itself a beacon            
        } else {
            e_set_body_led(0);
            e_set_led(8,0);
            e_set_led(1,1);
            e_set_led(7,1);
        }
        light_lover_neuron(light_direction);        
        
         e_restart_agenda(alpha_explorer_neuron);                   
               
    }
    
    
    if (t < T) {    
        t++;                
        status_print();    
        wait_jr_with_beeps_and_light_check(TIC);
        ordinary_step();
    
    } else if (t >= T) {
                       
        if (verbose) {
           sprintf(alpha_buffer, 
                    CYN "Total beeps recognized during this Cycle: %d \r\n" COL_RESET,
                    total_count_beep);
            btcomSendString(alpha_buffer);   
        }
                
        if (total_count_beep < goal_total_count_beep)
            current_behavior = ALPHA_TOO_FAR_BEHAVIOR_;
        else
            current_behavior = ALPHA_CLOSE_BEHAVIOR_;        
    }  // stops here and the main while start anew
}
    


/* ****************************************************************** */

/*  e_pucks_close_number: # of beep (and epucks) that must be heard to be considered close enough
 *  */

int run_light_alpha_algo(int e_pucks_close_number, int count_beep_offset_var, 
                bool verbose_var, int current_bevior_var){  
    
    goal_total_count_beep = e_pucks_close_number;
    verbose = verbose_var;
    count_beep_offset = (count_beep_offset_var * TIC_BETWEEN_BEEPS) % T;    
    current_behavior = current_bevior_var;
//    e_init_port();    			// configure port pins	
    
    /* IMPORTANT NOTE e_init_ad_scan
        e_init_ad_scan(___) 
        - for fft : I need e_init_ad_scan(MICRO_ONLY)
            otherwise the frequencies are strange...
            roughly twice the frequency emitted by a frequency generator
        - but for prox_sensor I need e_init_ad_scan(ALL_ADC)
        I tried to alternate both - it seems to work :-) 
     */
        

/* all the inits are done in the main program */ 
    /* this is done
     * 1/ to avoid some time taken at each loop
     * 2/ to avoid stopping after 5 cycles -> you need to do the loop in the main 
     * program */    
    
//    e_init_ad_scan(ALL_ADC);	// configure Analog-to-Digital Converter Module
//    e_calibrate_ir();    
//    e_init_uart1();	// initialize UART to 115200 Kbaud           
//    e_init_sound();   
    /* strange issue with epucks HWRev 1.3 => solved by placing e_init_sound
       directly in sync_beep_emit() instead of here.
     - if I place e_init_sound here -> they play a beep and stop :-(
     - if I place e_init_sound in sync_beep_emit() -> works
     playing sound call different things in assembler files
     perhaps some unexpected interaction with the e_init_ad_scan...
     */    
//    e_init_motors();    
//    e_start_agendas_processing(); 
    /* necessary among others for e_motors advanced    
     * w/o it : no movement of the wheels  
     * This is fully understandable as e_init_motors() in the advanced
     * version is 
     * void e_init_motors(void)
            {
              e_activate_agenda(run_left_motor, 0);
              e_activate_agenda(run_right_motor, 0);
            }
    */
    

    if (verbose) {   
        btcomSendString(GRN "\r\nrun_light_alpha_algo - Start\r\n");
        btcomSendString("--------------------\r\n" COL_RESET);
    }
    
        
//    //Reset if Power on (some problem for few robots)
//	if (RCONbits.POR) {
//		RCONbits.POR=0;
//		__asm__ volatile ("reset");
//	}
    
//    e_init_ad_scan(MICRO_ONLY);    
//    e_led_clear();      
    
//    e_init_ad_scan(ALL_ADC) // doit avoir ?t? lanc? dans main
    
    if (AUDIO_ON == 0) 
        e_init_sound();
    
    set_EL_basic_speed(BASIC_SPEED); //400
    start();        
//    alpha_too_far_behavior();
    return current_behavior;        
}

