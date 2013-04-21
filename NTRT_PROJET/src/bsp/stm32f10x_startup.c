/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : stm32f10x_vector.c
* Author             : MCD Tools Team
* Date First Issued  : 05/14/2007
* Description        : This file contains the vector table for STM32F10x.
*                      After Reset the Cortex-M3 processor is in Thread mode,
*                      priority is Privileged, and the Stack is set to Main.
********************************************************************************
* History:
* 05/14/2007: V0.2
* 06/24/2008: V0.3 modified the vector table and some section definition to use
*             the demo with eclipse and GCC.
*             Modified by Stefano Oliveri (mail to: software@stf12.net)
* 10/27/2009  V0.4 to match with NTRT 1A card
*             Modified by Filippo Rusco
*
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "app_int.h"


/*******************************************************************************
* Function Name  : Reset_Handler
* Description    : This is the code that gets called when the processor first starts execution
*		       following a reset event.  Only the absolutely necessary set is performed,
*		       after which the application supplied main() routine is called.
* Input          :
* Output         :
* Return         :
*******************************************************************************/
void Reset_Handler(void)
{
    unsigned long *pulSrc, *pulDest;
    unsigned long *startdata, *enddata, *endtext, *endsram;

    startdata=&_data;
    enddata=&_edata;
    endtext=&_etext;
    endsram=&_esram;
    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *(pulDest++) = *(pulSrc++);
    }

    //
    // Zero fill the bss segment.
    //
    for(pulDest = &_bss; pulDest < &_ebss; )
    {
        *(pulDest++) = 0;
    }

    for(pulDest = &_heap; pulDest < &_main_stack;)
    {
    	*(pulDest++) = 0xDEADBEEF;
    }


    //
    // Call the application's entry point.
    //
    main();
}


/**************************END OF FILE****/
