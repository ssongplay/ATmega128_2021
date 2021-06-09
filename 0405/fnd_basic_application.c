/*
 * fnd_basic_application.c
 *
 * Created: 2021-04-05 오후 3:22:26
 * Author: syj47
 */

// 예제 FND 기본 구동 응용
// PORTD.4에 연결된 DIP스위치가 ON되면, 0.5초마다 FND 1 증가
// PORTD.5에 연결된 DIP스위치가 ON되면, 0.5초마다 FND 3 증가
// PORTD.6에 연결된 DIP스위치가 ON되면, 0.5초마다 FND 1 감소 
// PORTD.7에 연결된 DIP스위치가 ON되면, 0.5초마다 FND 3 감소 
// 이때 숫자의 표시 범위는 0 ~ 9 (음수값이 될 경우 9로 변경. -2 -> 8)

#include <mega128.h>
#include <delay.h>

unsigned char num_data[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                            0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //애노드 공통


void main(void)
{
    int i=0;
    DDRB = 0xff; // 모든 포트 출력으로 설정
  
    while(1)
    {
        if(      !(PIND & (1<<PORTD4))){ i+=1; delay_ms(500); }
        else if( !(PIND & (1<<PORTD5))){ i+=3; delay_ms(500); }
        else if( !(PIND & (1<<PORTD6))){ i-=1; delay_ms(500); } 
        else if( !(PIND & (1<<PORTD7))){ i-=3; delay_ms(500); }
        
        if(i<0) i+=10; 
        if(i>10) i-=10;
        
        PORTB = num_data[i]  
    }
} 