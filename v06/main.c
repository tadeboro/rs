//****************************************************
//
//   EXAMPLE OF USING CS42L22 on STM32F4-Discovery
//
//   author: A.Finkelmeyer
//   http://www.mind-dump.net/configuring-the-stm32f4-discovery-for-audio
//
//****************************************************

#include "main.h"


int main(void)
{
      SystemInit();

      //enables GPIO clock for PortD
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

      GPIO_Init(GPIOD, &GPIO_InitStructure);

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

      codec_init();
      codec_ctrl_init();

      I2S_Cmd(CODEC_I2S, ENABLE);

      while(1)
      {

          if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE))
          {
                  SPI_I2S_SendData(CODEC_I2S, sample);

                  //only update on every second sample to insure that L & R ch. have the same sample value
                  if (sampleCounter & 0x00000001)
                  {
                          sawWave += NOTEFREQUENCY;
                          if (sawWave > 1.0)
                                  sawWave -= 2.0;
                          sample = (int16_t)(NOTEAMPLITUDE*sawWave);
                  }
                  sampleCounter++;
          }

          if (sampleCounter==48000)
          {
                  LED_BLUE_OFF;

          }
          else if (sampleCounter == 96000)
          {
                  LED_BLUE_ON;
                  sampleCounter = 0;
          }

      }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
