/*
*********************************************************************************************************
*
*                                         BOARD SUPPORT PACKAGE
*
*                                             ARM Cortex M3
*
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : Xavier Meyer
* TO-DO			: Create a file per functionality so it get completely enabled/disabled
* 				  with the bsp_conf.h configuration
*********************************************************************************************************
*/

#ifndef  __BSP_H__
#define  __BSP_H__

#include "os_cpu.h"

/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE        0x400
#define WRITE_READ_ADDR    0x2000

/*
*********************************************************************************************************
*                                             GLOBAL VARIABLES
*********************************************************************************************************
*/

// Direct LEDs
typedef enum
{
	LED1 = 0,
	LED2 = 1,
	LED3 = 2,
	LED4 = 3,
	LED5 = 4,
	LED6 = 5,
	LED7 = 6,
	LED8 = 7
} Led_TypeDef;

#define D_LEDn						8
#define D_LED_GPIO_PORT				GPIOG
#define D_LED_GPIO_CLK				RCC_APB2Periph_GPIOG

#define D_LED0_GPIO_PIN				GPIO_Pin_0
#define D_LED1_GPIO_PIN				GPIO_Pin_1
#define D_LED2_GPIO_PIN				GPIO_Pin_2
#define D_LED3_GPIO_PIN				GPIO_Pin_3
#define D_LED4_GPIO_PIN				GPIO_Pin_4
#define D_LED5_GPIO_PIN				GPIO_Pin_5
#define D_LED6_GPIO_PIN				GPIO_Pin_6
#define D_LED7_GPIO_PIN				GPIO_Pin_7


// SWITCH
typedef enum
{
	SW1 = 8,
	SW2 = 9,
	SW3 = 10,
	SW4 = 11,
	SW5 = 12,
	SW6 = 13,
	SW7 = 14,
	SW8 = 15
} SW_TypeDef;

#define SWn							8
#define SW_GPIO_PORT				GPIOG
#define SW_GPIO_CLK					RCC_APB2Periph_GPIOG

#define SW0_GPIO_PIN				GPIO_Pin_8
#define SW1_GPIO_PIN				GPIO_Pin_9
#define SW2_GPIO_PIN				GPIO_Pin_10
#define SW3_GPIO_PIN				GPIO_Pin_11
#define SW4_GPIO_PIN				GPIO_Pin_12
#define SW5_GPIO_PIN				GPIO_Pin_13
#define SW6_GPIO_PIN				GPIO_Pin_14
#define SW7_GPIO_PIN				GPIO_Pin_15

//I2C
#define I2C_PORT					I2C1
#define I2C_CLK						RCC_APB1Periph_I2C1
#define I2C_GPIO_PORT				GPIOB
#define I2C_GPIO_CLK				RCC_APB2Periph_GPIOB
#define I2C_OWN_ADDRESS				0x00
#define I2C_SPEED					100000
#define I2C_SCL						GPIO_Pin_6
#define I2C_SDA						GPIO_Pin_7
// ... INT
#define I2C_INT_CLK					RCC_APB2Periph_GPIOF
#define I2C_INT_PORT				GPIOF
#define I2C_INT						GPIO_Pin_10

// /!\ verifier si ca ne fonctionne pas!
#define I2C_ADDRESS					0x4E

#define I2C_INPUT_P0				0x00
#define I2C_INPUT_P1				0x01
#define I2C_OUTPUT_P0				0x02
#define I2C_OUTPUT_P1				0x03
#define I2C_POLINV_P0				0x04
#define I2C_POLINV_P1				0x05
#define I2C_CONFIG_P0				0x06
#define I2C_CONFIG_P1				0x07

// SDIO
#define SDIO_BLOCK_SIZE	            512 /* Block Size in Bytes */
#define SDIO_BUFFERW_SIZE	        (SDIO_BLOCK_SIZE >> 2)

#define SDIO_NB_BLOCK		        2  /* For Multi Blocks operation (Read/Write) */
#define SDIO_MULTIWSIZE 			((SDIO_BLOCK_SIZE * SDIO_NB_BLOCK) >> 2)

#define SDCARD_EN_PORT				GPIOE
#define SDCARD_EN_CLK				RCC_APB2Periph_GPIOE
#define SDCARD_EN_PIN				GPIO_Pin_5

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void            BSP_Init(void);

void            BSP_IntDisAll(void);
void  			BSP_IntEnAll(void);

INT32U      	BSP_CPU_ClkFreq(void);

void 			Tmr_TickISR_Handler(void);


// Direct LED commands
void 			BSP_LEDOn(Led_TypeDef Led);
void 			BSP_LEDOff(Led_TypeDef Led);
void 			BSP_LEDToggle(Led_TypeDef Led);
void 			BSP_LEDSet(Led_TypeDef Led, INT16U on);

// Switch commands
INT8U			BSP_SWValue(SW_TypeDef sw);
INT8U  			BSP_SWValues(void);

// I2C LEDs and Joystick
void			BSP_I2C_LEDS(INT8U reg);
INT8U 			BSP_I2C_JOY(void);

// SDIO
void 			BSP_SDCARD_ENABLE(void);
void 			BSP_SDCARD_DISABLE(void);
INT8U 			BSP_TEST_SDIO(void);

// ADC / Potentiometer
INT16U 			BSP_ADC_POT(void);

// RAM
INT8U 			BSP_TEST_SRAM(void);

// DAC / ADC
void 			BSP_SET_DAC2(INT16U value);
INT16U			BSP_GET_ADCe1(void);

#endif
