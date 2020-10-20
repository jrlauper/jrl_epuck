/* *****************************************************************************
 * 
 *                  E-PUCK1 (HWRev 1.1-1.3) 
 *                  SOUND GENERATOR LIBRARY
 * 
 * ************************************************************************** */

/* Jean-Roch LAUPER, unifr.ch, 2019/2020
 * 
 * >>> More details in the header file 
 * 
 */

#include "e_freq_sound.h"

#include <math.h>

#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_ad_conv.h"

#include "e_sound.h"
#include "e_led.h"
#include "e_prox.h" // for IR Piano

#include "utility_mod3.h"



/* ************** WAVE SOUND SHAPES *************** */
#define SINUS 0
#define TRIANGLE 1
#define SQUARE 2
#define SAWTOOTH 3

#define DEFAULT_SOUND_WAVE_SHAPE SINUS

extern unsigned int e_dci_unavailable; // e_sound.c


#define STD_TONES_WAITING_DURATION 100
#define STD_BEEP_WAITING_DURATION 150
#define STD_VOLUME 80
// max 120 - beyond -> noise


#define SAMPLE_RATE 7200
/* Hz -> fixed: see e_init_codec_slave.s and e_common.inc */

#define DEFAULT_WAVE_VAL_NUMBER 408
/* determine the standard length of the frequency or tone played : 
 * the corresponding duration depends on the wave-shape:
 * standard: SINUS : 175 ms
 * 
 * SINUS ~175 ms; TRIANGLE ~145 ms; SQUARE ~125 ms; SAWTOOTH ~150 ms */
 
/* [!] DEFAULT_WAVE_VAL_NUMBER
 * must be a multiple of 4 otherwhise just noise!!! */ 


static int wave_val_number = DEFAULT_WAVE_VAL_NUMBER;

#define PI 3.14159265358979

#define BEEP1 3072
#define BEEP2 2916


/* ********** Predefined 14 TONES with their HALF-TONES ******************* */
/* ********** TONES + HALF-TONES FREQUENCY around A880 and A1760 ********** */

#define F0 698.46
#define F0_sharp 739.99
#define G0 783.99
#define G0_sharp 830.61
#define A0 880
#define A0_sharp 932.33
#define B0 987.77

#define C 1046.5
#define C_sharp 1108.73
#define D 1174.66
#define D_sharp 1244.51
#define E 1318.51
#define F 1396.91
#define F_sharp 1479.98
#define G 1567.98
#define G_sharp 1661.22
#define A 1760 
#define A_sharp 1864.66
#define B 1975.53

#define C1 2093
#define C1_sharp 2217.46
#define D1 2349.32
#define D1_sharp 2489.02
#define E1 2637.02



/* *************** Tones_Array ************************** */
/* see "tones pinguinoz - mod epuck.pdf" for details*/

double tones_array[12][6] = {
    {65.41, 130.81, 261.63, 523.25, 1046.50, 2093.00},
    {69.30, 138.59, 277.18, 554.37, 1108.73, 2217.46},
    {73.42, 146.83, 293.66, 587.33, 1174.66, 2349.32},
    {77.78, 155.56, 311.13, 622.25,1244.51, 2489.02},
    {82.41, 164.81, 329.63, 659.26, 1318.51, 2637.02},
    {87.31, 174.61, 349.23, 698.46, 1396.91, 2793.83},
    {92.50, 185.00, 369.99, 739.99, 1479.98, 2959.96},
    {98.00, 196.00, 392.00, 783.99, 1567.98, 3135.96},
    {103.83, 207.65, 415.30, 830.61, 1661.22, 3322.44},
    {110.00, 220.00, 440.00, 880.00, 1760.00, 0},
    {116.54, 233.08, 466.16, 932.33, 1864.66, 0},
    {123.47, 246.94, 493.88, 987.77, 1975.53, 0}
};



/* *************** SETTINGS ************************************************* */

static int volume = STD_VOLUME;

void set_volume(int volume_var){
    volume = volume_var;
}


static int wave_shape = SINUS;

void set_sound_wave_shape(int shape){
    wave_shape = shape;
    if (shape < 0 || shape >3) // shape is neither SINUS 0, TRIANGLE 1, SQUARE 2, SAWTOOTH 3
        shape = DEFAULT_SOUND_WAVE_SHAPE;
}



/* *************** CORE FUNCTION ******************************************** */

void freq_sound_init(){
    e_init_sound();
}

