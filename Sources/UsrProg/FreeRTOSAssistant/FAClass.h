#ifndef __fa_H
#define __fa_H

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

////////////////////////////////////////////
// PUBLIC DEFINITIONS
///////////////////////////////////////////
#define fa_RELEASE				(0)
#define fa_CYBORG				"Dynamo"

#define faMAILBOX_SIZE			(6)
#define faUSER_DEFINE_SIGNAL	(10)
#define faSTACK_SIZE_UNIT		(sizeof(uint32_t))

#define faENTRY_ATOMIC()		portDISABLE_INTERRUPTS()
#define faEXIT_ATOMIC()			portENABLE_INTERRUPTS()

/* Enumeration ---------------------------------------------------------------*/
typedef enum eTaskRank {
	BRONZE = 1,
	SILVER,
	GOLD,
	PLANTINUM,
	DIAMOND,
} taskRank_t;

typedef enum eStackSize {
	STACK_SZ_MIN = 128,
	STACK_SZ_MEDIUM = 256,
	STACK_SZ_HIGH = 1024,
	STACK_SZ_HYPER = 2048,
	STACK_SZ_MAX = 4096,
} stackSize_t;

typedef QueueHandle_t MailBox_t;
typedef TimerHandle_t EventTick_t;

typedef struct {
	uint8_t destTaskId;
	uint8_t Signal;
	uint8_t *Payload;
	uint32_t Size;
} xEvent_t;

typedef struct {
	uint8_t tId;
	void (*funcCallback)(void *);
	stackSize_t sizeOfStack; /* eStackSize * faSTACK_SIZE_UNIT */
	taskRank_t Rank;
	const char *const Desc;
	QueueHandle_t EventQueue;
} xTask_t;

typedef struct {
	xTask_t *taskTbl;
	uint16_t taskTblSize;
} xTaskManager_t;

class FreeRTOSClass {
private:
	xTaskManager_t taskManager;
	SemaphoreHandle_t lcSemaphore = NULL;

	static void bannerIntro();
public:
	FreeRTOSClass();
	~FreeRTOSClass();
	void programInit(xTask_t *taskTbl, uint8_t taskTblSize);
	void syncAllTasksStarted(const char *taskEntry);
	void activeEvent(xEvent_t *evt);
	void postPureMsg(uint8_t destTaskId, uint8_t Sig);
	void postDynamicMsg(uint8_t destTaskId, uint8_t Sig, uint8_t *data, uint32_t len);
	void getMsgFrom(uint8_t tId, xEvent_t *evt);
	void freeMsg(xEvent_t *evt);

	EventTick_t setTimer(uint8_t destTaskId, uint8_t Sig, uint32_t period, bool isReload);
	void removeTimer(EventTick_t evtTim);
};

extern FreeRTOSClass DYNAMO_CYBORG;

extern void fa_ParamsTRUE(bool isTRUE, const char *file, uint32_t line);

#define faASSERT(assert)    fa_ParamsTRUE(assert, __FILE__, __LINE__)

////////////////////////////////////////////
// COLORATIONS
///////////////////////////////////////////
#define faRST	   	"\e[0m"
#define faGRN		"\e[0;32m"
#define faKNRM  	"\x1B[0m"
#define faKRED  	"\x1B[31m"
#define faKYEL  	"\x1B[33m"
#define faKMAG  	"\x1B[35m"
#define faKCYN  	"\e[0;36m"
#define faKWHT  	"\x1B[37m"

#ifdef __cplusplus
}
#endif

#endif /* __fa_H */
