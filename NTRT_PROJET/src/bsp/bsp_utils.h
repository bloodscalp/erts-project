/*
*********************************************************************************************************
*
*                                      Utilities functions for BSP
*
*                                             ARM Cortex M3
*
* Filename      : bsp_utils.h
* Version       : V1.00
* Programmer(s) : Xavier Meyer
*********************************************************************************************************
*/

#ifndef BSP_UTILS_H_
#define BSP_UTILS_H_

#include "stm32f10x_includes.h"

/****************************************************************
 *                    Functions prototype						*
 ***************************************************************/
void 		Fill_Buffer		(uint32_t *pBuffer,
							 uint16_t BufferLenght,
							 uint32_t Offset);

uint8_t 	eBuffercmp		(uint32_t* pBuffer,
							 uint16_t BufferLength);

uint8_t 	Buffercmp		(uint32_t* pBuffer1,
							 uint32_t* pBuffer2,
							 uint16_t BufferLength);

void 		Fill_Buffer16 	(uint16_t *pBuffer,
							 uint16_t BufferLenght,
							 uint32_t Offset);

uint8_t 	Buffercmp16		(uint16_t* pBuffer1,
							 uint16_t* pBuffer2,
							 uint16_t BufferLength);


#endif /* BSP_UTILS_H_ */
