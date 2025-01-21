
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <pic18f4520.h>


#pragma config OSC = INTIO67   // ????????RA6/RA7 ?? I/O
#pragma config WDT = OFF       // ?????
#pragma config LVP = OFF       // ?????????
#pragma config PBADEN = OFF    // PORTB ????? IO
#pragma config MCLRE = ON      // MCLR pin ?? (???????????)


#define _XTAL_FREQ 4000000  //8MHz

#define DS18B20_PIN       PORTAbits.RA1
#define DS18B20_PIN_LAT   LATAbits.LATA1
#define DS18B20_PIN_TRIS  TRISAbits.TRISA1

bool DS18B20_Reset(void);
void DS18B20_WriteBit(uint8_t dBit);
uint8_t DS18B20_ReadBit(void);
void DS18B20_WriteByte(uint8_t data);
uint8_t DS18B20_ReadByte(void);
int16_t DS18B20_ReadTemperature(void);
void displayTemperatureDifference(int8_t diff);


void main(void) 
{

    ADCON1 = 0x0F;  // digital I/O
    
    OSCCON = 0b01100000; // SCS=00, IRCF=111(8MHz), IDLEN=0

    DS18B20_PIN_TRIS = 1;  //RA1 input
    
    TRISD = 0x00;  //portD output
    PORTD = 0x00;  //LATD = 0 
    
    TRISCbits.TRISC0 = 0;
    PORTCbits.RC0 = 0;
    
    int16_t oldTemp = 0;
    int16_t newTemp = 0;
    int8_t  diff = 0;

    bool presenceOK = false;

    //----------------------------------------------------------------------------
    // detect if DS18B20 connected (Presence Pulse)
    //----------------------------------------------------------------------------
    presenceOK = DS18B20_Reset(); 
    
    while(!presenceOK) 
    {
        // if not detected
        PORTD = 0x0F;
        __delay_ms(250);
        PORTD = 0x00;
        __delay_ms(250);
    }

    //----------------------------------------------------------------------------
    // mainloop, measure oldTemp, wait for 0.5s, then newTemp
    // display current temperature on PORTD(RD0~RD3)
    //----------------------------------------------------------------------------
    while(1)
    {
        // 1) read current temperature
        oldTemp = DS18B20_ReadTemperature();

        // 2) delay 0.5 sec
        __delay_ms(500);

        // 3) read current temperature again
        newTemp = DS18B20_ReadTemperature();

        // 4) difference
        diff = (int8_t)(newTemp - oldTemp);

        // 5) display on LED
        displayTemperatureDifference(newTemp);
        
    }
}

//------------------------------------------------------------------------------
// 1-Wire Reset: return true if DS18B20 Presence Pulse get response
//------------------------------------------------------------------------------
bool DS18B20_Reset(void)
{
    bool presence;

    // set low
    DS18B20_PIN_TRIS = 0;  // output
    DS18B20_PIN_LAT = 0;   // set low
    __delay_us(480);

    DS18B20_PIN_TRIS = 1;  //input
    __delay_us(70);

    // read if DS18B20 set low (Presence)
    presence = (DS18B20_PIN == 0) ? true : false;
    __delay_us(410);

    return presence;
}

//------------------------------------------------------------------------------
// Write a bit (Write Bit)
//  - bit=0: hold low for 60us
//  - bit=1: hold low for 2~10us and then release
//------------------------------------------------------------------------------
void DS18B20_WriteBit(uint8_t dBit)
{
    DS18B20_PIN_TRIS = 0;  // output
    DS18B20_PIN_LAT = 0;   // set low
    __delay_us(2);   

    if(dBit) {
        // For writing '1', release as soon as possible
        DS18B20_PIN_TRIS = 1; 
    }
    __delay_us(60);
    // Ensure release at the end
    DS18B20_PIN_TRIS = 1;
    __delay_us(1);
}

