/*
 * read1row.c
 *
 * Created: 2021-04-12 오후 3:04:49
 * Author: syj47
 */
    
 
// 1열 읽기 
#include <mega128.h>
#include <delay.h>
#define PIN_ROW1 PORTC4
#define PIN_ROW2 PORTC5
#define PIN_ROW3 PORTC6
#define PIN_ROW4 PORTC7
//Test Ex. ROW 1개 선택하여 col 읽기
// 예상결과 : 키패드 하나의 row만 선택되어 16개중 4개만 반응함. 


void main(void)
{
    DDRB = 0xFF;    //LED 확인 위하여 PORTB 모든비트 출력으로 설정 
    DDRC = 0xF0;    // PORTC 4~7은 ROW 선택 위한 출력으로 설정 
                    // PORTC 0~3은 COL 값 읽기 위한 입력으로 설정
    PORTC |= 0xF0;  // PORTC 4~7에 HIGH 출력으로 ROW 선택 안함.     
    PORTC &= ~(1<<PIN_ROW1);      // PORTC 4에 LOW 출력으로 ROW1 선택 
    
while (1)
    {
        PORTB = PINC;      // COL의 값을 PORTB의 LED에 출력 
    }
}





// ------------------------------------------------------------



// 1열 읽기 - 함수를 이용하는 방법
#include <mega128.h>
#include <delay.h>
#define PIN_ROW1 PORTC4
#define PIN_ROW2 PORTC5
#define PIN_ROW3 PORTC6
#define PIN_ROW4 PORTC7
//Test Ex. ROW 1개 선택하여 col 읽기
// 예상결과 : 키패드 하나의 row만 선택되어 16개중 4개만 반응함. 

void SetRow(unsigned char row)
{
    PORTC |= 0xf0;      //PORTC 4~7에 HIGH출력으로 row 선택 안함
    switch (row)
    {
        case 1 : PORTC &= ~(1<<PIN_ROW1);   break;
        case 2 : PORTC &= ~(1<<PIN_ROW2);   break;
        case 3 : PORTC &= ~(1<<PIN_ROW3);   break;
        case 4 : PORTC &= ~(1<<PIN_ROW4);   break;
    }
}

void main()
{
    DDRB = 0xFF;    //LED 확인 위하여 PORTB 모든비트 출력으로 설정 
    DDRC = 0xF0;    // PORTC 4~7은 ROW 선택 위한 출력으로 설정 
                    // PORTC 0~3은 COL 값 읽기 위한 입력으로 설정
    SetRow(1);      // row 1번 선택 
    while(1)
    {
        PORTB = PINC;   // COL의 값을 PORTB의 LED에 출력
    }
}



// ------------------------------------------------------------------      



// 1열 읽기 - 매크로를 이용하는 방법 1
#include <mega128.h>
#include <delay.h>
// x : 1 ~4
#define SetRow(x)   PORTC |= 0xf0; PORTC &= ~(1<<((x-1)+PORTD4));

void main()
{
    DDRB = 0xFF;    //LED 확인 위하여 PORTB 모든비트 출력으로 설정 
    DDRC = 0xF0;    // PORTC 4~7은 ROW 선택 위한 출력으로 설정 
                    // PORTC 0~3은 COL 값 읽기 위한 입력으로 설정
    SetRow(1);      // row 1번 선택 
    while(1)
    {
        PORTB = PINC;   // COL의 값을 PORTB의 LED에 출력
    }
}





// ------------------------------------------------------------------      



// 1열 읽기 - 매크로를 이용하는 방법 2
#include <mega128.h>
#include <delay.h>
// x : PIN_ROW1  ~ PIN_ROW4
#define SetRow(x)   PORTC |= 0xf0; PORTC &= ~(1<<(x));

void main()
{
    DDRB = 0xFF;    //LED 확인 위하여 PORTB 모든비트 출력으로 설정 
    DDRC = 0xF0;    // PORTC 4~7은 ROW 선택 위한 출력으로 설정 
                    // PORTC 0~3은 COL 값 읽기 위한 입력으로 설정
    SetRow(PIN_ROW1);      // row 1번 선택 
    while(1)
    {
        PORTB = PINC;   // COL의 값을 PORTB의 LED에 출력
    }
}


