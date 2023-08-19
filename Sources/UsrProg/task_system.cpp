#include "app.h"
#include "app_dbg.h"
#include "frtosa.h"
#include "task_list.h"
#include "task_system.h"

#include "sys_io.h"
#include "sys_cfgCtl.h"

#define TAG "TaskSystem"

TaskHandle_t CreateSystem;

static void localStartInit();
static void localHandle(xEvent *EOb);

void taskSystemCb(void *params) {
	DYNAMO_CYBORG.syncAllTasksStarted();
	APP_LOG(TAG, "Started");
	localStartInit();

	xEvent EOb;

	for (;;) {
		while (DYNAMO_CYBORG.getMsgFrom(TASK_SYSTEM_ID, &EOb)) {
			localHandle(&EOb);
		}
		DYNAMO_CYBORG.watiForSignal(TASK_SYSTEM_ID);
	}
}

void localStartInit() {
    DYNAMO_CYBORG.setTimer(TASK_SYSTEM_ID, SYSTEM_PING_ALIVE, SYSTEM_ALIVE_NOFITY_INTERVAL, true);
}

void localHandle(xEvent *EOb) {
    switch (EOb->Signal) {
	case SYSTEM_PING_ALIVE: {
		APP_DBG_SIG(TAG, "SYSTEM_PING_ALIVE");
		blinkLedLife();
		watchdogRst();
		
	}
	break;

	case SYSTEM_SOFT_REBOOT: {
		APP_DBG_SIG(TAG, "SYSTEM_SOFT_REBOOT");

		softReset();
	}
	break;

	default:
	break;
	}
}
