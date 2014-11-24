// ******************************************************************************************* //

#include "p24fj64ga002.h"

#include "lcd.h"

#include "PWM.h"

// ******************************************************************************************* //


void InPWM(){//initializes the PWM
        //sets all pins connected to the two motors to null
            RPOR1bits.RP3R = 0;
            RPOR1bits.RP2R = 0;
            RPOR0bits.RP0R = 0;
            RPOR0bits.RP1R = 0;
//sets timer 3 which is used for the output compare 1
    PR3 = 512;
    T3CON = 0x8000;
     OC1CON = 0x000E;
    OC1R= 0;
    OC1RS = 0;

//sets timer 2 which is used for the output compare 2
    PR2 = 512;
    T2CON = 0x8000;

    OC2CON = 0x0006;
    OC2R = 0;
    OC2RS = 0;
    

}


// ******************************************************************************************* //


void MoveRobot(int state){
    //19 output compare 2
    //18 output compare 1
    //0  output  "NULL"

    //idle mode
    switch(state){
    case 0:
            RPOR1bits.RP3R = 0;
            RPOR1bits.RP2R = 0;
            RPOR0bits.RP0R = 0;
            RPOR0bits.RP1R = 0;
    break;
    
    case 1:
    //forward , clockwise motion of wheels
            RPOR1bits.RP3R= 0;//input 1
            RPOR1bits.RP2R = 19;//input 2 left motor
            RPOR0bits.RP0R = 0;//input 4
            RPOR0bits.RP1R = 18;//input 3
    break;

     
    case 2:
         //idle mode
            RPOR1bits.RP3R= 0;
            RPOR1bits.RP2R = 0;
            RPOR0bits.RP0R = 0;
            RPOR0bits.RP1R = 0;

    break;
   
   case 3:
        //backward, counter clockwise motion of wheels
            RPOR1bits.RP3R= 19;//input 1   connected to left motor
            RPOR1bits.RP2R = 0;//input 2
            RPOR0bits.RP0R = 18;//input 4 connected to right motor
            RPOR0bits.RP1R = 0;//input 3

    break;


}
}



