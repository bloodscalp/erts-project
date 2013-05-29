/***********************************************************************
 *  Speed regulation system
 *
 *	Filename      : regulation.h
 *  Version       : V1.1
 *  Programmer(s) : William Aebi, Christian Muller
 *
 ***********************************************************************/

#ifndef REGULATION_H_
#define REGULATION_H_

void thread_regulation (void *p_arg);

float get_regulation_throttle(void);

#endif /* REGULATION_H_ */
