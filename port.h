#ifndef __PORT_H__
#define __PORT_H__

#include <stdint.h>

/*
 ***************************************
 *    User message pool size define    *
 ****************************************/
#define AK_PURE_MSG_POOL_SIZE           ( 16 )
#define AK_DYNAMIC_MSG_POOL_SIZE        ( 8 )
#define AK_TIMER_POOL_SIZE		        ( 8 )


/**
 * @brief User must modify in accordance
 * with the used board/chip line
 ******************************************************/
#define DISABLE_IRQ()                   noInterrupts() /* From <Arduino.h> */
#define ENABLE_IRQ()                    interrupts()   /* From <Arduino.h> */
/******************************************************/

#define ENTRY_CRITICAL()                EntryCritital()
#define EXIT_CRITICAL()                 ExitCritital()

/**
 * @brief Function protptypes
 **/
extern void FATAL(const char *s, uint8_t c);
extern void EntryCritital(void);
extern void ExitCritital(void);
extern int GetNestEntry(void);

#endif /* __PORT_H__ */
