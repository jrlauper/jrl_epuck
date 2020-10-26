/************************************************************************/
/** Sound Test **
/** JRL, Spring 2019   **
 * 
 * Tones identifications
 * 
 * 
 * 
************************************************************************/



#include "e_epuck_ports.h"
#include "e_init_port.h"

#include "e_led.h"
#include "e_uart_char.h"

#include "e_ad_conv.h"
#include "utility_mod3.h"

#include "runfftlistener_mod.h" //source: demo EPuck
#include "utility_mod3.h"
    

#define PI 3.14159265358979




void do_nothing(){
    ;
}


int main() {
    int selector = 0;
    // char buffer[100];    
    e_init_uart1();				// initialize UART to 115200 Kbaud                           
    e_init_port();    			// configure port pins

    // e_init_ad_scan(ALL_ADC);	// configure Analog-to-Digital Converter Module
    // ad initialization done in the run_fft_... : MICRO_ONLY
    	    
    while(1) {        

        selector = getselector();
        
        switch (selector) {    
            case 15:
                e_init_ad_scan(ALL_ADC);
                battery_display();
                wait_jr(500);
                break;
            
            case 0: // 
                    run_fft_listener();
                    break;
            case 1: 
                    run_fft_listener_comm();
                    break;
            case 2: 
                    run_fft_listener_comm_stat();
                    break;
            // additions while writing report (january 2020)

            default: do_nothing();
                    break;            
                    
        }  //switch end
        
        
       
        
    } // end while1
    
    
}
