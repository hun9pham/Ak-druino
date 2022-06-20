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


void task_control(Ak_Msg_t* pxMsg) {
	switch (pxMsg->Signal) {
		case AC_CONTROL_LED_CONFIG: {
			/**
			 * @brief Config LED on board
			 **/
			LED_OnBoardConfig();
		}
		break;

		case AC_CONTROL_LED_STATUS: {
			uint16_t _len_data = EventSignal.GetLengthOfDataDynamicMessage( pxMsg );
			uint8_t *_data = EventSignal.GetDataDynamicMessage( pxMsg );

			Serial.print("Data: ");
			Serial.println( *_data );
			Serial.print("Len of data: ");
			Serial.println( _len_data );			

			if ((*_data) == 1) {
				Serial.println("Control LED-ON");
				LED_On();
			}
			else if ((*_data) == 0) {
				Serial.println("Control LED-OFF");
				LED_Off();
			}
		}
		break;

		default:
		break;
	}
}
