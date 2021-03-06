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
#define Black_Middle 250 // black threshold, upper limit for track
#define Black_Right 300

#define Black 490// threshold for barcode
#define Red 700
#define White 710

 #define Speed 370 // speed off motors at "full" speed
#define Pause 0 // speed for stop
//volatile unsigned int state;//variable used to assign direction
volatile int Left;
volatile int Right;
volatile int Middle;
volatile int Bar;


int main(void)
{
    TRISBbits.TRISB5=1;//enables the SW1 as input

//    CNEN2bits.CN27IE = 1;//change notification for SW1
//    IFS1bits.CNIF = 0;//enables the change notification interrupt
//    IEC1bits.CNIE = 1;//sets flag down
    int state=0;
    int redcnt=0;
    int blackcnt=0;
    int pos = -1;
    

   
 
//    state=0;//intializes in iddle state before going forward
     Left = 0;
     Right = 0;
     Middle=0;
     Bar =0;

   
    
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
   LCDClear();
   while(PORTBbits.RB5 == 1);

   OC1RS = Speed;
   OC2RS = Speed;

   RPOR4bits.RP8R= 19;//input 1 red left motor
   RPOR5bits.RP10R = 0;//input 2 black left motor

   RPOR0bits.RP0R = 18;//input 4 red right motor
   RPOR0bits.RP1R = 0;//input 3 black left motor
   

 
   
    while(1){

        //Clears the LCD screen, and displays the ADC value on the 1st line
//         LCDClear();
       //  value = AnalogtoDigital();
        Left = ADCLeft();
        Right = ADCRight();
        Middle = ADCCenter();
        Bar = ADCBarcode();
//              LCDClear();
//         LCDMoveCursor(1,0);
////
//         sprintf(ADV,"%4.0d",Left);
//         LCDPrintString(ADV);
//
         LCDMoveCursor(1,4);
         sprintf(ADV2,"%4.0d",Bar);
         LCDPrintString(ADV2);
//
//         LCDMoveCursor(0,0);
//         sprintf(ADV3,"%4.0d",Middle);
//         LCDPrintString(ADV3);
//        state=Barcode(Right, state);
        Calibrate(Left,Middle, Right);



         switch(state){
             case 0: //idle
                 if(Bar < Black)state = 1;
                 break;

             case 1: // start bit
                 if(Bar > White) state =2;
                 break;

             case 2: // white state
                 if(Bar < White) state =3;
                 break;

             case 3: // bit
                 state=3;
                 if(Bar > Black && Bar < Red) redcnt = 1;
                 else if(Bar < Black) blackcnt = 1;


                 else if(Bar > White){
                     state = 4;
                     pos++;
                 }
                 break;

             case 4: // print state
                 //LCDMoveCursor(0,pos);
                 if(redcnt ==1 && blackcnt ==0){
                     ADV3[pos] = '1';
                    // sprintf(ADV3,"%4.0d",redcnt);
                    // LCDPrintString(ADV3);
                 }

                 else if(redcnt ==1 && blackcnt ==1){
                     ADV3[pos] = '0';
                    // sprintf(ADV3,"%4.0d",(redcnt-1));
                    //LCDPrintString(ADV3);
                 }

                 if(pos == 3){
                     state = 0;
                     pos =-1; 
                     redcnt = 0;
                     blackcnt =0;
                     LCDMoveCursor(0,0);
                     //sprintf(ADV3,"%4.0d",ADV3);
                     LCDPrintString(ADV3);
                 }
                 else{
                     state = 2;
                     redcnt = 0;
                     blackcnt = 0;
                 }



         }

//        switch(state){
//            case 0://idle
//                if(Right<=Black && count==0){
//                    state=1;
//                    count=1;
//                }
//                else{
//                    count=0;
//                    red=0;
//                    black=0;
//                }
//
//                break;
//
//            case 1://waiting for white
//                if(Right>Red && Right<=White){
//                    state=2;
//                    check=0;
//                }
//                break;
//
//            case 2://determining if it is black or red
//                if(Right<=Black && check==0){
//                    black=1;
//                    check=1;
//                }
//                if(Right>Black && Right<=Red && check==0){
//                    red=1;
//                    check=1;
//                }
//                if(check>=1){
//                    if(Right>Black && Right<=Red)
//                        red++;
//                    if(Right<=Black)
//                        black++;
//                    if((black>=10)| (red>=10)){
//                        state=3;
//                    }
//                }
//                break;
//
//            case 3://printing the corresponding number
//                if(red>black){
//                    count++;
//                    LCDMoveCursor(0,count-1);
//                    LCDPrintChar('1');
//                    state=4;
//                }
//                else if(black>red){
//                    count++;
//                    LCDMoveCursor(0,count-1);
//                    LCDPrintChar('0');
//                    state=4;
//                }
//                break;
//
//            case 4://checking if all 4 numbers were printed
//                if(count==5){
//                    state=0;
//                }
//                else{
//                    state=1;
//                    red=0;
//                    black=0;
//                    check=0;
//                    count=1;
//                }
//                break;
//
//        }
//
//   }
}
}