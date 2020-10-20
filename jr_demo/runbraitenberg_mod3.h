/*******************************************************************************
 * Following and Avoidance Programs (Obstacles and Light):
 * Jean-Roch LAUPER, unifr.ch (2019-2020)
 *******************************************************************************
 *
 * "Braitenberg" -> inspired by Braitenberg's vehicles
 *          BRAITENBERG, Valentino (1984). 
 *          "Vehicles. Experiments in Synthetic Psychology." 
 *          Cambridge, Mass.: MIT Press
 * 
 * # the links toward Demo VIDEO are put directly after the corresponding function: "#Video"
 * 
 * (1) Obstacle following & avoidance (using IR sensors) : 			
 *
 *          (a) FS : FOLLOWER and SHOCKER
 *              Initial version:  Michael Bonani, Jonathan Besuchet 
 *              (Version 2.0 août 2007)              
 *              https://github.com/gctronic/e-puck-library/tree/master/program/DemoGCtronic-complete
 *              Files: runbreitenberg_adv.h/c
 *              > the corresponding functions and the initial comment are at the end
 *              of this file
 * 
 *          (b) EL : EXPLORER and LOVER (alternative to FS - more basic)
 *              Jean-Roch LAUPER, unifr.ch (2019)
 *              (basis: robotics lecture, unifr.ch, Spring 2016)
 *               * EXPLORER is very close to SHOCKER, but simpler : 
 *                      - for each side, SHOKER takes into account the 8 sensors
 *                      - EXPLORER takes only left-hand side sensors for left impact
 *               * FOLLOWER uses sensors 6,7,0,1 with emphasis on 7 and 0
 *                  LOVER is more actively looking ofr an object by taking into account
 *                  also the lateral sensors: so 5,6,7,0,1,2
 * 
 * (2) Light Follower : various functions enabling robots to turn and/or move towards light
 *              Jean-Roch LAUPER, unifr.ch (2019)
 * 
 * (3) "SwissXP" Function : Collective Heap Building
 *          Jean-Roch LAUPER, unifr.ch (2019)
 *          functions enabling to reproduce the experiment of 
 *          MARIS, Marinus and René te BOECKHORST (1996). 
 *          ?Exploiting Physical Constraints: 
 *          Heap Formation Through Behavioral Error in a Group of Robots?.
 *       
 * ****************************************************************************/




#ifndef _BREITENBERG
#define _BREITENBERG

#include <stdbool.h>



/* *****************************************************************************
 * (1b) EXPLORER & LOVER (JRL)
 * ****************************************************************************/

void braitenberg_init();
// in case the necessary initialization haven't already been done
void set_EL_basic_speed(int EL_basic_speed); // EL: Explorer & Lover
int get_EL_basic_speed(); // generates some slowness - to use only if necessary

/* [!!!] The following functionS are 
 * intended to be placed in a LOOP to work properly (typically in the main loop)*/
void run_braitenberg_explorer(void);
	// #Video: https://youtu.be/yz6ijhXEd9Y
    // uses explorer_neuron    
    void alpha_explorer_neuron();
    // made this neuron available externally for use in alpha-algo
void run_braitenberg_lover(void);
    // uses lover_neuron
	// #Video: https://youtu.be/RJmENOPq444
void run_braitenberg_explorer_and_lover(void);
    // alternates the explorer and the lover behavior during random time
	// #Video: https://youtu.be/iLH2aBzI9MI
void run_braitenberg_swiss_EL();
    // Collective Heap Building - "Swiss Xp"
    // uses explorer_neuron w/o ps7 and ps0
    // #Video: https://youtu.be/AevML_FSlEM



/* *****************************************************************************
 * (2) LIGHT Lover (JRL)
 * ****************************************************************************/

void set_light_relevance_threshold(int percent); 
    /* about the difference between 
     *      - the sensor with the min value (where there is the most light)
     *      - the average of all the sensors
     * if diff > threshold in percent -> return the sensor otherwise -1 
    default 5 : 12 good for light alpha go / 5 enough for light-lover */    


