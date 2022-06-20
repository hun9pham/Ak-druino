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


Message EventSignal;
Task Tasks(EventSignal, TableTaskScheduling, TableTaskPolling);
Timer TimerPost( Tasks );


void setup() {
    ENTRY_CRITICAL();

    /**
     * @brief Software setup
     **/
    Serial.begin( 9600 ); /* Serial monitor */
    TimerITConfig(); /* Timer period interrupt 1 millisecond */
    wdt_enable( WDTO_8S ); /* Watchdog 8s */

    /**
     * @brief Hardware setup
     **/
    LED_OnBoardConfig();

    /**
     * @brief Scheduler setup
     **/
    {
        TimerPost.TimerSet(AC_TASK_SYSTEM_ID, AC_SYSTEM_DISPLAY_SYSTEM_INFO, 
                            AC_SYSTEM_TASK_TIMER_DISPLAY_INFO_ONE_TIME, 
                            TIMER_ONE_SHOT
                        );
    }

    EXIT_CRITICAL();
}

void loop() {
    /* Waiting 1.5 second for system ready */
    delay ( 1500 );
    /**
    * @brief Active kernel
    * * @note 
    **/
    Tasks.Run( &TimerPost );
}

/**
 * @brief Timer IT Handle
 **/
ISR (TIMER1_OVF_vect) {
    /* Reload value timer counter */
    TCNT1 = 65285;

    /**
     * @brief Active kernel timing
     **/
    TimerPost.Tick( 1 );
}

/**
 * @brief Config arduino timer 
 * interupt after 1 milliseconds
 **/
void TimerITConfig() {
    /**
     * @brief Reset Timer/Counter1
     **/
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    
    /**
     * @brief Setup Timer/Counter1
     * @calc:
     * * Prescale: 64
     * * Atmega328P freq: 16MHZ / Prescale = 16MHz / 64 = 250KHz
     * * Atmega328P period = 1 / 250KHz = 4us
     * * Period intterupt need: 1ms -> 1ms / 4us = 250
     * * -> Reload = (65535 - 250) = 65285
     **/
    TCCR1B |= (1 << CS11) | (1 << CS10); /* Prescale = 64 */
    TCNT1 = 65285;

    /* Overflow interrupt enable */
    TIMSK1 = (1 << TOIE1);
}

/**
 * @brief LED on board
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
