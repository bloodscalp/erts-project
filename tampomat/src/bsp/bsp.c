/*
*********************************************************************************************************
*
*                                         BOARD SUPPORT PACKAGE
*
*                                             ARM Cortex M3
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : Xavier Meyer
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               INCLUDES
*********************************************************************************************************
*/

#include "includes.h"
#include "stm32f10x_includes.h"
#include "system_stm32f10x.c"


/*
*********************************************************************************************************
*                                          LOCAL VARIABLES
*********************************************************************************************************
*/


// Direct LED
GPIO_TypeDef* 	D_LED_PORT 			= D_LED_GPIO_PORT;


const uint16_t 	D_LED_PIN[D_LEDn] 	= {D_LED0_GPIO_PIN, D_LED1_GPIO_PIN, D_LED2_GPIO_PIN, D_LED3_GPIO_PIN,
								       D_LED4_GPIO_PIN, D_LED5_GPIO_PIN, D_LED6_GPIO_PIN, D_LED7_GPIO_PIN};

const uint32_t 	D_LED_CLK			= D_LED_GPIO_CLK;


// SWITCH
GPIO_TypeDef*	SWITCH_PORT			= SW_GPIO_PORT;


const uint32_t 	SWITCH_CLK			= SW_GPIO_CLK;

const uint16_t 	SWITCH_PIN[SWn]		= {SW0_GPIO_PIN, SW1_GPIO_PIN, SW2_GPIO_PIN, SW3_GPIO_PIN,
									   SW4_GPIO_PIN, SW5_GPIO_PIN, SW6_GPIO_PIN, SW7_GPIO_PIN};

// SDIO
SD_CardInfo SDCardInfo;
uint32_t Buffer_Block_Tx[SDIO_BUFFERW_SIZE];
uint32_t Buffer_Block_Rx[SDIO_BUFFERW_SIZE];
uint32_t Buffer_MultiBlock_Tx[SDIO_MULTIWSIZE];
uint32_t Buffer_MultiBlock_Rx[SDIO_MULTIWSIZE];
//volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;

/* Private function prototypes -----------------------------------------------*/
// Board configuration
// RCC
#if __RTC_ENABLE == ON
static  void		RTC_Configuration(void);
#else
static	void		RCC_Configuration(void);
#endif

#if __USART1_ENABLE == ON
static	void 		USART1_NVIC_Configuration(void);
static	void 		USART1_GPIO_Configuration(void);
static	void 		USART1_Configuration(void);
#endif

#if __USART3_ENABLE == ON
static	void 		USART3_NVIC_Configuration(void);
static	void 		USART3_GPIO_Configuration(void);
static	void 		USART3_Configuration(void);
#endif

#if __SDCARD_ENABLE == ON
static	void 		SDIO_NVIC_Configuration(void);
static 	void 		SDCARD_Configuration(void);
#endif

#if __LED_ENABLE == ON
static	void		LED_Configuration(void);
#endif

#if __SWITCH_ENABLE == ON
static	void 		SWITCH_Configuration(void);
#endif

#if __I2C_ENABLE == ON
static  void 		I2C_Local_Config(void);
static  void 		I2C_INT_Config(void);
static	void 		I2C_Configuration(void);
#endif

#if __POT_ENABLE == ON
static	void		ADC_POT_Configuration(void);
#endif


#if __USB_ENABLE == ON
static	void 		USB_Configuration(void);
#endif

#if __DAC1_ENABLE == ON
static 	void 		TIM2_Trig_Configuration(void);
static 	void 		DAC1_Configuration(void);
#endif

#if __DAC2_ENABLE == ON
static 	void 		DAC2_Configuration(void);
#endif

#if __SRAM_ENABLE == ON
static	void 		SRAM_Configuration(void);
#endif

#if __ADCe1_ENABLE == ON
static	void		ADCe1_Configuration(void);
#endif


