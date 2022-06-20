#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "ak.h"
#include "app.h"

/**
 * @brief Task table variables
 **/
extern Scheduler TableTaskScheduling[];
extern Polling TableTaskPolling[];

/**
 * @brief Scheduler-SCHEDULER-ID
 **/
enum {
	/* SYSTEM TASKS */
	AC_TASK_SYSTEM_ID,
	AC_TASK_CONTROL_ID,

	/* END Scheduler */
	AK_TASK_EOT_ID,
};


/**
 * @brief Scheduler-POLLING-ID
 **/
enum {
	/* APP TASKS */
	AC_TASK_POLLING_SERIAL_ID,

	/* END Scheduler */
	AK_TASK_POLLING_EOT_ID,
};

/**
 * @brief Scheduler function prototypes
 **/
extern void task_system(Ak_Msg_t*);
extern void task_control(Ak_Msg_t*);

extern void task_polling_serial(void);

#endif /*__TASK_LIST_H__ */