void freq_play(double freq, double length) {
    /* freq < ~3400 Hz :  beyond no sound -> filter
     * freq >   ~70 Hz :  below works, but starts to be very low already */
    /* length = 1 => (by default) corresponds to 175 ms (SINUS)
     * SINUS ~175 ms; TRIANGLE ~145 ms; SQUARE ~125 ms; SAWTOOTH ~150 ms */
    char wave_val[wave_val_number];    
    int i; 
    int rep;
    double angle_inc;
    int length_integer = (int)length;
    double length_remain = length - length_integer;
    int int_temp;
    double double_temp;
    
    /* in void e_play_sound(int* sound_nbr, int sound_length)
     * sound_length must be a multiple of 4 */
    if(wave_val_number % 4 !=0) {
        wave_val_number /= 4; // integer division -> no remainder
        wave_val_number *= 4; // to be sure to have a multiple of 4
    }
    
    angle_inc = freq*2.0*PI/SAMPLE_RATE;
    // each increment of j = an increment of an angle of angle_inc
    
    // A. wave creation/calculation
    switch (wave_shape) {
        
        case SINUS:
                for(i = 0; i < wave_val_number; i++){                
                    wave_val[i]= (char)(sin(i*angle_inc)*volume);   
                } 
                break;
                
        case SQUARE:
                for(i = 0; i < wave_val_number; i++){                
                    double_temp = i*angle_inc;
                    double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                        // reduction of x to the interval [0,2*PI[
                    
                    if (double_temp <= PI)  // double_temp in [0,PI]
                        double_temp = 1;
                    else // double_temp in ]PI, 2PI[
                        double_temp = -1;                    
                    
                    wave_val[i] = (char)(double_temp* volume);
                }
                break;
                
        case TRIANGLE:
                for(i = 0; i < wave_val_number; i++){                
                    double_temp = i*angle_inc;
                    double_temp -= PI/2.; // origin of x where triangle(x) = 1
                    double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                        // reduction of x to the interval [0,2*PI[
                    
                    if (double_temp <= PI)  // double_temp in [0,PI]
                        double_temp = 1 - 2./PI * double_temp;
                    else // double_temp in ]PI, 2PI[
                        double_temp = -1 + 2./PI * (double_temp - PI);                    
                    
                    wave_val[i] = (char)(double_temp* volume);
                }
                break;
                
        case SAWTOOTH:
                for(i = 0; i < wave_val_number; i++){                
                    double_temp = i*angle_inc;
                    double_temp -= PI; // origin of x where sawtooth(x) = -1
                    double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                        // reduction of x to the interval [0,2*PI[
                    
                    double_temp = -1 + double_temp/PI;
                    
                    wave_val[i] = (char)(double_temp* volume);
                }
                break;
                
        default:    ;
//            do_nothing;
                
    } // switch end  

    
    // B. wave play
    
    // B1. integer part of length
    for (rep = 0; rep < length_integer; rep++) {
        e_play_sound(wave_val, wave_val_number);
        while(e_dci_unavailable){
            NOP();
        } // wait until the DCI module stop playing
    }
    
    // B2. fractional part of length
    if (length_remain != 0.0) {
        int_temp = (int)(length_remain*wave_val_number);
        int_temp /= 4;
        int_temp *= 4;
        e_play_sound(wave_val, int_temp);
        
        while(e_dci_unavailable) {
            NOP();
        };	
    }        

}



/* *************** Variants of  CORE FUNCTION ******************************* */

