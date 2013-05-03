/*
 * regulation.c
 *
 *  Created on: 3 mai 2013
 *      Author: William
 */
#define THROTTLE_SAT	45.0
#define THROTTLE_ZERO	0.0
#define KP				8.113
#define KI				0.5

static float prop_act, int_act, pre_int = 0.0, throttle_loc = 0.0;

void proport_integr(float cruise_speed, float speed_sensor){
	prop_act		= KP*(cruise_speed-speed_sensor);
	int_act			= KI*((cruise_speed-speed_sensor)+pre_int);
	throttle_loc	= prop_act+int_act;
	pre_int			= (cruise_speed-speed_sensor)+pre_int;
}

void proport(float cruise_speed, float speend_sensor){
	prop_act		= KP*(cruise_speed-speed_sensor);
	int_act			= KI*(pre_int);
	throttle_loc	= prop_act+int_act;
	pre_int			= (cruise_speed-speed_sensor);
}

void sat_ctrl(boolean *saturation, float *throttle_l){
	if(throttle_loc>=THROTTLE_SAT){
		*saturation	= 1;
		*throttle_l	= THROTTLE_SAT;
	}
	else if(throttle_loc<=THROTTLE_ZERO){
		*saturation	= 1;
		*throttle_l	= THROTTLE_ZERO;
	}
	else{
		*saturation	= 0;
		*throttle_l	= throttle_loc;
	}
};
