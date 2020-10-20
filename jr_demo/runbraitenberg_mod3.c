/* Jean-Roch LAUPER, unifr.ch, 2019/2020
 * 
 * [1b] JRL EXPLORER AND LOVER
 *      EL : Explorer and Lover
 * [2]  JRL LIGHT LOVER (robots turn on themselves toward light)
 *      JRL LIGHT LOVER with MOVEMENT (robots move toward light)
 * [1a] ORIGINAL CODE EPUCK and GCTronic : 
 *      FS : Follower & Shocker
 *
 * >>> More details in the header file 
 */

#include "runbraitenberg_mod3.h"

#include <stdio.h>
#include <stdlib.h>
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

#include "utility_mod3.h"

//#include "e_acc.h" /* after attempt: renouncing to acc detection for swissxp*/


//#define VERBOSE (true)
#define VERBOSE (false)


// ***** FS : Follower and Shocker *****
#define PROXSCALING_FOLLOW 20
#define PROXSCALING_SHOCK 4
#define BASICSPEED_FS 550  // SHOCKER and FOLLOWER


// ****** EL : Explorer and Lover *****
#define MAXSPEED 1000 
int EL_basic_speed = 800; // 800 EXPLORER and LOVER 
    // "int" instead of "define" to enable external change
#define CLOSE_SENSOR_VALUE 600  //empirical : rough value at 1 cm 
/* CLOSE_SENSOR_VALUE; plays a role only in the JR versions (EL) not in the GCTronic version (FS)
 * (1) w/o reflecting tapes on robots, 600 is the max, otherwise, shocks between robots
 */

#define LEFT 0
#define RIGHT 1

#define DELAY1 1
#define DELAY2 1

#define LIGHT_LOVER_LINEAR_SPEED 400

static int i, s, m;
long potential[2];
int speed[2];
long ProxSensOffBuf[8];
int ui_lin = 0;

char braiten_buffer[100];


// most problematic issue for other robots detection -> ps6 and ps1
// if arrives straight there -> tendency to stop
// EXPLORER & LOVER weights
int weight[8] = {4,3,2,0,0,2,3,4}; 
//int weight[8] = {4,4,2,1,1,2,4,4}; // alternative attempt


// FOLLOWER weights
int factor_array[2][8] =
	{{-10,-30,15,0,0,-15,30,10},
	{10,30,-15,0,0,15,-30,-10}};

// SHOCKER/AVOIDER weights
int matrix_prox[2][8] =
	{{8,4,2,0,0,-4,-8,-16},
	{-16,-8,-4,0,0,2,4,8}};



/* *****************************************************************************
 * (1b) EXPLORER & LOVER (JRL)
 * ****************************************************************************/

void braitenberg_init(){
    e_init_port();    			// configure port pins
	e_init_ad_scan(ALL_ADC);	// configure Analog-to-Digital Converter Module
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
    
    e_init_motors();    
    e_start_agendas_processing(); 
//    e_acc_calibr(); // for shock in Swiss Xp
}


// Basic speed for Explorer and Lover
void set_EL_basic_speed(int EL_basic_speed_var) {
    EL_basic_speed = EL_basic_speed_var; // default 800
}

int get_EL_basic_speed() {
    return EL_basic_speed; // default 800
}

// EL = Explorer-Lover
void EL_basic_neuron(float sensibility) 
// not very clean... speed[0] and speed[1] in global variables - to improve
{    
    float ds[2] = {0,0};
    int i;          
    // weighted average
    for (i = 0; i < 4; i++){
        ds[RIGHT] += weight[i]*e_get_calibrated_prox(i);
    }
    for (i = 4; i < 8; i++){
        ds[LEFT] += weight[i]*e_get_calibrated_prox(i);
    }
    
    ds[RIGHT]  /= weight[0]+weight[1]+weight[2]+weight[3];    
    ds[LEFT] /= weight[4]+weight[5]+weight[6]+weight[7];
    
    // Inhibition of the speed according to sensors 
    // Direct connection : right sensor connected to right wheel 
    // -> correct speed change for LOVER 
    // for EXPLORER : reverse
    speed[LEFT]  = (int)(EL_basic_speed - sensibility*EL_basic_speed*ds[LEFT]/CLOSE_SENSOR_VALUE);
    speed[RIGHT] = (int)(EL_basic_speed - sensibility*EL_basic_speed*ds[RIGHT]/CLOSE_SENSOR_VALUE);    

	if (speed[1] > MAXSPEED)
		speed[1] = MAXSPEED;
	else if (speed[1] < -MAXSPEED )
		speed[1] = -MAXSPEED;

	if (speed[0] > MAXSPEED)
		speed[0] = MAXSPEED;
	else if (speed[0] < -MAXSPEED )
		speed[0] = -MAXSPEED;	
    
}


