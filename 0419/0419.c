/*
 * 0419.c
 *
 * Created: 2021-04-19 ���� 2:39:53
 * Author: syj47
 */
 
 
//--------------���� ���۰� ���ͷ�Ʈ�� ���� 1--------------------------- 
// 1�� ������ PORTB�� 4~7�� LED�� �����̸�, PD0�� ����ġ�� ���� ���
// PORTB�� 0~3�� LED �� Off�Ͽ���.


/*
//���� ��� 
#include <mega128.h>
#include <delay.h>

void main(void)
{
    DDRB = 0xff;    //��ƮB ��� ����
    DDRD = 0x00;    //��ƮD �Է� ����
    PORTB = 0x00;   //PORTB 0..7 LED ON
    
    while (1)
    {
        //SW(PD0)�� LOW�̸� PORTB 0..3 LED OFF
        if( !(PIND & 0x01)){ PORTB |= (0x0f); }
        
        PORTB &= ~(0xF0);   //PORTB 4..7 LED ON (LOW)
        delay_ms(1000);   
        
        PORTB |= (0xf0);    //PORTB 4..7 LED OFF (HIGH)
        delay_ms(1000);
    }
}
*/




/*
//���ͷ�Ʈ ���  
#include <mega128.h>
#include <delay.h>

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    PORTB |= (0x0f);
}

void main(void)
{
    DDRB = 0xff;    //��ƮB ��� ����
    DDRD = 0x00;    //��ƮD �Է� ����
    PORTB = 0x00;   //PORTB 0..7 LED ON
    
    //INT0 Falling Edge ���� ���� (ISC01 : 1, ISC00 : 0)   //���� �� ���� 
    EICRA &= ~(3<<ISC00);                   // 1111 1110
    EICRA |= (1<<ISC01) | (0<<ISC00);       // 0000 0010
    
    //INT0 Rising Edge ���� ����  (ISC01 : 1, ISC00 : 1)   //�� �� ���� 
    //EICRA &= ~(3<<ISC00);
    //EICRA |= (1<<ISC01) | (1<<ISC00); 
    
    EIMSK |= (1<<INT0);     //INT0 Ȱ��ȭ
    
    SREG |= (1<<SREG_I);    //��ü ���ͷ�Ʈ �㰡 
    
    while (1)
    {
        PORTB &= ~(0xF0);   //PORTB 4..7 LED ON (LOW)
        delay_ms(1000);   
        
        PORTB |= (0xf0);    //PORTB 4..7 LED OFF (HIGH)
        delay_ms(1000);
    }
}  
*/






//--------------���� ���۰� ���ͷ�Ʈ�� ���� 2--------------------------- 
// 1�� ������ PORTB�� 4~7�� LED�� �����̸�, PD0�� ����ġ�� ���� ���
// PORTB�� 0~3�� LED �� Off�Ͽ���.
// PD1�� ����ġ�� ���� ��� PORTB�� 0~3�� LED�� ON �Ͽ���


/*
//���� ��� 
#include <mega128.h>
#include <delay.h>

void main(void)
{
    DDRB = 0xff;    //��ƮB ��� ����
    DDRD = 0x00;    //��ƮD �Է� ����
    PORTB = 0x00;   //PORTB 0..7 LED ON
    
    while (1)
    {
        //SW(PD0)�� LOW�̸� PORTB 0..3 LED OFF
        if( !(PIND & 0x01)){ PORTB |= (0x0f); }
        
        //SW(PD1)�� LOW�̸� PORTB 0..3 LED ON 
        if( !(PIND & 0x02)){ PORTB &= ~(0x0f); }
        
        PORTB &= ~(0xF0);   //PORTB 4..7 LED ON (LOW)
        delay_ms(1000);   
        
        PORTB |= (0xf0);    //PORTB 4..7 LED OFF (HIGH)
        delay_ms(1000);
    }
}
*/





//���ͷ�Ʈ ���  
#include <mega128.h>
#include <delay.h>

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    PORTB |= (0x0f);
}

interrupt [EXT_INT1] void ext_int1_isr(void)
{
    PORTB &= ~(0x0f);
}

void main(void)
{
    DDRB = 0xff;    //��ƮB ��� ����
    DDRD = 0x00;    //��ƮD �Է� ����
    PORTB = 0x00;   //PORTB 0..7 LED ON
    
    //INT0 Falling Edge ���� ���� (ISC01 : 1, ISC00 : 0)   //���� �� ���� 
    EICRA &= ~(3<<ISC00);                   // 1111 1110
    EICRA |= (1<<ISC01) | (0<<ISC00);       // 0000 0010 
    //INT1 Falling Edge ���� ���� (ISC11 : 1, ISC10 : 0)   //���� �� ���� 
    EICRA &= ~(3<<ISC10);                   
    EICRA |= (1<<ISC11) | (0<<ISC10);       
            
    
    //INT0 Rising Edge ���� ����  (ISC01 : 1, ISC00 : 1)   //�� �� ���� 
    //EICRA &= ~(3<<ISC00);
    //EICRA |= (1<<ISC01) | (1<<ISC00);   
        
    
    EIMSK |= (1<<INT0) | (1<<INT1);     //INT0,1 Ȱ��ȭ
    SREG |= (1<<SREG_I);    //��ü ���ͷ�Ʈ �㰡 
    
    while (1)
    {
        PORTB &= ~(0xF0);   //PORTB 4..7 LED ON (LOW)
        delay_ms(1000);   
        
        PORTB |= (0xf0);    //PORTB 4..7 LED OFF (HIGH)
        delay_ms(1000);
    }
}  

