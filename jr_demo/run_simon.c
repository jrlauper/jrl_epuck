/* Jean-Roch LAUPER, unifr.ch, 2019/2020
 * 
 * >>> More details in the header file 
 * 
 */


#include "run_simon.h"

#include <stdio.h>
#include <stdbool.h>

#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_ad_conv.h"
#include "e_agenda.h"
#include "e_uart_char.h"
#include "btcom.h"

#include "e_led.h"
#include "e_freq_sound.h"
#include "e_freq_recognition.h"
#include "e_prox.h"

#include "utility_mod3.h"


//#define VERBOSE (true)
#define VERBOSE (false)


/* ***** Skill levels *****
 * 
 * According to : https://www.waitingforfriday.com/?p=586#Sound_frequencies_and_timing
 * "The game also features 4 skill levels:
 *      - Repeat a sequence of 8 colours
 *      - Repeat a sequence of 14 colours
 *      - Repeat a sequence of 20 colours
 *      - Repeat a sequence of 31 colours"
 * By experience, 31 is really hard...
 * 16 is reachable... 
 * So I opted for : 
 * - intermediary goal : 10 : -> Tune: star wars : imperial marc
 * - max goal : 20 : -> Tune: Rocky Movie Theme
 */
#define INTERMEDIARY_GOAL 10
#define MAX_GOAL 20

//for test
//#define INTERMEDIARY_GOAL 4
//#define MAX_GOAL 8


#define ARRAY_SIZE  MAX_GOAL

#define TONE_LENGTH 3.5

#define PROX_TRESHOLD 150
/* empirical tests for Prox_threshold : 
 * // -> take into account for ir-piano too
 * - 400 a bit too much
 * - 300 still */

#define CYCLE_TO_WAIT 120


/* ***** Button Definition */
#define GREEN_BUTTON 1
#define RED_BUTTON 2
#define YELLOW_BUTTON 3
#define BLUE_BUTTON 4


/* ***** Recognized Whistled Frequencies ***** */
/* REMINDER/BE CAREFUL
 * this is the frequency with ALL_ADC
 * they are not the "real" frequency you get with MICRO_ONLY */
static int green_freq[2]   = {3996, 4125}; // emitted 1760 Hz (A6))
static int red_freq[2]     = {3093, 2964}; // emitted 1318.51 Hz (E6)
static int yellow_freq[2]  = {2449, 2320}; // emitted 1046.5 Hz (C6)
static int blue_freq[2]    = {2062, 1933}; // emitted 880 Hz (A5))


int solution[ARRAY_SIZE]; // filled by void solution_init()
static char simon_buffer[100]; // alpha_buffer used for uart comm - 


static void solution_init(){
    int i;
    int temp;
    
    e_activate_agenda(snake_led,650);
    
    for (i = 0; i < ARRAY_SIZE; i++) {
        temp = jr_random(3)+1;        
        solution[i] = temp;          
        if (VERBOSE) {
            sprintf(simon_buffer, "number %d: %d \r\n", i, solution[i]);
            btcomSendString(simon_buffer);
        } else 
            wait_jr(100); 
        /* necessary to add a small delay : otherwise tendency to get too often
         * the same value */
                
    }
    e_destroy_agenda(snake_led);  
    e_led_clear();
}


/* big buttons arrangement on the original SIMON (the big one / not the pocket one)
 * (1) GREEN(A)     |   (2) RED(E)
 * (3) YELLOW(C)    |   (4) BLUE(A-) */
static void button_display (int button) {        
    e_led_clear();
    
    switch(button) {
            
        case 1: // green button
            if (VERBOSE)
                btcomSendString(GRN "1 " COL_RESET);
            e_set_led(7,1);
            freq_a(TONE_LENGTH);            
            break;
    
        case 2: // red button
            if (VERBOSE)
                btcomSendString(RED "2 " COL_RESET);
            e_set_led(1,1);
            freq_e(TONE_LENGTH);            
            break;
        
        case 3: // yellow button
            if (VERBOSE)
                btcomSendString(YEL "3 " COL_RESET);
            e_set_led(5,1);
            freq_c(TONE_LENGTH);            
            break;
        
        case 4: // blue button
            if (VERBOSE)
                btcomSendString(BLU "4 " COL_RESET);
            e_set_led(3,1);
            freq_a0(TONE_LENGTH);
            break;            
        
        default:
            if (VERBOSE)
                btcomSendString ("unexpected case");        
            ;
            
    } //switch end   
    
    e_led_clear();
    wait_jr(100);
}


