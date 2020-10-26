/* Jean-Roch LAUPER, unifr.ch, 2019/2020
 * 
 * >>> More details in the header file 
 * 
 */


#include <p30Fxxxx.h>
#include <dsp.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "e_epuck_ports.h"
#include "e_init_port.h"
#include "e_ad_conv.h"
#include "e_fft.h"
#include "e_fft_utilities.h"
#include "e_led.h"

#include "btcom.h"
#include "e_micro.h"
#include "e_freq_recognition.h"



#define VERBOSE true
//#define VERBOSE false



#define TONES_NB 14
#define MAX_FREQ_PER_TONE 5 
// check that the value is high enough given the init_...freq you want to use

#define NB_CHECK 5 
//#define NB_CHECK 20
// nb of fft analysis after which I check which has tone has max numbers of recognotion
// with HF7 : 5 enough
// with A880... 20 not bad


static int min_freq_limit = 1; // to avoid 0
static int max_freq_limit = 30000;


#define MIC_USED  0
// 0 = right mic
/* the MIC_USED need to be 0 (the right one) to be able to use
 all the work already done by the e-puck team*/

static int vol_freq_thresh = 30;
/* Volume above which the signal is treated
 * For piano and whistling: 35 is a good value
 * For comm in long range : 20 and 30 */




/* ********** Comparison with only ONE value for each tone ****************** */
/* used in run_fft_listener();
 * 
 * empirical measurements via uart/bluetooth 
 * e_init_ad_scan(MICRO_ONLY);*/

static int freq_thresh[14] =   {644, 773, 902, 1030, 
                        1031, 1160, 1289, 1417, 
                        1546, 1804, 1933, 2062, 
                        2320, 2707};



/* ********** Comparison with MULTIPLE values for each tone   *************** */
/* arrays used in 
 *      run_fft_listener_comm() and
 *      run_fft_listener_comm_stat() */

static int comm_freq[TONES_NB][MAX_FREQ_PER_TONE];
static int comm_freq_weight[TONES_NB][MAX_FREQ_PER_TONE];
static int comm_freq_sum[TONES_NB]; 
// Sum of the frequency recognized in each tone during NB_CHECK fft analysis




/* ********** A. Whistling and Piano **************************************** */
/* **********    run_fft_listener()  **************************************** */

/* mod on Display Volume of Besuchet and Bonani */
static void display_frequency_on_led(int frequency)
{
	e_led_clear();
    e_set_body_led(0);
    e_set_front_led(0);
 
	if (frequency <= freq_thresh[0]) {        
		e_set_led(4, 1);
        e_set_body_led(1);
    } else if(frequency <= freq_thresh[1]) {		
		e_set_led(5, 1); 
        e_set_body_led(1);
	} else if(frequency <= freq_thresh[2]) {		
		e_set_led(6, 1); 
        e_set_body_led(1);
	} else if(frequency <= freq_thresh[3]) {		
		e_set_led(7, 1); 
        e_set_body_led(1);
	} else if(frequency <= freq_thresh[4]) {		
		e_set_led(1, 1); 
	} else if(frequency <= freq_thresh[5]) {		
		e_set_led(2, 1); 
	} else if(frequency <= freq_thresh[6]) {		
		e_set_led(3, 1); 
	} else if(frequency <= freq_thresh[7]) {		
		e_set_led(4, 1); 
	} else if(frequency <= freq_thresh[8]) {		
		e_set_led(5, 1); 
	} else if(frequency <= freq_thresh[9]) {		
		e_set_led(6, 1); 
	} else if(frequency <= freq_thresh[10]) {		
		e_set_led(7, 1); 
	} else if(frequency <= freq_thresh[11]) {		
		e_set_led(1, 1); 
        e_set_front_led(1);
	} else if(frequency <= freq_thresh[12]) {		
		e_set_led(2, 1); 
        e_set_front_led(1);
	} else if(frequency <= freq_thresh[13]) {		
		e_set_led(3, 1); 
        e_set_front_led(1);
	} 
}


