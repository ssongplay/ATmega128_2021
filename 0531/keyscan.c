/*
 * keyscan.c
 *
 * Created: 2021-06-20 ���� 3:31:32
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>

#define FND_Null    0
#define FND_Star    14
#define FND_Sharp   15

unsigned int Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,
                                0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; //�ֳ�� ����      
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f, 0x0f};
unsigned char key_num = 0;
unsigned int buf_seg[4] = {FND_Null, FND_Null, FND_Null, FND_Null};
// bit Key_off_flag = 0;

void init_reg(void)
{
    DDRE = 0xf0;    // FND Sel
    DDRB = 0xff;    // FND Data Line
    
    DDRC = 0xf0;    //���� 4��Ʈ Row
    PORTC = 0xff;   // ��Ʈ �ʱ�ȭ
    PORTE = Port_fnd[4];    //ALL FND OFF
}

void Print_Segment(unsigned int* seg_value)
{
    PORTE = Port_fnd[0];
    PORTB = Port_char[seg_value[0]];
    delay_ms(1);
    
    PORTE = Port_fnd[1];
    PORTB = Port_char[seg_value[1]];
    delay_ms(1);   
    
    PORTE = Port_fnd[2];
    PORTB = Port_char[seg_value[2]];
    delay_ms(1);
    
    PORTE = Port_fnd[3];
    PORTB = Port_char[seg_value[3]];
    delay_ms(1);
}

unsigned char KeyScan(void) //4x4 Ű�е� ��ĵ �Լ�, ��°��� 10���� 1~16
{
    // ���� �Ϻ�(4��Ʈ)�� ����Ī(���������� ���ư��鼭)�ϸ鼭 ���
    unsigned int key_scan_line = 0xEF;
    
    // Ű ��ĵ ���� ������ ���� �ݺ��� ����, Ű ��Ʈ���� ���� �Է� ��   
    unsigned char key_scan_loop = 0, getPinData=0;      
    
    // Ű ��ĵ �ݺ���
    for(key_scan_loop=0; key_scan_loop<4; key_scan_loop++)
    {
        PORTC = key_scan_line;
        delay_us(1);
        
        getPinData = PINC & 0x0f;
        if(getPinData != 0x00)
        {
            switch(getPinData)
            {
                case 0x01: key_num = key_scan_loop*4 + 1; break;
                case 0x02: key_num = key_scan_loop*4 + 2; break; 
                case 0x04: key_num = key_scan_loop*4 + 3; break;
                case 0x08: key_num = key_scan_loop*4 + 4; break;
            }
            return key_num;
        }
        key_scan_line = (key_scan_line << 1);
        delay_us(1);
    }
}

unsigned char Key_data_trans(unsigned char New_key_data)
{
    unsigned char key_num = 0;
    if(New_key_data%4 != 0)
    {
        key_num = (New_key_data/4)*3 + (New_key_data%4);   
        switch(key_num)
        {
            case 10: key_num = FND_Star; break;
            case 11: key_num = FND_Null; break;
            case 12: key_num = FND_Sharp; break;
        }
    }
    else
        key_num = (New_key_data/4)+9;
    return key_num;
}

void Key_pad_in(unsigned char* New_key_data, unsigned char* Key_off_flag)
{
    if(*New_key_data)        
    {
        if(*Key_off_flag)
        {
            buf_seg[3] = buf_seg[2];
            buf_seg[2] = buf_seg[1];
            buf_seg[1] = buf_seg[0];
            *Key_off_flag = ~*Key_off_flag;
        }
        else
        buf_seg[0] = Key_data_trans(*New_key_data);
    }
    else
        *Key_off_flag=0xff;
}

void main(void)
{
    unsigned char Key_off_flag = 0;
    unsigned char New_key_data = 0;
    
    init_reg();
    while(1)
    {
        New_key_data = KeyScan();      
        Key_pad_in(&New_key_data, &Key_off_flag);
        Print_Segment(buf_seg);
    }
        
}        
    
    