/*
*********************************************************************************************************
*                                          LOCAL FUNCTION
*********************************************************************************************************
*/

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __RTC_ENABLE == OFF
static void RCC_Configuration(void)
{
  /*---------------------------------*/
  /*  /!\  PLL2 not configured  /!\  */
  /*---------------------------------*/
  ErrorStatus HSEStartUpStatus;
  // RCC system reset(for debug purpose)
  RCC_DeInit();
  // Enable HSE
  RCC_HSEConfig(RCC_HSE_ON);
  // Wait till HSE is ready
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    // Enable Prefetch Buffer
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    // Flash 2 wait state
    FLASH_SetLatency(FLASH_Latency_2);
    // HCLK = SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    // PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);
    // PCLK1 = HCLK/2
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    // PLLCLK = 8MHz * 9 = 72 MHz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    // Enable PLL
    RCC_PLLCmd(ENABLE);
    // Wait till PLL is ready
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {;}
    // Select PLL as system clock source
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    // Wait till PLL is used as system clock source
    while(RCC_GetSYSCLKSource() != 0x08)  ///????
    {;}
  }

    /* Enable peripheral clocks (FOR USART1)---------------------------------*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* Enable GPIOA / GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}
#endif

/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Try to launch the 32kHz quartz
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __RTC_ENABLE == ON
static void RTC_Configuration(void)
{
	  SystemInit();
	  /* Enable PWR and BKP clocks */
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	    /* Allow access to BKP Domain */
	    PWR_BackupAccessCmd(ENABLE);

	    /* Reset Backup Domain */
	    BKP_DeInit();

	    /* Enable LSE */
	    RCC_LSEConfig(RCC_LSE_ON);
	    /* Wait till LSE is ready */
	    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	    {}
}
#endif


/*******************************************************************************
* Function Name  : USART1_NVIC_Configuration
* Description    : Configures the NVIC (enables the USART1 Interrupt.)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __USART1_ENABLE == ON
static void USART1_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* USART */
  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
#endif

/*******************************************************************************
* Function Name  : USART3_NVIC_Configuration
* Description    : Configures the NVIC (enables the USART3 Interrupt.)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __USART3_ENABLE == ON
static void USART3_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* USART */
  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


}
#endif
/*******************************************************************************
* Function Name  : SDIO_NVIC_Configuration
* Description    : Configures the NVIC (enables the SDIO Interrupt.)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __SDCARD_ENABLE == ON
static void SDIO_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* USART */
  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
#endif

/*******************************************************************************
* Function Name  : USART1_GPIO_Configuration (FTDI)
* Description    : Configure the USART1 Pins
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __USART1_ENABLE == ON
static void USART1_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// USART1
	// Configure USART1 Tx (PA.09) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// Configure USART1 Rx (PA.10) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
#endif
/*******************************************************************************
* Function Name  : USART1_Configuration (FTDI)
* Description    : Configure the USART1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __USART1_ENABLE == ON
static void USART1_Configuration(void)
{

	// Periph CLK Init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |
						   RCC_APB2Periph_GPIOA, ENABLE);

	// GPIO Pins for UART1
	USART1_GPIO_Configuration();
	// NVIC for UART1
	USART1_NVIC_Configuration();

	// --------------- USART1 configuration ---------------
	// USART1 configured as follow:
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// Configure the USART1
	USART_Init(USART1, &USART_InitStructure);
	// Enable the USART Transmoit interrupt: this interrupt is generated when the
	// USART1 transmit data register is empty
	// USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	// Enable the USART Receive interrupt: this interrupt is generated when the
	// USART1 receive data register is not empty
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// Enable USART1
	USART_Cmd(USART1, ENABLE);

}
#endif

/*******************************************************************************
* Function Name  : USART3_GPIO_Configuration (Trypano)
* Description    : Configure the USART3 Pins
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __USART3_ENABLE == ON
static void USART3_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// USART3
	// Configure USART3 Tx (PB.10) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Configure USART3 Rx (PB.11) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
#endif

/*******************************************************************************
* Function Name  : USART3_Configuration (Trypano)
* Description    : Configure the USART3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __USART3_ENABLE == ON
static void USART3_Configuration(void)
{

	// Periph CLK Init
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// GPIO Pins for UART3
	USART3_GPIO_Configuration();
	// NVIC for UART3
	USART3_NVIC_Configuration();

	// --------------- USART3 configuration ---------------
	// USART1 configured as follow:
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// Configure the USART3
	USART_Init(USART3, &USART_InitStructure);
	// Enable the USART Transmit interrupt: this interrupt is generated when the
	// USART1 transmit data register is empty
	// USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	// Enable the USART Receive interrupt: this interrupt is generated when the
	// USART1 receive data register is not empty
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	// Enable USART1
	USART_Cmd(USART3, ENABLE);

}
#endif

/*******************************************************************************
* Function Name  : Direct_LED_Config
* Description    : Configure the Direct LED
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __LED_ENABLE == ON
static void LED_Configuration(void)
{
	// Periph CLK init
    RCC_APB2PeriphClockCmd(D_LED_CLK, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// Init the 8 direct led
	short iLed=0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	for(iLed=0; iLed<D_LEDn; iLed++){
		GPIO_InitStructure.GPIO_Pin = D_LED_PIN[iLed];
		GPIO_Init(D_LED_PORT, &GPIO_InitStructure);
	}
}
#endif

/*******************************************************************************
* Function Name  : SWITCH_Configuration
* Description    : Configure the Switch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __SWITCH_ENABLE == ON
static void SWITCH_Configuration(void)
{
	// Periph CLK init
    RCC_APB2PeriphClockCmd(SWITCH_CLK, ENABLE); //| RCC_APB2Periph_AFIO

	GPIO_InitTypeDef GPIO_InitStructure;

	// Mode for GPIO SWITCH pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	// Init the 8 SWITCH
	short iSW=0;
	for(iSW=0; iSW<SWn; iSW++){

		// GPIO Pin specification and init
		GPIO_InitStructure.GPIO_Pin = SWITCH_PIN[iSW];
		GPIO_Init(SWITCH_PORT, &GPIO_InitStructure);

	}
}
#endif

/*******************************************************************************
* Function Name  : I2C_Local_Config
* Description    : Configure the register the I2C module
* Input          : None
* Return         : None
* Misc.			 : Frequently used to reset the state of the I2C
*******************************************************************************/
#if __I2C_ENABLE == ON
static void I2C_Local_Config(void)
{

	I2C_InitTypeDef  I2C_InitStructure;

	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

	/* I2C Peripheral Enable */
	I2C_Cmd(I2C_PORT, ENABLE);
	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C_PORT, &I2C_InitStructure);

}
#endif