/*! \brief The "main" function of the demo */
void run_fft_listener(void)
{
	int volume;
	int frequency;
    
    char buffer[100];
	
	e_init_port();
	e_init_ad_scan(MICRO_ONLY);
    if (VERBOSE) 
        btcomSendString("\r\ne_init_ad_scan(MICRO_ONLY)\r\n");  
    if (VERBOSE) {
        btcomSendString("------------------------------------------------------------\r\n");
        btcomSendString("run_fft_listener()... \r\n");
        btcomSendString("start\r\n");                  
        sprintf(buffer, "Threshold vol %d\n\r\n\r", vol_freq_thresh);
        btcomSendString(buffer);        
        btcomSendString("------------------------------------------------------------\r\n");
    }

    fft_set_MIN_volume_threshold(vol_freq_thresh);
	while(1) {
        frequency = fft_get_frequency();        
        if (frequency >= min_freq_limit && frequency <= max_freq_limit ) {
            display_frequency_on_led(frequency);  
            if (VERBOSE) {
                sprintf(buffer, "vol: %d\t freq:%d \n\r",
                    volume, frequency);                    
                btcomSendString(buffer);                            
            }        
		}
	}		
}




/* *********** B. Robots communication A(880) - DIRECT Approach ************* */
/* ***********             run_fft_listener_com()               ************* */
/* comparison of the value returned by the fft to a group of values for each tone */

static void init_comm_freq_A880()
{    
    if(VERBOSE)
        btcomSendString("init_comm_freq_A880();\r\n");    
    int i,j;
    for (i=0; i<TONES_NB; i++){
        for (j=0; j<MAX_FREQ_PER_TONE; j++){
            comm_freq[i][j] = -1;
        }
    }
    
    // the following values come from measurements through the e-puck (via uart)
    // and some principles -> avoid common value for different waves... etc...
    
    // f0
    comm_freq[0][0] = 644;
    comm_freq[0][1] = 3480; // comes in D
//    comm_freq[0][2] = 4898; // comes in B0
//   12632; // comes in C1 too
    comm_freq[0][1] = 13406;
    comm_freq[0][2] = 15468;
    comm_freq[0][3] = 16113;
    
    // g0
    comm_freq[1][0] = 773;    
    comm_freq[1][1] = 2320; // comes in d1 too
    comm_freq[1][1] = 3996; // comes in E too]
    comm_freq[1][2] = 5542;
    comm_freq[1][3] = 10312;
    comm_freq[1][4] = 13148;
    
    // a0
    comm_freq[2][0] = 902;
    comm_freq[2][1] = 4382;
    comm_freq[2][2] = 6187;
//    comm_freq[2][3] = 7992; comes in E1 too
    
    // b0  
//    comm_freq[4][0] = 1031; // comes in C too       
    comm_freq[3][0] = 2964;
    comm_freq[3][1] = 5027; 
    comm_freq[3][2] = 6960;
    comm_freq[3][3] = 10957;
    
    // c     
//    comm_freq[4][0] = 1031; // comes in B0 too   
    comm_freq[4][1] = 5285; // comes in A too   
    comm_freq[4][0] = 7347; // comes in B0 too   
//    comm_freq[4][1] = 9410; // comes in A too   
    comm_freq[4][2] = 9539;    
    
    // d
    comm_freq[5][0] = 1160;
    comm_freq[5][1] = 3409;
//    comm_freq[5][2] = 5929; // to test : comes sometimes in B
    comm_freq[5][3] = 8250;
    comm_freq[5][4] = 10669; // comes in A too
    
    // e
    comm_freq[6][0] = 1289;
//    comm_freq[6][1] = 3996; // comes in G0 too
    comm_freq[6][2] = 6574;
    comm_freq[6][3] = 9281;
    
    // f
    comm_freq[7][0] = 1417; 
    comm_freq[7][1] = 4253;    
    comm_freq[7][2] = 9796;
//    comm_freq[7][2] = 7089; // comes in D1 too   
    
    // g
    comm_freq[8][0] = 1546;
    comm_freq[8][1] = 4769;
    comm_freq[8][2] = 7863;
    comm_freq[8][3] = 10312;    
    comm_freq[8][4] = 11085;    
    
    // a
    comm_freq[9][0] = 1804; 
//    comm_freq[9][1] = 5285; // comes for C too    
//    comm_freq[9][2] = 8894; // comes in B0 too
//    comm_freq[9][3] = 10699; // comes for D too)
    comm_freq[9][4] = 12375;
//    comm_freq[9][5] = 14179;
    
    // b
    comm_freq[10][0] = 1933;
    // 5929 comes more often for D
//    comm_freq[10][1] = 9925; // to test... comes sometimes for D
    comm_freq[10][2] = 11988;
    comm_freq[10][3] = 13921;    
    comm_freq[10][4] = 15855;   
    
    // c1
    comm_freq[11][0] = 2062;
    comm_freq[11][1] = 6316;
    comm_freq[11][2] = 10570; // to test : comes sometimes for A
//    comm_freq[11][3] = 12632;    // comes in f0 too    
    comm_freq[11][3] = 14695;
    
    // d1    
//    comm_freq[12][0] = 2320; // comes in G0 too
//    comm_freq[12][1] = 7089; // comes in F too
    comm_freq[12][0] = 11859;
    comm_freq[12][1] = 13019;
    comm_freq[12][2] = 15339;
    
    // e1
    comm_freq[13][0] = 2707;
    comm_freq[11][1] = 7992; //comes in A0 too
    comm_freq[13][2] = 13277;        
    
}


