/* Jean-Roch LAUPER, unifr.ch, 2019/2020
 * 
 * >>> More details in the header file 
 * 
 */



#include "utility_mod3.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_ad_conv.h"
#include "btcom.h"

#include "e_led.h"
#include "e_prox.h"
#include "e_micro.h"


extern int e_acc_scan[3][ACC_SAMP_NB];
extern unsigned int e_last_acc_scan_id;
extern unsigned int tickAdcIsr;

#define MAX_BATT_VALUE 2560 // corresponds to 4.2 volts
#define MIN_BATT_VALUE 2070 // corresponds to 3.4 volts
#define BATT_VALUES_RANGE (MAX_BATT_VALUE-MIN_BATT_VALUE)


//#define VERBOSE (true)
#define VERBOSE (false)

char utility_buffer[100];




#define DELAY1 29
#define DELAY2 64
// makes roughly 1ms together

void wait_jr(int x) /* ~ in ms */ // JRL addition
{
    int i, j, k ;
    for (i = 0; i < DELAY2; i++){
        for (j = 0; j < DELAY1; j++) {            
            for (k = 0; k < x; k++);
            NOP(); 
            /* addition of NOP for powersaving purpose 
             * see Microchip Low Power Tips 'n Tricks TIP #14 */
        }
    }
}

/* ***** Functions originally in utility.c ********************************** */

void wait(long num) {
	long i;
	for(i=0;i<num;i++) {
        NOP(); // JRL add
    }
}


int getselector() {
	return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}


unsigned int getBatteryValueRaw() {
    if(isEpuckVersion1_3()) {
        return e_acc_scan[2][e_last_acc_scan_id];   // the battery value on e-puck rev 1.3 is sampled on the same channel where the accelerometer z axis
                                                    // was sampled on e-puck 1.0
    } else {
        return 0;
    }
}


unsigned int getBatteryValuePercentage() {
    if(isEpuckVersion1_3()) {
        return 100*(float)(e_acc_scan[2][e_last_acc_scan_id]-MIN_BATT_VALUE)/(float)BATT_VALUES_RANGE;
    } else {
        return 0;
    }
}


void resetTime(void) {
    tickAdcIsr = 0;
}


// Based on ADC ISR interrupt frequency of 1/16384 (about 61 us). Each time the ISR is entered the tick
// is incremented, so we can compute the elapsed time from the last reset (knowing the ISR period)
float getDiffTimeMs(void) {
    return ((float)tickAdcIsr)*ADC_ISR_PERIOD_MS;   // the function itself takes 20-40 us (negligable)
}


float getDiffTimeMsAndReset(void) {
    float value = ((float)tickAdcIsr)*ADC_ISR_PERIOD_MS;   // the function itself takes 20-40 us (negligable)
    tickAdcIsr = 0;
    return value;
}




/* **************************** Battery ************************************* */

void battery_display(){
    
    int battery_percent;
    if (isEpuckVersion1_3()) {
        battery_percent = getBatteryValuePercentage();
        if(VERBOSE){
            sprintf(utility_buffer, YEL "Battery Percent: %d %% \r\n" COL_RESET,
                    battery_percent);
            btcomSendString(utility_buffer);
        }    
        e_led_clear();
        if (battery_percent >= 10) e_set_led(0,1);
        if (battery_percent >= 20) e_set_led(1,1);
        if (battery_percent >= 30) e_set_led(2,1); 
        if (battery_percent >= 40) e_set_led(3,1);
        if (battery_percent >= 50) e_set_led(4,1);
        if (battery_percent >= 60) e_set_led(5,1);
        if (battery_percent >= 70) e_set_led(6,1);
        if (battery_percent >= 80) e_set_led(7,1); 
        if (battery_percent >= 90) e_set_body_led(1); 
        if (battery_percent >= 100) e_set_front_led(1);     
    } else {
        if(VERBOSE) 
            btcomSendString(YEL "Battery Percent not available\r\n" COL_RESET);
        e_led_clear();
        e_set_led(1,1);
        e_set_led(3,1);
        e_set_led(5,1);
        e_set_led(7,1);
        e_set_body_led(1);
    }

}



/* ******************  Surrounded (ir-sensors) ****************************** */

// in case necessary initialization have not already been done.
void surrounded_init() {
    e_init_port();    			// configure port pins
    e_init_ad_scan(ALL_ADC);	// configure Analog-to-Digital Converter Module        
    e_calibrate_ir();       
}

