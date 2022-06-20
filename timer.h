#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>
#include "task.h"

/*========================================================================
 * #### Timer define  ####
 =========================================================================*/
#define TASK_TIMER_ID               ( 0xEF )

#define TIMER_TICK_SIGNAL           ( 1 )

#define TIMER_MSG_NULL				( (Ak_Timer_t*)0 )
#define TIMER_RET_OK				( 1 )
#define TIMER_RET_NG				( 0 )

/* Size of timer pool */
#ifndef AK_TIMER_POOL_SIZE
#define AK_TIMER_POOL_SIZE			( 8 )
#endif

typedef uint8_t TimerSignal_t;

typedef enum TIMER_TYPE {
	TIMER_ONE_SHOT,
	TIMER_PERIODIC
} TimerType_t;

typedef struct AK_TIMER {
	struct AK_TIMER *pxNextTimer;
	Task_ID_t Des_Task_ID;
	TimerSignal_t Signal;
	int32_t Counter;
	uint32_t Period;
    uint8_t Status;
} Ak_Timer_t;

/**
 *************************
 * @class Timer          *
 *************************/
class Timer {
private:
    /**
     * @brief: Reference to task object
     **/
    Task *Task_Ref;

    /**
     * @brief: Timer attribute
     **/
    Task_ID_t Task_ID;
    Ak_Timer_t TimerPool[AK_TIMER_POOL_SIZE];
    Ak_Timer_t *pxTimerPool;
    uint8_t TimerPoolUsed;
    volatile uint32_t TickCounter;

    /**
     * @brief: Timer private function
     **/
    Ak_Timer_t *GetTimerMessage(void);
    uint8_t FreeTimerMessage(TimerSignal_t Signal, Task_ID_t Task_ID);
    void SetTimerAttribute(Ak_Timer_t *pxTimerMsg, Task_ID_t Task_ID,
                    TimerSignal_t Signal, uint32_t Duty, TimerType_t Type);
public:
    Timer(Task &Task_Ref);
    uint8_t TimerSet(Task_ID_t Task_ID, TimerSignal_t Signal, 
                    uint32_t Duty, TimerType_t Type);
    void Tick(uint16_t _t);
    uint32_t GetTick(void);
    uint8_t GetTimerPoolUsed(void);
    uint8_t GetTimerPoolFree(void);
    uint8_t GetTimerPoolMax(void);
    uint8_t TimerRemoveAttribute(Task_ID_t Task_ID, TimerSignal_t Signal);
    void TimeOutCallback(void);
};


#endif /* __TIMER_H__ */
