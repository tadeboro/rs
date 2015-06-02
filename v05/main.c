#include "stm32f4_discovery.h"

static void
display(uint16_t data)
{
  int i;

  // clear LEDs
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDOff(LED5);
  STM_EVAL_LEDOff(LED6);

  // set LEDs
  if(data & 8) STM_EVAL_LEDOn(LED4);
  if(data & 4) STM_EVAL_LEDOn(LED3);
  if(data & 2) STM_EVAL_LEDOn(LED5);
  if(data & 1) STM_EVAL_LEDOn(LED6);

  for(i = 0; i < 5999999; i++) {}
}

static uint16_t
crc(uint16_t data,
    int      size,
    uint16_t poly,
    int      degree)
{
  uint16_t mask = (1 << degree) - 1;
  uint16_t rem = mask;

  degree--;
  while (size--)
    {
      uint16_t tmp = ((rem >> degree) ^ (data >> size)) & 1;

      rem <<= 1;
      rem = tmp ? rem ^ poly : rem;
      display(rem);
  }

  return ~rem & mask;
}

int
main (int    argc,
      char **argv)
{
  uint16_t data = 0x9; // 1001
  uint16_t poly = 0x5; // 0101
  uint16_t res;

  /* Init leds */
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);

  /* Do calculations */
  res = crc(data, 4, poly, 4);
  display(res);
  display(res);
  display(res);
  display(res);
  display(res);
  display(res);
  res = crc((data << 4) | res, 8, poly, 4);

  return 0;
}
