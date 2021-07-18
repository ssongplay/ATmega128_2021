#include <mega128.h>
#include <delay.h> 

#define LCD_WDATA PORTA     // LCD ������ ��Ʈ ����
#define LCD_WINST PORTA
#define LCD_CTRL  PORTG     // LCD ������Ʈ ����
#define LCD_EN    0
#define LCD_RW    1
#define LCD_RS    2

#define FND_Null    0
#define FND_Star    14
#define FND_Sharp   15

// ���� define 
#define DO 1908 
#define RE 1700 
#define MI 1515 
#define FA 1432 
#define SOL 1275 
#define LA 1136 
#define SI 1012
#define DDO 956 

// SERVO PIN ���� 
#define SERVO_PIN_LOW()     PORTD &= ~(1<<PORTD7)
#define SERVO_PIN_HIGH()    PORTD |= (1<<PORTD7)

int i = 0;
int j = 0;
int k =0 ;
int security = 0;
int sharp_flag = 0;
int right = 0;
int wrong = 0;
unsigned char input_pw[16]; 
unsigned char pw[16] = "2019146018";   
unsigned char Key_off_flag = 0;
unsigned char New_key_data = 0;    
unsigned int Port_char[] =  {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,
                                0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e}; //�ֳ�� ���� 
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f, 0x0f};
unsigned char key_num = 0;
unsigned int buf_seg[4] = {FND_Null, FND_Null, FND_Null, FND_Null};

unsigned int T2_DUTY_TIME_us;
unsigned int T2_CYCLE_TIME_us;
unsigned int T2_DUTY_TIME_cnt_us;
unsigned int T2_CYCLE_TIME_cnt_us;


void PortInit (void)
{
    // LCD 
    DDRA = 0xFF;   
    DDRG = 0x0F;  
    // FND
    DDRE = 0xf0;    // FND Sel
    DDRB = 0xff;    // FND Data Line
    // KEYPAD 
    DDRC = 0xf0;    //���� 4��Ʈ Row
    PORTC = 0xff;   // ��Ʈ �ʱ�ȭ
    PORTE = Port_fnd[4];    //ALL FND OFF 
    // BUTTON
    DDRD = 0x00; 
    // MOTOR 
    DDRD |= (1<<PORTD7); 
    // BUZZER 
    DDRG |= (1<<PORTG4); 
    PORTG |= (1<<PORTG4);
}

//------------------------------------LCD-----------------------------------
void LCD_Data(unsigned char ch)     // LCD_DR�� ������ ���
{
    LCD_CTRL |= (1 << LCD_RS);      // RS=1, =0���� ������ ���� ����Ŭ
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);      // LCD Enavle
    delay_us(50);
    LCD_WDATA = ch;                 // ������ ���
    delay_us(50);
    LCD_CTRL &= ~(1 << LCD_EN);     // LCD Disable
}

void LCD_Comm(unsigned char ch)     // LCD IR�� ��ɾ� ����
{
    LCD_CTRL &= ~(1 << LCD_RS);     // RS==0���� ��ɾ� ���� ����Ŭ
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);      // LCD Enable
    delay_us(50);
    LCD_WINST = ch;                 // ��ɾ� ���� 
    delay_us(50);        
    LCD_CTRL &= ~(1 << LCD_EN);     // LCD Disable
}

void LCD_CHAR(unsigned char c)      // �� ���� ���
{
    LCD_Data(c);
    delay_ms(1);
}

void LCD_STR(unsigned char *str)    // ���ڿ� ���
{
    while(*str != 0)
    {
        LCD_CHAR(*str);
        str++;
    }
}

void LCD_pos(unsigned char row, unsigned char col)  // LCD ������ ����
{
    LCD_Comm(0x80 | ((row*0x40) + col) );       
}

void LCD_Init(void)     // LCD �ʱ�ȭ 
{
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(4100);     // 4.1ms ����  
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(100);      // 100us ����
    LCD_Comm(0x30);     // �ʱ�ȭ Set,
    delay_us(100);      // 100us ����
    LCD_Comm(0x38);     // �ʱ�ȭ Set, �����ͱ��� 8bit, ǥ�ö��� 2�� ����� ���� ��� ���� 
    delay_us(1000);     // ����� ó���ϴµ� �ּ� 40us ������ �߻��ϱ⿡ ������ ����Ͽ� ���� 
    LCD_Comm(0x0c);     // Display ON, Cursor OFF, Blink OFF
    delay_us(1000);     // 40us �̻��� ��ٸ� 
    LCD_Comm(0x01);     // LCD Clear
    delay_us(2000);     // 1.64ms �̻��� ��ٸ�
    LCD_Comm(0x06);     // Cursor Entry Mode Set, ǥ����ġ +1�� ����
    delay_us(1000);     // 40us �̻��� ��ٸ� 
}

