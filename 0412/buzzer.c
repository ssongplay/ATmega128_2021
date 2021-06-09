/*
 * buzzer.c
 *
 * Created: 2021-04-12 ���� 3:45:25
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>
#define DO 1908 //262Hz (3817us) 1908us
#define RE 1700 //294Hz (3401us) 1701us
#define MI 1515 //330Hz (3030us) 1515us
#define FA 1432 //349Hz (2865us) 1433us
#define SOL 1275 //370Hz (2703us) 1351us
#define LA 1136 //440Hz (2273us) 1136us
#define SI 1012 //494Hz (2024us) 1012us

void myDelay_us(unsigned int delay)
{
    int i;
    for(i=0; i<delay; i++)
    {
        delay_us(1);
    }
}

void SSound(int time)
{
    int i, tim;
    tim = 50000 / time;
    for(i=0; i<tim; i++)
    {
        PORTG |= (1<<PORTG4); //buzzer on, PORTG�� 4�� �� on (out1)
        myDelay_us(time);
        PORTG &= ~(1<<PORTG4); //buzzer off, PORTG�� 4�� �� off(out0)
        myDelay_us(time);
    }
    PORTG |= (1<<PORTG4);    //buzzer off, PORTG�� 4�� �� off (out 0)
    
}


void main(void)
{
    unsigned char sw;
    DDRG |= (1<<PORTG4);    //������ ����Ǵ� PORTG.4 �� ������� ���� 
    PORTG |= (1<<PORTG4);   // �����뺸���� buzz�� ȸ�ΰ� active-low�� �Ǿ������Ƿ�
                            //HIGH ���� ����Ͽ� ���� ���� 
    DDRD = 0x00;            //TACT ����ġ�� �Է����� ���� 
    DDRB = 0xff;            //TACT ����ġ �Է������� LED�� Ȯ���ϱ� ����    
                         
    while (1)                    
    {
        sw = ~PIND;
        PORTB = sw;
        if(         sw & (1<<PORTD0)){ SSound(DO);  }
        else if(    sw & (1<<PORTD1)){ SSound(RE);  }
        else if(    sw & (1<<PORTD2)){ SSound(MI);  }
        else if(    sw & (1<<PORTD3)){ SSound(FA);  }
        else if(    sw & (1<<PORTD4)){ SSound(SOL);  }
        else if(    sw & (1<<PORTD5)){ SSound(LA);  }
        else if(    sw & (1<<PORTD6)){ SSound(SI);  }
        
    }
}
