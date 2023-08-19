#include "xprintf.h"

#include "sys_cfgCtl.h"
#include "sys_dbg.h"

IWDG_HandleTypeDef hiwdg;
UART_HandleTypeDef huart1;

static void uartCsPut(uint8_t ch) {
    HAL_UART_Transmit(&huart1, &ch, sizeof(uint8_t), 0xFFFF);
}

void UartCsInit() {
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitStruct.Pin = UART_DBG_TX_PIN | UART_DBG_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(UART_DBG_PORT, &GPIO_InitStruct);

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 460800;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    xfunc_output = (void(*)(int))uartCsPut;
}

void softReset() {
    NVIC_SystemReset();
}

void delayUs(uint32_t loadUs) {
    uint32_t count = (loadUs * (SystemCoreClock / 1000000)) / 4;
	for (uint32_t i = 0 ; i < count; ++i) {
		count--;
	}
}

void delayMs(uint32_t loadMs) {
    uint32_t count = ((loadMs * 1000) * (SystemCoreClock / 1000000)) / 4;
	for (uint32_t i = 0 ; i < count; ++i) {
		count--;
	}
}

void watchdogInit() {
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
    hiwdg.Init.Reload = 4095;
    HAL_IWDG_Init(&hiwdg);
}

void memMangCalc() {
    uint32_t flashUsed = 0;
    uint32_t sramUsed = 0;
    uint32_t dataSize = 0;
    uint32_t bssSize = 0;
    uint32_t stackAvai = 0;
    uint32_t heapLen = 0;
    
    SYS_PRINT("[MEMORY MANAGEMENT]\r\n");
    SYS_PRINT("FLASH used:\t\t%d bytes\n", flashUsed);
    SYS_PRINT("SRAM used:\t\t%d bytes\n", sramUsed);
    SYS_PRINT("\t.data:\t\t%d bytes\n", dataSize);
    SYS_PRINT("\t.bss:\t\t%d bytes\n", bssSize);
    SYS_PRINT("\tStack avaiable:\t%d bytes\n", stackAvai);
    SYS_PRINT("\tHeap length:\t%d bytes\n", heapLen);
    SYS_PRINT("\n");
}

void watchdogRst() {
    HAL_IWDG_Refresh(&hiwdg);
}

char* getRstReason(bool clr) {
    char *res;

	if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)) {
		res = "POR/PDR";
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
		res = "SOFT";
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
		res = "IWDG";
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) {
		res = "WWDG";
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)) {
		res = "LPWR";
	}
	else {
		res = "PIN";
	}
    
	if (clr) {
		__HAL_RCC_CLEAR_RESET_FLAGS();
	}
	return res;
}