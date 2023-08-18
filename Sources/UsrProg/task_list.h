#ifndef __TASK_LIST_H
#define __TASK_LIST_H

#include "FAClass.h"

#define TASK_SYSTEM_EN      (1)
#define TASK_CONSOLE_EN     (0)
#define TASK_DEVMANAGER_EN  (1)

enum {
#if TASK_SYSTEM_EN
    TASK_SYSTEM_ID,
#endif
#if TASK_CONSOLE_EN
    TASK_CONSOLE_ID,
#endif
#if TASK_DEVMANAGER_EN
    TASK_DEVMANAGER_ID,
#endif

    EOT_TASK_ID,
};

extern xTask_t taskList[];

#endif