/******************************************************************************* 
 * 
 * "MUSIC" Module - COMMUNICATION between Robots version
 * (the e_sound_const.s file is not music tones anymore)
 * Version 1.0. May 2019. 
 * Jean-Roch LAUPER (unifr.ch)
 * 
 ******************************************************************************/

/* The e_music_comm.h library plays pre-recorded sounds 
 * which have been converted from a wave file to a e_const_sound.s file
 * through a Matlab script : 
 * See https://github.com/jrlauper/jrl_epuck/tree/master/Matlab_Sound_Script
 * 
 * In the folder : 
 *      e_const_sound.s alternatives
 * you find three sound .s files I used to try robots communication
 * 
 * To use them : you must 
 *      - copy it in the main folder
 *      - rename it for e_const_sound.s
 *      - adapt the STD_TONES_DURATION value in this file by looking 
 *          into the corresponding pdf
 * 
 * Although the tones are not music tones anymore, I kept the A,B,C... notation
 * for convenience.
 * 
 * (1) example using : FGA(880)B_CDEFGABC_DE - Cut.s
 *      https://youtu.be/bFydJq7alVA
 *
 */
 


#ifndef MUSIC_H
#define	MUSIC_H

#define STD_WAITING_DURATION 150
// initially each tone lasts 200, but 150 gives better
// results for frequency recognition

//#define STD_TONES_DURATION (1600)
// For FGA(880)B_CDEFGABC_DE - Cut.s
//  this number comes from the e_const_sound.s file used

#define STD_TONES_DURATION (1440)
// for HighFrequ7_mod.s and Hz7200_mod.s
//  this number comes from the e_const_sound.s file used

#define STD_TONES_OFFSET 0
// must be a multiple of 4
// initial purpose -> to try to diminish the "tac" effect between notes
// didn't work

void wait_jr(int x);
void set_music_led(int i);

void f0(int d); // d : duration of the tone
void g0(int d);
void a0(int d); 
void b0(int d);

void c(int d);
void d(int d);
void e(int d);
void f(int d);
void g(int d);
void a(int d); 
void b(int d);

void c1(int d);
void d1(int d);
void e1(int d);

// ===================

//void fa0(int d);
//void sol0(int d);
//void la0(int d); 
//void si0(int d);
//
//void do_(int d);
//void re(int d);
//void mi(int d);
//void fa(int d);
//void sol(int d);
//void la(int d); 
//void si(int d);
//
//void do1(int d);
//void re1(int d);
//void mi1(int d);


#endif	/* MUSIC_H */