// after tests : unfortunately the rate of recognition is not good :-(
// this is one of the reason that prompts me to try to come back to the A(880) above
static void init_comm_freq_Hz7200()
{
    if(VERBOSE)
        btcomSendString("init_comm_freq_Hz7200;\r\n");
    
    int i,j;
    for (i=0; i<TONES_NB; i++){
        for (j=0; j<MAX_FREQ_PER_TONE; j++){
            comm_freq[i][j] = -1;
        }
    }
    
    // these values come from measurements
    // and some principles -> avoid common value for different waves... etc...
    
    comm_freq[0][0] = 3867; 
    comm_freq[0][1] = 9925;
    
    comm_freq[1][0] = 2449;
    comm_freq[1][1] = 5671;
        
    comm_freq[2][0] = 2578;
    comm_freq[2][1] = 4253;
    comm_freq[2][2] = 5929;
    
    comm_freq[3][0] = 4511;
    comm_freq[3][1] = 6316;
    
    comm_freq[4][0] = 3867;
    comm_freq[4][1] = 6703;
    
    comm_freq[5][0] = 5156;
    
    comm_freq[6][0] = 3351;
    comm_freq[6][1] = 5542;
    
    comm_freq[7][0] = 6058;
    comm_freq[7][1] = 10828;
    
    comm_freq[8][0] = 1289;
    comm_freq[8][1] = 6594;
    comm_freq[8][2] = 3996; // to test - some repetition... but otherhise not enough
    
    comm_freq[9][0] = 1417;
    comm_freq[9][1] = 4382;
    
    comm_freq[10][0]= 1546;
    comm_freq[10][1]= 1615;
    comm_freq[10][2]= 4769;
    comm_freq[10][3]= 7992;
    comm_freq[10][4]= 8121;
    
    comm_freq[11][0]= 1804;
    comm_freq[11][1]= 9023;
    comm_freq[11][2]= 12632;
    comm_freq[11][3]= 16371;
    
    comm_freq[12][0]= 2062;
    comm_freq[12][1]= 12375;
    comm_freq[12][2]= 14437; // test some repetition
    
    comm_freq[13][0]= 9667;
    comm_freq[13][1]= 12117;               
}


/*! JRL mod on Display Volume */
static int display_frequency_on_led_comm(int frequency)
{
    int i,j;
	e_led_clear();
    e_set_body_led(0);
    e_set_front_led(0);
    
    for (i=0; i< TONES_NB; i++){
        for (j=0; j < MAX_FREQ_PER_TONE; j++){
            if (frequency == comm_freq[i][j]) {
                goto  display_frequency_on_led_comm_NEXT;
                // I keep on NEXT while keeping the value of i which is what
                // is important for us
            }
        }
    }
    return -1; // if we reach this point -> not found
    
    //goto
    display_frequency_on_led_comm_NEXT: 
    
	if (i == 0) {        
		e_set_led(4, 1);
        e_set_body_led(1);        
    } else if(i == 1) {		
		e_set_led(5, 1); 
        e_set_body_led(1);
	} else if(i == 2) {		
		e_set_led(6, 1); 
        e_set_body_led(1);
	} else if(i == 3) {		
		e_set_led(7, 1); 
        e_set_body_led(1);
	} else if(i == 4) {		
		e_set_led(1, 1); 
	} else if(i == 5) {		
		e_set_led(2, 1); 
	} else if(i == 6) {		
		e_set_led(3, 1); 
	} else if(i == 7) {		
		e_set_led(4, 1); 
	} else if(i == 8) {		
		e_set_led(5, 1); 
	} else if(i == 9) {		
		e_set_led(6, 1); 
	} else if(i == 10) {		
		e_set_led(7, 1); 
	} else if(i == 11) {		
		e_set_led(1, 1); 
        e_set_front_led(1);
	} else if(i == 12) {		
		e_set_led(2, 1); 
        e_set_front_led(1);
	} else if(i == 13) {		
		e_set_led(3, 1); 
        e_set_front_led(1);
	}     
    return i;        
}


