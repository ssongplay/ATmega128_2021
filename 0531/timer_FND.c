/*
 * timer_FND.c
 *
 * Created: 2021-06-16 오후 2:40:59
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>
// x : 0~3
#define SetRow(x) PORTE&=~0xF0; PORTE|=(1<<((x)+PORTD4));
unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                                0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //애노드 공통
// FND에 표시하고자 하는 숫자 정보와 자릿수를 가르키는 변수 생성
unsigned char FND_DISP_DATA[4] = {0,};
unsigned char FND_DISP_DigitCnt = 1;

interrupt [TIM2_COMP] void timer2_cmp(void)
{
    PORTE &= ~0xF0;     //FND 끄기
    PORTB = Port_char[FND_DISP_DATA[FND_DISP_DigitCnt]];
    SetRow(FND_DISP_DigitCnt);      // FND 열 선택 
    
    FND_DISP_DigitCnt++;
    if(FND_DISP_DigitCnt>3)
        FND_DISP_DigitCnt = 0;
    // FND_DISP_DigitCnt :: 1..4
}

void Init_Timer2(void)      // 타이머/카운터2 - 2초 설정 및 PC PWM 듀티비 설정
{
    TIMSK |= (1 << OCIE2);  // 출력비교 인터럽트2 허가 
    TCCR2 = (1 << WGM21) | (5 << CS20); // CTC모드, 1024분주
    OCR2 = 14;      // 약 1ms 
    // OCR2 = (14745600Hz / 1024 분주비) * 0.001ms = 14.4 
}

void FND_Disp(int dec) 
{
    // 표시할 숫자를 자릿수 별로 잘라 FND_DISP_DATA[0..3] 에 저장 
    FND_DISP_DATA[3] = (dec/1000%10);     
    FND_DISP_DATA[2] = (dec/100%10);
    FND_DISP_DATA[1] = (dec/10%10);
    FND_DISP_DATA[0] = (dec/1%10);        
}

void Init_TimerINT(void)
{
    Init_Timer2();
    SREG |= 0X80;
}

void main(void)
{
    int i = 0;
    
    // FND 관련 포트 입출력 설정
    DDRB = 0xFF;
    DDRE = 0xF0;
    
    Init_TimerINT();    //타이머 초기화 
    
    while(1)
    {
        for(i=0;i<100;i++)
        { //1초 단위로 시간 표시 
            FND_Disp(i);
            delay_ms(1000);
        }
    }
}