//------------------------------------SEGMENT-----------------------------------
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

//------------------------------------KEYPAD---------------------------------
unsigned char KeyScan(void) //4x4 Ű�е� ��ĵ �Լ�, ��°��� 10���� 1~16
{
    // ���� �Ϻ�(4��Ʈ)�� ����Ī(���������� ���ư��鼭)�ϸ鼭 ���
    unsigned int key_scan_line = 0xEF;
    
    // Ű ��ĵ ���� ������ ���� �ݺ��� ����, Ű ��Ʈ���� ���� �Է� ��   
    unsigned char key_scan_loop = 0, getPinData=0;      

    // Ű ��ĵ �ݺ���
    for(key_scan_loop=0; key_scan_loop<4; key_scan_loop++)
    {   
        // Ű ��Ʈ������ ��ĵ ���� ������ ���� PORTC ��°� 
        PORTC = key_scan_line;
        delay_us(1);
        // Ű ��Ʈ������ �� �� ��� 
        // C ��Ʈ�� ���� �Ϻ�(4��Ʈ)RKQT, 74LS14 ������� ���� ��ȣ ���� 
        getPinData = PINC & 0x0f;
        if(getPinData != 0x00)
        {
            switch(getPinData)
            {
                case 0x01: key_num = key_scan_loop*4 + 1; break;  //1110�� ���� �Է��� �´� ��� 
                case 0x02: key_num = key_scan_loop*4 + 2; break; 
                case 0x04: key_num = key_scan_loop*4 + 3; break;
                case 0x08: key_num = key_scan_loop*4 + 4; break;
            }
            return key_num;
        }
        key_scan_line = (key_scan_line << 1);       // Init_data(0xEF) �� ����Ʈ�� 
        delay_us(1);
    }
}

unsigned char Key_data_trans(unsigned char New_key_data)   // 4x3 Ű�е�� �Է��ϱ� ���� �Լ� 
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

//------------------------------------BUZZER-----------------------------------
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
        PORTG |= (1<<PORTG4); //buzzer on, PORTG�� 4�� �� on (out1)
        myDelay_us(time);
        PORTG &= ~(1<<PORTG4); //buzzer off, PORTG�� 4�� �� off(out0)
        myDelay_us(time);
    }
    PORTG |= (1<<PORTG4);    //buzzer off, PORTG�� 4�� �� off (out 0)
    
}

//-----------------------------------MOTOR------------------------------------
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
    // Ÿ�̸�/ī���� 2 - 2�� ���� �� PC PWM ��Ƽ�� ���� 
    TIMSK |= (1<<OCIE2);                // ��º� ���ͷ�Ʈ2 �㰡
    TCCR2 = (1<<WGM21) | (2<<CS20);     // CTC���, 1024����
    OCR2 = 184;                         // �� 100us
    // OCR2 = (14745600Hz / 8���ֺ�) * 100us = 184.32
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

//------------------------------------INTERRUPT-----------------------------------
interrupt [EXT_INT0] void ext_int0_isr(void)
{   // PORTD 0�� ����ġ ������ �� �߻��ϴ� �ܺ� ���ͷ�Ʈ (��й�ȣ �缳��)
    
    if(k%2 == 0)
    {
        LCD_pos(0,1);             
        LCD_STR("PASSWORD SET   ");
        LCD_pos(1,1);
        LCD_STR("---------------");
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
                    if((key_num>=0) && (key_num<=9))
                    {
                        input_pw[i] = key_num + '0';
                        LCD_CHAR(input_pw[i]);
                        i++;
                        if(i>=14)
                        {
                            i = 0;
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
        }
    }
    
    if(k%2 == 1)        // D0���� �ι�° ������ �� 
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
        pw[i] = input_pw[i];       
    }
    
    k++;
}


