/* 
 * File:   app_io.h
 * Author: Omar Sevilla
 *
 * Created on 5 de diciembre de 2013, 02:30 AM
 */

#ifndef APP_IO_H
#define	APP_IO_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "system.h"
#include "stdtypedef.h"

#define TOGGLE_PIN(PIN)                 if(PIN == TRUE){PIN = 0;}\
                                        else{PIN = 1;}\
                                        PIN = PIN

#define SET_PIN(PIN)                    PIN = 1
#define CLEAR_PIN(PIN)                  PIN = 0


#ifdef	__cplusplus
}
#endif

#endif	/* APP_IO_H */

