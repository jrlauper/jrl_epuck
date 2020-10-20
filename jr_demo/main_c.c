/* *****************************************************************************
 * Robots Collective Behavior Demos
 *  using Embedded Programming with E-Pucks vers.1 (dsPIC) and Sound
 * 
 * Jean-Roch LAUPER, unifr.ch, 2019/2020
 * BSc Thesis main Demo Set
 * 
 * ****************************************************************************/

/* Demo Videos -> see directly in the header.h files 
 * containing the corresponding programs */



/* ########## LIBRARIES ##################################################### */

// C general libraries
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// e-puck original libraries
#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_motors.h"
#include "e_led.h"
#include "e_uart_char.h"
#include "e_ad_conv.h"
#include "e_prox.h"
#include "e_agenda.h"
#include "e_sound.h"
#include "e_remote_control.h"
#include "e_micro.h"

#include "btcom.h"

// e-puck additional general libraries (JRL))
#include "utility_mod3.h"
#include "remote_control_utility.h"
#include "e_freq_sound.h"
#include "e_freq_recognition.h"
//#include "e_wav_music2.h"


// Programs headers
#include "runbraitenberg_mod3.h"
#include "run_chirping.h"
#include "run_alpha_algo.h"
#include "run_simon.h"

#include "runlocatesound.h" 
#include "runfftlistener.h"


/* ########## end libraries ################################################# */


//#define VERBOSE (true)
#define VERBOSE (false)
// activate/deactivate BT Uart messages - useful for debugging

#define REMOTE_SELECTOR 14 
/* the value of the selector corresponding to the usage of the remote_control 
 * -> if the robot is on 14 -> remote control work 
 * I keep the 15 for battery test on HWRev 1.3 */

extern int left_volume_offset;
extern int right_volume_offset;
extern int back_volume_offset;
// defined in utility_mod3



/* ########################### MAIN ######################################### */


int main() {
    
    char main_buffer[100];    
     
    int vol_right_mic       = 0; 
    int vol_left_mic        = 0; 
    int vol_back_mic        = 0;
    int vol_average         = 0;
    
  
    /* ********** E-puck Various Initializations ******************************** */        
    
    //Reset if Power on (some problem for few robots)
	if (RCONbits.POR) {
		RCONbits.POR=0;
		__asm__ volatile ("reset");
	}
    
    e_init_port();    			// configure port pins
	e_init_ad_scan(ALL_ADC);	// configure Analog-to-Digital Converter Module    
    if (VERBOSE) 
        e_init_uart1();				// initialize UART to 115200 kbauds                               
    init_run_remote();          // remote-control initialization

        
    // ---------- Robot Start ----------
    
    if (VERBOSE) {
        // to test if no more automatic reset... 
        btcomSendString(GRN "..........\r\n"); // GRN
        btcomSendString("########## RESET ##########\r\n");
        btcomSendString("########## main_c.c ##########\r\n");
        btcomSendString("########## main_c.c ##########\r\n");
        btcomSendString(COL_RESET); //COL_RESET        
    }
    
    
    // ---------- IR Calibration (all LED light up) ----------
    // !!! be careful no to have any object close to robot sensors during this stage
    
    if(VERBOSE) btcomSendString(MAG "IR_Calibration start...\r\n" COL_RESET);    
    e_set_led(8,1); // 8 --> all the lights on
    wait_jr(100);                
    e_calibrate_ir();            
    if(VERBOSE) btcomSendString(MAG "...IR_Calibration done\r\n" COL_RESET); 
    e_led_clear();
    
    
    // ---------- Micros Volume Calibration (GREEN body light) ----------    
    
    if(VERBOSE) btcomSendString(CYN "Micros Volume Calibration start...\r\n" COL_RESET);           
    volume_calibration(); // includes body_led on/off    
    if(VERBOSE) btcomSendString(CYN "Micros Volume Calibration done\r\n" COL_RESET); 
        
    
    // ---------- (beep) Battery level (beepbeep) ----------       
    
    wait_jr(100);
    freq_beep1_3072_wo_light(0.5);
    battery_display();
    wait_jr(500);
    freq_beep1_3072_wo_light(0.5);
    wait_jr(100);
    freq_beep1_3072_wo_light(0.5);
    e_led_clear();
    // beep beep
    
    
    e_init_motors();    
    e_start_agendas_processing(); 
    
    // * ********** End of E-puck Initialization ****************************** */



    
    bool main_first_run = true;
    int alpha_selector = -1; // concerns alpha-algo only, otherwise stays at -1 
    /* the two vars are used with alpha algorithm (swarm coherency) only
     * -> to manage unexpected resets
     * disable the remote control after launching alpha-algo
     * -> the remote-control is again available after reset 
     * -> enables to relaunch reseted robots */
        
    
    int selector = 0;           
    
    // Main loop                             
    while(1) {      
        selector = getselector();
    
        if (selector == REMOTE_SELECTOR){ 
            if(alpha_selector == -1)
                selector = run_remote(); 
                /* run_remote 
                 * - returns current_choice when switch_on = true (program running)
                 * - returns -1 when switch_on = false 
                 *      then wheels are stopped and
                 *      choices change is possible with remote control */
            else
                selector = alpha_selector;        
        }
        
        if (selector != -1) { 
            // run_remote return -1 when selection changes is possible
            // if != -1 -> program can run

            if(selector == 15) {
                battery_display();
                wait_jr(500);

            } else {                 
                
            switch (selector) {
  
                case 0: // coherency alpha-algo with automatic threshold changes                                                                                
                    run_alpha_algo_from_to(6,1,50);
                    alpha_selector = selector;
                    break;
                
                case 1: // coherency alpha-algo with fixed threshold of 5                    
                    run_alpha_algo(5);
                    alpha_selector = selector;
                    break;
                    
                case 2: // coherency alpha-algo with fixed threshold of 3
                    run_alpha_algo(3);
                    alpha_selector = selector;
                    break;
                    
                case 3: // coherency alpha-algo with fixed threshold of 1
                    run_alpha_algo(1);
                    alpha_selector = selector;
                    break;
                    
                case 4: // collective heap building "Swiss-XP"
                    e_set_led(1,1); e_set_led(7,1); 
                    run_braitenberg_swiss_EL(); 
                    break;
                    
                case 5: // synchronization through wound "Chirping" / Strogatz
                    run_chirping();
                    break;
                    
                case 6: // Braitenberg Explorer
                    e_set_led(1,1); e_set_led(7,1); 
                    run_braitenberg_explorer();
                    break;
                
                case 7: // Braitenberg Explorer/Lover
                    run_braitenberg_explorer_and_lover();
                    break;
                
                case 8: // ir_piano or full sound demo if surrounded by paper
                    if (surrounded())
                        freq_video_full_demo();
                    else
                        freq_ir_piano_C_to_C1();                
                    break;
                
                case 9: // guiding the e-puck by whisling
                    run_fft_listener();
                    break;
                
                case 10: // rotating toward noise/clap
                    run_locatesound();
                    break;

                case 11: // rotating toward light
                    run_light_lover();
                    break;
                
                case 12: // exploring and moving toward light
                    run_moving_light_lover(true);
                    break;
                
                case 13: /* Simon game 
                          * repeating tones with hands (IR sensors) 
                          * or with sound/whisling (fft recognition) */
                    run_simon();
                    break;
                
                /* reminder: 
                 selector 14 -> remote-control working
                 selector 15 -> battery display with HWRev 1.3*/                        
                                    
                
                }  // end switch (selector)
            
            } //end else if if(selector == 15) 
        
       } // end if (selector != -1)
    
    } // main loop while(1) end 
  
}