void explorer_neuron(void)
{      
    EL_basic_neuron(2.0);
    /* sensibility 2.0 very good for wall and objects like white yogurt pot
     * but not enough for other epucks without reflective tapes
     * if sensibility high => ok for epuck but too sensitive for walls
     * avoid them with too much distance
     * => compromise at 3.5 */
    // [Update with reflecting tapes] 2

//    e_set_led(7,1); e_set_led(1,1);            
    // crossed connections for explorer
    e_set_speed_left(speed[RIGHT]);
	e_set_speed_right(speed[LEFT]);
    
    if (VERBOSE) {
        sprintf(braiten_buffer, "speed left : %d, speed right : %d, \n\r", 
                speed[LEFT], speed[RIGHT]);    
        btcomSendString(braiten_buffer);
    }
}


// for run_alpha_explorer with other sensibility 
// in order for the robots not to go far away too much from each other
void alpha_explorer_neuron()
{      
    EL_basic_neuron(1.5);

//    e_set_led(7,1); e_set_led(1,1);
    // crossed connections for explorer
    e_set_speed_left(speed[RIGHT]);
	e_set_speed_right(speed[LEFT]);
    
    if (VERBOSE) {
        sprintf(braiten_buffer, "speed left : %d, speed right : %d, \n\r", 
                speed[LEFT], speed[RIGHT]);    
        btcomSendString(braiten_buffer);
    }
}


//like explorer with front ps disabled (ps7 and ps0)
// EL = explorer/lover version vs follower/shocker version
void EL_swiss_neuron()
{
    weight[7]=weight[0]=0;
    explorer_neuron();
}


void lover_neuron(void) // lover_neuron 
{    
    int stop_speed = 80;
    
    EL_basic_neuron(4.0); 
    /* sensibility 4.0 very good for wall and object like white yogurt pot
     * but not enough for other epucks
     * => compromise 7.0 */
    /* [Update with reflecting tapes] 4.0 enough for e-puck, 
     * but not for following a moving e-puck explorer 
     * The issue is the speed as the explorer accelerates 
     * when the lover decelerates 
     * */
    
    if((speed[1] < stop_speed  && speed[1] > -stop_speed) &&          
    	(speed[0] < stop_speed && speed[0] > -stop_speed)) 
    {	        
		speed[0] = 0;
        speed[1] = 0;
	}
   
//    e_set_led(0,1);
    
    e_set_speed_left(speed[LEFT]);
	e_set_speed_right(speed[RIGHT]);
    
    if (VERBOSE) {
        sprintf(braiten_buffer, "speed left : %d, speed right : %d, \n\r", 
                speed[RIGHT], speed[LEFT]);    
        btcomSendString(braiten_buffer);
    }
}


/* "main" function of the LOVER */
void run_braitenberg_lover(void)
{
    if (VERBOSE) {
        sprintf(braiten_buffer,">> LOVER << \n\r ");
        btcomSendString(braiten_buffer);
    }

	e_activate_agenda(lover_neuron, 650);
    
    /* if agenda already created -> doesn't create a new one, no issue
     * see e_activate_agenda for more details : return 1 in every cases
     * but the  return happens at different points */
    
//	e_start_agendas_processing();
//	while(1);    
}


/* "main" function of the EXPLORER */
void run_braitenberg_explorer(void)
{	
    if (VERBOSE){
        sprintf(braiten_buffer,">> EXPLORER << \n\r ");
        btcomSendString(braiten_buffer);
    }

	e_activate_agenda(explorer_neuron, 650);
    /* if agenda already created -> doesn't create a new one, no issue
     * see e_activate_agenda for more details : return 1 in every cases
     * but the  return happens at different points */
}


