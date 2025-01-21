#include <xc.h>

#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

#define _XTAL_FREQ 4000000 // ??????? 4MHz
#define B3  247
#define C4  261
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494
#define C5  523
#define D5  587
#define E5  659
#define FSharp4  370

volatile unsigned int song_choice = 2; // ???????0 = ????1 = ???
volatile unsigned  int size = 108; // ????????
volatile unsigned int prev_song_choice=2;

const unsigned int melodies[3][120] = {
    {F4 ,0,F4,0,F4,0,E4,0,E4,0,E4,0,B3,0,B3,0,C4,0,D4,0,
     E4,0,E4,0,E4,0,D4,0,D4,0,D4,0,D4,0,C4,0,E4,0,
    F4 ,0,F4,0,F4,0,E4,0,E4,0,E4,0,B3,0,B3,0,C4,0,D4,0,
            D4,0,E4,0,E4,0,E4,0,E4,0,D4,0,C4,0,G4,0,C4,0,B3,0,C4,0
            
    },//APT
    {C5,0, D5, 0, B4, 0,C5,0, D5,0, B4,0, C5, 0, D5, 0, B4, 0, C5, 0, B4, 0, C5, 0,D5,0,
     A4, 0, B4, 0, G4, 0, A4, 0, B4, 0,G4 , 0,A4 ,0,B4, 0,G4, 0,A4,0,G4,0,A4,0,B4,0,
     C5,0, D5, 0, B4, 0,C5,0, D5,0, B4,0, C5, 0, D5, 0, B4, 0, C5, 0, B4, 0, C5, 0,D5,0,
     A4, 0, B4, 0, G4, 0, A4, 0, B4, 0,G4 , 0,A4 ,0,B4, 0,G4, 0,A4,0,G4,0,A4,0,E5,0,D5,0,B4,0
            
    },//good night
    {E4,0, G4, 0, E4, 0,FSharp4,0, G4,0, FSharp4,0, D4, 0, E4, 0, FSharp4, 0, G4, 0, FSharp4, 0, D4, 0,E4,0
    }
};

const unsigned int durations[3][120] = {
    {250,50,125,50,250,50,125,50,125,50,125,50,250,50,250,50,250,50,250,50,
    250,50,125,50,250,50,125,50,125,50,125,50,250,50,250,50,250,250,
    250,50,125,50,250,50,125,50,125,50,125,50,250,50,250,50,250,50,250,50,
    125,50,125,50,125,50,125,50,125,50,125,50,250,50,250,50,250,50,250,50,250,50
            
    },
    {250,0,250,0,250,0,250,0,250,0,250,0,250,0,250,0,400,0,100,0,250,0,250,0,500,500,
    250,0,250,0,250,0,250,0,250,0,250,0,250,0,250,0,400,0,100,0,250,0,250,0,500,500,
    250,0,250,0,250,0,250,0,250,0,250,0,250,0,250,0,400,0,100,0,250,0,250,0,500,500,  
    250,0,250,0,250,0,250,0,250,0,250,0,250,0,250,0,400,0,100,0,250,0,250,0,250,0,250,0,500,500
    },
    {200,50, 200, 50, 400, 100,200,50, 200,50, 200,50, 200, 50, 400, 50,200 , 50, 200, 50, 200, 50, 200, 50,250,200
    }
    
};
const unsigned char lights[3][120] = {
    {7,0,7,0,7,0,1,0,2,0,4,0,5,0,2,0,5,0,2,0,
     7,0,7,0,7,0,1,0,2,0,4,0,5,0,2,0,7,0,
    7 ,0,7,0,7,0,1,0,2,0,4,0,5,0,2,0,5,0,2,0,
            1,0,2,0,4,0,1,0,2,0,4,0,1,0,5,0,2,7,0,7,0
            
    },//APT
    {1,0, 2, 0, 4, 0,1,0, 2,0, 4,0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0,7,0,
     1, 0, 2, 0, 4, 0,1, 0, 2, 0,4 , 0,1 ,0,2, 0,1, 0,2,0,1,0,2,0,7,0,
    1,0, 2, 0, 4, 0,1,0, 2,0, 4,0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0,7,0,
     1, 0, 2, 0, 4, 0, 1, 0, 2, 0,4 , 0,1 ,0,2, 0,1, 0,2,0,1,0,2,0,7,0,7,0,7,0        
    },//good night
    {7,0, 7, 0, 7, 7,1,0, 2,0, 4,0, 1, 0, 2, 0,4 , 0, 1, 0, 2, 0, 4, 0,7,0
    } 
};
unsigned long pwm_period;

