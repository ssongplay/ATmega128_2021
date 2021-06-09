/*
 * ex7_3.c
 *
 * Created: 2021-05-17 오후 5:01:35
 * Author: syj47
 */

// 1kHz 구형파를 PORTB.0상에 출력하는 프로그램
// - 1kHz의 구형파를 만들기 위해서는 펄스의 On 시간과 off 시간이 각각 500us



#include <mega128.h>

unsigned char c_cnt;

void Init_Timer2(void)
{
    TCCR2 = 0x00;           // 타이머/카운터 동작 금지
    TCCR2 |= (1<<WGM21);    // TCCR2 레지스터 CTC 모드 설정
    OCR2 = 100;             // 출력 비교 레지스터의 주기는 50us
    TIMSK = (1<<OCIE2);     // 출력 비교 인터럽트 허가상태
    TCCR2 |= 1<<CS21;       // 타이머/카운터2 동작
}

//출력비교 인터럽트가 발생했을 때 처리하는 인터럽트 함수
interrupt [TIM2_COMP] void timer2_out_comp(void)
{
    c_cnt++;
}

void main(void)
{
    DDRB.0 = 1;             //PORTB의 0번핀을 출력으로 설정
    c_cnt = 0;              // c_cnt 변수 클리어
    Init_Timer2();          // 타이머2 초기화 
    SREG |= 0x80;           // 전체 인터럽트 허가
    while(1)
    {
        if(c_cnt == 10)     // 50us x 10 = 500us
        {
            PORTB.0 = 
}