//------------------------------------------------------------------------------
// Read one bit (Read Bit)
//  - Hold low for 2us, then switch to input and read state between 5~15us
//------------------------------------------------------------------------------
uint8_t DS18B20_ReadBit(void)
{
    uint8_t bitVal;

    // Hold low
    DS18B20_PIN_TRIS = 0; 
    DS18B20_PIN_LAT = 0;
    __delay_us(2);

    // Release
    DS18B20_PIN_TRIS = 1;  //switch to input
    __delay_us(8); // Wait for approximately 5~15us

    // Read pin state
    bitVal = (DS18B20_PIN) ? 1 : 0;

    // Complete the timeslot (~60us)
    __delay_us(50);

    return bitVal;
}

//------------------------------------------------------------------------------
// Write one Byte
//------------------------------------------------------------------------------
void DS18B20_WriteByte(uint8_t data)
{
    for(uint8_t i = 0; i < 8; i++) {
        DS18B20_WriteBit(data & 0x01);
        data >>= 1;
    }
}

//------------------------------------------------------------------------------
// Read one Byte
//------------------------------------------------------------------------------
uint8_t DS18B20_ReadByte(void)
{
    uint8_t value = 0;
    for(uint8_t i = 0; i < 8; i++) {
        value |= (DS18B20_ReadBit() << i);
    }
    return value;
}

//------------------------------------------------------------------------------
// Read DS18B20 temperature (integer part in °C)
//  - First, send Convert T (0x44) and wait 750ms (for 12-bit resolution)
//  - Then, send Read Scratchpad (0xBE) to retrieve the lower and upper bytes of the temperature
//  - Calculate the result (temp / 16) to obtain the integer value in °C (assuming 12-bit resolution)
//------------------------------------------------------------------------------
int16_t DS18B20_ReadTemperature(void)
{
    // Step1: Reset + Skip ROM + Convert T
    DS18B20_Reset();
    DS18B20_WriteByte(0xCC); // Skip ROM
    DS18B20_WriteByte(0x44); // Convert T
    __delay_ms(750);      

    // Step2: Reset + Skip ROM + Read Scratchpad
    DS18B20_Reset();
    DS18B20_WriteByte(0xCC); // Skip ROM
    DS18B20_WriteByte(0xBE); // Read Scratchpad

    // read two Byte (LSB + MSB)
    uint8_t tempL = DS18B20_ReadByte();
    uint8_t tempH = DS18B20_ReadByte();

    // Combine the two bytes
    int16_t rawTemp = (int16_t)((tempH << 8) | tempL);

    // For DS18B20 in 12-bit resolution, each bit represents 0.0625°C => Divide by 16 to get the integer value in °C
    return (rawTemp / 16);
}

//------------------------------------------------------------------------------
// display current temperature
//------------------------------------------------------------------------------
void displayTemperatureDifference(int8_t diff)
{
    //uint8_t absDiff = (diff < 0) ? -diff : diff; 
    diff = diff-20;
    switch(diff){
        case 0:
            PORTD = 0; 
            break;
        case 1:
            PORTD = 1; 
            break;
        case 2:
            PORTD = 2; 
            break;
        case 3:
            PORTD = 3; 
            break;
        case 4:
            PORTD = 4;
            break;
        case 5:
            PORTD = 5; 
            break;
        case 6:
            PORTD = 6;
            break;
        case 7:
            PORTD = 7;  
            break;
        case 8:
            PORTD = 8; 
            break;
        case 9:
            PORTD = 9; 
            break;
        case 10:
            PORTD = 10; 
            break;
        case 11:
            PORTD = 11;  
            break;
        case 12:
            PORTD = 12; 
            break;
        case 13:
            PORTD = 13; 
            break;
        case 14:
            PORTD = 14; 
            break;
        case 15:
            PORTD = 15; 
            break;
    }
    if(PORTD>6){
    //while(1){
        PORTCbits.RC0 = 1;
    }else{
        PORTCbits.RC0 = 0; 
    //}
    } 
    
}