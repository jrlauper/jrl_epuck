/******************************************************************************* 
 * 
 * "MUSIC" Module - Communication between Robots version
 * (the e_sound_const.s file is not tones anymore but sounds that are well recognizable
 * via FFT)
 * Version 1.0. Spring 2019. 
 * Jean-Roch LAUPER (unifr.ch)
 * 
 ******************************************************************************/

/* Please, see description in "music.h"*/

#include "e_music_comm.h"
#include "e_epuck_ports.h"
#include "e_init_port.h"
#include "e_sound.h"
#include "e_led.h"

#include "e_music_comm.h"


int music_led=1 ;



#define DELAY1 29
#define DELAY2 64
// makes roughly 1ms together

void wait_jr(int x) {
    int i, j, k ;
    for (i = 0; i < DELAY2; i++){
        for (j = 0; j < DELAY1; j++) {            
            for (k = 0; k < x; k++);
        }
    }
}

void set_music_led(int i){
    music_led=i;
}

void rest(float d){
    wait_jr((int)(d*STD_WAITING_DURATION));
}

// for the complete table of the tones, their frequency, 
// and their position in e_const_sound.s see the joinded pdf
// FGA(880)B_CDEFGABC_DE_table.pdf


// ########## TONES ##########


void f0(int d){
    int i;
    for(i=0;i<d;i++) { 
        if (music_led) {
            e_set_body_led(1);
            e_set_led(4, 1);
        }        
        e_play_sound(0*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(1*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(2*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(3*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(4*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(5*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(6*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(7*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(8*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(9*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(10*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(11*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(12*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
        e_play_sound(13*STD_TONES_DURATION + STD_TONES_OFFSET, STD_TONES_DURATION);
        wait_jr(STD_WAITING_DURATION);
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
