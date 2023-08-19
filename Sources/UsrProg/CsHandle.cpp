#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "app.h"
#include "task_list.h"
#include "task_console.h"

#include "sys_io.h"
#include "sys_cfgCtl.h"

#include "cmd_line.h"

static int8_t csInfo(uint8_t* argv);
static int8_t csHelp(uint8_t* argv);
static int8_t csRst(uint8_t* argv);
static int8_t csFatal(uint8_t* argv);

const cmdLineStruct_t lgnCmdTable[] = {
	{(const int8_t*)"info",		csInfo,	    	(const int8_t*)"System information"	},
	{(const int8_t*)"help",		csHelp,			(const int8_t*)"Help information"	},
	{(const int8_t*)"rst",		csRst,			(const int8_t*)"Reset system"		},
	{(const int8_t*)"fatal",	csFatal,		(const int8_t*)"Fatal information"	},
	{(const int8_t*)0,			(pfCmdFunc)0,	(const int8_t*)0					}
};

int8_t csInfo(uint8_t* argv) {
	(void)argv;

	{



	}

	return 0;
}

int8_t csHelp(uint8_t* argv) {
	(void)argv;

	{



	}

	return 0;
}

int8_t csRst(uint8_t* argv) {
	(void)argv;
	softReset();

	return 0;
}

int8_t csFatal(uint8_t* argv) {
	(void)argv;

	{



	}

	return 0;
}