/*******************************************************************************
* Function Name  : I2C_INT_Config
* Description    : Configure the interruption for I2C
* Input          : None
* Return         : None
*******************************************************************************/
#if __I2C_ENABLE == ON
static void I2C_INT_Config(void)
{
	// Periph CLK init
	RCC_APB2PeriphClockCmd(I2C_INT_CLK, ENABLE);

	// Pin configuration for nINT_Joy
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = I2C_INT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(I2C_INT_PORT, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource10);

	// EXTI configuration
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);

	// NVIC configuration
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
#endif

/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : Configure the I2C module
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if __I2C_ENABLE == ON
static void I2C_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//I2C_InitTypeDef  I2C_InitStructure;

	// Periph CLK init
	RCC_APB1PeriphClockCmd(I2C_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(I2C_GPIO_CLK, ENABLE);

	// Pin configuration for SCL / SDA
	GPIO_InitStructure.GPIO_Pin =  I2C_SCL | I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);

	// Configure nINT_Joy
	I2C_INT_Config();
	// Configure the register of the I2C module
	I2C_Local_Config();

	// Configure the first register as an output port
	I2C_GenerateSTART(I2C_PORT, ENABLE);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C_PORT, I2C_ADDRESS, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C_PORT, I2C_CONFIG_P0);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C_PORT, 0x00);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2C_PORT, ENABLE);

	// Configure the second register as an input port
	I2C_GenerateSTART(I2C_PORT, ENABLE);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C_PORT, I2C_ADDRESS, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C_PORT, I2C_CONFIG_P1);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C_PORT, 0xFF);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2C_PORT, ENABLE);
}
#endif

/*******************************************************************************
* Function Name  : ADC/Potentiometer configuration
* Description    : Configure the ADC to convert the Potentiometer value
* Input          : None
* Return         : None
*******************************************************************************/
#if __POT_ENABLE == ON
static void ADC_POT_Configuration(void)
{
	// Periph clock config.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |
						   RCC_APB2Periph_GPIOA,
						   ENABLE);

	// GPIO pin config.
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure PC.01 (ADC Channel0) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1; // (V_Pot => PCA01)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//Configuration
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

    // ADC1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);

	// Enable ADC1 reset calibration register
	ADC_ResetCalibration(ADC1);
	// Check the end of ADC1 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC1));

	// Start ADC1 calibration
	ADC_StartCalibration(ADC1);
	// Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC1));
}
#endif

