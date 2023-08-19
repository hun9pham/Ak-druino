#include "app.h"
#include "app_dbg.h"
#include "frtosa.h"
#include "task_list.h"
#include "task_devManager.h"

#include "sys_io.h"
#include "sys_cfgCtl.h"

#define TAG "TaskDevManager"

TaskHandle_t CreateDevManager;

static void localStartInit();
static void localHandle(xEvent *EOb);

void taskDevManagerCb(void *params) {
	DYNAMO_CYBORG.syncAllTasksStarted();
	APP_LOG(TAG, "Started");
	localStartInit();

	xEvent EOb;

	for (;;) {
		while (DYNAMO_CYBORG.getMsgFrom(TASK_DEVMANAGER_ID, &EOb)) {
			localHandle(&EOb);
		}
		DYNAMO_CYBORG.watiForSignal(TASK_DEVMANAGER_ID);
	}
}

void localStartInit() {
    DYNAMO_CYBORG.postMsg(TASK_DEVMANAGER_ID, DEVMNG_SENSORS_UPDATE);
}

void localHandle(xEvent *EOb) {
    switch (EOb->Signal) {
	case DEVMNG_SENSORS_UPDATE: {
		APP_DBG_SIG(TAG, "DEVMNG_SENSORS_UPDATE");
		
	}
	break;

	default:
	break;
	}
}
