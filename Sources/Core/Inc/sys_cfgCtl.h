#ifndef __SYS_CFGCTL_H
#define __SYS_CFGCTL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

#define UART_DBG_TX_PIN     (GPIO_PIN_9)
#define UART_DBG_RX_PIN     (GPIO_PIN_10)
#define UART_DBG_PORT       (GPIOA)

extern IWDG_HandleTypeDef hiwdg;
extern UART_HandleTypeDef huart1;

extern void uartCsInit();
extern void watchdogInit();
extern void memMangCalc();

extern void softReset();
extern void delayUs(uint32_t loadUs);
extern void delayMs(uint32_t loadMs);
extern void watchdogRst();
extern char* getRstReason(bool clr);

#ifdef __cplusplus
}
#endif

#endif