/* "main" function of Swiss XP */
void run_braitenberg_swiss_EL()
{	
    if (VERBOSE) {
        sprintf(braiten_buffer,">> Swiss EL << \n\r ");
        btcomSendString(braiten_buffer);
    }

//    e_set_led(7,1); e_set_led(1,1);      
//    e_set_led(4,1); 
	e_activate_agenda(EL_swiss_neuron, 650);
    /* if agenda already created -> doesn't create a new one, no issue
     * see e_activate_agenda for more details : return 1 in every cases
     * but the  return happens at different points */
    wait_jr(500);
            	
//	while(1);
}


/* improved version using agenda */
 void run_braitenberg_explorer_and_lover()
{    
    int r;    

    if (VERBOSE) 
        btcomSendString(">> EXPLORER and LOVER alternates << \n\r ");    
    e_activate_agenda(explorer_neuron,650);
    e_pause_agenda(explorer_neuron);
    e_activate_agenda(lover_neuron, 650);
    e_pause_agenda(lover_neuron);
        
        
//    while(1) {     
        
    // EXPLORER        
        r = 1; // r -> random number for the waiting time
        if (VERBOSE) 
            btcomSendString(">> EXPLORER << \n\r "); 
        else
            r *= 4; // if no btcomSendString everything goes faster
        e_led_clear();
        e_set_led(1,1); e_set_led(7,1);
        e_restart_agenda(explorer_neuron);        
        
        r *= (e_get_calibrated_prox(0) % 4) +1; 
            // pseudo-random between 1 and 4                           
        /* Note: btcom + explorer neuron 
         * makes everything going slower: 
         * for each cycle of the agenda btcom send the speed of the 
         * two wheels! */                
        wait_jr(r*1000); // ms
        
        e_pause_agenda(explorer_neuron);
        
    // LOVER                
        r = 1;
        if (VERBOSE)
            btcomSendString(">> LOVER << \n\r ");                    
        else
            r *= 4;
        e_led_clear();   
        e_set_led(0,1); 
        e_restart_agenda(lover_neuron);
                                        
        r *= (e_get_calibrated_prox(1) % 4) +1; 
            // pseudo-random between 1 and 4                          
        wait_jr(r*500);
        
        e_pause_agenda(lover_neuron);
//   	}
	
}


/* *****************************************************************************
 * (2) LIGHT Lover (JRL)
 * ****************************************************************************/

static void stop_behavior(){
    e_set_speed_left(0);
    e_set_speed_right(0);
}

/* basis for direction turn behavior : demoEPuck (Besuchet) runlocatesound.c/h
    https://github.com/gctronic/e-puck-library/tree/master/program/demo */
#define TURN_SPEED 1000
#define STEPS_FOR_2PI 1300
// initial value 1300 too-much by experiences
// clockwise

// the e-puck rotates on itself from some angle (clockwise, degrees))
void direction_turn_behavior(int turn_degree_angle) {        
    int steps = 0;
    if (VERBOSE) {
        sprintf(braiten_buffer, "> enter direction_turn_behavior with direction %d \r\n", turn_degree_angle);
        btcomSendString(braiten_buffer);
    }
    int end_turn;    
    end_turn = (int)(STEPS_FOR_2PI*(turn_degree_angle/360.0));
    if (turn_degree_angle == 0) ;
    else if (turn_degree_angle > 0) {
        e_set_steps_left(0); //Set the number of left motor steps            
        e_set_speed_left(TURN_SPEED);
        e_set_speed_right(-TURN_SPEED);        
        if (VERBOSE) {
            sprintf(braiten_buffer, "end_turn = %d \r\n", end_turn);
            btcomSendString(braiten_buffer);
        }
        while(steps < end_turn) {  // turn until done         
            steps = e_get_steps_left();
            if (VERBOSE) {
                sprintf(braiten_buffer, "e_get_steps_left %d \r\n", steps);
                btcomSendString(braiten_buffer);
            }        
        }
    } else { // if turn_degree_angle < 0
        e_set_steps_right(0);
        e_set_speed_left(-TURN_SPEED);
        e_set_speed_right(TURN_SPEED);
        end_turn = -end_turn; 
        if (VERBOSE) {
            sprintf(braiten_buffer, "end_turn = %d \r\n", end_turn);
            btcomSendString(braiten_buffer);
        }         
        while(steps < end_turn){
            steps = e_get_steps_right();
            if (VERBOSE) {
                sprintf(braiten_buffer, "e_get_steps_right %d \r\n", steps);
                btcomSendString(braiten_buffer);
            }
        }
    }
    
    stop_behavior();
    if (VERBOSE)
        btcomSendString("< end direction_turn_behavior \r\n");        
}


