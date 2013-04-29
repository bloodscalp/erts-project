/*
*********************************************************************************************************
*
*                                      Utilities functions for BSP
*
*                                             ARM Cortex M3
*
* Filename      : bsp_utils.c
* Version       : V1.00
* Programmer(s) : Xavier Meyer
*********************************************************************************************************
*/

#include "bsp_utils.h"


/*********************************************************************************
 *             					Global functions						         *
 ********************************************************************************/


/**
  *   Function name : Fill_Buffer
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLenght: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer(uint32_t* pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLenght; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/**
  *   Function name : eBuffercmp
  * @brief  Checks if a buffer has all its values are equal to zero.
  * @param  pBuffer: buffer to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer values are zero
  *   FAILED: At least one value from pBuffer buffer is diffrent
  *   from zero.
  */
uint8_t eBuffercmp(uint32_t* pBuffer, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != 0xFFFFFFFF) // Erase seems to put FF on this card
    {
      return -1;
    }

    pBuffer++;
  }

  return 1;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  *   : - BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *   FAILED: pBuffer1 differs from pBuffer2
  */
uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return -1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 1;
}

/**
  *   Function name : Fill_Buffer
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLenght: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer16(uint16_t* pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLenght; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  *   : - BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *   FAILED: pBuffer1 differs from pBuffer2
  */
uint8_t Buffercmp16(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return -1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 1;
}
