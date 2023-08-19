#ifndef __FreeRTOS_ASSISTANT_H
#define __FreeRTOS_ASSISTANT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "message_buffer.h"
#include "task.h"
#include "event_groups.h"

#include "eventbox.h"

#define BUILD_RELEASE			(0)
#define CYBORG					"Dynamo"

#if BUILD_RELEASE
    #define BUILD   (const char*)"RELEASE"
    #define VERSION	(const char*)"v1.1"
    #define ASSERT	(0)
#else
    #define BUILD   (const char*)"DEBUG"
    #define VERSION (const char*)"v1.0"
    #define ASSERT	(1)
#endif

#define USER_DEFINE_SIGNAL		(10)

#define ENTRY_CRITICAL()		portDISABLE_INTERRUPTS()
#define EXIT_CRITICAL()			portENABLE_INTERRUPTS()

typedef enum {
	BRONZE = 1,
	SILVER,
	GOLD,
	PLANTINUM,
	DIAMOND,
} eTaskRank;

typedef enum {
	STACK_SZ_MIN = 128,
	STACK_SZ_MEDIUM = 256,
	STACK_SZ_HIGH = 1024,
	STACK_SZ_HYPER = 2048,
	STACK_SZ_MAX = 4096,
} eStackSize;

typedef TimerHandle_t EventTick_t;

typedef struct {
	uint8_t tId;
	void (*funcCallback)(void *);
	eStackSize sizeOfStack; /* eStackSize * faSTACK_SIZE_UNIT */
	eTaskRank Rank;
	const char *const Desc;
	TaskHandle_t CreateTask;
	xEvent *EOb;
} xTask_t;

typedef struct {
	xTask_t *taskTbl;
	uint16_t taskTblSize;
} xTaskManager_t;

class FreeRTOSClass {
private:
	xTaskManager_t m_TaskManager;
	EventBox m_rtosEvent;
	SemaphoreHandle_t m_Semaphore = NULL;

	static void bannerIntro();
public:
	FreeRTOSClass();
	~FreeRTOSClass();
	void programInit(xTask_t *taskTbl, uint8_t taskTblSize);
	void syncAllTasksStarted();
	void activeEOb(xEvent*);
	void postMsg(uint8_t destTaskId, uint8_t Sig, uint8_t *data=NULL, uint32_t len=0);
	bool getMsgFrom(uint8_t tId, xEvent *EOb);
	void watiForSignal(uint8_t taskIdWait);
	EventTick_t setTimer(uint8_t destTaskId, uint8_t Sig, uint32_t period, bool isReload);
	void removeTimer(EventTick_t evtTim);
};

extern FreeRTOSClass DYNAMO_CYBORG;

extern void assertParams(bool IsTrue, const char *file, uint32_t line);

#define ASSERT_PARAM(assert)    assertParams(assert, __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif /* __FreeRTOS_ASSISTANT_H */