void freq_play_from_to(double start_freq, double end_freq, double length) {
    // enable glissendo
    char wave_val[wave_val_number];    
    int i; 
    int rep;
    double angle_inc;
    int length_integer = (int)length;
    double length_remain = length - length_integer;
    int int_temp;
    double double_temp;
    
    double freq_inc;
    double freq;        
    
    /* in void e_play_sound(int* sound_nbr, int sound_length)
     * sound_length must be a multiple of 4 */
    if(wave_val_number % 4 !=0) {
        wave_val_number /= 4; // integer division -> no remainder
        wave_val_number *= 4; // to be sure to have a multiple of 4
    }
    
    freq_inc = (end_freq-start_freq)/(wave_val_number*length);
    freq = start_freq;
        
    // each increment of i = an increment of an angle of angle_inc
    
    // 1. integer part of length
    
    for (rep = 0; rep<length_integer; rep++){
        
        // 1a. Wave creation
        switch (wave_shape) {

            case SINUS:
                    for(i = 0; i < wave_val_number; i++){   
                        freq += freq_inc;
                        angle_inc = freq*2.0*PI/SAMPLE_RATE;
                        wave_val[i]= (char)(sin(i*angle_inc)*volume);   
                    } 
                    break;

            case SQUARE:
                    for(i = 0; i < wave_val_number; i++){  
                        freq += freq_inc;
                        angle_inc = freq*2.0*PI/SAMPLE_RATE;
                        double_temp = i*angle_inc;
                        double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                            // reduction of x to the interval [0,2*PI[

                        if (double_temp <= PI)  // double_temp in [0,PI]
                            double_temp = 1;
                        else // double_temp in ]PI, 2PI[
                            double_temp = -1;                    

                        wave_val[i] = (char)(double_temp* volume);
                    }
                    break;

            case TRIANGLE:
                    for(i = 0; i < wave_val_number; i++){   
                        freq += freq_inc;
                        angle_inc = freq*2.0*PI/SAMPLE_RATE;
                        double_temp = i*angle_inc;
                        double_temp -= PI/2.; // origin of x where triangle(x) = 1
                        double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                            // reduction of x to the interval [0,2*PI[

                        if (double_temp <= PI)  // double_temp in [0,PI]
                            double_temp = 1 - 2./PI * double_temp;
                        else // double_temp in ]PI, 2PI[
                            double_temp = -1 + 2./PI * (double_temp - PI);                    

                        wave_val[i] = (char)(double_temp* volume);
                    }
                    break;

            case SAWTOOTH:
                    for(i = 0; i < wave_val_number; i++){   
                        freq += freq_inc;
                        angle_inc = freq*2.0*PI/SAMPLE_RATE;
                        double_temp = i*angle_inc;
                        double_temp -= PI; // origin of x where sawtooth(x) = -1
                        double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                            // reduction of x to the interval [0,2*PI[

                        double_temp = -1 + double_temp/PI;

                        wave_val[i] = (char)(double_temp* volume);
                    }
                    break;

            default:    ;
                // do_nothing;

        } // switch end  

        //1b. Wave play
        e_play_sound(wave_val, wave_val_number);
        while(e_dci_unavailable){
            NOP();
        }	// wait until the DCI module stop playing        
            
    } // end of integer part of length
    
    
    // 2. fractional part of length    
    if (length_remain > 0) {
        
        // 2a. Wave creation
        switch (wave_shape) {

            case SINUS:
                    for(i = 0; i < (wave_val_number*length_remain); i++){   
                        freq += freq_inc;
                        angle_inc = freq*2.0*PI/SAMPLE_RATE;
                        wave_val[i]= (char)(sin(i*angle_inc)*volume);   
                    } 
                    break;

            case SQUARE:
                    for(i = 0; i < (wave_val_number*length_remain); i++){  
                        freq += freq_inc;
                        angle_inc = freq*2.0*PI/SAMPLE_RATE;
                        double_temp = i*angle_inc;
                        double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                            // reduction of x to the interval [0,2*PI[

                        if (double_temp <= PI)  // double_temp in [0,PI]
                            double_temp = 1;
                        else // double_temp in ]PI, 2PI[
                            double_temp = -1;                    

                        wave_val[i] = (char)(double_temp* volume);
                    }
                    break;

            case TRIANGLE:
                    for(i = 0; i < (wave_val_number*length_remain); i++){   
                        freq += freq_inc;
                        angle_inc = freq*2.0*PI/SAMPLE_RATE;
                        double_temp = i*angle_inc;
                        double_temp -= PI/2.; // origin of x where triangle(x) = 1
                        double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                            // reduction of x to the interval [0,2*PI[

                        if (double_temp <= PI)  // double_temp in [0,PI]
                            double_temp = 1 - 2./PI * double_temp;
                        else // double_temp in ]PI, 2PI[
                            double_temp = -1 + 2./PI * (double_temp - PI);                    

                        wave_val[i] = (char)(double_temp* volume);
                    }
                    break;

            case SAWTOOTH:
                    for(i = 0; i < (wave_val_number*length_remain); i++){   
                        freq += freq_inc;
                        angle_inc = freq*2.0*PI/SAMPLE_RATE;
                        double_temp = i*angle_inc;
                        double_temp -= PI; // origin of x where sawtooth(x) = -1
                        double_temp -= (int)((double_temp/(2.*PI)))  *2.*PI;
                            // reduction of x to the interval [0,2*PI[

                        double_temp = -1 + double_temp/PI;

                        wave_val[i] = (char)(double_temp* volume);
                    }
                    break;

            default:    ;
    //            do_nothing;

        } // switch end  

        // 2b. Wave play
        e_play_sound(wave_val, (wave_val_number*length_remain));
        while(e_dci_unavailable) {
            NOP();
        }	// wait until the DCI module stop playing        

    } // end of fractional part
}


/* *************** End of CORE FUNCTION and variants  *********************** */




// ########## BEEPS ##########

/* the choice of 3072 and 2816:
 * both are frequencies that are well recognized when played and listened by robot
 * cf. jr_music_comm for more details
 * */


void freq_beep1_3072(double std_percent) {            
    e_set_led(0,1);
    freq_play(BEEP1, std_percent);    
    e_set_led(0,0);
}

void freq_beep1_3072_wo_light(double std_percent) {             
    freq_play(BEEP1, std_percent);        
}

void freq_beep2_2816(double std_percent) {            
    e_set_led(0,1);    
    freq_play(BEEP2, std_percent);    
    e_set_led(0,0);
}

void freq_beep2_2816_wo_light(double std_percent) {             
    freq_play(BEEP2, std_percent);        
}

void freq_beep_up(double length){
    freq_play_from_to(A0,A, length);
}

void freq_beep_down(double length){
    freq_play_from_to(A,A0, length);
}


// ********** Specific TONE ***********

void freq_tone_play(int octave, int tone, double d) {
    freq_play(tones_array[tone][octave], d);
}

// ########## TONES & HALF-TONES ##########
/* see table "Beeps and tones around A(880).pdf"
 * half-tones are also available see below 
 * all the below notes can be played with freq_tone_play too,
 * but in a less convenient way 
 */


static int music_led=1 ;

/* if i = 1 : led switched on
 * then a light corresponding to the tone lights up on the robot
 * see : "Sensors and Leds - faded - with tones (JR).png"  */
void freq_set_music_led(int i){
    music_led=i;
}


void freq_rest(float d){
    wait_jr((int)(STD_TONES_WAITING_DURATION*d));
}


