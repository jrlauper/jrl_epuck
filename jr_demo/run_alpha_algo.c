/* Jean-Roch LAUPER, unifr.ch, 2019/2020 
 * 
 * >>> More details in the header file 
 * 
 */

/* Note: alpha = goal_beep_number  */
 


#include "run_alpha_algo.h"

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

// JRL librairies
#include "e_freq_sound.h"
#include "e_freq_recognition.h"
#include "runbraitenberg_mod3.h"

#include "utility_mod3.h"



// settings macros for test

#define VERBOSE (false)
//#define VERBOSE (true)  

#define RECOVERY_VERBOSE (false)
//#define RECOVERY_VERBOSE (true)

#define MOVEMENT (ON)
//#define MOVEMENT (OFF)
/* For test it can be useful to make the e-puck remain static
 * this MOVEMENT enables it */



#define NO_MOVEMENT_WAITING_TICs 5
/* when no movement (MOVEMENT = OFF)
 * you need some time to add extra time to simulate : 
 * typically when the robot would turn on itself. 
 * to do this : wait_jr_with_beeps_check(NO_MOVEMENT_WAITING_TICs);
 * } */


#define ON true
#define OFF false

#define LEFT_MIC 1
#define RIGHT_MIC 0
#define BACK_MIC 2

// Sound detection 
#define MIC_USED RIGHT_MIC




// ***** BEHAVIORS *****
/* the 4 possible behaviors depending on the loss of connection and 
 * total_count above or below alpha */
#define BEEPS_EQUAL_BEHAVIOR 0
// # of beeps equal between two cycles
#define BEEPS_INCREASE_BEHAVIOR 1
#define BEEPS_DECREASE_ABOVE_ALPHA_BEHAVIOR 2
#define BEEPS_DECREASE_BELOW_ALPHA_BEHAVIOR 3



#define LATENCY_AFTER_BEEP 20
    /* goal of this small latency: avoiding the robot 
     * to count its own beep - it happens sometimes but 
     * not every time and not for every robot...
     * Besides - we can't use the max-vol threshold
     * as the beep from other robots can be as loud as their own beep */
    
    /* Empirical Tests for Right Values: 
     *      10 ms : not enough for some robots (in my case 3423 especially sensitive to this issue)
     *      20 ms : works fine
     *      15 seems enough -> if not -> increase
     */ 



#define RANDOM_ANGLE 20
    /* when the e-puck is close enough of other e-pucks (it hears enough beeps)
     * it makes a small random turn between -RANDOM_ANGLE and RANDOM_ANGLE
     * before being in explorer_mode again 
     * cf. BEEPS_INCREASE_BEHAVIOR*/



#define BEEP_EMIT_HWRev1_3 80
#define BEEP_EMIT_HWrev1_1 60
/* volume of the count beep emited: (freq_beep1_3012)
 * the value comes from empirical test to have a similar volume emited
 * context: robot close to each other */



int min_volume_threshold = 150; // for HWRev1_3 micros
/* There is no perfect value for min_volume_threshold,
 * and some compromise/balance must be looked for.
 * See report for more details (in the github of the project), 
 * but in very brief :
 * 1. the non-central position of the SPEAKER causes variations in the volume perceived
 *      depending on the position of the emitting robot; while
 * 2. the position of the three MICROS + the position of the back micro under
 *      the jumper -> causes here too variations in the volume perceived depending on
 *      the position of the listening robot.
 * => both combined : for a same distance between two robots (an emitting and a listening one)
 * the volume perceived can be quite different. */

#define HWREV1_1_MICRO_SENSITIVITY_DIFFERENCE (-17) 
/* the micro of HWRev 1.1. are less sensitive: 15% according to doc 
 * 17% according to our tests 
 * => as they are less sensitive the threshold must be decreased of 17% for them
 */

/* Used only in the 2nd phase of beep_check()
 * when we test if the average volume is loud enough
 * For the 1st phrase of beep_check() - fft frequency recognition -
 * we take into account all the volume from 0 t0 1000 (default threshold value)
 */



#define TURN_SPEED 730
#define BASIC_SPEED 275

/* If for test you want no movement -> use MOVEMENT above
 * 
 * A too high BASIC_SPEED is to detrimental to cohesion
 * you have to find a balance:
 * 
 * My empirical tests : 
 *      600 is too much given the size of the arena -> too much dispersion
 *      400 was ok, but still a bit too much
 *      300-400 good
 *      200 a bit too low
 */

/* speed : had also an impact to the unexpected and unpredictable resets 
 * -> energy issue
 * I had to decrease the TURN_SPEED for this reason
 */



#define T 50
// cycle length
/* T = 100 verbose off -> 1 beep every 1.1 sec*/


#define TIC 1
 /*  wait_jr_with_beeps_check(int x) check fft every TIC
 *  but the process take time.*/

