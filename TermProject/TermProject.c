/*
 * TermProject.c
 *
 * Created: 2021-06-07 오후 2:21:45
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h> 


#define LCD_WDATA PORTA     // LCD 데이터 포트 정의
#define LCD_WINST PORTA
#define LCD_CTRL  PORTG     // LCD 제어포트 정의
#define LCD_EN    0
#define LCD_RW    1
#define LCD_RS    2


// x : 0~3
#define SetKEYPADRow(x) PORTC |= 0xF0; PORTC &= ~(1 << ((x)+PORTC4));
#define SetFNDRow(x) PORTE&=~0xF0; PORTE|=(1<<((x)+PORTE4));



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

//---------------
unsigned char KEYPAD_SEL_rowIndex; //읽고자 하는 키패드의 행 선택 Index 
unsigned char KEYPAD_Pushed = 0;   // 키패드 전체 읽으며 눌렸는지 검사 위함 
unsigned char KEYPAD_Code;    // 현재 눌러진 키패드의 코드 [0..15] 
unsigned char KEYPAD_hCode;   // 과거에 눌러진 키패드의 코드 [0..15]

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                                0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //애노드 공통
                                
// FND에 표시하고자 하는 숫자 정보와 자릿수를 가르키는 변수 생성
unsigned char FND_DISP_DATA[4] = {0,};
unsigned char FND_DISP_DigitCnt = 1;


interrupt [TIM2_COMP] void timer2_cmp(void)
{
    unsigned char keypadColData;
    PORTE &= ~0xF0;     //FND 끄기
    PORTB = Port_char[FND_DISP_DATA[FND_DISP_DigitCnt]];
    SetFNDRow(FND_DISP_DigitCnt);      // FND 열 선택 
    
    FND_DISP_DigitCnt++;
    if(FND_DISP_DigitCnt>3)
        FND_DISP_DigitCnt = 0;
    // FND_DISP_DigitCnt :: 1..4       
    
    // 키패드 읽는 부분
    keypadColData = (PINC & 0x0F);
    switch(keypadColData){
    case 0x01 : KEYPAD_Code = (KEYPAD_SEL_rowIndex)*4 + 0; break; // 0번 col의 스위치가 눌린 경우
    case 0x02 : KEYPAD_Code = (KEYPAD_SEL_rowIndex)*4 + 1; break; // 1번 col의 스위치가 눌린 경우
    case 0x04 : KEYPAD_Code = (KEYPAD_SEL_rowIndex)*4 + 2; break; // 2번 col의 스위치가 눌린 경우
    case 0x08 : KEYPAD_Code = (KEYPAD_SEL_rowIndex)*4 + 3; break; // 3번 col의 스위치가 눌린 경우
    }
    
    KEYPAD_SEL_rowIndex++;
    if(KEYPAD_SEL_rowIndex > 3) KEYPAD_SEL_rowIndex = 0;
    SetKEYPADRow(KEYPAD_SEL_rowIndex);
}

void Init_Timer2(void)      // 타이머/카운터2 - 2초 설정 및 PC PWM 듀티비 설정
{
    TIMSK |= (1 << OCIE2);  // 출력비교 인터럽트2* 허가 
    TCCR2 = (1 << WGM21) | (5 << CS20); // CTC모드, 1024분주
    OCR2 = 14;      // 약 1ms 
    // OCR2 = (14745600Hz / 1024 분주비) * 0.001ms = 14.4 
}

void FND_Disp(int dec) 
{
    // 표시할 숫자를 자릿수 별로 잘라 FND_DISP_DATA[0..3] 에 저장 
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
    
    PortInit();     // LCD 출력 포트 설정 
                     // 필요시 주석 해제 
    LCD_Init();     // LCD 초기화         
    
    LCD_pos(0,1);   // LCD 포지션 0행 1열 지정
    LCD_STR(str);   // 문자열 str을 LCD 출력  
    LCD_pos(1,1);   // LCD 포지션 1행 1열 지정
    LCD_STR(str1);

    
    // FND 관련 포트 입출력 설정
    DDRB = 0xFF;
    DDRE = 0xF0;  
    // KEYPAD 관련 입출력 설정 
    DDRC = 0xF0;
    Init_TimerINT();    //타이머 초기화 
    while(1)
    {
        if(KEYPAD_hCode != KEYPAD_Code)
        {
            FND_Disp(KEYPAD_Code);
            KEYPAD_hCode = KEYPAD_Code;
        }
    }
}