void freq_f0(double d){        
    if (music_led) {
        e_set_body_led(1);
        e_set_led(4, 1);
    }        
    freq_play(F0, d);        
    if (music_led) {
        e_set_led(4, 0);        
        e_set_body_led(0);
    } 
}
void freq_fa0(double d){
    freq_f0(d);
}


void freq_f0_sharp(double d){        
    if (music_led) {
        e_set_body_led(1);
        e_set_led(4, 1);
        e_set_led(5, 1);
    }        
    freq_play(F0_sharp, d);        
    if (music_led) {
        e_set_led(4, 0);
        e_set_led(5, 0);
        e_set_body_led(0);
    }    
}
void freq_fa0_dieze(double d){
    freq_f0_sharp(d);
}
void freq_g0_flat(double d){
    freq_f0_sharp(d);
}
void freq_sol0_bemol(double d){
    freq_f0_sharp(d);
}


void freq_g0(double d){        
    if (music_led) {
        e_set_body_led(1);
        e_set_led(5, 1);   
    }
    freq_play(G0, d);        
    if (music_led) {
        e_set_led(5, 0);        
        e_set_body_led(0);
    }    
}
void freq_sol0(double d){
    freq_g0(d);
}


void freq_g0_sharp(double d){    
    if (music_led) {
        e_set_body_led(1);
        e_set_led(5, 1);   
        e_set_led(6, 1);
    }
    freq_play(G0_sharp, d);        
    if (music_led) {
        e_set_led(5, 0);        
        e_set_led(6, 0);
        e_set_body_led(0);
    }    
}
void freq_sol0_dieze(double d){
    freq_g0_sharp(d);
}
void freq_a0_flat(double d){
    freq_g0_sharp(d);
}
void freq_la0_bemol(double d){
    freq_g0_sharp(d);
}


void freq_a0(double d){        
        if (music_led) {
            e_set_body_led(1);
            e_set_led(6, 1);   
        }
        freq_play(A0,d);        
        if (music_led) {
            e_set_led(6, 0);  
            e_set_body_led(0);
        }    
}
void freq_la0(double d){
    freq_a0(d);
}


void freq_a0_sharp(double d){
    if (music_led) {
        e_set_body_led(1);
        e_set_led(6, 1);   
        e_set_led(7, 1);
    }
    freq_play(A0_sharp, d);        
    if (music_led) {
        e_set_led(6, 0);
        e_set_led(7, 0);
        e_set_body_led(0);
    }    
}
void freq_la0_dieze(double d){
    freq_a0_sharp(d);
}
void freq_b0_flat(double d){
    freq_a0_sharp(d);
}
void freq_si0_bemol(double d){
    freq_a0_sharp(d);
}


void freq_b0(double d){
    if (music_led) {
        e_set_body_led(1);
        e_set_led(7, 1);   
    }
    freq_play(B0, d);   
    if (music_led) {
        e_set_led(7,0);
        e_set_body_led(0);
    }    
}
void freq_si0(double d) {
    freq_b0(d);
}


void freq_c(double d){               
    if (music_led) {
        e_set_led(1, 1);   
    }
    freq_play(C, d);        
    if (music_led) {
        e_set_led(1,0);        
    }    
}
void freq_do_(double d){
    freq_c(d);
}


void freq_c_sharp(double d){               
    if (music_led) {
        e_set_led(1, 1);
        e_set_led(2,1);
    }
    freq_play(C_sharp,d);        
    if (music_led) {
        e_set_led(1,0);
        e_set_led(2,0);
    }    
}
void freq_do_dieze(double d){
    freq_c_sharp(d);
}
void freq_d_flat(double d){
    freq_c_sharp(d);
}
void freq_re_bemol(double d){
    freq_c_sharp(d);
}


void freq_d(double d){              
    if (music_led) {
        e_set_led(2, 1);   
    }
    freq_play(D, d);        
    if (music_led) {
        e_set_led(2,0); 
    }    
}
void freq_re(double d){
    freq_d(d);
}


void freq_d_sharp(double d){              
    if (music_led) {
        e_set_led(2, 1);   
        e_set_led(3,1);
    }
    freq_play(D_sharp,d);        
    if (music_led) {
        e_set_led(2,0);
        e_set_led(3,0);
    }    
}
void freq_re_dieze(double d){
    freq_d_sharp(d);
}
void freq_e_flat(double d){
    freq_d_sharp(d);
}
void freq_mi_bemol(double d){
    freq_d_sharp(d);
}


void freq_e(double d){               
    if (music_led) {
        e_set_led(3, 1);   
    }
    freq_play(E, d);        
    if (music_led) {
        e_set_led(3,0);        
    }    
}
void freq_mi(double d){
    freq_e(d);
}


void freq_f(double d){               
    if (music_led) {
        e_set_led(4, 1);   
    }
    freq_play(F,d);        
    if (music_led) {
        e_set_led(4,0);        
    }    
}
void freq_fa(double d){
    freq_f(d);
}


