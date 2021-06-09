/*
 * w5_mission.c
 *
 * Created: 2021-04-05 ���� 4:55:51
 * Author: syj47
 */

// PORTD.4�� ����� DIP����ġ�� ON�Ǹ�, 0.5�ʸ��� FND 1 ����
// PORTD.5�� ����� DIP����ġ�� ON�Ǹ�, 0.5�ʸ��� FND 10 ����
// PORTD.6�� ����� DIP����ġ�� ON�Ǹ�, 0.5�ʸ��� FND 1 ���� 
// PORTD.7�� ����� DIP����ġ�� ON�Ǹ�, 0.5�ʸ��� FND 10 ����
// �̶� ������ ǥ�� ������ 0 ~ 9999 (�������� �� ��� 9999�� ����, -2 -> 9998)


#include <mega128.h>
#include <delay.h>

unsigned char Port_char[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90}; //�ֳ�� ����
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f}; // FND0 ON, FND1 ON, FND2 ON, FND3 ON


void PORT_Init(void)
{
    DDRE = 0xf0; //PORTE.4~ PORTE.7 FND ���
                // PE.4 : FND0, PE.5 : FND1 , PE.6 : FND2, PE.7 : FND3)   
    DDRB = 0xff; //���׸�Ʈ�� ������Ʈ ��� 
                // PB.0 : a, PB.1 : b, PB.2 : c, PB.3 : d, PB.4 : e, PB.5 : f , PB.6 : g, PB.7 : dot   
}

void segment(unsigned int number)
{
    unsigned int temp;
    unsigned int D1000, D100, D10, D1;
    
    D1000 = number/1000; //õ�� �ڸ�
    temp = number % 1000; // 1000���� ���� ������ �� ����
    D100 = temp / 100; // �����ڸ�
    temp = temp % 100;  // 100���� ���� ������ �� ����
    D10 = temp / 10;  // �����ڸ�
    D1 = temp % 10;   // �����ڸ�  
    

    PORTE = Port_fnd[3]; // �� ��° FND�� ON ��Ų��.
    PORTB = Port_char[D1000]; // õ�� �ڸ����� ����Ѵ�.  
    delay_ms(1);   
    PORTB = 0xff;
   
    PORTE = Port_fnd[2]; // �� ��° FND�� ON ��Ų��.
    PORTB = Port_char[D100]; // ���� �ڸ����� ����Ѵ�. 
    delay_ms(1);
    PORTB = 0xff;
    
    
    PORTE = Port_fnd[1]; // �� ��° FND�� ON ��Ų��.
    PORTB = Port_char[D10]; // ���� �ڸ����� ����Ѵ�.   
    delay_ms(1); 
    PORTB = 0xff;
    
    
    PORTE = Port_fnd[0]; // ù ��° FND�� ON ��Ų��.
    PORTB = Port_char[D1]; // ���� �ڸ����� ����Ѵ�.
    delay_ms(1);
    PORTB = 0xff;  

}

void main(void)
{
    int i;
    int count=0;               //����ġ ���ۿ� ���� ����� ���� ���� 
    int count_past = count;   //����ġ ���ۿ� ������ ������ ���Ű� 
                              //����ġ ���� � ���� ����� ������� �����ϱ� ���� 
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
            // ������ i�� ������ i�� �ٸ��� �������� -> 500ms ���� 
            for(i=0; i<125; i++)
            {
                segment(count);
            }
        }
        count_past = count;   
        
         
    }
}

//--------------------�����Բ�--------------------------------------


#include <mega128.h>
#include <delay.h>

// FND ����ǥ �� FND ��Ʈ ���� 
unsigned char Port_char[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90
                                , 0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e }; //�ֳ�� ����
unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f}; // FND0 ON, FND1 ON, FND2 ON, FND3 ON


void PORT_Init(void)
{
    DDRE = 0xf0; //PORTE.4~ PORTE.7 FND ���
                // PE.4 : FND0, PE.5 : FND1 , PE.6 : FND2, PE.7 : FND3)   
    DDRB = 0xff; //���׸�Ʈ�� ������Ʈ ��� 
                // PB.0 : a, PB.1 : b, PB.2 : c, PB.3 : d, PB.4 : e, PB.5 : f , PB.6 : g, PB.7 : dot   
}


// FND�� ����� �ϴ� �Լ�
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
    int decm=0;               //����ġ ���ۿ� ���� ����� ���� ���� 
    int hdecm = decm;         //����ġ ���ۿ� ������ ������ ���Ű� 
                              //����ġ ���� � ���� ����� ������� �����ϱ� ���� 
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
            // ������ i�� ������ i�� �ٸ��� �������� -> 500ms ���� 
            for(i=0; i<125; i++)
            {
                FND_Disp(decm);
            }
        }
        hdecm = decm;
    }
}


// ----------flag�� �̿��� ����ġ ���� ����-------------
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
               