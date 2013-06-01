/*
 * globals.c
 *
 *  Created on: 13 mai 2013
 *      Author: William
 */

#include "globals.h"
#include "OS_CPU.h"

#define MUTEX_PRIO 4

/* Declaration of the mutexes */
OS_EVENT  	*cmd_on_mutex,
			*cmd_off_mutex,
			*cmd_acc_mutex,
			*cmd_dec_mutex,
			*cmd_set_mutex,
			*cmd_res_mutex,
			*saturation_mutex;

statusReg statusReg_i;

statusRegOn statusRegOn_i;

statusThrottleCtrl statusThrottleCtrl_i;

float 	speed_sensor,
		cruise_speed;

uint8_t throttle_g,
		acc_sensor,
		dec_sensor;

bool 	cmd_on,
		cmd_off,
		cmd_acc,
		cmd_dec,
		cmd_set,
		cmd_res,
		saturation;

bool create_getset_mutex(void){
	uint8_t cnt=0;
	cmd_on_mutex = OSSemCreate(1);	//Initialement non bloquant
	if (cmd_on_mutex == (void *)0)
		cnt+=1; //+ 1 error
	cmd_off_mutex = OSSemCreate(1);	//Initialement non bloquant
	if (cmd_off_mutex == (void *)0)
		cnt+=1; //+ 1 error
	cmd_acc_mutex = OSSemCreate(1);	//Initialement non bloquant
	if (cmd_acc_mutex == (void *)0)
		cnt+=1; //+ 1 error
	cmd_dec_mutex = OSSemCreate(1);	//Initialement non bloquant
	if (cmd_dec_mutex == (void *)0)
		cnt+=1; //+ 1 error
	cmd_set_mutex = OSSemCreate(1);	//Initialement non bloquant
	if (cmd_set_mutex == (void *)0)
		cnt+=1; //+ 1 error
	cmd_res_mutex = OSSemCreate(1);	//Initialement non bloquant
	if (cmd_res_mutex == (void *)0)
		cnt+=1; //+ 1 error
	saturation_mutex = OSSemCreate(1);	//Initialement non bloquant
	if (saturation_mutex == (void *)0)
		cnt+=1; //+ 1 error
	if (cnt!=0)
		return FALSE; //error in mutex creation
	else
		return TRUE;  //no error
}

bool destroy_getset_mutex(void){
	INT8U *perr=OS_ERR_NONE;
	uint8_t cnt=0;
	OSSemDel(cmd_on_mutex,OS_DEL_ALWAYS,perr);
	if (*perr!=OS_ERR_NONE)
		cnt+=1;
	OSSemDel(cmd_off_mutex,OS_DEL_ALWAYS,perr);
	if (*perr!=OS_ERR_NONE)
		cnt+=1;
	OSSemDel(cmd_acc_mutex,OS_DEL_ALWAYS,perr);
	if (*perr!=OS_ERR_NONE)
		cnt+=1;
	OSSemDel(cmd_dec_mutex,OS_DEL_ALWAYS,perr);
	if (*perr!=OS_ERR_NONE)
		cnt+=1;
	OSSemDel(cmd_set_mutex ,OS_DEL_ALWAYS,perr);
	if (*perr!=OS_ERR_NONE)
		cnt+=1;
	OSSemDel(cmd_res_mutex,OS_DEL_ALWAYS,perr);
	if (*perr!=OS_ERR_NONE)
		cnt+=1;
	OSSemDel(saturation_mutex,OS_DEL_ALWAYS,perr);
	if (*perr!=OS_ERR_NONE)
		cnt+=1;
	if (cnt!=0)
		return FALSE; //error in mutex deletion
	else
		return TRUE;  //no error
}

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

void set_cruise_speed(float speed)
{
	cruise_speed = speed;
}

float get_cruise_speed(void)
{
	return cruise_speed;
}


void set_saturation(bool saturation_param)
{
	saturation = saturation_param;
}

uint8_t get_saturation(void)
{
	return saturation;
}

void set_throttle(uint8_t throttle_param){
	throttle_g = throttle_param;
}
uint8_t get_throttle(void){
	return throttle_g;
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
	INT8U erreur;
	OSSemPend(cmd_on_mutex, 0, &erreur);
		cmd_on = cmd;
	OSSemPost(cmd_on_mutex);
}
bool get_cmd_on(void){
	int tmp;
	tmp = cmd_on;
	cmd_on = 0;
	return tmp;
}

void set_cmd_off(bool cmd){
	INT8U erreur;
	OSSemPend(cmd_off_mutex, 0, &erreur);
		cmd_off = cmd;
	OSSemPost(cmd_off_mutex);
}
bool get_cmd_off(void){
	int tmp;
	tmp = cmd_off;
	cmd_off = 0;
	return tmp;
}

void set_cmd_acc(bool cmd){
	INT8U erreur;
	OSSemPend(cmd_acc_mutex, 0, &erreur);
		cmd_acc = cmd;
	OSSemPost(cmd_acc_mutex);
}
bool get_cmd_acc(void){
	int tmp;
	tmp = cmd_acc;
	cmd_acc = 0;
	return tmp;
}

void set_cmd_dec(bool cmd){
	INT8U erreur;
	OSSemPend(cmd_dec_mutex, 0, &erreur);
		cmd_dec = cmd;
	OSSemPost(cmd_dec_mutex);
}
bool get_cmd_dec(void){
	int tmp;
	tmp = cmd_dec;
	cmd_dec = 0;
	return tmp;
}

void set_cmd_set(bool cmd){
	INT8U erreur;
	OSSemPend(cmd_set_mutex, 0, &erreur);
		cmd_set = cmd;
	OSSemPost(cmd_set_mutex);
}
bool get_cmd_set(void){
	int tmp;
	tmp = cmd_set;
	cmd_set = 0;
	return tmp;
}

void set_cmd_res(bool cmd){
	INT8U erreur;
	OSSemPend(cmd_res_mutex, 0, &erreur);
		cmd_res = cmd;
	OSSemPost(cmd_res_mutex);
}
bool get_cmd_res(void){
	int tmp;
	tmp = cmd_res;
	cmd_res = 0;
	return tmp;
}

