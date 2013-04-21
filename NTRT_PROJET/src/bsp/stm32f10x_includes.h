/***********************************************************************
 *  Include files for the STM32F10X
 *
 *
 *	Filename      : stm32f10x_includes.h
 *  Version       : V1.2
 *  Programmer(s) : Filipo Rusco / Xavier Meyer
 *
 ***********************************************************************/

// STD LIB
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "sdcard.h"
#include "misc.h"

// USB LIB
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_prop.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "hw_config.h"

//SRAM LIB
#include "fsmc_sram.h"

// DEFINE
#define STDIN_FILENO  0 // standard input file descriptor
#define STDOUT_FILENO 1 // standard output file descriptor
#define STDERR_FILENO 2 // standard error file descriptor

