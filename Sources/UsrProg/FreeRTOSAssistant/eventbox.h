#ifndef __EVENT_BOX_H
#define __EVENT_BOX_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_EVENT_POOL_SIZE     (16)
#define EVENT_MSG_NULL          (xEvent*)NULL

typedef struct xEvent {
    struct xEvent *link;
    uint8_t destTaskId;
    uint8_t Signal;
    uint8_t *Payload;
    uint32_t Size;
} xEvent;

class EventBox {
private:
    xEvent m_Pools[MAX_EVENT_POOL_SIZE];
    xEvent *m_freePool;
    uint32_t m_nbrPoolFreeSize;

public:
    EventBox();
    ~EventBox();
    void inits();
    xEvent *mallocEvent();
    void freeEvent(xEvent*);
    uint32_t getSizeUsed();
    uint32_t getSizeMax();
};

#endif /* __FRTOS_MAILBOX_H */
