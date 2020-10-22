/********************************************************************************

			Initialization of port of e-puck
			Version 1.0 november 2005
			Michael Bonani, Francesco Mondada, Davis Dadie


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2005-2007 Michael Bonani, Francesco Mondada, Davis Dadie

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup motor_LED
 * \brief Initialize the ports on standard configuration.
 * \author Code: Michael Bonani, Francesco Mondada, Davis Dadie \n Doc: Jonathan Besuchet
 */

#include "e_epuck_ports.h"
//#include "../I2C/e_I2C_protocol.h"
//#include "../acc_gyro/e_lsm330.h"
#include "e_I2C_protocol.h"
#include "e_lsm330.h"

unsigned char isPresentFlag = 0;

///* set configuration bit for MPLAB
//!!!remove this macros if you have want different configuration bit!!!*/
//_FOSC(CSW_FSCM_OFF & XT_PLL8);
//_FWDT(WDT_OFF);
//_FBORPOR(PBOR_OFF & MCLR_EN);
//_FGS(CODE_PROT_OFF);

/* set configuration bit for MPLAB
!!!remove this macros if you have want different configuration bit!!!*/


/* *********************** JRL new Configuration Bits Settings ************** */
/* in MPLab -> Production -> set configuration bits
 *  possibility of exporting them
 * -> clearer than the initial above */


// DSPIC30F6014A Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FOSFPR = XT_PLL8         // Oscillator (XT w/PLL 8x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF             // Watchdog Timer (Enabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = NONE            // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_OFF         // PBOR Enable (Disabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FBS
#pragma config BWRP = WR_PROTECT_BOOT_OFF// Boot Segment Program Memory Write Protect (Boot Segment Program Memory may be written)
#pragma config BSS = NO_BOOT_CODE       // Boot Segment Program Flash Memory Code Protection (No Boot Segment)
#pragma config EBS = NO_BOOT_EEPROM     // Boot Segment Data EEPROM Protection (No Boot EEPROM)
#pragma config RBS = NO_BOOT_RAM        // Boot Segment Data RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WR_PROT_SEC_OFF   // Secure Segment Program Write Protect (Disabled)
#pragma config SSS = NO_SEC_CODE        // Secure Segment Program Flash Memory Code Protection (No Secure Segment)
#pragma config ESS = NO_SEC_EEPROM      // Secure Segment Data EEPROM Protection (No Segment Data EEPROM)
#pragma config RSS = NO_SEC_RAM         // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = GSS_OFF            // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

/* ************************************************************************** */




void testAccGyroPresence() {
    unsigned char ret;
    e_i2cp_init();
    e_i2c_reset();
    e_i2cp_enable();
    ret = e_i2cp_read(0x3C, 0x0F); // read WHO_AM_I register of accelerometer
    e_i2cp_disable();
    if (ret == 0x40) {
        isPresentFlag = 1;
    } else {
        isPresentFlag = 0;
    }
}

/*! \brief Initialize all ports (in/out)
 *
 * Call this method to set all the standards output
 * components (LEDs, IR, camera, motors, I2C, audio) on
 * their defaults values and set their corresponding PIN
 * to "output".
 * The method also set the corresponding PIN to "input"
 * for all the standards inputs components
 * (IR receiver, selector, camera, battery level).
 */
void e_init_port(void) {
   
    /********************** OUTPUTS ***************************/
    /*LEDS*/
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    LED0_DIR = OUTPUT_PIN;
    LED1_DIR = OUTPUT_PIN;
    LED2_DIR = OUTPUT_PIN;
    LED3_DIR = OUTPUT_PIN;
    LED4_DIR = OUTPUT_PIN;
    LED5_DIR = OUTPUT_PIN;
    LED6_DIR = OUTPUT_PIN;
    LED7_DIR = OUTPUT_PIN;

    FRONT_LED = 0;
    FRONT_LED_DIR = OUTPUT_PIN;

    BODY_LED = 0;
    BODY_LED_DIR = OUTPUT_PIN;

    /*IR*/
    PULSE_IR0 = 0;
    PULSE_IR1 = 0;
    PULSE_IR2 = 0;
    PULSE_IR3 = 0;
    PULSE_IR0_DIR = OUTPUT_PIN;
    PULSE_IR1_DIR = OUTPUT_PIN;
    PULSE_IR2_DIR = OUTPUT_PIN;
    PULSE_IR3_DIR = OUTPUT_PIN;

    /*basic audio*/
    AUDIO_ON = 0; /*turn of speaker and codec*/
    AUDIO_ON_DIR = OUTPUT_PIN;

    /*motors*/
    MOTOR1_PHA = 0;
    MOTOR1_PHB = 0;
    MOTOR1_PHC = 0;
    MOTOR1_PHD = 0;
    MOTOR2_PHA = 0;
    MOTOR2_PHB = 0;
    MOTOR2_PHC = 0;
    MOTOR2_PHD = 0;
    MOTOR1_PHA_DIR = OUTPUT_PIN;
    MOTOR1_PHB_DIR = OUTPUT_PIN;
    MOTOR1_PHC_DIR = OUTPUT_PIN;
    MOTOR1_PHD_DIR = OUTPUT_PIN;
    MOTOR2_PHA_DIR = OUTPUT_PIN;
    MOTOR2_PHB_DIR = OUTPUT_PIN;
    MOTOR2_PHC_DIR = OUTPUT_PIN;
    MOTOR2_PHD_DIR = OUTPUT_PIN;

    /*camera*/
    CAM_RESET = 0;
    CAM_RESET_DIR = OUTPUT_PIN;

    /*I2C*/
    SIO_C = 0;
    SIO_D = 0;

    SIO_C_DIR = OUTPUT_PIN;
    SIO_D_DIR = OUTPUT_PIN;

    /********************** INPUTS **************************/

    /*low battery signal active low when Vbatt<3.4V*/
    BATT_LOW_DIR = INPUT_PIN;

    /*IR TV receiver on normal extension*/
    REMOTE_DIR = INPUT_PIN;

    /* selector*/
    SELECTOR0_DIR = INPUT_PIN;
    SELECTOR1_DIR = INPUT_PIN;
    SELECTOR2_DIR = INPUT_PIN;
    SELECTOR3_DIR = INPUT_PIN;

    /*camera*/
    CAM_y0_DIR = INPUT_PIN;
    CAM_y1_DIR = INPUT_PIN;
    CAM_y2_DIR = INPUT_PIN;
    CAM_y3_DIR = INPUT_PIN;
    CAM_y4_DIR = INPUT_PIN;
    CAM_y5_DIR = INPUT_PIN;
    CAM_y6_DIR = INPUT_PIN;
    CAM_y7_DIR = INPUT_PIN;

    testAccGyroPresence();
    // JRL : if there is a Giro -> we know that this is a e-puck HWRev 1.3
    // as there wasn't any Gyro before
    if (isPresentFlag) {
        initAccAndGyro();
    }
}

// the isPresentFlag -> is decided by the presence or not of a gyroscope
unsigned char isEpuckVersion1_3(void) {
    return isPresentFlag;
}

