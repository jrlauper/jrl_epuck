/*******************************************************************************
 * (Decentralized) swarm coherence using sound -- "Alpha-Algorithm"
 * Jean-Roch LAUPER, unifr.ch (2019-2020)
 *******************************************************************************
 *
 * 
 * Adaptation of the alpha-algorithm described in:
 *          - section 3.2. of Nembrini,Winfield, and Melhuish (2002) and
 *          - section 4.2.2. (pp.85-92) of Nembrini (2005)
 * to e-pucks using beep sound as communication means.
 * The alpha-algo is an improvement (usage of an alpha threshold) of
 *          - 
 * 
 * #Videos: 
 *      - alpha-algo :    
 *              (1) 6 parallel runs with alpha = 5,3 and 1 : 
 *                  https://youtu.be/n0Ww2lsuaYE
 *              (2) 3 parallel runs with varying alpha: 6 to 1 with cycle of 50_
 *                  https://youtu.be/PN_FrZWih0Q
        - scope-visualizer : see directly after the corresponding function
 *          
 * 
 * 
 * [!] To be used with e-pucks HWRev 1.3 only (for volume level)
 * If you want to use the program with another version of e-puck 
 * you need to make new volume test
 * In the code, you can easily uncomment some part to try to make e-pucks HWRev 1.1 and 1.3
 * together. But in practice I found it difficult to find the right volumes : 
 * - the right volume for emission for the two versions and
 * - the right volume for reception
 * => this is a next step to develop
 * 
 * Sources : 
 * 
 * (1) NEMBRINI, Julien, Alan WINFIELD, and Chris MELHUISH (2002). 
 * "Minimalist Coherent Swarming of Wireless Networked Autonomous Mobile Robots". 
 * In: Proceedings of the Seventh International Conference on Simulation of 
 * Adaptive Behavior on From Animals to Animats. ICSAB. pp. 373-382. 
 * URL: http://dl.acm.org/citation.cfm?id=773380.773437
 * 
 * (2) NEMBRINI, Julien (2005). 
 * "Minimalist Coherent Swarming ofWireless Networked Autonomous Mobile Robots". 
 * PhD thesis. Bristol. 
 * URL: http://infoscience.epfl.ch/record/50997 (visited on 12/18/2019).
 * 
 * (3) STØY, Kasper (2001). ?Using Situated Communication in Distributed Autonomous Mobile Robotics?.
 * In: Proceedings of the Seventh Scandinavian Conference on Artificial Intelligence. 
 * SCAI '01. IOS Press, pp. 44-52.
 * 
 * 
 ******************************************************************************/


#ifndef RUN_ALPHA_ALGO_H
#define	RUN_ALPHA_ALGO_H


#include <stdbool.h>



/* ********** ALPHA-ALGO FUNCTION ********************************************** */


void run_alpha_algo(int goal_beep_number);
// intended to be placed in a LOOP to work properly (typically in the main loop)

    /* At the end of each current cycle, the program check the total number of beep heard 
     * WHEN there is LOSS, i.e. when #this-cycle < #previous-cycle 
     *  THEN the programm check 
     *      IF #this-cycle >= goal_beep THEN close behavior (Braitenberg Explorer)
     *      IF #this-cycle < goal_beep THEN too-far behavior 
     *          (corrective behavior to reconnect : half-turn) */


void run_alpha_algo_from_to
        (int max_goal_beep_number, 
        int min_goal_beep_number, 
        int cycle_number);
// intended to be placed in a LOOP to work properly (typically in the main loop)

    /* Start alpha_algo with beep goal (alpha) = max.
     * After some number of cycles (cycle_number)
     * the beep goal is decreased from 1 till alpha = min
     * Then alpha increase again till max and so on.
     * 
     * In other words: this function is equivalent to run_alpha_algo 
     * with beep goal (alpha) changing gradually 
     * from max to min and then 
     * from min to max and so on... 
     * each time after cycle_number cycles
     */


void alpha_algo_init(); 
/* reset first_run var to true if necessary. 
 * Usually not necessary if a reset has been done before launch
 * as the initial value of first_run in the run_alpha_algo.c is true */



/* *********** TOOLS: scope-visualizer ************************************** */


/* The alpha-algorithm detects beeps above a certain volume threshold:
 * MIN_volume_threshold
 * The scope-visualizer allows to see spatially the distance 
 * corresponding to a certain volume threshold.
 *      - a robot, surrounded by a paper, emits a regular beep
 *      - the other robots (w/o paper) turn on themselves in 45? steps 
 *          and light up only if the beep is higher than the set volume.
 */

void set_MIN_volume_threshold(int min_vol);
int get_MIN_volume_threshold();


void scope_visualizer(bool move_on_spot, bool degrees_display);
/* the MIN_threshold is set via set_MIN_volume_threshold(int min_vol)
 *  
 * move_on_spot: the epucks turns on themselves from some angle every 
 *                  x beeps
 * 
 * degrees_display 
 *          off :   just switch on if above min_threshold and off otherwhise
 *          on:     one led lit if above min, and then one more for 10 extra volume
 *                  until +70 (led 7)
 * 
  */


void run_scope_visualizer_demo(int min_start, int min_end, int increment);
/* #videos : 
 * Function used in all the demos : 
 * run_scope_visualizer_demo(100,240,10);
 * 
 * (1) Emitting Speaker Close vs Far (All robots HWRev1.3)
 *      show the impact of the position of the speaker
 *      and the micros on the volume perceived :
 *          - Emitted volume: 80
 *      #Video: https://youtu.be/oM8CY36nAgM
 * 
 * 
 * Comparisons with the situation (1) as reference (Emitting speakers and listeners HWRev 1.3)
 * 
 * (2) Emitting Speaker HWRev 1.1. vs HWRev 1.3 (All the listener are HWRev 1.3)
 *          - Emitted volume: 
 *              . HWRev 1.1: 60
 *              . HWRev 1.3: 80
 *      #Video: https://youtu.be/CEjNUAbJp1g
 *
 * (3) Listener HWRev 1.1. vs HWRev 1.3 (Both Emitting Speaker are HWRev 1.3)
 *          - Emitted volume: HWRev 1.3: 80
 *          - Detected volume : -17% for HWRev 1.1
 *      #Video: https://youtu.be/1sPNnn6-OlYs
 *    
 * (4) Emitting Speaker & Listener HWRev 1.1. vs HWRev 1.3 
 *          - Emitted volume: 
 *              . HWRev 1.1: 60
 *              . HWRev 1.3: 80
 *          - Detected volume : -17% for HWRev 1.1
 * 
 *      #Video: https://youtu.be/Xp_bidK0A_4
 * 
 * 
 * 
 */





#endif	/* RUN_ALPHA_ALGO_H */

