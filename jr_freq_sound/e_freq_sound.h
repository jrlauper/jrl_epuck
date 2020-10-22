/* *****************************************************************************
 * 
 *                  E-PUCK1 (HWRev 1.1-1.3) 
 *                  SOUND GENERATOR LIBRARY
 *                       "On the Fly"
 * 
 * ************************************************************************** 
 * Jean-Roch LAUPER, unifr.ch (2019-2020)
 *  
 * 
 * #video demo : https://youtu.be/udtJUNvuzgM (septembre 2020) 
 * To launch it: void freq_video_full_demo()
 *
 * 
 * [basis] used in this library: 
 *      https://www.gctronic.com/doc/index.php/E-Puck#Audio_recording
 *      http://projects.gctronic.com/E-Puck/DemoGCtronic-recording/DemoGCtronic-recording.zip
 * plus: the key information: the sample rate for the codec must be 7200 Hz.
 * 
 * 
 *  
 * [!!!] Modified Codec folder files:
 * ----------------------------------
 * Being based on the audio-recording project mentioned just above, 
 * this library uses a MODIFIED version of the CODEC folder files 
 * in comparison with the ones present in the standard library of the e-puck
 * (especially e_sub_dci_kickoff.s)
 * Therefore, if you uses the original codec files of the standard library
 * and not the ones present in this folder, jr_sound_freq stops to work. 
 * 
 * In connection: you can't use this modified version of codec files 
 *  with projects made for the unmodified version of the standard library 
 * - typically the e_wav_music2 library (which uses pre-recorded sounds)
 * 
 *
 * 
 * [A. core function of the library]
 * 
 *          freq_play(double freq, double length);
 * 
 *  enables to play the sound corresponding to some frequency 
 *  (and this w/o having to record any sound beforehand)
 *  
 *  You can also choose  
 *  - the volume of the sound (0-120)
 *  - the shape of the sound wave: SINUS, TRIANGLE, SQUARE, SAWTOOTH
 * 
 *  The duration of the sound corresponding to length=1 
 *  depends on the wave shape chosen: 
 *  (more or less complex operations to calculate the wave)
 *  With 
 *      #define DEFAULT_WAVE_VAL_NUMBER 408 
 *  in e_freq_sound.c,
 *  the duration are approximatively : 
 *  (based on precise video editing footages)
 *      - for a SINUS wave :    175 ms
 *      - for a TRIANGLE wave : 145 ms
 *      - for a SQUARE wave :   125 ms
 *      - for a SAWTOOTH wave : 150 ms
 * 
 *  One variant of the core function enables to play a "glissendo" 
 *  starting from some frequency to another one. 
 * 
 * 
 *  
 * [B. Auxiliary Function]
 * 
 *  To make the use of the library as convenient as possible:
 *  you have two ways to play directly some tone (A,B,C... / do, re, mi...).
 * 
 *  The default setting are the following ones : 
 *      - wave shape :  SINUS 
 *      - volume:       80
 *      - tone length:  175 ms      (length for SINUS see above)
 * 
 *  (1) a series of functions calling directly some tone among 
 *      14 tones + their half-tone going 
 *      from F/fa 698.46Hz  to E/mi 2637.02 Hz
 *      + two beeps.
 *      (see table "Beeps and tones around A(880).pdf")
 *      examples:   freq_a(1); to play an A at 880Hz
 *                  freq_beep1_3072(0.5); to play a beep of half standard length
 *      this library should be sufficient for most purpose. 
 * 
 *  (2) a function a bit more cumbersome to use but enabling to play
 *      more tones and half-tones on 6 octaves, going from 65Hz to 3322Hz if necessary
 *      void freq_tone_play(int octave, int tone, double length);
/*      example     freq_tone_play(O_5, _A, 1); for A 880 of length 1
 *                  Note: O_5 is "Octave 5" and note "Zero_5" 
 *      see below for more details. 
 *
 * 
 * 
 *  Here is a basic example showing two ways to play a beep at 3072
 * 
 *         #include "e_init_port.h"
 *         #include "e_sound.h"
 *         #include "e_freq_sound.h"
 * 
 *         int main() {
 *             e_init_port();
 *             e_init_sound();
 *     
 *             freq_play(3072, 0.5);    
 *             freq_rest(2);
 *             freq_beep1_3072(0.5);
 *             freq_rest(2);        
 *         }
 * 
 * 
 * #video: demo : https://youtu.be/udtJUNvuzgM
 * To launch it: freq_video_full_demo()
 */
 
