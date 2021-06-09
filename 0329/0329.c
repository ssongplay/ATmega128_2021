/*
 * 0329.c
 *
 * Created: 2021-03-29 오후 3:01:23
 * Author: syj47
 */  
       
        
 
 
// 예제 5-3: 입출력 결합 시험 (스위치 상태에 따른 switch 구문의 활용)
#include <mega128.h>
#include <delay.h>

void delay(unsigned int i)
{
    while(--i);
}

void PB_LShift(void)    //비트가 왼쪽으로 이동 --> led는 오른쪽으로
{
    int i;
    unsigned char LED; // led 변수의 정의
    LED = 0xfe; // led 변수에 초기값 설정
    for(i=0; i<4;i++)
    {
        delay_ms(200);
        PORTB = LED;
        LED = (LED<<1) | 0x01;
        // 좌로 1비트 시프트한 후에 0x01값을 OR함으로써
        // 최하위비트 0의 값을 1로 세트함.
        delay_ms(200);
    }
}

void PB_RShift(void) // 비트가 오른쪽으로 이동 --> led는 왼쪽으로
{
    int i;
    unsigned char LED; // led 변수의 정의
    LED = 0xf7; // led 변수에 초기값 설정
    for(i=0; i<4;i++)
    {
        delay_ms(200);
        PORTB = LED;
        LED = (LED>>1) | 0x80;
        // 우로 1비트 시프트한 후에 0x80값을 OR함으로써
        // 최상위비트 0의 값을 1로 세트함.
        delay_ms(200);
    }
}  

void PB_LEDOnOff(void)
{
    unsigned char LED;
    
    LED = 0xf0; 
    PORTB = LED; 
    delay_ms(200);
    
    LED = 0xff; 
    PORTB = LED;
    delay_ms(200);
      
    
}

void PB_LEDSwitch(void)
{
    unsigned char LED;
    
    LED = 0xfc; 
    PORTB = LED; 
    delay_ms(200);
    
    LED = 0xf3; 
    PORTB = LED; 
    delay_ms(200);   
  
}        


void main(void)
{
    unsigned char key; // key 변수의 정의
    DDRB = 0xff; // 모든 포트 출력으로 설정
    DDRD = 0x00; // 상위포트를 입력, 하위 포트를 출력으로 설정
   
    while(1)
    {
        //버튼스위치의 값을 읽기 위해서 PINB를 0xf0으로 AND하여 검사한다.     
        key = (PIND & 0xf0);     
        switch(key)
        {
            case 0xe0 : 
                PB_LShift();
                
                break;
            case 0xd0 :        
                PB_RShift();
                
                break;
            case 0xb0 :
                PB_LEDOnOff();
                
                break;
            case 0x70 :
                PB_LEDSwitch();
                
                break;
            default:
                break;
        }
    }
} 
 
 
 
 
// 방법 1
void main(void)
{
    unsigned char key; // key 변수의 정의
    DDRB = 0xff; // 모든 포트 출력으로 설정
    DDRD = 0x00; // 상위포트를 입력, 하위 포트를 출력으로 설정
   
    while(1)
    {
        key = (PIND & 0xf0);
        if(     (key & 0x10) = = 0x00){PB_LShift(); }
        else if((key & 0x20) = = 0x00){PB_RShift(); }
        else if((key & 0x40) = = 0x00){PB_LEDOnOff(); }
        else if((key & 0x80) = = 0x00){PB_LEDSwitch(); } 
    }
}  

// 방법 2
void main(void)
{
    unsigned char key; // key 변수의 정의
    DDRB = 0xff; // 모든 포트 출력으로 설정
    DDRD = 0x00; // 상위포트를 입력, 하위 포트를 출력으로 설정
   
    while(1)
    {
        key = (PIND & 0xf0);
        if(     !(key & (1<<PORTD4))) {PB_LShift(); }
        else if(!(key & (1<<PORTD5))) {PB_RShift(); }
        else if(!(key & (1<<PORTD6))){PB_LEDOnOff(); }
        else if(!(key & (1<<PORTD7))){PB_LEDSwitch(); } 
    }
}  
 
 
 
 
 

// 예제 5-2: LED를 좌우로 시프트하여 구동하기(LR_Shift 함수 작성) 

#include <mega128.h>
#include <delay.h>

void delay(unsigned int i)
{
    while(--i);
}

void PB_LShift(void)
{
    int i;
    unsigned char LED; // led 변수의 정의
    LED = 0xfe;// led 변수에 초기값 설정
    for(i=0; i<8;i++)
    {
        delay_ms(200);
        PORTB = LED;
        LED = (LED<<1) | 0x01;
        // 좌로 1비트 시프트한 후에 0x01값을 OR함으로써
        // 최하위비트 0의 값을 1로 세트함.
        delay_ms(200);
    }
}

void PB_RShift(void) // 예제 5.1을 활용하여 오른쪽 시프트 함수 작성
{
    int i;
    unsigned char LED; // led 변수의 정의
    LED = 0x7f; // led 변수에 초기값 설정
    for(i=0; i<8;i++)
    {
        delay_ms(200);
        PORTB = LED;
        LED = (LED>>1) | 0x80;
        // 좌로 1비트 시프트한 후에 0x01값을 OR함으로써
        // 최하위비트 0의 값을 1로 세트함.
        delay_ms(200);
    }
}

void PB_LRShift(void)
// PB_LShift와 PB_Rshift 함수를 활용하여 좌우 시프트 함수 작성
{
    PB_LShift();
    PB_RShift();
}


void main(void)
{
    DDRB = 0xff; // PB 포트를 모두 출력으로 설정
    PORTB = 0xff; // 모든 LED를 Off
    while(1)
    {
    PB_LRShift();
    }
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
// 예제 5-1: LED를 시프트하여 구동하기 (PB_LShift 함수 작성) 
//#include <mega128.h>
//#include <delay.h>
//
//void delay(unsigned int i)
//{
//    while(--i);
//}
//
//void PB_LShift(void)
//{
//    int i;
//    unsigned char LED; // led 변수의 정의
//    LED = 0xfe;// led 변수에 초기값 설정
//    for(i=0; i<8;i++)
//    {
//        delay_ms(200);
//        PORTB = LED;
//        LED = (LED<<1) | 0x01;
//        // 좌로 1비트 시프트한 후에 0x01값을 OR함으로써
//        // 최하위비트 0의 값을 1로 세트함.
//        delay_ms(200);
//    }
//}
//
//void main(void)
//{
//    DDRB = 0xff; // PB 포트를 모두 출력으로 설정
//    PORTB = 0xff; // 모든 LED를 Off
//    while(1)
//    {
//    PB_LShift();
//    delay_ms(200);
//    }
//}











//#include <mega128.h>
//#include <delay.h>
//
//void delay(unsigned int cnt)
//{
//    while(cnt--);
//}
//    
//
//void main(void)
//{
//    // PORTB의 0번을 출력으로 설정
//     
//    // DDRB = 0b00000001;  //0x01;  
//    DDRB = (1<<PORTB0);  //0x01;
//    PORTB &= ~(1<<PORTB0);
//    //PORTB = PORTB & (~(1<<0));
//    
//    while (1)
//    {
//        if(PIND & (1<<PORTD3))
//        {
//        PORTB &= ~(1<<PORTB0);
//        }
//        else
//        {
//        PORTB |= (1<<PORTB0);
//        }
//    }
//}
