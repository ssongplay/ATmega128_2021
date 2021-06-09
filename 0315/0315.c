/*
 * 0315.c
 *
 * Created: 2021-03-15 ¿ÀÈÄ 4:36:19
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>

void main(void)
{

    DDRB = 0xff;
    while (1)
        {
        PORTB = 0x00;
        delay_ms(500);
        PORTB = 0xFF;
        delay_ms(500); 
        // Please write your application code here

        }
}
