/*
 * 123.c
 */                         
#include <mega128.h>
#include <delay.h>

//////////////////////////////////////////////////////////////////////////////
// FND - START

// DIGIT 1 - PORTE.4
// DIGIT 2 - PORTE.5
// DIGIT 3 - PORTE.6
// DIGIT 4 - PORTE.7

// FND_a   - PORTB.0
// FND_b   - PORTB.1
// FND_c   - PORTB.2
// FND_d   - PORTB.3
// FND_e   - PORTB.4
// FND_f   - PORTB.5
// FND_g   - PORTB.6
// FND_dt  - PORTB.7

const unsigned char FND_FONT[16] = {
        0b00111111
    ,   0b00000110
    ,   0b01011011
    ,   0b01001111
    ,   0b01100110
    ,   0b01101101
    ,   0b01111101
    ,   0b00000111
    ,   0b01111111
    ,   0b01100111
    ,   0b01110111  // a
    ,   0b01111100  // b
    ,   0b00111001  // C
    ,   0b01011110  // d  
    ,   0b01111011  // e
    ,   0b01110001  // f  
};

void SEG_out(unsigned char digit, unsigned char number)
{
    if(digit >= 4)
        return;   
        
    if(number >= 16)
        return;
        
    PORTE = 0x00;       
    PORTB = 0xFF;
    
     
    PORTB = ~FND_FONT[number]; 
    PORTE = (0x10<<digit);    
}                                                                                     
// FND - END
//////////////////////////////////////////////////////////////////////////////

  
//////////////////////////////////////////////////////////////////////////////
// FND - STET EXAMPLE - START
void main()
{   
    int dec = 0;
    int digitCnt = 0;
    
    DDRE = 0xF0;
    DDRB = 0xFF; 
    
    while(1)
    {      
        for(digitCnt = 0; digitCnt<10 ;digitCnt++)
        {   
            SEG_out(0,(dec/1)%10);   delay_ms(10);  
            SEG_out(1,(dec/10)%10);   delay_ms(10);  
            SEG_out(2,(dec/100)%10);   delay_ms(10);  
            SEG_out(3,(dec/1000)%10);   delay_ms(10);   
        }   
        dec++;
        dec = dec %1000;             
    }    
}
// FND - STET EXAMPLE - END  
//////////////////////////////////////////////////////////////////////////////