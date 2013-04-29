/*
 * Vectors core Cortex-M3
 *
 * Cortex-M3 Specific Interrupt Section
 * Cortex-M3 interrupt vector table and reset (see Cortex-M3 Technical Reference Manual
 * Sections 5.2 and 5.9.1, pages 5-3 and 5-19)
 *
 * MODIFIED FOR uCos-II (Search for entry postfixed with "uCos-II" for more information)
 *
 */

/******************************************************************************
*
* The minimal vector table for a Cortex M3.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
*
* Reference manual: document 13902.pdf pag 123
*
* 60 maskable interrupt channels (not including the 16 interrupt lines of Cortex™-M3)
* 16 programmable priority levels (4 bits of interrupt priority are used)
******************************************************************************/

#include "app_int.h"
#include "bsp.h"


/* Vector table: ....
 * However rather than start at zero the vector table starts at address 0x00000004,
 * the first four bytes are used to store the starting address of the stack pointer.
*/

__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
	(void (*)(void))( (&_main_stack) ),
  Reset_Handler,			// Address: 0x0000_0004  // The reset handler
  NMIException,				// Address: 0x0000_0008
  HardFaultException,		// Address: 0x0000_000C
  MemManageException,		// Address: 0x0000_0010
  BusFaultException,		// Address: 0x0000_0014
  UsageFaultException,		// Address: 0x0000_0018
  0,						// Address: 0x0000_001C  /* Reserved */
  0,						// Address: 0x0000_0020  /* Reserved */
  0,						// Address: 0x0000_0024  /* Reserved */
  0,						// Address: 0x0000_0028  /* Reserved */
  SVCHandler,				// Address: 0x0000_002C
  DebugMonitor,				// Address: 0x0000_0030
  0,						// Address: 0x0000_0034  /* Reserved */
  OS_CPU_PendSVHandler,		// Address: 0x0000_0038  /* uCos-II  */
  Tmr_TickISR_Handler,		// Address: 0x0000_003C  /* uCos-II  */
  /**/
  WWDG_IRQHandler,			// Address: 0x0000_0040  /* Position 0*/
  PVD_IRQHandler,			// Address: 0x0000_0044
  TAMPER_IRQHandler,		// Address: 0x0000_0048
  RTC_IRQHandler,			// Address: 0x0000_004C
  FLASH_IRQHandler,			// Address: 0x0000_0050
  RCC_IRQHandler,			// Address: 0x0000_0054
  EXTI0_IRQHandler,			// Address: 0x0000_0058
  EXTI1_IRQHandler,			// Address: 0x0000_005C
  EXTI2_IRQHandler,			// Address: 0x0000_0060
  EXTI3_IRQHandler,			// Address: 0x0000_0064
  EXTI4_IRQHandler,			// Address: 0x0000_0068
  DMAChannel1_IRQHandler,	// Address: 0x0000_006C
  DMAChannel2_IRQHandler,	// Address: 0x0000_0070
  DMAChannel3_IRQHandler,	// Address: 0x0000_0074
  DMAChannel4_IRQHandler,	// Address: 0x0000_0078
  DMAChannel5_IRQHandler,	// Address: 0x0000_007C
  DMAChannel6_IRQHandler,	// Address: 0x0000_0080
  DMAChannel7_IRQHandler,	// Address: 0x0000_0084
  ADC1_2_IRQHandler,		// Address: 0x0000_0088
  USB_HP_CAN_TX_IRQHandler,	// Address: 0x0000_008C
  USB_LP_CAN_RX0_IRQHandler,	// Address: 0x0000_0090
  CAN_RX1_IRQHandler,		// Address: 0x0000_0094
  CAN_SCE_IRQHandler,		// Address: 0x0000_0098
  EXTI9_5_IRQHandler,		// Address: 0x0000_009C
  TIM1_BRK_IRQHandler,		// Address: 0x0000_00A0
  TIM1_UP_IRQHandler,		// Address: 0x0000_00A4
  TIM1_TRG_COM_IRQHandler,	// Address: 0x0000_00A8
  TIM1_CC_IRQHandler,		// Address: 0x0000_00AC
  TIM2_IRQHandler,			// Address: 0x0000_00B0
  TIM3_IRQHandler,			// Address: 0x0000_00B4
  TIM4_IRQHandler,			// Address: 0x0000_00B8
  I2C1_EV_IRQHandler,		// Address: 0x0000_00BC
  I2C1_ER_IRQHandler,		// Address: 0x0000_00C0
  I2C2_EV_IRQHandler,		// Address: 0x0000_00C4
  I2C2_ER_IRQHandler,		// Address: 0x0000_00C8
  SPI1_IRQHandler,			// Address: 0x0000_00CC
  SPI2_IRQHandler,			// Address: 0x0000_00D0
  USART1_IRQHandler,		// Address: 0x0000_00D4
  USART2_IRQHandler,		// Address: 0x0000_00D8
  USART3_IRQHandler,		// Address: 0x0000_00DC
  EXTI15_10_IRQHandler,		// Address: 0x0000_00E0
  RTCAlarm_IRQHandler,		// Address: 0x0000_00E4
  USBWakeUp_IRQHandler,		// Address: 0x0000_00E8
  TIM8_BRK_IRQHandler,		// Address: 0x0000_00EC
  TIM8_UP_IRQHandler,		// Address: 0x0000_00F0
  TIM8_TRG_COM_IRQHandler,	// Address: 0x0000_00F4
  TIM8_CC_IRQHandler,		// Address: 0x0000_00F8
  ADC3_IRQHandler,			// Address: 0x0000_00FC
  FSMC_IRQHandler,			// Address: 0x0000_0100
  SDIO_IRQHandler,			// Address: 0x0000_0104
  TIM5_IRQHandler,			// Address: 0x0000_0108
  SPI3_IRQHandler,			// Address: 0x0000_010C
  USART4_IRQHandler,		// Address: 0x0000_0110
  USART5_IRQHandler,		// Address: 0x0000_0114
  TIM6_IRQHandler,			// Address: 0x0000_0118
  TIM7_IRQHandler,			// Address: 0x0000_011C
  DMA2Channel1_IRQHandler,	// Address: 0x0000_0120
  DMA2Channel2_IRQHandler,	// Address: 0x0000_0124
  DMA2Channel3_IRQHandler,	// Address: 0x0000_0128
  DMA2Channel4_5_IRQHandler,	// Address: 0x0000_012C
};
