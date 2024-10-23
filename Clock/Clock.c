/*
 * Clock.c
 *
 * Created: 10/23/2024 5:47:02 PM
 *  Author: AHMED GAMAL
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "timer.h"
#include "seven segment.h"
#include "keypad_driver.h"
#include "LCD.h"
#include "std_macros.h"

volatile unsigned char sec=0;
unsigned char hour=0,min=0;
unsigned char val,frist_dig,sec_dig;

int main(void)
{
	keypad_vInit();
	LCD_vInit();
	seven_seg_vinit('B');
	SET_BIT(DDRC,0);
	SET_BIT(DDRC,1);
	SET_BIT(DDRC,2);
	SET_BIT(DDRC,3);
	SET_BIT(DDRC,4);
	SET_BIT(DDRC,5);
	
	LCD_vSend_string("Press 1 to:");
	LCD_movecursor(2,1);
	LCD_vSend_string("set clock");
	timer2_overflow_init_interrupt();
	
    while(1)
    {
		val=keypad_u8check_press();
		if (val!=NOTPRESSED)
		{
			if(val=='1')
			{
				LCD_clearscreen();
				LCD_vSend_string("Hours=--");
				LCD_movecursor(1,7);
				_delay_ms(500);
				do 
				{
					frist_dig=keypad_u8check_press();
				} while (frist_dig==NOTPRESSED);
				LCD_vSend_char(frist_dig);
				_delay_ms(300);
				do 
				{
					sec_dig=keypad_u8check_press();
				} while (sec_dig==NOTPRESSED);
				LCD_vSend_char(sec_dig);
				_delay_ms(300);
				hour=(sec_dig-48)+(frist_dig-48)*10;
				LCD_clearscreen();
				
				LCD_vSend_string("Minutes=--");
				LCD_movecursor(1,9);
				do 
				{
					frist_dig=keypad_u8check_press();
				} while (frist_dig==NOTPRESSED);
				LCD_vSend_char(frist_dig);
				_delay_ms(300);
				do 
				{
					sec_dig=keypad_u8check_press();
				} while (sec_dig==NOTPRESSED);
				LCD_vSend_char(sec_dig);
				_delay_ms(300);
				min=(sec_dig-48)+10*(frist_dig-48);
				LCD_clearscreen();
				LCD_vSend_string("Seconds=--");
				LCD_movecursor(1,9);
				do 
				{
					frist_dig=keypad_u8check_press();
				} while (frist_dig==NOTPRESSED);
				LCD_vSend_char(frist_dig);
				_delay_ms(300);
				do 
				{
					sec_dig=keypad_u8check_press();
				} while (sec_dig==NOTPRESSED);
				LCD_vSend_char(sec_dig);
				_delay_ms(300);
				sec=(sec_dig-48)+10*(frist_dig-48);
				LCD_clearscreen();
				LCD_vSend_string("Press 1 to:");
				LCD_movecursor(2,1);
				LCD_vSend_string("set clock");
			}
			else
			{
				LCD_clearscreen();
				LCD_vSend_string("wrong choice");
				_delay_ms(1000);
				LCD_vSend_string("Press 1 to:");
				LCD_movecursor(2,1);
				LCD_vSend_string("set clock");
			}
		}
         
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		CLR_BIT(PORTC,0);
		seven_seg_write('B',sec%10);
		_delay_ms(5);
		
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		CLR_BIT(PORTC,1);
		seven_seg_write('B',sec/10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		CLR_BIT(PORTC,2);
		seven_seg_write('B',min%10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		CLR_BIT(PORTC,3);
		seven_seg_write('B',min/10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,5);
		CLR_BIT(PORTC,4);
		seven_seg_write('B',hour%10);
		_delay_ms(5);
		
		SET_BIT(PORTC,0);
		SET_BIT(PORTC,1);
		SET_BIT(PORTC,2);
		SET_BIT(PORTC,3);
		SET_BIT(PORTC,4);
		CLR_BIT(PORTC,5);
		seven_seg_write('B',hour/10);
		_delay_ms(5);
		
		if (sec>=60)
		{
			sec=0;
			min++;
		}
		if (min>=60)
		{
			min=0;
			hour++;
		}
		if (hour>12)
		{
			hour=1;
		}
    }
}


ISR(TIMER2_OVF_vect)
{
	sec++;
}