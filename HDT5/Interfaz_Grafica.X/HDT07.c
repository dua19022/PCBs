/******************************************************************************
 * HDT 07 Lector de Potenciometros
 ******************************************************************************
 * File:   HDT07.c
 * Author: Marco
 * 
 *
 */

//-----------------------------------------------------------------------------
//                            Librerias 
//-----------------------------------------------------------------------------
/*
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf
#include <string.h> // Concatenar
#include <stdlib.h>

// Librerias propias

#include "ADC.h"



//-----------------------------------------------------------------------------
//                            Bits de configuracion 
//-----------------------------------------------------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//-----------------------------------------------------------------------------
//                            Variables 
//-----------------------------------------------------------------------------

uint8_t pot01, pot02, pot03, pot04;
char valor01, valor02, valor03, valor04;

//-----------------------------------------------------------------------------
//                            Prototipos 
//-----------------------------------------------------------------------------

void setup(void);
void chanel(void);
void putch(char data);
void text(void);


//-----------------------------------------------------------------------------
//                            Interrupciones
//-----------------------------------------------------------------------------
void __interrupt() isr(void){
   
    // Interrupcion del ADC
    
       if(PIR1bits.ADIF == 1)       // Reviso la bandera del ADC
       {

       if(ADCON0bits.CHS == 0) { // Si estoy en el canal 0 
             pot01 = ADRESH;
             
           }
       
        if(ADCON0bits.CHS == 1){        // canal 1 
            pot02 = ADRESH;
           }
       
        if(ADCON0bits.CHS == 2){       // canal 2
            pot03 = ADRESH;
            
           
             }
       if(ADCON0bits.CHS == 3){        // canal 3
            pot04 = ADRESH;

             }

           PIR1bits.ADIF = 0;
       }
}
//-----------------------------------------------------------------------------
//                            Main
//-----------------------------------------------------------------------------

void main(void) {
    setup();

    while(1){
        
        chanel();
        
        text();
        
        
    }
    return;
}
//-----------------------------------------------------------------------------
//                            Funciones
//-----------------------------------------------------------------------------

void setup(void){
    ANSEL = 0b00001111;
    ANSELH = 0b00000000;
    
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;

    //limpiar puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Configurar reloj interno
    OSCCONbits.IRCF0 = 0;        //reloj interno de 4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  //internal oscillator is used for system clock
    
    //configuracion de interrupciones
    PIE1bits.ADIE = 1;      //enable de la int del ADC
    PIR1bits.ADIF = 0;      //limpiar la interrupcion del ADC
    INTCONbits.GIE = 1;     //habilita las interrupciones globales
    INTCONbits.PEIE = 1;    //periferical interrupts
    
    // Configuracion del ADC
    ADC_config(2);
    
    // Configuraciones TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    
    SPBRG = 207;    //Baud Rate de 9600
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    TXSTAbits.TX9 = 0;          // 8 bits
    
    TXSTAbits.TXEN = 1;
    
    PIR1bits.RCIF = 0;  // Bandera rx
    PIR1bits.TXIF = 0;  // bandera tx
}

void chanel(void){  // Se crea rutina para cambio de canales
 
    if(ADCON0bits.GO == 0){     // Se crea un cambio de canal
            if(ADCON0bits.CHS == 0){ // si es 1 que se vuelva 0
                ADCON0bits.CHS = 1;
            }
            else if (ADCON0bits.CHS == 1){   // si no es 1, que se vuelva 1
                ADCON0bits.CHS = 2;
            }
            else if (ADCON0bits.CHS == 2){   // si no es 1, que se vuelva 1
                ADCON0bits.CHS = 3;
            }
            else if (ADCON0bits.CHS == 3){   // si no es 1, que se vuelva 1
                ADCON0bits.CHS = 0;
            }
            
            __delay_us(50);        // Se espera un tiempo para hacer el cambio
            ADCON0bits.GO = 1;  // Activo las conversiones
        }
}

void putch(char info){//Se transmite la cadena de caracteres a esta funcion 
                      // por el printf
    while (TXIF == 0);// Se espera algo que haya que transmitir
    TXREG = info;// lo que hay en data se pasa al registro de transmision para 
                 // para que se depliegue en la terminal virtual.
}

void text (void){
    
    // Comunicacion USART
        __delay_ms(150);
     printf("%d %d %d %d", pot01, pot02, pot03, pot04 );
     __delay_ms(150);
     

}
*/

#include <xc.h>
#include <stdint.h>
#include <stdio.h>// Libreria para poder usar printf junto a la funcion putch.
//------------------------------------------------------------------------------
//                         BITS DE CONFIGURACION
//------------------------------------------------------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO 
                                   //oscillator: I/O function on RA6/OSC2/CLKOUT
                                   //pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
                                // pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                // protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code
                                // protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                //(Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                //(Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3/PGM 
                                // pin has PGM function, low voltage programming 
                                // enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                //Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                //(Write protection off)

#define _XTAL_FREQ 8000000//Para usar la funcion de 'delay'

//------------------------------------------------------------------------------
//                                VARIABLES
//------------------------------------------------------------------------------
uint8_t POT1;
uint8_t POT2;
uint8_t POT3;
uint8_t POT4;
char centenas; //Paraa que el valor este en valores ASCII
char decenas;
char unidades;
int a, b, c;

