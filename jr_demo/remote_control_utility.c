/* Jean-Roch LAUPER, unifr.ch, 2019/2020
 * 
 * >>> More details in the header file 
 * 
 */

#include "remote_control_utility.h"

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_agenda.h"
#include "e_uart_char.h"
#include "btcom.h"

#include "e_motors.h"
#include "e_led.h"

#include "e_sound.h"
#include "e_remote_control.h"

// JRL Libraries
#include "e_freq_sound.h"
#include "utility_mod3.h"



#define VERBOSE (false)
//#define VERBOSE (true)

static char remote_buffer[100];


/* [****** Strange Behavior observed *****] -> impact on the code below
 * ("strange" because I don't grasp what causes it)
 *
 * Sometimes but regularly:
 *  
 * When the selection 3 or 4 (respectively 11 or 12) 
 * are selected (leds 3 and 4)
 * 
 * the IR remote control receiver of the e-puck 
 * keeps on reading signal that is not here.
 * 
 * The robot beep... beeps... and doesn't stop and then it resets
 * 
 * The happening of this phenomenon depends on the robot: 
 * with some it happens frequently, with other not.
 * In our lab, the e-puck 3435 is e.g. especially sensitive to this phenomenon
 * 
 * Is the cause some kind of signal noise? defect in the ir_receptor?
 */



static void do_nothing(){
    ;
}

// to save energy - for instance between demos when placing robots
static void go_to_sleep(){ 
    e_close_sound();
    e_led_clear();
    e_set_body_led(0);
    Sleep();;      
    e_set_led(3,1);
    // to check that it remains in Sleep -> otherwise the led swithes on
    wait_jr(1000);
}
/* SLEEP mode note : (from Microchip DSPic documentation) 
 *      "Sleep Mode: (low power options)
 *      Sleep Mode turns off the system clock. (...). 
 *      The CPU and all peripherals requiring the system clock will cease operation. 
 *      The contents of the memory and registers are maintained in sleep mode.
 *      (...)
 *      Sleep Mode will end and the CPU will execute the instruction immediately 
 *      following the PWRSAV instruction upon the following conditions:
 *          - A Reset condition, other than a Power-On Reset.
 *          - A Watchdog Timer overflow 
 *          - Any interrupt request occurs."
 */



/* used for choice display 
 *      see the picture "IR-Remote Control led-selection.png" in this folder 
 *      for the led/choice correspondence */
static void remote_display(int current_choice) {
    e_led_clear();
    
    if (current_choice < 0) {
        current_choice += 16;
    }
    
    if (current_choice >= 0 && current_choice <= 7) {           
        e_set_led(current_choice, 1); 
        e_set_front_led(0);
    } else if (current_choice > 7 && current_choice <= 15) {
        e_set_led(current_choice%8, 1);
        e_set_front_led(1);        
    }
}



/* ***** Global Vars ***** 
 * used in 
 *      - init_run_remote()
 *      - run_remote() and
 *      - ir_remote_code_key_identification()
 */
int ir_check = 0;
int ir_address = 0;
int ir_data = 0;
int previous_check = 0;
int previous_address = 0;
int previous_data = 0;

int current_choice = 0;      
int switch_on = false;



void init_run_remote(){
    e_init_port();
 	e_init_remote_control();
  	e_start_agendas_processing(); 
    if (AUDIO_ON == 0)
        e_init_sound();
    
    ir_check = 0;
    ir_address = 0;
    ir_data = 0;
    previous_check = 0;
    previous_address = 0;
    previous_data = 0;

    current_choice = 0;      
    switch_on = false; // switch_on = true, when a programm is running
    
}

