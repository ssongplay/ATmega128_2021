/*
 * read1row.c
 *
 * Created: 2021-04-12 ���� 3:04:49
 * Author: syj47
 */
    
 
// 1�� �б� 
#include <mega128.h>
#include <delay.h>
#define PIN_ROW1 PORTC4
#define PIN_ROW2 PORTC5
#define PIN_ROW3 PORTC6
#define PIN_ROW4 PORTC7
//Test Ex. ROW 1�� �����Ͽ� col �б�
// ������ : Ű�е� �ϳ��� row�� ���õǾ� 16���� 4���� ������. 


void main(void)
{
    DDRB = 0xFF;    //LED Ȯ�� ���Ͽ� PORTB ����Ʈ ������� ���� 
    DDRC = 0xF0;    // PORTC 4~7�� ROW ���� ���� ������� ���� 
                    // PORTC 0~3�� COL �� �б� ���� �Է����� ����
    PORTC |= 0xF0;  // PORTC 4~7�� HIGH ������� ROW ���� ����.     
    PORTC &= ~(1<<PIN_ROW1);      // PORTC 4�� LOW ������� ROW1 ���� 
    
while (1)
    {
        PORTB = PINC;      // COL�� ���� PORTB�� LED�� ��� 
    }
}





// ------------------------------------------------------------



// 1�� �б� - �Լ��� �̿��ϴ� ���
#include <mega128.h>
#include <delay.h>
#define PIN_ROW1 PORTC4
#define PIN_ROW2 PORTC5
#define PIN_ROW3 PORTC6
#define PIN_ROW4 PORTC7
//Test Ex. ROW 1�� �����Ͽ� col �б�
// ������ : Ű�е� �ϳ��� row�� ���õǾ� 16���� 4���� ������. 

void SetRow(unsigned char row)
{
    PORTC |= 0xf0;      //PORTC 4~7�� HIGH������� row ���� ����
    switch (row)
    {
        case 1 : PORTC &= ~(1<<PIN_ROW1);   break;
        case 2 : PORTC &= ~(1<<PIN_ROW2);   break;
        case 3 : PORTC &= ~(1<<PIN_ROW3);   break;
        case 4 : PORTC &= ~(1<<PIN_ROW4);   break;
    }
}

void main()
{
    DDRB = 0xFF;    //LED Ȯ�� ���Ͽ� PORTB ����Ʈ ������� ���� 
    DDRC = 0xF0;    // PORTC 4~7�� ROW ���� ���� ������� ���� 
                    // PORTC 0~3�� COL �� �б� ���� �Է����� ����
    SetRow(1);      // row 1�� ���� 
    while(1)
    {
        PORTB = PINC;   // COL�� ���� PORTB�� LED�� ���
    }
}



// ------------------------------------------------------------------      



// 1�� �б� - ��ũ�θ� �̿��ϴ� ��� 1
#include <mega128.h>
#include <delay.h>
// x : 1 ~4
#define SetRow(x)   PORTC |= 0xf0; PORTC &= ~(1<<((x-1)+PORTD4));

void main()
{
    DDRB = 0xFF;    //LED Ȯ�� ���Ͽ� PORTB ����Ʈ ������� ���� 
    DDRC = 0xF0;    // PORTC 4~7�� ROW ���� ���� ������� ���� 
                    // PORTC 0~3�� COL �� �б� ���� �Է����� ����
    SetRow(1);      // row 1�� ���� 
    while(1)
    {
        PORTB = PINC;   // COL�� ���� PORTB�� LED�� ���
    }
}





// ------------------------------------------------------------------      



// 1�� �б� - ��ũ�θ� �̿��ϴ� ��� 2
#include <mega128.h>
#include <delay.h>
// x : PIN_ROW1  ~ PIN_ROW4
#define SetRow(x)   PORTC |= 0xf0; PORTC &= ~(1<<(x));

void main()
{
    DDRB = 0xFF;    //LED Ȯ�� ���Ͽ� PORTB ����Ʈ ������� ���� 
    DDRC = 0xF0;    // PORTC 4~7�� ROW ���� ���� ������� ���� 
                    // PORTC 0~3�� COL �� �б� ���� �Է����� ����
    SetRow(PIN_ROW1);      // row 1�� ���� 
    while(1)
    {
        PORTB = PINC;   // COL�� ���� PORTB�� LED�� ���
    }
}