void run_fft_listener_comm(void)
{
	int volume;	
	int frequency;
    
    init_comm_freq_A880();
    
    char buffer[100];	
	e_init_port();
	e_init_ad_scan(MICRO_ONLY);        
    
    if (VERBOSE) {
        btcomSendString("------------------------------------------------------------\r\n");
        btcomSendString("run_fft_listener_comm()...\r\n");
        btcomSendString("start\r\n");          
        sprintf(buffer, "Threshold vol: %d\n\r\n\r", vol_freq_thresh);
        btcomSendString(buffer);        
        btcomSendString("------------------------------------------------------------\r\n");
    }        
	
    fft_set_MIN_volume_threshold(vol_freq_thresh);
	while(1){	
        frequency = fft_get_frequency();             
        if (frequency >= min_freq_limit && frequency <= max_freq_limit ) {                
            if (VERBOSE) {
                sprintf(buffer, "vol: %d\t freq:%d \n\r",
                    volume, frequency);
                btcomSendString(buffer);
            }
            sprintf(buffer, "return: %d\n\r", 
                    display_frequency_on_led_comm(frequency));  
            if (VERBOSE) 
                btcomSendString(buffer);        	
        }
	}		
}




/* *********** C. Robots communication A(880) - STAT Approach   ************* */
/* ***********           run_fft_listener_com_stat()            ************* */

/* we sum the nb of recognitions of a tone on NB_CHECK fft analysis
 *  and choose the highest one as tone identification
 * like this : possible to have some frequencies present in different tones */

static void init_comm_freq_sum(){
    int i;
    for (i=0; i<TONES_NB; i++){
        comm_freq_sum[i] = 0;
    }
}


static void display_frequency_on_led_comm_stat(int i)
{    
    e_led_clear();
	if (i == 0) {        
		e_set_led(4, 1);
        e_set_body_led(1);        
    } else if(i == 1) {		
		e_set_led(5, 1); 
        e_set_body_led(1);
	} else if(i == 2) {		
		e_set_led(6, 1); 
        e_set_body_led(1);
	} else if(i == 3) {		
		e_set_led(7, 1); 
        e_set_body_led(1);
	} else if(i == 4) {		
		e_set_led(1, 1); 
	} else if(i == 5) {		
		e_set_led(2, 1); 
	} else if(i == 6) {		
		e_set_led(3, 1); 
	} else if(i == 7) {		
		e_set_led(4, 1); 
	} else if(i == 8) {		
		e_set_led(5, 1); 
	} else if(i == 9) {		
		e_set_led(6, 1); 
	} else if(i == 10) {		
		e_set_led(7, 1); 
	} else if(i == 11) {		
		e_set_led(1, 1); 
        e_set_front_led(1);
	} else if(i == 12) {		
		e_set_led(2, 1); 
        e_set_front_led(1);
	} else if(i == 13) {		
		e_set_led(3, 1); 
        e_set_front_led(1);
	} 
}


