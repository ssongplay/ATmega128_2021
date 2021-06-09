/*
 * w7_mission.c
 *
 * Created: 2021-05-01 ���� 9:56:50
 * Author: syj47
 */

/* 
-----------------7���� �̼�-----------------------------
-------------2019146018 �ۿ���---------------------------
> MAIN �Լ� ���ο��� ����ġ �д� �κ��� ���� ��
> MAIN �Լ� ���ο��� FND�� ��¸� ������ ��
> ����ġ�� ������ ���� �����ϵ��� �ϸ�, ������ �ִ� �߰� �� ���� ������ �̺�Ʈ�� ����
> �̶� ����ġ�� ������ �����ϰ� FND�� ���������� �����Ͽ�����
> ����ġ ������ �����Ǵ� ��� ���� ���(4��Ÿ�� SI)�� 1ȸ/10ms ����(�̶�, FND ���� ���)
> PORTD.0�� ����� ����ġ�� ON�Ǹ� FND 1����
> PORTD.1�� ����� ����ġ�� ON�Ǹ� FND 4����
> PORTD.2�� ����� ����ġ�� ON�Ǹ� FND 1����
> PORTD.3�� ����� ����ġ�� ON�Ǹ� FND 5���� 
----------------------------------------------------------
*/ 

#include <mega128.h>
#include <delay.h>
#define SI 1012 //494Hz (2024us) 1012us

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f};
int dec = 0;

void PORT_init(void)
{
    DDRE = 0xf0; //PORTE.4~ PORTE.7 FND ���
                // PE.4 : FND0, PE.5 : FND1 , PE.6 : FND2, PE.7 : FND3)
    DDRB = 0xff; //���׸�Ʈ�� ������Ʈ ��� 
                // PB.0 : a, PB.1 : b, PB.2 : c, PB.3 : d, PB.4 : e, PB.5 : f , PB.6 : g, PB.7 : dot        
                
    DDRG |= (1<<PORTG4);    //������ ����Ǵ� PORTG.4 �� ������� ���� 
    PORTG &= ~(1<<PORTG4);
}

void myDelay_us(unsigned int delay)
{
    int i;
    for(i = 0; i < delay; i++){
        delay_us(1);
    }
}

void SSound(int time) 
{
    int i, tim;
    tim = 5000 / time;
    for(i = 0; i < tim; i++) {
        PORTG |= (1<<PORTG4); //buzzer on, PORTG�� 4�� �� on (out1)
        myDelay_us(time);
        PORTG &= ~(1<<PORTG4); //buzzer off, PORTG�� 4�� �� off(out0)
        myDelay_us(time);
    }
}

interrupt [EXT_INT0] void ext_int0(void)
{
    dec += 1;
    SSound(SI);    
}
interrupt [EXT_INT1] void ext_int1(void)
{
    dec += 4;
    SSound(SI);    
}
interrupt [EXT_INT2] void ext_int2(void)
{
    dec -= 1;
    SSound(SI);    
}
interrupt [EXT_INT3] void ext_int3(void)
{
    dec -= 5;
    SSound(SI);   
}

void Interrupt_init(void)
{
    EICRA &= ~(3<<ISC00); //INT0 Falling Edge ���� ����
    EICRA |= (1<<ISC01) | (0<<ISC00);
    
    EICRA &= ~(3<<ISC10); //INT1 Falling Edge ���� ����
    EICRA |= (1<<ISC11) | (0<<ISC10);
    
    EICRA &= ~(3<<ISC20); //INT2 Falling Edge ���� ����
    EICRA |= (1<<ISC21) | (0<<ISC20);
    
    EICRA &= ~(3<<ISC30); //INT3 Falling Edge ���� ����
    EICRA |= (1<<ISC31) | (0<<ISC30);
    
    EIMSK |= (1<<INT0) | (1<<INT1) | (1<<INT2) | (1<<INT3); // INT 0,1,2,3 Ȱ��ȭ
    SREG |= (1<<SREG_I); //��ü ���ͷ�Ʈ �㰡
} 

// FND ���                
void FND_Disp(void)
{   
    PORTE = Port_fnd[3];
    PORTB = Port_char[(dec / 1000%10)]; 
    delay_ms(1);
    PORTB = 0xff;
    
    PORTE = Port_fnd[2];
    PORTB = Port_char[(dec / 100%10)]; 
    delay_ms(1);
    PORTB = 0xff;
    
    PORTE = Port_fnd[1];
    PORTB = Port_char[(dec / 10%10)]; 
    delay_ms(1);
    PORTB = 0xff;
    
    PORTE = Port_fnd[0];
    PORTB = Port_char[(dec / 1%10)]; 
    delay_ms(1);
    PORTB = 0xff; 
    
    if(dec > 9999) 
    {
        dec -= 10000;
    } 
    if(dec < 0) 
    {
        dec += 10000;
    }
}

void main(void)
{
    PORT_init();
    Interrupt_init();
    
    while (1)
    {
        FND_Disp();
    }
}