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
    return;
}

void periodic_task_32tk(void) {
    /* RC522 Task */
    app_RC522_TaskMng();
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
    if (app_RC522_ModuleIsFail() == FALSE) {
        //app_LED_Task();
    } else {
        /* Do Nothing */
    }
    return;
}

void TASK_DUMMY(void) {
    return;
}