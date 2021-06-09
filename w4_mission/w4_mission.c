/*
 * w4_mission.c
 *
 * Created: 2021-04-01 ���� 12:44:09
 * Author: syj47
 */    
         
 
/*  
    4���� �̼�    
    
 - ���� ���� 5-3 �� Ȱ���Ͽ� ������ Switch Case ������ ���α׷��� if else�� �̿��� ���·� �����Ұ� 

 - �̶� Pin Mask ����� �̿��Ͽ� ���α׷��� ������ ��(�߿�) 

 - �̶� ����ġ�� ���ÿ� ������� ���� ��ȣ�� ����ġ�� ���� �켱������ ���� �� ��

   (Ex PD4, PD6 �� ������ ��� -> PD4�� ���� ����)       
   
 */
            
 
 
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
        delay_ms(40);
        PORTB = LED;
        LED = (LED<<1) | 0x01;
        // �·� 1��Ʈ ����Ʈ�� �Ŀ� 0x01���� OR�����ν�
        // ��������Ʈ 0�� ���� 1�� ��Ʈ��.
        delay_ms(40);
    }
}

void PB_RShift(void) // ��Ʈ�� ���������� �̵� --> led�� ��������
{
    int i;
    unsigned char LED; // led ������ ����
    LED = 0xf7; // led ������ �ʱⰪ ����
    for(i=0; i<4;i++)
    {
        delay_ms(40);
        PORTB = LED;
        LED = (LED>>1) | 0x80;
        // ��� 1��Ʈ ����Ʈ�� �Ŀ� 0x80���� OR�����ν�
        // �ֻ�����Ʈ 0�� ���� 1�� ��Ʈ��.
        delay_ms(40);
    }
}  

void PB_LEDOnOff(void)
{
    unsigned char LED;
    
    LED = 0xf0; 
    PORTB = LED; 
    delay_ms(80);
    
    LED = 0xff; 
    PORTB = LED;
    delay_ms(80);      
    
}

void PB_LEDSwitch(void)
{
    unsigned char LED;
    
    LED = 0xfc; 
    PORTB = LED; 
    delay_ms(80);
    
    LED = 0xf3; 
    PORTB = LED; 
    delay_ms(80);   
  
}


void main(void)
{       
    DDRB = 0xff; // ��� ��Ʈ ������� ����
    DDRD = 0x0f;
    
    while(1)
    {
        if(!(PIND & 0x10))    
        {
            PB_LShift();   
        }
        
        else if(!(PIND & 0x20))
        {
            PB_RShift(); 
        }      
        
        else if(!(PIND & 0x40))
        {
            PB_LEDOnOff();
        }
         
        else if(!(PIND & 0x80))
        {
            PB_LEDSwitch();  
        } 

    }
} 
