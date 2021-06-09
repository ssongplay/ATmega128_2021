/*
 * w4_mission.c
 *
 * Created: 2021-04-01 오전 12:44:09
 * Author: syj47
 */    
         
 
/*  
    4주차 미션    
    
 - 기존 예제 5-3 을 활용하여 기존의 Switch Case 형태의 프로그램을 if else를 이용한 형태로 수정할것 

 - 이때 Pin Mask 기법을 이용하여 프로그램을 수정할 것(중요) 

 - 이때 스위치가 동시에 눌릴경우 낮은 번호의 스위치가 높은 우선순위로 동작 할 것

   (Ex PD4, PD6 이 눌러진 경우 -> PD4의 동작 수행)       
   
 */
            
 
 
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
        delay_ms(40);
        PORTB = LED;
        LED = (LED<<1) | 0x01;
        // 좌로 1비트 시프트한 후에 0x01값을 OR함으로써
        // 최하위비트 0의 값을 1로 세트함.
        delay_ms(40);
    }
}

void PB_RShift(void) // 비트가 오른쪽으로 이동 --> led는 왼쪽으로
{
    int i;
    unsigned char LED; // led 변수의 정의
    LED = 0xf7; // led 변수에 초기값 설정
    for(i=0; i<4;i++)
    {
        delay_ms(40);
        PORTB = LED;
        LED = (LED>>1) | 0x80;
        // 우로 1비트 시프트한 후에 0x80값을 OR함으로써
        // 최상위비트 0의 값을 1로 세트함.
        delay_ms(40);
    }
}  

void PB_LEDOnOff(void)
{
    unsigned char LED;
    
    LED = 0xf0; 
    PORTB = LED; 
    delay_ms(80);
    
    LED = 0xff; 
    PORTB = LED;
    delay_ms(80);      
    
}

void PB_LEDSwitch(void)
{
    unsigned char LED;
    
    LED = 0xfc; 
    PORTB = LED; 
    delay_ms(80);
    
    LED = 0xf3; 
    PORTB = LED; 
    delay_ms(80);   
  
}


void main(void)
{       
    DDRB = 0xff; // 모든 포트 출력으로 설정
    DDRD = 0x0f;
    
    while(1)
    {
        if(!(PIND & 0x10))    
        {
            PB_LShift();   
        }
        
        else if(!(PIND & 0x20))
        {
            PB_RShift(); 
        }      
        
        else if(!(PIND & 0x40))
        {
            PB_LEDOnOff();
        }
         
        else if(!(PIND & 0x80))
        {
            PB_LEDSwitch();  
        } 

    }
} 
