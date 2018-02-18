/*
C Program sensor Ultrasonic HCSR-04 dengan AVR Microcontroller
- LCD 8 BIT
- ATMEGA32
- UKK 2018 SMK PGRI 1 BANYUWANGI
- LCD DATA pada Port C
- Kontrol LCD pada Port D ( RS ==>PD6, RW==>GND, EN==>PD5 )
- Sensor Ultrasonic Triger Pin =PD0 , ECHO Pin = PD2 (INT0)
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define enable  5
#define register_num 6

void kirim_perintah_LCD(unsigned char command);
void kirim_karakter_LCD(unsigned char character);
void kirim_string_LCD(char *string_of_characters);

static volatile int pulse = 0;
static volatile int i = 0;

int main(void)
{
    //DDRA = 0xFF;
    DDRC = 0xFF;//LCD DATA
    DDRD = 0b11111011;//LCD CONTROL DAN SENSOR TRIGER pada bit LSB
    _delay_ms(50);
    
    GICR|=(1<<INT0);//enabling interrupt0
    MCUCR|=(1<<ISC00);//setting interrupt triggering logic change
    
    TCCR1A = 0;
    
    int16_t HITUNG = 0;//storing digital output
    char TAMPIL [16];
    

    kirim_perintah_LCD(0x01); //Clear Screen 0x01 = 00000001
    _delay_ms(50);
    kirim_perintah_LCD(0x38);// setting  16X2 LCD mode 8bit 
    _delay_ms(50);
    kirim_perintah_LCD(0b00001111);
    _delay_ms(50);
    
    sei();
    
    while(1)
    {
        PORTD|=(1<<PINC0);
        _delay_us(15);//trigger sensor untuk 15 uS ( mikro detik )
        PORTD &=~(1<<PINC0);
        
        /*perhitungan rumus Jarak , kecepatan, dan waktu
		V= 340 m/s
		V= 0,034 cm/uS
		Time = distance / speed
		t=s/v
		t= 10/ 0,034 = 294 uS
		distance = t x 0,034 / 2
		
		*/
		HITUNG = pulse * 0.024 /20;		
		
        
			if (HITUNG < 0){
				kirim_string_LCD ("Error           ");//jumlah karakter 16
				kirim_perintah_LCD(0x80 + 0x40 + 0);
			}
			else {
				kirim_string_LCD ("UKK ELIND 2018");
				kirim_perintah_LCD(0x80 + 0x40 + 0);
				kirim_string_LCD ("JARAK=");
				itoa(HITUNG,TAMPIL,10);
				/*ITOA mengubah nilai integer menjadi string,
				dan konversi menjadi desimal 
				*/
				kirim_string_LCD(TAMPIL);  
				kirim_string_LCD ("cm    ");
				kirim_perintah_LCD(0x80 + 0); //kembali ke kolom 1 dan baris 1
		
		}
		

    }
}

ISR(INT0_vect)//interrupt  routine
{
    if (i==1)//when logic from HIGH to LOW
    {
        TCCR1B=0;//disabling counter
        pulse=TCNT1;//count memory is updated to integer
        TCNT1=0;//resetting the counter memory
        i=0;
    }
    if (i==0)
    {
        TCCR1B|=(1<<CS10);
        i=1;
    }
}

void kirim_perintah_LCD(unsigned char command)
{
    PORTC = command;
    PORTD &= ~ (1<<register_num);
    PORTD |= 1<<enable;
    _delay_ms(8);
    PORTD &= ~1<<enable;
    PORTC = 0;
}

void kirim_karakter_LCD(unsigned char character)
{
    PORTC = character;
    PORTD |= 1<<register_num;
    PORTD |= 1<<enable;
    _delay_ms(8);
    PORTD &= ~1<<enable;
    PORTC = 0;
}
void kirim_string_LCD(char *string_of_characters)
{
    while(*string_of_characters > 0)
    {
        kirim_karakter_LCD(*string_of_characters++);
    }
}