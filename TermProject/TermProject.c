#include <mega128.h>
#include <delay.h> 

//============lcd define========================================
#define LCD_WDATA PORTA     // LCD 데이터 포트 정의
#define LCD_WINST PORTA
#define LCD_CTRL  PORTG     // LCD 제어포트 정의
#define LCD_EN    0
#define LCD_RW    1
#define LCD_RS    2

//============fnd 0 , *, #==================================
#define FND_Null    0
#define FND_Star    14
#define FND_Sharp   15



//==========음계===========================================
#define DO 1908 //262Hz (3817us) 1908us
#define RE 1700 //294Hz (3401us) 1701us
#define MI 1515 //330Hz (3030us) 1515us
#define FA 1432 //349Hz (2865us) 1433us
#define SOL 1275 //370Hz (2703us) 1351us
#define LA 1136 //440Hz (2273us) 1136us
#define SI 1012 //494Hz (2024us) 1012us
#define DDO 956 //높은 도 

unsigned int sharp_flag;

//int  flag1=0;                                            
//int j = 0;
int i = 0;
//int wrong=0;
//int right=0;
                     
    
unsigned char input_pw[16];
    
unsigned char Key_off_flag = 0;
unsigned char New_key_data = 0;
    
unsigned char pw[16] = "20";
//unsigned char master_key[16] = "21";

//====================키패드 ====================================


unsigned int Port_char[] =  {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                                0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //애노드 공통 
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f, 0x0f};
unsigned char key_num = 0;
unsigned int buf_seg[4] = {FND_Null, FND_Null, FND_Null, FND_Null};

//===============모터===========
unsigned int T2_DUTY_TIME_us;
unsigned int T2_CYCLE_TIME_us;

unsigned int T2_DUTY_TIME_cnt_us;
unsigned int T2_CYCLE_TIME_cnt_us;

#define SERVO_PIN_LOW()     PORTF &= ~(1<<PORTF3)
#define SERVO_PIN_HIGH()    PORTF |= (1<<PORTF3)



int k =0 ;
int security = 0;


//=================================================================
void PortInit (void)
{
    DDRA = 0xFF;    // PORTA(LCD data pin)를 출력으로
    DDRG = 0x0F;    // PORTC의 하위 4비트를 출력으로 
    
    DDRE = 0xf0;    // FND Sel
    DDRB = 0xff;    // FND Data Line
    
    DDRC = 0xf0;    //상위 4비트 Row
    PORTC = 0xff;   // 포트 초기화
    PORTE = Port_fnd[4];    //ALL FND OFF 
    
    DDRD = 0x00; 
    
    DDRF |= (1<<PORTF3);    // 모터
    
    DDRG |= (1<<PORTG4);    //부저와 연결되는 PORTG.4 를 출력으로 설정 
    PORTG &= ~(1<<PORTG4);

}

//============================================================================
//====================lcd 함수================================================
//============================================================================

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
    LCD_Comm(0x0c);     // Display ON, Cursor OFF, Blink OFF
    delay_us(1000);     // 40us 이상을 기다림 
    LCD_Comm(0x01);     // LCD Clear
    delay_us(2000);     // 1.64ms 이상을 기다림
    LCD_Comm(0x06);     // Cursor Entry Mode Set, 표시위치 +1씩 증가
    delay_us(1000);     // 40us 이상을 기다림 
}

//==================================================================================================


void Print_Segment(unsigned int* seg_value)
{
    PORTE = Port_fnd[0];
    PORTB = Port_char[seg_value[0]];
    delay_ms(1);
    
    PORTE = Port_fnd[1];
    PORTB = Port_char[seg_value[1]];
    delay_ms(1);   
    
    PORTE = Port_fnd[2];
    PORTB = Port_char[seg_value[2]];
    delay_ms(1);
    
    PORTE = Port_fnd[3];
    PORTB = Port_char[seg_value[3]];
    delay_ms(1);
}

