#include "FAClass.h"

#include "xprintf.h"
#include "sys_io.h"
#include "sys_cfgCtl.h"

FreeRTOSClass DYNAMO_CYBORG;

////////////////////////////////////////////
// LOCAL DEFINITIONS
///////////////////////////////////////////
#define lcLOG_PRINT_EN  (1)
#define lcLOG_DBG_EN    (0)

#define LOCAL_PRINT         	xprintf

#if lcLOG_PRINT_EN
#define LOCAL_LOG(fmt, ...) 	LOCAL_PRINT(faKRED "[FAClass] " faKMAG fmt faRST "\r\n", ##__VA_ARGS__)
#else
#define LOCAL_LOG(fmt, ...)
#endif

#if lcLOG_DBG_EN
#define LOCAL_DBG(fmt, ...) 	LOCAL_PRINT(faKYEL "[DBG]->" faKCYN fmt faRST "\r\n", ##__VA_ARGS__)
#else
#define LOCAL_DBG(fmt, ...)
#endif

#if fa_RELEASE
    #define BUILD   (const char*)"RELEASE"
    #define VERSION	(const char*)"v1.1"
    #define ASSERT	(0)
#else
    #define BUILD   (const char*)"DEBUG"
    #define VERSION (const char*)"v1.0"
    #define ASSERT	(1)
#endif

void fa_ParamsTRUE(bool isTRUE, const char *file, uint32_t line) {
    bool firstPrint = true;
    if (!isTRUE) {
        faENTRY_ATOMIC();
        do {
            if (firstPrint) {
                firstPrint = false;
                LOCAL_PRINT("[ASSERT]\r\n");
                LOCAL_PRINT(" -File: %s\r\n", file);
                LOCAL_PRINT(" -Line: %ld\r\n", line);
            }
            delayMs(30);
            watchdogRst();
            blinkLedLife();
        } while(ASSERT);
        softReset();
    }
}

////////////////////////////////////////////
// FreeRTOS Assistant Class
///////////////////////////////////////////
void FreeRTOSClass::bannerIntro() {
#define faBANNER(fmt, ...)  LOCAL_PRINT(faKCYN fmt faRST, ##__VA_ARGS__)
    faBANNER("\r\n"
            " __ \\                                           \r\n"
            " |   |  |   |  __ \\    _` |  __ `__ \\    _ \\  \r\n"
            " |   |  |   |  |   |  (   |  |   |   |  (   |    \r\n"
            "____/  \\__, | _|  _| \\__,_| _|  _|  _| \\___/  \r\n"
            "       ____/                                     \r\n"
            );
    faBANNER("FreeRTOS Assistant %s\r\n", VERSION);
    faBANNER(" -Bot: %s\r\n", fa_CYBORG);
    faBANNER(" -Run as: %s\r\n", BUILD);
    faBANNER(" -Date: %s, %s\r\n\r\n", __DATE__, __TIME__);
}

FreeRTOSClass::FreeRTOSClass() {

}               

FreeRTOSClass::~FreeRTOSClass() {

}

void FreeRTOSClass::programInit(xTask_t *taskTbl, uint8_t taskTblSize) {
    FreeRTOSClass::bannerIntro();

    taskManager.taskTbl = taskTbl;
    taskManager.taskTblSize = taskTblSize;
    LOCAL_LOG("TaskTableLen: %d", taskManager.taskTblSize);
    faASSERT(taskManager.taskTbl != NULL && taskManager.taskTblSize != 0);

    lcSemaphore = xSemaphoreCreateCounting(taskManager.taskTblSize, taskManager.taskTblSize);
    faASSERT(lcSemaphore != NULL);

    for (uint8_t tId = 0; tId < taskManager.taskTblSize; ++tId) {
        taskManager.taskTbl[tId].EventQueue = xQueueCreate(faMAILBOX_SIZE, sizeof(xEvent_t));
        faASSERT(taskManager.taskTbl[tId].EventQueue != NULL);
        faASSERT(
            pdFAIL != xTaskCreate(taskManager.taskTbl[tId].funcCallback,
                                    taskManager.taskTbl[tId].Desc,
                                    taskManager.taskTbl[tId].sizeOfStack,
                                    NULL,
                                    taskManager.taskTbl[tId].Rank,
                                    NULL
                                )
                );
        LOCAL_LOG("[TaskCreate] %s\tId: %d\tStackSize: %d\tPrio: %d\tCb: 0x%x",
                taskManager.taskTbl[tId].Desc,
                taskManager.taskTbl[tId].tId,
                taskManager.taskTbl[tId].sizeOfStack,
                taskManager.taskTbl[tId].Rank,
                taskManager.taskTbl[tId].funcCallback
                );
    }
    LOCAL_LOG("HeapFreeSize: %d", xPortGetFreeHeapSize());
}