static void solution_display_till(int last_digit_to_show) {
    int i;
    for (i = 0; i < last_digit_to_show; i++)
        button_display(solution[i]);       
    e_led_clear();
}


/* leds 0 to 7 used for numbers 1 to 8
 * front led used for number 9
 * for tens, robot body lit up at the same time */
static void score_display_rec(int score, int intermediary_wait){
    int i;
    int score_ten = score / 10;
    int score_unit = score % 10;        
    
    // A. Tens
    
    if (score_ten >= 1) {            
        e_led_clear();
        e_set_front_led(0);
        e_set_body_led(1); // for
        wait_jr(500);
        if (intermediary_wait != 0){
            freq_beep_up(1);
        }
        for(i = 1; i < 9; i++){
            if(score_ten>=i) {
                e_set_led(i-1,1);                    
                wait_jr(intermediary_wait);
            }
        }
        if (score_ten == 9) {
            e_set_front_led(1);
            wait_jr(intermediary_wait);
        }        
        
//        if (intermediary_wait == 0) 
            wait_jr(1000);
    
    }
    
    // B. Units
     
    e_led_clear();
    e_set_front_led(0);
    e_set_body_led(0);
    wait_jr(500);

    if (intermediary_wait != 0){
        freq_beep_up(1);
        freq_beep_up(1);
    }
    for(i = 1; i < 9; i++){
        if(score_unit>=i) {
            e_set_led(i-1,1);                    
            wait_jr(intermediary_wait);
        }
    }
    if (score_unit == 9) {
        e_set_front_led(1);
        wait_jr(intermediary_wait);
    }
    
//    if (intermediary_wait == 0) 
        wait_jr(1000);
       
}


static first_score_display = true;

static void score_display(int score) {    
    if (first_score_display) {
        score_display_rec(score, 100); 
        /* intermediary wait = 100 only the very first tine
         * consequences: 
         * 1/ progressive display of the score
         * 2/ small crescendo beeps accompanying only the first time */      
        first_score_display = false;
    } else 
        score_display_rec(score, 0);
}


static void you_loose(int tune_var){
    e_set_led(8,1);
        
    freq_play_from_to(2000,1700,5);
    e_set_led(0,0);
    
    e_set_led(7,0);
    e_set_led(1,0);

    freq_play_from_to(1700,1400, 5);
    e_set_led(6,0);
    e_set_led(2,0);

    freq_play_from_to(1400, 1100, 5);
    e_set_led(5,0);
    e_set_led(3,0);

    freq_play_from_to(1100, 700, 5);
    e_set_led(4,0);
    freq_play(700,5);
    
    wait_jr(500);
    if (tune_var == 0)
        freq_tunes_examples(10); // marche funebre
    else if (tune_var == 1)
        freq_tunes_examples(4); // star_wars
    wait_jr(500);    
}


static int button_check(){
    int button = 0;
    int cycle = 0;            
    int fft_temp = 0;
    int i;

    while(button == 0 && cycle < CYCLE_TO_WAIT) {                
        
        // A. prox detection
        if(e_get_calibrated_prox(6) > PROX_TRESHOLD)
            button = GREEN_BUTTON;
        else if (e_get_calibrated_prox(1) > PROX_TRESHOLD)
            button = RED_BUTTON;
        else if (e_get_calibrated_prox(4) > PROX_TRESHOLD)
            button = YELLOW_BUTTON;
        else if (e_get_calibrated_prox(3) > PROX_TRESHOLD)
            button = BLUE_BUTTON;
        
        // B. fft_detection
        fft_temp = fft_get_frequency();
        for (i=0;i<2;i++){
            if(fft_temp == green_freq[i]) {
                button = GREEN_BUTTON;
                break;
            }else if(fft_temp == red_freq[i]) {
                button = RED_BUTTON;
                break;
            }else if(fft_temp == yellow_freq[i]){
                button = YELLOW_BUTTON;
                break;
            }else if (fft_temp == blue_freq[i]){
                button = BLUE_BUTTON;
                break;
            }            
        }
                
//        wait_jr(10);
        cycle++;
    } 
    return button;
}


