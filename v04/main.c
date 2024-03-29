#include "stm32f4_discovery.h"

int
main (void)
{
  // Init. GPIOB, kjer sta pina TX in RX (PB6 in PB7)
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef gpio_init;
  gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init (GPIOB, &gpio_init);
  GPIO_PinAFConfig (GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
  GPIO_PinAFConfig (GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

  // Init. USART1
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
  USART_InitTypeDef usart_init;
  usart_init.USART_BaudRate = 1;
  usart_init.USART_WordLength = USART_WordLength_8b;
  usart_init.USART_StopBits = USART_StopBits_1;
  usart_init.USART_Parity = USART_Parity_No;
  usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init (USART1, &usart_init);
  USART_Cmd (USART1, ENABLE);

  // Init leds
  STM_EVAL_LEDInit (LED3);
  STM_EVAL_LEDInit (LED4);
  STM_EVAL_LEDInit (LED5);
  STM_EVAL_LEDInit (LED6);

  uint16_t data = 10;
  int i;

  do
    {
      /* Wait for data */
      while (!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
      data = USART_ReceiveData (USART1);

      /* Turn on leds */
      STM_EVAL_LEDOff (LED3);
      STM_EVAL_LEDOff (LED4);
      STM_EVAL_LEDOff (LED5);
      STM_EVAL_LEDOff (LED6);
      if (data & 8) STM_EVAL_LEDOn (LED4);
      if (data & 4) STM_EVAL_LEDOn (LED3);
      if (data & 2) STM_EVAL_LEDOn (LED5);
      if (data & 1) STM_EVAL_LEDOn (LED6);
      data--;

      /* Delay */
      for (i = 0; i < 5999999; i++);

      /* Send data */
      while (!USART_GetFlagStatus (USART1, USART_FLAG_TXE));
      USART_SendData (USART1, data);
    }
  while (data > 0);

  return 0;
}