/*******************************************************************************
* Function Name  : USB configuration
* Description    : Init of the USB using the STM32 USB lib
* Input          : None
* Return         : None
*******************************************************************************/
#if __USB_ENABLE == ON
static void USB_Configuration(void)
{
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();
}
#endif

/*******************************************************************************
* Function Name  : Timer2 trigger configuration
* Description    : Used for DAC signal generation
* Input          : None
* Return         : None
*******************************************************************************/
#if __DAC1_ENABLE == ON
static void TIM2_Trig_Configuration(void)
{

	// Timer 2 init
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xF;
	TIM_TimeBaseStructure.TIM_Prescaler = 0xF;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	// TIM2 TRGO selection
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	// TIM2 enable counter
	TIM_Cmd(TIM2, ENABLE);
}
#endif

/*******************************************************************************
* Function Name  : DAC Configuration
* Description    : Init the Digital/Analogic converter to send a signal
* 				   It sends a triangle signal continuously on the DAC0 pin
* Input          : None
* Return         : None
*******************************************************************************/
#if __DAC1_ENABLE == ON
static void DAC1_Configuration(void)
{

	// Timer2 trig configuration as Trigger
	TIM2_Trig_Configuration();

    /* DAC Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	// GPIO : PIN Init
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4; // (DAC0 => PA04)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// DAC channel1 Configuration
	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

	// Init channel 1
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	// Enable channel 1
	DAC_Cmd(DAC_Channel_1, ENABLE);

}
#endif

/*******************************************************************************
* Function Name  : DAC2 Configuration
* Description    : Init the Digital/Analogic converter to send a signal on
* 				   the DAC1 pin. The value are set by calling BSP_SET_DAC2.
* Input          : None
* Return         : None
*******************************************************************************/
#if __DAC2_ENABLE == ON
static void DAC2_Configuration(void)
{
    /* DAC Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	// GPIO : PIN Init
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5; // (DAC1 => PA05)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// DAC channel2 Configuration
	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	// Init channel 2
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	// Enable channel 2
	DAC_Cmd(DAC_Channel_2, ENABLE);

}
#endif

/*******************************************************************************
* Function Name  : ADC1 Configuration
* Description    : Init the ADC to convert the value of ADCe1
* Input          : None
* Return         : None
*******************************************************************************/
#if __ADCe1_ENABLE == ON
static void ADCe1_Configuration(void)
{

	// Periph clock config.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 |
						   RCC_APB2Periph_GPIOC,
						   ENABLE);

	/* Configure PC.05 (ADC Channel14) as analog input -------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//Configuration
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);

	// ADC1
	ADC_RegularChannelConfig(ADC2, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);

	// Enable ADC1
	ADC_Cmd(ADC2, ENABLE);

	// Enable ADC1 reset calibration register
	ADC_ResetCalibration(ADC2);
	// Check the end of ADC1 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC2));

	// Start ADC1 calibration
	ADC_StartCalibration(ADC2);
	// Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC2));
}
#endif


/*******************************************************************************
* Function Name  : SRAM Configuration
* Description    : Configure the FSMC module for the SRAM
* Input          : None
* Return         : None
*******************************************************************************/
#if __SRAM_ENABLE == ON
static void SRAM_Configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	FSMC_SRAM_Init();
}
#endif

/*******************************************************************************
* Function Name  : SDCARD_Configuration
* Description    : Configure the SDCard eanble pin and the NVIC
* Input          : None
* Return         : None
*******************************************************************************/

#if __SDCARD_ENABLE == ON
static void SDCARD_Configuration(void)
{
    /* SDCARD Periph clock enable */
    RCC_APB2PeriphClockCmd(SDCARD_EN_CLK, ENABLE);

    // NVIC configuration
    SDIO_NVIC_Configuration();

	// GPIO : PIN Init
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  SDCARD_EN_PIN; // (DAC0 => PA04)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
#endif

/*
******************************************************************************************************************************
******************************************************************************************************************************
*                                     GENERAL INTERFACE FUNCTIONS
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

void  BSP_Init (void)
{
	// Clock Config: HSE 72 MHz
#if __RTC_ENABLE == ON
	RTC_Configuration();
#else
	RCC_Configuration();
#endif


#if __USART1_ENABLE == ON
	// USART1 Config ==> FTDI
	USART1_Configuration();
#endif

#if __USART3_ENABLE == ON
	// USART3 Config ==> Trypano support card
	USART3_Configuration();
#endif


#if __SRAM_ENABLE == OFF && __LED_ENABLE == ON
	// LED Config
		LED_Configuration();
#endif

#if __SWITCH_ENABLE == ON
	// SWITCH Config
	SWITCH_Configuration();
#endif

	// I2C Config
#if __I2C_ENABLE == ON
		I2C_Configuration();
#endif

#if __POT_ENABLE == ON
	// Pot / ADC Config
	ADC_POT_Configuration();
#endif

#if __USB_ENABLE == ON
	//USB Config
	USB_Configuration();
#endif

#if __DAC1_ENABLE == ON
	//DAC Config and start
	DAC1_Configuration();
#endif


#if __SRAM_ENABLE == ON
	// RAM Config
	SRAM_Configuration();
#endif

#if __SDCARD_ENABLE == ON
	// SDCard Config
	SDCARD_Configuration();
#endif

#if __DAC2_ENABLE == ON
	DAC2_Configuration();
#endif

#if __ADCe1_ENABLE == ON
	ADCe1_Configuration();
#endif

}

/*
*********************************************************************************************************
*                                      Get the CPU Clock Frequency
*
* Description: This function reads CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s): none
*
* Returns    : none
*********************************************************************************************************
*/

INT32U  BSP_CPU_ClkFreq (void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	return (INT32U)RCC_Clocks.SYSCLK_Frequency;
}

/*
*********************************************************************************************************
*                                      Disable the CPU interruption
*
* Description: Disable the Hardware interruptions
*
* Argument(s): none
*
* Returns    : none
*********************************************************************************************************
*/
void  BSP_IntDisAll(void)
{
	__disable_irq();
}

/*
*********************************************************************************************************
*                                      Enable the CPU interruption
*
* Description: Enable the Hardware interruptions
*
* Argument(s): none
*
* Returns    : none
*********************************************************************************************************
*/
void  BSP_IntEnAll(void)
{
	__enable_irq();
}

/*
*********************************************************************************************************
*                                      Enable Direct LED x
*
* Description: Enable one of the LED
*
* Argument(s): Identifier of the LED
*
* Returns    : none
*********************************************************************************************************
*/

void BSP_LEDOn (Led_TypeDef Led)
{
	D_LED_PORT->BSRR = D_LED_PIN[Led];
}

/*
*********************************************************************************************************
*                                      Disable Direct LED x
*
* Description: Disable one of the LED
*
* Argument(s): Identifier of the LED
*
* Returns    : none
*********************************************************************************************************
*/

void BSP_LEDOff (Led_TypeDef Led)
{
	D_LED_PORT->BRR = D_LED_PIN[Led];
}

/*
*********************************************************************************************************
*                                      Enable SDCARD
*
* Description: Enable the SDCARD component
*
* Argument(s): none
*
* Returns    : none
*********************************************************************************************************
*/

void BSP_SDCARD_ENABLE(void)
{
	SDCARD_EN_PORT->BRR = SDCARD_EN_PIN;
}

/*
*********************************************************************************************************
*                                      Disable SDCARD
*
* Description: Disable the SDCARD component
*
* Argument(s): none
*
* Returns    : none
*********************************************************************************************************
*/

void BSP_SDCARD_DISABLE(void)
{
	SDCARD_EN_PORT->BSRR = SDCARD_EN_PIN;
}

/*
*********************************************************************************************************
*                                      Toggle Direct LED x
*
* Description: Toggle one of the LED
*
* Argument(s): Identifier of the LED
*
* Returns    : none
*********************************************************************************************************
*/

void BSP_LEDToggle (Led_TypeDef Led)
{
	D_LED_PORT->ODR ^= D_LED_PIN[Led];
}

/*
*********************************************************************************************************
*                                      Set Direct LED x
*
* Description: Set one of the LED
*
* Argument(s): Identifier of the LED, Value to set
*
* Returns    : none
*********************************************************************************************************
*/

void BSP_LEDSet (Led_TypeDef Led, INT16U on)
{
	if(on != 0)
		BSP_LEDOn(Led);
	else
		BSP_LEDOff(Led);
}
/*
*********************************************************************************************************
*                                   	Switch get x value
*
* Description: Get the value of a switch button
*
* Argument(s): Identifier of the switch
*
* Returns    : The value of the specified switch
*********************************************************************************************************
*/

INT8U BSP_SWValue (SW_TypeDef sw)
{
	return ((SWITCH_PORT->IDR >> sw) & 1) ^ 1;
}

/*********************************************************************************************************
*                                   	Switch get all values
*
* Description: Get the value of all the switch buttons
*
* Argument(s): null
*
* Returns    : The switches values
*********************************************************************************************************
*/

INT8U BSP_SWValues (void)
{
	return (SWITCH_PORT->IDR >> 8) ^ 0xFF;
}

/*********************************************************************************************************
*                                   	I2C LEDs
*
* Description: Set the I2C LEDs register to the given value
*
* Argument(s): The values to put in the register
*
* Returns    : null
*********************************************************************************************************
*/

void BSP_I2C_LEDS(INT8U reg)
{

	/* Disable the I2C peripheral */
	I2C_Cmd(I2C_PORT, DISABLE);

	/* Reset all I2C2 registers */
	I2C_SoftwareResetCmd(I2C_PORT, ENABLE);
	I2C_SoftwareResetCmd(I2C_PORT, DISABLE);

	/* Configure the I2C peripheral */
	I2C_Local_Config();

	/* Enable the I2C peripheral --- err*/
	//I2C_GenerateSTART(I2C1, ENABLE);

	/* Send the LEDs value */
	I2C_GenerateSTART(I2C_PORT, ENABLE);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C_PORT, I2C_ADDRESS, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C_PORT, I2C_OUTPUT_P0);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C_PORT, reg);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2C_PORT, ENABLE);

}