unsigned char KeyScan(void) //4x4 키패드 스캔 함수, 출력값은 10진수 1~16
{
    // 상위 니블(4비트)을 스위칭(연속적으로 돌아가면서)하면서 출력
    unsigned int key_scan_line = 0xEF;
    
    // 키 스캔 라인 변경을 위한 반복문 인자, 키 매트릭스 열의 입력 값   
    unsigned char key_scan_loop = 0, getPinData=0;      
    
    // 키 스캔 반복문
    for(key_scan_loop=0; key_scan_loop<4; key_scan_loop++)
    {
        PORTC = key_scan_line;
        delay_us(1);
        
        getPinData = PINC & 0x0f;
        if(getPinData != 0x00)
        {
            switch(getPinData)
            {
                case 0x01: key_num = key_scan_loop*4 + 1; break;
                case 0x02: key_num = key_scan_loop*4 + 2; break; 
                case 0x04: key_num = key_scan_loop*4 + 3; break;
                case 0x08: key_num = key_scan_loop*4 + 4; break;
            }
            return key_num;
        }
        key_scan_line = (key_scan_line << 1);
        delay_us(1);
    }
}


unsigned char Key_data_trans(unsigned char New_key_data)
{
    unsigned char key_num = 0;
    if(New_key_data%4 != 0)
    {
        key_num = (New_key_data/4)*3 + (New_key_data%4);   
        switch(key_num)
        {
            case 10: key_num = FND_Star; break;
            case 11: key_num = FND_Null; break;
            case 12: key_num = FND_Sharp;
                     sharp_flag = 1;
                     break;
        }
    }
    else
        key_num = (New_key_data/4)+9;
    return key_num;
}
//
//
//void Key_pad_in(unsigned char* New_key_data, unsigned char* Key_off_flag)
//{
//    if(*New_key_data)        
//    {
//        if(*Key_off_flag)
//        {
//            buf_seg[3] = buf_seg[2];
//            buf_seg[2] = buf_seg[1];
//            buf_seg[1] = buf_seg[0];
//            *Key_off_flag = ~*Key_off_flag;
//        }
//        else
//        buf_seg[0] = Key_data_trans(*New_key_data);
//    }
//    else
//        *Key_off_flag=0xff;
//}




//--------------부저----------------

void myDelay_us(unsigned int delay)
{
    int i;
    for(i=0; i<delay; i++)
    {
        delay_us(1);
    }
}

void SSound(int time)
{
    int i, tim;
    tim = 50000 / time;
    for(i=0; i<tim; i++)
    {
        PORTG |= (1<<PORTG4); //buzzer on, PORTG의 4번 핀 on (out1)
        myDelay_us(time);
        PORTG &= ~(1<<PORTG4); //buzzer off, PORTG의 4번 핀 off(out0)
        myDelay_us(time);
    }
    PORTG &= ~(1<<PORTG4);    //buzzer off, PORTG의 4번 핀 off (out 0)
    
}  

//------------------------------------------모터 -----------------------------------------------------  

interrupt [TIM2_COMP] void timer2_cmp(void)
{
    T2_DUTY_TIME_cnt_us += 100;
    T2_CYCLE_TIME_cnt_us += 100;
    
    if(T2_DUTY_TIME_cnt_us <= T2_DUTY_TIME_us)
    {SERVO_PIN_HIGH();}      
    else
    {SERVO_PIN_LOW();}       
    
    if(T2_CYCLE_TIME_cnt_us == T2_CYCLE_TIME_us)
    {
        T2_CYCLE_TIME_cnt_us = 0;
        T2_DUTY_TIME_cnt_us = 0;
    }
}

void Init_Timer2(void)
{
    // 타이머/카운터 2 - 2초 설정 및 PC PWM 듀티비 설정 
    TIMSK |= (1<<OCIE2);                // 출력비교 인터럽트2 허가
    TCCR2 = (1<<WGM21) | (2<<CS20);     // CTC모드, 1024분주
    OCR2 = 184;                         // 약 100us
    // OCR2 = (14745600Hz / 8분주비) * 100us = 184.32
}

