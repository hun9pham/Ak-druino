#include "task_list.h"
#include "task.h"


Scheduler TableTaskScheduling[] = {
	/**
	 * @brief Scheduler-SCHEDULING
	 **/
	{ AC_TASK_SYSTEM_ID, 	TASK_PRI_LEVEL_2,  task_system 	},
	{ AC_TASK_CONTROL_ID,   TASK_PRI_LEVEL_3,  task_control },

	/**
	 * @brief End of table
	 **/
	{  AK_TASK_EOT_ID,	 TASK_PRI_LEVEL_0, (_pf_scheduling)0 }
};


Polling TableTaskPolling[] = {
	/**
	 * @brief Scheduler-POLLING
	 **/
	{ AC_TASK_POLLING_SERIAL_ID, AK_ENABLE,	task_polling_serial },

	/**
	 * @brief End of table
	 **/
	{ AK_TASK_POLLING_EOT_ID, AK_DISABLE, (_pf_polling)0 }
};
