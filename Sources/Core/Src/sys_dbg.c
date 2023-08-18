#include "sys_dbg.h"
#include "sys_io.h"
#include "sys_cfgCtl.h"

// static fatalLog_t fatalLog;

void fatalInit() {
	char *reason = getRstReason(true);
	SYS_PRINT("\r\nReset Reason: %s\r\n", reason);

	// flashRead(FATAL_LOG_FLASH_ADDRESS, (uint8_t*)&fatalLog, sizeof(fatalLog));
	// if (fatalLog.magicNumber != (uint32_t)FATAL_LOG_MAGIC_NUMBER) {
	// 	memset(&fatalLog, 0, sizeof(fatalLog));
	// 	fatalLog.magicNumber = FATAL_LOG_MAGIC_NUMBER;
	// }
	// else {
	// 	++(fatalLog.restartTimes);
	// }

	// strcpy(fatalLog.rstReason, reason);
	// flashEraseSector(FATAL_LOG_FLASH_ADDRESS);
	// flashWrite(FATAL_LOG_FLASH_ADDRESS, (uint8_t*)&fatalLog, sizeof(fatalLog));
}

uint8_t fatalClear() {
	// memset(&fatalLog, 0, sizeof(fatalLog_t));
	// return flashWrite(FATAL_LOG_FLASH_ADDRESS, (uint8_t*) &fatalLog, sizeof(fatalLog));
    return 0;
}

void fatal(const char *s) {
	__disable_irq();
	watchdogRst();

	SYS_PRINT("\r\nFATAL: %s\r\n", s);

// 	flashRead(FATAL_LOG_FLASH_ADDRESS, (uint8_t*)&fatalLog, sizeof(fatalLog));
// 	strcpy(fatalLog.string, s);
// 	flashEraseSector(FATAL_LOG_FLASH_ADDRESS);
// 	flashWrite(FATAL_LOG_FLASH_ADDRESS, (uint8_t*)&fatalLog, sizeof(fatalLog));

#ifdef RELEASE
	delayMillis(100);
	softReset();
#else
	while (1) {
		blinkLedLife();
		watchdogRst();
		delayMs(50);
	}
#endif
}

