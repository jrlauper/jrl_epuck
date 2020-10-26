extract from e_wav_music2.h

/******************************************************************************* 
 * 
 * TONES MODULE - from pre-recorded sound
 * Version 2.0. August 2019. 
 * Jean-Roch LAUPER (unifr.ch)
 * 
 ******************************************************************************/

/* This library enables to play tones among FGA(880)B CDEFGABC DE + 2 beeps
 * for the frequencies show the following file in this folder: 
 *       Beeps_and_A(880).pdf      
 * 
 * One function corresponds to each tone: e.g. b(1) to play a B during one beat
 * Tones can be called with the ABC... notation 
 * or with do,re,mi,... e.g. b(1) can be called alternatively with si(1)
 
 * Tones are in fact small sin waves recorded and tranformed into a .s file
 *      e_const_sound.s
 * Be careful: this file is not the original e_const_sound.s file 
 * available with the demos from GCTronic and EPuck!
 * 
 * The choice of 14 tones has been guided :
 * First of all by the limited space available for the e_const_sound.s file
 * (=> like this it takes already almost 50% of the program words of the epuck!) 
 * But also : 
 * 1/ by the fact that these tones enables to play already a lot of things
 * although w/o chromaticism
 * 2/ their height seems interesting to try to communicate between robots.
 * (the main motivation behind this library)
  
 * This library relies heavily on the e_sound.c and e_sound.h files
 * from the official e-puck library.
 
 * Here is a code Example which plays the start of Beethoven 5th Symphony
 * (rest(pause) are necessary for "cut-repetition" of a same tone
 
 *          #include "e_epuck_ports.h"
 *          #include "e_wav_music2.h"
 * 
 *          int main() {         
 *              e_init_port();    
 *              e_led_clear();         
 *              e_init_sound();  
 *              float pause = 1.0/16;
 * 
 *              mi(1); rest(pause);mi(1);rest(pause);mi(1);do_(11);
 *              re(1);rest(pause);re(1);rest(pause);re(1);si0(11);
 * 
 *              mi(1);rest(pause);mi(1);rest(pause);mi(1);do_(1);
 *              fa(1);rest(pause);fa(1);rest(pause);fa(1);mi(1);
 *              do1(1);rest(pause);do1(1);rest(pause);do1(1);la(5);
 * 
 *              mi(1);rest(pause);mi(1);rest(pause);mi(1);si0(1);
 *              fa(1);rest(pause);fa(1);rest(pause);fa(1);mi(1);
 *              re1(1);rest(pause);re1(1);rest(pause);re1(1);si(5);
 * 
 *              mi(1);rest(pause);mi(1);re(1);do_(3);re(1);mi(1);
 *              mi(1);rest(pause);mi(1);re(1);do_(3);re(1);mi(1);
 * 
 *              mi(1);rest(pause);mi(1);re(1);do_(4);la0(4);mi(11); 
 *              rest(2); 
 * 
 *          }
 * 
 * 
 * #Video : https://youtu.be/MA7AYAWtUhg
 * shows a series of tunes played with this library
 * 
 * #to convert wave file to e_const_sound.s : you have to use the matlab
 * script you'll find there :
 *      https://github.com/jrlauper/jrl_epuck/tree/master/Matlab_Sound_Script
 * 
 ******************************************************************************/
