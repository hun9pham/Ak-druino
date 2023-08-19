#include "eventbox.h"

EventBox::EventBox() {

}

EventBox::~EventBox() {

}

void EventBox::inits() {
    uint8_t ind;

    m_freePool = (xEvent*)m_Pools;

    for (ind = 0; ind < MAX_EVENT_POOL_SIZE; ind++) {
        m_Pools[ind].link = &m_Pools[ind + 1];
    }
    m_Pools[MAX_EVENT_POOL_SIZE - 1].link = EVENT_MSG_NULL;

    m_nbrPoolFreeSize = MAX_EVENT_POOL_SIZE;
}

xEvent* EventBox::mallocEvent() {
    xEvent *ret;

	ret = m_freePool;

	if (ret != EVENT_MSG_NULL) {
        m_freePool = ret->link;
        --m_nbrPoolFreeSize;
        ret->link = EVENT_MSG_NULL;
    }
  
	return ret;
}

void EventBox::freeEvent(xEvent *evt) {
    evt->link = m_freePool;
    m_freePool = evt;
    ++m_nbrPoolFreeSize;
}

uint32_t EventBox::getSizeUsed() {
    return MAX_EVENT_POOL_SIZE - m_nbrPoolFreeSize;
}

uint32_t EventBox::getSizeMax() {
    return MAX_EVENT_POOL_SIZE;
}
