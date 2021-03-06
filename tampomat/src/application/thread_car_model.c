/***********************************************************************
 *  Function of the thread car_model
 *
 *	Filename    : 	thread_car_model.c
 *  Version     : 	V1.0
 *  Programmers : 	Mikael Trigo - Gregoire Hagmann
 *  				William Aebi - Christian Mueller
 *
 *  Email 		:	prenom.nom@master.hes-so.ch
 *
 ***********************************************************************/
#include <stdint.h>

#include "stm32f10x_includes.h"

#include "thread_car_model.h"
#include "regulation.h"
#include "globals.h"
#include "model.h"


void thread_car_model (void *p_arg)
{
	(void)p_arg;
	//pedals: 0% to 100%
	uint8_t breaks_sim;
	uint8_t throttle_sim;
	float speed_sensor;

//	int next_deadline = OSTimeGet()+CST_PERIOD_CAR_MODEL;
//	while (1) {
		//Take into account the pedals only if >= 3%, else value from regulator (throttle) or 0 (break)
		throttle_sim = get_acc_sensor();
		if (throttle_sim < PEDALS_MIN)
			throttle_sim = get_regulation_throttle();

		set_throttle(throttle_sim);

		breaks_sim = get_dec_sensor();
		if (breaks_sim < PEDALS_MIN){
			breaks_sim = 0;
			//Si on appuie sur les freins la FSm perds le controle
		}
		else 			throttle_sim = get_acc_sensor();

		//Update the speed of the car according to the pedals/regulator
		speed_sensor = get_speed_sensor();
		car_simulation(&speed_sensor, breaks_sim, throttle_sim);
		set_speed_sensor(speed_sensor);

//		OSTimeDly(next_deadline-OSTimeGet());
//		next_deadline += CST_PERIOD_CAR_MODEL;
//	}
//	OSTaskDel(OS_PRIO_SELF);
}
