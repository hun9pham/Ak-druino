#include "sys_io.h"

void ledLifeInit() {
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = IO_LED_LIFE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(IO_LED_LIFE_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(IO_LED_LIFE_PORT, IO_LED_LIFE_PIN, GPIO_PIN_SET);
}

void ledLifeOn() {
    HAL_GPIO_WritePin(IO_LED_LIFE_PORT, IO_LED_LIFE_PIN, GPIO_PIN_RESET);
}

void ledLifeOff() {
    HAL_GPIO_WritePin(IO_LED_LIFE_PORT, IO_LED_LIFE_PIN, GPIO_PIN_SET);
}

void blinkLedLife() {
    HAL_GPIO_TogglePin(IO_LED_LIFE_PORT, IO_LED_LIFE_PIN);
}

void periphsClkStart() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}