/*********************************************************************************************************
*                                   	I2C Joystick
*
* Description: Get the value of the Joystick Register
*
* Argument(s): void
*
* Returns    : The value of the Joystick Register
*********************************************************************************************************
*/

INT8U BSP_I2C_JOY(void)
{

	INT8U buffer = 0xFF;

	/* Disable the I2C peripheral */
	I2C_Cmd(I2C_PORT, DISABLE);

	/* Reset all I2C2 registers */
	I2C_SoftwareResetCmd(I2C_PORT, ENABLE);
	I2C_SoftwareResetCmd(I2C_PORT, DISABLE);

	/* Configure the I2C peripheral */
	I2C_Local_Config();

	/* Read the joystick values */
	/* First phase => Preparing the reading : mode write*/
	I2C_GenerateSTART(I2C_PORT, ENABLE);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C_PORT, I2C_ADDRESS, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Clear EV6 by setting again the PE bit */
	// ??? I2C_Cmd(I2C_PORT, ENABLE);

	I2C_SendData(I2C_PORT, I2C_INPUT_P1); // Input port 1
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Second phase => reading, stop when not ack by the master */
	I2C_GenerateSTART(I2C_PORT, ENABLE);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C_PORT, I2C_ADDRESS, I2C_Direction_Receiver);
	// Disabling the ACK from the STM32 to the I2C
	// This way the I2C stop sending the registers after the first one
	I2C_AcknowledgeConfig(I2C_PORT, DISABLE);
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	// Waiting for the first register value
	while (!I2C_CheckEvent(I2C_PORT, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Reading the register value
	buffer = I2C_ReceiveData(I2C_PORT);

	// Should be stopped by not ack the "packet", but this way we are sure
	I2C_GenerateSTOP(I2C_PORT, ENABLE);

	// We put back the ACK from the STM32
	I2C_AcknowledgeConfig(I2C_PORT, ENABLE);

	return buffer;
}

/*********************************************************************************************************
*                                   	SDIO (CARD SD)
*
* Description: TEST SDIO - erasing the SD card and then proceeding to a little write / read comparison
*
* Argument(s): void
*
* Returns    : void
*********************************************************************************************************
*/

uint8_t BSP_TEST_SDIO(void)
{
	uint8_t t_status = 0;

	/*-------------------------- SD Init ----------------------------- */
	BSP_SDCARD_ENABLE();

	Status = SD_Init();

	if (Status == SD_OK)
	{
	/*----------------- Read CSD/CID MSD registers ------------------*/
		Status = SD_GetCardInfo(&SDCardInfo);
	}

	if (Status == SD_OK)
	{
	  /*----------------- Select Card --------------------------------*/
	  Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
	}

	if (Status == SD_OK)
	{
	  Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
	}

	/*------------------- Block Erase -------------------------------*/
	if (Status == SD_OK)
	{
	    /* Erase NumberOfBlocks Blocks of WRITE_BL_LEN(512 Bytes) */
	  Status = SD_Erase(0x00, (SDIO_BLOCK_SIZE * SDIO_NB_BLOCK));
	}

	/* Set Device Transfer Mode to DMA */
	if (Status == SD_OK)
	{
	  Status = SD_SetDeviceMode(SD_DMA_MODE);
	}

	if (Status == SD_OK)
	{
	  Status = SD_ReadMultiBlocks(0x00, Buffer_MultiBlock_Rx, SDIO_BLOCK_SIZE, SDIO_NB_BLOCK);
	}

	if (Status == SD_OK)
	{
		t_status = eBuffercmp(Buffer_MultiBlock_Rx, SDIO_MULTIWSIZE);
	}

	/*------------------- Block Read/Write --------------------------*/
	/* Fill the buffer to send */
	Fill_Buffer(Buffer_Block_Tx, SDIO_BUFFERW_SIZE, 0xFFFF);

	if (Status == SD_OK)
	{
	  /* Write block of 512 bytes on address 0 */
	  Status = SD_WriteBlock(0x00, Buffer_Block_Tx, SDIO_BLOCK_SIZE);
	}

	if (Status == SD_OK)
	{
	  /* Read block of 512 bytes from address 0 */
	  Status = SD_ReadBlock(0x00, Buffer_Block_Rx, SDIO_BLOCK_SIZE);
	}

	if (Status == SD_OK)
	{
	  /* Check the corectness of written dada */
		t_status &= Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, SDIO_BUFFERW_SIZE);
	}

	BSP_SDCARD_DISABLE();

	return t_status;
}

