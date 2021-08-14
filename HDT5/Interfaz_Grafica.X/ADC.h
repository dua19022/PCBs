/* 
 * File:   ADC.h
 * Author: Marco
 *
 * Created on 18 de julio de 2021
 */


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#pragma config FOSC = INTRC_NOCLKOUT
#include <xc.h> // include processor files - each processor file is guarded.  

//defino el prototipo de la funcion
void ADC_config(char frec); //tomara el valor que deseamos de la frecuencia

#endif	/* XC_HEADER_TEMPLATE_H */