/************************************************************************/
/** Selector-Tone **/
/** JRL, Spring 2019   **/

/* play one of the 14 tones for each selector
 * I use the codec directly like this 
 * -> selector on 0 -> plays segment 0 of e_const_sound.s
 * 
/************************************************************************/

#include "e_init_port.h"
#include "e_epuck_ports.h"
#include "e_wav_music2.h"

#include "e_sound.h"
#include "e_led.h"

#include "e_ad_conv.h" // for ir prox
#include "e_prox.h"
#include "utility_mod3.h"




/************************************************************************/

int main() {
    int selector = 0;
    int i = 0;
   
    e_init_port();        

    e_init_sound();    
    e_led_clear();
    
    // for ir piano
    e_init_ad_scan(ALL_ADC);
    
    e_set_led(8,1); // 8 --> all the lights on
    wait_jr(100);                
    e_calibrate_ir();            
    e_led_clear();
        
    
    while(1) {
        selector = getselector();
        e_led_clear();
        e_set_body_led(0);
        set_music_led(1);
              
        if(selector <= 9)
            tunes_examples(selector);
        else if (selector == 10) {
            beep1_3072(1.5);            
            rest(1);
            beep1_3072(1.25);
            rest(1);
            beep1_3072(1);
            rest(1);
            beep1_3072(0.75);
            rest(1);
            beep1_3072(0.5);
            rest(1);
            beep1_3072(0.25);
            rest(1);
        } else if (selector == 11) {
            beep2_2816(1.5);            
            rest(1);
            beep2_2816(1.25);
            rest(1);
            beep2_2816(1);
            rest(1);
            beep2_2816(0.75);
            rest(1);
            beep2_2816(0.5);
            rest(1);
            beep2_2816(0.25);
            rest(1);
        } else if (selector == 12) {
            ;
        } else if (selector == 13) {
            all_tones(2,1);
            wait_jr(500);
        } else if (selector == 14){
            ir_piano_C_to_C1();
        } else if (selector == 15){
            battery_display();
            wait_jr(500);
        }
        

           
        }            
//    }
    
    e_close_sound();
    
}
