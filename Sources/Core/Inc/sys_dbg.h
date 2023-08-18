#ifndef __SYS_DBG_H
#define __SYS_DBG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "xprintf.h"

#define SYS_PRINT 			xprintf

#define FATAL_LOG_FLASH_ADDRESS	0
#define FATAL_LOG_MAGIC_NUMBER  (0xFFAAFAFA)

typedef struct t_fatalLog {
	uint32_t magicNumber;
	uint32_t restartTimes;
	char string[16];
	char rstReason[16];
} fatalLog_t;

extern void fatalInit();
extern uint8_t fatalClear();
extern void fatal(const char *s);

#define FATAL(s)		fatal(s)

#ifdef __cplusplus
}
#endif

#endif