/* E-Puck reacts to light : warning! some led flashlight don't work*/
void run_light_lover(void); 
    /* intended to be placed in a LOOP to work properly (typically in the main loop)*/
    // the robot ROTATES toward the light   
    // "TURNs toward light without MOVING toward it"
    // #Video : https://youtu.be/kJHteD2_1GQ
    int light_direction_neuron();
    // returns the number of the ps where light is detected or -1 if no light
    void light_lover_neuron(int direction_var);
    //made available externally for use in light_alpha_algo
void run_moving_light_lover(bool explorer_on);
    /* intended to be placed in a LOOP to work properly (typically in the main loop)*/
    /* if some light is perceived: the robot MOVE TOWARD the light
     * explorer_on :    false : when no light : the robot stop
	 *					#Video: https://youtu.be/TeyvKGIYQrE
     *                  true  : when no light : explorer     
	 *					#Video: https://youtu.be/uTkaBSoXj2o */     

void direction_turn_behavior(int turn_degree_angle); 
/* auxiliary function useful outside the light_lover
 * enable the epuck to turn on the spot from a certain angle
 * (clockwise)
 */ 

 

/* *****************************************************************************
 * (1b) FOLLOWER & SHOCKER
 *      Original Epuck and Gctronic Demos : Besuchet et Bonani (2007)
 *      with slight modifications and additions (JRL)
 * ****************************************************************************/


/* [!!!] The following functionS are 
 * intended to be placed in a LOOP to work properly (typically in the main loop)*/
void run_braitenberg_avoider(void);
    // uses shock_neuron
    // Video demo : http://www.youtube.com/watch?v=Y-RsvDyUfUE
void run_braitenberg_follower(void);
    // uses follow_neuron
    // Video demo: http://www.youtube.com/watch?v=6DYp5O1cnew
void run_braitenberg_follower_alt(void);
    // uses follow_neuron_alt
    // was commented in the original code

// their extension for Swiss XP (JRL))
void run_braitenberg_swiss_FS(void);
    // Collective Heap Building - "Swiss Xp"
    // my adaptation using the code of Bonani and Besuchet
    // like shock_neuron except weight for ps7 and ps0 = 0
    // Jean-Roch LAUPER, unifr.ch (2019)
    // still to test and perhaps to adjust


#endif


// Original comment from Bonani and Besuchet

/**************************************************************************
* 			Programm to follow/avoid obstacles					          *
*			Version 2.0 aot 2007				                          *
*			Michael Bonani, Jonathan Besuchet				              *
*									                                      *
**************************************************************************/

/*! \file
 * \brief Obstacles follower/avoider
 * \section sect1 Introduction
 * This demo is made to illustrate how to use the 8 proximities sensors.
 * \n The demo is divided in two programms:
 * - A programm to follow what is detected on the front of the e-puck
 * (for exemple another e-puck).
 * - A programm to avoid the obstacles.
 * 
 * These two programms work exactelly on the same way. First we do the
 * acquisition of the proxymities sensor and then we do a level-headed
 * sum of this acquisition. Finally we set the speed in concordance of
 * the level-headed sum.
 *
 * \section sect2 Playing the demo
 * \subsection subsect1 Playing the follower
 * First of all, move the selector to the position 4 and reset the e-puck.
 * The e-puck will go forward until he meets anything (it works good with
 * fingers or another e-puck). After that he will
 * follow him by staying always on the same distance.
 * \subsection subsect2 Playing the avoider
 * First of all, move the selector to the position 5 and reset the e-puck.
 * The e-puck will go forward until he meets an obstacle. At this time he
 * will find the better way to avoid him.
 *
 * \section sect3 Video of the demo
 * - The video of the follower demo: http://www.youtube.com/watch?v=6DYp5O1cnew
 * - The video of the avoider demo: http://www.youtube.com/watch?v=Y-RsvDyUfUE
 *
 * \author Jonathan Besuchet
 */
