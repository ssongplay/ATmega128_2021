/*
 * 0329.c
 *
 * Created: 2021-03-29 ���� 3:01:23
 * Author: syj47
 */  
       
        
 
 
// ���� 5-3: ����� ���� ���� (����ġ ���¿� ���� switch ������ Ȱ��)
#include <mega128.h>
#include <delay.h>

void delay(unsigned int i)
{
    while(--i);
}

void PB_LShift(void)    //��Ʈ�� �������� �̵� --> led�� ����������
{
    int i;
    unsigned char LED; // led ������ ����
    LED = 0xfe; // led ������ �ʱⰪ ����
    for(i=0; i<4;i++)
    {
        delay_ms(200);
        PORTB = LED;
        LED = (LED<<1) | 0x01;
        // �·� 1��Ʈ ����Ʈ�� �Ŀ� 0x01���� OR�����ν�
        // ��������Ʈ 0�� ���� 1�� ��Ʈ��.
        delay_ms(200);
    }
}

void PB_RShift(void) // ��Ʈ�� ���������� �̵� --> led�� ��������
{
    int i;
    unsigned char LED; // led ������ ����
    LED = 0xf7; // led ������ �ʱⰪ ����
    for(i=0; i<4;i++)
    {
        delay_ms(200);
        PORTB = LED;
        LED = (LED>>1) | 0x80;
        // ��� 1��Ʈ ����Ʈ�� �Ŀ� 0x80���� OR�����ν�
        // �ֻ�����Ʈ 0�� ���� 1�� ��Ʈ��.
        delay_ms(200);
    }
}  

void PB_LEDOnOff(void)
{
    unsigned char LED;
    
    LED = 0xf0; 
    PORTB = LED; 
    delay_ms(200);
    
    LED = 0xff; 
    PORTB = LED;
    delay_ms(200);
      
    
}

void PB_LEDSwitch(void)
{
    unsigned char LED;
    
    LED = 0xfc; 
    PORTB = LED; 
    delay_ms(200);
    
    LED = 0xf3; 
    PORTB = LED; 
    delay_ms(200);   
  
}        


void main(void)
{
    unsigned char key; // key ������ ����
    DDRB = 0xff; // ��� ��Ʈ ������� ����
    DDRD = 0x00; // ������Ʈ�� �Է�, ���� ��Ʈ�� ������� ����
   
    while(1)
    {
        //��ư����ġ�� ���� �б� ���ؼ� PINB�� 0xf0���� AND�Ͽ� �˻��Ѵ�.     
        key = (PIND & 0xf0);     
        switch(key)
        {
            case 0xe0 : 
                PB_LShift();
                
                break;
            case 0xd0 :        
                PB_RShift();
                
                break;
            case 0xb0 :
                PB_LEDOnOff();
                
                break;
            case 0x70 :
                PB_LEDSwitch();
                
                break;
            default:
                break;
        }
    }
} 
 
 
 
 
// ��� 1
void main(void)
{
    unsigned char key; // key ������ ����
    DDRB = 0xff; // ��� ��Ʈ ������� ����
    DDRD = 0x00; // ������Ʈ�� �Է�, ���� ��Ʈ�� ������� ����
   
    while(1)
    {
        key = (PIND & 0xf0);
        if(     (key & 0x10) = = 0x00){PB_LShift(); }
        else if((key & 0x20) = = 0x00){PB_RShift(); }
        else if((key & 0x40) = = 0x00){PB_LEDOnOff(); }
        else if((key & 0x80) = = 0x00){PB_LEDSwitch(); } 
    }
}  

// ��� 2
void main(void)
{
    unsigned char key; // key ������ ����
    DDRB = 0xff; // ��� ��Ʈ ������� ����
    DDRD = 0x00; // ������Ʈ�� �Է�, ���� ��Ʈ�� ������� ����
   
    while(1)
    {
        key = (PIND & 0xf0);
        if(     !(key & (1<<PORTD4))) {PB_LShift(); }
        else if(!(key & (1<<PORTD5))) {PB_RShift(); }
        else if(!(key & (1<<PORTD6))){PB_LEDOnOff(); }
        else if(!(key & (1<<PORTD7))){PB_LEDSwitch(); } 
    }
}  
 
 
 
 
 