static void init_comm_freq_A880_stat()
{        
    if(VERBOSE)
        btcomSendString("init_comm_freq_A880_stat();\r\n");
    
    int i,j;    
    int default_weight = NB_CHECK;
    int decreased_weight = 1;    
            
    for (i=0; i<TONES_NB; i++){
        for (j=0; j<MAX_FREQ_PER_TONE; j++){
            comm_freq[i][j] = -1;
        }
    }
    
    for (i=0; i<TONES_NB; i++){
        for (j=0; j<MAX_FREQ_PER_TONE; j++){
            comm_freq_weight[i][j] = default_weight;
        }
    }
    
    // these values come from measurements
    // and some principles -> avoid common value for different waves... etc...
    
    // when in spite of the stat approach conflict 
    // I put in comment the less frequent one
    
    /* the problematic duos : 
     * (perhaps just sacrifying one on two... tbd)
     *  (1) b0-c : works quite well but sensitive
     *  (2) g0-e : the issue is around 3996 - which is too frequent just too drop it
     *  (3) f0-d : around 3480
     */ 
    
    // f0
    comm_freq[0][0] = 644;
    comm_freq[0][1] = 3480; // comes in D
        comm_freq_weight[0][1] = decreased_weight;
    comm_freq[0][2] = 4898; // comes in B0
    comm_freq[0][3] = 12632; // comes in C1 too
    comm_freq[0][4] = 13406;
    comm_freq[0][5] = 15468;
    comm_freq[0][6] = 16113;
    
    // g0
    comm_freq[1][0] = 773;    
    comm_freq[1][1] = 2320; // comes in d1 too - but more frequent there
        comm_freq_weight[1][1] = decreased_weight;
    comm_freq[1][2] = 3996; // comes in E too] - with same frequency
        comm_freq_weight[1][2] = decreased_weight;
    comm_freq[1][3] = 5542;
    comm_freq[1][4] = 10312;
    comm_freq[1][5] = 13148;
    
    // a0
    comm_freq[2][0] = 902;
    comm_freq[2][1] = 4382;
    comm_freq[2][2] = 6187;
    comm_freq[2][3] = 7992; //comes in E1 too - more frequent in E1
        comm_freq_weight[2][3] = decreased_weight;
    
    // b0  
    comm_freq[3][0] = 1031; // comes in C too   
        comm_freq_weight[3][0] = decreased_weight;
    comm_freq[3][1] = 2964;
    comm_freq[3][2] = 5027; 
    comm_freq[3][3] = 6960;
    comm_freq[3][4] = 8994;
        comm_freq_weight[3][4] = 4*default_weight;
    comm_freq[3][4] = 10957;
    
    // c     
    comm_freq[4][0] = 1031; // comes in B0 too   
        comm_freq_weight[4][0] = decreased_weight;
    comm_freq[4][1] = 5285; // comes in A too   
    comm_freq[4][2] = 7347; // comes in B0 too   
    comm_freq[4][3] = 9410; // comes in A and D1 too   
    comm_freq[4][4] = 9539;    
    
    // d
    comm_freq[5][0] = 1160;
    comm_freq[5][1] = 3409;
    comm_freq[5][2] = 3480; // present in f0 too
    comm_freq_weight[5][2] = 3*decreased_weight;
    comm_freq[5][2] = 5929; // to test : comes sometimes in B
    comm_freq[5][3] = 8250;
    comm_freq[5][4] = 10669; // comes in A too - but less frequent there
    
    // e
    comm_freq[6][0] = 1289;
    comm_freq[6][1] = 3996; // comes in G0 too - same frequency
    comm_freq_weight[6][1] = 3*decreased_weight;
    comm_freq[6][2] = 6574;
    comm_freq[6][3] = 9281;
    
    // f
    comm_freq[7][0] = 1417; 
    comm_freq[7][1] = 4253;    
    comm_freq[7][2] = 9796;
    comm_freq[7][3] = 7089; // comes in D1 too - but more frequent in D1
    comm_freq_weight[7][3] = decreased_weight;
    
    
    // g
    comm_freq[8][0] = 1546;
    comm_freq[8][1] = 4769;
    comm_freq[8][2] = 7863;
    comm_freq[8][3] = 10312;    
    comm_freq[8][4] = 11085;    
    comm_freq[8][5] = 14179;
    
    // a
    comm_freq[9][0] = 1804; 
    comm_freq[9][1] = 5285; // comes for C too    
    comm_freq[9][2] = 8894; // comes in B0 too
    comm_freq[9][3] = 10699; // comes for D too), but less frequent
        comm_freq_weight[9][3] = 0;
    comm_freq[9][4] = 12375;
    comm_freq[9][5] = 14179;
    
    // b
    comm_freq[10][0] = 1933;
    comm_freq[10][1] = 5929;// 5929 comes more often for D
    comm_freq[10][2] = 9925; // to test... comes sometimes for D
    comm_freq[10][3] = 11988;
    comm_freq[10][4] = 13921;    
    comm_freq[10][5] = 15855;   
    
    // c1
    comm_freq[11][0] = 2062;
    comm_freq[11][1] = 6316;
    comm_freq[11][2] = 10570; // to test : comes sometimes for A
    comm_freq[11][3] = 12632;    // comes in f0 too    
    comm_freq[11][4] = 14695;
    
    // d1    
    comm_freq[12][0] = 2320; // comes in G0 too - but more frequent here in D1
    comm_freq[12][1] = 7089; // comes in F too - but more frequent here in D1
    comm_freq[12][2] = 9410; // comes in c too, but less frequent
    comm_freq[12][3] = 11859;
    comm_freq[12][4] = 13019;
    comm_freq[12][5] = 15339;
    
    // e1
    comm_freq[13][0] = 2707;
    comm_freq[13][1] = 7992; // comes in A0 too - but more frequent here in E1
    comm_freq[13][2] = 13277;            
}