// if necessary initializations not already done
void run_simon_init(){                    
    e_init_port();    			// configure port pins
	e_init_ad_scan(ALL_ADC);	// configure Analog-to-Digital Converter Module
    if (VERBOSE)
        e_init_uart1();				// initialize UART to 115200 kbauds                           
        
    if (VERBOSE) {
        // to test if no more automatic reset... 
        btcomSendString(GRN "..........\r\n"); // GRN
        btcomSendString("########## RESET ##########\r\n");
        btcomSendString("########## main_c.c ##########\r\n");
        btcomSendString("########## main_c.c ##########\r\n");
        btcomSendString(COL_RESET); //COL_RESET        
    }
    
    if(VERBOSE) 
        btcomSendString(MAG "IR_Calibration start...\r\n" COL_RESET);
    e_set_led(8,1); wait_jr(100);            
    
    e_calibrate_ir();
    
    if(VERBOSE)
        btcomSendString(MAG "...IR_Calibration done\r\n" COL_RESET);        
    e_led_clear(); wait_jr(100);
    e_set_led(8,1); wait_jr(100);
    e_led_clear();
    
    e_start_agendas_processing();         
}



/* ***** MAIN Function ***** */

static bool first_run = true;

void run_simon(){
/* !!! the necessary initializations should have been done before*/                  
    
    static int i;    // here "static" -> duration: permanent while scope: function
    static int last; // last tone reached
    static int game_over;    
    static int button;    
    
    /* *** very first run initialization *** */    
    if(first_run) {    
        
        i = 0;
        last = 0;
        game_over = false;
        button = 0;
        
        freq_set_music_led(false);
        set_volume(40);
        set_sound_wave_shape(SINUS);
    
        if (VERBOSE){
            btcomSendString("SUPER SIMON \r\n");     
            btcomSendString("***********\r\n");
        }

        solution_init(); 

        // led display
            wait_jr(100);
        e_set_led(8,1);
            wait_jr(300);
        e_led_clear();
            wait_jr(200);
        e_set_led(8,1);
            wait_jr(300);
        e_led_clear();
            wait_jr(200);
        
        first_run = false;
    }
    
    
    /* *** Ordinary Step *** */
        // while(last < ARRAY_SIZE && game_over == false){ 
        /* "while" replaced by an if to enable a main loop use and remote control use */
    if(last < ARRAY_SIZE && game_over == false){    
        last++;                
        wait_jr(300);
      
        solution_display_till(last);         
        wait_jr(250);
      
        if (VERBOSE)
            btcomSendString("Your turn : ");
        
        for (i = 0; i < last; i++) {                        
            button = button_check();                  
            button_display(button); 
            if (button != solution[i]) { // if an error happens             
                set_sound_wave_shape(SQUARE);
                freq_tone_play(3, _D, 10);                 
                set_sound_wave_shape(SINUS);                
                wait_jr(1000);
                game_over = true;                
                break; // exit of the for loop
            }
        }        
    } // if end

    
    /* *** END of the game *** */ 
    // two possible ends
    
    // [end] 1: win : all tones repeated
    else if (game_over == false) { 
        /* last == ARRAY_SIZE --> all the tones have been repeated
         * while game_over == false
         */ 
        if (VERBOSE){
            sprintf(simon_buffer, "you win!!! your score is %d\r\n", ARRAY_SIZE);
            btcomSendString(simon_buffer);
        }
        if (first_score_display) { //  music before the 1st display
            e_activate_agenda(e_blink_led,650);
            freq_tunes_examples(12); // rocky theme        
            e_pause_agenda(e_blink_led);
            // first_score_display = false // => done in score_display_rec()            
        }
        score_display(ARRAY_SIZE);
    
    // end 2: game over
    } else {
        if (VERBOSE) {            
            sprintf(simon_buffer, "you lose. Your final score is %d\r\n", last-1);
            btcomSendString(simon_buffer);
        }
        if (first_score_display) { // music before the 1st display
            if (last-1 >= INTERMEDIARY_GOAL) { // different music if you loose but with a high score
                you_loose(1);
            } else
                you_loose(0);
            // first_score_display = false // => done in score_display_rec()            
        }
        score_display(last-1);                                
    }
        
}