void freq_f_sharp(double d){               
    if (music_led) {
        e_set_led(4, 1);   
        e_set_led(5,1);
    }
    freq_play(F_sharp,d);       
    if (music_led) {
        e_set_led(4,0);
        e_set_led(5,0);
    }    
}
void freq_fa_dieze(double d){
    freq_f_sharp(d);
}
void freq_g_flat(double d){
    freq_f_sharp(d);
}
void freq_sol_bemol(double d){
    freq_f_sharp(d);
}


void freq_g(double d){               
    if (music_led) {
        e_set_led(5, 1);   
    }
    freq_play(G, d);        
    if (music_led) {
        e_set_led(5,0);        
    }    
}
void freq_sol(double d){
    freq_g(d);
}


void freq_g_sharp(double d){               
    if (music_led) {
        e_set_led(5, 1);   
        e_set_led(6,1);
    }
    freq_play(G_sharp, d);        
    if (music_led) {
        e_set_led(5,0);        
        e_set_led(6,0);
    }    
}
void freq_sol_dieze(double d){
    freq_g_sharp(d);
}
void freq_a_flat(double d){
    freq_g_sharp(d);    
}
void freq_la_bemol(double d){
    freq_g_sharp(d);
}


void freq_a(double d){               
    if (music_led) {
        e_set_led(6, 1);   
    }
    freq_play(A, d);        
    if (music_led) {
        e_set_led(6,0);        
    }    
}
void freq_la(double d){
    freq_a(d);
}


void freq_a_sharp(double d){               
    if (music_led) {
        e_set_led(6, 1);   
        e_set_led(7,1);
    }
    freq_play(A_sharp, d);        
    if (music_led) {
        e_set_led(6,0);
        e_set_led(7,0);
    }    
}
void freq_la_dieze(double d){
    freq_a_sharp(d);
}
void freq_b_flat(double d){
    freq_a_sharp(d);
}
void freq_si_bemol(double d){
    freq_a_sharp(d);
}


void freq_b(double d){               
    if (music_led) {
        e_set_led(7, 1);   
    }
    freq_play(B, d);        
    if (music_led) {
        e_set_led(7,0);      
    }    
}
void freq_si(double d){
    freq_b(d);
}


void freq_c1(double d){               
    if (music_led) {
        e_set_front_led(1);
        e_set_led(1, 1);   
    }
    freq_play(C1, d);        
    if (music_led) {
        e_set_led(1,0);        
        e_set_front_led(0);
    }    
}
void freq_do1(double d){
    freq_c1(d);
}


void freq_c1_sharp(double d){               
    if (music_led) {
        e_set_front_led(1);
        e_set_led(1, 1);   
        e_set_led(2,1);
    }
    freq_play(C1_sharp, d);        
    if (music_led) {
        e_set_led(1,0);        
        e_set_led(2,0);
        e_set_front_led(0);
    }    
}
void freq_do1_dieze(double d){
    freq_c1_sharp(d);
}
void freq_d1_flat(double d){
    freq_c1_sharp(d);
}
void freq_re1_bemol(double d){
    freq_c1_sharp(d);
}


void freq_d1(double d){               
    if (music_led) {
        e_set_front_led(1);
        e_set_led(2, 1);   
    }
    freq_play(D1, d);        
    if (music_led) {
        e_set_led(2,0);        
        e_set_front_led(0);
    }    
}
void freq_re1(double d){
    freq_d1(d);
}


void freq_d1_sharp(double d){               
    if (music_led) {
        e_set_front_led(1);
        e_set_led(2, 1);   
        e_set_led(3,1);
    }
    freq_play(D1_sharp, d);        
    if (music_led) {
        e_set_led(2,0);        
        e_set_led(3,0);
        e_set_front_led(0);
    }    
}
void freq_re1_dieze(double d){
    freq_d1_sharp(d);
}
void freq_e1_flat(double d){
    freq_d1_sharp(d);
}
void freq_mi1_bemol(double d){
    freq_d1_sharp(d);
}


void freq_e1(double d){               
    if (music_led) {
        e_set_front_led(1);
        e_set_led(3, 1);   
    }
    freq_play(E1, d);        
    if (music_led) {
        e_set_led(3,0);        
        e_set_front_led(0);
    }    
}
void freq_mi1(double d){
    freq_e1(d);
}

// ########## TONES end ##########




// ########## SMALL demos functions ########


void freq_all_array_tones(int note_duration, float rest_duration){
    int note, octave;
    
    e_led_clear();
    freq_set_music_led(0);          
    
    for (octave = 0; octave < 6; octave ++){
        for (note = 0; note < 12; note++) {
            freq_tone_play(octave, note, note_duration);
            if (rest_duration > 0) {
                freq_rest(rest_duration);
            }
        }
    }
    freq_f0(note_duration);     
        freq_rest(rest_duration);
}


