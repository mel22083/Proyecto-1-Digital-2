/*
 * LCD.h
 *
 * Created: 2/10/2026 10:12:21 AM
 *  Author: willi
 */ 

#define F_CPU 16000000UL
#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <util/delay.h>

//funcion para iniciar la LCD
void inicioLCD_8bits(void);

//funcion para
void LCD_Port(char a);

//funcion para enviar un comando
void LCD_comando(char a);

//funcion para enviar un caracter
void LCD_caracter(char c);

//funcion para enviar una cadena
void LCD_cadena(char *a);

//desplazamiento a la derecha
void LDC_desp_der(void);

//desplazamiento a la izquierda
void LCD_desp_izq(void);

//establecer el cursos
void LCD_cursor(char c, char f);

#endif /* LCD_H_ */
