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

	//pedals: 0% to 100%
	uint8_t breaks_sim;
	uint8_t throttle_sim;
	float speed_sensor;

	int prochaine_echeance = OSTimeGet()+CST_PERIOD_CAR_MODEL;
	while (1) {
		//Take into account the pedals only if >= 3%, else value from regulator (throttle) or 0 (break)
		throttle_sim=get_acc_sensor();
		if (throttle_sim < PEDALS_MIN)
			throttle_sim=get_throttle();
		breaks_sim=get_dec_sensor();
		if (breaks_sim < PEDALS_MIN)
			breaks_sim=0;

		//Update the speed of the car according to the pedals/regulator
		speed_sensor=get_speed_sensor();
		simulation(&speed_sensor, breaks_sim, throttle_sim);
		set_speed_sensor(speed_sensor);

		OSTimeDly(prochaine_echeance-OSTimeGet());
		prochaine_echeance=prochaine_echeance+CST_PERIOD_CAR_MODEL;
	}
	OSTaskDel(OS_PRIO_SELF);
}