/* is inversely proportional to T:
 e.g. TIC 10 / T 50 -> gives roughly the same total cycle time as
 TIC 1 / T 500 
 for the right sensibility -> really to try experimentally*/


// uncomment if used
//#define TIC_BETWEEN_BEEPS 3
// the minimum number of beep between beep offset -> to avoid beep juste one after another
// You have to take into account T... to have enough possible offsets



#define TIME_FOR_PROX 100
/* Time during each TIC devoted to proximity detection
 * if none ->  robots are not enough reactive
 * Each TIC includes both
 * - a time devoted to fft 
 * - a time devoted to proximity detection
 * A balance to find : 
 *  TIME_FOR_POX must be as low as possible
 *  while e-pucks are still reactive enough
 *  */
/* Empirical Tests: 
 *      100 ok
 *      75 as soon as carton darker... goes straight into it
 *      50, 10 too low : the e-puck goes into the wall
 */
/* used with wait() and not with wait_jr()
 * roughly : 2000 = 1ms
 */



// If Speaker switch ON all the time
#define BEEP_FREQU_NB 12
static int beep_frequ[BEEP_FREQU_NB] =
    {2191, 2320, 2449, 7089, 
	7218, 7347, 8378, 10441,
	10570, 11859, 11730, 15339};
//(by order of occurrences number)

// If HP switch ON just at the time of emission
// new test with (E)1 and 3 and (L) 1 and 3 : 21.10.2019
// for freq_beep1_3072(0.5)
//#define BEEP_FREQU_NB 14
//static int beep_frequ[BEEP_FREQU_NB] =
//    {1417, 2320, 2449, 
//    4640, 4769, 4898,
//    6960, 7089, 
//    11601, 11730, 11859, 11988,
//    14050, 14179};
//(by order of occurrences number)



/* ***** REOVERY MODE CONSTANTS ***** */

/* Recovery mode : when a robot is lost and hear again another robot
 * it doesn't turn as soon as it stops hearing it -> to have more chance to 
 * catch back other robots and to reconstitute coherence
 */
#define CYCLE_WO_BEEP_THRESHOLD 7
/* nb of cycle during which a robot must not hear anything to toggle to RECOVERY mode*/

#define WAIT_BEFORE_TURN_AFTER_RECOVERY 4
/* a number of cycles */
/* the VAR recovery_mode_wait_before_turn = WAIT_BEFORE_TURN_AFTER_RECOVERY
 * WHEN cycles_wo_beep > CYCLE_WO_BEEP_THRESHOLD.
 * Then this VAR then decrease at each cycle;
 * as long as VAR recovery_mode_wait_before_turn > 0 -> no turn back movement
 * while decrease of beep count below alpha;
 * 
 * About the value to chose for WAIT_BEFORE_TUNR_AFTER_RECOVERY: 
 * count 2 + the number to wait
 * Indeed : 
 * - when a lost robot hears again some robot 
 * -> recovery_mode_wait_before_turn decreases from 1 
 * (it takes already 1 cycle to recognize)
 * # then do not hear again -> again decreases from 1
 * if WAIT_BEFORE_TURN_AFTER_RECOVERY is 2 -> then it turns out : normal behavior
*/ 



/* ******************** Global variables ************************************ */

/* carefull!!! for readibility : I used just "t" for a global variable
But do not use t for another usage */
static int t = 0; // internal time in [0;T]

static char alpha_buffer[100]; // alpha_buffer used for uart comm - 

static bool beep_detected = false;
static bool prev_beep_detected = false;
static bool prev_prev_beep_detected = false;

static int beep_offset = 50;
static int previous_total_count = 10;
/* 10 arbitrary but: goal: for the very first run of the algo
 * if total_counts < alpha -> 180 turn to keep the original coherence */
static int total_count = 0; // -1 as each robot detects its own beep
static int alpha = 0;


static int current_behavior = 0;
    // #define BEEPS_EQUAL_BEHAVIOR 0 // see above


// volume calibration variables -- defined in utility_mod3.c/h
extern int left_volume_offset;
extern int right_volume_offset;
extern int back_volume_offset; 

extern bool volume_calibration_done;



/* #################### Functions LIST ###################################### */


// settings
void set_alpha(int alpha_var);
static void set_alpha_algo_offset();
void set_MIN_volume_threshold(int min_vol);
int get_MIN_volume_threshold();


static int beeps_check(); 
/* updates the global variable:
 *      beep_detected;
 *      prev_beep_detected; and
 *      prev_prev_beep_detected = false;
 * 
 * the int return is the volume of beep_detected
 * it is used for the scope visualizer only */
 

// utilities
static void wait_jr_with_beeps_check(int x);
static void status_print();
static void total_count_display();


// behaviors
static void beep_emit();
static void stop_behavior(); 
static void alpha_direction_turn_behavior(int turn_degree_angle); 
    /* used 
     *  - for the first turn of run_alpha_algo => random angle initial direction
     *  - in scope_visualizer when robots turn on themselves. */
