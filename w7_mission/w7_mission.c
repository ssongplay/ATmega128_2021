/*
 * w7_mission.c
 *
 * Created: 2021-05-01 오후 9:56:50
 * Author: syj47
 */

/* 
-----------------7주차 미션-----------------------------
-------------2019146018 송연주---------------------------
> MAIN 함수 내부에는 스위치 읽는 부분이 없을 것
> MAIN 함수 내부에는 FND의 출력만 수행할 것
> 스위치를 누르는 순간 동작하도록 하며, 누르고 있는 중간 및 떼는 순간의 이벤트는 없음
> 이때 스위치의 누름과 무관하게 FND는 지속적으로 동작하여야함
> 스위치 누름이 감지되는 경우 부저 출력(4옥타브 SI)을 1회/10ms 수행(이때, FND 정지 허용)
> PORTD.0에 연결된 스위치가 ON되면 FND 1증가
> PORTD.1에 연결된 스위치가 ON되면 FND 4증가
> PORTD.2에 연결된 스위치가 ON되면 FND 1감소
> PORTD.3에 연결된 스위치가 ON되면 FND 5감소 
----------------------------------------------------------
*/ 

#include <mega128.h>
#include <delay.h>
#define SI 1012 //494Hz (2024us) 1012us

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f};
int dec = 0;

void PORT_init(void)
{
    DDRE = 0xf0; //PORTE.4~ PORTE.7 FND 출력
                // PE.4 : FND0, PE.5 : FND1 , PE.6 : FND2, PE.7 : FND3)
    DDRB = 0xff; //세그먼트의 문자포트 출력 
                // PB.0 : a, PB.1 : b, PB.2 : c, PB.3 : d, PB.4 : e, PB.5 : f , PB.6 : g, PB.7 : dot        
                
    DDRG |= (1<<PORTG4);    //부저와 연결되는 PORTG.4 를 출력으로 설정 
    PORTG &= ~(1<<PORTG4);
}

void myDelay_us(unsigned int delay)
{
    int i;
    for(i = 0; i < delay; i++){
        delay_us(1);
    }
}

void SSound(int time) 
{
    int i, tim;
    tim = 5000 / time;
    for(i = 0; i < tim; i++) {
        PORTG |= (1<<PORTG4); //buzzer on, PORTG의 4번 핀 on (out1)
        myDelay_us(time);
        PORTG &= ~(1<<PORTG4); //buzzer off, PORTG의 4번 핀 off(out0)
        myDelay_us(time);
    }
}

interrupt [EXT_INT0] void ext_int0(void)
{
    dec += 1;
    SSound(SI);    
}
interrupt [EXT_INT1] void ext_int1(void)
{
    dec += 4;
    SSound(SI);    
}
interrupt [EXT_INT2] void ext_int2(void)
{
    dec -= 1;
    SSound(SI);    
}
interrupt [EXT_INT3] void ext_int3(void)
{
    dec -= 5;
    SSound(SI);   
}

void Interrupt_init(void)
{
    EICRA &= ~(3<<ISC00); //INT0 Falling Edge 감지 설정
    EICRA |= (1<<ISC01) | (0<<ISC00);
    
    EICRA &= ~(3<<ISC10); //INT1 Falling Edge 감지 설정
    EICRA |= (1<<ISC11) | (0<<ISC10);
    
    EICRA &= ~(3<<ISC20); //INT2 Falling Edge 감지 설정
    EICRA |= (1<<ISC21) | (0<<ISC20);
    
    EICRA &= ~(3<<ISC30); //INT3 Falling Edge 감지 설정
    EICRA |= (1<<ISC31) | (0<<ISC30);
    
    EIMSK |= (1<<INT0) | (1<<INT1) | (1<<INT2) | (1<<INT3); // INT 0,1,2,3 활성화
    SREG |= (1<<SREG_I); //전체 인터럽트 허가
} 

// FND 출력                
void FND_Disp(void)
{   
    PORTE = Port_fnd[3];
    PORTB = Port_char[(dec / 1000%10)]; 
    delay_ms(1);
    PORTB = 0xff;
    
    PORTE = Port_fnd[2];
    PORTB = Port_char[(dec / 100%10)]; 
    delay_ms(1);
    PORTB = 0xff;
    
    PORTE = Port_fnd[1];
    PORTB = Port_char[(dec / 10%10)]; 
    delay_ms(1);
    PORTB = 0xff;
    
    PORTE = Port_fnd[0];
    PORTB = Port_char[(dec / 1%10)]; 
    delay_ms(1);
    PORTB = 0xff; 
    
    if(dec > 9999) 
    {
        dec -= 10000;
    } 
    if(dec < 0) 
    {
        dec += 10000;
    }
}

void main(void)
{
    PORT_init();
    Interrupt_init();
    
    while (1)
    {
        FND_Disp();
    }
}