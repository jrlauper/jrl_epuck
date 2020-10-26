/************************************************************************/
/** Selector-Tone **/
/** JRL, Spring 2019   **/

/* play one of the 14 tones for each selector
 * I use the codec directly like this 
 * -> selector on 0 -> plays segment 0 of e_const_sound.s
 * 
/************************************************************************/

#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_music_comm.h"

#include "e_motors.h"
#include "e_sound.h"
#include "e_led.h"


void do_nothing();
int getselector();

void all_tones(int note_duration, float rest_duration);
void all_tones_com(int note_duration, float rest_duration);


/* -------------------------------------------------------------------------  */

void do_nothing(){
    ;
}


int getselector() {
    return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}


void all_tones(int note_duration, float rest_duration)
{
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
}


/************************************************************************/

// like all-tones minus some notes -> attempt to have good 
// tones recognition between robots while still having a good amount of tones
// (as the comm is quite slow)

// 1st attempt: 14.06.2019 : suppress G0 and A
void all_tones_com(int note_duration, float rest_duration)
{
    // note_duration : multiplier of 
    e_led_clear();
    set_music_led(1);          
    
    f0(note_duration);     
        rest(rest_duration);
//    g0(note_duration);                
//        rest(rest_duration);
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
//    a(note_duration);        
//        rest(rest_duration);
    b(note_duration);        
        rest(rest_duration);
    c1(note_duration);        
        rest(rest_duration);
    d1(note_duration);        
        rest(rest_duration);
    e1(note_duration);       
}

/* ************************************************************************** */

int main() {
    int selector = 0;
    int i = 0;    
    e_init_port();        
    e_init_sound();    
    set_music_led(1);
               
    while(1) {
        selector = getselector();
        e_led_clear();
         
        if (selector == 0) {
            f0(1);            
        } else if (selector == 1) {
            g0(1);        
        } else if (selector == 2) {
            a0(1);
        } else if (selector == 3) {
            b0(1);
        } else if (selector == 4) {
            c(1);
        } else if (selector == 5) {
            d(1);
        } else if (selector == 6) {
            e(1);
        } else if (selector == 7) {
            f(1);
        } else if (selector == 8) {
            g(1);
        } else if (selector == 9) {
            a(1);
        } else if (selector == 10) {
            b(1);
        } else if (selector == 11) {            
              all_tones(2, 4);
              wait_jr(1000);     
//            c1(1);            
        } else if (selector == 12) {
            all_tones(2, 0);
            wait_jr(1000);    
//            d1(1);    
        } else if (selector == 13) {
            all_tones(1, 4);
            wait_jr(1000);    
//            e1(1);                            
        } else if (selector == 14) {
            all_tones(1, 0);        
            wait_jr(1000);                
        } else if (selector == 15) {
//            all_tones_com(2,4);
//            wait_jr(1000);     
            // to flash w/o sound :-) 
        }
        else {
            do_nothing();                        
            
           
        }            
    }
    
    e_close_sound();
    
}
