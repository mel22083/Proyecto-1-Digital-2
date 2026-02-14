/*
 * I2C.c
 *
 * Created: 2/10/2026 4:56:56 PM
 *  Author: willi
 */ 

#include "I2C.h"

void I2C_MASTER_INIT(unsigned long SCL_Clock, uint8_t Prescaleri2)
{
	DDRC &= ~((1 << PC4) | (1 << PC5)); // ENTRADAS PC4 Y PC5 (SDA Y SCL)
	
	switch(Prescaleri2)
	{
		case 1:
		TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
		break;
		
		case 4:
		TWSR &= ~(1 << TWPS1);
		TWSR |= (1 << TWPS0);
		break;
		
		case 16:
		TWSR &= ~(1 << TWPS0);
		TWSR |= (1 << TWPS1);
		break;
		
		case 64:
		TWSR |= (1 << TWPS1) | (1 << TWPS0);
		break;
		default:
		TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
		Prescaleri2 = 1;
		break;
		
	}
	TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaleri2); // Calculo de velocidad
	TWCR |= (1 << TWEN); // "ENCENDER" I2C
}

uint8_t I2C_MASTER_START(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);// Activar bandera de interrupcion, hacerse master y echar a andar la comunicacion.
	while (!(TWCR & (1<< TWINT))); // SE ESPERA A QUE SE ENCIENDA LA BANDERA DE INTERRUPCION
	return ((TWSR & 0xF8) == 0x08); // se comparan los bits de estado con 0x08 para empezar la comunicacion
}

uint8_t I2C_MASTER_REPEATEDSTART(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);// Activar bandera de interrupcion, hacerse master y echar a andar la comunicacion.
	while (!(TWCR & (1<< TWINT))); // SE ESPERA A QUE SE ENCIENDA LA BANDERA DE INTERRUPCION
	return ((TWSR & 0xF8) == 0x10); // se comparan los bits de estado con 0x10 para repetir el comienzo de la comunicacion
}

void I2C_MASTER_STOP(void)
{
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO); // Envio de secuencia de parada
	while (TWCR & (1 << TWSTO)); // Se espera a que el bit twsto este en 0
}

uint8_t I2C_MASTER_WRITE(uint8_t dato) // escritura de datos en slave
{
	uint8_t estado;
	
	TWDR = dato; // se carga el dato al registro de datos
	TWCR = (1 << TWEN) | (1 << TWINT); // Inicio de la secuencia de envio de datos
	
	while (!(TWCR & (1 << TWINT))); // ESPERA A QUE LA BANDERA DE INTERRUPCION SE ENCIENDA
	estado = TWSR & 0xF8;  // UTILIZAR SOLO BITS DE ESTADO
	
	if (estado == 0x18 || estado == 0x28) // Se revisa si se envio un SLA (direccion) + W (dato a escribir) + ACK O UN DATO + ACK
	{
		return 1;
	}
	else
	{
		return estado;
	}
}

uint8_t I2C_MASTER_READ(uint8_t *buffer, uint8_t ack)
{
	uint8_t estado;
	if (ack)
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // SE "ENCIENDE" I2C, ACTIVA INTERRUPCION Y SE HABILITA ACKNOWLEDGE AUTOMATICO
	}
	else
	{
		TWCR = (1 << TWINT) | (1 << TWEN); // "ENCENDER" I2C sin envio de ack
	}
	
	while (!(TWCR &(1 << TWINT))); // ESPERAR FLAG DE INTERRUPCION
	estado = TWSR & 0xF8; // LEER NADA MAS BITS DE ESTADO
	if(ack && estado != 0x50) return 0; // revisar si se recibio dato con ack
	if(!ack && estado != 0x58) return 0; // revisar dato y no ack
	
	*buffer = TWDR; // SE ESCRIBE EL DATO RECIBIDO EN UN BUFFER
	return 1;
}

void I2C_SLAVE_INIT(uint8_t address)
{
	DDRC &= ~((1 << PC4) | (1 << PC5));  // PINES I2C COMO ENTRADAS
	TWAR = address << 1; // ASIGNACION DE DIRECCION
	TWCR = (1 << TWEA) | (1 << TWEN)| (1 << TWIE);
}