// Important Note: lighter the light, lower the value
// hence the minimum and not the maximum!
// if very light -> close to 0

/* if one of the ir-sensor detects some light in an higher way than the other
 * (higher than relevance_threshold)
 * then light_direction_neuron gives back the number of the ir-sensor
 * otherwise it gives back -1
 * 
*/

 int relevance_threshold = 5; //percent
/* 5 worked well also to reject usual min in usual condition
* while no special light is applied 
* But in other attempts : not enough 
*   to change if necessary 
*   5 ok for light lover
    * 12 for alpha-light */
 void set_light_relevance_threshold(int percent) {
     relevance_threshold = percent;
 }

int light_direction_neuron(void) 
{    
    if (VERBOSE)
        btcomSendString(">> enter light_direction_neuron \r\n");
    float ds[2] = {0,0};
    int i;      
    float average = 0.0;
    int min = 4000;
    int min_i = 0;
    
    float difference;
    // if the difference between the min and the average is bigger than this
    // threshold -> we consider the light is from this direction   
    
    int light_value[8];
            
    for (i = 0; i < 8; i++){
        light_value[i] = e_get_ambient_light(i);                
        // test with ambient and see if better with calibrated
    }
    
    if (VERBOSE) {
        for (i = 0; i < 8; i++ ){
            sprintf(braiten_buffer, "PS %d = %d; ",
                    i, light_value[i]);
            btcomSendString(braiten_buffer);
        }
    }
                            
    for (i= 0; i < 8; i++){
        if (light_value[i] < min) {
            min = light_value[i];
            min_i = i;
        }
        average += light_value[i];
    }
    
    average /= 8;
    difference = (average-min)*100/average;
    
    if (VERBOSE) {
        sprintf(braiten_buffer, 
                "max = %d; max_i = %d; average = %.2f; difference = %.2f \r\n",
                min, min_i, average, difference);
        btcomSendString(braiten_buffer);
    }
        
    if (difference >= relevance_threshold) {
        if (VERBOSE) {
            sprintf(braiten_buffer, "<< exit light_direction_neuron with return %d \r\n", min_i);
            btcomSendString(braiten_buffer);
        }
        return min_i;
    } else {
        if (VERBOSE)
            btcomSendString("<< exit light_direction_neuron with return -1 \r\n");
        return -1;           
    }
}


/* *****************************************************************************
 * (2a) TURN toward light without MOVING toward it
 * ****************************************************************************/

// not "static" to make it available in run_light_alpha_algo
void light_lover_neuron(int direction_var)
{      
    if (VERBOSE)
        btcomSendString("\r\n>>> enter light_lover_neuron\r\n");    
    
    int direction;
    e_led_clear();
    e_set_body_led(0); // body on -> light detected
            
    direction = direction_var;
    
    if (direction >  -1) {        
        e_set_body_led(1);
        // discrepancy between led and pas after ps4
        if (direction <= 3)
            e_set_led(direction,1);
        else if (direction < 7)
            e_set_led(direction+1,1);
        else { // if == 7 
            e_set_led(0,1);
//            e_set_body_led(1);
        }
    } else
        e_led_clear();                           
    
    switch (direction) {
        if (VERBOSE)
            btcomSendString(">>> enter switch direction \r\n");
        case -1: 
            direction_turn_behavior(0);
            break;
        case 0: // ps0        
            direction_turn_behavior(0); 
            // and not +5 otherwise move constantly :-)
            break;
        case 7: // ps7
            direction_turn_behavior(0);
            break;        
        case 1: // ps1
            direction_turn_behavior(+45);
            break;
        case 2: // ps2
            direction_turn_behavior(+90);
            break;
        case 3: // ps3
            direction_turn_behavior(+90+60);
            break;
        case 4: // ps4
            direction_turn_behavior(-90-60); // angle must be positive!
            break;
        case 5: // ps5
            direction_turn_behavior(-90);
            break;
        case 6: // ps6
            direction_turn_behavior(-45);
            break;
        
        default:
            break;
    } // switch end
        
    if (VERBOSE)
        btcomSendString("<<< exit light_lover_neuron\r\n");
}


