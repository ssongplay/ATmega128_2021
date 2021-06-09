/*
 * ex7_5.c
 *
 * Created: 2021-05-24 ���� 3:19:46
 * Author: syj47
 */


// ���� 7-5 : PC PWM ��� 

#include <mega128.h>

unsigned int cnt2;
unsigned char twosecond;

void Init_Timer0(void)      //Ÿ�̸�/ī����  0 - PC PWM ���� 
{
    TIMSK |= 1<<OCIE0;                              // ��� �� ���ͷ�Ʈ0 �㰡   
    TCCR0 |= (1<<WGM00 | 1<<COM01 | (1<<CS01));     // PC PWM ���, 8���� 
    OCR0 = 0x00;                                    // PC PWM���, 8���� 
}

void Init_Timer2(void)
{
    // Ÿ�̸�/ī���� 2 : CTC ��带 �̿��Ͽ� 2�� ���� �� PC PWM ��Ƽ�� ���� 
    TIMSK |= (1<<OCIE2);                // ��º� ���ͷ�Ʈ2 �㰡 
    TCCR2 |= (1<<WGM21) | (1<<CS21);    // CTC���, 8���� 
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
        switch(twosecond)         //2�ʸ��� ������ ����
        {   
            case 1: OCR0 = 0;    break;  //duty�� 0%
            case 2: OCR0 = 51;   break;  //duty�� 20%
            case 3: OCR0 = 102;  break;  //duty�� 40%
            case 4: OCR0 = 153;  break;  //duty�� 60%
            case 5: OCR0 = 204;  break;  //duty�� 80%
            case 6: OCR0 = 255; twosecond=0;  break;  //duty�� 100%
            default: twosecond=0;   break;
        }
    }
}

void Init_TimerINT(void)
{
    Init_Timer0();
    Init_Timer2();
    SREG |= 0x80;   //���ͷ�Ʈ ���۽��� (��ü���ͷ�Ʈ�㰡)
}

void main(void)
{
    cnt2 = 0;
    twosecond = 0;
    Init_TimerINT();
    DDRB = 0xff;            //PORTB�� 0,1�� ��Ʈ�� ������� ���
    PORTB = 0Xff;           // ��� LED OFF
    while(1);
}
