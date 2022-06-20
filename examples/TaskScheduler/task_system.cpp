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


void task_system(Ak_Msg_t* pxMsg) {
	switch (pxMsg->Signal) {
		case AC_SYSTEM_DISPLAY_SYSTEM_INFO: {
			Serial.println("");
			Serial.println("   __    _  _ ");
			Serial.println("  /__\\  ( )/ )");
			Serial.println(" /(__)\\ (   ( ");
			Serial.println("(__)(__)(_)\\_)");
			Serial.println("Wellcome to Active Kernel");

			Serial.print("\tPure message pool: ");
			Serial.println(EventSignal.GetPureMessagePoolMax());

			Serial.print("\tDynamic message pool: ");
			Serial.println(EventSignal.GetDynamicMessagePoolMax());
		}
		break;

		default:
		break;
	}
}
