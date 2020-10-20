/*******************************************************************************
 * Remote Control Utility Functions
 * Jean-Roch LAUPER, unifr.ch (2019-2020)
 *******************************************************************************
 * 
 * The picture 
 *      IR-Remote Control led-selection.png
 * you can find in this folder show the correspondence between
 * the leds and the selection chosen by the IR Remote Control
 * 
 * Based on e_remote_control.c/h of the standard library 
 * this software module facilitates the use of a remote control 
 * with the e-puck in order to
 *  - to select one of the selector position 
 *  - to launch or stop one of this selection
 *  - to reset the e-puck
 *  - to put e-puck on sleep (to save energy)
 * 
 * In this file, in the "#define section" 
 * you must enter the couple (address;data) of the 
 * button of the remote control you want to use.
 * You can identifies these values with : 
 *      void ir_remote_code_key_identification();
 * and connecting the e-puck to a terminal through bluetooth.
 * 
 * The current values are the one of the specific remote-control we used ourselves
 * See 
 *      Philips-Long-IR-Remote-Control.png 
 * in this folder.
 * 
 * Once everything is set up, 
 * you must add in main.c :
 *      void init_run_remote();
 * during the initialization phase of your program 
 * and then use :
 *      run_remote_control() 
 * which return the values of the selection 
 * and enables the other functions (program on/off, reset, sleep...
 * 
 */


#ifndef RUN_REMOTE_CONTROL_UTILITY_H
#define	RUN_REMOTE_CONTROL_UTILITY_H



/* ***************** Button Addresses and Data ****************************** */

/* to find the addresses and data sent by the button you want to use 
 * for some function (example to do RESET), please use 
 * the function ir_remote_code_key_identification();*/

// address and data sent by the button chosen for RESET
// in our case [Stop] button with CD selected]: 
#define ROBOT_RESET_add 20
#define ROBOT_RESET_data 54

// [mute] button
#define SLEEP_add 16
#define SLEEP_data 13

// [Play/pause] button with CD selected
#define START_STOP_add 20
#define START_STOP_data 53

// [Down] Button with CD
// our remote control gives two values
#define CHOICE_RESET_add 19
#define CHOICE_RESET_data 7
    #define CHOICE_RESET2_add 7
    #define CHOICE_RESET2_data 63

// [Next Track] Button with CD
#define CHOICE_NEXT_add 20
#define CHOICE_NEXT_data 32

// [Previous Track] Button with CD
#define CHOICE_PREV_add 20
#define CHOICE_PREV_data 33



/* ***************** Functions ****************************** */

void init_run_remote(); 
// to place in the initialization phase of main.c to be able to use run_remote())
// also necessary to be able to use ir_remote_code_key_identification()


// you need to run init_run_remote() first to use this function
int run_remote();
/* Enable the management of the robot through a remote-control:
 *      programs on/off, reset, sleep, program choice
 * returns the program chosen as values.
 * */


// you need to run init_run_remote() first to use this function
void ir_remote_code_key_identification();
/* Enables to identify the couple (address;data) corresponding to some
 * remote control device */
/* If functions returns the same value whatever the button pressed you have
 * probably a remote control that is not compatible with the e-puck.
 * You can't tell in advance: e.g. I have several philips remote control : 
 * some work, some don't */


#endif	/* RUN_REMOTE_CONTROL_UTILITY_H */