// removal of some tones to test
void init_comm_freq_A880_stat_MOD()
{        
    if(VERBOSE)
        btcomSendString("init_comm_freq_A880_stat_MOD();\r\n");
    
    int i,j;    
    int default_weight = NB_CHECK;
    int decreased_weight = 1;    
            
    for (i=0; i<TONES_NB; i++){
        for (j=0; j<MAX_FREQ_PER_TONE; j++){
            comm_freq[i][j] = -1;
        }
    }
    
    for (i=0; i<TONES_NB; i++){
        for (j=0; j<MAX_FREQ_PER_TONE; j++){
            comm_freq_weight[i][j] = default_weight;
        }
    }
    
    // these values come from measurements
    // and some principles -> avoid common value for different waves... etc...
    
    // when in spite of the stat approach conflict 
    // I put in comment the less frequent one
    
    /* the problematic duos : 
     * (perhaps just sacrifying one on two... tbd)
     *  (1) b0-c : works quite well but sensitive
     *  (2) g0-e : the issue is around 3996 - which is too frequent just too drop it
     *  (3) f0-d : around 3480
     */ 
    
    // f0
    comm_freq[0][0] = 644;
    comm_freq[0][1] = 3480; // comes in D
        comm_freq_weight[0][1] = decreased_weight;
    comm_freq[0][2] = 4898; // comes in B0
    comm_freq[0][3] = 12632; // comes in C1 too
    comm_freq[0][4] = 13406;
    comm_freq[0][5] = 15468;
//    comm_freq[0][6] = 16113;
    
//    // g0
//    comm_freq[1][0] = 773;    
//    comm_freq[1][1] = 2320; // comes in d1 too - but more frequent there
//        comm_freq_weight[1][1] = decreased_weight;
//    comm_freq[1][2] = 3996; // comes in E too] - with same frequency
//        comm_freq_weight[1][2] = decreased_weight;
//    comm_freq[1][3] = 5542;
//    comm_freq[1][4] = 10312;
//    comm_freq[1][5] = 13148;
    
    // a0
    comm_freq[2][0] = 902;
    comm_freq[2][1] = 4382;
    comm_freq[2][2] = 6187;
    comm_freq[2][3] = 7992; //comes in E1 too - more frequent in E1
        comm_freq_weight[2][3] = decreased_weight;
    
    // b0  
    comm_freq[3][0] = 1031; // comes in C too   
        comm_freq_weight[3][0] = decreased_weight;
    comm_freq[3][1] = 2964;
    comm_freq[3][2] = 5027; 
    comm_freq[3][3] = 6960;
    comm_freq[3][4] = 8994;
        comm_freq_weight[3][4] = 4*default_weight;
    comm_freq[3][4] = 10957;
    
    // c     
    comm_freq[4][0] = 1031; // comes in B0 too   
        comm_freq_weight[4][0] = decreased_weight;
    comm_freq[4][1] = 5285; // comes in A too   
    comm_freq[4][2] = 7347; // comes in B0 too   
    comm_freq[4][3] = 9410; // comes in A and D1 too   
    comm_freq[4][4] = 9539;    
    
    // d
    comm_freq[5][0] = 1160;
    comm_freq[5][1] = 3409;
    comm_freq[5][2] = 3480; // present in f0 too
    comm_freq_weight[5][2] = 3*decreased_weight;
    comm_freq[5][2] = 5929; // to test : comes sometimes in B
    comm_freq[5][3] = 8250;
    comm_freq[5][4] = 10669; // comes in A too - but less frequent there
    
    // e
    comm_freq[6][0] = 1289;
    comm_freq[6][1] = 3996; // comes in G0 too - same frequency
    comm_freq_weight[6][1] = 3*decreased_weight;
    comm_freq[6][2] = 6574;
    comm_freq[6][3] = 9281;
    
    // f
    comm_freq[7][0] = 1417; 
    comm_freq[7][1] = 4253;    
    comm_freq[7][2] = 9796;
    comm_freq[7][3] = 7089; // comes in D1 too - but more frequent in D1
    comm_freq_weight[7][3] = decreased_weight;
    
    
    // g
    comm_freq[8][0] = 1546;
    comm_freq[8][1] = 4769;
    comm_freq[8][2] = 7863;
    comm_freq[8][3] = 10312;    
    comm_freq[8][4] = 11085;    
    comm_freq[8][5] = 14179;
    
//    // a
//    comm_freq[9][0] = 1804; 
//    comm_freq[9][1] = 5285; // comes for C too    
//    comm_freq[9][2] = 8894; // comes in B0 too
//    comm_freq[9][3] = 10699; // comes for D too), but less frequent
//        comm_freq_weight[9][3] = 0;
//    comm_freq[9][4] = 12375;
//    comm_freq[9][5] = 14179;
    
    // b
    comm_freq[10][0] = 1933;
    comm_freq[10][1] = 5929;// 5929 comes more often for D
    comm_freq[10][2] = 9925; // to test... comes sometimes for D
    comm_freq[10][3] = 11988;
    comm_freq[10][4] = 13921;    
    comm_freq[10][5] = 15855;   
    
    // c1
    comm_freq[11][0] = 2062;
    comm_freq[11][1] = 6316;
    comm_freq[11][2] = 10570; // to test : comes sometimes for A
    comm_freq[11][3] = 12632;    // comes in f0 too    
    comm_freq[11][4] = 14695;
    
    // d1    
    comm_freq[12][0] = 2320; // comes in G0 too - but more frequent here in D1
    comm_freq[12][1] = 7089; // comes in F too - but more frequent here in D1
    comm_freq[12][2] = 9410; // comes in c too, but less frequent
    comm_freq[12][3] = 11859;
    comm_freq[12][4] = 13019;
    comm_freq[12][5] = 15339;
    
    // e1
    comm_freq[13][0] = 2707;
    comm_freq[13][1] = 7992; // comes in A0 too - but more frequent here in E1
    comm_freq[13][2] = 13277;            
}


