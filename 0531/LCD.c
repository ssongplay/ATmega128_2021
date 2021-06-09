/*
 * LCD.c
 *
 * Created: 2021-05-31 오후 2:55:58
 * Author: syj47
 */

// 강의자료 40p 
// ---------- 문자열을 LCD에 표시하는 프로그램의 작성 -------------

#include <mega128.h>
#include <delay.h>

#define LCD_WDATA PORTA     // LCD 데이터 포트 정의
#define LCD_WINST PORTA
#define LCD_CTRL  PORTG     // LCD 제어포트 정의
#define LCD_EN    0
#define LCD_RW    1
#define LCD_RS    2

void PortInit (void)
{
    DDRA = 0xFF;    // PORTA(LCD data pin)를 출력으로
    DDRG = 0x0F;    // PORTC의 하위 4비트를 출력으로 
}

void LCD_Data(unsigned char ch)     // LCD_DR에 데이터 출력
{
    LCD_CTRL |= (1 << LCD_RS);      // RS=1, =0으로 데이터 쓰기 사이클
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);      // LCD Enavle
    delay_us(50);
    LCD_WDATA = ch;                 // 데이터 출력
    delay_us(50);
    LCD_CTRL &= ~(1 << LCD_EN);     // LCD Disable
}

void LCD_Comm(unsigned char ch)     // LCD IR에 명령어 쓰기
{
    LCD_CTRL &= ~(1 << LCD_RS);     // RS==0으로 명령어 쓰기 사이클
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);      // LCD Enable
    delay_us(50);
    LCD_WINST = ch;                 // 명령어 쓰기 
    delay_us(50);        
    LCD_CTRL &= ~(1 << LCD_EN);     // LCD Disable
}

void LCD_delay(unsigned char ms)
{
    delay_ms(ms);
}

void LCD_CHAR(unsigned char c)      // 한 문자 출력
{
    LCD_Data(c);
    delay_ms(1);
}

void LCD_STR(unsigned char *str)    // 문자열 출력
{
    while(*str != 0)
    {
        LCD_CHAR(*str);
        str++;
    }
}

void LCD_pos(unsigned char row, unsigned char col)  // LCD 포지션 설정
{
    LCD_Comm(0x80 | ((row*0x40) + col) );       
}

void LCD_Clear(void)        //화면 클리어(1)
{
    LCD_Comm(0x01);
    LCD_delay(2);
}

void LCD_Init(void)     // LCD 초기화 
{
    LCD_Comm(0x30);     // 초기화 Set,
    delay_us(4100);     // 4.1ms 지연  
    LCD_Comm(0x30);     // 초기화 Set,
    delay_us(100);      // 100us 지연
    LCD_Comm(0x30);     // 초기화 Set,
    delay_us(100);      // 100us 지연
    LCD_Comm(0x38);     // 초기화 Set, 데이터길이 8bit, 표시라인 2행 사용을 위한 기능 설정 
    delay_us(1000);     // 명령을 처리하는데 최소 40us 지연이 발생하기에 여유를 고려하여 설정 
    LCD_Comm(0x0e);     // Display ON, Cursor ON, Blink OFF
    delay_us(1000);     // 40us 이상을 기다림 
    LCD_Comm(0x01);     // LCD Clear
    delay_us(2000);     // 1.64ms 이상을 기다림
    LCD_Comm(0x06);     // Cursor Entry Mode Set, 표시위치 +1씩 증가
    delay_us(1000);     // 40us 이상을 기다림 
}

void main(void)
{
    unsigned char str[] = "LCD Test..";  
    unsigned char str1[] = "Micom World";
    
    PortInit();     // LCD 출력 포트 설정 
                     // 필요시 주석 해제 
    LCD_Init();     // LCD 초기화         
    
    LCD_pos(0,1);   // LCD 포지션 0행 1열 지정
    LCD_STR(str);   // 문자열 str을 LCD 출력  
    LCD_pos(1,1);   // LCD 포지션 1행 1열 지정
    LCD_STR(str1);

    while(1);
}