/*********************************************************************************************************
*                                   	SRAM MEMORY
*
* Description: TEST SRAM - Write / read comparison  on the full RAM using random value
*
* Argument(s): void
*
* Returns    : void
*********************************************************************************************************
*/

INT8U BSP_TEST_SRAM(void)
{

	return FSMC_SRAM_test();

}


/*********************************************************************************************************
*                                   	ADC / POTENTIOMETER
*
* Description: Get the current value of the POTENTIOMETER
*
* Argument(s): void
*
* Returns    : The value of the Potentiometer
*********************************************************************************************************
*/

INT16U BSP_ADC_POT(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);

	INT16U ADCConvertedValue = ADC_GetConversionValue(ADC1);

	return ADCConvertedValue;
}

/*********************************************************************************************************
*                                   	DAC2 set value
*
* Description: Set the value of the DAC2
*
* Argument(s): void
*
* Returns    : The value of the Potentiometer
*********************************************************************************************************
*/

void BSP_SET_DAC2(uint16_t value)
{
	DAC_SetChannel2Data(DAC_Align_12b_R, value);
}

uint16_t BSP_GET_ADCe1()
{
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);

	while(ADC_GetFlagStatus(ADC2 ,ADC_FLAG_EOC) != SET);

	uint16_t ADCConvertedValue = ADC_GetConversionValue(ADC2);

	return ADCConvertedValue;
}


/*
*********************************************************************************************************
*                                         TIMER #0 IRQ HANDLER
*
* Description : This function handles the timer interrupt that is used to generate TICKs for uC/OS-II.
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)     :
*********************************************************************************************************
*/
void Tmr_TickISR_Handler (void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL(); /* Tell uC/OS-II that we are starting an ISR */
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	OSTimeTick(); /* Call uC/OS-II's OSTimeTick() */
	OSIntExit(); /* Tell uC/OS-II that we are leaving the ISR */
}