/* The "main" function of the light explorer */
void run_light_lover(void)
{	
    if (VERBOSE){
        sprintf(braiten_buffer,">> LIGHT LOVER << \n\r ");
        btcomSendString(braiten_buffer);
    }
    
    light_lover_neuron(light_direction_neuron());

}


/* *****************************************************************************
 * (2b) TURN toward light WITH MOVING toward it
 * ****************************************************************************/

/* Robot goes toward LIGHT */

//#define LIGHT_LOVER_LINEAR_SPEED 200

/* basis for direction turn behavior : demoEPuck (Besuchet) runlocatesound.c */
//#define TURN_SPEED 1000
//#define STEPS_FOR_2PI 1300
// initial value 1300 too-much by experiences
// clockwise

//#define IR_THRESHOLD 200 
#define IR_THRESHOLD 500 
// with reflecting tape, 500 ok
static bool front_ir_ok() {
    bool ok = true;
    if(e_get_calibrated_prox(6) > IR_THRESHOLD && ok) ok = false;
    if(e_get_calibrated_prox(7) > IR_THRESHOLD && ok) ok = false;
    if(e_get_calibrated_prox(0) > IR_THRESHOLD && ok) ok = false;
    if(e_get_calibrated_prox(1) > IR_THRESHOLD && ok) ok = false;
        
    if (VERBOSE) {
        sprintf(braiten_buffer, 
            "[wide front] (L) IR6=%d, IR7=%d | (R) IR0=%d, IR1=%d \n\r",
                e_get_calibrated_prox(6),
                e_get_calibrated_prox(7), 
                e_get_calibrated_prox(0),
                e_get_calibrated_prox(1));   
        btcomSendString(braiten_buffer);
    }
    
    wait_jr(2); // put in comments if using bt comm for front_ir_ok    
    return ok;
}

/* to get the right number of steps with linear speed :
 * 1/2*(left_steps + right_steps) == end_turn
 * => the case without linear speed is just a special case */

static void direction_turn_and_moving_behavior(int turn_degree_angle) {        
//    int i = 0;
    int steps = 0;
    int delta = TURN_SPEED + LIGHT_LOVER_LINEAR_SPEED - MAXSPEED;
    int moving_turn_speed = TURN_SPEED - delta;
    if (VERBOSE) {
        sprintf(braiten_buffer, "> enter direction_turn_behavior with direction %d \r\n", turn_degree_angle);
        btcomSendString(braiten_buffer);
    }
    int end_turn;    
    end_turn = (int)(STEPS_FOR_2PI*(turn_degree_angle/360.0));
    
    if (turn_degree_angle == -1) {
        ;
    }
    
    else if (turn_degree_angle == 0) {
        e_set_steps_left(0);
        e_set_speed_left(LIGHT_LOVER_LINEAR_SPEED);
        e_set_speed_right(LIGHT_LOVER_LINEAR_SPEED);
        /* LIGHT_LOVER_LINEAR_SPEED steps arbitrary decision : 
         * - not too long or too short to keep fluidity -> 1 sec */
        while(steps < LIGHT_LOVER_LINEAR_SPEED && front_ir_ok()) {
            steps = (e_get_steps_left());
        }
    }
    
    else if (turn_degree_angle > 0) {
        e_set_steps_left(0); //Set the number of left motor steps   
        e_set_steps_right(0);
        e_set_speed_left(moving_turn_speed + LIGHT_LOVER_LINEAR_SPEED);
        e_set_speed_right(-moving_turn_speed + LIGHT_LOVER_LINEAR_SPEED);        
        if (VERBOSE) {
            sprintf(braiten_buffer, "end_turn = %d \r\n", end_turn);
            btcomSendString(braiten_buffer);
        }
        
        while(steps < end_turn) {  // turn until done         
            steps = (e_get_steps_left()-e_get_steps_right())/2;            
            if (VERBOSE) {
                sprintf(braiten_buffer, "e_get_steps_left %d \r\n", steps);
                btcomSendString(braiten_buffer);
            }        
        }
    
    } else { // if turn_degree_angle < 0
        e_set_steps_right(0);
        e_set_steps_left(0);
        e_set_speed_left(-moving_turn_speed + LIGHT_LOVER_LINEAR_SPEED);
        e_set_speed_right(moving_turn_speed + LIGHT_LOVER_LINEAR_SPEED);
        end_turn = -end_turn; 
        if (VERBOSE) {
            sprintf(braiten_buffer, "end_turn = %d \r\n", end_turn);
            btcomSendString(braiten_buffer);
        }         
        while(steps < end_turn){
            steps = (e_get_steps_right()-e_get_steps_left())/2;            
            if (VERBOSE) {
                sprintf(braiten_buffer, "e_get_steps_right %d \r\n", steps);
                btcomSendString(braiten_buffer);
            }
        }
    }
    
    stop_behavior();
    if (VERBOSE)
        btcomSendString("< end direction_turn_behavior \r\n");        
//    e_restart_agenda(light_lover_neuron);
}


