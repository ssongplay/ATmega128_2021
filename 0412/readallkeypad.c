/*
 * readallkeypad.c
 *
 * Created: 2021-05-02 ���� 11:03:16
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>
// x : 1 ~4
#define SetRow(x)   PORTC |= 0xf0; PORTC &= ~(1<<((x-1)+PORTD4));

unsigned char Port_char[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90
                                , 0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e }; //�ֳ�� ����
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f}; // FND0 ON, FND1 ON, FND2 ON, FND3 ON

void PORT_Init(void)
{
    DDRE = 0xf0; //PORTE.4~ PORTE.7 FND ���
                // PE.4 : FND0, PE.5 : FND1 , PE.6 : FND2, PE.7 : FND3)  
                 
    DDRB = 0xff; //���׸�Ʈ�� ������Ʈ ��� 
                // PB.0 : a, PB.1 : b, PB.2 : c, PB.3 : d, PB.4 : e, PB.5 : f , PB.6 : g, PB.7 : dot
                
    DDRC = 0xF0;    // PORTC 4~7�� ROW ���� ���� ������� ���� 
                    // PORTC 0~3�� COL �� �б� ���� �Է����� ����
}

void FND_Disp(int dec)
{
    PORTE = Port_fnd[3];
    PORTB = Port_char[(dec/1000%10)];
    delay_ms(1);
    PORTB = 0xFF;
    
    PORTE = Port_fnd[2];
    PORTB = Port_char[(dec/100%10)];
    delay_ms(1);
    PORTB = 0xFF; 
    
    PORTE = Port_fnd[1];
    PORTB = Port_char[(dec/10%10)];
    delay_ms(1);
    PORTB = 0xFF; 
    
    PORTE = Port_fnd[0];
    PORTB = Port_char[(dec/1%10)];
    delay_ms(1);
    PORTB = 0xFF;
    
    PORTE = 0x00;
}

void main()
{
    unsigned char sel_row;
    unsigned char keypad_code = 0;
    unsigned char fnd_value = 0;
    unsigned char col;
    PORT_Init();   
    
    while(1)
    {
        FND_Disp(fnd_value);
        keypad_code = 0xff; 
        
        for(sel_row=1; sel_row<=4; sel_row++)
        {
            SetRow(sel_row);    //row ����
            delay_us(10);       //�������� col �б� ���� 
            col = (PINC & 0x0f);
            switch(col)
            {
                case 0x01 : keypad_code = (sel_row - 1)*4 + 1;  break;  // 1�� col�� ����ġ�� ���� ���
                case 0x02 : keypad_code = (sel_row - 1)*4 + 2;  break;  // 2�� col�� ����ġ�� ���� ���
                case 0x04 : keypad_code = (sel_row - 1)*4 + 3;  break;  // 3�� col�� ����ġ�� ���� ���
                case 0x08 : keypad_code = (sel_row - 1)*4 + 4;  break;  // 4�� col�� ����ġ�� ���� ���
            }
        }
        
        if(keypad_code != 0xff)
        {
            fnd_value = keypad_code;    //Ű�е尡 �������� FND ǥ�� ���� ���� ����
        }  
        
    }

}