static void normal_behavior();
static void increase_behavior();
static void decrease_below_alpha_behavior();


// states
static void cycle_start();
static void ordinary_step(); // key function of the algorithm
static void very_first_run(int alpha);


// core functions
void alpha_algo_init();
void run_alpha_algo(int alpha);
void run_alpha_algo_from_to(int max_alpha, int min_alpha, int cycle_number);


// tool scope-visualizer
void scope_visualizer(bool move_on_spot, bool degrees_display);
void run_scope_visualizer_demo(int min_start, int min_end, int increment);



/* ########################################################################## */



/* ********* SETTINGS functions ********************************************* */

void set_alpha(int alpha_var){
    alpha = alpha_var;        
}

static void set_alpha_algo_offset() {
    beep_offset = jr_random(T*TIC);     
    // the random value should be between 0 and T*TIC (cycle_length)
    // beep_offset = (beep_offset * TIC_BETWEEN_BEEPS) % T;    
    if (VERBOSE) {
        sprintf(alpha_buffer, "[Very first cycle offset] %d \r\n", beep_offset);
        btcomSendString(alpha_buffer);        
    }    
}

void set_MIN_volume_threshold(int min_vol){ // TODO test with HWRev1.1.
    min_volume_threshold = min_vol; 
    if (isEpuckVersion1_3() == false) {
        min_volume_threshold *= (100+HWREV1_1_MICRO_SENSITIVITY_DIFFERENCE);
        min_volume_threshold /= 100;
    }
}

int get_MIN_volume_threshold(){
    return min_volume_threshold;
}



/* ********** Sound (fft and volume) Recognition **************************** */

/* Update the global variable : bool beep_detected
 * depending on :
 * - right frequency recognized
 * - louder enough : above the volume   min_volume_threshold  */
static int beeps_check(){     
    
//    int volume = 0;	
	int frequency;    
    int i;      
    
    int vol_right_mic       = 0; 
    int vol_left_mic        = 0; 
    int vol_back_mic        = 0;
    int vol_average         = 0;
          
    beep_detected = false;    

    
    // ********** [I.] BEEP detection 1st stage: right FREQUENCY ***************
    /* fft recognition : not absolutely necessary -> but good to allow 
     * extension of the algo to beta-algo */
    /* if power issue ->  comment this 2nd stage*/
    
    frequency = fft_get_frequency();     
    /* reminder : fft_get_frequency 
     * -> get the frequency between 
     *      - a min and 
     *      - a max volume 
     *             
     * by default : static int vol_freq_MIN_thresh = 0; 
                    static int vol_freq_MAX_thresh = 1000;*/
    
//     *** if some frequency test is needed : uncomment
        if (VERBOSE) {
            if (frequency >= 5) {
                sprintf(alpha_buffer, "frequency: %d\r\n", frequency);
                btcomSendString(alpha_buffer);
            }
        }
    
    vol_right_mic = fft_get_frequency_volume(false) 
                    - right_volume_offset;
    /* very important to get the volume of the right mic here and in this way 
     * and NOT below with the other mics not used for fft.
     * Indeed, if right_mic_vol is taken below -> very strange value 
     * -> generated by the use of fft : 
     * suddenly right volume at 1200 whereas it should be around 400 
     * 
     * I don't know the exact reason of this behavior. But observed. */       
    /* note about fft_get_frequency_volume(bool corrected_value_for_hwrev1_1)
     * => I take false : I don't want the value returned to be corrected 
     * for HWRev 1.1 here
     * Indeed: 
     * (1) I want to make the average between vol_right_mic and left and back first
     * (2) But the left and the back values are not corrected
     * (3) while the min_threshold
     * 
     */

    for (i = 0; i<BEEP_FREQU_NB; i++){
        if (frequency == beep_frequ[i]) {
            beep_detected = true;
        }
    }   
    
    
    // ********** [II.] BEEP detection 2nd stage: VOLUME (on the 3 mics) ********
    /* important to start here 
     * if we want to do at the same time : fft and volume
     * => strange value for right_mic */
    
    /* Volume calibration must have been done in the main.c*/
    
    if (VERBOSE) {
        if (volume_calibration_done == false)
            btcomSendString(RED "the volume calibration has not been previously done!!! \r\n" COL_RESET);
    }
    
    if (beep_detected) {    

        vol_left_mic    = e_get_micro_volume(LEFT_MIC) - left_volume_offset;
        // vol_right_mic   = e_get_micro_volume(RIGHT_MIC);
        // important to take vol_right_mic above and NOT here - see note above (strange values))
        vol_back_mic    = e_get_micro_volume(BACK_MIC) - back_volume_offset;
        // volume on a sample!!! MIC_SAMP_NB

        vol_average += vol_left_mic + vol_right_mic + vol_back_mic;
        vol_average /= 3.;        

        if (vol_average >= min_volume_threshold) {
            beep_detected = true;
        } else {
            beep_detected = false; 
            // if not loud enough, although beep_detected at fft level, not detected
        }
        
        if (VERBOSE) {
//                sprintf(alpha_buffer, 
//                        "[mic volume] left:%d, right:%d, back:%d || average:%d\r\n",
//                        vol_left_mic, vol_right_mic, vol_back_mic,
//                        vol_average);
            // CSV version
            sprintf(alpha_buffer, 
                    "%d, %d, %d, %d\r\n",
                    vol_left_mic, vol_right_mic, vol_back_mic,
                    vol_average);
            btcomSendString(alpha_buffer);                                
            }
        
    }
                        
    
    if (beep_detected == true) 
        return vol_average ; // -> for degree scope visualizer
    else
        return -1;
    
}