int light_direction_neuron(void);
// defined above

static void moving_light_lover_neuron(bool explorer_on, int direction_var)
{      
    int direction;    
    
    if (VERBOSE) {
        btcomSendString("\r\n>>> enter moving_light_lover_neuron :");
        if(explorer_on == true) 
            btcomSendString("Explorer Mode On\r\n");
        else
            btcomSendString("Explorer Mode Off\r\n");
    }    
    
    direction = direction_var;
    e_led_clear();
    e_set_body_led(0);
    
    // A. no light detected
    if (direction == -1) { // -1 = no light detected
        if (explorer_on == true) {
            e_set_led(7,1); e_set_led(1,1);
            e_restart_agenda(explorer_neuron);
        }
        
    // B. light detected 
        /* the percent of detection can be changed in light_direction_neuron()
         * e.g. if we want only a very light light to be detected... */
    } else { // direction != -1   
        e_set_body_led(1); // sign of light detected
        if (explorer_on == true) {
            e_pause_agenda(explorer_neuron);
        }              
        
        // B.1. light detected, but obstacle
        if (front_ir_ok() == false) { // obstacle in front of the robot                   
            ; //do_nothing
//            if (explorer_on == true) {
//                // if explorer_on, simply goes in explorer
//                // but stop in case just in front of the beacon
//                if (direction != 0 && direction != 7) {
//                    e_reset_agenda(explorer_neuron);
//                }                 
//            } // finally I renounce to this -> as with lot of robots -> can stop not only in front of the beacon
            
        // B.2. light detected and no obstacle in front
        } else { // if front_ir_ok == true // no obstacle in front of the robot
            // lighting leds corresponding to max light direction
            // discrepancy between led# and PS# after PS4 -> hence this if/else sequence            
            if (direction <= 3)
                e_set_led(direction,1);
            else if (direction < 7)
                e_set_led(direction+1,1);
            else { // if == 7 
                e_set_led(0,1);
            }
            // moves if none obstacle and light
            switch (direction) {
                if (VERBOSE)
                    btcomSendString(">>> enter switch direction \r\n");
    //            case -1: 
    //                direction_turn_and_moving_behavior(-1);
    //                break;
                case 0: // ps0        
                    direction_turn_and_moving_behavior(0); 
                    // and not +5 otherwise move constantly :-)
                    break;
                case 7: // ps7
                    direction_turn_and_moving_behavior(0);
                    break;        
                case 1: // ps1
                    direction_turn_and_moving_behavior(+45);
                    break;
                case 2: // ps2
                    direction_turn_and_moving_behavior(+90);
                    break;
                case 3: // ps3
                    direction_turn_and_moving_behavior(+90+60);
                    break;
                case 4: // ps4
                    direction_turn_and_moving_behavior(-90-60); // angle must be positive!
                    break;
                case 5: // ps5
                    direction_turn_and_moving_behavior(-90);
                    break;
                case 6: // ps6
                    direction_turn_and_moving_behavior(-45);
                    break;

                default:
                    break;
            } // switch end
        } // else end
    } // end direction != -1
        
    if (VERBOSE)
        btcomSendString("<<< exit light_lover_neuron\r\n");
}


/* The "main" function of the light explorer */
/* if explorer_on ->  when not following light, the robot is in explorer mode*/
void run_moving_light_lover(bool explorer_on)
{	
    if (VERBOSE){
        sprintf(braiten_buffer,">> LIGHT LOVER << \n\r ");
        btcomSendString(braiten_buffer);
    } 
    
    if (explorer_on == true) {
        e_activate_agenda(explorer_neuron, 650);
    }    
    moving_light_lover_neuron(explorer_on, light_direction_neuron());
    // "neuron" name not fully adequate here...
}


