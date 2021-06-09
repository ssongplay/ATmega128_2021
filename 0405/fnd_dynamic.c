/*
 * fnd_dynamic.c
 *
 * Created: 2021-04-05 오후 4:20:27
 * Author: syj47
 */



#include <mega128.h>
#include <delay.h>

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                            0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //애노드 공통

unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f}; // FND0 ON, FND1 ON, FND2 ON, FND3 ON
void PORT_Init(void)
{
    DDRE = 0xf0; //PORTE.4~ PORTE.7 FND 출력
                // PE.4 : FND0, PE.5 : FND1 , PE.6 : FND2, PE.7 : FND3)
    DDRB = 0xff; //세그먼트의 문자포트 출력 
                // PB.0 : a, PB.1 : b, PB.2 : c, PB.3 : d, PB.4 : e, PB.5 : f , PB.6 : g, PB.7 : dot
}


void main(void)
{
    unsigned char FND0, FND1, FND2, FND3;
    PORT_Init();
    
    while (1)
    {
        for(FND3 = 0; FND3<16; FND3++)
        {
            for(FND2 = 0; FND2<16; FND2++)
            {
                for(FND1 = 0; FND1<16; FND1++)
                {
                    for(FND0 = 0; FND0<16; FND0++)
                    {
                        PORTE = Port_fnd[0];
                        PORTB = Port_char[FND0];
                        delay_ms(1);
                        
                        PORTE = Port_fnd[1];
                        PORTB = Port_char[FND1];
                        delay_ms(1);
                        
                        PORTE = Port_fnd[2];
                        PORTB = Port_char[FND2];
                        delay_ms(1);
                        
                        PORTE = Port_fnd[3];
                        PORTB = Port_char[FND3];
                        delay_ms(1);
                    }
                }
            }
        }
                        

    }
}
