/*******************************************************************************
 * MB Simon Game Adaptation 
 * playable with hands (IR-sensors) or whisling (FFT recognition)
 * Jean-Roch LAUPER, unifr.ch (2019-2020)
 *******************************************************************************
 * (This electronic memory game is "a pop culture symbol of the 80's" (wikipedia)
 * 
 * 
 * #The original game: 
 *      https://en.wikipedia.org/wiki/Simon_(game)
 *      There are a lot of videos on youtube, but e.g.
 *      https://youtu.be/1Yqj76Q4jJ4 Simon. Marbles The Brain Store (2013)
 * 
 * 
 * The device is a circle divided in 4 big "buttons:
 *      a blue, a yellow, a green and a red one
 *      (See the picture MB_Simon.png in the folder)
 * A tone is associated to each button.
 * 
 * The device plays a series of tones
 * The player has to repeat this series.
 * At each turn, the length of the series is increased.
 * 
 * According to the sources on the web, there is disagreement 
 * about which tone are played for each button:
 *
 * The one I choose (asking a musician friend to identify them)
 *      green:  1760Hz (A6)
 *      red:    1318.51Hz (E6)
 *      yellow: 1046.5 Hz (C6)
 *      blue:   880 Hz (A5)
 * 
 * #Video: https://youtu.be/mGj8a8Cdw6Y
 *
 */

#ifndef RUN_SIMON_H
#define	RUN_SIMON_H

void run_simon();
// intended to be placed in a LOOP to work properly (typically in the main loop)


void run_simon_init();
/* if necessary initialization not already done: 
 * port, ad-converter, ...
 */


#endif	/* RUN_SIMON_H */

