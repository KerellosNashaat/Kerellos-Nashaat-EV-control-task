
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

      //1-The initial state for AVR 2: waiting a data to be sent from AVR 1
      while ( !(UCSRA & (1 << RXC)) )
	 {
	    Lcd4_Set_Cursor(1,0);
	    Lcd4_Write_String("Waiting for");
	    Lcd4_Set_Cursor(2,0);
	    Lcd4_Write_String("question.");
	 }

      //2-Receive the sent data, and check the sent question
      Rxmessage=UDR;

      Lcd4_Clear();

      if (Rxmessage==0x10)	//the sent question is "Are you Busy"
      {
	 //3- waiting for user to choose an option
	 while (1)
	 {
	    Lcd4_Set_Cursor(1,0);
	    Lcd4_Write_String("Are you Busy");
	    Lcd4_Set_Cursor(2,0);
	    Lcd4_Write_String("1.Yes, 2.No");

	 if ((PINA & (1<<BUTTON1)))
	 {
	    Txmessage=0x10;	//the user answer is: yes
	    break;
	 }
	 if ((PINA & (1<<BUTTON2)))
	 {
	    Txmessage=0x01;	//the user answer is: no
	    break;
	 }
	 }

      }
      else if (Rxmessage==0x01)		//the sent question is "Are you Hungry"
      {
	 //3- waiting for user to choose an option
	 while (1)
	 {

	    Lcd4_Set_Cursor(1,0);
	    Lcd4_Write_String("Are you Hungry");
	    Lcd4_Set_Cursor(2,0);
	    Lcd4_Write_String("1.Yes, 2.No");

	 if ((PINA & (1<<BUTTON1)))
	 {
	    Txmessage=0x10;	//the user answer is: yes
	    break;
	 }

	 if ((PINA & (1<<BUTTON2)))
	 {
	    Txmessage=0x01;	////the user answer is: no
	    break;
	 }

	 }


    }

     //4-Sending the answer to AVR 1
	 while (! (UCSRA & (1 << UDRE)) ); //Wait until the Transmitter is ready
	 UDR=Txmessage;  //send the answer

	 //finished and return to the initial state


 }
  return 0;
 }
