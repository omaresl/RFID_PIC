#include "periodic_task_exec.h"
#include "mcc_generated_files/mcc.h"
#include "app/app_LED.h"

void periodic_task_tk(void)
{
    return;
}

void periodic_task_2tk(void)
{
    return;
}

void periodic_task_4tk(void)
{
    return;
}

void periodic_task_8tk(void)
{
    return;
}

void periodic_task_32tk(void)
{
    app_LED_Task();
    return;
}

void periodic_task_1024tk(void)
{

    return;
}

void TASK_DUMMY(void)
{
    return;
}