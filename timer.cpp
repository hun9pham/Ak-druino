#include "ak.h"
#include "task.h"
#include "timer.h"


/**
 ************************
 * @class Timer         *
 ************************
 */
Timer :: Timer(Task &Task_Ref) {
    ENTRY_CRITICAL();

    this->Task_Ref = &Task_Ref;

    this->Task_ID = TASK_TIMER_ID;
    this->pxTimerPool = &this->TimerPool[0];
    this->TimerPoolUsed = 0;
    this->TickCounter = 0;

    /**
     * @brief Init timer pool
     **/
    for (uint8_t iDx = 0; iDx < AK_TIMER_POOL_SIZE - 1; ++iDx) {
        this->TimerPool[iDx].Status = AK_DISABLE;
        this->TimerPool[iDx].pxNextTimer = &this->TimerPool[iDx + 1];
    }
    this->TimerPool[AK_TIMER_POOL_SIZE - 1].Status = AK_DISABLE;
    this->TimerPool[AK_TIMER_POOL_SIZE - 1].pxNextTimer = TIMER_MSG_NULL;

    EXIT_CRITICAL();
}

Ak_Timer_t* Timer :: GetTimerMessage() {
    Ak_Timer_t *_px_allocate_tim;

    ENTRY_CRITICAL();
    _px_allocate_tim = this->pxTimerPool;

    while (_px_allocate_tim != TIMER_MSG_NULL &&
            _px_allocate_tim->Status == AK_ENABLE)
    {
        _px_allocate_tim = _px_allocate_tim->pxNextTimer;        
    }

    /* Pool timer overflow */
    if (_px_allocate_tim == TIMER_MSG_NULL) {
        FATAL("TIM", 0x01);
    }

    ++(this->TimerPoolUsed);

    EXIT_CRITICAL();

    return (_px_allocate_tim);
}

uint8_t Timer :: FreeTimerMessage(TimerSignal_t Signal, Task_ID_t Task_ID) {
    uint8_t _r = TIMER_RET_NG;
    Ak_Timer_t *_px_free_tim_msg;

    ENTRY_CRITICAL();
    _px_free_tim_msg = &this->pxTimerPool[0];

    while (_px_free_tim_msg->Signal != Signal &&
            _px_free_tim_msg->Des_Task_ID != Task_ID)
    {
        _px_free_tim_msg = _px_free_tim_msg->pxNextTimer;        
    }

    /**
     * @brief Timer signal has found
     **/
    if (_px_free_tim_msg != TIMER_MSG_NULL) {
        _px_free_tim_msg->Status = AK_DISABLE;
        _r = TIMER_RET_OK;
    }

    --(this->TimerPoolUsed);
    EXIT_CRITICAL();

    return (_r);
}

void Timer :: SetTimerAttribute(Ak_Timer_t *pxTimerMsg, Task_ID_t Task_ID,
                TimerSignal_t Signal, uint32_t Duty, TimerType_t Type)
{
    pxTimerMsg->Signal = Signal;
    pxTimerMsg->Counter = Duty;
    pxTimerMsg->Des_Task_ID = Task_ID;
    
    switch (Type) {
    case TIMER_ONE_SHOT: {
        pxTimerMsg->Period = 0;
    }
    break;

    case TIMER_PERIODIC: {
        pxTimerMsg->Period = Duty;
    }
    break;
    
    default:
        FATAL("TIM", 0x02);
    break;
    }
}

uint8_t Timer :: TimerSet(Task_ID_t Task_ID, TimerSignal_t Signal, 
                    uint32_t Duty, TimerType_t Type)
{
    Ak_Timer_t *_px_tim_msg;

    ENTRY_CRITICAL();
    _px_tim_msg = &this->pxTimerPool[0];

    while (_px_tim_msg != TIMER_MSG_NULL) {
        if (_px_tim_msg->Des_Task_ID == Task_ID &&
                _px_tim_msg->Signal == Signal)
        {
            _px_tim_msg->Counter = Duty;

            EXIT_CRITICAL();
            return TIMER_RET_OK;
        }
        else {
            _px_tim_msg = _px_tim_msg->pxNextTimer;
        }
    }

    _px_tim_msg = Timer :: GetTimerMessage();
    Timer :: SetTimerAttribute(_px_tim_msg, Task_ID, Signal, Duty, Type);

    /**
     * @brief Enable this timer
     **/
    _px_tim_msg->Status = AK_ENABLE;

    EXIT_CRITICAL();

    return TIMER_RET_OK;
}


/**
 * @brief This must be call in timer interrupt
 **/
void Timer :: Tick(uint16_t _t) {
    this->TickCounter += _t;
}

uint32_t Timer :: GetTick() {
    uint32_t _r = 0;

    ENTRY_CRITICAL();
    _r = this->TickCounter;

    /* Refresh tick value after get tick */
    this->TickCounter = 0;
    EXIT_CRITICAL();

    return (_r);
}

uint8_t Timer :: GetTimerPoolUsed() {
    uint8_t _r = 0;

    ENTRY_CRITICAL();
    _r = this->TimerPoolUsed;
    EXIT_CRITICAL();
    
    return (_r);
}

uint8_t Timer :: GetTimerPoolFree() {
    uint8_t _r = 0;

    ENTRY_CRITICAL();
    _r = AK_TIMER_POOL_SIZE - this->TimerPoolUsed;
    EXIT_CRITICAL();

    return (_r);
}

uint8_t Timer :: GetTimerPoolMax() {
    return (AK_TIMER_POOL_SIZE);
}

uint8_t Timer :: TimerRemoveAttribute(Task_ID_t Task_ID, TimerSignal_t Signal) {
    uint8_t _r = Timer :: FreeTimerMessage(Signal, Task_ID);
    this->Task_Ref->RemoveMessage(Task_ID, Signal);

    return (_r);
}

#include <Arduino.h>

void Timer :: TimeOutCallback() {
    uint32_t _Ticks;

    Ak_Timer_t *_px_timer_pool = &this->pxTimerPool[0];

    ENTRY_CRITICAL();
    _Ticks = Timer :: GetTick();

    while (_px_timer_pool != TIMER_MSG_NULL) {
        if (_px_timer_pool->Status != AK_DISABLE) {
            _px_timer_pool->Counter -= _Ticks;

            if (_px_timer_pool->Counter <= 0) { /* Time-OUT */
                /* Update task current ID */
                {
                    extern Task_ID_t TaskCurrentID; /* From task.h */
                    TaskCurrentID = this->Task_ID;
                }
                
                Task_Ref->PostPureMessage(_px_timer_pool->Des_Task_ID, 
                                                _px_timer_pool->Signal);
                
                /* Check Timer-TYPE */
                if (_px_timer_pool->Period == 0) { /* Type ONE-SHOT */
                    _px_timer_pool->Status = AK_DISABLE;
                    --(this->TimerPoolUsed);
                }
                else { /* Type PERIOD */
                    _px_timer_pool->Counter =_px_timer_pool->Period;
                }
            }
        }
        
        /**
         * @brief Go to next timer  
         **/
        _px_timer_pool = _px_timer_pool->pxNextTimer;
    }

    EXIT_CRITICAL();
}
