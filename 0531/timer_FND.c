/*
 * timer_FND.c
 *
 * Created: 2021-06-16 ���� 2:40:59
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>
// x : 0~3
#define SetRow(x) PORTE&=~0xF0; PORTE|=(1<<((x)+PORTD4));
unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                                0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //�ֳ�� ����
// FND�� ǥ���ϰ��� �ϴ� ���� ������ �ڸ����� ����Ű�� ���� ����
unsigned char FND_DISP_DATA[4] = {0,};
unsigned char FND_DISP_DigitCnt = 1;

interrupt [TIM2_COMP] void timer2_cmp(void)
{
    PORTE &= ~0xF0;     //FND ����
    PORTB = Port_char[FND_DISP_DATA[FND_DISP_DigitCnt]];
    SetRow(FND_DISP_DigitCnt);      // FND �� ���� 
    
    FND_DISP_DigitCnt++;
    if(FND_DISP_DigitCnt>3)
        FND_DISP_DigitCnt = 0;
    // FND_DISP_DigitCnt :: 1..4
}

void Init_Timer2(void)      // Ÿ�̸�/ī����2 - 2�� ���� �� PC PWM ��Ƽ�� ����
{
    TIMSK |= (1 << OCIE2);  // ��º� ���ͷ�Ʈ2 �㰡 
    TCCR2 = (1 << WGM21) | (5 << CS20); // CTC���, 1024����
    OCR2 = 14;      // �� 1ms 
    // OCR2 = (14745600Hz / 1024 ���ֺ�) * 0.001ms = 14.4 
}

void FND_Disp(int dec) 
{
    // ǥ���� ���ڸ� �ڸ��� ���� �߶� FND_DISP_DATA[0..3] �� ���� 
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
    
    // FND ���� ��Ʈ ����� ����
    DDRB = 0xFF;
    DDRE = 0xF0;
    
    Init_TimerINT();    //Ÿ�̸� �ʱ�ȭ 
    
    while(1)
    {
        for(i=0;i<100;i++)
        { //1�� ������ �ð� ǥ�� 
            FND_Disp(i);
            delay_ms(1000);
        }
    }
}