void freq_all_tones(int note_duration, float rest_duration){
    e_led_clear();
    freq_set_music_led(1);          
    
    freq_f0(note_duration);     
        freq_rest(rest_duration);
    freq_g0(note_duration);                
        freq_rest(rest_duration);
    freq_a0(note_duration);        
        freq_rest(rest_duration);
    freq_b0(note_duration);        
        freq_rest(rest_duration);
    freq_c(note_duration);        
        freq_rest(rest_duration);
    freq_d(note_duration);        
        freq_rest(rest_duration);
    freq_e(note_duration);        
        freq_rest(rest_duration);
    freq_f(note_duration);        
        freq_rest(rest_duration);
    freq_g(note_duration);        
        freq_rest(rest_duration);
    freq_a(note_duration);        
        freq_rest(rest_duration);
    freq_b(note_duration);        
        freq_rest(rest_duration);
    freq_c1(note_duration);        
        freq_rest(rest_duration);
    freq_d1(note_duration);        
        freq_rest(rest_duration);
    freq_e1(note_duration);     
//    e_close_sound();
}

void freq_chromatic_all_tones(int note_duration, float rest_duration){

    e_led_clear();
    freq_set_music_led(1);          
    
    freq_f0(note_duration);     
        freq_rest(rest_duration);
        freq_f0_sharp(note_duration);    
        freq_rest(rest_duration);
    freq_g0(note_duration);                
        freq_rest(rest_duration);
        freq_g0_sharp(note_duration);                
        freq_rest(rest_duration);
    freq_a0(note_duration);        
        freq_rest(rest_duration);
        freq_a0_sharp(note_duration);        
        freq_rest(rest_duration);
    freq_b0(note_duration);        
        freq_rest(rest_duration);        
    freq_c(note_duration);        
        freq_rest(rest_duration);
        freq_c_sharp(note_duration);        
        freq_rest(rest_duration);
    freq_d(note_duration);        
        freq_rest(rest_duration);
        freq_d_sharp(note_duration);        
        freq_rest(rest_duration);
    freq_e(note_duration);        
        freq_rest(rest_duration);        
    freq_f(note_duration);        
        freq_rest(rest_duration);
        freq_f_sharp(note_duration);        
        freq_rest(rest_duration);
    freq_g(note_duration);        
        freq_rest(rest_duration);
        freq_g_sharp(note_duration);        
        freq_rest(rest_duration);
    freq_a(note_duration);        
        freq_rest(rest_duration);
        freq_a_sharp(note_duration);        
        freq_rest(rest_duration);
    freq_b(note_duration);        
        freq_rest(rest_duration);        
    freq_c1(note_duration);        
        freq_rest(rest_duration);
        freq_c1_sharp(note_duration);        
        freq_rest(rest_duration);
    freq_d1(note_duration);        
        freq_rest(rest_duration);
        freq_d1_sharp(note_duration);        
        freq_rest(rest_duration);
    freq_e1(note_duration);     
//    e_close_sound();
}


// "ir piano"
// plays tones when we pass the finger/hand before the IR sensors
void freq_ir_piano_C_to_C1() {   

    e_led_clear();
    freq_set_music_led(0);
    /* the led lit won't be "the official" one 
     * i.e. the one of "Sensors and Leds - faded - with tones (JR).png"
     * it is more close to the sensor triggered */
    e_set_front_led(0);
    
    if (e_get_calibrated_prox(1)>400) {      
        e_set_led(1,1);
        freq_c(1);
    }
    if (e_get_calibrated_prox(2)>400) { 
        e_set_led(2,1);
        freq_d(1);
    }
    if (e_get_calibrated_prox(3)>400) {
        e_set_led(3,1);
        freq_e(1);    
    }
    if (e_get_calibrated_prox(4)>400) {                    
        e_set_led(5,1);                                    
        freq_f(1);
    }
    if (e_get_calibrated_prox(5)>400) {                                    
        e_set_led(6,1);                                        
        freq_g(1);                    
    }
    if (e_get_calibrated_prox(6)>400) {                    
        e_set_led(7,1);                                    
        freq_a(1);
    }
    if (e_get_calibrated_prox(7)>400) {                    
        e_set_led(0,1);                                    
        freq_b(1);
    }
    if (e_get_calibrated_prox(0)>400) {                                        
        e_set_front_led(1);                    
        freq_c1(1);
    }
}


