#include "bsp_globals.h"

// Globals for the switch
uint8_t SW0_STATE = 0;
uint8_t SW1_STATE = 0;
uint8_t SW2_STATE = 0;
uint8_t SW3_STATE = 0;
uint8_t SW4_STATE = 0;
uint8_t SW5_STATE = 0;
uint8_t SW6_STATE = 0;
uint8_t SW7_STATE = 0;

// Global for the joystick
uint8_t JOY_STATE = 0;

// Buffer for the RX char on USART1
uint8_t USART1_CNT_IN = 0;
uint8_t USART1_BUFFER_IN[CHAR_BUFFER_SIZE];

// Used for STM32 USB
uint32_t count_out = 0;
uint8_t buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];
