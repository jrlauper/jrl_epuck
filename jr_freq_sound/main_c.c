/* Jean-Roch LAUPER, unifr.ch, 2019/2020 */


#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_sound.h"
#include "e_led.h"
// this is the simples led library among the three possible ones
// but you can of course change it without issue if you want to use a more complexe one

#include "e_ad_conv.h"
// for battery_display()  / not necessary for playing sound

#include "e_prox.h"
// for ir_piano / not necessary for sound playing

#include "e_freq_sound.h"
#include "utility_mod3.h"



int main() {
    
    char main_buffer[100];  
    int selector;
    int i;
    
    e_init_port();      // configure port pins
                                        
    //system initialization 
	e_init_port();    	// configure port pins
    e_init_sound();    

    e_init_ad_scan(ALL_ADC); 
        /* here, necessary for 
         *      - battery_display() and
         *      - ir_piano() (proximity sensors)
         * not necessary to play sound*/
    
    e_set_led(8,1); // 8 --> all the lights on
    wait_jr(100);                
    e_calibrate_ir(); // for ir_piano() only               
    e_led_clear();
    
    
    while (1) {    
        selector = getselector();
        
        switch (selector){
            
            case 15:
                battery_display();
                wait_jr(500);
                break;
                
            case 14: // play all the tunes one after the other                  
                for (i = 1; i<=12; i++)
                    freq_tunes_examples(i);
                    wait_jr(500);               
                break;
            
            case 0:
                freq_video_full_demo();
                break;
            
            case 1:
                freq_ir_piano_C_to_C1();
                break;
            
            case 2: // daft punk "veridis quo"                
                freq_tunes_examples(2);
                break;
                
            case 3: // pop-corn
                freq_tunes_examples(5);
                break;
            
            case 4: // star wars
                freq_tunes_examples(4);
                break  ;                                          
            
            default:
                ;            
        } // switch end
      
        
    } // end while(1))
	return 0;
}

