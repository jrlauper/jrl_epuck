/* Jean-Roch LAUPER, unifr.ch, 2019/2020
 * 
 * >>> More details in the header file 
 * 
 */


#include <p30Fxxxx.h>
#include <dsp.h>

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_ad_conv.h"
//#include "e_agenda.h"
#include "btcom.h"

#include "e_micro.h"
#include "e_fft.h"
#include "e_fft_utilities.h"

#include "utility_mod3.h"



//#define VERBOSE (true)
#define VERBOSE (false)


int mic_used = 0; 
// Right Mic - I've not been able to change it


// default values for Mode, MIN and MAX
// the fft recognition happens only if volume between MIN and MAX
static int fft_mode = MICRO_ONLY;
static int vol_freq_MIN_thresh = 0; 
static int vol_freq_MAX_thresh = 1000; 


#define VOL_MIN_HwRev1_1_MIC_CORRECTION -17
// percent! 
/* The Micros of HWRev 1.1 and HWRev 1.2 are less sensitive than 
 * the ones of the HWRev 1.3
 * according to gctronic wiki 15%  / our tests: 17%
 * the min threshold must be decreased to get
 * similar results to the ones of HwRev1.3 */


/* ********** Originally in runfftlistener.c of Bonani and Besuchet ********* */

/* Extern definitions */
/* External definitions of the global variables, 
 * of the different arrays where the signals of the different microphones 
 * and the FFT of the selected microphone will be stored, and
 * definitions of useful functions for averaging and copying from one buffer to another*/
/* Typically, the input signal to an FFT routine is a complex array containing samples of an input signal. */
/* For this example, we will provide the input signal in an array declared in Y-data space. */
extern fractcomplex sigCmpx[FFT_BLOCK_LENGTH] __attribute__ ((section (".ydata, data, ymemory"),aligned (FFT_BLOCK_LENGTH * 2 *2)));      		
/* Access to the mic. samples */
extern int e_mic_scan[3][FFT_BLOCK_LENGTH];


/*! \brief Localize the bigger pic of the array
 * \param spectre The array in which the FFT was made
 * \param spectre_length The length of the scan in the array
 * \return The index of the bigger pic detected
 */
int localise_pic_max(fractcomplex *spectre, int spectre_length)
{			
	int i = 0 ;
	long ampl_max= 0 ;	// Max amplitude init
	long ampl_courante = 0 ;  // Current amplitude init 
	int pic_max;
	for (i = 0; i < spectre_length/2; i++)	
	{
		ampl_courante = spectre[i].real*spectre[i].real+spectre[i].imag*spectre[i].imag ; 
        // Calculation of the amplitude of the FFT at the current i position
		
		if (ampl_courante>ampl_max)						
            // If the current amplitude is greater than the maximum amplitude stored so far...
			{
				pic_max = i ;							
                // The position of the Maxima is stored in k_max_1
				ampl_max = ampl_courante ;				
                // The value of the maximum amplitude is replaced by the current value.
			}
	}
	return pic_max;
}



/*! \brief Get the max volume of the sound detected
 * \param spectre The array in which the FFT was made
 * \param pic_pos The index of the louder frequency
 * \return The amplitude of the louder frequency
 */
int get_volume(fractcomplex *spectre, int pic_pos)
{
	if(pic_pos < 0 || pic_pos >= FFT_BLOCK_LENGTH/2)
		return 0;
	return spectre[pic_pos].real*spectre[pic_pos].real+spectre[pic_pos].imag*spectre[pic_pos].imag;
}



/*! \brief Calcul the corresponding frequency of the bigger pic detected
 * \param pic_pos The index of the bigger pic detected
 * \return The corresponding frequency
 */
static int calcul_frequence(int pic_pos)
{
	return (pic_pos*33000)/FFT_BLOCK_LENGTH;
}



/* ********* JRL adaptations/additions ************************************** */


/* ********* SETTINGS ******************************************************* */

// default 0
void fft_set_MIN_volume_threshold(int min_vol) {
    char buffer[100];
    int temp = min_vol;
    
    if (isEpuckVersion1_3()) { //  if HWRev 1.3
        vol_freq_MIN_thresh = min_vol;    
    } else { // if HWRev 1.1            
        temp *= (100+VOL_MIN_HwRev1_1_MIC_CORRECTION);
        temp /= 100; // in two step given integer division
        if (VERBOSE) {
            sprintf(buffer, "Volume HWRev 1.1: original : %d, after correction : %d (%d%%)\r\n", 
                    min_vol, temp, VOL_MIN_HwRev1_1_MIC_CORRECTION);
            btcomSendString(buffer);
        }
        vol_freq_MIN_thresh = temp;
    }	    
}


