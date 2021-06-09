/*
 * ex7_3.c
 *
 * Created: 2021-05-17 ���� 5:01:35
 * Author: syj47
 */

// 1kHz �����ĸ� PORTB.0�� ����ϴ� ���α׷�
// - 1kHz�� �����ĸ� ����� ���ؼ��� �޽��� On �ð��� off �ð��� ���� 500us



#include <mega128.h>

unsigned char c_cnt;

void Init_Timer2(void)
{
    TCCR2 = 0x00;           // Ÿ�̸�/ī���� ���� ����
    TCCR2 |= (1<<WGM21);    // TCCR2 �������� CTC ��� ����
    OCR2 = 100;             // ��� �� ���������� �ֱ�� 50us
    TIMSK = (1<<OCIE2);     // ��� �� ���ͷ�Ʈ �㰡����
    TCCR2 |= 1<<CS21;       // Ÿ�̸�/ī����2 ����
}

//��º� ���ͷ�Ʈ�� �߻����� �� ó���ϴ� ���ͷ�Ʈ �Լ�
interrupt [TIM2_COMP] void timer2_out_comp(void)
{
    c_cnt++;
}

void main(void)
{
    DDRB.0 = 1;             //PORTB�� 0������ ������� ����
    c_cnt = 0;              // c_cnt ���� Ŭ����
    Init_Timer2();          // Ÿ�̸�2 �ʱ�ȭ 
    SREG |= 0x80;           // ��ü ���ͷ�Ʈ �㰡
    while(1)
    {
        if(c_cnt == 10)     // 50us x 10 = 500us
        {
            PORTB.0 = 
}
