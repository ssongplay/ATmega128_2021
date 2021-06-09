/*
 * ServoMotor.c
 *
 * Created: 2021-05-31 오후 4:35:45
 * Author: syj47
 */

// ------타이머를 활용한 서보모터의 구동(CTC모드)------

#include <mega128.h>
#include <delay.h>

unsigned int T2_DUTY_TIME_us;
unsigned int T2_CYCLE_TIME_us;

unsigned int T2_DUTY_TIME_cnt_us;
unsigned int T2_CYCLE_TIME_cnt_us;

#define SERVO_PIN_LOW()     PORTF &= ~(1<<PORTF3)
#define SERVO_PIN_HIGH()    PORTF |= (1<<PORTF3)

// 100us
interrupt [TIM2_COMP] void timer2_cmp(void)
{
    T2_DUTY_TIME_cnt_us += 100;
    T2_CYCLE_TIME_cnt_us += 100;
    
    if(T2_DUTY_TIME_cnt_us <= T2_DUTY_TIME_us)
    {SERVO_PIN_HIGH();}      
    else
    {SERVO_PIN_LOW();}       
    
    if(T2_CYCLE_TIME_cnt_us == T2_CYCLE_TIME_us)
    {
        T2_CYCLE_TIME_cnt_us = 0;
        T2_DUTY_TIME_cnt_us = 0;
    }
}

void Init_Timer2(void)
{
    // 타이머/카운터 2 - 2초 설정 및 PC PWM 듀티비 설정 
    TIMSK |= (1<<OCIE2);                // 출력비교 인터럽트2 허가
    TCCR2 = (1<<WGM21) | (2<<CS20);     // CTC모드, 1024분주
    OCR2 = 184;                         // 약 100us
    // OCR2 = (14745600Hz / 8분주비) * 100us = 184.32
}

void Init_TimerINT(void)
{
    Init_Timer2();
    SREG |= 0x80;
}

void SetServoDeg(unsigned int deg)
{
    T2_DUTY_TIME_us = 500 + (deg*200/18);
}

void main(void)
{
    DDRF |= (1<<PORTF3);
    
    Init_TimerINT();    // 타이머 초기화  
    
    T2_DUTY_TIME_us = 1500;     // PWM 듀티 폭 설정
    T2_CYCLE_TIME_us = 20000;   // PWM 주기 설정 
    
    while(1)
    {
        SetServoDeg(0);
        delay_ms(1000);
        SetServoDeg(45);
        delay_ms(1000);
        SetServoDeg(90);
        delay_ms(1000);
        SetServoDeg(135);
        delay_ms(1000);
        SetServoDeg(180);
        delay_ms(1000);
        SetServoDeg(90);
        delay_ms(1000);
    }
}
