/*
 * w11_mission.c
 *
 * Created: 2021-05-31 ���� 4:54:10
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>

#define LCD_WDATA PORTA     // LCD ������ ���� ���� (������ ����)
#define LCD_WINST PORTA     // LCD ������ ���� ���� (��ɾ� ����)
#define LCD_CTRL  PORTG     // LCD ������Ʈ ����
#define LCD_EN    0         // Enable ��ȣ
#define LCD_RW    1         // �б�(1) / ����(0)
#define LCD_RS    2         // ������(1) / ��ɾ�(0)

void PortInit (void)
{
    DDRA = 0xFF;    // PORTA(LCD data pin)�� �������
    DDRG = 0x0F;    // PORTG�� ���� 4��Ʈ�� ������� 
}

void LCD_Data(unsigned char ch)     // LCD_DR�� ������ ���
{
    LCD_CTRL |= (1 << LCD_RS);      // RS=1, =0���� ������ ���� ����Ŭ
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);      // LCD Enable
    delay_us(50);
    LCD_WDATA = ch;                 // ������ ���
    delay_us(50);
    LCD_CTRL &= ~(1 << LCD_EN);     // LCD Disable
}

void LCD_Comm(unsigned char ch)     // LCD IR�� ��ɾ� ����
{
    LCD_CTRL &= ~(1 << LCD_RS);     // RS==0���� ��ɾ� ���� ����Ŭ
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);      // LCD Enable
    delay_us(50);
    LCD_WINST = ch;                 // ��ɾ� ���� 
    delay_us(50);        
    LCD_CTRL &= ~(1 << LCD_EN);     // LCD Disable
}

void LCD_delay(unsigned char ms)
{
    delay_ms(ms);
}

void LCD_CHAR(unsigned char c)      // �� ���� ���
{
    LCD_Data(c);
    delay_ms(1);
}

void LCD_STR(unsigned char *str)    // ���ڿ� ���
{
    while(*str != 0)
    {
        LCD_CHAR(*str);
        str++;
    }
}

void LCD_pos(unsigned char row, unsigned char col)  // LCD ������ ����
{
    LCD_Comm(0x80 | ((row*0x40) + col) );       
}

void LCD_Init(void)     // LCD �ʱ�ȭ 
{
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(4100);     // 4.1ms ����  
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(100);      // 100us ����
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(100);      // 100us ����
    LCD_Comm(0x3c);     // �ʱ�ȭ Set, �����ͱ��� 8bit, ǥ�ö��� 2�� ����� ���� ��� ���� 
    delay_us(1000);     // ����� ó���ϴµ� �ּ� 40us ������ �߻��ϱ⿡ ������ ����Ͽ� ���� 
    LCD_Comm(0x0c);     // Display ON, Cursor OFF, Blink OFF
    delay_us(1000);     // 40us �̻��� ��ٸ� 
    LCD_Comm(0x01);     // LCD Clear
    delay_us(2000);     // 1.64ms �̻��� ��ٸ�
    LCD_Comm(0x06);     // Cursor Entry Mode Set, ǥ����ġ +1�� ����
    delay_us(1000);     // 40us �̻��� ��ٸ� 
}


void CGRAM_Set()
{
    int i;
    unsigned char song[] = {0x04, 0x0a, 0x11, 0x04, 0x1f, 0x0e, 0x11, 0x0e};
    unsigned char yeon[] = {0x01, 0x0b, 0x15, 0x15, 0x0b, 0x01, 0x10, 0x1f};
    unsigned char joo[] = {0x1f, 0x04, 0x0a, 0x11, 0x00, 0x1f, 0x04, 0x04};
    
//------------------------- �� -------------------------------------  
    // CGRAM ��� (DB6 = set) �ּ� ���� : CGRAM 0���� (0bx1000xxx)
    LCD_delay(1);
    LCD_Comm(0x40);       // CGRAM address set
    LCD_delay(1);
    
    // CGRAM�� Font �����͸� �ִ� �κ� (CGRAM ADDR : 0x40..7)
    for(i=0;i<8;i++)
    {
        LCD_CHAR(song[i]);
        LCD_delay(1);
    }  
     
//------------------------- �� -------------------------------------      
    // CGRAM ��� (DB6 = set) �ּ� ���� : CGRAM 1���� (0bx1001xxx)
    LCD_Comm(0x48);       // CGRAM address set
    LCD_delay(1);
    
    // CGRAM�� Font �����͸� �ִ� �κ� (CGRAM ADDR : 0x48..F)
    for(i=0;i<8;i++)
    {
        LCD_CHAR(yeon[i]);
        LCD_delay(1);
    }  
    
//------------------------- �� -------------------------------------      
    // CGRAM ��� (DB6 = set) �ּ� ���� : CGRAM 2���� (0bx1010xxx)
    LCD_Comm(0x50);       // CGRAM address set
    LCD_delay(1);
    
    // CGRAM�� Font �����͸� �ִ� �κ� (CGRAM ADDR : 0x50..7)
    for(i=0;i<8;i++)
    {
        LCD_CHAR(joo[i]);
        LCD_delay(1);
    }
}

void main(void)
{
    unsigned char student_id[] = "2019146018"; 
    
    PortInit();             // LCD ��� ��Ʈ ���� 
    LCD_Init();             // LCD �ʱ�ȭ     
    LCD_Comm(0x0C);         // display ON, cursor OFF, Blink OFF --> 5x8 ����ǥ��
    delay_us(1000);
    
    LCD_pos(0,0);           // LCD ������ 1�� 1�� ����
    LCD_STR(student_id);    // ���ڿ� student_id LCD ���   
    
    CGRAM_Set();  
    LCD_pos(1,0);           // LCD ������ 2�� 1�� ����
    LCD_CHAR(0x00);        // ��
    LCD_CHAR(0x01);        // �� 
    LCD_CHAR(0x02);        // ��

    while(1);
}
