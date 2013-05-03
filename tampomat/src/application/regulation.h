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

void proport_integr(float cruise_speed, float speed_sensor);
void proport(float cruise_speed, float speend_sensor);
void sat_ctrl(bool *saturation, float *throttle_l);

#endif /* REGULATION_H_ */
