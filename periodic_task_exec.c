#include "periodic_task_exec.h"
#include "mcc_generated_files/mcc.h"
#include "app/app_LED.h"
#include "app/app_RC522.h"
#include "mcc_generated_files/eusart2.h"

void periodic_task_tk(void) {
    return;
}

void periodic_task_2tk(void) {
    return;
}

void periodic_task_4tk(void) {
    return;
}

void periodic_task_8tk(void) {
    /* RC522 Task */
    app_RC522_TaskMng();
    return;
}

void periodic_task_32tk(void) {
    if(IO_RD0_GetValue() == FALSE)
    {
        app_RC522_WriteRequest();
        LED_BLUE_ON();
    }
    else if(IO_RD2_GetValue() == FALSE)
    {
        app_RC522_EraseRequest();
        LED_RED_ON();
        LED_BLUE_ON();
    }
    else
    {
        
    }
    return;
}

void periodic_task_128tk(void) {
    return;
}

void periodic_task_512tk(void) {
    if (app_RC522_ModuleIsFail() == TRUE) {
        LED_RED_TOOGLE();
        LED_GREEN_OFF();
        LED_BLUE_OFF();
    } else {
        /* Do Nothing */
    }
    return;
}

void periodic_task_1024tk(void) {
    if (app_RC522_ModuleIsFail() == FALSE && app_RC522_SearchOnGoing() == TRUE) {
        LED_RED_OFF();
        LED_GREEN_OFF();
        LED_BLUE_TOOGLE();
    } else {
        LED_BLUE_OFF();
    }
    return;
}

void TASK_DUMMY(void) {
    return;
}