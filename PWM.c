// ******************************************************************************************* //

#include "p24fj64ga002.h"

#include "lcd.h"

#include "PWM.h"

// ******************************************************************************************* //
#define Black 180
#define Red 280
#define White 250

void InPWM(){//initializes the PWM
        //sets all pins connected to the two motors to null
            RPOR4bits.RP8R = 0;
            RPOR5bits.RP10R = 0;
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
int Barcode(int BarR, int state){ // side 1 is left , side  2 is right, side 3 is both
    char first;
    char second;
    char third;
    char fourth;
    int check=0;

    switch (state){

        case 0: // Idle
            //LCDPrintChar('0');
           // while(1);

            if(BarR <= Black){
                state = 1;
            }


            break;


    case 1: // start bit
//        LCDPrintChar('1');
//        while(1);

        if(BarR > Red) state=2;
        break;


        case 2: //  white space
           if(BarR < White) state=3;
        break;

        case 3: // first bit

            if(BarR < Black && check ==0){
               first = '0';
                 LCDMoveCursor(0,0);
               LCDPrintChar(first);
               check=1;
            }

            else if(BarR > Black && BarR <= Red && check==0){
                first = '1';
                 LCDMoveCursor(0,0);
               LCDPrintChar(first);
               check=1;
            }

            else if(BarR > Red) state = 4;
            break;

            case 4: // second white
           if(BarR < White) state=5;

        break;

        case 5: // second bit

            if(BarR < Black && check==0){
                second = '0';
                 LCDMoveCursor(0,1);
               LCDPrintChar(second);
               check=1;
            }

            else if(BarR > Black && BarR < Red && check==0){
                second = '1';
                 LCDMoveCursor(0,1);
               LCDPrintChar(second);
               check=1;
            }


            else if(BarR > Red) state = 6;
            break;

            case 6: //third white

            if(BarR < White) state=7;
                break;

        case 7: // third bit

            if(BarR < Black && check==0){
                third = '0';
                 LCDMoveCursor(0,2);
               LCDPrintChar(third);
               check=1;
            }

            else if(BarR > Black && BarR < Red &&check==0){
                third = '1';
                 LCDMoveCursor(0,2);
               LCDPrintChar(third);
               check=1;
            }


            else if(BarR > Red) state = 8;
            break;

            case 8: // fourth white
                if(BarR < White) state=9;
                    break;

        case 9: // fourth bit

            if(BarR < Black &&check==0 ){
                fourth = '0';
                 LCDMoveCursor(0,3);
               LCDPrintChar(fourth);
               check=1;
            }

            else if(BarR > Black && BarR < Red && check==0){
                fourth = '1';
                 LCDMoveCursor(0,3);
               LCDPrintChar(fourth);
               check = 1;
            }


            else if(BarR > Red) state = 10;
            break;

            case 10: // fifth white, print barcode




             if(BarR < Black) {
                 state = 0;
                 LCDClear();
             }
             break;

    }
    return state;
}






