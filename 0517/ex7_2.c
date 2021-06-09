/*
 * ex7_2.c
 *
 * Created: 2021-05-17 오후 4:10:22
 * Author: syj47
 */

#include <mega128.h>

void Init_Timer2(void)
{
    TCCR2 = 0x00;
    // TCCR2 레지스터 0x08로 설정 CTC모드
    // TCCR2 레지스터 0x10로 설정 비교일치시 OC2 출력을 토글   
    TCCR2 |= (1<<WGM21) | (1<<COM20) |(1<<CS21)|(1<<CS20);  
    // --> 뒤에 |(1<<cs21)|(1<<cs20);  --> 64분주 할것 (강의자료 21)
    // 출력비교 레지스터로 TCNT0 레지스터와 비교하여 OC2 단자에 출력신호 발생
    OCR2 = 114.2;       //64분주 
    // OCR2=100은  8분주??? 
     
    TIMSK |= 1 << OCIE2;    // 출력 비교 인터럽트 허가 상태
    TCCR2 |= 1 << CS21;     // 타이머/카운터2 동작
}

interrupt [TIM2_COMP] void timer2_out_comp(void)
{
    #asm("nop");
}

void main(void)
{
    Init_Timer2();
    DDRB.7 = 1;         //PORTB의 7번 핀을 출력으로 설정
    SREG |= 0x80;       // 인터럽트 허가
    while(1);
}                                   
