/* 
 * File:   periodic_task_exec.h
 * Author: Omar Sevilla
 *
 * Created on 6 de diciembre de 2013, 01:37 AM
 */

#ifndef PERIODIC_TASK_EXEC_H
#define	PERIODIC_TASK_EXEC_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "system.h"
#include "stdtypedef.h"

    extern void periodic_task_tk(void);
    extern void periodic_task_2tk(void);
    extern void periodic_task_4tk(void);
    extern void periodic_task_8tk(void);
    extern void periodic_task_32tk(void);
    extern void periodic_task_1024tk(void);
    extern void TASK_DUMMY(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PERIODIC_TASK_EXEC_H */

