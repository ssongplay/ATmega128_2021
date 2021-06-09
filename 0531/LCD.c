/*
 * LCD.c
 *
 * Created: 2021-05-31 ���� 2:55:58
 * Author: syj47
 */

// �����ڷ� 40p 
// ---------- ���ڿ��� LCD�� ǥ���ϴ� ���α׷��� �ۼ� -------------

#include <mega128.h>
#include <delay.h>

#define LCD_WDATA PORTA     // LCD ������ ��Ʈ ����
#define LCD_WINST PORTA
#define LCD_CTRL  PORTG     // LCD ������Ʈ ����
#define LCD_EN    0
#define LCD_RW    1
#define LCD_RS    2

void PortInit (void)
{
    DDRA = 0xFF;    // PORTA(LCD data pin)�� �������
    DDRG = 0x0F;    // PORTC�� ���� 4��Ʈ�� ������� 
}

void LCD_Data(unsigned char ch)     // LCD_DR�� ������ ���
{
    LCD_CTRL |= (1 << LCD_RS);      // RS=1, =0���� ������ ���� ����Ŭ
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);      // LCD Enavle
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

void LCD_Clear(void)        //ȭ�� Ŭ����(1)
{
    LCD_Comm(0x01);
    LCD_delay(2);
}

void LCD_Init(void)     // LCD �ʱ�ȭ 
{
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(4100);     // 4.1ms ����  
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(100);      // 100us ����
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(100);      // 100us ����
    LCD_Comm(0x38);     // �ʱ�ȭ Set, �����ͱ��� 8bit, ǥ�ö��� 2�� ����� ���� ��� ���� 
    delay_us(1000);     // ����� ó���ϴµ� �ּ� 40us ������ �߻��ϱ⿡ ������ ����Ͽ� ���� 
    LCD_Comm(0x0e);     // Display ON, Cursor ON, Blink OFF
    delay_us(1000);     // 40us �̻��� ��ٸ� 
    LCD_Comm(0x01);     // LCD Clear
    delay_us(2000);     // 1.64ms �̻��� ��ٸ�
    LCD_Comm(0x06);     // Cursor Entry Mode Set, ǥ����ġ +1�� ����
    delay_us(1000);     // 40us �̻��� ��ٸ� 
}

void main(void)
{
    unsigned char str[] = "LCD Test..";  
    unsigned char str1[] = "Micom World";
    
    PortInit();     // LCD ��� ��Ʈ ���� 
                     // �ʿ�� �ּ� ���� 
    LCD_Init();     // LCD �ʱ�ȭ         
    
    LCD_pos(0,1);   // LCD ������ 0�� 1�� ����
    LCD_STR(str);   // ���ڿ� str�� LCD ���  
    LCD_pos(1,1);   // LCD ������ 1�� 1�� ����
    LCD_STR(str1);

    while(1);
}
