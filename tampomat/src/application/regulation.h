/***********************************************************************
 *  Spee regulation system
 *
 *	Filename    : 	regulation.h
 *  Version     : 	V1.0
 *  Programmers : 	Mikael Trigo - Gregoire Hagmann
 *  				William Aebi - Christian Mueller
 *
 *  Email 		:	prenom.nom@master.hes-so.ch
 *
 ***********************************************************************/
#ifndef REGULATION_H_
#define REGULATION_H_

void thread_regulation (void *p_arg);

float get_regulation_throttle(void);

#endif /* REGULATION_H_ */
