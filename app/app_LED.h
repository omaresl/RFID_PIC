/* 
 * File:   app_LED.h
 * Author: uidj2522
 *
 * Created on October 26, 2017, 2:56 PM
 */

#ifndef APP_LED_H
#define	APP_LED_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "../mcc_generated_files/pin_manager.h"

    /*************************************
     * Definitions
     *************************************/
#define LED_RED_ON          IO_RB0_SetHigh
#define LED_RED_OFF         IO_RB0_SetLow
#define LED_RED_TOOGLE      IO_RB0_Toggle

#define LED_GREEN_ON        IO_RB2_SetHigh
#define LED_GREEN_OFF       IO_RB2_SetLow   
#define LED_GREEN_TOOGLE    IO_RB2_Toggle

#define LED_BLUE_ON         IO_RB1_SetHigh
#define LED_BLUE_OFF        IO_RB1_SetLow
#define LED_BLUE_TOOGLE     IO_RB1_Toggle

    /*************************************
     * Prototypes
     *************************************/
    extern void app_LED_Task(void);


#ifdef	__cplusplus
}
#endif

#endif	/* APP_LED_H */

