#include <stdio.h>

#include "ak.h"
#include "task.h"
#include "message.h"
#include "timer.h"

/**
 * @brief Variables
 **/
Task_ID_t TaskCurrentID = 0;
static Ak_Msg_t SignalCurrent;

/**
 ************************
 * @class Scheduler     *
 *************************/
Scheduler :: Scheduler(Task_ID_t ID, TaskPriority_t Prio, 
                            _pf_scheduling _scheduling)
{
    this->ID = ID;
    this->Prio = Prio;
    this->Status = INACTIVE;
    this->_scheduling = _scheduling;

    pxNextTask = SCHEDULING_NULL;
    this->TaskQueue.px_msg_head = this->TaskQueue.px_msg_tail = AK_MSG_NULL;
}

Task_ID_t Scheduler :: GetTaskID() {
    return (this->ID);
}

TaskPriority_t Scheduler :: GetTaskPriority() {
    return (this->Prio);
}

TaskStatus_t Scheduler :: GetTaskStatus() {
    return (this->Status);
}

_pf_scheduling Scheduler :: GetTaskExecution() {
    return (this->_scheduling);
}

void Scheduler :: UpdateTaskStatus(TaskStatus_t NewState) {
    this->Status = NewState;
}

void Scheduler :: PutMessageToTaskQueue(Ak_Msg_t *pxMsg) {
    /**
     * @brief Put message to task queue
     **/
    if (this->TaskQueue.px_msg_tail == AK_MSG_NULL) {
        this->TaskQueue.px_msg_head =pxMsg;
        this->TaskQueue.px_msg_tail = pxMsg;
    }
    else {
        this->TaskQueue.px_msg_tail->pxNext = pxMsg;
        this->TaskQueue.px_msg_tail = pxMsg;
    }
}

void Scheduler :: TaskSchedulerExecution(Ak_Msg_t *pxMsg) {
    this->_scheduling(pxMsg);
}

/**
 ************************
 * @class Polling       *
 ************************
 */
Polling :: Polling(Task_ID_t ID, uint8_t Ability, 
                    _pf_polling _polling)
{
    this->ID = ID;
    this->Ability = Ability;
    this->_polling = _polling;
}

Task_ID_t Polling :: GetTaskPollingID() {
    return (this->ID);
}

uint8_t Polling ::  GetTaskPollingAbility() {
    return (this->Ability);
}

_pf_polling Polling :: GetTaskPollingExecution() {
    return (this->_polling);
}

void Polling :: SetAbility(uint8_t Ability) {
    this->Ability = Ability;
}

void Polling :: TaskPollingExecution() {
    this->_polling();
}

/**
 ************************
 * @class Task          *
 ************************
 */
Task :: Task(Message &Message_Ref, Scheduler *pxSchedulerTable, 
									Polling *pxPollingTable)
{   
    uint8_t _size_of_table = 0;

    this->Message_Ref = &Message_Ref;
    this->SchedulingQueue.px_task_head = this->SchedulingQueue.px_task_tail = SCHEDULING_NULL;

    /**
     * @brief Init each of scheduler task in table
     **/
    {
        this->pxSchedulerTable = pxSchedulerTable;
        
        while (this->pxSchedulerTable[_size_of_table].GetTaskExecution() != (_pf_scheduling)0) {
            ++(_size_of_table);
        }
        
        this->SchedulerTableSize = _size_of_table;
    }

    _size_of_table = 0;

    /**
     * @brief Init each of polling task in table
     **/
    {
        this->pxPollingTable = pxPollingTable;

        while (this->pxPollingTable[_size_of_table].GetTaskPollingExecution() != (_pf_polling)0) {
            ++(_size_of_table);
        }
        
        this->PollingTableSize = _size_of_table;
    }
}

uint8_t Task :: GetSchedulerTableSize() {
    return (this->SchedulerTableSize);
}

uint8_t Task :: GetPollingTableSize() {
    return (this->PollingTableSize);
}

