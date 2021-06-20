/*
 * keyscan.c
 *
 * Created: 2021-06-20 오후 3:31:32
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>

#define FND_Null    0
#define FND_Star    14
#define FND_Sharp   15

unsigned int Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,
                                0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; //애노드 공통      
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f, 0x0f};
unsigned char key_num = 0;
unsigned int buf_seg[4] = {FND_Null, FND_Null, FND_Null, FND_Null};
// bit Key_off_flag = 0;

void init_reg(void)
{
    DDRE = 0xf0;    // FND Sel
    DDRB = 0xff;    // FND Data Line
    
    DDRC = 0xf0;    //상위 4비트 Row
    PORTC = 0xff;   // 포트 초기화
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

unsigned char KeyScan(void) //4x4 키패드 스캔 함수, 출력값은 10진수 1~16
{
    // 상위 니블(4비트)을 스위칭(연속적으로 돌아가면서)하면서 출력
    unsigned int key_scan_line = 0xEF;
    
    // 키 스캔 라인 변경을 위한 반복문 인자, 키 매트릭스 열의 입력 값   
    unsigned char key_scan_loop = 0, getPinData=0;      
    
    // 키 스캔 반복문
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
    
    