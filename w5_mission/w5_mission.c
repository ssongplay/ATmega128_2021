/*
 * w5_mission.c
 *
 * Created: 2021-04-05 오후 4:55:51
 * Author: syj47
 */

// PORTD.4에 연결된 DIP스위치가 ON되면, 0.5초마다 FND 1 증가
// PORTD.5에 연결된 DIP스위치가 ON되면, 0.5초마다 FND 10 증가
// PORTD.6에 연결된 DIP스위치가 ON되면, 0.5초마다 FND 1 감소 
// PORTD.7에 연결된 DIP스위치가 ON되면, 0.5초마다 FND 10 감소
// 이때 숫자의 표시 범위는 0 ~ 9999 (음수값이 될 경우 9999로 변경, -2 -> 9998)


#include <mega128.h>
#include <delay.h>

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90}; //애노드 공통
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f}; // FND0 ON, FND1 ON, FND2 ON, FND3 ON


void PORT_Init(void)
{
    DDRE = 0xf0; //PORTE.4~ PORTE.7 FND 출력
                // PE.4 : FND0, PE.5 : FND1 , PE.6 : FND2, PE.7 : FND3)   
    DDRB = 0xff; //세그먼트의 문자포트 출력 
                // PB.0 : a, PB.1 : b, PB.2 : c, PB.3 : d, PB.4 : e, PB.5 : f , PB.6 : g, PB.7 : dot   
}

void segment(unsigned int number)
{
    unsigned int temp;
    unsigned int D1000, D100, D10, D1;
    
    D1000 = number/1000; //천의 자리
    temp = number % 1000; // 1000으로 나눈 나머지 값 저장
    D100 = temp / 100; // 백의자리
    temp = temp % 100;  // 100으로 나눈 나머지 값 저장
    D10 = temp / 10;  // 십의자리
    D1 = temp % 10;   // 일의자리  
    

    PORTE = Port_fnd[3]; // 네 번째 FND를 ON 시킨다.
    PORTB = Port_char[D1000]; // 천의 자리수를 출력한다.  
    delay_ms(1);   
    PORTB = 0xff;
   
    PORTE = Port_fnd[2]; // 세 번째 FND를 ON 시킨다.
    PORTB = Port_char[D100]; // 백의 자리수를 출력한다. 
    delay_ms(1);
    PORTB = 0xff;
    
    
    PORTE = Port_fnd[1]; // 두 번째 FND를 ON 시킨다.
    PORTB = Port_char[D10]; // 십의 자리수를 출력한다.   
    delay_ms(1); 
    PORTB = 0xff;
    
    
    PORTE = Port_fnd[0]; // 첫 번째 FND를 ON 시킨다.
    PORTB = Port_char[D1]; // 일의 자리수를 출력한다.
    delay_ms(1);
    PORTB = 0xff;  

}

void main(void)
{
    int i;
    int count=0;               //스위치 동작에 따른 계수를 위한 변수 
    int count_past = count;   //스위치 동작에 따르는 변수의 과거값 
                              //스위치 누름 등에 따른 계수값 변경됨을 감지하기 위함 
    PORT_Init();
    
    while(1)
    {  
     
        segment(count);
        if(      !(PIND & (1<<PORTD4))){ count+=1;  }
        else if( !(PIND & (1<<PORTD5))){ count+=10; }
        else if( !(PIND & (1<<PORTD6))){ count-=1;  } 
        else if( !(PIND & (1<<PORTD7))){ count-=10; }
        
        if(count<0) count+=10000; 
        if(count>9999) count-=10000;
        
        if(count_past != count)
        {   
            // 과거의 i와 지금의 i가 다르면 눌러진것 -> 500ms 지연 
            for(i=0; i<125; i++)
            {
                segment(count);
            }
        }
        count_past = count;   
        
         
    }
}

//--------------------교수님꺼--------------------------------------


#include <mega128.h>
#include <delay.h>

// FND 문자표 및 FND 포트 설정 
unsigned char Port_char[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90
                                , 0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e }; //애노드 공통
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f}; // FND0 ON, FND1 ON, FND2 ON, FND3 ON


void PORT_Init(void)
{
    DDRE = 0xf0; //PORTE.4~ PORTE.7 FND 출력
                // PE.4 : FND0, PE.5 : FND1 , PE.6 : FND2, PE.7 : FND3)   
    DDRB = 0xff; //세그먼트의 문자포트 출력 
                // PB.0 : a, PB.1 : b, PB.2 : c, PB.3 : d, PB.4 : e, PB.5 : f , PB.6 : g, PB.7 : dot   
}


// FND에 출력을 하는 함수
void FND_Disp(int dec)
{
    PORTE = Port_fnd[3];
    PORTB = Port_char[(dec/1000%10)];
    delay_ms(1);
    PORTB = 0xFF;
    
    PORTE = Port_fnd[2];
    PORTB = Port_char[(dec/100%10)];
    delay_ms(1);
    PORTB = 0xFF; 
    
    PORTE = Port_fnd[1];
    PORTB = Port_char[(dec/10%10)];
    delay_ms(1);
    PORTB = 0xFF; 
    
    PORTE = Port_fnd[0];
    PORTB = Port_char[(dec/1%10)];
    delay_ms(1);
    PORTB = 0xFF;
}    


void main(void)
{
    int i;
    int decm=0;               //스위치 동작에 따른 계수를 위한 변수 
    int hdecm = decm;         //스위치 동작에 따르는 변수의 과거값 
                              //스위치 누름 등에 따른 계수값 변경됨을 감지하기 위함 
    PORT_Init();
    
    while(1)
    {  
     
        FND_Disp(decm);
        if(      !(PIND & (1<<PORTD4))){ decm += 1;  }
        else if( !(PIND & (1<<PORTD5))){ decm += 10; }
        else if( !(PIND & (1<<PORTD6))){ decm -= 1;  } 
        else if( !(PIND & (1<<PORTD7))){ decm -= 10; }
        
        if(decm<0) decm += 10000; 
        if(decm>9999) decm -= 10000;
        
        if(hdecm != decm)
        {   
            // 과거의 i와 지금의 i가 다르면 눌러진것 -> 500ms 지연 
            for(i=0; i<125; i++)
            {
                FND_Disp(decm);
            }
        }
        hdecm = decm;
    }
}


// ----------flag를 이용한 스위치 누름 감지-------------
void main (void)
{
    int i;
    int decm = 0;
    int flg = 0;
    PORT_Init();
    
    while(1)
    {
        FND_Disp(decm);
        if(      !(PIND & (1<<PORTD4))){ decm += 1;     flg = 1;  }
        else if( !(PIND & (1<<PORTD5))){ decm += 10;    flg = 1;  }
        else if( !(PIND & (1<<PORTD6))){ decm -= 1;     flg = 1;  } 
        else if( !(PIND & (1<<PORTD7))){ decm -= 10;    flg = 1;  } 
        else {                                          flg = 0;  }  
        
        if(decm<0) decm += 10000; 
        if(decm>9999) decm -= 10000; 
        
        if(flg)
        {
            for(i=0; i<125; i++)
            {
                FND_Disp(decm);
            }
        }
    }
}
               