/* *****************************************************************************
 * (1b) FOLLOWER & SHOCKER
 *      Original Epuck and Gctronic Demos : Besuchet et Bonani (2007)
 *      with slight modifications, comments and additions (JRL)
 * ****************************************************************************/
/*  https://github.com/gctronic/e-puck-library/tree/master/program/DemoGCtronic-complete
    Files: runbreitenberg_adv.h/c*/


/*! \brief Calcul the speed to set on each wheel for avoiding
 *
 * Here we do a level-headed sum to take advantage of each captor
 * depending of there position. For exemple if the captor number 0
 * detect something, he has to set the right speed high and set 
 * the left speed low.
 */
void shock_neuron(void) //AVOIDER
{
	for (m = 0; m < 2; m++)
	{
		potential[m] = 0;
		for (s = 0; s < 8; s++)
			potential[m] += (matrix_prox[m][s]*e_get_calibrated_prox(s)); // get values from proximity sensors
            //JRL no division by the sum of the weights -> but the sum can give negative number or null
        speed[m] = (potential[m]/PROXSCALING_SHOCK + BASICSPEED_FS);
	}

	if((speed[1] < 50 && speed[1] > -50)
		&& (speed[0] < 50 && speed[0] > -50)) {
		speed[1] = speed[1] * 20;
		speed[0] = speed[0] * 20;
	}

	if (speed[1] > MAXSPEED)
		speed[1] = MAXSPEED;
	else if (speed[1] < -MAXSPEED )
		speed[1] = -MAXSPEED;

	if (speed[0] > MAXSPEED)
		speed[0] = MAXSPEED;
	else if (speed[0] < -MAXSPEED )
		speed[0] = -MAXSPEED;

	e_set_speed_left(speed[1]);
	e_set_speed_right(speed[0]);
    
    if (VERBOSE) {
        sprintf(braiten_buffer, "speed left : %d, speed right : %d, \n\r", 
                speed[1], speed[0]);    
        btcomSendString(braiten_buffer);
    }
}

// like shock_neuron except weight for ps7 and ps0 = 0
//int matrix_prox[2][8] =
//	{{8,4,2,0,0,-4,-8,-16},
//	{-16,-8,-4,0,0,2,4,8}};
void swiss_FS_neuron(void) 
{
    matrix_prox[0][0] = matrix_prox[0][7] = 0;
    matrix_prox[1][0] = matrix_prox[1][7] = 0;
    shock_neuron();
}


/*! \brief Calcul the speed to set on each wheel for following
 *
 * Here we do a level-headed sum to take advantage of each captor
 * depending of there position. For exemple if the captor number 0
 * detect something, he has to set the left speed high and set 
 * the right speed low.
 */
void follow_neuron_alt(void) //JRL: uses only front sensors (7 and 0)
{
	int basic_speed;

	speed[0] = 0;
	speed[1] = 0;
	
	for (m = 0; m < 2; m++)
		for (i = 0; i<8; i++)
			speed[m]  += e_get_calibrated_prox(i)*factor_array[m][i];
	
	basic_speed = MAXSPEED - (e_get_calibrated_prox(7) + e_get_calibrated_prox(0))*2;
	//basic_speed = 1600 - (e_get_prox(7)-ProxSensOffBuf[7] + e_get_prox(0)-ProxSensOffBuf[0]);
	speed[1] = basic_speed + (speed[1]/PROXSCALING_FOLLOW);
	speed[0] = basic_speed + (speed[0]/PROXSCALING_FOLLOW);
	
	if (speed[0] > MAXSPEED)
		speed[0] = MAXSPEED;
	else if ( speed[0] < -MAXSPEED )
		speed[0] = -MAXSPEED;
	if (speed[1] > MAXSPEED)
		speed[1] = MAXSPEED;
	else if ( speed[1] < -MAXSPEED )
		speed[1] = -MAXSPEED;
	e_set_speed_left(speed[1]);
	e_set_speed_right(speed[0]);
    
    if (VERBOSE) {
        sprintf(braiten_buffer, "speed left : %d, speed right : %d, \n\r", 
                speed[1], speed[0]);    
        btcomSendString(braiten_buffer);
    }
}



