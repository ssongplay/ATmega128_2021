/*
 * ex7_4.c
 *
 * Created: 2021-05-24 ���� 2:37:57
 * Author: syj47
 */


// ���� 7-4 : ��� PWM ���

#include <mega128.h>

void Init_Timer2(void)
{
    TIMSK |= (1<<OCIE2);        //��� �� ���ͷ�Ʈ �㰡
    // FAST PWM ��� : WGM20 ~ WGM21 : '11'
    // OC2���� ����� �� ��� : COM21, COM20 : '10'
    // �ý��� Ŭ���� 8����, TCNT2 ��� ���� : CS22, CS21, CS20 = '010'
    TCCR2 |= (1<<WGM20 | 1<<WGM21 | 1<<COM21) | (1<<CS21);
    OCR2 = 0x00;                // OCR2 ���������� ���� 0���� ����
    DDRB.7 = 1;                 // PORTB�� 7�� ��Ʈ�� ������� ���� (OC2���)
    DDRD = 0x00;                // PORTD�� �Է����� ����
    // SREG |= 0x80;            // ���ͷ�Ʈ ���� ���� (��ü���ͷ�Ʈ�㰡)
}

interrupt [TIM2_COMP] void timer2_out_comp(void)
{ #asm("nop");}

void main(void)
{
    char ch;
    // int Temp = 0;
    Init_Timer2();      // Ÿ�̸�2 �ʱ�ȭ
    SREG |= 0x80;       // ��ü ���ͷ�Ʈ �㰡
    TCCR2 |= 1<<CS21;   // Ÿ�̸�/ī����2 ����  
    
    while(1)
    {
        ch = (PIND & 0xf0)>>4;      // PORTD�� ������Ʈ�� DIP����ġ �Է��� ���� ���
        switch(ch)
        {   
            // duty (OCR2��) = ��Ƽ�� * 255 
            
            case 0x07:              // PORTB.7 ����ġ�� ON�� ��� 75% ��Ƽ��
                // 0.75 * 255 =  191.25 (�� 192 --> 0xC0 )
                OCR2 = 0xC0;        // OCR2 ���� 0xC0���� ����
                break;
            case 0x0b:              // PORTB.6 ����ġ�� ON�� ��� 50% ��Ƽ�� 
                OCR2 = 0x80;        // OCR2 ���� 0x80���� ����
                break;
            case 0x0d:              // PORTB.5 ����ġ�� ON�� ��� 25% ��Ƽ��
                OCR2 = 0x40;        // OCR2 ���� 0x40���� ����    
                break;
            default:

            break;
        }
    }
}