/* ************************************************************************** */


/* [!] A lot of #define for the various tones 
 * go straight down to find the functions :-) */ 


#ifndef JR_FREQ_SOUND_H
#define	JR_FREQ_SOUND_H


/* ************** SOUND WAVE SHAPES *************** */
#define SINUS 0
#define TRIANGLE 1
#define SQUARE 2
#define SAWTOOTH 3



/* *************** Tones_Array / specific tones ************************** */

/* ***** More tones ******
 * 
 * Further down in this header files, you have specific functions to play every
 * tones and half-tones going 
 *      - from  F0 698.46[Hz]       void freq_f0(double d)  void freq_fa0(double d);
 *      - to    E1 2637.02[Hz]      void freq_e1(double d)  void freq_mi1(double d);
 * 
 * But sometimes, you can want to emit tones lower or higher
 * The tones_array and the corresponding #define just below serve this purpose,
 * enabling to chose tones and half-tones on 6 Octaves.
 * 
 * Through the use of 
 *      void freq_tone_play(int octave, int tone, double length);
 * they enables to pick any tone or half-tones on the 12x6 table represented in 
 *      "tones pinguinoz - mod epuck.pdf" (present in this folder); 
 * In this array, tones goes from 
 *      - from  O_2 _C : 65.41[Hz]      freq_tone_play(O_2, _C, 1)
 *      - to    O_7 _A : 3520[Hz]       freq_tone_play(O_7, _A, 1)
 */


extern double tones_array[12][6]; 

// column of tones_array - Octaves from 2 to 7
#define O_2 0
#define O_3 1
#define O_4 2
#define O_5 3
#define O_6 4
#define O_7 5

// lines of tones_array - be careful to put the _
#define _C 0
#define _C_sharp 1
#define _D_flat 1
#define _D 2
#define _D_sharp 3
#define _E_flat 3
#define _E 4
#define _F 5
#define _F_sharp 6
#define _G_flat 6
#define _G 7
#define _G_sharp 8
#define _A_flat 8
#define _A 9
#define _A_sharp 10
#define _B_flat 10
#define _B 11

#define _DO 0
#define _DO_dieze 1
#define _RE_bemol 1
#define _RE 2
#define _RE_dieze 3
#define _MI_bemol 3
#define _MI 4
#define _FA 5
#define _FA_dieze 6
#define _SOL_bemol 6
#define _SOL 7
#define _SOL_dieze 8
#define _LA_bemol 8
#define _LA 9
#define _LA_dieze 10
#define _SI_bemol 10
#define _SI 11



// ********** SMALL demos functions *******************************************

void freq_all_tones(int note_duration, float rest_duration);
/* plays all tones between freq_f0 and freq_e1 */

void freq_chromatic_all_tones(int note_duration, float rest_duration);
/* plays all tones and half-tones between freq_f0 and freq_e1 */

void freq_ir_piano_C_to_C1(); 
/* allows you to play small melodies 
 * by passing your hands in front of the infrared sensors.
 * #video: https://youtu.be/VEd9eDacvss */

void freq_tunes_examples(int choice);
/* enables to plays various very-well known melodies among 12 choices 
 * - see the code for the list of the melodies */

void freq_all_array_tones(int note_duration, float rest_duration);
/* plays all the tones of the multiple Octaves array_tones */

void freq_video_full_demo();
/* shows the various possibilities of this library in one function*/



// ********** SETTINGS ********************************************************

void set_volume(int volume_var);
void set_wave_val_number(int wave_val_number_var); 
// define the number of lines of the array "coding" the sound-wave
// by default 408 
void set_sound_wave_shape(int shape);
// available shape: SINUS, TRIANGLE, SQUARE, SAWTOOTH



// ********** CORE FUNCTION *************************************************** 

void freq_sound_init(); // if necessary

void freq_play(double freq, double length);
/* freq < ~3400 Hz :  beyond no sound -> filter
 * freq >   ~70 Hz :  below works, but starts to be very low already */
 
/* length = 1 corresponds to ~125-175ms depending on the wave shape chosen 
 * see comment on the top
 * SINUS ~175 ms; TRIANGLE ~145 ms; SQUARE ~125 ms; SAWTOOTH ~150 ms */


void freq_play_from_to(double start_freq, double end_freq, double length);
/* glissendo variant */



// ********** AUXILIARY FUNCTIONS *********************************************


// ---------- a Specific TONE  among 70 --------------------------------------

