/* *************** FFT FREQUENCY RECOGNITION **********************************
 * 
 * Summer 2019, unifr.ch, Jean-Roch LAUPER
 *
 * This file is a slight adaptation and simplification of 
 * the great program of Michael Bonani and Jonahtan Besuchet 
 * done for one of the GCTronic demo.
 * 
 * Original folder : 
 *      https://github.com/gctronic/e-puck-library/tree/master/program/demo
 * Original files: 
 *      runfftlistener.c/h
 * 
 * The goal of this adaptation/simplification is simply to change their program 
 * to make the FFT recognition easily available outside their demo 
 * for other programs to use
 *  
 * ************************************************************************** */



#ifndef _E_FREQ_RECOGN
#define _E_FREQ_RECOGN



/* ********** INITIALIZATION (if necessary) ********************************* */
void fft_init(); 
    /* not always necessary if the necessary initializations
     * have already been done before
     * can even slow down needlessly the program if call unnecessary 
     * 
     * includes micros volume calibration
     */



/* ********** SETTINGS ***************************************************** */

void fft_set_mode(int fft_mode); // default mode MICRO_ONLY
    /* (ALL_ADC or MICRO_ONLY)
     * 
     * MCIRO_ONLY is better for fft recognition.
     * However, sometimes you need to stay/be in ALL_ADC
     * if other sensors need ad scan and conversion too: 
     * typically if you need IR prox at the same time 
     *      
     * NOT NECESSARY 
     * if fft_init() not used
     * and initialization already done with : 
     *      e_init_ad_scan(fft_mode)
     * in the programm
     * with the desired mode for fft_mode */     



/* about the use of MIN and MAX volume threshold look below 
 * fft_get_frequency */

void fft_set_MIN_volume_threshold(int min_vol); // default 0
/* note: the volume is defined for HWRev 1.3
 * an automatic correction is made to have comparable results with
 * the micros of HWRev 1.1. which are less sensitive
 * cf. VOL_MIN_HwRev1_1_MIC_CORRECTION
 * currently -20 (HWRev1_1 Mics less sensitive) */
int fft_get_MIN_volume_threshold();

void fft_set_MAX_volume_threshold(int max_vol); // default 1000
int fft_get_MAX_volume_threshold();



/* ********** CORE FUNCTION ************************************************* */

int fft_get_frequency(void);
/* thanks to fft 
 * returns the frequency of a sound recorded 
 * ONLY IF the volume of the sound is between 
 * MIN and MAX volume threshold (can be set with above functions)
 */

int fft_get_frequency_volume(bool corrected_value_for_hwrev1_1);
/* int fft_get_frequency(void) 
 * gets the FREQUENCY only between a MIN and a MAX threshold.
 * However, for some applications (ex. scope visualizer in run_alpha_algo.c/h), 
 * the user needs to get the VOLUME back too
 * (volume of course between min and max, otherwise -1) */
/* Note: the volume is the volume for HWRev 1.3
 * for HWRev 1.1, give the corrected volume if asked,
 * i.e. if corrected_value_for_hwrev1_1 = true
 * cf. VOL_MIN_HwRev1_1_MIC_CORRECTION
 */

#endif
