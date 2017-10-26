/* 
 * File:   PIN_Configuration.h
 * Author: OmarEmanuel
 *
 * Created on 4 de junio de 2016, 02:56 PM
 */

#ifndef PIN_CONFIGURATION_H
#define	PIN_CONFIGURATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "system.h"
    
/*Botones*/
#define BUTTON1 PORTAbits.RA3
#define BUTTON2 PORTAbits.RA4
#define BUTTON3 PORTAbits.RA5
    
/*Entrada de Contador*/
#define COUNT_INPUT     PORTAbits.RA1
#define COUNT_INDICATOR PORTCbits.RC4
    
/*Valvulas*/
#define VALVULA_A   PORTCbits.RC4
#define VALVULA_B   PORTCbits.RC5
    
/*Display*/
#define PIN_A_7SEG  PORTBbits.RB0
#define PIN_B_7SEG  PORTBbits.RB1
#define PIN_C_7SEG  PORTBbits.RB2
#define PIN_D_7SEG  PORTBbits.RB3
#define PIN_E_7SEG  PORTBbits.RB4
#define PIN_F_7SEG  PORTBbits.RB5
#define PIN_G_7SEG  PORTBbits.RB6
/*Transistores de multiplexor*/
#define DIGIT1_MUX_PORT     PORTCbits.RC0
#define DIGIT2_MUX_PORT     PORTCbits.RC1
#define DIGIT3_MUX_PORT     PORTCbits.RC2
    
/*HD44780*/
#define PIN_RS  LATD2
#define PIN_RW  LATD3
#define PIN_EN  LATD1
    
#define PIN_DB4 LATD4
#define PIN_DB5 LATD5
#define PIN_DB6 LATD6
#define PIN_DB7 LATD7
    
/*Virtual Uart*/
#define PIN_RX_VIRT_UART    PORTAbits.RA2
    
#ifdef	__cplusplus
}
#endif

#endif	/* PIN_CONFIGURATION_H */