/* ******************** TOOLS - Scope-Visualizers *************************** */

/* sv for Scope Visualizer*/
const int sv_waiting_time = 20; 
    /* [ms] to have some latency in the display
     * used in jr_wait() */

int sv_total_time = 0; // incremented by sv_waiting_time
/* Two uses: 
 *  - display latency: to have the time to see the lit leds
 *  - to know when the robots must turn on itself */

const int turning_angle = 45;
const int total_turns_number = 360/45; 
        // nb of partial turns to get a full turn

int turns_number = -1; // 45? turn (not a full turn!!!)
/* turns_number: used in run_scope_visualizer_demo
 * turns_number++ when a robot turn on itself */


/* min_volume_threshold (global) set via 
        set_MIN_volume_threshold(int min_vol)         
*/ 
void set_MIN_volume_threshold(int min_vol); // defined above

void scope_visualizer(bool move_on_spot, bool degrees_display){
    const int inc = 10; // volume increment between two led if degrees
    int sv_volume = 0;    
    int i;                
    
    const int beep_time = 500; // [ms] time between two emitted beep
    const int turning_time = 4*beep_time; 
        // nb of beeps after which the robot turn of 45? on itself.]
    int sv_vol_freq_MIN_thresh;        
    
    
     /* if one of the robots is surrounded by a cardboard/paper circle 
      * -> it emits sound => enables to get one function for two behaviors */
    if (surrounded()) {                  
        beep_emit();
        wait_jr(beep_time);        
    }
    
    else {
    
        sv_volume = beeps_check(); 
            /* volume value if volume > min_volume_threshold
             * -1 otherwise */
        sv_vol_freq_MIN_thresh = get_MIN_volume_threshold();

        // display clearing after some latency
        if(sv_total_time % (5*sv_waiting_time) == 0) { // to have some latency in the display
            e_led_clear(); 
            e_set_body_led(0);
        }     
       
        // the robot should turn on itselv
        if (sv_total_time > turning_time){
            sv_total_time %= turning_time;            
            if (move_on_spot)
                direction_turn_behavior(turning_angle);
            turns_number++; // used in run_scope_visualizer_demo()            
        }                    
        
        if (VERBOSE) {
            if (sv_volume > -1) {
                sprintf(alpha_buffer, "sv_volume = %d\r\n", sv_volume);
                btcomSendString(alpha_buffer);
            }
        }                    
                
        switch (degrees_display) {        

            case false: // degrees_display_off                            
                if (sv_volume > -1) 
                    e_set_led(8,1);   
                break;

            case true: // degrees_display_on            
                if (sv_volume > -1) {
                    e_set_body_led(1);
                    for (i = 0; i < 8; i++){                        
                        if (sv_volume >= sv_vol_freq_MIN_thresh + i*inc) 
                            e_set_led(i,1);
                    }      
                }
                break;
        
        } // end of switch
        
    wait_jr(sv_waiting_time);
    
    sv_total_time += sv_waiting_time;
    
    } // end of else
        
}


static int run_scope_vizalizer_demo_i = 0;
/* number of full (360) turns
 */
/* EXTERNALIZED LOOPS!!!
 * to enable stopping with remote_control any time
 * the loops for i and turns_number are "externalized"
 */ 


