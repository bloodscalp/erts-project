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

#ifndef THREAD_CAR_MODEL_H_
#define THREAD_CAR_MODEL_H_

#define CST_PERIOD_CAR_MODEL  OS_TICKS_PER_SEC/20	//FIXME quelle periode? actuellement=> 20Hz,50msec

void thread_car_model(void *p_arg);

#endif /* THREAD_CAR_MODEL_H_ */
