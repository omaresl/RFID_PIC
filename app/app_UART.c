
#include "../Headers/system.h"
#include "../Headers/stdtypedef.h"
#include "app_UART.h"

/**********************************************************
 * Code
 **********************************************************/

/**********************************************************
 * Name: app_UART_SetBaudRate
 * Description: This function allows set the baud rate for
 * EUSART2
 **********************************************************/
void app_UART_SetBaudRate(void) {

    // Set the EUSART2 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD2CON = 0x00;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC2STA = 0x00;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX2STA = 0x00;

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD2CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC2STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX2STA = 0x24;

    // Baud Rate = 19200; SP2BRGL 208; 
    SP2BRGL = 0x40;

    // Baud Rate = 19200; SP2BRGH 0; 
    SP2BRGH = 0x03;
}