void Init_TimerINT(void)
{
    Init_Timer2();
    SREG |= 0x80;
}

void SetServoDeg(unsigned int deg)
{
    T2_DUTY_TIME_us = 500 + (deg*200/18);
}

//----------인터럽트 ---------------------------------------

interrupt [EXT_INT0] void ext_int0_isr(void)
{ 
    

   if(k%2 ==0)
   {
        LCD_pos(0,1);             
        LCD_STR("PASSWORD SET  ");  
        LCD_pos(1,1);  
        LCD_STR("--------------"); 
        LCD_pos(1,1);
    
        while(1)
        {
    
      
            New_key_data = KeyScan();      
            Print_Segment(buf_seg);
            
      
           if(New_key_data)
           {   
             SSound(SI); 
             delay_ms(100);
             key_num = Key_data_trans(New_key_data);            
                if(Key_off_flag) 
                { 
//                    buf_seg[3] = buf_seg[2];
//                    buf_seg[2] = buf_seg[1];
//                    buf_seg[1] = buf_seg[0];
//                    
                        if((key_num>=0) && (key_num<=9))
                        {  
                            input_pw[i] = key_num + '0';  
                         
                          
                            LCD_CHAR(input_pw[i]);
                            i++;
                             
                            if(i>=14)
                            {
                                i=0; 
                                
                                LCD_pos(1,1);      
                                
                            }  
                            
          
                        
                        }
                        
                        
                        
                        
                      Key_off_flag = ~Key_off_flag;   
                }  
                else 
                {
                    buf_seg[0] = key_num;
                }
                
                     
           } 
          else if (key_num == 14)
          {
                
                break;
          }
           else 
           {
                Key_off_flag = 0xff;   
           }
           
        //if(!(PIND & (1<<PORTD0))){break;} 
        }
        
   
   }
   


    if(k%2 ==1)
    {   
        
        
        SSound(DO);
        delay_ms(100);
        SSound(RE);
        delay_ms(100);
        SSound(MI);
        delay_ms(100);
        
        LCD_pos(0,1);             
        LCD_STR("PASSWORD SET  ");  
        LCD_pos(1,1);  
        LCD_STR(" -SUCCESS-      "); 
        LCD_pos(1,1); 
    
    } 
    
   for(i=0;i<16;i++)
   {
        pw[i]=input_pw[i];  
   } 
    
   k++; 
        
}
        


