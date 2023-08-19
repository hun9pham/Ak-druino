#include "frtosa.h"

#include "xprintf.h"
#include "sys_io.h"
#include "sys_cfgCtl.h"

FreeRTOSClass DYNAMO_CYBORG;

#define lcLOG_PRINT_EN  (1)
#define lcLOG_DBG_EN    (0)

#define LOCAL_PRINT         	xprintf

#if lcLOG_PRINT_EN
#define LOCAL_LOG(fmt, ...) 	LOCAL_PRINT("[frtosa] " "\x1B[35m" fmt "\e[0m" "\r\n", ##__VA_ARGS__)
#else
#define LOCAL_LOG(fmt, ...)
#endif

#if lcLOG_DBG_EN
#define LOCAL_DBG(fmt, ...) 	LOCAL_PRINT("[DBG]->" "\e[0;36m" fmt "\e[0m" "\r\n", ##__VA_ARGS__)
#else
#define LOCAL_DBG(fmt, ...)
#endif

void assertParams(bool IsTrue, const char *file, uint32_t line) {
    bool firstPrint = true;
    if (!IsTrue) {
        ENTRY_CRITICAL();
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

void FreeRTOSClass::bannerIntro() {
    LOCAL_PRINT("\r\n"
            " __ \\                                           \r\n"
            " |   |  |   |  __ \\    _` |  __ `__ \\    _ \\  \r\n"
            " |   |  |   |  |   |  (   |  |   |   |  (   |    \r\n"
            "____/  \\__, | _|  _| \\__,_| _|  _|  _| \\___/  \r\n"
            "       ____/                                     \r\n"
            );
    LOCAL_PRINT("FreeRTOS Assistant %s\r\n", VERSION);
    LOCAL_PRINT(" -Bot: %s\r\n", CYBORG);
    LOCAL_PRINT(" -Run as: %s\r\n", BUILD);
    LOCAL_PRINT(" -Date: %s, %s\r\n\r\n", __DATE__, __TIME__);
}

FreeRTOSClass::FreeRTOSClass() {

}               

FreeRTOSClass::~FreeRTOSClass() {

}

void FreeRTOSClass::programInit(xTask_t *taskTbl, uint8_t taskTblSize) {
    FreeRTOSClass::bannerIntro();

    m_TaskManager.taskTbl = taskTbl;
    m_TaskManager.taskTblSize = taskTblSize;
    LOCAL_LOG("Task table size: %d", m_TaskManager.taskTblSize);
    ASSERT_PARAM(m_TaskManager.taskTbl != NULL && m_TaskManager.taskTblSize != 0);

    m_Semaphore = xSemaphoreCreateCounting(m_TaskManager.taskTblSize, m_TaskManager.taskTblSize);
    ASSERT_PARAM(m_Semaphore != NULL);

    m_rtosEvent.inits();

    for (uint8_t tId = 0; tId < m_TaskManager.taskTblSize; ++tId) {
        ASSERT_PARAM(
            pdFAIL != xTaskCreate(m_TaskManager.taskTbl[tId].funcCallback,
                                    m_TaskManager.taskTbl[tId].Desc,
                                    m_TaskManager.taskTbl[tId].sizeOfStack,
                                    NULL,
                                    m_TaskManager.taskTbl[tId].Rank,
                                    &m_TaskManager.taskTbl[tId].CreateTask
                                )
                );
        m_TaskManager.taskTbl[tId].EOb = EVENT_MSG_NULL;

        LOCAL_LOG("[TaskCreate] %s\tId: %d\tStackSize: %d\tPrio: %d\tCb: 0x%x",
                m_TaskManager.taskTbl[tId].Desc,
                m_TaskManager.taskTbl[tId].tId,
                m_TaskManager.taskTbl[tId].sizeOfStack,
                m_TaskManager.taskTbl[tId].Rank,
                m_TaskManager.taskTbl[tId].funcCallback
                );
    }
    LOCAL_LOG("Heap free size: %d", xPortGetFreeHeapSize());
}

void FreeRTOSClass::syncAllTasksStarted() {
    ASSERT_PARAM(pdFAIL != xSemaphoreTake(m_Semaphore, 10));
    while (uxSemaphoreGetCount(m_Semaphore) > 0) {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void FreeRTOSClass::activeEOb(xEvent *EOb) {
    ENTRY_CRITICAL();
    xEvent *newEOb = m_rtosEvent.mallocEvent();
    
    ASSERT_PARAM(newEOb != EVENT_MSG_NULL);
    memcpy(newEOb, EOb, sizeof(xEvent));

    if (m_TaskManager.taskTbl[EOb->destTaskId].EOb == EVENT_MSG_NULL) {
        m_TaskManager.taskTbl[EOb->destTaskId].EOb = newEOb;
    }
    else {
        xEvent *lastTaskEOb = m_TaskManager.taskTbl[EOb->destTaskId].EOb;

        /* Traverse until meeting last event of task */
        while (lastTaskEOb->link != EVENT_MSG_NULL) {
            lastTaskEOb = lastTaskEOb->link;
        }
        lastTaskEOb->link = newEOb;
    }
    EXIT_CRITICAL();
}

void FreeRTOSClass::postMsg(uint8_t destTaskId, uint8_t Sig, uint8_t *data, uint32_t len) {
    xEvent newEOb = {
        .destTaskId = destTaskId,
        .Signal = Sig,
        .Payload = data,
        .Size = len
    };
    FreeRTOSClass::activeEOb(&newEOb);
}

bool FreeRTOSClass::getMsgFrom(uint8_t tId, xEvent *EOb) {
    bool ret = false;

    ENTRY_CRITICAL();

    ASSERT_PARAM(tId < m_TaskManager.taskTblSize);
    if (m_TaskManager.taskTbl[tId].EOb != EVENT_MSG_NULL) {
        LOCAL_DBG("[MSG]");
        LOCAL_DBG(" -DestTaskId: %d", m_TaskManager.taskTbl[tId].EOb->destTaskId);
        LOCAL_DBG(" -Signal: %d", m_TaskManager.taskTbl[tId].EOb->Signal);

        xEvent *freeEOb = m_TaskManager.taskTbl[tId].EOb;
        ret = true;
        memcpy(EOb, m_TaskManager.taskTbl[tId].EOb, sizeof(xEvent));
        m_TaskManager.taskTbl[tId].EOb = m_TaskManager.taskTbl[tId].EOb->link;
        m_rtosEvent.freeEvent(freeEOb);
    }
    EXIT_CRITICAL();

    return ret;
}

void FreeRTOSClass::watiForSignal(uint8_t taskIdWait) {
    (void)taskIdWait;
    vTaskDelay(50);
}

static void localSoftTimerCb(TimerHandle_t timId) {
    xEvent *EObTim = (xEvent*)pvTimerGetTimerID(timId);

    ASSERT_PARAM(EObTim != NULL);
    LOCAL_DBG("[SoftEventTimer]");
    LOCAL_DBG(" -DestTaskId: %d", EObTim->destTaskId);
    LOCAL_DBG(" -Signal: %d", EObTim->Signal);

    DYNAMO_CYBORG.activeEOb(EObTim);
}

EventTick_t FreeRTOSClass::setTimer(uint8_t destTaskId, uint8_t Sig, uint32_t period, bool isReload) {
    ASSERT_PARAM(destTaskId < m_TaskManager.taskTblSize);

    EventTick_t softTimId = NULL;
    xEvent *EObTim = NULL;

    EObTim = (xEvent*)pvPortMalloc(sizeof(xEvent));
    ASSERT_PARAM(EObTim != NULL);
    {
        EObTim->link = NULL;
        EObTim->destTaskId = destTaskId;
        EObTim->Signal = Sig;
        EObTim->Payload = NULL;
        EObTim->Size = 0;
    }
    
    softTimId = xTimerCreate("SoftEvtTimer",
                            pdMS_TO_TICKS(period),
                            (isReload) ? pdTRUE : pdFALSE,
                            (void*)EObTim,
                            localSoftTimerCb
                            );

    ASSERT_PARAM(NULL != softTimId);                                
    xTimerStart(softTimId, pdMS_TO_TICKS(100));

   return softTimId;
}

void FreeRTOSClass::removeTimer(EventTick_t EObTim) {
    xEvent *eTimId = (xEvent*)pvTimerGetTimerID(EObTim);

	if (eTimId != NULL) {
		vPortFree(eTimId);
	}
	ASSERT_PARAM(pdPASS == xTimerDelete(eTimId, pdMS_TO_TICKS(100)));
}