void main(void)
{
    PortInit();                 // ��Ʈ ���� 
    LCD_Init();                 // LCD ��Ʈ ���� 
    Init_TimerINT();            // Ÿ�̸� �ʱ�ȭ
    T2_DUTY_TIME_us = 1500;     // PWM ��Ƽ �� ���� 
    T2_CYCLE_TIME_us = 20000;   // PWM �ֱ� ����   
    
    //INT0 Falling Edge ���� ���� (ISC01 : 1, ISC00 : 0)   //���� �� ���� 
    EICRA &= ~(3<<ISC00);                   // 1111 1110
    EICRA |= (1<<ISC01) | (0<<ISC00);       // 0000 0010
    EIMSK |= (1<<INT0);      //INT0 Ȱ��ȭ
    SREG |= (1<<SREG_I);    //��ü ���ͷ�Ʈ �㰡  
    
    while(1)
    {
        New_key_data = KeyScan();
        Print_Segment(buf_seg);
        
        if(New_key_data)        // ���ο� ���� keypad�� ������
        {
            SSound(SI);
            delay_ms(100);
            key_num = Key_data_trans(New_key_data);
            
            if(Key_off_flag)
            {
                if(sharp_flag == 1 && (key_num>=0) && (key_num<=9))
                {   
                    // 0 ~ 9 ��ȣ�� ������ �� 
                    input_pw[i] = key_num + '0';
                    LCD_CHAR(input_pw[i]);
                    i++;
                    
                    if(i>=14)    // ��й�ȣ�� �ִ� 14�ڸ� 
                    {
                        i = 0;
                        LCD_pos(1,1);
                    }
                }
                else if (key_num>9)
                {
                    if(key_num == 15)       // # ������ �� 
                    {
                        for(j=0;j<16;j++)
                        {
                            input_pw[j] = 0;    // �Է��ߴ� input_pw �ʱ�ȭ 
                        }
                        
                        // �ʱ� ȭ�� ��� 
                        LCD_pos(0,1);
                        LCD_STR("PASSWORD       ");     
                        LCD_pos(1,1);
                        LCD_STR("**************"); 
                        LCD_pos(1,1);
                    }
                    
                    else if(key_num == 14)  // * ������ �� 
                    {   
                        // ��й�ȣ ��ġ Ȯ��
                        for(i=0;i<16;i++)
                        {   
                            if(pw[i] == input_pw[i])
                            {      
                                // ��й�ȣ ��ġ �� right flag 1 
                                right = 1; 
                            }
                            else
                            {   // ��й�ȣ ����ġ��
                                wrong = 1;
                                right = 0;
                                break;
                            }
                        }
                    }
                }
                Key_off_flag = ~Key_off_flag;
            }
            else
            {
                buf_seg[0] = key_num;
            }
        }
        else 
        {
            Key_off_flag = 0xff;
        }
        
        
        if(right == 1)
        {   
            // �� - 1) ��й�ȣ ��ġ �� 
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
            // �� - 3) �ʱ���� ���� 
            LCD_pos(0,1);
            LCD_STR("               ");            
            LCD_pos(1,1);
            LCD_STR("               "); 
 
            right = 0;  
        }
        
        if(wrong == 1) 
        {   
            // �� - 2) ��й�ȣ ����ġ ��
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
            // �� - 3) �ʱ���� ����  
            LCD_pos(0,1);
            LCD_STR("               ");             
            LCD_pos(1,1);
            LCD_STR("               "); 
            security++;  // ���ȴ�å 2 - �� �� ���� security 1 ���� 
            wrong = 0;
            
        } 
        
        if(security>=3)
        {   
            // ���ȴ�å 2 - �� ) 3ȸ �̻� Ʋ�� �� �����
            LCD_pos(0,1);
            LCD_STR("warning...        "); 
            LCD_pos(1,1); 
            LCD_STR(" if.theft         ");  
            SSound(DDO);    
            delay_ms(50);
            SSound(SOL);
            delay_ms(50);
            
            while(key_num==10)  // 2 - ��) ������Ű��� 
            {   
                // Ű�е��� �� ��ư ���� �� ������Ű��� ���� 
                delay_ms(100);
                LCD_pos(0,1);
                LCD_STR("MASTER MODE        "); 
                LCD_pos(1,1); 
                LCD_STR("**************"); 
            }       
        }
        
        if(!(PIND & (1<<PORTD1)))
        {   
            // 1 - �� ) PIND1�� ����ġ�� ���� ��� 
            delay_ms(3000);
            SSound(SOL);
            delay_ms(100);
            SSound(FA);
            delay_ms(100);
            SSound(MI);
            delay_ms(100);  
            SSound(RE);
            SetServoDeg(0); 
            delay_ms(3000);
            //�ʱ���� ���� 
            LCD_pos(0,1);
            LCD_STR("               ");           
            LCD_pos(1,1);
            LCD_STR("               ");          
        }
    
    }
}
            

