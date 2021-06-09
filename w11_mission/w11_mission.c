/*
 * w11_mission.c
 *
 * Created: 2021-05-31 오후 4:54:10
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>

#define LCD_WDATA PORTA     // LCD 데이터 버스 정의 (데이터 쓰기)
#define LCD_WINST PORTA     // LCD 데이터 버스 정의 (명령어 쓰기)
#define LCD_CTRL  PORTG     // LCD 제어포트 정의
#define LCD_EN    0         // Enable 신호
#define LCD_RW    1         // 읽기(1) / 쓰기(0)
#define LCD_RS    2         // 데이터(1) / 명령어(0)

void PortInit (void)
{
    DDRA = 0xFF;    // PORTA(LCD data pin)를 출력으로
    DDRG = 0x0F;    // PORTG의 하위 4비트를 출력으로 
}

void LCD_Data(unsigned char ch)     // LCD_DR에 데이터 출력
{
    LCD_CTRL |= (1 << LCD_RS);      // RS=1, =0으로 데이터 쓰기 사이클
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);      // LCD Enable
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

void LCD_Init(void)     // LCD 초기화 
{
    LCD_Comm(0x30);     // 초기화 Set,
    delay_us(4100);     // 4.1ms 지연  
    LCD_Comm(0x30);     // 초기화 Set,
    delay_us(100);      // 100us 지연
    LCD_Comm(0x30);     // 초기화 Set,
    delay_us(100);      // 100us 지연
    LCD_Comm(0x3c);     // 초기화 Set, 데이터길이 8bit, 표시라인 2행 사용을 위한 기능 설정 
    delay_us(1000);     // 명령을 처리하는데 최소 40us 지연이 발생하기에 여유를 고려하여 설정 
    LCD_Comm(0x0c);     // Display ON, Cursor OFF, Blink OFF
    delay_us(1000);     // 40us 이상을 기다림 
    LCD_Comm(0x01);     // LCD Clear
    delay_us(2000);     // 1.64ms 이상을 기다림
    LCD_Comm(0x06);     // Cursor Entry Mode Set, 표시위치 +1씩 증가
    delay_us(1000);     // 40us 이상을 기다림 
}


void CGRAM_Set()
{
    int i;
    unsigned char song[] = {0x04, 0x0a, 0x11, 0x04, 0x1f, 0x0e, 0x11, 0x0e};
    unsigned char yeon[] = {0x01, 0x0b, 0x15, 0x15, 0x0b, 0x01, 0x10, 0x1f};
    unsigned char joo[] = {0x1f, 0x04, 0x0a, 0x11, 0x00, 0x1f, 0x04, 0x04};
    
//------------------------- 송 -------------------------------------  
    // CGRAM 사용 (DB6 = set) 주소 설정 : CGRAM 0번지 (0bx1000xxx)
    LCD_delay(1);
    LCD_Comm(0x40);       // CGRAM address set
    LCD_delay(1);
    
    // CGRAM에 Font 데이터를 넣는 부분 (CGRAM ADDR : 0x40..7)
    for(i=0;i<8;i++)
    {
        LCD_CHAR(song[i]);
        LCD_delay(1);
    }  
     
//------------------------- 연 -------------------------------------      
    // CGRAM 사용 (DB6 = set) 주소 설정 : CGRAM 1번지 (0bx1001xxx)
    LCD_Comm(0x48);       // CGRAM address set
    LCD_delay(1);
    
    // CGRAM에 Font 데이터를 넣는 부분 (CGRAM ADDR : 0x48..F)
    for(i=0;i<8;i++)
    {
        LCD_CHAR(yeon[i]);
        LCD_delay(1);
    }  
    
//------------------------- 주 -------------------------------------      
    // CGRAM 사용 (DB6 = set) 주소 설정 : CGRAM 2번지 (0bx1010xxx)
    LCD_Comm(0x50);       // CGRAM address set
    LCD_delay(1);
    
    // CGRAM에 Font 데이터를 넣는 부분 (CGRAM ADDR : 0x50..7)
    for(i=0;i<8;i++)
    {
        LCD_CHAR(joo[i]);
        LCD_delay(1);
    }
}

void main(void)
{
    unsigned char student_id[] = "2019146018"; 
    
    PortInit();             // LCD 출력 포트 설정 
    LCD_Init();             // LCD 초기화     
    LCD_Comm(0x0C);         // display ON, cursor OFF, Blink OFF --> 5x8 문자표현
    delay_us(1000);
    
    LCD_pos(0,0);           // LCD 포지션 1행 1열 지정
    LCD_STR(student_id);    // 문자열 student_id LCD 출력   
    
    CGRAM_Set();  
    LCD_pos(1,0);           // LCD 포지션 2행 1열 지정
    LCD_CHAR(0x00);        // 송
    LCD_CHAR(0x01);        // 연 
    LCD_CHAR(0x02);        // 주

    while(1);
}
