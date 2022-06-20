#ifndef __TASK_H__
#define __TASK_H__


#include <stdint.h>
#include <stdbool.h>

#include "message.h"

/*========================================================================
 * #### Task define  ####
 =========================================================================*/
#define SCHEDULING_NULL				( (Scheduler *)0 )

typedef void (*_pf_scheduling)(Ak_Msg_t *);
typedef void (*_pf_polling)(void);

typedef uint8_t	Task_ID_t;

typedef enum TASK_STATUS {
	INACTIVE = 0,
	ACTIVE = !INACTIVE,
} TaskStatus_t;

typedef enum TASK_PRIORITY {
	TASK_PRI_LEVEL_0 = 0,
	TASK_PRI_LEVEL_1,
	TASK_PRI_LEVEL_2,
	TASK_PRI_LEVEL_3,
} TaskPriority_t;

typedef struct TASK_QUEUE_MESSAGE {
	Ak_Msg_t *px_msg_head;
	Ak_Msg_t *px_msg_tail;
} QueueMessage_t;

/**
 *************************
 * @class Scheduler		 *
 *************************/
class Scheduler {
private:
	/* Task info */
	Task_ID_t ID;
	TaskPriority_t Prio;
	TaskStatus_t Status;
	_pf_scheduling _scheduling;
public:
	/* Message queue of task scheduler */
	QueueMessage_t TaskQueue;

	/* Pointer to next task execution */
	Scheduler *pxNextTask;

	Scheduler(Task_ID_t ID, TaskPriority_t Prio, _pf_scheduling _scheduling);
	Task_ID_t GetTaskID(void);
	TaskPriority_t GetTaskPriority(void);
	_pf_scheduling GetTaskExecution(void);
	TaskStatus_t GetTaskStatus(void);
	void UpdateTaskStatus(TaskStatus_t NewState);
	void PutMessageToTaskQueue(Ak_Msg_t *pxMsg);
	void TaskSchedulerExecution(Ak_Msg_t *pxMsg);
};

/**
 *************************
 * @class Polling		 *
 *************************/
class Polling {
private:
	Task_ID_t ID;
	uint8_t Ability;
	_pf_polling _polling;

public:
	Polling(Task_ID_t ID, uint8_t Ability, _pf_polling _polling);
	Task_ID_t GetTaskPollingID(void);
	uint8_t GetTaskPollingAbility(void);
	_pf_polling GetTaskPollingExecution(void);
	void SetAbility(uint8_t Ability);
	void TaskPollingExecution(void);
};

/**
 * @brief Scheduling queue of task scheduler
 **/
typedef struct SCHEDULING_QUEUE {
	Scheduler *px_task_head;
	Scheduler *px_task_tail;
} SchedulingQueue_t;


/**
 *************************
 * @class Task		     *
 *************************/
class Task {
private:
	/**
	 * @brief Reference to message pool
	 **/
	Message *Message_Ref;

	/**
	 * @brief Task manager
	 **/
	Scheduler *pxSchedulerTable;
	uint8_t SchedulerTableSize;

	Polling *pxPollingTable;
	uint8_t PollingTableSize;

	/**
	 * @brief Queue of scheduler task
	 **/
	SchedulingQueue_t SchedulingQueue;

	/**
	 * @brief Private function task run
	 **/
	void SchedulerRun(void);
	void PollingRun(void);
public:
	Task(Message &Message_Ref, Scheduler *pxSchedulerTable, 
									Polling *pxPollingTable);
	uint8_t GetSchedulerTableSize(void);
	uint8_t GetPollingTableSize(void);

	int8_t Run(void *Timer_Ref);
	void Post(Task_ID_t Task_ID);
	void PostPureMessage(Task_ID_t Des_Task_ID, uint8_t Signal);
	void PostDynamicMessage(Task_ID_t Des_Task_ID, uint8_t Signal, 
										uint8_t *pxData, uint16_t Len);
	void SetPollingAbility(Task_ID_t Task_ID, uint8_t Ability);
	void RemoveMessage(Task_ID_t Task_ID, uint8_t Signal);

	static Task_ID_t GetCurrentTaskID(void);
	static Ak_Msg_t GetCurrentMessage(void);
};

/**
 * @brief Extern variables
 **/
extern Task_ID_t TaskCurrentID;


#endif /* __TASK_H__ */
