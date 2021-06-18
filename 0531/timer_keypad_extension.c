/*
 * timer_keypad_extension.c
 *
 * Created: 2021-06-16 ���� 2:21:31
 * Author: syj47
 */

#include <mega128.h>
#include <delay.h>
// x : 0~3
#define SetKEYPADRow(x) PORTC |= 0xF0; PORTC &= ~(1 << ((x)+PORTC4));
#define SetFNDRow(x) PORTE&=~0xF0; PORTE|=(1<<((x)+PORTE4));

unsigned char KEYPAD_SEL_rowIndex; //�а��� �ϴ� Ű�е��� �� ���� Index 
unsigned char KEYPAD_Pushed = 0;   // Ű�е� ��ü ������ ���ȴ��� �˻� ���� 
unsigned char KEYPAD_Code = -1;    // ���� ������ Ű�е��� �ڵ� [0..15] 
unsigned char KEYPAD_hCode = -1;   // ���ſ� ������ Ű�е��� �ڵ� [0..15]

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                                0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //�ֳ�� ����
                                
// FND�� ǥ���ϰ��� �ϴ� ���� ������ �ڸ����� ����Ű�� ���� ����
unsigned char FND_DISP_DATA[4] = {0,};
unsigned char FND_DISP_DigitCnt = 1;

unsigned char KEYPAD_SEL_rowindex;  // �а��� �ϴ� Ű�е��� �� ���� index
unsigned char KEYPAD_Code;
unsigned char KEYPAD_hCode;

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

void Init_Timer2(void)      // Ÿ�̸�/ī����2 - 2�� ���� �� PC PWM ��Ƽ�� ����
{
    TIMSK |= (1 << OCIE2);  // ��º� ���ͷ�Ʈ2 �㰡 
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
    int i = 0;
    char keypadNbs[16] = {1,2,3,10,4,5,6,11,7,8,9,12,13,0,14,15};
    int keyData = 0;
    int keyNumber = 0;
    
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
