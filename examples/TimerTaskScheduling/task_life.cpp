#include <Arduino.h>
#include <avr/wdt.h>

#include "ak.h"
#include "port.h"

#include "app.h"
#include "task_list.h"


/**
 * @brief Extern variables
 **/
extern Message EventSignal;
extern Task Tasks;
extern Timer TimerPost;


/**
 * @brief This task will be refresh watchdog and toggle led periodically
 **/
void task_life(Ak_Msg_t* pxMsg) {
	switch (pxMsg->Signal) {
		case AC_LIFE_SYSTEM_CHECK: {
			Serial.println("[task_life] AC_LIFE_SYSTEM_CHECK");

			/* Refresh IWDG */
			wdt_reset();
			
			/* Toggle LED to INDICATE */
			LED_Toggle();
		}
		break;

		default:
		break;
	}
}