void run_scope_visualizer_demo(int min_start, int min_end, int increment){
    /*  min_start: minimum volume threshold at start
     *  min_end :    "      "       "       at the end
     */

    int increment_number;   
    
    if (increment == 0 || 
            (min_end - min_start) == 0) { // extreme cases
        set_MIN_volume_threshold(min_start);
        scope_visualizer(true, false); 
    } 
    
    else {
    
        if (increment < 0)          // increment = |increment|
            increment *= -1;        
        increment_number = (min_end - min_start)/increment;

        if (increment_number  < 0)  // increment_number = |increment_number|
            increment_number *= -1;

        if((min_end - min_start)%increment != 0) 
            increment_number++;
        /* arbitrary decision: if the difference between min_start
         * and min_end is not a multiple of increment
         * we go till the next multiple of increment after min_end */
        
        if (min_end < min_start)
            increment *= -1;

        // very first passage in the function
        // turns_number has been initialized at -1
        if (turns_number == -1) {
            set_MIN_volume_threshold(min_start);        
            if (VERBOSE) {
                sprintf(alpha_buffer, "min_volume_threshold %d, increment %d \r\n", get_MIN_volume_threshold(), increment);
                btcomSendString(alpha_buffer);
            }
            turns_number = 0;
        }

        scope_visualizer(true, false);
        // turns_number++ in scope_visualizer

        /* total_turns_number: 360/45 -> nb of rotation on itself before changing 
         * min_volume_threshold value
         */
        if (turns_number >= total_turns_number) {             
            run_scope_vizalizer_demo_i++;

            if(increment < 0) {                
                freq_beep_down(0.5);                
            } else {
                freq_beep_up(0.5);                            
            }

            if (run_scope_vizalizer_demo_i == increment_number+1) {
                run_scope_vizalizer_demo_i = 0; 
                    /* implementation decision : 
                     * once the upper limit min_end is reached we start again at min_start */
                e_set_body_led(1);
                freq_beep1_3072(0.5);
                freq_beep2_2816(0.5);
                freq_beep1_3072(0.5);                
                e_set_body_led(0);
            }

            set_MIN_volume_threshold(min_start+
                    increment*run_scope_vizalizer_demo_i);        

            if (VERBOSE) {
                sprintf(alpha_buffer, "vol_freq_MIN_thresh %d, increment %d \r\n", get_MIN_volume_threshold(), increment);
                btcomSendString(alpha_buffer);
            }

            turns_number = 0;
        }
        
    } // end of big else

}



/* ******************** UTILITIES ******************************************* */

#define DELAY1 3
#define DELAY2 64
// makes roughly 1ms together

/* global variable defined at the top 
 *      static bool beep_detected = false;
 *      static bool prev_beep_detected = false;
 *      static bool prev_prev_beep_detected = false;
 */


static void wait_jr_with_beeps_check(int x) {    
    int k ;  

    prev_prev_beep_detected = prev_beep_detected;
    prev_beep_detected = beep_detected;
        
    for (k = 0; k < x; k++) {                                          
        beeps_check(); // update the global var beep_detected        
        wait(TIME_FOR_PROX); // to let the prox work enough
    }    
    
    if(VERBOSE) {
        sprintf(alpha_buffer, "beep_detected: %d \tprev_beep_detected: %d \r\n",
                beep_detected, prev_beep_detected);
        btcomSendString(alpha_buffer);
    }
    
//    e_set_body_led(beep_detected);     
        // finally I keep the green-light for beep emission
        // as total_count_display show the beep received    
    /* if the frequency is detected : 
     * - robot's body in green
     * - otherwise: switched off  */
    
    // need of the two prev_ prev_prev_ sometimes just a jump and
    // recognized two many times even with prev_
    if(beep_detected
            && prev_beep_detected == false
                && prev_prev_beep_detected == false) 
    {
        total_count++;
        if(VERBOSE) {
            sprintf(alpha_buffer, 
                    CYN "Count Frequ Recognized. Total: %d \r\n" COL_RESET,
                    total_count);
            btcomSendString(alpha_buffer);                            
        }

    }
    
}


static void status_print(){
    if(VERBOSE) {
        sprintf(alpha_buffer, "t: %d \t beep_offset: %d\t current_behav: %d \r\n", 
                t, beep_offset, current_behavior);
        btcomSendString(alpha_buffer);
//    sprintf(alpha_buffer, "TMR1: %d \tTMR2: %d \tTMR3: %d \tTMR4: %d \tTMR5: %d \r\n", 
//            TMR1, TMR2, TMR3, TMR4, TMR5);
//    btcomSendString(alpha_buffer);
    }
}


static void total_count_display(){ 
    int i;
    for (i = 1; i <= 8; i++) {
        if (total_count >= i)
            e_set_led(i-1,1);
        else
            e_set_led(i-1,0);
    }    
}



/* ******************** BEHAVIORS ******************************************* */

static void beep_emit() {
        
      /* difference of volume if HWRev1.1 or HWRev 1.3
       * made once for all in first_run
       */
    
    e_set_body_led(1);
    freq_beep1_3072_wo_light(0.5);    
    e_set_body_led(0);
        
    if(VERBOSE) {
        sprintf(alpha_buffer, MAG "########### COUNT BEEP! ########## \r\n" COL_RESET);    
        btcomSendString(alpha_buffer);                
    }
    
    wait_jr(LATENCY_AFTER_BEEP); // ms    
    // slight latency to avoid self_recognition          
    
}


static void stop_behavior() {
    e_set_speed_left(0);
    e_set_speed_right(0);
}  