//------------------------------------------------------------------------------
//                          PROTOTIPOS FUNCIONES 
//------------------------------------------------------------------------------
void setup(void); 
void putch(char data);
void division(uint8_t variable);
void text(void);
void timer(void);
// Se establece el vector de interrupcion
void __interrupt() isr(void){

    if (PIR1bits.ADIF == 1)//Interrupcion del ADC 
    {
        if (ADCON0bits.CHS == 0)//si se esta en este canal que haga lo siguiente
        {
            ADCON0bits.CHS = 1;
            POT1 = ADRESH;
        }
        else if (ADCON0bits.CHS == 1)
        {
            ADCON0bits.CHS = 2;
            POT2 = ADRESH;
        }
        else if (ADCON0bits.CHS == 2)
        {
            ADCON0bits.CHS = 3;
            POT3 = ADRESH;
        }
        else if (ADCON0bits.CHS == 3)
        {
            ADCON0bits.CHS = 0;
            POT4 = ADRESH;
        }
        __delay_us(50);//tiempo necesario para el cambio de canal 
        PIR1bits.ADIF = 0;//Se apaga el valor de la bandera de interrupcion ADC
    }
}
//------------------------------------------------------------------------------
//                             CICLO PRINCIPAL 
//------------------------------------------------------------------------------
void main(void) {
    setup();// Se llama a la funcion setup para configuracion de I/O
    
    while (1) // Se implemta el loop
    {
    ADCON0bits.GO = 1; //para empezar de nuevo la ejecucion del ADC
     
    text();
    
    timer();
}
}
//------------------------------------------------------------------------------
//                             CONFIGURACIONES
//------------------------------------------------------------------------------
void setup(void){
    // configuracion de puertos 
    ANSEL = 0b00001111; //setea AN0 y AN1
    ANSELH = 0X00;//se establecen los pines como entras y salidas digitales
    
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;//Se ponen como entradas los primeros pines del puertoB
    
    PORTA = 0X00;//Se limpian los puertos utilizados
    
    // configuracion del oscilador 
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1; //Se configura el oscilador a una frecuencia de 250KHz
    OSCCONbits.SCS = 1;
    
    // configuracion del ADC
  
    ADCON0bits.CHS = 0; // CANAL AN0
    
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 1; //Frc que trabaja con el oscilador interno
    
    ADCON0bits.ADON = 1; //Activa el modulo ADC
    
    ADCON1bits.ADFM = 0; // justificacion a la izquierda.
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;  //Vss y Vcc
    
    //Configuracion TX y RX 
    TXSTAbits.BRGH = 1;  // Para alta velocidad.
    BAUDCTLbits.BRG16 = 1; // Se usan los 16 bits
    
    TXSTAbits.SYNC = 0; // transmision asincrona
    RCSTAbits.SPEN = 1; // Se enciende el modulo 
    RCSTAbits.CREN = 1; // Se abilita la recepcion 
    
    TXSTAbits.TXEN = 1; // Se abilita la transmision 
    
    RCSTAbits.RX9 = 0; // Se determina que no se quieren 9 bits
    
    SPBRG = 207; //BAUD RATE de 9600
    SPBRGH = 0;
    
    // configuracion de interrupciones 
    INTCONbits.GIE = 1;
    PIR1bits.ADIF = 0; // BANDERA de interrupcion del ADC
    PIE1bits.ADIE = 1; // Habilita la interrupcion del ADC
    INTCONbits.PEIE = 1; // Interrupcion de los periferico   
}
void putch(char info){//Se transmite la cadena de caracteres a esta funcion 
                      // por el printf
    while (TXIF == 0);// Se espera algo que haya que transmitir
    TXREG = info;// lo que hay en data se pasa al registro de transmision para 
                 // para que se depliegue en la terminal virtual.
}
void division(uint8_t variable){
    uint8_t val;
    val = (variable);              //Se guarda en valor la variable que entra
    centenas = (val/100);       //SE OBTIENE EL VALOR DE CENTRENAS
    val = (val - (centenas*100));
    decenas = (val/10);         //SE OBTIENE EL VALOR DE DECENAS
    val = (val - (decenas*10));
    unidades = (val);         //SE OBTIENE EL VALOR DE UNIDADES
    
    centenas = centenas + 48; //Paraa que el valor este en valores ASCII
    decenas = decenas + 48;
    unidades = unidades + 48;
}

void text(void){
     __delay_ms(50);
     printf(" ");
     __delay_ms(50);
     division(POT1);
     __delay_ms(50);
     if(TXIF == 1){
        TXREG = centenas; 
       }
     __delay_ms(50);
    if(TXIF == 1){
        TXREG = decenas; 
       }
    __delay_ms(50);
    if(TXIF == 1){
        TXREG = unidades; 
       }
     __delay_ms(50);
     printf(" ");
     __delay_ms(50);
     
     division(POT2);
     __delay_ms(50);
     if(TXIF == 1){
        TXREG = centenas; 
       }
     __delay_ms(50);
    if(TXIF == 1){
        TXREG = decenas; 
       }
    __delay_ms(50);
    if(TXIF == 1){
        TXREG = unidades; 
       }
     __delay_ms(50);
     printf(" ");
     __delay_ms(50);
     
   division(POT3);
     __delay_ms(50);
     if(TXIF == 1){
        TXREG = centenas; 
       }
     __delay_ms(50);
    if(TXIF == 1){
        TXREG = decenas; 
       }
    __delay_ms(50);
    if(TXIF == 1){
        TXREG = unidades; 
       }
     __delay_ms(50);
     printf(" ");
     __delay_ms(50);
     
     division(POT4);
     __delay_ms(50);
     if(TXIF == 1){
        TXREG = centenas; 
       }
     __delay_ms(50);
    if(TXIF == 1){
        TXREG = decenas; 
       }
    __delay_ms(50);
    if(TXIF == 1){
        TXREG = unidades; 
       }
     __delay_ms(50);
    
    __delay_ms(50);
     printf(" ");
     __delay_ms(50);
}

void timer (void){
    a = 1;
    b = 0;
    
    while (a == 1){
        b = b + 1;
        if (b == 30000){
            a = 0;
        }
        else{
            a = 1;
        }
    }
    
}