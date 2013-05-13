/*
 * getset.h
 *
 *  Created on: 13 mai 2013
 *      Author: William
 */

#ifndef GETSET_H_
#define GETSET_H_

#include <stdint.h>

typedef enum statusReg_t {
	reg_init,
	on,
	off,
	standby,
	interrupted
} statusReg;

typedef enum statusRegOn_t {
	on_init,
	blocked,
	setSpeed
} statusRegOn;

typedef enum statusThrottleCtrl_t {
	throttle_init,
	proport_int,
	proport_sat
} statusThrottleCtrl;

typedef enum bool_t {
	false = 0,
	true = 1
} bool;

void set_statusReg(statusReg status);
statusReg get_statusReg(void);

void set_statusRegOn(statusRegOn status);
statusRegOn get_statusRegOn(void);

void set_statusThrottleCtrl(statusThrottleCtrl status);
statusThrottleCtrl get_statusThrottleCtrl(void);

void set_speed_sensor(float speed);
float get_speed_sensor(void);

void set_throttle(uint8_t throttle);
uint8_t get_throttle(void);

void set_acc_sensor(uint8_t acc);
uint8_t get_acc_sensor(void);

void set_dec_sensor(uint8_t dec);
uint8_t get_dec_sensor(void);

void set_cmd_on(bool cmd);
bool get_cmd_on(void);

void set_cmd_off(bool cmd);
bool get_cmd_off(void);

void set_cmd_acc(bool cmd);
bool get_cmd_acc(void);

void set_cmd_dec(bool cmd);
bool get_cmd_dec(void);

void set_cmd_set(bool cmd);
bool get_cmd_set(void);

void set_cmd_res(bool cmd);
bool get_cmd_res(void);


#endif /* GETSET_H_ */
