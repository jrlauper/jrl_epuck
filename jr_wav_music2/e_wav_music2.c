/******************************************************************************* 
 * 
 * TONES MODULE - from pre-recorded sound
 * Version 2.0. May 2019. 
 * Jean-Roch LAUPER (unifr.ch)
 * 
 ******************************************************************************/

/* Please, see description in "music2.h"*/

#include "e_wav_music2.h"

#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_sound.h"
#include "e_led.h"


#include "e_prox.h"
#include "utility_mod3.h"


extern unsigned int e_dci_unavailable; // e_sound.c


#define STD_TONES_DURATION 1224
// WARNING! this duration is not ms! => this duration describes
// the number of lines in e_const_sound corresponding to the sound
// 1224 -> 170ms in audacity

#define STD_TONES_WAITING_DURATION 170
// 170 [ms]


#define STD_TONE_OFFSET 2880
// the line at which the tone start in .s
// in the current case, 0-2880 -> reserved for beep

#define STD_BEEP_DURATION 1440
#define STD_BEEP_WAITING_DURATION 150



// ########## BEEPS ##########

/* the choice of 3072 and 2816:
 * both are frequencies that are well recognized when played and listened by robot */

void beep1_3072(float std_percent) {       
    int i;
    int std_perc_integer = (int)std_percent;
    double std_perc_remain = std_percent - std_perc_integer;
    e_set_led(0,1);
    if (AUDIO_ON == 0) 
        e_init_sound();    
    
    for (i=0; i < std_perc_integer; i++) {
        e_play_sound(0, STD_BEEP_DURATION);
        while(e_dci_unavailable);        
    }
    
    if (std_perc_remain > 0) {
        e_play_sound(0, (int)(std_perc_remain*STD_BEEP_DURATION));
        while(e_dci_unavailable);
    }    
    e_close_sound();
    e_set_led(0,0);
}

void beep2_2816(float std_percent) {                    
    int i;
    int std_perc_integer = (int)std_percent;
    double std_perc_remain = std_percent - std_perc_integer;
    e_set_led(0,1);
    if (AUDIO_ON == 0) 
        e_init_sound();    
    
    for (i=0; i < std_perc_integer; i++) {
        e_play_sound(1440, STD_BEEP_DURATION);
        while(e_dci_unavailable);        
    }
    
    if (std_perc_remain > 0) {
        e_play_sound(1440, (int)(std_perc_remain*STD_BEEP_DURATION));
        while(e_dci_unavailable);
    }    
    e_close_sound();
    e_set_led(0,0);
    
    
}



// for the complete table of the tones, their frequency, 
// and their position in e_const_sound.s see the joinded pdf
// Beeps_and_A(880).pdf


// ########## TONES ##########

int music_led=1 ;

void set_music_led(int i){
    music_led=i;
}


void rest(float d){
    wait_jr((int)(STD_TONES_WAITING_DURATION*d));
}


void f0(int d){
    int i;
    for(i=0;i<d;i++) { 
        if (music_led) {
            e_set_body_led(1);
            e_set_led(4, 1);
        }        
        e_play_sound(STD_TONE_OFFSET + 0*STD_TONES_DURATION, STD_TONES_DURATION);
        while(e_dci_unavailable);	// wait until the DCI module stop playing
        // a waiting time is necessary for the sound to have the time to be played
        if (music_led) {
            e_set_led(4, 0);        
            e_set_body_led(0);
        }
    }
}

void fa0(int d){
    f0(d);
}


void g0(int d){
    int i;
    for(i=0;i<d;i++) {
        if (music_led) {
            e_set_body_led(1);
            e_set_led(5, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 1*STD_TONES_DURATION, STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(5, 0);        
            e_set_body_led(0);
        }
    }
}

void sol0(int d){
    g0(d);
}


void a0(int d){
    int i;
    for(i=0;i<d;i++) {
        if (music_led) {
            e_set_body_led(1);
            e_set_led(6, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 2*STD_TONES_DURATION,STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(6, 0);  
            e_set_body_led(0);
        }
    }
}

void la0(int d){
    a0(d);
}


void b0(int d){
    int i;
    for(i=0;i<d;i++) {
        if (music_led) {
            e_set_body_led(1);
            e_set_led(7, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 3*STD_TONES_DURATION,STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(7,0);
            e_set_body_led(0);
        }
    }
}

void si0(int d) {
    b0(d);
}


void c(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_led(1, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 4*STD_TONES_DURATION,STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(1,0);        
        }
    }
}

void do_(int d){
    c(d);
}

void d(int d){
    int i;
    for(i=0;i<d;i++) {       
        if (music_led) {
            e_set_led(2, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 5*STD_TONES_DURATION,STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(2,0); 
        }
    }
}

void re(int x){
    d(x);
}


void e(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_led(3, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 6*STD_TONES_DURATION,STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(3,0);        
        }
    }
}

void mi(int d){
    e(d);
}


void f(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_led(4, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 7*STD_TONES_DURATION,STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(4,0);        
        }
    }
}

void fa(int d){
    f(d);
}


void g(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_led(5, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 8*STD_TONES_DURATION, STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(5,0);        
        }
    }
}

