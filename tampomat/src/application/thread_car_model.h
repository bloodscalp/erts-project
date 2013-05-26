/***********************************************************************
 *  Header for function of the thread car_model
 *
 *  OS: uCosII
 *  Cible: NTRT
 *
 *	Filename      : thread_car_model.h
 *  Version       : V1.1
 *  Programmer(s) : Gregoire Hagmann
 ***********************************************************************/
#include "model.h"
#include "getset.h"
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include <includes.h>
//#include "stm32f10x_includes.h"

#ifndef THREAD_CAR_MODEL_H_
#define THREAD_CAR_MODEL_H_

#define CST_PERIOD_CAR_MODEL  OS_TICKS_PER_SEC/20	//FIXME quelle periode? actuellement=> 20Hz,50msec

static void thread_car_model (void *p_arg);

#endif /* THREAD_CAR_MODEL_H_ */