/* see table tones "pinguinoz - mod epuck.pdf" for details 
 * 6 octaves available with tone and half-tones
 * 70 tones available in total
 * 
 * - half-tone names only with sharp 
 * but you can easily complete with flat if necessary.
 * - in a same way: only english/german name for tone (ABC...)
 * but can be easily completed if necessary
 * 
 * For most of the case the 14 tones below should be enough
 * but tone can be useful for specific needs
 */

void freq_tone_play(int octave, int tone, double length);
/* standard length (SINUS) 175ms => length*175ms 
 * SINUS ~175 ms; TRIANGLE ~145 ms; SQUARE ~125 ms; SAWTOOTH ~150 ms */
 
/* examples 
 *      freq_tone_play(O_5, _A, 1); for A 440 of length 1
 *      freq_tone_play(O_5 + (selector/12), (_C+selector)%12, 1);
 * use O_# for octave number and _ToneName 
 
/* length = 1 corresponds to ~125-175ms depending on the wave shape chosen 
 * see comment on the top
 * SINUS ~175 ms; TRIANGLE ~145 ms; SQUARE ~125 ms; SAWTOOTH ~150 ms */



// ---------- BEEPS -----------------------------------------------------------

void freq_beep1_3072(double std_percent);
void freq_beep2_2816(double std_percent);

void freq_beep1_3072_wo_light(double std_percent); 
    // useful in some circumstances (e.g. alpha_algo)
void freq_beep2_2816_wo_light(double std_percent);

void freq_beep_up(double length);
void freq_beep_down(double length);



// ---------- Predefined 14 TONES with their HALF-TONES -----------------------
/* see table "Beeps_and_A(880) - light.pdf" for the frequency of the plain tones
 * (present in this folder)
 * half-tones are also available see below 
 * 
 * tones names available in the two convention: "A,B,C..." / "do, re, mi ..."
 * half-tones for sharp/flat / for dieze/bemol
 */


/* if i = 1 : led switched on
 * then a light corresponding to the tone lights up on the robot
 * see : "Sensors and Leds - faded - with tones (JR).png"  */
void freq_set_music_led(int i);

void freq_rest(float d); 
    // d: duration of the rest: fraction of STD_WAITING_DURATION

// ===================

void freq_f0(double d);     
void freq_f0_sharp(double d);

void freq_g0_flat(double d);
void freq_g0(double d);
void freq_g0_sharp(double d);

void freq_a0_flat(double d);
void freq_a0(double d); // 440 Hz
void freq_a0_sharp(double d);

void freq_b0_flat(double d);
void freq_b0(double d);

void freq_c(double d);
void freq_c_sharp(double d);

void freq_d_flat(double d);
void freq_d(double d);
void freq_d_sharp(double d);

void freq_e_flat(double d);
void freq_e(double d);

void freq_f(double d);
void freq_f_sharp(double d);

void freq_g_flat(double d);
void freq_g(double d);
void freq_g_sharp(double d);

void freq_a_flat(double d);
void freq_a(double d); // 880 Hz
void freq_a_sharp(double d);

void freq_b_flat(double d);
void freq_b(double d);

void freq_c1(double d);
void freq_c1_sharp(double d);

void freq_d1_flat(double d);
void freq_d1(double d);
void freq_d1_sharp(double d);

void freq_e1_flat(double d);
void freq_e1(double d);

// ===================

void freq_fa0(double d);
void freq_fa0_dieze(double d);

void freq_sol0_bemol(double d);
void freq_sol0(double d);
void freq_sol0_dieze(double d);

void freq_la0_bemol(double d);
void freq_la0(double d); // 440 Hz
void freq_la0_dieze(double d);

void freq_si0_bemol(double d);
void freq_si0(double d);

void freq_do_(double d);
void freq_do_dieze(double d);

void freq_re_bemol(double d);
void freq_re(double d);
void freq_re_dieze(double d);

void freq_mi_bemol(double d);
void freq_mi(double d);

void freq_fa(double d);
void freq_fa_dieze(double d);

void freq_sol_bemol(double d);
void freq_sol(double d);
void freq_sol_dieze(double d);

void freq_la_bemol(double d);
void freq_la(double d); // 880 Hz
void freq_la_dieze(double d);

void freq_si_bemol(double d);
void freq_si(double d);

void freq_do1(double d);
void freq_do1_dieze(double d);

void freq_re1_bemol(double d);
void freq_re1(double d);
void freq_re1_dieze(double d);

void freq_mi1_bemol(double d);
void freq_mi1(double d);


#endif	/* JR_FREQ_SOUND_H */