// TO ADD : reference of the notes emitted in a pdf
void init_comm_freq_HF7_stat()
{        
     if(VERBOSE)
        btcomSendString("init_comm_freq_HF7_stat();\r\n");
    
    int i,j;    
//    int default_weight = NB_CHECK;
//    int decreased_weight = 1;    
            
    for (i=0; i<TONES_NB; i++){
        for (j=0; j<MAX_FREQ_PER_TONE; j++){
            comm_freq[i][j] = -1;
        }
    }
    
//    for (i=0; i<TONES_NB; i++){
//        for (j=0; j<MAX_FREQ_PER_TONE; j++){
//            comm_freq_weight[i][j] = default_weight;
//        }
//    }
    
    // these values come from measurements
    // and some principles -> avoid common value for different waves... etc...
    
    // when in spite of the stat approach conflict 
    // I put in comment the less frequent one
    
        
    // f0
    comm_freq[0][0] = 1675;
    comm_freq[0][1] = 5027; 
    comm_freq[0][2] = 8378;     
    comm_freq[0][3] = 11730;
    comm_freq[0][4] = 15082;    
    
    // g0
    comm_freq[1][0] = 1804;    
    comm_freq[1][1] = 5414; 
    comm_freq[1][2] = 9023; 
    comm_freq[1][3] = 12632;
        
    // a0
    comm_freq[2][0] = 2191;
    comm_freq[2][1] = 6574;
    comm_freq[2][2] = 10957;
    comm_freq[2][3] = 15339; 
        
    // b0  
    comm_freq[3][0] = 2320;  //not detected but should be this
    comm_freq[3][1] = 6960;
    comm_freq[3][2] = 11601; 
    comm_freq[3][3] = 13921;
    comm_freq[3][4] = 16242;            

    
    // c     
    comm_freq[4][0] = 2449; //not detected but should be this
    comm_freq[4][1] = 7347; 
    comm_freq[4][2] = 12246;
    comm_freq[4][3] = 15855;    
    
    // d
    comm_freq[5][0] = 2578;
    comm_freq[5][1] = 5156;
    comm_freq[5][2] = 7734;        
    comm_freq[5][3] = 10312;
    comm_freq[5][4] = 12890;
    
    // e
    comm_freq[6][0] = 2707; //not detected but should be this
    comm_freq[6][1] = 8121;    
    comm_freq[6][2] = 10828;
    comm_freq[6][3] = 13535;
    
    // f
    comm_freq[7][0] = 2835; 
    comm_freq[7][1] = 8507;    
    comm_freq[7][2] = 14179;
        
    // g
    comm_freq[8][0] = 2964;
    comm_freq[8][1] = 8894;
    comm_freq[8][2] = 14824;    
    
    // a
    comm_freq[9][0] = 3093; 
    comm_freq[9][1] = 9281; 
    comm_freq[9][2] = 15468;    
    
    // b
    comm_freq[10][0] = 3222;
    comm_freq[10][1] = 9667;
    comm_freq[10][2] = 16113;    
    
    
    // c1
    comm_freq[11][0] = 3351;
    comm_freq[11][1] = 9925;
    comm_freq[11][2] = 10054;
            
    // d1    
    comm_freq[12][0] = 3480;     
    
    // e1
    comm_freq[13][0] = 3609;
    
}


