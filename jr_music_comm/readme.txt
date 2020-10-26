extract from e_music_comm.h

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
 * (2) example using : HighFrequ7_mod.s
 *      https://youtu.be/w7GG8Ibt3go
 * 
 */
