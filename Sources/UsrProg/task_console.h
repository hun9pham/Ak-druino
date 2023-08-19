#ifndef __TASK_CONSOLE_H
#define __TASK_CONSOLE_H

#include "cmd_line.h"

#define RX_QUEUE_SIZE	(16)

extern const cmdLineStruct_t lgnCmdTable[];
extern TaskHandle_t CreateConsole;

extern void taskConsoleCb(void *);

#endif /* __TASK_CONSOLE_H */