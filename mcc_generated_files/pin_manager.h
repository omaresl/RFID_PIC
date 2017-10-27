/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.0
        Device            :  PIC18F46K40
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RB0 aliases
#define IO_RB0_TRIS               TRISBbits.TRISB0
#define IO_RB0_LAT                LATBbits.LATB0
#define IO_RB0_PORT               PORTBbits.RB0
#define IO_RB0_WPU                WPUBbits.WPUB0
#define IO_RB0_OD                ODCONBbits.ODCB0
#define IO_RB0_ANS                ANSELBbits.ANSELB0
#define IO_RB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define IO_RB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define IO_RB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define IO_RB0_GetValue()           PORTBbits.RB0
#define IO_RB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define IO_RB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define IO_RB0_SetPullup()      do { WPUBbits.WPUB0 = 1; } while(0)
#define IO_RB0_ResetPullup()    do { WPUBbits.WPUB0 = 0; } while(0)
#define IO_RB0_SetPushPull()    do { ODCONBbits.ODCB0 = 1; } while(0)
#define IO_RB0_SetOpenDrain()   do { ODCONBbits.ODCB0 = 0; } while(0)
#define IO_RB0_SetAnalogMode()  do { ANSELBbits.ANSELB0 = 1; } while(0)
#define IO_RB0_SetDigitalMode() do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set IO_RB1 aliases
#define IO_RB1_TRIS               TRISBbits.TRISB1
#define IO_RB1_LAT                LATBbits.LATB1
#define IO_RB1_PORT               PORTBbits.RB1
#define IO_RB1_WPU                WPUBbits.WPUB1
#define IO_RB1_OD                ODCONBbits.ODCB1
#define IO_RB1_ANS                ANSELBbits.ANSELB1
#define IO_RB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define IO_RB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define IO_RB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define IO_RB1_GetValue()           PORTBbits.RB1
#define IO_RB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define IO_RB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define IO_RB1_SetPullup()      do { WPUBbits.WPUB1 = 1; } while(0)
#define IO_RB1_ResetPullup()    do { WPUBbits.WPUB1 = 0; } while(0)
#define IO_RB1_SetPushPull()    do { ODCONBbits.ODCB1 = 1; } while(0)
#define IO_RB1_SetOpenDrain()   do { ODCONBbits.ODCB1 = 0; } while(0)
#define IO_RB1_SetAnalogMode()  do { ANSELBbits.ANSELB1 = 1; } while(0)
#define IO_RB1_SetDigitalMode() do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set IO_RB2 aliases
#define IO_RB2_TRIS               TRISBbits.TRISB2
#define IO_RB2_LAT                LATBbits.LATB2
#define IO_RB2_PORT               PORTBbits.RB2
#define IO_RB2_WPU                WPUBbits.WPUB2
#define IO_RB2_OD                ODCONBbits.ODCB2
#define IO_RB2_ANS                ANSELBbits.ANSELB2
#define IO_RB2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define IO_RB2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define IO_RB2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define IO_RB2_GetValue()           PORTBbits.RB2
#define IO_RB2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define IO_RB2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define IO_RB2_SetPullup()      do { WPUBbits.WPUB2 = 1; } while(0)
#define IO_RB2_ResetPullup()    do { WPUBbits.WPUB2 = 0; } while(0)
#define IO_RB2_SetPushPull()    do { ODCONBbits.ODCB2 = 1; } while(0)
#define IO_RB2_SetOpenDrain()   do { ODCONBbits.ODCB2 = 0; } while(0)
#define IO_RB2_SetAnalogMode()  do { ANSELBbits.ANSELB2 = 1; } while(0)
#define IO_RB2_SetDigitalMode() do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()     do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()   do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode() do { ANSELCbits.ANSELC6 = 1; } while(0)
#define RC6_SetDigitalMode()do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()    do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()   do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()   do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()         PORTCbits.RC7
#define RC7_SetDigitalInput()   do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()  do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()     do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()   do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode() do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RC7_SetDigitalMode()do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/