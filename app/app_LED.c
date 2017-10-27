
#include <pic18f46k40.h>
#include "stdtypedef.h"
#include "app_LED.h"
#include "../mcc_generated_files/pin_manager.h"

/*************************************
 * Function Name: app_LED_Task
 * Description: TBD
 *************************************/
void app_LED_Task(void)
{
    IO_RB0_Toggle();
    IO_RB1_Toggle();
    IO_RB2_Toggle();
}