static void stat_frequency_calculus(int frequency)
{
    int i,j;	
    
    e_set_body_led(0);
    e_set_front_led(0);            
    for (i=0; i< TONES_NB; i++){
        for (j=0; j < MAX_FREQ_PER_TONE; j++){
            if (frequency == -1) goto stat_frequency_calculus_END;
            if (frequency == comm_freq[i][j]) {
//                comm_freq_sum[i] += comm_freq_weight[i][j];
                comm_freq_sum[i]++;
            }
        }
    }            
    stat_frequency_calculus_END:;
}

// give the index of the tone with the highest recognition
static int max_comm_freq_sum () 
{ 
    int i;
    char buffer[100];
    int max_index = 0;
    
    for (i = 1; i < TONES_NB; i++){
        if (comm_freq_sum[i] > comm_freq_sum[max_index]) {
            max_index = i;
        }    
    }
    
    if (VERBOSE) {
        if (max_index == 0) {
            if (comm_freq_sum[0] == 0) {
                btcomSendString("---------------------------------------\r\n");    
                btcomSendString("no Tone recognized\n\r");
                btcomSendString("---------------------------------------\r\n");    
                return -1;
            }
        }

        btcomSendString("---------------------------------------\r\n");    
        sprintf(buffer, "Recognized Tone : %d (%d)\n\r",
                    max_index, comm_freq_sum[max_index]);
        btcomSendString(buffer);
        display_frequency_on_led_comm_stat(max_index);

        init_comm_freq_sum();
        btcomSendString("---------------------------------------\r\n");
    }
    
    return max_index;    
}



void run_fft_listener_comm_stat(void)
{
	int volume;
	int pos_pic;
	int frequency;    
    
    char buffer[100];
	
	e_init_port();
	e_init_ad_scan(MICRO_ONLY);        
    
        
    if (VERBOSE) {
        btcomSendString("------------------------------------------------------------\r\n");
        btcomSendString("start\r\n");          
        sprintf(buffer, "Threshold vol: %d\n\r\n\r", vol_freq_thresh);
        btcomSendString(buffer);        
        btcomSendString("------------------------------------------------------------\r\n");
    }
        
    fft_set_MIN_volume_threshold(vol_freq_thresh);
    int check = 0; // nb of fft check before decision
    int maxi_freq = 0;
    
    init_comm_freq_A880_stat();
//    init_comm_freq_A880_stat_MOD(); // removal of some notes
//    init_comm_freq_HF7_stat();
    init_comm_freq_sum();
	
	while(1) {				
        frequency = fft_get_frequency();             
        if (frequency >= min_freq_limit && frequency <= max_freq_limit ) {                
            if (VERBOSE) {
                sprintf(buffer, "vol: %d\t freq:%d \n\r",
                    volume, frequency);
                btcomSendString(buffer);        
            }
            stat_frequency_calculus(frequency);  
            check++;
            if (check == NB_CHECK) {
                max_comm_freq_sum();
                check = 0;
            }
        }
		if (frequency == -1) { 
            /* if silence -> so cut between two tones 
             * but we want to send the information once through uart
             */
            if(check != 0) {
                check = 0;
                if (VERBOSE)
                    btcomSendString("#silence#\n\r");
            }
        }
	}		
}

