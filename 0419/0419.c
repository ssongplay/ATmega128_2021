/*
 * 0419.c
 *
 * Created: 2021-04-19 오후 2:39:53
 * Author: syj47
 */
 
 
//--------------폴링 동작과 인터럽트의 동작 1--------------------------- 
// 1초 단위로 PORTB의 4~7의 LED가 깜빡이며, PD0의 스위치가 눌린 경우
// PORTB의 0~3번 LED 를 Off하여라.


/*
//폴링 방식 
#include <mega128.h>
#include <delay.h>

void main(void)
{
    DDRB = 0xff;    //포트B 출력 설정
    DDRD = 0x00;    //포트D 입력 설정
    PORTB = 0x00;   //PORTB 0..7 LED ON
    
    while (1)
    {
        //SW(PD0)가 LOW이면 PORTB 0..3 LED OFF
        if( !(PIND & 0x01)){ PORTB |= (0x0f); }
        
        PORTB &= ~(0xF0);   //PORTB 4..7 LED ON (LOW)
        delay_ms(1000);   
        
        PORTB |= (0xf0);    //PORTB 4..7 LED OFF (HIGH)
        delay_ms(1000);
    }
}
*/




/*
//인터럽트 방식  
#include <mega128.h>
#include <delay.h>

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    PORTB |= (0x0f);
}

void main(void)
{
    DDRB = 0xff;    //포트B 출력 설정
    DDRD = 0x00;    //포트D 입력 설정
    PORTB = 0x00;   //PORTB 0..7 LED ON
    
    //INT0 Falling Edge 감지 설정 (ISC01 : 1, ISC00 : 0)   //누를 때 동작 
    EICRA &= ~(3<<ISC00);                   // 1111 1110
    EICRA |= (1<<ISC01) | (0<<ISC00);       // 0000 0010
    
    //INT0 Rising Edge 감지 설정  (ISC01 : 1, ISC00 : 1)   //뗄 때 동작 
    //EICRA &= ~(3<<ISC00);
    //EICRA |= (1<<ISC01) | (1<<ISC00); 
    
    EIMSK |= (1<<INT0);     //INT0 활성화
    
    SREG |= (1<<SREG_I);    //전체 인터럽트 허가 
    
    while (1)
    {
        PORTB &= ~(0xF0);   //PORTB 4..7 LED ON (LOW)
        delay_ms(1000);   
        
        PORTB |= (0xf0);    //PORTB 4..7 LED OFF (HIGH)
        delay_ms(1000);
    }
}  
*/






//--------------폴링 동작과 인터럽트의 동작 2--------------------------- 
// 1초 단위로 PORTB의 4~7의 LED가 깜빡이며, PD0의 스위치가 눌린 경우
// PORTB의 0~3번 LED 를 Off하여라.
// PD1의 스위치가 눌린 경우 PORTB의 0~3번 LED를 ON 하여라


/*
//폴링 방식 
#include <mega128.h>
#include <delay.h>

void main(void)
{
    DDRB = 0xff;    //포트B 출력 설정
    DDRD = 0x00;    //포트D 입력 설정
    PORTB = 0x00;   //PORTB 0..7 LED ON
    
    while (1)
    {
        //SW(PD0)가 LOW이면 PORTB 0..3 LED OFF
        if( !(PIND & 0x01)){ PORTB |= (0x0f); }
        
        //SW(PD1)이 LOW이면 PORTB 0..3 LED ON 
        if( !(PIND & 0x02)){ PORTB &= ~(0x0f); }
        
        PORTB &= ~(0xF0);   //PORTB 4..7 LED ON (LOW)
        delay_ms(1000);   
        
        PORTB |= (0xf0);    //PORTB 4..7 LED OFF (HIGH)
        delay_ms(1000);
    }
}
*/





//인터럽트 방식  
#include <mega128.h>
#include <delay.h>

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    PORTB |= (0x0f);
}

interrupt [EXT_INT1] void ext_int1_isr(void)
{
    PORTB &= ~(0x0f);
}

void main(void)
{
    DDRB = 0xff;    //포트B 출력 설정
    DDRD = 0x00;    //포트D 입력 설정
    PORTB = 0x00;   //PORTB 0..7 LED ON
    
    //INT0 Falling Edge 감지 설정 (ISC01 : 1, ISC00 : 0)   //누를 때 동작 
    EICRA &= ~(3<<ISC00);                   // 1111 1110
    EICRA |= (1<<ISC01) | (0<<ISC00);       // 0000 0010 
    //INT1 Falling Edge 감지 설정 (ISC11 : 1, ISC10 : 0)   //누를 때 동작 
    EICRA &= ~(3<<ISC10);                   
    EICRA |= (1<<ISC11) | (0<<ISC10);       
            
    
    //INT0 Rising Edge 감지 설정  (ISC01 : 1, ISC00 : 1)   //뗄 때 동작 
    //EICRA &= ~(3<<ISC00);
    //EICRA |= (1<<ISC01) | (1<<ISC00);   
        
    
    EIMSK |= (1<<INT0) | (1<<INT1);     //INT0,1 활성화
    SREG |= (1<<SREG_I);    //전체 인터럽트 허가 
    
    while (1)
    {
        PORTB &= ~(0xF0);   //PORTB 4..7 LED ON (LOW)
        delay_ms(1000);   
        
        PORTB |= (0xf0);    //PORTB 4..7 LED OFF (HIGH)
        delay_ms(1000);
    }
}  

