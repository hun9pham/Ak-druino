#include <Arduino.h>

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


void task_system(Ak_Msg_t* pxMsg) {
	switch (pxMsg->Signal) {
		case AC_SYSTEM_DISPLAY_SYSTEM_INFO: {
			Serial.println("");
			Serial.println("   __    _  _ ");
			Serial.println("  /__\\  ( )/ )");
			Serial.println(" /(__)\\ (   ( ");
			Serial.println("(__)(__)(_)\\_)");
			Serial.println("Wellcome to Active Kernel");

			Serial.print("\tPURE MESSAGE POOL: ");
			Serial.println(EventSignal.GetPureMessagePoolMax());

			Serial.print("\tDYNAMIC MESSAGE POOL: ");
			Serial.println(EventSignal.GetDynamicMessagePoolMax());

			Serial.print("\tTIMER MESSAGE POOL: ");
			Serial.println(TimerPost.GetTimerPoolMax());

			/**
			 * @brief Timer period post message to task_life
			 **/
			TimerPost.TimerSet(AC_TASK_LIFE_ID, AC_LIFE_SYSTEM_CHECK, 
								AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL, 
								TIMER_PERIODIC
							);
		}
		break;

		default:
		break;
	}
}