// thanks to Celine Lauper for the identification of the tones of the tunes :-) 
void freq_tunes_examples(int choice) {
    int i;
    float pause;
//    if (AUDIO_ON == 0) 
//        e_init_sound();
    switch(choice) {
        
        
        case 0 : // scale and possibilities
                    freq_c(2);freq_d(2);freq_e(2);freq_f(2);freq_g(2);freq_a(2);freq_b(2);freq_c1(4);
                    freq_rest(1);
                    freq_c1(2);freq_b(2);freq_a(2);freq_g(2);freq_f(2);freq_e(2);freq_d(2);freq_c(4);
                    freq_rest(1);
                    freq_c(2);freq_b0(2);freq_a0(2);freq_g0(2);freq_f0(2);
                    freq_rest(1);
                    freq_c1(2); freq_d1(2); freq_e1(2);
                    freq_rest(2);
                    break;      

                    
        case 1 : // scale 2 and possibilities
            freq_do_(2);freq_re(2);freq_mi(2);freq_fa(2);freq_sol(2);freq_la(2);freq_si(2);freq_do1(4);
            freq_rest(1);
            freq_do1(2);freq_si(2);freq_la(2);freq_sol(2);freq_fa(2);freq_mi(2);freq_re(2);freq_do_(4);
            freq_rest(1);
            freq_do_(2);freq_si0(2);freq_la0(2);freq_sol0(2);freq_fa0(2);
            freq_rest(1);
            freq_do1(2); freq_re1(2); freq_mi1(2);
            freq_rest(2);
            break; 

            
        case 2 : // daft punk "veridis quo"

            for(i=0; i<2; i++){
                freq_fa(1);freq_mi(1);freq_fa(1);freq_re(3);    
                    freq_fa(1);freq_do_(1);freq_fa(1);freq_si0(9);
                freq_mi(1);freq_re(1);freq_mi(1);freq_do_(3);   
                    freq_mi(1);freq_si0(1);freq_mi(1);freq_la0(9);
            }

            freq_la(1);freq_sol(1);freq_la(1);freq_fa(3);   
                freq_la(1);freq_mi(1);freq_la(1);freq_re(9);
            freq_sol(1);freq_fa(1);freq_sol(1);freq_mi(3);  
                freq_sol(1);freq_re(1);freq_sol(1);freq_do_(9);

            freq_fa(1);freq_mi(1);freq_fa(1);freq_re(3);    
                freq_fa(1);freq_do_(1);freq_fa(1);freq_si0(9);
            freq_mi(1);freq_re(1);freq_mi(1);freq_do_(3);   
                freq_mi(1);freq_si0(1);freq_mi(1);freq_la0(9);                    

            freq_rest(2);
            break;

            
        case 3 : // "frère Jacques" "Are you sleeping? (brother John")

            for(i = 0; i<2; i++){
                freq_do_(3);freq_re(3);freq_mi(3);freq_do_(3);
            }
            for(i = 0; i<2; i++){
                freq_mi(3);freq_fa(3);freq_sol(7);
            }
            for(i = 0; i<2; i++){
                freq_sol(1);freq_la(1);freq_sol(1);freq_fa(1);freq_mi(3);freq_do_(3);
            }
            for(i = 0; i<2; i++){
                freq_do_(3);freq_sol0(3);freq_do_(7);
            }                                
            freq_rest(2);
            break;

            
        case 4 : // usage of halt-tones
            // "star wars - The imperial March" improved (thanks to half-tones)             
            pause=0.125; // 1/8 works well just for a staccato
            freq_rest(pause);freq_mi(3);freq_rest(pause);freq_mi(3);freq_rest(pause);freq_mi(3);
            freq_do_(2);freq_rest(pause);freq_sol(1);freq_mi(3);
            freq_do_(2);freq_rest(pause); freq_sol(1);freq_mi(6);
            freq_rest(pause); freq_si(3);freq_rest(pause);freq_si(3);freq_rest(pause);freq_si(3);
            freq_do1(2); freq_rest(pause); freq_sol(1); freq_mi_bemol(3); // ask Celine -> in fact mi b
            freq_do_(2); freq_rest(pause); freq_sol(1); freq_mi(6);
            freq_rest(2);                
            break;     

            
        case 5 : // Digger game / "popcorn"
            for(i = 0; i<2; i++){
                freq_la(1);freq_sol(1);freq_la(1);freq_mi(1);freq_do_(1);freq_mi(1);freq_la0(3);
            }
            freq_la(1);freq_si(1);    
            freq_do1(1);freq_si(1);freq_do1(1);freq_la(1);
            freq_si(1);freq_la(1); freq_si(1); freq_sol(1);
            freq_la(1);freq_sol(1);freq_la(1);freq_sol(1);freq_la(3);
            break;

            
        case 6 : //Beethoven 5th   
            freq_mi(1); freq_mi(1);freq_mi(1);freq_do_(14);
            freq_re(1);freq_re(1);freq_re(1);freq_si0(14);

            freq_mi(1);freq_mi(1);freq_mi(1);freq_do_(1);
            freq_fa(1);freq_fa(1);freq_fa(1);freq_mi(1);
            freq_do1(1);freq_do1(1);freq_do1(1);freq_la(8);

            freq_mi(1);freq_mi(1);freq_mi(1);freq_si0(1);
            freq_fa(1);freq_fa(1);freq_fa(1);freq_mi(1);
            freq_re1(1);freq_re1(1);freq_re1(1);freq_si(8);

            freq_mi(1);freq_mi(1);freq_re(1);freq_do_(6);freq_re(1);freq_mi(1);
            freq_mi(1);freq_mi(1);freq_re(1);freq_do_(6);freq_re(1);freq_mi(1);

            freq_mi(1);freq_mi(1);freq_re(1);freq_do_(8);freq_la0(8);freq_mi(12);
            freq_rest(2);                                
            break;

            
        case 7: // Game of Thrones
            freq_mi(6);freq_la0(6);
                freq_do_(1);freq_re(1);freq_mi(3);freq_la0(5);
                freq_do_(1);freq_re(1);freq_si0(24);
            freq_re(6);freq_sol0(6);
                freq_do_(1);freq_si0(1);freq_re(3);freq_sol0(5);
                freq_do_(1); freq_rest(1); freq_do_(1); freq_si0(1);freq_la0(24);
            freq_rest(2);
            break;

            
        case 8: // Jingle Bells   
            freq_mi(1);freq_mi(1);freq_mi(3);
            freq_mi(1);freq_mi(1);freq_mi(3);
            freq_mi(1);freq_sol(1);freq_do_(1);freq_re(1);freq_mi(6);

            freq_fa(1);freq_fa(1);freq_fa(3);
            freq_rest(1); freq_sol(0.5);freq_fa(1);freq_mi(1);freq_mi(1);
            freq_mi(1);freq_mi(1);freq_re(1);freq_re(1);freq_mi(1);freq_re(3);freq_sol(6);                    
            break;

            
        case 9: // Mozart "Petite musique de nuit"                                        
            freq_do_(4);freq_sol0(1);freq_do_(4);freq_sol0(1);freq_do_(1);freq_sol0(1);freq_do_(1);freq_mi(1);freq_sol(6);
            freq_fa(4);freq_re(1);freq_fa(4);freq_re(1);freq_fa(1);freq_re(1);freq_si0(1);freq_re(1);freq_sol0(6);
            freq_do_(1);freq_rest(1);freq_do_(4);freq_mi(1);freq_re(1);freq_do_(1);freq_do_(1);freq_si0(4);freq_rest(1);
            freq_re(1);freq_fa(1);freq_si0(1);freq_re(1);freq_do_(4);freq_rest(1);
            freq_mi(1);freq_re(1);freq_do_(1);freq_do_(1);freq_si0(4);freq_rest(1);
            freq_re(1);freq_fa(1);freq_si0(1);freq_do_(6);
            freq_rest(2);
            break;

            
        case 10: // usage of half-tones
            // Chopin "Marche Funebre" Piano Sonata No. 2 in B-flat minor, Op. 35., 3rd mvt     
            freq_la0(5);  freq_la0(4);  
                freq_la0(1); freq_la0(5); 
            freq_do_(4);  freq_si0(1);  
                freq_si0(3);  
            freq_la0(1); freq_la0(3); 
                freq_sol0_dieze(1); freq_la0(5);
            freq_rest(4.);
            break;

            
        case 11: // usage  of halt-tones
            // Beethoven "Fuer Elise"
            freq_mi1(1);freq_re1_dieze(1);freq_mi1(1);freq_re1_dieze(1);
            freq_mi1(1);freq_si(1);freq_re1(1);freq_do1(1);freq_la(8);
            freq_do_(1);freq_mi(1);freq_la(1);freq_si(8);
            freq_mi(1); freq_sol_dieze(1); freq_si(1); freq_do1(8);
            freq_rest(4.);
            break;
  
            
        case 12: // Rocky Theme
            pause = 0.5;
            freq_mi(1); freq_rest(pause); freq_sol(3); freq_rest(pause); freq_la(8); 
                freq_la(1); freq_rest(pause); freq_si(3); freq_rest(pause); freq_mi(8); 
            freq_mi(1);  freq_rest(pause); freq_sol(3); freq_rest(pause); freq_la(8); 
                freq_la(1); freq_rest(pause); freq_si(3); freq_rest(pause);freq_mi(8); 

            freq_re(1); freq_rest(1); 
                freq_re(1);freq_do_(1);freq_re(2); freq_rest(1);
                freq_do_(1);freq_re(1);freq_mi(5); freq_rest(1);

            freq_do_(1); freq_rest(1); 
                freq_do_(1);freq_do_(1);freq_si0(1); freq_rest(1);
                freq_si0(1); freq_la0(2);  freq_rest(1); 
                freq_sol0(3); freq_rest(1);
                freq_fa(1);freq_mi(6);
            freq_rest(4.);
            break;
            
        default:
            // do_nothing
            break;                
        }
//    e_close_sound();
}


