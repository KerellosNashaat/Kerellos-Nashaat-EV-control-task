
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#define D4 eS_PORTA4
#define D5 eS_PORTA5
#define D6 eS_PORTA6
#define D7 eS_PORTA7
#define RS eS_PORTB0
#define EN eS_PORTB1

#define BUTTON1 0
#define BUTTON2 1

#include <avr/io.h>
#include "lcd.h"
#include <util/delay.h>


int main()
 {
    unsigned char pressedbutton;
    unsigned char Txmessage;
    unsigned char Rxmessage;

    DDRA = 0x0F;
    DDRB = 0b00000011;
    Lcd4_Init();

    //Communication UART specifications
    int UBRR_value=25;
    UBRRH=(unsigned char)(UBRR_value>>8);
    UBRRL=(unsigned char)(UBRR_value);
    UCSRB=(1<<RXEN)|(1<<TXEN);
    UCSRC |= (1 << USBS) | (3 << UCSZ0);

   while (1)
   {
       Lcd4_Clear();
      //1-The initial state for AVR 1: waiting for user to choose a question
      while ( 1 )
      {

	 Lcd4_Set_Cursor(1,0);
	 Lcd4_Write_String("Choose Questions");
	 Lcd4_Set_Cursor(2,0);
	 Lcd4_Write_String("1.Busy, 2.Hungry");
	 if ((PINA & (1<<BUTTON1)))
	 {
	    pressedbutton=0x10;
	    break;
	 }
	 if ((PINA & (1<<BUTTON2)))
	 {
	    pressedbutton=0x01;
	    break;
	 }

      }

      //2-Sending the presed button to AVR 2
	 Txmessage=pressedbutton;

	 while (! (UCSRA & (1 << UDRE)) ); //Wait until the Transmitter is ready
	 UDR=Txmessage;	//Send the message

	 Lcd4_Clear();

	//3-waiting a data to be sent from AVR 2
	 while ( !(UCSRA & (1 << RXC)) )
	 {


	    if (pressedbutton==0x10)
	    {
	       Lcd4_Set_Cursor(1,0);
	       Lcd4_Write_String("Asked if Busy");
	    }
	    else if (pressedbutton==0x01)
	    {
	       Lcd4_Set_Cursor(1,0);
	       Lcd4_Write_String("Asked if Hungry");
	    }

	    Lcd4_Set_Cursor(2,0);
	    Lcd4_Write_String("Waiting answer.");
	 }

	 //4-Receive the sent data, and check the sent Answer
	 Rxmessage=UDR;

	 Lcd4_Clear();
	 if (Rxmessage==0x10)
	 {
	    Lcd4_Set_Cursor(1,0);
	    Lcd4_Write_String("Yes, I am.");
	 }

	 else if (Rxmessage==0x01)
	 {
	    Lcd4_Set_Cursor(1,0);
	    Lcd4_Write_String("No, I am not.");
	 }

	 _delay_ms(2000);

	 //finished and return to the initial state

 }
  return 0;
 }

