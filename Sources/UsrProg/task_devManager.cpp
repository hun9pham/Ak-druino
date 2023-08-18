#include "app.h"
#include "app_dbg.h"
#include "FAClass.h"
#include "task_list.h"
#include "task_devManager.h"

#include "sys_io.h"
#include "sys_cfgCtl.h"

#define TAG "TaskDevManager"

static void localStartInit();
static void localHandle(xEvent_t *evtHandler);

void taskDevManagerCb(void *params) {
	DYNAMO_CYBORG.syncAllTasksStarted("TaskDevManager");
	APP_LOG(TAG, "Started");
	localStartInit();

	xEvent_t EventObject;

	for (;;) {
        DYNAMO_CYBORG.getMsgFrom(TASK_DEVMANAGER_ID, &EventObject);
		localHandle(&EventObject);
		DYNAMO_CYBORG.freeMsg(&EventObject);
	}
}

void localStartInit() {
    DYNAMO_CYBORG.setTimer(TASK_DEVMANAGER_ID, DEVMNG_SENSORS_UPDATE, DEVMNG_SENSORS_UPDATE_INTERVAL, true);
}

void localHandle(xEvent_t *evtHandler) {
    switch (evtHandler->Signal) {
	case DEVMNG_SENSORS_UPDATE: {
		APP_DBG_SIG(TAG, "DEVMNG_SENSORS_UPDATE");

	}
	break;

	default:
	break;
	}
}
