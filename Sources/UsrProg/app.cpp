#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "frtosa.h"
#include "xprintf.h"
#include "app.h"
#include "app_dbg.h"
#include "task_list.h"

#include "sys_dbg.h"
#include "sys_io.h"
#include "sys_cfgCtl.h"

static void softInit() {
    periphsClkStart();
    watchdogInit();
    UartCsInit();
    fatalInit();
    memMangCalc();
}

static void hardInit() {
    ledLifeInit();
}

void appStartInit() {
    softInit();
    hardInit();
    DYNAMO_CYBORG.programInit((xTask_t*)taskList, EOT_TASK_ID);
}
