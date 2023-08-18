#ifndef __SYS_IO_H
#define __SYS_IO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define IO_LED_LIFE_PIN     (GPIO_PIN_8)
#define IO_LED_LIFE_PORT    (GPIOB)

extern void ledLifeInit();
extern void ledLifeOn();
extern void ledLifeOff();
extern void blinkLedLife();

extern void periphsClkStart(void);

#ifdef __cplusplus
}
#endif

#endif