void Variable_Delay(unsigned int ms) {
    while (ms--) {
        __delay_ms(1); // ???? 1ms
    }
}

// PWM ?????
void PWM_Init() {
    IRCF2 = 1; // default setting 4M Hz
    IRCF1 = 1;
    IRCF0 = 0;
    TRISCbits.TRISC2 = 0; // ? RC2 ?????CCP1?
    CCP1CON = 0x0C;       // CCP1 ???? PWM ??
    PR2 = 0;              // ??? PR2
    T2CON = 0x04;         // ?? Timer2???? 1:1
    CCPR1L = 0;           // ?????? 0
}
void INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //disable low priority interrupt
}



// ?? PWM ??
void Set_PWM_Frequency(unsigned int freq) {
   
    unsigned int prescaler = 1; // ?? TMR2 ???? 1:1

    // ?? PWM ???? PR2 ?
    pwm_period = ((unsigned long)_XTAL_FREQ / (4 * freq * prescaler)) - 1;

    if (pwm_period > 255) {
        prescaler = 4; // ?? PR2 ????? 1:4 ???
        pwm_period = ((unsigned long)_XTAL_FREQ / (4 * freq * prescaler)) - 1;
        if (pwm_period > 255) {
            prescaler = 16; // ????????? 1:16 ???
            pwm_period = ((unsigned long)_XTAL_FREQ / (4 * freq * prescaler)) - 1;
        }
    }

    if (pwm_period <= 255) {
        PR2 =(unsigned char) pwm_period; // ?? PR2
        T2CON = (prescaler == 1) ? 0x04 : (prescaler == 4) ? 0x05 : 0x06; // ?? TMR2 ???
        CCPR1L =(unsigned char) pwm_period / 2; // ?????? 50%
    } 
    else {
        PR2 = 255; // ?????????????
        CCPR1L = 0; // ????????
    }
}

// ????
void Play_Melody(unsigned int melody[], unsigned int duration[],unsigned char light[] , int size) {
    for (int i = 0; i < size; i++) {
        if(song_choice!=prev_song_choice){
            break;
        }
        if(song_choice==3){//stop
            Set_PWM_Frequency(0);
            break;
        }
        if (melody[i] == 0) {
            Set_PWM_Frequency(0); // ??
        } else {
            Set_PWM_Frequency(melody[i]); // ????
            
        }
         LATD=light[i];
         Variable_Delay(duration[i]); // ????????
    }
    Set_PWM_Frequency(0); // ???????
}

// ???
void main() {
    PWM_Init(); // ??? PWM ??
    INTERRUPT_Initialize ();
    INTCONbits.INT0IE=1;
    INTCONbits.INT0IF=0;
    ADCON1=0x0f;
    TRISBbits.TRISB0=1;
    TRISDbits.TRISD0 =0 ;            
    TRISDbits.TRISD1 =0;    
    TRISDbits.TRISD2 =0 ;            
    TRISDbits.TRISD3 =0;   //output light
    LATD=0;
    
    
  
  
    
    while (1) {
        if(song_choice==3){
            Set_PWM_Frequency(0);//stop
            continue;
        }
        if(song_choice!=prev_song_choice){
           prev_song_choice=song_choice;
           Play_Melody(melodies[song_choice], durations[song_choice],lights[song_choice], size); // ?????????
            
        }else{
            Play_Melody(melodies[song_choice], durations[song_choice],lights[song_choice], size); // ?????????
        }
    }
}
void __interrupt(high_priority) Hi_ISR(void)
{
    if(INTCONbits.INT0IF==1){
        LATD=0;
        Set_PWM_Frequency(0); // ??
        __delay_ms(500);
        if(song_choice==0){  
            song_choice=1;
            size=108;
        }else if(song_choice==1){
            song_choice=2;
            size=26;
        } else if(song_choice==2){
            song_choice=3;//stop
        }else{
            song_choice=0;//stop
            size=80;
        }
                
       
    }
   
   
    INTCONbits.INT0IF=0;
    return;
}
