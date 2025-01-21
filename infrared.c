#include <xc.h>
#include <stdlib.h>
#include "stdio.h"      
#include "string.h"

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON    // Brown-out Reset Enable bit
#pragma config PBADEN = OFF  // Watchdog Timer Enable bit
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)
#define _XTAL_FREQ 125000 //  125 kHz

static const unsigned char segTable[10] = {
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12,0x03,0x78,0x00,0x18  
    //0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

int sum = 0;
int flag = 0;

void main(void) {
    IRCF2 = 0;
    IRCF1 = 0;
    IRCF0 = 1;
    //TRISCbits.RC4 = 1;
    
    TRISD = 0x00;  //output
    PORTD = 0xFF; // ????????????????????
    
    TRISC = 0b00011000;
    //LATC = 0;
    
    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    LATB = 0;
    
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b01;
    
     CCP1CONbits.CCP1M = 0b1100;
     PR2 = 0x9b;
     
    CCPR1L = 0x0B;
    CCP1CONbits.DC1B = 0b01;
    
    
    //while(1) {
        LATCbits.LC0 = 0;
        LATCbits.LC1 = 0;   
    //}
        LATD = segTable[0];
    
    
    while(1) {
        if(PORTCbits.RC3 == 1) {
           if(flag == 0) {
              if(sum == 8) {
                   //sum = 8;
                    flag = 1;////
                   //LATD = segTable[sum];
              }
             else {
                    CCPR1L = 0x04;
                    sum++;
                    //sum %= 10;
                    //LATD = segTable[sum];
                    flag = 1;
                    //__delay_ms(150);


                    __delay_ms(500);
                    __delay_ms(500);
                     //CCPR1L = 0x0B;
                    CCPR1L = 0x0C;
                    __delay_ms(250);   
              }
                LATD = segTable[sum];
            }
        }
        else {
            flag = 0;
            /*__delay_ms(500);
            __delay_ms(500);
            __delay_ms(500);
            __delay_ms(500);
            CCPR1L = 0x0B;
            __delay_ms(150);*/
        }
        /*
        if(sum > 4)  LATCbits.LC0 = 1; //1
        else {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        zz
            LATCbits.LC0 = 0;
            LATCbits.LC1 = 0;
        } */
        
        
         if(PORTCbits.RC4 == 0) {
             __delay_ms(50);
                LATCbits.LC0 = 0; //1
                LATCbits.LC1 = 0;
        }
        else {
            LATCbits.LC0 = 0;
            LATCbits.LC1 = 1;
        }
    }
    
    return;
}