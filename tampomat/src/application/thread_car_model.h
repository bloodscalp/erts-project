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

#ifndef THREAD_CAR_MODEL_H_
#define THREAD_CAR_MODEL_H_

#define CST_PEDALS_MIN	3 //3% à etre remplacé par une constante déclarée à une meilleures place
#define CST_PERIOD_CAR_MODEL  OS_TICKS_PER_SEC/20	//20Hz,50msec

static void thread_car_model (void *p_arg);



#endif /* THREAD_CAR_MODEL_H_ */
