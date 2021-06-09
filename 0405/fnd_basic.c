/*
 * fnd_basic.c
 *
 * Created: 2021-04-05 ���� 2:46:01
 * Author: syj47
 */

/*
#include <mega128.h>
#include <delay.h>

void main(void)
{
    int i=1;
    DDRB = 0xFF;

while (1)
    {
         PORTB = ~i; // 0000 0001
                     // 1111 1110
         delay_ms(500);
         
         i = i<<1; // 1 0000 0000
         if (!(iS & 0xFF))
            i = 1;
    }
}
*/



//���� FND �⺻ ����
#include <mega128.h>
#include <delay.h>

unsigned char num_data[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                            0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //�ֳ�� ����

void PORT_Init(void)
{
    DDRB = 0xff; //���׸�Ʈ�� ������Ʈ ���
                    // (PB.0:a, PB.1:b, PB.2:c, PB.3:d, PB.4:e, PB.5:f, PB.6:g, PB.7:dot)
}

void main()
{
    unsigned char i;
    PORT_Init();
    
    while(1)
    {
        for(i=0; i<16; i++)  //7���׸�Ʈ�� 16���� ǥ��
        {
            PORTB = num_data[i]; //FND 0 �� �迭 segment�� ���
            delay_ms(1000);
        }
    }
}

    