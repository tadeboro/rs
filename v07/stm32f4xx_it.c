/**
  ******************************************************************************
  * @file    FLASH_Program/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4_discovery.h"

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

extern uint32_t base_addr;
extern uint32_t sector;
extern int counter;
extern int mode;

void TIM3_IRQHandler(void)
{
  __IO uint8_t data;
  __IO uint8_t *addr;

  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) == RESET)
    return;

  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
  if (mode == 0) // Read cycle
    {
      STM_EVAL_LEDToggle(LED4);
      addr = (__IO uint8_t *)(base_addr + counter);
      if (*addr)
        STM_EVAL_LEDOn(LED6);
      else
        STM_EVAL_LEDOff(LED6);
    }
  else // Write cycle
    {
      if (counter == 0) // Flash address before programming
        {
          FLASH_Unlock();
          FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR |
                          FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
                          FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR); 
          FLASH_EraseSector(sector, VoltageRange_3);
        }
      STM_EVAL_LEDToggle(LED5);
      FLASH_ProgramByte(base_addr + counter,
                        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
    }
  counter++;
  if (counter == 50)
    {
      mode++;
      counter = 0;
    }
  if (mode > 1)
    {
      TIM_Cmd(TIM3, DISABLE);
      STM_EVAL_LEDOn(LED3);
    }
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
