/*
 * fnd_basic_application.c
 *
 * Created: 2021-04-05 ���� 3:22:26
 * Author: syj47
 */

// ���� FND �⺻ ���� ����
// PORTD.4�� ����� DIP����ġ�� ON�Ǹ�, 0.5�ʸ��� FND 1 ����
// PORTD.5�� ����� DIP����ġ�� ON�Ǹ�, 0.5�ʸ��� FND 3 ����
// PORTD.6�� ����� DIP����ġ�� ON�Ǹ�, 0.5�ʸ��� FND 1 ���� 
// PORTD.7�� ����� DIP����ġ�� ON�Ǹ�, 0.5�ʸ��� FND 3 ���� 
// �̶� ������ ǥ�� ������ 0 ~ 9 (�������� �� ��� 9�� ����. -2 -> 8)

#include <mega128.h>
#include <delay.h>

unsigned char num_data[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                            0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //�ֳ�� ����


void main(void)
{
    int i=0;
    DDRB = 0xff; // ��� ��Ʈ ������� ����
  
    while(1)
    {
        if(      !(PIND & (1<<PORTD4))){ i+=1; delay_ms(500); }
        else if( !(PIND & (1<<PORTD5))){ i+=3; delay_ms(500); }
        else if( !(PIND & (1<<PORTD6))){ i-=1; delay_ms(500); } 
        else if( !(PIND & (1<<PORTD7))){ i-=3; delay_ms(500); }
        
        if(i<0) i+=10; 
        if(i>10) i-=10;
        
        PORTB = num_data[i]  
    }
} 