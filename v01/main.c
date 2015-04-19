#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

#define LIMIT 10000000

/* Initial configuration of board */
static void
init_board(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* Function that delays for 1 ms for each iteration of outer loop */
static int
delay(int ms,
      int increment)
{
  int changes = 0;
  int old = 0;
  int new = 0;

  volatile int n = 6500 * ms;
  while (changes == 1 || n--)
    {
      new = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
      changes += new != old;
      old = new;
    }
  return changes ? (increment + 2) & 0x03 : increment;
}

/* Entry point */
int
main()
{
  unsigned int pins[] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
  int active = 0;
  int increment = 1;

  init_board();

  GPIO_SetBits(GPIOD, pins[active]);
  while (1)
    {
      increment = delay(1000, increment);
      GPIO_ResetBits(GPIOD, pins[active]);
      active = (active + increment) & 0x03;
      GPIO_SetBits(GPIOD, pins[active]);
    }

  return 0;
}