void Task :: SchedulerRun() {
    Scheduler *_px_task;
    Ak_Msg_t *_px_msg = AK_MSG_NULL;

    ENTRY_CRITICAL();

    _px_task = this->SchedulingQueue.px_task_head;

    while (_px_task != SCHEDULING_NULL) {
        /**
         * @brief Point to next task
         **/
        this->SchedulingQueue.px_task_head = this->SchedulingQueue.px_task_head->pxNextTask;
        
        /* Update current task */
        TaskCurrentID = _px_task->GetTaskID();

        /**
         * @brief Execute task until message of task queue runs out
         **/
        while (_px_task->GetTaskStatus() == ACTIVE) {   
            if (_px_task->TaskQueue.px_msg_head == AK_MSG_NULL) {
                /**
                 * @brief Disable task
                 **/
                _px_task->UpdateTaskStatus( INACTIVE );
                _px_task->TaskQueue.px_msg_tail = AK_MSG_NULL;
                _px_msg = AK_MSG_NULL;
        
                break;
            }

            _px_msg = _px_task->TaskQueue.px_msg_head;
            
            /* Update current message */
            SignalCurrent = (*_px_msg);

            EXIT_CRITICAL();

            /**
             * @brief Execute task
             **/
            _px_task->TaskSchedulerExecution( _px_msg );

            ENTRY_CRITICAL();
            _px_task->TaskQueue.px_msg_head = _px_task->TaskQueue.px_msg_head->pxNext;

            /**
             * @brief Free message
             **/
            _px_msg->pxNext = AK_MSG_NULL;
            this->Message_Ref->Free( _px_msg );
        }

        /**
         * @brief Next task execution
         **/
        _px_task->pxNextTask = SCHEDULING_NULL;
        _px_task = this->SchedulingQueue.px_task_head;
    }

    EXIT_CRITICAL();
}

void Task :: PollingRun() {
    uint8_t iDx = 0;

    Polling *_px_task_poll = &this->pxPollingTable[iDx];

    while (iDx < this->PollingTableSize) {
        ENTRY_CRITICAL();

        if (_px_task_poll[iDx].GetTaskPollingAbility() == AK_ENABLE) {
            EXIT_CRITICAL();

            /**
             * @brief Execute task
             **/
            _px_task_poll[iDx].TaskPollingExecution();
        }
        else {
            EXIT_CRITICAL();
        }

        ++(iDx);
    }
}

int8_t Task :: Run(void *Timer_Ref) {
    for (;;) {
        Task :: SchedulerRun();
        Task :: PollingRun();

        /**
         * @brief Timer system post message
         **/
        if (Timer_Ref != NULL) {
            ((Timer *)Timer_Ref)->TimeOutCallback();
        }
    }
    
    return 0;
}

/**
 * @brief Put task to scheduler queue
 **/
void Task :: Post(Task_ID_t Task_ID) {
    if (Task_ID > this->SchedulerTableSize) {
        FATAL("TK", 0x02);
    }

    Scheduler *_px_task_insert = &this->pxSchedulerTable[Task_ID];
    TaskPriority_t _task_insert_prio = _px_task_insert->GetTaskPriority();

    Scheduler *_px_traversing = this->SchedulingQueue.px_task_head;
    Scheduler *_px_previous = SCHEDULING_NULL;

    if (_px_task_insert->GetTaskStatus() == ACTIVE) {
        FATAL("TK", 0x03);
    }

    ENTRY_CRITICAL();

    if (this->SchedulingQueue.px_task_head == SCHEDULING_NULL) {
        this->SchedulingQueue.px_task_head = _px_task_insert;
        _px_task_insert->pxNextTask = this->SchedulingQueue.px_task_tail;
    }
    else {
        while (_px_traversing != SCHEDULING_NULL &&
                _px_traversing->GetTaskPriority() >= _task_insert_prio)
        {
            _px_previous = _px_traversing;
            _px_traversing = _px_traversing->pxNextTask;
        }
        
        if (_px_previous != SCHEDULING_NULL) {
            if (_px_traversing == SCHEDULING_NULL) {
                _px_previous->pxNextTask = _px_task_insert;
                _px_task_insert->pxNextTask = this->SchedulingQueue.px_task_tail;
            }
            else {
                _px_previous->pxNextTask = _px_task_insert;
                _px_task_insert->pxNextTask = _px_traversing;
            }
        }
        else {
            _px_task_insert->pxNextTask = this->SchedulingQueue.px_task_head;
            this->SchedulingQueue.px_task_head = _px_task_insert;
        }
    }

    /* Update task status */
    _px_task_insert->UpdateTaskStatus( ACTIVE );

    EXIT_CRITICAL();
}

