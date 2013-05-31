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

#define CRUISE_SPEED_MAX		150 // km/h
#define CRUISE_SPEED_MIN		30 	// km/h
#define CRUISE_SPEED_STEP 5

#define KP				8.113
#define KI				0.5


#include "regulation.h"
#include "stm32f10x_includes.h"
#include "globals.h"

float 	prop_act,
		int_act,
		pre_int,
		throttle_loc,
		regulation_throttle;

/* Function prototypes */
void regulation_init(void);
void regultation_fsm(void);
void check_breaks(void);
void proport_integr(float cruise_speed, float speed_sensor);
void proport(float cruise_speed, float speend_sensor);
void sat_ctrl(void);
void on_fsm(void);
void throttle_control(void);

void regulation_init()
{
	prop_act = 0.0;
	int_act = 0.0;
	pre_int = 0.0;
	throttle_loc = 0.0;
	regulation_throttle  = 0.0;

	set_statusReg(reg_init);
	set_statusRegOn(on_init);
	set_statusThrottleCtrl(throttle_init);
}

/* This FMS manages the global car speed regulation */
void regultation_fsm()
{

	switch (get_statusReg()) {
	case reg_init:
		set_cruise_speed(0);
		set_statusReg(off);
		break;


	case on:

		on_fsm();

		/* Go in interruption mode if breaks hit */
		check_breaks();

		/* Disable regulation on command detection */
		if(get_cmd_off()) {
			set_statusReg(off);
		}

		/* Go standby if pedal is pushed or car speed is out of range */
		if(get_acc_sensor() > PEDALS_MIN || get_speed_sensor() > CRUISE_SPEED_MAX || get_speed_sensor() < CRUISE_SPEED_MIN) {
			set_statusReg(standby);
		}


		/* Throttle control sub FSM */
		throttle_control();

		break;

	case off:

		regulation_throttle = 0;

		/* Enable regulation on command detection */
		if(get_cmd_on()) {
			set_statusReg(on);
			set_cruise_speed(0);
		}

		break;

	case standby:

		regulation_throttle = 0;

		/* Go in interruption mode if breaks hit */
		check_breaks();

		/* Set new cruise speed on command detection */
		if(get_cmd_set()) {
			set_cruise_speed(get_speed_sensor());
		}

		/* Go back to on if pedals are released or car speed is in range again */
		if(get_acc_sensor() <= PEDALS_MIN && get_speed_sensor() <= CRUISE_SPEED_MAX && get_speed_sensor() >= CRUISE_SPEED_MIN) {
			set_statusReg(on);
		}
		break;

	case interrupted:
		regulation_throttle = 0;

		/* Set new cruise speed on command detection */
		if(get_cmd_set()) {
			set_cruise_speed(get_speed_sensor());
			set_statusReg(on);
		}

		/* Enable regulation on command detection */
		if(get_cmd_res())
			set_statusReg(on);

		break;
	}
}


void check_breaks()
{
	if(get_dec_sensor() > PEDALS_MIN) {
		set_statusReg(interrupted);
	}
}


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
		regulation_throttle = THROTTLE_SAT;
	} else if (throttle_loc <= THROTTLE_ZERO) {
		set_saturation(TRUE);
		regulation_throttle = THROTTLE_ZERO;
	} else {
		set_saturation(FALSE);
		regulation_throttle = throttle_loc;
	}
}

void on_fsm()
{
	switch(get_statusRegOn()) {
	case on_init:
		set_statusRegOn(setSpeed);
		break;

	case setSpeed:
		/* Set new cruise speed on command detection */
		if(get_cmd_set()) {
			set_cruise_speed(get_speed_sensor());
		}

		/* Increase cruise speed by one step */
		if(get_cmd_acc()) {
			set_cruise_speed(get_cruise_speed()+CRUISE_SPEED_STEP);
		}

		/* Decrease cruise speed by one step */
		if(get_cmd_dec()) {
			set_cruise_speed(get_cruise_speed()-CRUISE_SPEED_STEP);

#if BUG_NR == 1
			regulation_throttle = 100;
			while (1){
						OSTimeDly(OS_TICKS_PER_SEC / 10);};
#endif /* BUG 1 */


		}

		/*  */
		if(get_cruise_speed() >= CRUISE_SPEED_MAX || get_cruise_speed() <= CRUISE_SPEED_MIN)
			set_statusRegOn(blocked);

		break;

	case blocked:
		if(get_cruise_speed() < CRUISE_SPEED_MAX || get_cruise_speed() < CRUISE_SPEED_MIN)
			set_statusRegOn(setSpeed);
		break;
	}
}

void throttle_control()
{
	switch (get_statusThrottleCtrl()) {
		case throttle_init:
			set_statusThrottleCtrl(proport_int);
			break;

		case proport_int:
			proport_integr(get_cruise_speed(), get_speed_sensor());

			if(get_saturation()){
				set_statusThrottleCtrl(proport_sat);
			}
			break;

		case proport_sat:
			proport(get_cruise_speed(), get_speed_sensor());

			if(!get_saturation()){
				set_statusThrottleCtrl(proport_int);
			}
			break;
	}

	sat_ctrl();
}

float get_regulation_throttle() {
	return regulation_throttle;
}

void thread_regulation(void *p_arg)
{
	(void)p_arg;
	regulation_init();

	int next_deadline = OSTimeGet()+CST_PERIOD_CAR_MODEL;
	while (1) {
		regultation_fsm();

		OSTimeDly(next_deadline-OSTimeGet());
		next_deadline += CST_PERIOD_REGULATION;
	}
}
