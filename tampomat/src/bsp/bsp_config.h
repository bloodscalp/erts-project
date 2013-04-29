/*
 * bsp_config.h
 *
 *  Created on: 8 févr. 2010
 *      Author: meyerx
 */

#ifndef BSP_CONFIG_H_
#define BSP_CONFIG_H_



#define ON 	1
#define OFF 0

// Enable the SRAM but disable the leds
#define __LED_ENABLE 		ON
#define __I2C_ENABLE 		ON
#define __SWITCH_ENABLE 	ON
#define __USART1_ENABLE     ON
#define __POT_ENABLE 		ON
#define __USB_ENABLE 		ON
#define __SDCARD_ENABLE 	ON
#define __USART3_ENABLE     OFF
#define __DAC1_ENABLE 		OFF
#define __DAC2_ENABLE 		OFF
#define __ADCe1_ENABLE 		OFF
#define __SRAM_ENABLE 		OFF
#define __RTC_ENABLE 		OFF

//Constant define
// Define the size of the USART1 RX Buffer
#define CHAR_BUFFER_SIZE 20

// Define the size of the USB RX Buffer
#define VIRTUAL_COM_PORT_DATA_SIZE 64


#endif /* BSP_CONFIG_H_ */
