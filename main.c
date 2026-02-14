/*
 * Proyecto 1, Digital 2
 *
 * Created: 2/10/2026 10:09:51 AM
 * Author : William Melgar Pineda
 * Carnet : 22083
 */ 

// Librerias
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "I2C.h"
#include "UART.h"

#define slave1 0x30
#define slave2 0x40

#define slave1R (0x30 << 1) | 0x01
#define slave1W ((0x30 << 1) | 0x00)
uint8_t direccion;
uint8_t temp;
uint8_t bufferI2C = 0;
uint8_t buffer;
uint8_t ack;
uint8_t a = 0;


static void LCD_print1023(uint16_t x)
{
	LCD_caracter('0' + (x / 1000));        // miles
	LCD_caracter('0' + ((x / 100) % 10));  // centenas
	LCD_caracter('0' + ((x / 10) % 10));   // decenas
	LCD_caracter('0' + (x % 10));          // unidades
}

int main(void)
{
	DDRB |= (1 << PB2);   // PB2 como salida
	PORTB &= ~(1 << PB2);
	
	DDRB |= (1 << PORTB5);
	PORTB &= ~(1 << PORTB5);
	
	I2C_MASTER_INIT(100000,1); // iniciarlo como master a 100kHz, Prescaler 1
	
	_delay_ms(50);
	inicioLCD_8bits();
	LCD_cursor(3,1);
	LCD_cadena("S1:");
	_delay_ms(500);
	
	LCD_cursor(8,1);
	LCD_cadena("S2:");
	_delay_ms(500);
	
	LCD_cursor(13,1);
	LCD_cadena("S3:");
	_delay_ms(500);
	//PORTB |= (1 << PORTB2);

	while(1)
	{
		if (!I2C_MASTER_START())
		{
			I2C_MASTER_STOP();
			return;
		}
		
		if(!I2C_MASTER_WRITE(slave1W))
		 {
			 I2C_MASTER_STOP();
			 return;
		}
		if (!I2C_MASTER_WRITE('R'))
		{
			
			I2C_MASTER_STOP();
			return;
		}
		if(!I2C_MASTER_REPEATEDSTART())
		{
			I2C_MASTER_STOP();
			return;
		}
		if(!I2C_MASTER_WRITE(slave1R))
		{
			I2C_MASTER_STOP();
			return;
		}
		if (!I2C_MASTER_READ(&a, 0))
		{
			I2C_MASTER_STOP();
			return;
		}
		
		I2C_MASTER_STOP();
		
		LCD_cursor(2,2);
		if (a) LCD_cadena("si");
		else   LCD_cadena("no");

		_delay_ms(200);
		
	}
}
