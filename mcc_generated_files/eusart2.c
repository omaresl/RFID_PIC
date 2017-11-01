/**
  EUSART2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart2.c

  @Summary
    This is the generated driver implementation file for the EUSART2 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for EUSART2.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.3
        Device            :  PIC18F46K40
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "eusart2.h"
#include "interrupt_manager.h"

/**
  Section: EUSART2 APIs
*/

void EUSART2_Initialize(void)
{
    // Set the EUSART2 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD2CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC2STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX2STA = 0x24;

    // Baud Rate = 9600; SP2BRGL 130; 
    SP2BRGL = 0x82;

    // Baud Rate = 9600; SP2BRGH 6; 
    SP2BRGH = 0x06;

}


uint8_t EUSART2_Read(void)
{
    INTERRUPT_GlobalInterruptHighDisable();
    while(!PIR3bits.RC2IF)
    {
    }


    if(1 == RC2STAbits.OERR)
{
        // EUSART2 error - restart

        RC2STAbits.SPEN = 0; 
        RC2STAbits.SPEN = 1; 
    }
    INTERRUPT_GlobalInterruptHighEnable();
    return RC2REG;
}

void EUSART2_Write(uint8_t txData)
{
    while(0 == PIR3bits.TX2IF)
{
        }

    TX2REG = txData;    // Write the data byte to the USART.
    }
/**
  End of File
*/
