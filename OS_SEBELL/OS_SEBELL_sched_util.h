/*
 * OS_SEBELL_sched_util.h
 *
 *  Created on: 10/08/2015
 *      Author: uidj2522
 */


#ifndef OS_SEBELL_OS_SEBELL_SCHED_UTIL_H_
#define OS_SEBELL_OS_SEBELL_SCHED_UTIL_H_

#include "OS_SEBELL_sched_cfg.h"

#define TASK_FINDER_MASK	0x01	//Max Task Number
#define SCH_MAX_COUNT		0xFFFE	//Max Scheduler Counter Value
#define SCH_MIN_COUNT		0x0001	//Min Scheduler Counter Value

unsigned char sched_util_task_finder(T_SCH_CNT sch_counter);


#endif /* OS_SEBELL_OS_SEBELL_SCHED_UTIL_H_ */
