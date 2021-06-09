/*
 * ex7_1.c
 *
 * Created: 2021-05-17 ���� 3:38:14
 * Author: syj47
 */






#include <mega128.h>

void Init_Timer2(void)
{
    TCCR2 = 1<<CS21;    // 0b00000010 �Ϲݸ��, 8���ֺ� ���
    TCNT2 = -92;       // ī���� �� ���� (50us)
    TIFR &= ~(1<<TOV2); // �����÷ο� �÷��� 0���� �ʱ�ȭ 
    TIMSK = (1<<TOIE2); // �����÷ο� (TOV2) ���ͷ�Ʈ �㰡
}

interrupt [TIM2_OVF] void timer2_overflow(void)
{
    TCNT2 = -92;       // ī���� �� ����(50us)
    PORTB.0 = ~PORTB.0;  // 50us
}

void main(void)
{
    Init_Timer2();      // Ÿ�̸�/ī����2�� �ʱ�ȭ
    DDRB = 0x0f;        // PORTB�� ���� �Ϻ��� ��� ��Ʈ�� ���� 
    PORTB.0 = 0;        // PORTB.0�� Ŭ����     
    SREG |= 0x80;       // ��ü ���ͷ�Ʈ �㰡
    while(1);           // ���ѷ��� 
}
