/*
 * OS_SEBELL_schedule.c
 *
 *  Created on: 10/08/2015
 *      Author: uidj2522
 */


#include "OS_SEBELL_sched.h"
#include "OS_SEBELL_sched_util.h"
#include "OS_SEBELL_sched_cfg.h"
#include "../periodic_task_exec.h"

unsigned char sch_alarm;
T_SCH_CNT scheduler_counter;
unsigned char Execute_task;

const FCT_POINTER tasks_list[MAX_TASKS] =
{
		&periodic_task_tk,
		&periodic_task_2tk,
		&periodic_task_4tk,
		&periodic_task_8tk,
		&TASK_DUMMY,//16tk
		&periodic_task_32tk,//32tk
		&TASK_DUMMY,//64tk
		&TASK_DUMMY,//128tk
        &TASK_DUMMY,//256tk
        &TASK_DUMMY,//512tk
        &periodic_task_1024tk,//1024tk
        &TASK_DUMMY,//2048tk
        &TASK_DUMMY,//4096tk
        &TASK_DUMMY,//8192tk
        &TASK_DUMMY,//16384tk
        &TASK_DUMMY//32768tk
};

/************************************************************
 * Name: scheduler_loop
 * Description: Loop that will search and execute the task
 * Parameters: Void
 * Return: Void
*************************************************************/

void scheduler_loop(void)
{
	/*Start the loop*/
	for(;;){
		if( ALARM_ACTIVE == sch_alarm){
				/*Time to work! , increment the counter and turn off the alarm.*/
				sch_alarm = ALARM_INACTIVE;
				scheduler_counter++;
				Execute_task = EXECUTE;
				/*See if we already over passed the value!
			  Note: Checking after count increment is safe since the max value is 254
			  */
			if(SCH_MAX_COUNT < scheduler_counter){
				scheduler_counter = SCH_MIN_COUNT; //Clear counter
			}//End of check counter
			else{
				/*Do nothing*/
			}
		}//End of Alarm Active check
		else{
			if(EXECUTE == Execute_task  ){
				/*Time to execute*/
					Execute_task = IDLE; //Now rest for a while
					tasks_list[sched_util_task_finder(scheduler_counter)](); //Execute task
                    }
		}//End of executing

	}//End of infinite loop
}//End of scheduler_loop function




