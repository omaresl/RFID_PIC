#include "periodic_task_exec.h"
#include "mcc_generated_files/mcc.h"
#include "app/app_LED.h"
#include "app/app_RC522.h"

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
    //app_RC522_TaskMng();
    return;
}

void periodic_task_32tk(void) {
    return;
}

void periodic_task_512tk(void) {
    return;
}

void periodic_task_1024tk(void) {
    app_LED_Task();
    return;
}

void TASK_DUMMY(void) {
    return;
}