/* [!!!]Important note : we can't simply use 
 *      function: direction_turn_behavior();
 *      files: runbraitenberg_mod3.c/h
 * for synchronicity issue :
 *      - needs of t++ during the turn
 *      - needs of beeps_emit during the turn depending of the offset
 */

/* basis for direction turn behavior : demoEPuck (Besuchet) runlocatesound.c */
// original TURN_SPEED 1000 -> but powersave change at 999
// + surge and reset
//#define TURN_SPEED 800
#define STEPS_FOR_2PI 1300
// initial value 1300 too-much by experiences
// clockwise

static void alpha_direction_turn_behavior(int turn_degree_angle) { 
    
    if (VERBOSE) {
        sprintf(alpha_buffer, "> enter direction_turn_behavior with direction %d \r\n", turn_degree_angle);
        btcomSendString(alpha_buffer);
    }
    
    int steps = 0;    
    int end_turn;    
    
    end_turn = (int)(STEPS_FOR_2PI*(turn_degree_angle/360.0));
    
    if (turn_degree_angle == 0) ;
    
    else if (turn_degree_angle > 0) {
        e_set_steps_left(0); //Set the number of left motor steps            
        e_set_speed_left(TURN_SPEED);
        e_set_speed_right(-TURN_SPEED);        
        if (VERBOSE) {
            sprintf(alpha_buffer, "end_turn = %d \r\n", end_turn);
            btcomSendString(alpha_buffer);
        }
        while(steps < end_turn) {  // turn until done         
            steps = e_get_steps_left();
            if (VERBOSE) {
                sprintf(alpha_buffer, "e_get_steps_left %d \r\n", steps);
                btcomSendString(alpha_buffer);
            }        
            /* part added for alpha_algo sychro */
            if (t == beep_offset) 
                beep_emit();
            t++;
            wait_jr_with_beeps_check(TIC);
            /* end part added for alpha_algo synchro */
        }
    } else { // if turn_degree_angle < 0
        e_set_steps_right(0);
        e_set_speed_left(-TURN_SPEED);
        e_set_speed_right(TURN_SPEED);
        end_turn = -end_turn; 
        if (VERBOSE) {
            sprintf(alpha_buffer, "end_turn = %d \r\n", end_turn);
            btcomSendString(alpha_buffer);
        }         
        while(steps < end_turn){
            steps = e_get_steps_right();
            if (VERBOSE) {
                sprintf(alpha_buffer, "e_get_steps_right %d \r\n", steps);
                btcomSendString(alpha_buffer);
            }
            /* part added for alpha_algo sychro */
            if (t == beep_offset) 
                beep_emit();
            t++;
            wait_jr_with_beeps_check(TIC);
            /* end part added for alpha_algo synchro */
        }
    }
    
    stop_behavior();
    if (VERBOSE)
        btcomSendString("< end direction_turn_behavior \r\n");        
}


/* when Total number of beeps heard equal to the previous cycle
 * or inferior but above or equal alpha threshold
 * */
static void normal_behavior() {      
    e_restart_agenda(alpha_explorer_neuron);   
//    if (!(MOVEMENT)) 
//        wait_jr(NO_MOVEMENT_WAITING_TIME);
    /* not necessary : MOVEMENT = OFF -> put speed at 0*/
}


//  at the start of a new cycle
static void increase_behavior() {       
    int angle;    
    e_pause_agenda(alpha_explorer_neuron);         
    angle = jr_random(2*RANDOM_ANGLE)-RANDOM_ANGLE; 
    
    if (MOVEMENT) {        
        alpha_direction_turn_behavior(angle);
    } else 
        wait_jr_with_beeps_check(NO_MOVEMENT_WAITING_TICs);
    
    e_restart_agenda(alpha_explorer_neuron);     
}


/* happens at the start of the ordinary step if 
 * at the end of the previous cycle, not enough beeps
 * So we have to keep counting the t is we want to keep the offset
 * This is very roughly done.
 * */
static void decrease_below_alpha_behavior() {

    e_pause_agenda(alpha_explorer_neuron); 
    
    if (MOVEMENT) {
        if(VERBOSE)
            btcomSendString(RED "I turn back!!!\r\n" COL_RESET);
        alpha_direction_turn_behavior(180);
    }else 
        wait_jr_with_beeps_check(NO_MOVEMENT_WAITING_TICs);
    
    e_restart_agenda(alpha_explorer_neuron);     
}



/* ******************** STATES ********************************************** */

// REVOVERY MODE variableS
static unsigned int cycles_wo_beep = 0;
static unsigned int recovery_mode_wait_before_turn = 0; 
/* # of cycle before turning again when connection loss below the threshold
 * more detail about recovery mode see at above
 * */

