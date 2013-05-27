/***********************************************************************
 *  Speed regulation system
 *
 *	Filename      : regulation.c
 *  Version       : V1.1
 *  Programmer(s) : William Aebi, Christian Muller
 *
 ***********************************************************************/

#define THROTTLE_SAT	45.0
#define THROTTLE_ZERO	0.0
#define THROTTLE_STEP	1

//#define PEDALS_MIN 		0 //FIXME: put correct value
#define SPEED_MAX		150 // km/h
#define SPEED_MIN		30 	// km/h

#define KP				8.113
#define KI				0.5

#include "regulation.h"
#include "stm32f10x_includes.h"

static float prop_act, int_act, pre_int = 0.0, throttle_loc = 0.0;

void proport_integr(float cruise_speed, float speed_sensor)
{
	prop_act = KP * (cruise_speed - speed_sensor);
	int_act = KI * ((cruise_speed - speed_sensor) + pre_int);
	throttle_loc = prop_act + int_act;
	pre_int = (cruise_speed - speed_sensor) + pre_int;
}

void proport(float cruise_speed, float speed_sensor)
{
	prop_act = KP * (cruise_speed - speed_sensor);
	int_act = KI * (pre_int);
	throttle_loc = prop_act + int_act;
	pre_int = (cruise_speed - speed_sensor);
}

void sat_ctrl()
{
	if (throttle_loc >= THROTTLE_SAT) {
		set_saturation(TRUE);
		set_throttle(THROTTLE_SAT);
	} else if (throttle_loc <= THROTTLE_ZERO) {
		set_saturation(TRUE);
		set_throttle(THROTTLE_ZERO);
	} else {
		set_saturation(FALSE);
		set_throttle(throttle_loc);
	}
}


float setted_speed;

void regulation_init()
{
	set_statusReg(reg_init);
	set_statusRegOn(on_init);
	set_statusThrottleCtrl(throttle_init);
}

void on_fsm()
{
	switch(get_statusRegOn()) {
	case on_init:
		set_statusRegOn(setSpeed);
		break;

	case setSpeed:

		break;

	case blocked:

		break;
	}
}

void check_breaks()
{
	if(get_dec_sensor() > PEDALS_MIN) {
		set_statusReg(interrupted);
	}
}

void throttle_control()
{
	bool saturation;

	switch (get_statusThrottleCtrl()) {
	case throttle_init:
		set_statusThrottleCtrl(proport_int);
		break;

	case proport_int:

		proport_integr(get_cruise_speed(), get_speed_sensor());

		sat_ctrl();
		if(get_saturation()){
			set_statusThrottleCtrl(proport_sat);
		}
		break;

	case proport_sat:

		proport(get_cruise_speed(), get_speed_sensor());

		sat_ctrl();
		if(!get_saturation()){
			set_statusThrottleCtrl(proport_int);
		}
		break;
	}
}

/* This FMS manages the global car speed regulation */
void regultation_fsm()
{

	switch (get_statusReg()) {
	case reg_init:
		setted_speed = 0;
		set_statusReg(off);
		break;


	case on:
		check_breaks();

		/* Adapt regulation to reach cruise speed */

		if(get_speed_sensor() < get_cruise_speed())
			set_throttle(get_throttle()+THROTTLE_STEP);
		else if(get_speed_sensor() > get_cruise_speed())
			set_throttle(get_throttle()-THROTTLE_STEP);

		// ON FSM
		/* Set new cruise speed on command detection */
		if(get_cmd_set()) {
			set_cruise_speed(get_speed_sensor());
		}

		on_fsm();

		// END ON FSM

		/* Disable regulation on command detection */
		if(get_cmd_off()) {
			set_statusReg(off);
		}

		/* Go standby if pedal is pushed or car speed is out of range */
		if(get_acc_sensor() > PEDALS_MIN || get_speed_sensor() > SPEED_MAX || get_speed_sensor() < SPEED_MIN) {
			set_statusReg(standby);
		}

		break;

	case off:

		/* Enable regulation on command detection */
		if(get_cmd_on()) {
			set_statusReg(on);
			setted_speed = 0;
		}

		break;

	case standby:
		check_breaks();

		/* Go back to on if pedals are released or car speed is in range again */
		if(get_acc_sensor() <= PEDALS_MIN && get_speed_sensor() <= SPEED_MAX && get_speed_sensor() >= SPEED_MIN) {
			set_statusReg(on);
		}
		break;

	case interrupted:
		/* Enable regulation on command detection */
		if(get_cmd_res())
			set_statusReg(on);

		break;
	}
}
