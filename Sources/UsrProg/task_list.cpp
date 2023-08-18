#include "task_list.h"

#include "task_system.h"
#include "task_console.h"
#include "task_devManager.h"

xTask_t taskList[] = {
    /////////////////////////
    // TASK SYSTEM
    ////////////////////////
#if TASK_SYSTEM_EN
    {
        TASK_SYSTEM_ID,
        taskSystemCb,
        STACK_SZ_MIN,
        DIAMOND,
        "TaskSystem",
        NULL
    },
#endif
    /////////////////////////
    // TASK CONSOLE
    ////////////////////////
#if TASK_CONSOLE_EN
	{	
        TASK_CONSOLE_ID,
        taskConsoleCb,
        STACK_SZ_MIN,
        GOLD,
        "TaskConsole",
        NULL 
    }
#endif
    /////////////////////////
    // TASK DEVICE MANAGER
    ////////////////////////
#if TASK_DEVMANAGER_EN
	{	
        TASK_DEVMANAGER_ID,
        taskDevManagerCb,
        STACK_SZ_MIN,
        GOLD,
        "TaskDevManager",
        NULL 
    }
#endif
};