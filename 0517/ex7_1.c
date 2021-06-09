/*
 * ex7_1.c
 *
 * Created: 2021-05-17 오후 3:38:14
 * Author: syj47
 */






#include <mega128.h>

void Init_Timer2(void)
{
    TCCR2 = 1<<CS21;    // 0b00000010 일반모드, 8분주비 사용
    TCNT2 = -92;       // 카운터 값 설정 (50us)
    TIFR &= ~(1<<TOV2); // 오버플로우 플래그 0으로 초기화 
    TIMSK = (1<<TOIE2); // 오버플로우 (TOV2) 인터럽트 허가
}

interrupt [TIM2_OVF] void timer2_overflow(void)
{
    TCNT2 = -92;       // 카운터 값 설정(50us)
    PORTB.0 = ~PORTB.0;  // 50us
}

void main(void)
{
    Init_Timer2();      // 타이머/카운터2의 초기화
    DDRB = 0x0f;        // PORTB의 하위 니블을 출력 포트로 설정 
    PORTB.0 = 0;        // PORTB.0을 클리어     
    SREG |= 0x80;       // 전체 인터럽트 허가
    while(1);           // 무한루프 
}
