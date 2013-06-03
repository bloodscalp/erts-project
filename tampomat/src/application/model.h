/***********************************************************************
 *  Car simulation model
 *
 *	Filename    : 	model.h
 *  Version     : 	V1.0
 *  Programmers : 	Mikael Trigo - Gregoire Hagmann
 *  				William Aebi - Christian Mueller
 *
 *  Email 		:	prenom.nom@master.hes-so.ch
 *
 ***********************************************************************/

#ifndef MODEL_H_
#define MODEL_H_

#define TENGINE			-0.04f
#define IENGINE			0.025f
#define KBRAQUE			200.0f
#define TCYCLE			0.1f
#define VEHICLDYNAMIC	2.5f
#define MASSE			1450.0f
#define TORK_MAX		400.0f

void car_simulation(float *speed_sensor, float dec_sensor, float throttle);

#endif /* MODEL_H_ */
