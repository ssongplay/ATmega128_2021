/*
 * ex7_2.c
 *
 * Created: 2021-05-17 ���� 4:10:22
 * Author: syj47
 */

#include <mega128.h>

void Init_Timer2(void)
{
    TCCR2 = 0x00;
    // TCCR2 �������� 0x08�� ���� CTC���
    // TCCR2 �������� 0x10�� ���� ����ġ�� OC2 ����� ���   
    TCCR2 |= (1<<WGM21) | (1<<COM20) |(1<<CS21)|(1<<CS20);  
    // --> �ڿ� |(1<<cs21)|(1<<cs20);  --> 64���� �Ұ� (�����ڷ� 21)
    // ��º� �������ͷ� TCNT0 �������Ϳ� ���Ͽ� OC2 ���ڿ� ��½�ȣ �߻�
    OCR2 = 114.2;       //64���� 
    // OCR2=100��  8����??? 
     
    TIMSK |= 1 << OCIE2;    // ��� �� ���ͷ�Ʈ �㰡 ����
    TCCR2 |= 1 << CS21;     // Ÿ�̸�/ī����2 ����
}

interrupt [TIM2_COMP] void timer2_out_comp(void)
{
    #asm("nop");
}

void main(void)
{
    Init_Timer2();
    DDRB.7 = 1;         //PORTB�� 7�� ���� ������� ����
    SREG |= 0x80;       // ���ͷ�Ʈ �㰡
    while(1);
}                                   
