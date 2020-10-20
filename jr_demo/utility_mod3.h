/*******************************************************************************
 * General Utility Functions
 * Jean-Roch LAUPER, unifr.ch (2019-2020)
 *******************************************************************************
 *
 * Basis of this file: the utility.c/h files of the standard library :
 * 
 * Extra functions added: 
 *  1. a wait function in ms
 *  2 a function checking if the robot is surrounded by a paper 
 *      enables to have two function on one selector (e.g. for scope-visualizer 
 *      in run_alpha_algo.c/h
 *  3. a function displaying the charge level on the led (for HWRev 1.3)
 *  4. a function for the initial calibration of the micro
 *  5. a pseudo-random number generator based on ir-sensors noise
 *  + 
 *  6. color code for UART message
 * 
 **************************************************************************** */


#ifndef UTILITY_MOD
#define	UTILITY_MOD


#include <stdbool.h>


                                                                                  
/* ********** UART Text Color ********** */
/* works well with Putty :-), 
 * to check with other terminal  */

#define RED   "\033[1;31m"
#define GRN   "\033[1;32m"
#define YEL   "\033[1;33m"
#define BLU   "\033[1;34m"
#define MAG   "\033[1;35m"
#define CYN   "\033[1;36m"
#define WHT   "\033[1;37m"
#define COL_RESET "\033[1;0m"

/* usage ex printf(RED "red\n" COL_RESET); */

                                                                                  

/* ***** Standard Library Functions (initially in utility.c/h  ***** - */

void wait(long num);
int getselector();
unsigned int getBatteryValueRaw();
unsigned int getBatteryValuePercentage();
void resetTime(void);
float getDiffTimeMs(void);
float getDiffTimeMsAndReset(void);



/* ***** JRL additions ***** */

void wait_jr(int x); // in ms
void battery_display();


bool surrounded(); 
/* check if the epuck is surrounded by a paper or sth else.
 * allows an extra "selector" for alternative behavior 
 * when one of the epuck can stay still */


/* pseudo-random number generator */
void jr_random_init();
    /* if necessary e-puck initialization not already done*/
int jr_random(int max_number);
    /* return a pseudo-random number based on ir_sensors noise 
     * between 0 and max_number included) */    
void jr_random_visualization();
    /* enables to visualized through leds the results of get_basic_random.
     * get_basic_random gives back a number between 0 and 9
     * by taking advantage of the inherent noise happening a the ir-sensors
     * get_basic_random is the core behind jr_random
     * 
     * #video: https://youtu.be/MvjC3RLYyS8
     */


extern int left_volume_offset;
extern int right_volume_offset;
extern int back_volume_offset;
extern bool volume_calibration_done;

bool volume_calibration();

/******************************************************************************/

#endif	