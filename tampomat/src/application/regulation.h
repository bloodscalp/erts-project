/*
 * regulation.h
 *
 *  Created on: 3 mai 2013
 *      Author: William
 */

#ifndef REGULATION_H_
#define REGULATION_H_

#define THROTTLE_SAT	45.0
#define THROTTLE_ZERO	0.0
#define KP				8.113
#define KI				0.5

#include <stdint.h>

enum statusReg_t {
	reg_init,
	on,
	off,
	standby,
	interrupted
} statusReg;

enum statusRegOn_t {
	on_init,
	blocked,
	setSpeed
} statusRegOn;

enum statusThrottleCtrl_t {
	throttle_init,
	proport_int,
	proport_sat
} statusThrottleCtrl;

typedef enum bool_t {
	false = 0,
	true = 1
} bool;

float speed_sensor;
uint8_t throttle,
		acc_sensor,
		dec_sensor;

bool 	cmd_on,
		cmd_off,
		cmd_acc,
		cmd_dec,
		cmd_set,
		cmd_res;


/* Function prototypes */
void proport_integr(float cruise_speed, float speed_sensor);
void proport(float cruise_speed, float speend_sensor);
void sat_ctrl(bool *saturation, float *throttle_l);

#endif /* REGULATION_H_ */
