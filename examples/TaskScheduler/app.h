#ifndef __APP_H__
#define __APP_H__

#include "ak.h"

/**
 * @brief Hardware IO define
 **/
#define LED_PIN                         ( 13 )


/**
 * @brief SYSTEM task define signal
 **/
enum {
    AC_SYSTEM_DISPLAY_SYSTEM_INFO = AK_USER_DEFINE_SIG,
};

/**
 * @brief LIFE task define signal
 **/
#define AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL		( 1000U )

enum {
    AC_CONTROL_LED_CONFIG = AK_USER_DEFINE_SIG,
    AC_CONTROL_LED_STATUS,
};

/**
 * @brief App function prototypes
 **/
extern void LED_OnBoardConfig(void);
extern void LED_Toggle(void);
extern void LED_On(void);
extern void LED_Off(void);


#endif /* __APP_H__ */
