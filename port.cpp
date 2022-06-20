#include <Arduino.h>
#include <avr/wdt.h>

#include "port.h"
#include "task.h"


static int NestEntryCriticalCounter = 0;

void FATAL(const char *s, uint8_t c) {
	/**
	 * @brief Disable AK_Timer IT
	 **/
	if ((TIMSK1 & 0x01) == 1) {
		TIMSK1 = (0 << TOIE1); /* Overflow interrupt disable */
	}

	/**
	 * @brief DO-NOT disable global interrupt.
	 * This will disable serial print of arduino
	 **/
    Serial.print("FATAL: ");
	Serial.print(String(s));
	Serial.print("  ");
	Serial.println(c);
	Serial.print("Scheduler CURRENT ID [");
	Serial.print(Task :: GetCurrentTaskID());
	Serial.println("]");
    
	while(1) {
		wdt_reset();
	}
}

void EntryCritital() {
    if (NestEntryCriticalCounter == 0) {
		/**
		 * @brief Disable IT
		 */
		 DISABLE_IRQ();
	}
	NestEntryCriticalCounter++;
}

void ExitCritital() {
    --(NestEntryCriticalCounter);
    
	if (NestEntryCriticalCounter == 0) {
        /**
		 * @brief Prevent call exit_critical many times
		 */
		NestEntryCriticalCounter = 0;

		/**
		 * @brief Enable IT
		 */
		ENABLE_IRQ();
	}
	else if (NestEntryCriticalCounter < 0) {
		FATAL("ITR", 0x01);
	}
}

int GetNestEntry() {
	return NestEntryCriticalCounter;
}
