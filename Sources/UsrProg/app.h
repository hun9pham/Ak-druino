#ifndef __APP_H
#define __APP_H

#ifdef __cplusplus
extern "C"
{
#endif

#define USER_DEFINE_SIGNAL  (10)

/////////////////////////////////////
// TASK SYSTEM SIGNALS
/////////////////////////////////////
#define SYSTEM_ALIVE_NOFITY_INTERVAL        ( 1000 )

enum {
	SYSTEM_PING_ALIVE = USER_DEFINE_SIGNAL,
	SYSTEM_SOFT_REBOOT,
};

/////////////////////////////////////
// TASK CONSOLE SIGNALS
/////////////////////////////////////

/////////////////////////////////////
// TASK DEVICE MANAGER SIGNALS
/////////////////////////////////////
#define DEVMNG_SENSORS_UPDATE_INTERVAL      ( 1000 )

enum {
	DEVMNG_SENSORS_UPDATE = USER_DEFINE_SIGNAL,
};

/////////////////////////////////////
// APPLICATION DEFINITIONS
/////////////////////////////////////
#define HW_VERSION      (const char*)"v2.1"
#define FW_VERSION      (const char*)"v1.0.0"

extern void appStartInit();

#ifdef __cplusplus
}
#endif

#endif