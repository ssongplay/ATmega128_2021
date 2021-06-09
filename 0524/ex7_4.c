/*
 * ex7_4.c
 *
 * Created: 2021-05-24 오후 2:37:57
 * Author: syj47
 */


// 예제 7-4 : 고속 PWM 모드

#include <mega128.h>

void Init_Timer2(void)
{
    TIMSK |= (1<<OCIE2);        //출력 비교 인터럽트 허가
    // FAST PWM 모드 : WGM20 ~ WGM21 : '11'
    // OC2핀을 비반전 비교 출력 : COM21, COM20 : '10'
    // 시스템 클럭을 8분주, TCNT2 계수 시작 : CS22, CS21, CS20 = '010'
    TCCR2 |= (1<<WGM20 | 1<<WGM21 | 1<<COM21) | (1<<CS21);
    OCR2 = 0x00;                // OCR2 레지스터의 값을 0으로 설정
    DDRB.7 = 1;                 // PORTB의 7번 비트를 출력으로 설정 (OC2출력)
    DDRD = 0x00;                // PORTD를 입력으로 설정
    // SREG |= 0x80;            // 인터럽트 동작 시작 (전체인터럽트허가)
}

interrupt [TIM2_COMP] void timer2_out_comp(void)
{ #asm("nop");}

void main(void)
{
    char ch;
    // int Temp = 0;
    Init_Timer2();      // 타이머2 초기화
    SREG |= 0x80;       // 전체 인터럽트 허가
    TCCR2 |= 1<<CS21;   // 타이머/카운터2 동작  
    
    while(1)
    {
        ch = (PIND & 0xf0)>>4;      // PORTD의 상위비트의 DIP스위치 입력이 들어올 경우
        switch(ch)
        {   
            // duty (OCR2값) = 듀티비 * 255 
            
            case 0x07:              // PORTB.7 스위치가 ON인 경우 75% 듀티비
                // 0.75 * 255 =  191.25 (약 192 --> 0xC0 )
                OCR2 = 0xC0;        // OCR2 값을 0xC0으로 설정
                break;
            case 0x0b:              // PORTB.6 스위치가 ON인 경우 50% 듀티비 
                OCR2 = 0x80;        // OCR2 값을 0x80으로 설정
                break;
            case 0x0d:              // PORTB.5 스위치가 ON인 경우 25% 듀티비
                OCR2 = 0x40;        // OCR2 값을 0x40으로 설정    
                break;
            default:

            break;
        }
    }
}
