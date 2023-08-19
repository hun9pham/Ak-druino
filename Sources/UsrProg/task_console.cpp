#include "frtosa.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_console.h"

#include "sys_io.h"
#include "sys_cfgCtl.h"

#include "cmd_line.h"

#define TAG "TaskConsole"

TaskHandle_t CreateConsole;

static QueueHandle_t rxCsQueue = NULL;
static uint8_t u8RxCs;

static void localStartInit();
static void localHandle(xEvent *EOb);
static void ConsoleTaskInput(void*);

void taskConsoleCb(void *params) {
    DYNAMO_CYBORG.syncAllTasksStarted();
	APP_LOG(TAG, "Started");
	localStartInit();

	xEvent EOb;

	for (;;) {
        while (DYNAMO_CYBORG.getMsgFrom(TASK_CONSOLE_ID, &EOb)) {
			localHandle(&EOb);
		}
		DYNAMO_CYBORG.watiForSignal(TASK_CONSOLE_ID);
	}
}

void localStartInit() {
	rxCsQueue = xQueueCreate(RX_QUEUE_SIZE, sizeof(uint8_t));
	ASSERT_PARAM(rxCsQueue != NULL);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	HAL_UART_Receive_IT(&huart1, &u8RxCs, sizeof(uint8_t));

	ASSERT_PARAM(
		pdFAIL != xTaskCreate(ConsoleTaskInput,
							"InputConsole",
							256,
							NULL,
							3,
							NULL
						)
		);
	
	DYNAMO_CYBORG.postMsg(TASK_CONSOLE_ID, CONSOLE_CLI_INIT);
}

void localHandle(xEvent *EOb) {
	switch (EOb->Signal) {
	case CONSOLE_CLI_INIT: {
		APP_DBG_SIG(TAG, "CONSOLE_CLI_INIT");
		
		CMDLINE.tblCmdLineInit(lgnCmdTable);
	}
	break;

	case CONSOLE_HANDLE_CMD_LINE: {
		APP_DBG_SIG(TAG, "CONSOLE_HANDLE_CMD_LINE");

		switch (CMDLINE.parserChar(' ')) {
		case CMD_SUCCESS:
		break;
		case CMD_NOT_FOUND:
		break;
		case CMD_TOO_LONG:
		break;
		case CMD_TBL_NOT_FOUND:
		break;
		default:
		break;
		}
		APP_PRINT("-- ");
	}
	break;

	default:
	break;
	}
}

void ConsoleTaskInput(void*) {
	char u8Data;

	for (;;) {
        if (pdFAIL != xQueueReceive(rxCsQueue, &u8Data, portMAX_DELAY)) {
			APP_PRINT("%c", (char)u8Data);
			
			if (u8Data == '\r' || u8Data == '\n') {
				APP_PRINT("\r\n");

				DYNAMO_CYBORG.postMsg(TASK_CONSOLE_ID, CONSOLE_HANDLE_CMD_LINE);
			}
			else if (u8Data == 8 || u8Data == 127) { /* Backspace & Delete */
				CMDLINE.getChar();
			}
			else {
				CMDLINE.putChar(u8Data);
			}
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	APP_DBG(TAG, "HAL_UART_RxCpltCallback");

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