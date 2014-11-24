// ******************************************************************************************* //

// BY : Team 205: Jesus Chavolla, Uriel Garcia , Joshua Hurley

///////////////////////////////////////////////////////////////////////////////////////////////
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"
#include "ADC.h"
#include "PWM.h"


// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.


_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
		 BKBUG_ON & COE_ON & ICS_PGx1 &
		 FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
		 IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* /
/*
VSS is connected to 9V battery and VS is connected to 5V on the board
pin 3 on H-bridge OUTPUT 1 is connected to RED >> LEFT motor
pin 6 on H-bridge OUTPUT 2 is connect ed to BLACK >> LEFT motor
pin 14 on H-bridge OUTPUT 3 is connected to RED >> RIGHT motor
pin 11 on H-bridge OUTPUT 4 is connected to BLACK >> RIGHT motor

SW1 is RB5 and is used to transition from IDLE->forward->IDLE->Backward->IDLE....etc.
 
JP5 ( potentiometer in board) is  connected to pin 2 and is configured as analog inputs
pin 7 is RP 3 connected to pin 2 on H-bridge (input 1)  controls counter-clockwise for left motor
pin 6 is RP 2 connected to pin 7 on H-bridge  (input 2) controls clockwise for left motor
pin 4 is RP 0 connected to pin 15 on H-bridge (input 3) controls counter-clockwise for right motor
pin 5 is RP 1 connected to pin 10 on H-bridge  (input 4)controls clockwise for right motor
********************************************************************/
// ******************************************************************************************* //
 #define Black_Left 300
#define Black_Middle 250 // black threshold, upper limit
#define Black_Right 300
 #define Speed 420 // speed off motors at "full" speed
#define Pause 0 // speed for stop
//volatile unsigned int state;//variable used to assign direction
volatile int Left;
volatile int Right;
volatile int Middle;


int main(void)
{
    TRISBbits.TRISB5=1;//enables the SW1 as input

//    CNEN2bits.CN27IE = 1;//change notification for SW1
//    IFS1bits.CNIF = 0;//enables the change notification interrupt
//    IEC1bits.CNIE = 1;//sets flag down


   
 
//    state=0;//intializes in iddle state before going forward
     Left = 0;
     Right = 0;
     Middle=0;

   
    
    char ADV[4];//used to print the ADC value
    char ADV2[4];//used to print the duty cycles
    char ADV3[4];

    	PR4 = 57;//1 micro second delay
	TMR4 = 0;//resets timer 1
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
	T4CONbits.TCKPS = 3;
	T4CONbits.TON = 1;

   
    
//initializes LCD, the ADC and the PWM
     LCDInitialize();
    InADC();
   InPWM();

   TRISBbits.TRISB5 = 1;
   while(PORTBbits.RB5 == 1);

   OC1RS = Speed;
   OC2RS = Speed;

   RPOR4bits.RP8R= 19;//input 1 red left motor
   RPOR1bits.RP2R = 0;//input 2 black left motor

   RPOR0bits.RP0R = 18;//input 4 red right motor
   RPOR0bits.RP1R = 0;//input 3 black left motor
   

 
   
    while(1){

        //Clears the LCD screen, and displays the ADC value on the 1st line
//         LCDClear();
       //  value = AnalogtoDigital();
        Left = ADCLeft();
        Right = ADCRight();
        Middle = ADCCenter();
       
              LCDClear();
         LCDMoveCursor(1,0);
//
         sprintf(ADV,"%4.0d",Left);
         LCDPrintString(ADV);

         LCDMoveCursor(1,4);
         sprintf(ADV2,"%4.0d",Right);
         LCDPrintString(ADV2);

         LCDMoveCursor(0,0);
         sprintf(ADV3,"%4.0d",Middle);
         LCDPrintString(ADV3);

        Calibrate(Left,Middle, Right);

   }
}
//    void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
//        //change interrupt for SW1
//
//
//        IFS1bits.CNIF = 0;//set flag down
//
//    }





