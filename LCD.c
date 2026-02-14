/*
 * LCD.c
 *
 * Created: 2/10/2026 10:12:01 AM
 *  Author: willi
 */ 

#include "LCD.h"

void inicioLCD_8bits(void){
	DDRD = 0xFC;
	DDRB |= (1<<PORTB0) | (1<<PORTB1);
	DDRC |= (1<<PORTC2) | (1<<PORTC3);   // A2 y A3 como salidas
	PORTD = 0x00;
	PORTB &= ~((1<<PORTB0) | (1<<PORTB1));
	PORTC &= ~((1<<PORTC2) | (1<<PORTC3));
	_delay_ms(20);
	//encendido
	LCD_Port(0x00);
	_delay_ms(15);
	LCD_comando(0x30);
	_delay_ms(5);
	LCD_comando(0x30);
	_delay_us(100);
	LCD_comando(0x30);
	_delay_us(100);
	LCD_comando(0x38);   // 8-bit
	LCD_comando(0x08);   // display apagado
	LCD_comando(0x01);   // borramos
	_delay_ms(2);
	LCD_comando(0x06);   // entry mode
	LCD_comando(0x0C);
}

void LCD_comando(char a){
	PORTB &= ~(1<<PORTB0);   // RS = 0
	LCD_Port(a);

	PORTB |=  (1<<PORTB1);   // E = 1
	_delay_us(2);
	PORTB &= ~(1<<PORTB1);   // E = 0

	if((uint8_t)a == 0x01 || (uint8_t)a == 0x02)
	_delay_ms(2);
	else
	_delay_us(50);
}

//mapeo de los 8 bits
void LCD_Port(char a)
{
	if(a & (1<<0)) PORTC |=  (1<<PORTC2);
	else           PORTC &= ~(1<<PORTC2);

	if(a & (1<<1)) PORTC |=  (1<<PORTC3);
	else           PORTC &= ~(1<<PORTC3);

	if(a & (1<<2)) PORTD |=  (1<<PORTD2);
	else           PORTD &= ~(1<<PORTD2);

	if(a & (1<<3)) PORTD |=  (1<<PORTD3);
	else           PORTD &= ~(1<<PORTD3);

	if(a & (1<<4)) PORTD |=  (1<<PORTD4);
	else           PORTD &= ~(1<<PORTD4);

	if(a & (1<<5)) PORTD |=  (1<<PORTD5);
	else           PORTD &= ~(1<<PORTD5);

	if(a & (1<<6)) PORTD |=  (1<<PORTD6);
	else           PORTD &= ~(1<<PORTD6);

	if(a & (1<<7)) PORTD |=  (1<<PORTD7);
	else           PORTD &= ~(1<<PORTD7);
}

//funcion caracter
void LCD_caracter(char c)
{
	PORTB |= (1<<PORTB0);    // RS = 1 (dato)
	LCD_Port(c);

	// Pulso E
	PORTB |=  (1<<PORTB1);
	_delay_us(2);
	PORTB &= ~(1<<PORTB1);
	_delay_us(50);
}

//funcion cadena
void LCD_cadena(char *a)
{
	uint8_t i = 0;

	while (a[i] != '\0')
	{
		LCD_caracter(a[i]);
		i++;
	}
}

//desplazamiento derecha
void LCD_desp_der(void)
{
	LCD_comando(0x1C);
}

//desplazamiento izquierda
void LCD_desp_izq(void)
{
	LCD_comando(0x18);
}

//Cursor
void LCD_cursor(char c, char f)
{
	char direccion;

	if (f == 1)
	{
		direccion = 0x80 + (c - 1);   // fila 1
	}
	else if (f == 2)
	{
		direccion = 0xC0 + (c - 1);   // fila 2
	}
	else
	{
		return; // fila inválida
	}

	LCD_comando(direccion);
}

