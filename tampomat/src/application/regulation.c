/*
 * regulation.c
 *
 *  Created on: 3 mai 2013
 *      Author: William
 */

#define THROTTLE_SAT	45.0
#define THROTTLE_ZERO	0.0
#define THROTTLE_STEP	1

#define PEDALS_MIN 		0 //FIXME: put correct value
#define SPEED_MAX		150 // km/h
#define SPEED_MIN		30 	// km/h

#define KP				8.113
#define KI				0.5

#include "regulation.h"

static float prop_act, int_act, pre_int = 0.0, throttle_loc = 0.0;

void proport_integr(float cruise_speed, float speed_sensor)
{
	prop_act = KP * (cruise_speed - speed_sensor);
	int_act = KI * ((cruise_speed - speed_sensor) + pre_int);
	throttle_loc = prop_act + int_act;
	pre_int = (cruise_speed - speed_sensor) + pre_int;
}

void proport(float cruise_speed, float speend_sensor)
{
	prop_act = KP * (cruise_speed - speed_sensor);
	int_act = KI * (pre_int);
	throttle_loc = prop_act + int_act;
	pre_int = (cruise_speed - speed_sensor);
}

void sat_ctrl(bool *saturation, float *throttle_l)
{
	if (throttle_loc >= THROTTLE_SAT) {
		*saturation = true;
		*throttle_l = THROTTLE_SAT;
	} else if (throttle_loc <= THROTTLE_ZERO) {
		*saturation = true;
		*throttle_l = THROTTLE_ZERO;
	} else {
		*saturation = false;
		*throttle_l = throttle_loc;
	}
}


float setted_speed;

void regulation_init() {
	statusReg = reg_init;
	statusRegOn = on_init;
	statusThrottleCtrl = throttle_init;
}

void on_fsm()
{
	switch(statusRegOn) {
	case on_init:
		statusRegOn = setSpeed;
		break;

	case setSpeed:

		break;

	case blocked:

		break;
	}
}

void check_breaks() {
	if(dec_sensor > PEDALS_MIN) {
		statusReg = interrupted;
	}
}

void throttle_control() {
	bool saturation;

	switch (statusThrottleCtrl) {
	case throttle_init:
		statusThrottleCtrl = proport_int;
		break;

	case proport_int:

		proport_integr(setted_speed, speed_sensor); // FIXME: no effect

		sat_ctrl(&saturation, &throttle);
		if(saturation){
			statusThrottleCtrl = proport_sat;
		}
		break;

	case proport_sat:

		proport(setted_speed, speed_sensor); // FIXME: no effect

		sat_ctrl(&saturation, &throttle);
		if(!saturation){
			statusThrottleCtrl = proport_int;
		}
		break;
	}
}

/* This FMS manages the global car speed regulation */
void regultation_fsm()
{

	switch (statusReg) {
	case reg_init:
		setted_speed = 0;
		statusReg = off;
		break;


	case on:
		check_breaks();

		/* Adapt regulation to reach cruise speed */

		if(speed_sensor < setted_speed)
			throttle += THROTTLE_STEP;
		else if(speed_sensor > setted_speed)
			throttle -= THROTTLE_STEP;

		// ON FSM
		/* Set new cruise speed on command detection */
		if(cmd_set) {
			cmd_set = 0; // FIXME: get mutex
			setted_speed = speed_sensor;
		}

		on_fsm();

		// END ON FSM

		/* Disable regulation on command detection */
		if(cmd_off) {
			cmd_off = 0; // FIXME: get mutex
			statusReg = off;
		}

		/* Go standby if pedal is pushed or car speed is out of range */
		if(acc_sensor > PEDALS_MIN || speed_sensor > SPEED_MAX || speed_sensor < SPEED_MIN) {
			statusReg = standby;
		}

		break;

	case off:

		/* Enable regulation on command detection */
		if(cmd_on) {
			cmd_on = 0; // FIXME: get mutex
			statusReg = on;
			setted_speed = 0;
		}

		break;

	case standby:
		check_breaks();

		/* Go back to on if pedals are released or car speed is in range again */
		if(acc_sensor<=PEDALS_MIN && speed_sensor<=SPEED_MAX && speed_sensor >= SPEED_MIN) {
			statusReg = on;
		}
		break;

	case interrupted:
		/* Enable regulation on command detection */
		if(cmd_res) {
			cmd_res = 0; // FIXME: get mutex
			statusReg = on;
		}
		break;
	}
}
