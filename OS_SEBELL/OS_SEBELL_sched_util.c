/*
 * OS_SEBELL_sched_util.c
 *
 *  Created on: 10/08/2015
 *      Author: uidj2522
 */
#include "OS_SEBELL_sched_cfg.h"
#include "OS_SEBELL_sched_util.h"


/*Task Finder*/
unsigned char sched_util_task_finder(T_SCH_CNT sch_counter)
{
	/*Exit Indicator for Task Finder*/
	unsigned char lub_ExitFlag = 0x01;
	/*Task Finder Index*/
	unsigned char lub_TaskIndex = 0x00;

	for(lub_TaskIndex = 0; (lub_TaskIndex < (MAX_TASKS - 1)) && (lub_ExitFlag == 0x01); lub_TaskIndex++)
	{
		/*Check for the next Task*/
		if(((sch_counter >> lub_TaskIndex) & TASK_FINDER_MASK) == 0x01)
		{
			/*Task Found it. Exit from for loop*/
			lub_ExitFlag = 0x00;
		}
	}
	/*Return the index*/
	return(lub_TaskIndex - 1);
}


//TODO: change the magic numbers for TRUE and FALSE statments
