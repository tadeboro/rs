#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

#define LIMIT 10000000
#define LEFT  0
#define RIGHT 1

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

/* Entry point */
int
main()
{
  uint16_t pins[] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
  int active = 0;
  int old;
  int new;
  int dir;

  init_board();

  /* Set initial state */
  old = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
  dir = LEFT;

  /* Turn on first led */
  GPIO_SetBits(GPIOD, pins[active]);
  while (1)
    {
      /* Delay */
      volatile int n = 6500 * 300;
      while (n--)
        {
          new = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
          dir = (dir + (new < old)) & 1;
          old = new;
        }
      /* Change led */
      GPIO_ResetBits(GPIOD, pins[active]);
      active += dir ? 3 : 1;
      active &= 0x03;
      GPIO_SetBits(GPIOD, pins[active]);
    }

  return 0;
}