int lin_speed_calc(int distance, int gain)
{
	int consigne = 100;
	float h = 0.05;
	int ti = 3;
	int ecart = consigne-distance; 
        //JRL ecart = consigne-1/2(IR7+IR0)
	int lin_speed;

	ui_lin = ui_lin + h * ecart / ti;
    /*JRL not sure to fully grasp ui_lin
    // it has a very samll cumulative impact over time
     */
    
	lin_speed = (ecart + ui_lin) * gain;
    
    if (VERBOSE){
        sprintf(braiten_buffer, "tmp_lin_speed : %d, ui_lin : %d, \n\r", 
                lin_speed, ui_lin);    
        btcomSendString(braiten_buffer);
    }

    
    //JRL bounding speed in [-MAXSPEED;MAXSPEED]
	if(lin_speed >= MAXSPEED)
	{
		ui_lin = 999/gain - ecart;
		if(ui_lin > 60)			// valeur aberrante vue sur matlab, donc on restreint ? 40 la valeur de ui
			ui_lin = 60.0;
		lin_speed = 999;
	}
	else if(lin_speed <= -MAXSPEED)
	{
		ui_lin = -999/gain + ecart;
		if(ui_lin < -10)		// valeur aberrante vue sur matlab, donc on restreint ? -10 la valeur de ui
			ui_lin = -10.0;
		lin_speed = -999;
	}
	
    return lin_speed;
}

int angle_speed_calc(int pos, int gain)
{
	int consigne = 0;
	int angle_speed = 0;
	int ecart = consigne - pos;

	angle_speed = ecart*gain;

	if(angle_speed >= MAXSPEED)
		angle_speed = 999;
	else if(angle_speed <= -MAXSPEED)
		angle_speed = -999;

	return angle_speed;
}

void follow_neuron(void)
{
	int lin_speed = lin_speed_calc((e_get_calibrated_prox(7)+e_get_calibrated_prox(0))/2, 6);
    //JRL lin_speed: IR 7 & 0
	int angle_speed = angle_speed_calc((e_get_calibrated_prox(0)+e_get_calibrated_prox(1)) -
										(e_get_calibrated_prox(7)+e_get_calibrated_prox(6)), 4);
    //JRL angle_speed: IR 7 & 0 and IR 6 and 1

	e_set_speed_left (lin_speed - angle_speed);
	e_set_speed_right(lin_speed + angle_speed);

    if (VERBOSE) {
        sprintf(braiten_buffer, "lin_speed : %d, angle_speed : %d, \n\r", 
                lin_speed, angle_speed);    
        btcomSendString(braiten_buffer);    
        sprintf(braiten_buffer, "speed left : %d, speed right : %d, \n\r", 
                lin_speed - angle_speed, 
                lin_speed + angle_speed);    
        btcomSendString(braiten_buffer);
    }
}

/*! \brief The "main" function of the follower demo */
void run_braitenberg_follower(void)
{
    if (VERBOSE) {
        sprintf(braiten_buffer,">> FOLLOWER << \n\r ");
        btcomSendString(braiten_buffer);
    }    

	e_activate_agenda(k2000_led, 2500);
	e_activate_agenda(follow_neuron, 650);
	e_start_agendas_processing();
//	while(1);
}

/*! \brief The "main" function of the follower demo */
void run_braitenberg_follower_alt(void)
{
	if (VERBOSE){
        sprintf(braiten_buffer,">> ALT_FOLLOWER << \n\r ");
        btcomSendString(braiten_buffer);
    } 

	e_activate_agenda(snake_led, 2500);
	e_activate_agenda(follow_neuron_alt, 650);
	e_start_agendas_processing();
//	while(1);
}

/*! \brief The "main" function of the avoider demo */
void run_braitenberg_avoider(void)
{	
	if (VERBOSE) {
        sprintf(braiten_buffer,">> AVOIDER << \n\r ");
        btcomSendString(braiten_buffer);
    }    

	e_activate_agenda(flow_led, 900);
	e_activate_agenda(shock_neuron, 650);
	e_start_agendas_processing();
//	while(1);
}

void run_braitenberg_swiss_FS()
{
	if (VERBOSE) {
        sprintf(braiten_buffer,">> Swiss FS << \n\r ");
        btcomSendString(braiten_buffer);
    }

	e_set_led(4,1);
	e_activate_agenda(swiss_FS_neuron, 650);
	e_start_agendas_processing();
//	while(1);
}



