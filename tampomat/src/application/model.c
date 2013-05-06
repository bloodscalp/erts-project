/*
 * model.c
 *
 *  Created on: 3 mai 2013
 *      Author: William
 */

/*#define TENGINE			-0.04f
#define IENGINE			0.025f
#define KBRAQUE			200.0f
#define TCYCLE			0.1f
#define VEHICLDYNAMIC	2.5f
#define MASSE			1450.0f
#define TORK_MAX		400.0f*/

#include "model.h"
void simulation(float *speed_sensor, float dec_sensor, float throttle){
	static float pre_speed = 0.0f;
	static float torq;
	float a, b, c, d, e, f, g, h, x;
	torq = throttle;
	if(torq<0.0f)
		torq = 100.0f;
	else if(torq>100.0f)
		torq = 100.0f;
	torq = torq*TENGINE;
	torq = torq*(torq/2.0f*(torq/3.0f*(torq/4.0f*(torq/5.0f+1)+1)+1)+1);
	torq = torq*torq*(TORK_MAX/IENGINE);

	if(torq<=1.0)
		a = 150.0f;
	else
		a = 0.01;

	b =	dec_sensor*KBRAQUE;
	c =	pre_speed*pre_speed*0.3f;
	d =	pre_speed*2.5f;
	e =	a+b+c+d;
	f = -e;

	if(pre_speed>0.1)
		g = e;
	else
		g = 0.0f;
	if(pre_speed<-0.1f)
		h = f;
	else
		h = g;

	x = ((torq-h)*TCYCLE*VEHICLDYNAMIC)/MASSE;
	pre_speed		= x+pre_speed;
	*speed_sensor	= pre_speed;
}
