/*
 * timer_keypad_extension.c
 *
 * Created: 2021-06-16 오후 2:21:31
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>
// x : 0~3
#define SetKEYPADRow(x) PORTC |= 0xF0; PORTC &= ~(1 << ((x)+PORTC4));
#define SetFNDRow(x) PORTE&=~0xF0; PORTE|=(1<<((x)+PORTE4));

unsigned char KEYPAD_SEL_rowIndex; //읽고자 하는 키패드의 행 선택 Index 
unsigned char KEYPAD_Pushed = 0;   // 키패드 전체 읽으며 눌렸는지 검사 위함 
unsigned char KEYPAD_Code = -1;    // 현재 눌러진 키패드의 코드 [0..15] 
unsigned char KEYPAD_hCode = -1;   // 과거에 눌러진 키패드의 코드 [0..15]

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                                0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //애노드 공통
                                
// FND에 표시하고자 하는 숫자 정보와 자릿수를 가르키는 변수 생성
unsigned char FND_DISP_DATA[4] = {0,};
unsigned char FND_DISP_DigitCnt = 1;

unsigned char KEYPAD_SEL_rowindex;  // 읽고자 하는 키패드의 형 선택 index
unsigned char KEYPAD_Code;
unsigned char KEYPAD_hCode;

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
    
    KEYPAD_SEL_rowindex++;
    if(KEYPAD_SEL_rowindex > 3)
    {
        if(KEYPAD_Pushed == 0)
        {
            KEYPAD_Code = -1;
            KEYPAD_hCode = -1;
        }
        KEYPAD_SEL_rowIndex = 0;
        KEYPAD_Pushed = 0;
    }
    SetKEYPADRow(KEYPAD_SEL_rowIndex);
}

void Init_Timer2(void)      // 타이머/카운터2 - 2초 설정 및 PC PWM 듀티비 설정
{
    TIMSK |= (1 << OCIE2);  // 출력비교 인터럽트2 허가 
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
    int i = 0;
    char keypadNbs[16] = {1,2,3,10,4,5,6,11,7,8,9,12,13,0,14,15};
    int keyData = 0;
    int keyNumber = 0;
    
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
            KEYPAD_hCode = KEYPAD_Code;
            if(KEYPAD_Code == -1) 
            {
                continue;
            }
            if( (0<=keypadNbs[KEYPAD_Code]) && (keypadNbs[KEYPAD_Code]<10) )
            {
                keyNumber = keyNumber % 1000;
                keyNumber = keyNumber * 10;
                keyNumber += keypadNbs[KEYPAD_Code];
                FND_Disp(keyNumber);
            }
        }
    }
}
