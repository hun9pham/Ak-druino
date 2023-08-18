#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "FAClass.h"
#include "xprintf.h"
#include "app.h"
#include "app_dbg.h"
#include "task_list.h"

#include "sys_dbg.h"
#include "sys_io.h"
#include "sys_cfgCtl.h"

void appStartInit() {
    ///////////////////////////////
    // Software initial
    ///////////////////////////////
    periphsClkStart();
    watchdogInit();
    uartCsInit();
    fatalInit();
    memMangCalc();

    ///////////////////////////////
    // Hardware initial
    ///////////////////////////////
    ledLifeInit();

    ///////////////////////////////
    // User programming
    ///////////////////////////////
    DYNAMO_CYBORG.programInit((xTask_t*)taskList, EOT_TASK_ID);
}
