/***********************************************************************
 *  Function of the thread car_model
 *
 *  OS: uCosII
 *  Cible: NTRT
 *
 *	Filename      : thread_car_model.h
 *  Version       : V1.1
 *  Programmer(s) : Gregoire Hagmann
 ***********************************************************************/
#include "thread_car_model.h"

static void thread_car_model (void *p_arg)
{
	(void)p_arg;

	//pedals: 0 to 100 = %
	int breaks_sim;
	int throttle_sim;

	int prochaine_echeance = OSTimeGet()+CST_PERIOD_CAR_MODEL;
	while (1) {
		/*if (acc_sensor >= CST_PEDALS_MIN)
			throttle_sim=acc_sensor;
		else
			throttle_sim=throttle;
		if (dec_sensor >= CST_PEDALS_MIN)
			breaks_sim=acc_sensor;
		else
			breaks_sim=0;

		simulation(&speed_sensor, breaks_sim, throttle_sim);
*/
		OSTimeDly(prochaine_echeance-OSTimeGet());
		prochaine_echeance=prochaine_echeance+CST_PERIOD_CAR_MODEL;
	}
	OSTaskDel(OS_PRIO_SELF);
}
