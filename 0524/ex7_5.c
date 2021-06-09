/*
 * ex7_5.c
 *
 * Created: 2021-05-24 오후 3:19:46
 * Author: syj47
 */


// 예제 7-5 : PC PWM 모드 

#include <mega128.h>

unsigned int cnt2;
unsigned char twosecond;

void Init_Timer0(void)      //타이머/카운터  0 - PC PWM 구동 
{
    TIMSK |= 1<<OCIE0;                              // 출력 비교 인터럽트0 허가   
    TCCR0 |= (1<<WGM00 | 1<<COM01 | (1<<CS01));     // PC PWM 모드, 8분주 
    OCR0 = 0x00;                                    // PC PWM모드, 8분주 
}

void Init_Timer2(void)
{
    // 타이머/카운터 2 : CTC 모드를 이용하여 2초 설정 및 PC PWM 듀티비 설정 
    TIMSK |= (1<<OCIE2);                // 출력비교 인터럽트2 허가 
    TCCR2 |= (1<<WGM21) | (1<<CS21);    // CTC모드, 8분주 
    OCR2 = 100;                         // 50us
}

interrupt [TIM0_COMP] void timer0_out_comp(void)
{ #asm("nop") }

interrupt [TIM2_COMP] void timer2_out_comp(void)
{
    cnt2++;
    if(cnt2 == 40000)   // 50us * 20000 = 1sec      40000 = 2sec
    {
        cnt2 = 0;
        twosecond++;
        PORTB ^= (1<<PORTB0);
        switch(twosecond)         //2초마다 다음을 실행
        {   
            case 1: OCR0 = 0;    break;  //duty비 0%
            case 2: OCR0 = 51;   break;  //duty비 20%
            case 3: OCR0 = 102;  break;  //duty비 40%
            case 4: OCR0 = 153;  break;  //duty비 60%
            case 5: OCR0 = 204;  break;  //duty비 80%
            case 6: OCR0 = 255; twosecond=0;  break;  //duty비 100%
            default: twosecond=0;   break;
        }
    }
}

void Init_TimerINT(void)
{
    Init_Timer0();
    Init_Timer2();
    SREG |= 0x80;   //인터럽트 동작시작 (전체인터럽트허가)
}

void main(void)
{
    cnt2 = 0;
    twosecond = 0;
    Init_TimerINT();
    DDRB = 0xff;            //PORTB의 0,1번 비트를 출력으로 사용
    PORTB = 0Xff;           // 모든 LED OFF
    while(1);
}