// ���� 5-2: LED�� �¿�� ����Ʈ�Ͽ� �����ϱ�(LR_Shift �Լ� �ۼ�) 

#include <mega128.h>
#include <delay.h>

void delay(unsigned int i)
{
    while(--i);
}

void PB_LShift(void)
{
    int i;
    unsigned char LED; // led ������ ����
    LED = 0xfe;// led ������ �ʱⰪ ����
    for(i=0; i<8;i++)
    {
        delay_ms(200);
        PORTB = LED;
        LED = (LED<<1) | 0x01;
        // �·� 1��Ʈ ����Ʈ�� �Ŀ� 0x01���� OR�����ν�
        // ��������Ʈ 0�� ���� 1�� ��Ʈ��.
        delay_ms(200);
    }
}

void PB_RShift(void) // ���� 5.1�� Ȱ���Ͽ� ������ ����Ʈ �Լ� �ۼ�
{
    int i;
    unsigned char LED; // led ������ ����
    LED = 0x7f; // led ������ �ʱⰪ ����
    for(i=0; i<8;i++)
    {
        delay_ms(200);
        PORTB = LED;
        LED = (LED>>1) | 0x80;
        // �·� 1��Ʈ ����Ʈ�� �Ŀ� 0x01���� OR�����ν�
        // ��������Ʈ 0�� ���� 1�� ��Ʈ��.
        delay_ms(200);
    }
}

void PB_LRShift(void)
// PB_LShift�� PB_Rshift �Լ��� Ȱ���Ͽ� �¿� ����Ʈ �Լ� �ۼ�
{
    PB_LShift();
    PB_RShift();
}


void main(void)
{
    DDRB = 0xff; // PB ��Ʈ�� ��� ������� ����
    PORTB = 0xff; // ��� LED�� Off
    while(1)
    {
    PB_LRShift();
    }
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
// ���� 5-1: LED�� ����Ʈ�Ͽ� �����ϱ� (PB_LShift �Լ� �ۼ�) 
//#include <mega128.h>
//#include <delay.h>
//
//void delay(unsigned int i)
//{
//    while(--i);
//}
//
//void PB_LShift(void)
//{
//    int i;
//    unsigned char LED; // led ������ ����
//    LED = 0xfe;// led ������ �ʱⰪ ����
//    for(i=0; i<8;i++)
//    {
//        delay_ms(200);
//        PORTB = LED;
//        LED = (LED<<1) | 0x01;
//        // �·� 1��Ʈ ����Ʈ�� �Ŀ� 0x01���� OR�����ν�
//        // ��������Ʈ 0�� ���� 1�� ��Ʈ��.
//        delay_ms(200);
//    }
//}
//
//void main(void)
//{
//    DDRB = 0xff; // PB ��Ʈ�� ��� ������� ����
//    PORTB = 0xff; // ��� LED�� Off
//    while(1)
//    {
//    PB_LShift();
//    delay_ms(200);
//    }
//}











//#include <mega128.h>
//#include <delay.h>
//
//void delay(unsigned int cnt)
//{
//    while(cnt--);
//}
//    
//
//void main(void)
//{
//    // PORTB�� 0���� ������� ����
//     
//    // DDRB = 0b00000001;  //0x01;  
//    DDRB = (1<<PORTB0);  //0x01;
//    PORTB &= ~(1<<PORTB0);
//    //PORTB = PORTB & (~(1<<0));
//    
//    while (1)
//    {
//        if(PIND & (1<<PORTD3))
//        {
//        PORTB &= ~(1<<PORTB0);
//        }
//        else
//        {
//        PORTB |= (1<<PORTB0);
//        }
//    }
//}