// you need to run init_run_remote() first to use this function
int run_remote() {
                       
    // while (1) {        // placed in the main loop
       
        ir_check = e_get_check();
        ir_address = e_get_address();
        ir_data = e_get_data();           
                        
        if (!(ir_check==previous_check 
                && ir_address == previous_address 
                && ir_data == previous_data)) {
            // basically if keypressed              
            
            /* **** filter **** 
             *      to manage strange behavior explained in comments above ln 38
             *      keeping on only if ir_address included in what we're looking for 
             *      otherwise, we add a little wait time of 0.1 second 
             *      and read again the address and the data without doing anything before */
            if (ir_address == ROBOT_RESET_add || ir_address == CHOICE_RESET_add ||
                    ir_address == CHOICE_RESET2_add || ir_address == SLEEP_add) 
                ; // do_nothing - ok ->  keep on
            else {
                wait_jr(100);// ms
                goto filter_not_ok;
            }
            
            e_set_body_led(1); 
            freq_beep1_3072(0.5);               
            
            // ***** [Robot Reset] *****                       
            if (ir_address == ROBOT_RESET_add && ir_data == ROBOT_RESET_data) {            
                RESET();
                e_set_led(8,1);
            }
            
            // ***** [Robot Sleep] *****           
            else if (ir_address == SLEEP_add && ir_data == SLEEP_data) {
                // Sleep mode
                go_to_sleep();
            }                     
            
            // ***** [Start/Stop Selector Chosen] *****            
            else if (ir_address == START_STOP_add && ir_data == START_STOP_data){            
                wait_jr(100); // to avoid to read twice a button press
                switch_on =!switch_on;
                e_led_clear();
                e_set_front_led(0);
            }
            
            else if (switch_on == false) { // change of selection only when stopped
                wait_jr(100); // to avoid to read twice a button press                
                
                // ***** [Reset Selector Chosen] *****                                
                if ((ir_address == CHOICE_RESET_add &&  ir_data == CHOICE_RESET_data)||
                    (ir_address == CHOICE_RESET2_add &&  ir_data == CHOICE_RESET2_data))
                {
                    current_choice = 0;                                       
                }
                
                // ***** [Selector Chosen ++] *****                
                else if (ir_address == CHOICE_NEXT_add && ir_data == CHOICE_NEXT_data)
                {
                    current_choice++; 
                    if (current_choice == 16) 
                        current_choice = 0;                                        
                }

                // ***** [Selector Chosen --] *****
                else if (ir_address == CHOICE_PREV_add && ir_data == CHOICE_PREV_data) 
                {
                    current_choice--;
                    if (current_choice == -1)
                            current_choice = 15;                            
                }                                                
            } // end of else if (switch_on == false)
                        
            if (VERBOSE) {
                sprintf(remote_buffer, "check: %d address: %d data: %d tmr2: %d switch_on:%d, current_choice:%d\r\n\r\n",
                        ir_check, ir_address, ir_data, TMR2, switch_on, current_choice);          
                btcomSendString(remote_buffer);            
            }
                
            filter_not_ok:;
            
            previous_check = ir_check;
            previous_address = ir_address;
            previous_data = ir_data;             
            
            e_set_body_led(0);
        } // end of "if keypressed"
        
        if (switch_on == true) { // if a program runs 
            return current_choice;            
        } else { // if no program runs and we choose a selection
            e_set_speed_left(0);
            e_set_speed_right(0);
            remote_display(current_choice);  
            return -1;
        }
        
    // } // end of while(1)

}

// you need to run init_run_remote() first to use this function
void ir_remote_code_key_identification() {

    int current_led = 0;        
    e_init_uart1();        
    
    while (1) {
        ir_check = e_get_check();
        ir_address = e_get_address();
        ir_data = e_get_data();
        if (!(ir_check==previous_check && ir_address == previous_address 
                && ir_data == previous_data)) {
            sprintf(remote_buffer, "check: %d address: %d data: %d tmr2: %d\r\n\r\n",
            e_get_check(), e_get_address(), e_get_data(), TMR2);
            btcomSendString(remote_buffer);
            
            e_set_led(current_led++,0);
            if (current_led > 7)
                current_led = 0;
            e_set_led(current_led,1);

            previous_check = ir_check;
            previous_address = ir_address;
            previous_data = ir_data;            
        }        
    }
}


