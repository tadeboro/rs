#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "codec.h"

// LEDS
#define LED_BLUE_ON   GPIOD->BSRRL = GPIO_Pin_15
#define LED_BLUE_OFF  GPIOD->BSRRH = GPIO_Pin_15

// Sound configuration
#define NOTEAMPLITUDE 50

static void
do_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  SystemInit();

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
}

typedef enum
{
  TON_C = 0,
  TON_D,
  TON_E,
  TON_F,
  TON_G,
  TON_A,
  TON_H
}
Note;

typedef enum
{
  DUR_W = 0,
  DUR_H,
  DUR_Q,
  DUR_E
}
Duration;

static void
play_tone(Note     note,
          Duration dur)
{
  // Constants
  static float tones[] = {
      0.01090f, 0.01223f, 0.01373f, 0.01455f,
      0.01633f, 0.01833f, 0.02058f, 0.02180f
  };
  static int durations[] = {
      72000, 36000, 18000, 9000
  };
  static int pauses[] = {
      24000, 12000, 6000, 3000
  };

  volatile int16_t sample = 0;
  float sawWave = 0.0;

  // Actually play the sound
  LED_BLUE_ON;
  float tone = tones[note];
  int cntr = durations[dur];
  while (cntr--)
    {
      // Data for left channel
      while (!SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE));
      SPI_I2S_SendData(CODEC_I2S, sample);
      // Data for right channel
      while (!SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE));
      SPI_I2S_SendData(CODEC_I2S, sample);
      // Update data
      sawWave += tone;
      if (sawWave > 1.0)
        sawWave -= 2.0;
      sample = (int16_t)(NOTEAMPLITUDE*sawWave);
    }

  LED_BLUE_OFF;
  cntr = pauses[dur];
  while (cntr--)
    {
      while (!SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE));
      SPI_I2S_SendData(CODEC_I2S, 0);
      while (!SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE));
      SPI_I2S_SendData(CODEC_I2S, 0);
    }
}

int
main(void)
{
  do_init ();

  // Song
  Note notes[] = {
      TON_C, TON_C, TON_C, TON_C, TON_D, TON_D, TON_D, TON_D,
      TON_E, TON_E, TON_D, TON_D, TON_C, TON_C, TON_C
  };
  Duration durs[] = {
      DUR_Q, DUR_Q, DUR_Q, DUR_Q, DUR_Q, DUR_Q, DUR_Q, DUR_Q,
      DUR_Q, DUR_Q, DUR_Q, DUR_Q, DUR_Q, DUR_Q, DUR_H
  };

  int len = sizeof (notes) / sizeof (notes[0]);

  LED_BLUE_OFF;

  int i;
  for (i = 0; i < len; i++)
    play_tone(notes[i], durs[i]);

  return 0;
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
