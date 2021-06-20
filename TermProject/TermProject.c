/*
 * TermProject.c
 *
 * Created: 2021-06-07 ���� 2:21:45
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h> 


#define LCD_WDATA PORTA     // LCD ������ ��Ʈ ����
#define LCD_WINST PORTA
#define LCD_CTRL  PORTG     // LCD ������Ʈ ����
#define LCD_EN    0
#define LCD_RW    1
#define LCD_RS    2


// x : 0~3
#define SetKEYPADRow(x) PORTC |= 0xF0; PORTC &= ~(1 << ((x)+PORTC4));
#define SetFNDRow(x) PORTE&=~0xF0; PORTE|=(1<<((x)+PORTE4));



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

//---------------
unsigned char KEYPAD_SEL_rowIndex; //�а��� �ϴ� Ű�е��� �� ���� Index 
unsigned char KEYPAD_Pushed = 0;   // Ű�е� ��ü ������ ���ȴ��� �˻� ���� 
unsigned char KEYPAD_Code;    // ���� ������ Ű�е��� �ڵ� [0..15] 
unsigned char KEYPAD_hCode;   // ���ſ� ������ Ű�е��� �ڵ� [0..15]

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                                0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //�ֳ�� ����
                                
// FND�� ǥ���ϰ��� �ϴ� ���� ������ �ڸ����� ����Ű�� ���� ����
unsigned char FND_DISP_DATA[4] = {0,};
unsigned char FND_DISP_DigitCnt = 1;


interrupt [TIM2_COMP] void timer2_cmp(void)
{
    unsigned char keypadColData;
    PORTE &= ~0xF0;     //FND ����
    PORTB = Port_char[FND_DISP_DATA[FND_DISP_DigitCnt]];
    SetFNDRow(FND_DISP_DigitCnt);      // FND �� ���� 
    
    FND_DISP_DigitCnt++;
    if(FND_DISP_DigitCnt>3)
        FND_DISP_DigitCnt = 0;
    // FND_DISP_DigitCnt :: 1..4       
    
    // Ű�е� �д� �κ�
    keypadColData = (PINC & 0x0F);
    switch(keypadColData){
    case 0x01 : KEYPAD_Code = (KEYPAD_SEL_rowIndex)*4 + 0; break; // 0�� col�� ����ġ�� ���� ���
    case 0x02 : KEYPAD_Code = (KEYPAD_SEL_rowIndex)*4 + 1; break; // 1�� col�� ����ġ�� ���� ���
    case 0x04 : KEYPAD_Code = (KEYPAD_SEL_rowIndex)*4 + 2; break; // 2�� col�� ����ġ�� ���� ���
    case 0x08 : KEYPAD_Code = (KEYPAD_SEL_rowIndex)*4 + 3; break; // 3�� col�� ����ġ�� ���� ���
    }
    
    KEYPAD_SEL_rowIndex++;
    if(KEYPAD_SEL_rowIndex > 3) KEYPAD_SEL_rowIndex = 0;
    SetKEYPADRow(KEYPAD_SEL_rowIndex);
}

void Init_Timer2(void)      // Ÿ�̸�/ī����2 - 2�� ���� �� PC PWM ��Ƽ�� ����
{
    TIMSK |= (1 << OCIE2);  // ��º� ���ͷ�Ʈ2* �㰡 
    TCCR2 = (1 << WGM21) | (5 << CS20); // CTC���, 1024����
    OCR2 = 14;      // �� 1ms 
    // OCR2 = (14745600Hz / 1024 ���ֺ�) * 0.001ms = 14.4 
}

void FND_Disp(int dec) 
{
    // ǥ���� ���ڸ� �ڸ��� ���� �߶� FND_DISP_DATA[0..3] �� ���� 
    FND_DISP_DATA[3] = (dec/1000%10);     
    FND_DISP_DATA[2] = (dec/100%10);
    FND_DISP_DATA[1] = (dec/10%10);
    FND_DISP_DATA[0] = (dec/1%10);        
}

void Init_TimerINT(void)
{
    Init_Timer2();
    SREG |= 0X80;
}


void main(void)
{
    unsigned char str[] = "PASSWORD";  
    unsigned char str1[] = "Micom World";
    int i = 0;
    char keypadNbs[16] = {1,2,3,10,4,5,6,11,7,8,9,12,13,0,14,15};
    int keyData = 0;
    int keyNumber = 0; 
    
    PortInit();     // LCD ��� ��Ʈ ���� 
                     // �ʿ�� �ּ� ���� 
    LCD_Init();     // LCD �ʱ�ȭ         
    
    LCD_pos(0,1);   // LCD ������ 0�� 1�� ����
    LCD_STR(str);   // ���ڿ� str�� LCD ���  
    LCD_pos(1,1);   // LCD ������ 1�� 1�� ����
    LCD_STR(str1);

    
    // FND ���� ��Ʈ ����� ����
    DDRB = 0xFF;
    DDRE = 0xF0;  
    // KEYPAD ���� ����� ���� 
    DDRC = 0xF0;
    Init_TimerINT();    //Ÿ�̸� �ʱ�ȭ 
    while(1)
    {
        if(KEYPAD_hCode != KEYPAD_Code)
        {
            FND_Disp(KEYPAD_Code);
            KEYPAD_hCode = KEYPAD_Code;
        }
    }
}


