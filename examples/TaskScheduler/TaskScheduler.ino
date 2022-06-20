/* Arduino header */
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

/* AK header */
#include "ak.h"
#include "port.h"

/* App header */
#include "app.h"
#include "task_list.h"

/**
 * @brief Global variables
 **/
Message EventSignal;
Task Tasks(EventSignal, TableTaskScheduling, TableTaskPolling);


void setup() {
    ENTRY_CRITICAL();
    /**
     * @brief Software setup
     **/
    {
        Serial.begin( 9600 ); /* Serial monitor */
    }

    /**
     * @brief Hardware setup
     **/
    {

    }

    /**
     * @brief Scheduler post message setup
     **/
    {
        Tasks.PostPureMessage(AC_TASK_SYSTEM_ID, AC_SYSTEM_DISPLAY_SYSTEM_INFO);
        Tasks.PostPureMessage(AC_TASK_CONTROL_ID, AC_CONTROL_LED_CONFIG);
    }

    EXIT_CRITICAL();
}

void loop() {
    /* Waiting 1.5 second for system ready */
    delay ( 1500 );

    /**
    * @brief Active kernel
    * * @note If not using timer, param for Task :: Run() MUST-BE as NULL
    **/
    Tasks.Run( NULL );
}


/**
 * @brief LED on board function
 **/
void LED_OnBoardConfig() {
    pinMode(LED_PIN, OUTPUT);
}

void LED_Toggle() {
    static uint8_t _led_state = 0;

    if (_led_state == 0) {
        digitalWrite(LED_PIN, HIGH);
        _led_state = 1;
    }
    else {
        digitalWrite(LED_PIN, LOW);
        _led_state = 0;
    }
}

void LED_On() {
    digitalWrite(LED_PIN, HIGH);
}

void LED_Off() {
    digitalWrite(LED_PIN, LOW);
}