void sol(int d){
    g(d);
}


void a(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_led(6, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 9*STD_TONES_DURATION, STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(6,0);        
        }
    }
}

void la(int d){
    a(d);
}


void b(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_led(7, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 10*STD_TONES_DURATION, STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(7,0);      
        }
    }
}

void si(int d){
    b(d);
}


void c1(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_front_led(1);
            e_set_led(1, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 11*STD_TONES_DURATION, STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(1,0);        
            e_set_front_led(0);
        }
    }
}

void do1(int d){
    c1(d);
}

void d1(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_front_led(1);
            e_set_led(2, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 12*STD_TONES_DURATION, STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(2,0);        
            e_set_front_led(0);
        }
    }
}

void re1(int d){
    d1(d);
}


void e1(int d){
    int i;
    for(i=0;i<d;i++) {        
        if (music_led) {
            e_set_front_led(1);
            e_set_led(3, 1);   
        }
        e_play_sound(STD_TONE_OFFSET + 13*STD_TONES_DURATION, STD_TONES_DURATION);
        while(e_dci_unavailable);
        if (music_led) {
            e_set_led(3,0);        
            e_set_front_led(0);
        }
    }
}

void mi1(int d){
    e1(d);
}

// ########## TONES end ##########


// ########## SMALL demos functions ########

void all_tones(int note_duration, float rest_duration)
{
    
    if (AUDIO_ON == 0) 
        e_init_sound();
    // note_duration : multiplier of 
    e_led_clear();
    set_music_led(1);          
    
    f0(note_duration);     
        rest(rest_duration);
    g0(note_duration);                
        rest(rest_duration);
    a0(note_duration);        
        rest(rest_duration);
    b0(note_duration);        
        rest(rest_duration);
    c(note_duration);        
        rest(rest_duration);
    d(note_duration);        
        rest(rest_duration);
    e(note_duration);        
        rest(rest_duration);
    f(note_duration);        
        rest(rest_duration);
    g(note_duration);        
        rest(rest_duration);
    a(note_duration);        
        rest(rest_duration);
    b(note_duration);        
        rest(rest_duration);
    c1(note_duration);        
        rest(rest_duration);
    d1(note_duration);        
        rest(rest_duration);
    e1(note_duration);     
    e_close_sound();
}

// "ir piano"
// plays tones when we pass the finger before the IR sensors
// to use it you need have made the necessary initialization : 
//          e_init_ad_scan(ALL_ADC);
//          e_calibrate_ir();
void ir_piano_C_to_C1()
{   
    if (AUDIO_ON == 0) 
    e_init_sound();
    e_led_clear();
    set_music_led(0);
    // the led lit won't be "the official" one 
    // it is more close to the sensor triggred
    e_set_front_led(0);
    if (e_get_calibrated_prox(1)>400) {      
        e_set_led(1,1);
        c(1);
    }
    if (e_get_calibrated_prox(2)>400) { 
        e_set_led(2,1);
        d(1);
    }
    if (e_get_calibrated_prox(3)>400) {
        e_set_led(3,1);
        e(1);    
    }
    if (e_get_calibrated_prox(4)>400) {                    
        e_set_led(5,1);                                    
        f(1);
    }
    if (e_get_calibrated_prox(5)>400) {                                    
        e_set_led(6,1);                                        
        g(1);                    
    }
    if (e_get_calibrated_prox(6)>400) {                    
        e_set_led(7,1);                                    
        a(1);
    }
    if (e_get_calibrated_prox(7)>400) {                    
        e_set_led(0,1);                                    
        b(1);
    }
    if (e_get_calibrated_prox(0)>400) {                                        
        e_set_front_led(1);                    
        c1(1);
    }
}

