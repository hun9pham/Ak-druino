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

void task_polling_serial() {
    String _cmd = "";
    int8_t _led_state = -1;

    while (Serial.available()) {
        delay( 2 );
        char _char = Serial.read();
        _cmd += _char;

        if (_cmd.equals("LED On")) {
	        _led_state = 1;
        }
        else if (_cmd.equals("LED Off")) {
	        _led_state = 0;
        }

    /**
     * @brief Post data message to task control
     **/
	if (_led_state != -1) {
        Tasks.PostDynamicMessage(AC_TASK_CONTROL_ID, AC_CONTROL_LED_STATUS, 
                	            &_led_state, sizeof(_led_state));
		_led_state = -1;
	}
    }
}
