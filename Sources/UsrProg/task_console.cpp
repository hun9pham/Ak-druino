#include "app.h"
#include "app_dbg.h"
#include "FAClass.h"
#include "task_list.h"
#include "task_console.h"

#include "sys_io.h"
#include "sys_cfgCtl.h"

#define TAG "TaskConsole"

static QueueHandle_t rxCsQueue = NULL;
static uint8_t u8RxCs;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (huart->Instance == USART1) {
		if (!xQueueIsQueueFullFromISR(rxCsQueue)) {
			xQueueSendFromISR(rxCsQueue, &u8RxCs, &xHigherPriorityTaskWoken);
			if(xHigherPriorityTaskWoken) {
				portYIELD();
			}
		}
		HAL_UART_Receive_IT(&huart1, &u8RxCs, sizeof(uint8_t));
	}
}

void taskConsoleCb(void *params) {
    // DYNAMO_CYBORG.syncAllTasksStarted();
	APP_LOG(TAG, "Started");

	rxCsQueue = xQueueCreate(RX_QUEUE_SIZE, sizeof(uint8_t));
	faASSERT(rxCsQueue != NULL);
	HAL_UART_Receive_IT(&huart1, &u8RxCs, sizeof(uint8_t));

	char ch;
	for (;;) {
        if (pdFAIL != xQueueReceive(rxCsQueue, &ch, portMAX_DELAY)) {
			APP_PRINT("%c", (char)ch);
			
			if (ch == '\r' || ch == '\n') {
				APP_PRINT("\r\n");

				// switch (cmdLineParser(lgnCmdTable, ' ')) {
				// case CMD_SUCCESS:
				// break;

				// case CMD_NOT_FOUND:
				// break;

				// case CMD_TOO_LONG:
				// break;

				// case CMD_TBL_NOT_FOUND:
				// break;

				// default:
				// break;
				// }

				APP_PRINT("-- ");
			}
			else if (ch == 8 || ch == 127) { /* Backspace & Delelte */
				// cmdLineClrChar();
			}
			else {
				// cmdLinePutChar(ch);
			}
		}
	}
}
