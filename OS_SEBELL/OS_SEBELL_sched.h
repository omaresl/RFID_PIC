/*
 * OS_SEBELL_schedule.h
 *
 *  Created on: 10/08/2015
 *      Author: uidj2522
 */

#ifndef OS_SEBELL_OS_SEBELL_SCHEDULE_H_
#define OS_SEBELL_OS_SEBELL_SCHEDULE_H_


#define ALARM_ACTIVE 0x01u
#define ALARM_INACTIVE 0x00u
#define EXECUTE 0x01u
#define IDLE   0x00u

void scheduler_loop(void);

typedef void( *FCT_POINTER )(void);

extern unsigned char sch_alarm;

#endif /* OS_SEBELL_OS_SEBELL_SCHEDULE_H_ */