void freq_video_full_demo(){                 
    freq_rest(10);
    freq_beep1_3072(1);
    freq_rest(1);
    freq_beep2_2816(1);
    freq_rest(1);
    freq_beep_up(1);
    freq_rest(1);
    freq_beep_down(1);
    
    // wave_shapes
    freq_all_tones(1,0);
    set_sound_wave_shape(TRIANGLE);
    freq_all_tones(1,0);
    set_sound_wave_shape(SQUARE);
    freq_all_tones(1,0);
    set_sound_wave_shape(SAWTOOTH);
    freq_all_tones(1,0);

    set_sound_wave_shape(SINUS);
    
    // volume adjustable "on the fly"
    // default volume 80
    int i;
    int vol = 120;            
    for (i = 4; i > 0; i-- )
    {
        set_volume(vol);
        freq_all_tones(1,0);
        freq_rest(2);
        vol /= 2;
    }
    set_volume(80);

    freq_rest(2);                                    

    freq_chromatic_all_tones(1,0);

    freq_rest(2);
    freq_all_array_tones(1,0);

    freq_rest(2);

    freq_play_from_to(400,1000, 10);
    freq_play_from_to(1000,400,5);
    freq_play_from_to(400,1000, 5);
    freq_play_from_to(1000,400,2.5);
    freq_play_from_to(400,1000, 1);
    freq_play_from_to(1000,400,0.5);

    freq_rest(2);
    freq_tunes_examples(4);

    freq_rest(2);
    freq_tunes_examples(11);
            
}