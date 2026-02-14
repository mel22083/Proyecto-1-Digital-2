/*
 * I2C.h
 *
 * Created: 2/10/2026 4:55:32 PM
 *  Author: willi
 */ 


#ifndef I2C_H_
#define I2C_H_

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <stdint.h>

void I2C_MASTER_INIT(unsigned long SCL_Clock, uint8_t Prescaleri2); // inicializacion maestro
uint8_t I2C_MASTER_START(void); // inicio de la comunicacion
uint8_t I2C_MASTER_REPEATEDSTART(void);
void I2C_MASTER_STOP(void); // fin de la comunicacion
uint8_t I2C_MASTER_WRITE(uint8_t dato); // enviar datos al esclavo
uint8_t I2C_MASTER_READ(uint8_t *buffer, uint8_t ack); // leer datos enviados por esclavo
void I2C_SLAVE_INIT(uint8_t address); // inicializacion esclavo // revisado 1 y bien






#endif /* I2C_H_ */