/* it is assumed that the necessary initializations have been done
 * if this is not the case, just uncomment the first lines */
bool surrounded(){
    const int threshold = 400;
    bool result = true;
    int i;
    for (i = 0; i<8; i++) {
        if (e_get_calibrated_prox(i) < threshold)
                result = false;
    }
    return result;    
}



/* **************************** Random ************************************* */


static int ir_random_offset = 0;
/* to add some randomness with the starting ir_sensor noise in jr_random
 * this has no impact when jr_random is called once.
 * But important when a same robot at a same place ask several times jr_random
 * like in simon */


void jr_random_init(){
    e_init_port();    			// configure port pins
	e_init_ad_scan(ALL_ADC);	// configure Analog-to-Digital Converter Module        
}

/* give_back a random number from 0 to 9, while starting from some sensors
 * -> there is always three passes / the uses of three ir-sensors 
 * empirical tests gives the best results like this! */
static int get_basic_random(int ir_sensor) {
    int temp;
    int ir_sensor_temp = ir_sensor % 8;
       
    // 1st pass
    temp = e_get_prox(ir_sensor_temp);
    temp %= 8;
    
    // 2nd pass
    temp = e_get_prox(temp);
    temp %= 8;
    
    // 3rd and last pass
    
    temp = e_get_prox(temp);
    temp %= 10;
    
    if(VERBOSE) {
        sprintf(utility_buffer, "get_basic_random return: %d \r\n", temp);
        btcomSendString(utility_buffer);
    }
    
    return temp;        
}


int jr_random(int max_number) {
    int remain;
    int result;
    
    int i;
    int figures_number;
    
        
    result = 0;    
    i = 0;
    remain = max_number;
    
    while (remain != 0){
        remain /= 10;
        i++;                
    }
    
    figures_number = i;
    
    for (i = 0; i < figures_number; i++){
        result += (get_basic_random(i+ir_random_offset))*pow(10,i);
    }
    
    result %= (max_number+1);
    
    ir_random_offset++;
    ir_random_offset %= 7;
    
    if(VERBOSE) {
        sprintf(utility_buffer, "jr_random return: %d \r\n", result);
        btcomSendString(utility_buffer);
    }
    
    return result;            
}


static int i = 0;
void jr_random_visualization() {    
    int temp;
    e_led_clear();
    wait_jr(50); 
        // to be able to see when twice the same number
    e_set_body_led(0);
    temp = get_basic_random(i++);    
    e_set_led(temp,1);
    if (temp==9)
        e_set_body_led(1);
    wait_jr(200);        
    i %= 8;
}



/* *********************** Volume Calibration ******************************* */
/* takes the average of a nb of samples on each micros */

#define LEFT_MIC 1
#define RIGHT_MIC 0
#define BACK_MIC 2

extern int left_volume_offset = 0;
extern int right_volume_offset = 0;
extern int back_volume_offset = 0;

extern bool volume_calibration_done = false;


bool volume_calibration(){

    int samples_nb = 100;
    int wait_btw_samples = 10; //ms
    
    long int left_sum = 0;
    long int right_sum = 0;
    long int back_sum = 0;
    int i;
    
    e_set_body_led(1);
    for (i = 0; i < samples_nb; i++){
        left_sum += e_get_micro_volume(LEFT_MIC);
        right_sum += e_get_micro_volume(RIGHT_MIC);
        back_sum += e_get_micro_volume(BACK_MIC);
        if(wait_btw_samples != 0) 
            wait_jr(wait_btw_samples);
    }    
    
    // overflow management
    if (left_sum < 0) 
        left_sum = 0;
    if (right_sum < 0)
        right_sum = 0;
    if (back_sum < 0)
        back_sum = 0;
    
    left_volume_offset = left_sum/samples_nb;
    right_volume_offset = right_sum/samples_nb;
    back_volume_offset = back_sum/samples_nb;
            
    if(VERBOSE){
        sprintf(utility_buffer, "[Vol Offset] L: %d | R: %d | B: %d \r\n",
                left_volume_offset, right_volume_offset, back_volume_offset);
        btcomSendString(utility_buffer);
    }
    
    volume_calibration_done = true; 
    e_set_body_led(0);
    
    return true;
}