// cycle_start
static void cycle_start() {        
    t = 0;
    total_count = 0;     
    
    if (VERBOSE) {
        sprintf(alpha_buffer, GRN "[Cycle Start]\r\n" COL_RESET);
        btcomSendString(alpha_buffer);
        status_print();
    }                 
    
    e_led_clear();
    
    wait_jr_with_beeps_check(TIC);
    
    ordinary_step();
    
 }

/* **************************************************************************
 * ****** The KEY PART of the algo ******************************************
 * ************************************************************************** */
static void ordinary_step() {   
    int delta; 
        // current_total_count - previous_total_count
    total_count_display(); 
        // switch on the number of led according to the beeps heard during the last cycle    
    
    /* Note about the global VAR current_behavior : 
     * during the cycle -> the behavior is always BEEPS_EQUAL_BEHAVIOR
     * as there can be no loss or gain (the total cannot be known during the cycle)
     * The other behaviors happen just BETWEEN two cycles: 
     * - set at the end of a cycle, when t == T
     * - "played" at the start of a cycle
     */
                
    switch(current_behavior) {
        case BEEPS_EQUAL_BEHAVIOR :            
            normal_behavior();
            break;
        case BEEPS_INCREASE_BEHAVIOR :
            increase_behavior();
            break;
        case BEEPS_DECREASE_ABOVE_ALPHA_BEHAVIOR :
            normal_behavior();            
            break;
        case BEEPS_DECREASE_BELOW_ALPHA_BEHAVIOR:
            if (VERBOSE || RECOVERY_VERBOSE) {
                sprintf(alpha_buffer, YEL "Beeps Decrease Below: recovery mode = %d \r\n" COL_RESET,
                        recovery_mode_wait_before_turn);
                btcomSendString(alpha_buffer);
            }                
            if (recovery_mode_wait_before_turn == 0) {                
                decrease_below_alpha_behavior();
            } else {                
                normal_behavior();                
                recovery_mode_wait_before_turn--;                 
            }
            break;
        default: ;
    }      
    
    current_behavior = BEEPS_EQUAL_BEHAVIOR; 
    // see note above : during cycle -> always BEEPS_EQUAL_BEHAVIOR
            
    if (t == beep_offset) {        
        beep_emit();
    }
    
    if (t < T) {    
        t++;                
        status_print();    
        wait_jr_with_beeps_check(TIC);
        ordinary_step();
    
        
    } else {  //if (t >= T) { // END of CYCLE
        
        delta = total_count - previous_total_count;
        
        if (VERBOSE || RECOVERY_VERBOSE) {
            sprintf(alpha_buffer, 
                    CYN "Total beeps recognized during this Cycle: %d \r\n" COL_RESET,
                    total_count);
            btcomSendString(alpha_buffer);   
            sprintf(alpha_buffer, "difference with previous cycle: %d \r\n", delta);
            btcomSendString(alpha_buffer);   
        }               
        
        if (delta == 0) {
            current_behavior = BEEPS_EQUAL_BEHAVIOR;
        }  else if (delta > 0) {
            current_behavior = BEEPS_INCREASE_BEHAVIOR;
            
        } else /* if (delta < 0) */ { /* if delta < 0 -> loss of connection */
            if (total_count >= alpha) {
                current_behavior = BEEPS_DECREASE_ABOVE_ALPHA_BEHAVIOR;
            } else /* if(total_count < alpha */ {
                current_behavior = BEEPS_DECREASE_BELOW_ALPHA_BEHAVIOR;
            }
        }   
        
        if (VERBOSE || RECOVERY_VERBOSE) {
           sprintf(alpha_buffer, 
                    WHT "Current behavior %d\r\n" COL_RESET,
                    current_behavior);
            btcomSendString(alpha_buffer);   
        }
        
        /* recovery mode management */
        
        if (total_count == 0)  { // the robot didn't hear anything during the last cycle
            cycles_wo_beep++;
        }  else
            cycles_wo_beep = 0;
                
        if (cycles_wo_beep >= CYCLE_WO_BEEP_THRESHOLD) {
            /* robot not hearing any beep for several cycles */                                                                
            recovery_mode_wait_before_turn = WAIT_BEFORE_TURN_AFTER_RECOVERY;
            // number of cycles during which it doesn't turn if connection loss
            // first initialization
        } else if (recovery_mode_wait_before_turn > 0) {
            /* robot was in recovery mode and during some number of cycles doesn't
             turn back when decrease below threshold*/
            recovery_mode_wait_before_turn--; 
        }
        
        if (VERBOSE || RECOVERY_VERBOSE) {
           sprintf(alpha_buffer, 
                    "Cycle wo beeps %d, wait_before_turn %d\r\n" COL_RESET,
                    cycles_wo_beep, 
                    recovery_mode_wait_before_turn);
            btcomSendString(alpha_buffer);   
        }                       
        
        previous_total_count = total_count;
        
    }  // stops here and the main while start anew
}
    


/* **************** CORE FUNCTIONs ****************************************** */