void FreeRTOSClass::syncAllTasksStarted(const char *taskEntry) {
    faASSERT(pdFAIL != xSemaphoreTake(lcSemaphore, 10));
    while (uxSemaphoreGetCount(lcSemaphore) > 0) {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void FreeRTOSClass::activeEvent(xEvent_t *evt) {
    LOCAL_DBG("[EventActive]");
    LOCAL_DBG(" -DestTaskId: %d", evt->destTaskId);
    LOCAL_DBG(" -Signal: %d", evt->Signal);
    LOCAL_DBG(" -Type: %s", evt->Payload != NULL ? "Dynamic" : "Pure");
    faASSERT(evt->destTaskId < taskManager.taskTblSize);
    faASSERT(uxQueueSpacesAvailable(taskManager.taskTbl[evt->destTaskId].EventQueue) > 0);
    xQueueSend(taskManager.taskTbl[evt->destTaskId].EventQueue, (void*)evt, portMAX_DELAY);
}

void FreeRTOSClass::postPureMsg(uint8_t destTaskId, uint8_t Sig) {
    xEvent_t evtObject = {
        .destTaskId = destTaskId,
        .Signal = Sig,
        .Payload = NULL,
        .Size = 0
    };
    activeEvent(&evtObject);
}

void FreeRTOSClass::postDynamicMsg(uint8_t destTaskId, uint8_t Sig, uint8_t *data, uint32_t len) {
    faASSERT(data!= NULL && len != 0);
    xEvent_t evtObject = {
        .destTaskId = destTaskId,
        .Signal = Sig,
        .Payload = data,
        .Size = len
    };
    activeEvent(&evtObject);
}

void FreeRTOSClass::getMsgFrom(uint8_t tId, xEvent_t *evt) {
    faASSERT(tId < taskManager.taskTblSize);
    if (pdPASS == xQueueReceive(taskManager.taskTbl[tId].EventQueue, evt, portMAX_DELAY)) {
        ;
    }
}

void FreeRTOSClass::freeMsg(xEvent_t *evt) {
    if (evt->Payload != NULL) {
        vPortFree(evt->Payload);
    }
}

static void localSoftTimerCb(TimerHandle_t timId) {
    xEvent_t *evtTimer = (xEvent_t*)pvTimerGetTimerID(timId);
    faASSERT(evtTimer != NULL);
    LOCAL_DBG("[SoftEventTimer]");
    LOCAL_DBG(" -DestTaskId: %d", evtTimer->destTaskId);
    LOCAL_DBG(" -Signal: %d", evtTimer->Signal);
    DYNAMO_CYBORG.activeEvent(evtTimer);
}

EventTick_t FreeRTOSClass::setTimer(uint8_t destTaskId, uint8_t Sig, uint32_t period, bool isReload) {
    faASSERT(destTaskId < taskManager.taskTblSize);

    EventTick_t softTimId = NULL;
    xEvent_t *evtTim = NULL;
    evtTim = (xEvent_t*)pvPortMalloc(sizeof(xEvent_t));
    faASSERT(evtTim != NULL);

    evtTim->destTaskId = destTaskId;
    evtTim->Signal = Sig;
    evtTim->Payload = NULL;
    evtTim->Size = 0;
    softTimId = xTimerCreate("SoftEvtTimer",
                            pdMS_TO_TICKS(period),
                            (isReload) ? pdTRUE : pdFALSE,
                            (void*)evtTim,
                            localSoftTimerCb
                            );

    faASSERT(NULL != softTimId);                                
    xTimerStart(softTimId, pdMS_TO_TICKS(100));

   return softTimId;
}

void FreeRTOSClass::removeTimer(EventTick_t evtTim) {
    xEvent_t *evtTimId = (xEvent_t*)pvTimerGetTimerID(evtTim);

	if (evtTimId != NULL) {
		vPortFree(evtTimId);
	}
	faASSERT(pdPASS == xTimerDelete(evtTimId, pdMS_TO_TICKS(100)));
}