void main(void)
{

    //int  flag1;                                            
    int j = 0;
    int i = 0;
    int wrong=0;
    int right=0;
                     
    
    unsigned char input_pw[16];
    
    unsigned char Key_off_flag = 0;
    unsigned char New_key_data = 0;
    
    unsigned char pw[16] = "20"; 
    
    PortInit();     // LCD 출력 포트 설정 
                     // 필요시 주석 해제 
    LCD_Init();     // LCD 초기화         
    
                     
    
    
    
    Init_TimerINT();    // 타이머 초기화  
    
    T2_DUTY_TIME_us = 1500;     // PWM 듀티 폭 설정
    T2_CYCLE_TIME_us = 20000;   // PWM 주기 설정
                                                      
    
    
    
    
    
    //INT0 Falling Edge 감지 설정 (ISC01 : 1, ISC00 : 0)   //누를 때 동작 
    EICRA &= ~(3<<ISC00);                   // 1111 1110
    EICRA |= (1<<ISC01) | (0<<ISC00);       // 0000 0010
    
    EIMSK |= (1<<INT0);      //INT0 활성화
    SREG |= (1<<SREG_I);    //전체 인터럽트 허가
    
    
    
 
    while(1)
    {
    
      
    New_key_data = KeyScan();      
    Print_Segment(buf_seg);
            
      
           if(New_key_data)
            {   
             SSound(SI);
             delay_ms(100);
             key_num = Key_data_trans(New_key_data);            
                if(Key_off_flag) 
                { 
//                    buf_seg[3] = buf_seg[2];
//                    buf_seg[2] = buf_seg[1];
//                    buf_seg[1] = buf_seg[0];
                    
                        if(sharp_flag==1  && (key_num>=0) && (key_num<=9))
                        {  
                         input_pw[i] = key_num + '0';  
                         
//                         if(flag1)
//                         {
                          LCD_CHAR(input_pw[i]);
//                         }
                         
                         i++;   
                         
                         if(i>=14)
                         {
                            i=0;
                            LCD_pos(1,1);
                         }  
                        
                        }
                        else if(key_num>9) 
                        {
                            //==========#===========
                            if(key_num ==15)
                            {    
                                for(j=0; j<16; j++)
                                {
                                  input_pw[j]=0;
                                }
                            
                                LCD_pos(0,1);             
                                LCD_STR("PASSWORD        ");  
                                LCD_pos(1,1);  
                                LCD_STR("**************"); 
                                LCD_pos(1,1); 
//                                
//                                flag1=1;
                            }
                            
                            else if(key_num == 14)
                            {
                                for(i=0; i<16; i++)
                                {   
                                    if(pw[i] == input_pw[i])
                                    {
                                        right = 1;
                                    }
                                    else
                                    {
                                        wrong = 1; 
                                        right = 0; 
                                        break;
                                    }
                                     
                                }
                            }
                            
                        } 
                      Key_off_flag = ~Key_off_flag;   
                }  
                else buf_seg[0] = key_num;
                     
            }
            else Key_off_flag = 0xff;   
              
              
              
              
              
        if(right==1)
        {
            LCD_pos(0,1);             
            LCD_STR("DOOR OPEN       ");
            LCD_pos(1,1);
            LCD_STR("               ");  
            SSound(DO);
            delay_ms(100);
            SSound(MI);
            delay_ms(100);
            SSound(SOL);
            delay_ms(100);  
            SSound(DDO);
            SetServoDeg(180); 
            
            
            delay_ms(3000);
            
            //초기상태 복귀 
            LCD_pos(0,1);
            LCD_STR("               "); 
                        
            LCD_pos(1,1);
            LCD_STR("               "); 
            
            
            
            right = 0;
              
        }      
        
        
        if(wrong==1)
        {
            LCD_pos(0,1);
            LCD_STR("PASSWORD        "); 
            LCD_pos(1,1); 
            LCD_STR(" error.....        "); 
            SSound(LA);
            delay_ms(100);
            SSound(LA);
            delay_ms(100);  
            SSound(LA);  
            
            delay_ms(3000);
            
            //초기상태 복귀 
            LCD_pos(0,1);
            LCD_STR("               "); 
                        
            LCD_pos(1,1);
            LCD_STR("               "); 
            security++;
            wrong = 0;
              
        }    
        
        
        if(security>=3)
        {   
            LCD_pos(0,1);
            LCD_STR("warning...        "); 
            LCD_pos(1,1); 
            LCD_STR(" if.theft         ");  
            SSound(DDO);    
            delay_ms(50);
            SSound(SOL);
            delay_ms(50);
            
            
            while(key_num==10)  // 마스터키모드 
            {   
                delay_ms(100);
                LCD_pos(0,1);
                LCD_STR("MASTER MODE        "); 
                LCD_pos(1,1); 
                LCD_STR("**************"); 
            } 
                
        }
        
        if(!(PIND & (1<<PORTD1)))
        {   
            delay_ms(3000);
            
            SSound(SOL);
            delay_ms(100);
            SSound(FA);
            delay_ms(100);
            SSound(MI);
            delay_ms(100);  
            SSound(RE);
            SetServoDeg(0); 
            
            //초기상태 복귀 
            LCD_pos(0,1);
            LCD_STR("               "); 
                        
            LCD_pos(1,1);
            LCD_STR("               ");
                   
        }
        
        
        
            

                                                                                     
    }    
   
}