int fft_get_MIN_volume_threshold() {
    return vol_freq_MIN_thresh;
}


// default: 1000
void fft_set_MAX_volume_threshold(int max_vol) {
    vol_freq_MAX_thresh = max_vol;
}

int fft_get_MAX_volume_threshold() { 
    return vol_freq_MAX_thresh;
}

//MICRO_ONLY or ALL_ADC / default: MICRO_ONLY
void fft_set_mode(int fft_mode_var){ 
    fft_mode = fft_mode_var;
    e_init_ad_scan(fft_mode);   
}


void fft_init(void) { 
 	e_init_port();
//	e_start_agendas_processing();        
	e_init_ad_scan(fft_mode);   
    if(VERBOSE) btcomSendString(CYN "Micros Volume Calibration start...\r\n" COL_RESET);           
    volume_calibration(); // includes body_led on/off    
    if(VERBOSE) btcomSendString(CYN "Micros Volume Calibration done\r\n" COL_RESET); 
}


/* ********* CORE FUNCTIONS ************************************************* */

static int volume = -1;
// global var to enable get_volume if necessary

/* The "main" function of the demo 
 * slightly modified code from Bonani and Besuchet */
int fft_get_frequency(void){    
    
	int pos_pic;
	int frequency;
    
    char buffer[100];

    // Micros scanning
    e_ad_scan_on();

    // Waiting for the acquisition of all values
    while(!e_ad_is_array_filled());
    e_ad_scan_off();

    /* JRL: e_get_micro_volume should be put here and not after.
     * Indeed: the functions uses the e_mic_scan array to make its calculus
     * and this array is modified after to get the fft values */        
    volume = e_get_micro_volume(mic_used);


    // Centering of the signal at the zero point (average = 0)
    e_subtract_mean(e_mic_scan[mic_used], FFT_BLOCK_LENGTH, LOG2_BLOCK_LENGTH);

    // Copies the mic 0 signal into the buffer for FFT.
    // Assigns values to sigCmpx.real (real) and 0 to sigCmpx.imag (imaginary)
    e_fast_copy(e_mic_scan[mic_used], (int*)sigCmpx, FFT_BLOCK_LENGTH);

    // The result is saved => We start a new acquisition
    e_ad_scan_on();

    // Running FFT on the buffer
    e_doFFT_asm(sigCmpx);

    // Searching for the K position of the two maximum frequencies 
    pos_pic = localise_pic_max(sigCmpx, FFT_BLOCK_LENGTH);

//		volume = get_volume(sigCmpx, pos_pic);
    // JRL I don't use it as I've got the value for this from get_volume above        

    if(volume > vol_freq_MIN_thresh 
    && volume < vol_freq_MAX_thresh)
    {
        frequency = calcul_frequence(pos_pic);                                        
        if(VERBOSE) {
            if (frequency >= 5) { // arbitrary floor threshold to avoid too much display
                sprintf(buffer,"vol: %d\t freq:%d \n\r",
                        volume, frequency);    
                btcomSendString(buffer); 
            }
        } 
        return frequency;
    } else {
        return -1;
    }
    
}


/* corrected_value_for_hwrev1_1 option  :
 * 
 * - Depending on the program using fft_get_frequency_volume
 *      it can be useful to return a CORRECTED volume value or not
 *      taking into account the fact that the micros of HWRev 1.1. are
 *      less sensitive than the one of HWRev 1.3
 * - e.g for alpha-algo, you don't need to get a corrected value
 *      as the threshold is adapted for HWRev 1.1. in the file elsewhere
 * - The reference of the return value  is always the HWRev 1.3. micros.
 * Thus, if there is correction (because HWRev 1.1.)
 * the value returned will be increased.
 */
int fft_get_frequency_volume(bool corrected_value_for_hwrev1_1){
    if(corrected_value_for_hwrev1_1){
        if (isEpuckVersion1_3() == false) {
            volume *= (100+VOL_MIN_HwRev1_1_MIC_CORRECTION);
            volume /= 100; // in two step given integer division        
        }
    }
    return volume;
}