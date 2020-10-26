extract from e_freq_sound.h

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