/* current_behavior is a global variable 
 * static int current_behavior */

/* ***** BEHAVIORS reminder ***** 
 * (defined at the start of the file))
 
 * the 4 possible behaviors depending on the loss of connection and 
 * total_count above or below alpha
 * 
 * #define BEEPS_EQUAL_BEHAVIOR 0
 *      # of beeps equal between two cycles
 * #define BEEPS_INCREASE_BEHAVIOR 1
 * #define BEEPS_DECREASE_ABOVE_ALPHA_BEHAVIOR 2
 * #define BEEPS_DECREASE_BELOW_ALPHA_BEHAVIOR 3 */


static bool first_run = true;

void alpha_algo_init(){
    first_run = true;
}


static void very_first_run(int alpha) {
    int angle;
    int delta;
    
    
    // before in alpha_algo
    if (VERBOSE) {   
        btcomSendString(GRN "\r\nrun_alpha_algo - Start\r\n");
        btcomSendString("--------------------\r\n" COL_RESET);
    }

    if (AUDIO_ON == 0) 
        e_init_sound();

    if (MOVEMENT)
        set_EL_basic_speed(BASIC_SPEED); 
    else
        set_EL_basic_speed(0); 
        /* For test it can be useful to make the e-puck remain static
         * the macro MOVEMENT enables it */                        
    
    // recovery mode var
    cycles_wo_beep = 0;
    recovery_mode_wait_before_turn = 0;
    
    set_alpha_algo_offset();
    set_alpha(alpha); // alpha-threshold
    set_MIN_volume_threshold(min_volume_threshold);    
        
    if (isEpuckVersion1_3())
        set_volume(BEEP_EMIT_HWRev1_3);
    else
        set_volume(BEEP_EMIT_HWrev1_1);  

    if (MOVEMENT) {    
        /* random position at start
        * the delta is used for each robot to use roughly the same time
        * for this small stage */
        angle = jr_random(360); // angle in [0;+360]]
        delta = 360-angle;
        delta /= 2;    
        direction_turn_behavior(angle + delta);
        direction_turn_behavior(-delta);
    }
    
    e_activate_agenda(alpha_explorer_neuron, 200);
    /* ### agenda cycle value : 200 [millisec/10] ###
     * I had to try several value to find the right one
     * to prevent unexpected and untimely resets
     * It seems that here was a conflict between 
     * - the alpha_explorer_neuron and
     * - the agenda from the  remote control
     * But less and less convinced : I tried to disable the remote control
     * module by using the manual selector and the reset behavior is still there.
     * Values tested: 650 (the one I usually use), 100, 1000 (too long) 
     */
    /* alpha_explorer_neuron vs explorer_neuron
     * a bit less sensitive in order for the robots not to go 
     * too far away from each other */
    
    first_run = false;
            
}



// note/reminder! the loop is "externally" made in the main loop

void run_alpha_algo(int alpha){
    if (first_run){
        very_first_run(alpha);
//        first_run = false;
        /* do not already turn first_run to false here as we use it also
         * in alpha_algo(...) 
         * it will be turned to false there */        
    }
    cycle_start();
}



// changes of the GOAL_COUNT_BEEP when one cycle over
#define BEEP_GOAL_DECREASE -1
#define BEEP_GOAL_INCREASE 1
#define BEEP_GOAL_STATIONARY 0

int cycle_counter = 0;
int current_alpha;
int change_direction = BEEP_GOAL_DECREASE;

void run_alpha_algo_from_to(int max_alpha, int min_alpha, int cycle_number){
    if (first_run){
        cycle_counter = 0;
        current_alpha = max_alpha;
        change_direction = BEEP_GOAL_DECREASE;
    }
    
    set_alpha(current_alpha); // alpha-threshold
    run_alpha_algo(current_alpha);
    cycle_counter++;
    
    if (cycle_counter == cycle_number){
        
        switch (change_direction) {
            
            case BEEP_GOAL_STATIONARY:
                                    ; // do_nothing
                                    break;
                                    
            case BEEP_GOAL_DECREASE: 
                                if (current_alpha == min_alpha) {
                                    current_alpha++;
                                    freq_beep_up(0.5);                                    
                                    change_direction = BEEP_GOAL_INCREASE;
                                } else {
                                    current_alpha--;                                    
                                    freq_beep_down(0.5);                                    
                                }
                                break;
            
            case BEEP_GOAL_INCREASE: 
                                if (current_alpha == max_alpha){
                                    current_alpha--;
                                    freq_beep_down(0.5);                                    
                                    change_direction = BEEP_GOAL_DECREASE;
                                } else {
                                    current_alpha++;
                                    freq_beep_up(0.5);                                    
                                }
                                break;
            
            default: ;           
        } //end switch    
        wait_jr(LATENCY_AFTER_BEEP); // again for power issue / avoiding energy jump
        cycle_counter = 0;
    }    
}

