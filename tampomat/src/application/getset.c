/*
 * getset.c
 *
 *  Created on: 13 mai 2013
 *      Author: William
 */

#include "getset.h"

statusReg statusReg_i;

statusRegOn statusRegOn_i;

statusThrottleCtrl statusThrottleCtrl_i;

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

void set_statusReg(statusReg status){
	statusReg_i = status;
}
statusReg get_statusReg(void){
	return statusReg_i;
}

void set_statusRegOn(statusRegOn status){
	statusRegOn_i = status;
}
statusRegOn get_statusRegOn(void){
	return statusRegOn_i;
}

void set_statusThrottleCtrl(statusThrottleCtrl status){
	statusThrottleCtrl_i = status;
}
statusThrottleCtrl get_statusThrottleCtrl(void){
	return statusThrottleCtrl_i;
}

void set_speed_sensor(float speed){
	speed_sensor = speed;
}
float get_speed_sensor(void){
	return speed_sensor;
}

void set_throttle(uint8_t throttle){
	throttle = throttle;
}
uint8_t get_throttle(void){
	return throttle;
}

void set_acc_sensor(uint8_t acc){
	acc_sensor = acc;
}
uint8_t get_acc_sensor(void){
	return acc_sensor;
}

void set_dec_sensor(uint8_t dec){
	dec_sensor = dec;
}
uint8_t get_dec_sensor(void){
	return dec_sensor;
}

void set_cmd_on(bool cmd){
	cmd_on = cmd;
}
bool get_cmd_on(void){
	return cmd_on;
}

void set_cmd_off(bool cmd){
	cmd_off = cmd;
}
bool get_cmd_off(void){
	return cmd_off;
}

void set_cmd_acc(bool cmd){
	cmd_acc = cmd;
}
bool get_cmd_acc(void){
	return cmd_acc;
}

void set_cmd_dec(bool cmd){
	cmd_dec = cmd;
}
bool get_cmd_dec(void){
	return cmd_dec;
}

void set_cmd_set(bool cmd){
	cmd_set = cmd;
}
bool get_cmd_set(void){
	return cmd_set;
}

void set_cmd_res(bool cmd){
	cmd_res = cmd;
}
bool get_cmd_res(void){
	return cmd_res;
}