void Task :: PostPureMessage(Task_ID_t Des_Task_ID, uint8_t Signal) {
    Scheduler *_px_task_posted = &this->pxSchedulerTable[Des_Task_ID];
    Ak_Msg_t *_px_pure_msg = AK_MSG_NULL;

    ENTRY_CRITICAL();

    if (_px_task_posted->GetTaskStatus() == INACTIVE) {
        /* Put task to scheduler queue */
        Task :: Post( Des_Task_ID );
    }

    /* Setup message */
    _px_pure_msg = this->Message_Ref->GetPureMessage();
    this->Message_Ref->SetMessageInformation(_px_pure_msg, Des_Task_ID, Signal);

    /* Put message to task queue */
    _px_task_posted->PutMessageToTaskQueue(_px_pure_msg);

    EXIT_CRITICAL();
}

void Task :: PostDynamicMessage(Task_ID_t Des_Task_ID, uint8_t Signal, 
                                    uint8_t *pxData, uint16_t Len)
{
    Scheduler *_px_task_posted = &this->pxSchedulerTable[Des_Task_ID];
    Ak_Msg_t *_px_dynamic_msg = AK_MSG_NULL;

    ENTRY_CRITICAL();

    if (_px_task_posted->GetTaskStatus() == INACTIVE) {
        /* Put task to scheduler queue */
        Task :: Post(Des_Task_ID);
    }

    /* Setup message */
    _px_dynamic_msg = this->Message_Ref->GetDynamicMessage();
    this->Message_Ref->SetMessageInformation(_px_dynamic_msg, Des_Task_ID, Signal);
    this->Message_Ref->SetDataDynamicMessage(_px_dynamic_msg, pxData, Len);

    /* Put message to task queue */
    _px_task_posted->PutMessageToTaskQueue(_px_dynamic_msg);

    EXIT_CRITICAL();
}

void Task :: SetPollingAbility(Task_ID_t Task_ID, uint8_t Ability) {
    this->pxPollingTable[Task_ID].SetAbility(Ability);
}

void Task :: RemoveMessage(Task_ID_t Task_ID, uint8_t Signal) {
    Ak_Msg_t *_px_msg_delete = AK_MSG_NULL;
    Scheduler *_px_task_queue = &this->pxSchedulerTable[Task_ID];
    Ak_Msg_t *_px_msg_traversing = _px_task_queue->TaskQueue.px_msg_head;
    Ak_Msg_t *_px_msg_previous = AK_MSG_NULL;

    ENTRY_CRITICAL();
    while (_px_msg_traversing != AK_MSG_NULL &&
            _px_msg_traversing->Signal != Signal)
    {
        _px_msg_previous = _px_msg_traversing;
        _px_msg_traversing = _px_msg_traversing->pxNext;
    }
    
    /* Signal has found */ 
    if (_px_msg_traversing != AK_MSG_NULL) {
        _px_msg_delete = _px_msg_traversing;

        if (_px_msg_previous != AK_MSG_NULL) {
            _px_msg_previous->pxNext = _px_msg_traversing->pxNext;
            _px_msg_traversing->pxNext = AK_MSG_NULL;

            /* Update tail of task queue message */
            if (_px_msg_previous->pxNext == AK_MSG_NULL) {
                _px_task_queue->TaskQueue.px_msg_tail = _px_msg_previous;
            }
        }
        else {
            /* Update head of task queue message */
            _px_task_queue->TaskQueue.px_msg_head = _px_msg_traversing->pxNext;
        }
    }

    /* Delete message if it's found */
    if (_px_msg_delete != AK_MSG_NULL) {
        this->Message_Ref->Free(_px_msg_delete);
    }

    /* Refresh task queue if no message exist */
    if (_px_task_queue->TaskQueue.px_msg_head == AK_MSG_NULL) {
        _px_task_queue->TaskQueue.px_msg_tail = AK_MSG_NULL;
    }

    EXIT_CRITICAL();
}

Task_ID_t  Task :: GetCurrentTaskID() {
    return TaskCurrentID;
}

Ak_Msg_t Task :: GetCurrentMessage() {
    return SignalCurrent;
}
