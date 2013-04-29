#ifndef BSP_GLOBALS_H_
#define BSP_GLOBALS_H_
#include <stdint.h>
#include "includes.h"

// Switches global
extern uint8_t SW0_STATE;
extern uint8_t SW1_STATE;
extern uint8_t SW2_STATE;
extern uint8_t SW3_STATE;
extern uint8_t SW4_STATE;
extern uint8_t SW5_STATE;
extern uint8_t SW6_STATE;
extern uint8_t SW7_STATE;

// Joystick global
extern uint8_t JOY_STATE;

// USART1 RX Globals
extern uint8_t USART1_CNT_IN;
extern uint8_t USART1_BUFFER_IN[20];

// Used for STM32 USB
extern uint32_t count_out;
extern uint8_t buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];

#endif /* GLOBALS_H_ */