// thanks to Celine Lauper for having helped me to find the tones composing the tunes
void tunes_examples(int choice) {
    int i;
    float pause;
    if (AUDIO_ON == 0) 
        e_init_sound();
    switch(choice) {
        case 0 : // scale and possibilities
                    c(2);d(2);e(2);f(2);g(2);a(2);b(2);c1(4);
                    rest(1);
                    c1(2);b(2);a(2);g(2);f(2);e(2);d(2);c(4);
                    rest(1);
                    c(2);b0(2);a0(2);g0(2);f0(2);
                    rest(1);
                    c1(2); d1(2); e1(2);
                    rest(2);
                    break;      

                case 1 : // scale 2 and possibilities
                    do_(2);re(2);mi(2);fa(2);sol(2);la(2);si(2);do1(4);
                    rest(1);
                    do1(2);si(2);la(2);sol(2);fa(2);mi(2);re(2);do_(4);
                    rest(1);
                    do_(2);si0(2);la0(2);sol0(2);fa0(2);
                    rest(1);
                    do1(2); re1(2); mi1(2);
                    rest(2);
                    break; 

                case 2 : // daft punk "veridis quo"

                    for(i=0; i<2; i++){
                        fa(1);mi(1);fa(1);re(2);    fa(1);do_(1);fa(1);si0(9);
                        mi(1);re(1);mi(1);do_(2);   mi(1);si0(1);mi(1);la0(9);
                    }

                    la(1);sol(1);la(1);fa(2);   la(1);mi(1);la(1);re(9);
                    sol(1);fa(1);sol(1);mi(2);  sol(1);re(1);sol(1);do_(9);

                    fa(1);mi(1);fa(1);re(2);    fa(1);do_(1);fa(1);si0(9);
                    mi(1);re(1);mi(1);do_(2);   mi(1);si0(1);mi(1);la0(9);                    
                    
                    rest(2);
                    break;

                case 3 : // "frère Jacques" "Are you sleeping? (brother John")

                    for(i = 0; i<2; i++){
                        do_(2);re(2);mi(2);do_(2);
                    }
                    for(i = 0; i<2; i++){
                        mi(2);fa(2);sol(4);
                    }
                    for(i = 0; i<2; i++){
                        sol(1);la(1);sol(1);fa(1);mi(2);do_(2);
                    }
                    for(i = 0; i<2; i++){
                        do_(2);sol0(2);do_(4);
                    }                                
                    rest(2);
                    break;

                case 4 : // "star wars - The imperial March"              
                    pause=1.0/8; // 1/8 works well just for a staccato
                    rest(pause);mi(3);rest(pause);mi(3);rest(pause);mi(3);
                    do_(2);sol(1);mi(3);
                    do_(2);sol(1);mi(6);
                    rest(pause); si(3);rest(pause);si(3);rest(pause);si(3);
                    do1(2); sol(1); mi(3); // ask Celine -> in fact mi b
                    do_(2); sol(1); mi(6);
                    rest(2);                
                    break;     

                case 5 : // Digger game / "popcorn"
                    for(i = 0; i<2; i++){
                        la(1);sol(1);la(1);mi(1);do_(1);mi(1);la0(2);
                    }
                    la(1);si(1);    
                    do1(1);si(1);do1(1);la(1);
                    si(1);la(1); si(1); sol(1);
                    la(1);sol(1);la(1);sol(1);la(2);
                    break;

                case 6 : //Beethoven 5th
                    pause = 1.0/16;
                    mi(1); rest(pause);mi(1);rest(pause);mi(1);do_(11);
                    re(1);rest(pause);re(1);rest(pause);re(1);si0(11);

                    mi(1);rest(pause);mi(1);rest(pause);mi(1);do_(1);
                    fa(1);rest(pause);fa(1);rest(pause);fa(1);mi(1);
                    do1(1);rest(pause);do1(1);rest(pause);do1(1);la(5);

                    mi(1);rest(pause);mi(1);rest(pause);mi(1);si0(1);
                    fa(1);rest(pause);fa(1);rest(pause);fa(1);mi(1);
                    re1(1);rest(pause);re1(1);rest(pause);re1(1);si(5);

                    mi(1);rest(pause);mi(1);re(1);do_(3);re(1);mi(1);
                    mi(1);rest(pause);mi(1);re(1);do_(3);re(1);mi(1);

                    mi(1);rest(pause);mi(1);re(1);do_(4);la0(4);mi(11);
                    rest(2);                                
                    break;

                case 7: // Game of Thrones
                    mi(6);la0(6);do_(1);re(1);mi(4);la0(4);do_(1);re(1);si0(24);
                    re(6);sol0(6);do_(1);si0(1);re(4);sol0(4);do_(1);si0(1);la0(24);
                    rest(2);
                    break;

                case 8: // Jingle Bells
                    pause = 1.0/4;
                    mi(1);rest(pause);mi(1);rest(pause);mi(2);rest(1);
                    mi(1);rest(pause);mi(1);rest(pause);mi(2);rest(1);
                    mi(1);sol(1);do_(1);re(1);mi(3);rest(1);
                   
                    fa(1);rest(pause);fa(1);rest(pause);fa(2);rest(1);
                    sol(1);fa(1);mi(1);rest(pause);mi(1);rest(1);
                    mi(1);rest(pause);mi(1);rest(pause);re(1);rest(pause);re(1);mi(1);re(3);sol(4);
                    rest(2);
                    break;

                case 9: // Mozart "Petite musique de nuit"                                        
                    do_(3);sol0(1);do_(3);sol0(1);do_(1);sol0(1);do_(1);mi(1);sol(4);
                    fa(3);re(1);fa(3);re(1);fa(1);re(1);si0(1);re(1);sol0(4);
                    do_(1);rest(1);do_(3);mi(1);re(1);do_(1);do_(1);si0(3);rest(1);
                    re(1);fa(1);si0(1);re(1);do_(3);rest(1);
                    mi(1);re(1);do_(1);do_(1);si0(3);rest(1);
                    re(1);fa(1);si0(1);do_(4);
                    rest(2);
                    break;

                default:
//                    do_nothing();
                    break;                
        }
    e_close_sound();

}