#include "app.h"
#include "app_dbg.h"
#include "FAClass.h"
#include "task_list.h"
#include "task_system.h"

#include "sys_io.h"
#include "sys_cfgCtl.h"

#define TAG "TaskSystem"

static void localStartInit();
static void localHandle(xEvent_t *evtHandler);

void taskSystemCb(void *params) {
	DYNAMO_CYBORG.syncAllTasksStarted("TaskSystem");
	APP_LOG(TAG, "Started");
	localStartInit();

	xEvent_t EventObject;

	for (;;) {
        DYNAMO_CYBORG.getMsgFrom(TASK_SYSTEM_ID, &EventObject);
		localHandle(&EventObject);
		DYNAMO_CYBORG.freeMsg(&EventObject);
	}
}

void localStartInit() {
    DYNAMO_CYBORG.setTimer(TASK_SYSTEM_ID, SYSTEM_PING_ALIVE, SYSTEM_ALIVE_NOFITY_INTERVAL, true);
}

void localHandle(xEvent_t *evtHandler) {
    switch (evtHandler->Signal) {
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
