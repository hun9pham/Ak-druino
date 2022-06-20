#include <stdlib.h>

#include "ak.h"
#include "message.h"
#include "task.h"


/**
 *************************
 * @class Pure message   *
 *************************/
PureMessage :: PureMessage() {
	ENTRY_CRITICAL();
	this->pxFreePureMessagePool = (Ak_Msg_t *)(&this->PureMessagePool[0]);
	this->PureMessagePoolUsed = 0;

	for (uint8_t iDx = 0; iDx < AK_PURE_MSG_POOL_SIZE - 1; ++iDx) {
		this->PureMessagePool[iDx].Msg_Header.Type = PURE_MSG_TYPE;
		this->PureMessagePool[iDx].Msg_Header.pxNext = (Ak_Msg_t *)(&this->PureMessagePool[iDx + 1]);
	}
	this->PureMessagePool[AK_PURE_MSG_POOL_SIZE - 1].Msg_Header.Type = PURE_MSG_TYPE;
	this->PureMessagePool[AK_PURE_MSG_POOL_SIZE - 1].Msg_Header.pxNext = AK_MSG_NULL;
	EXIT_CRITICAL();
}

Ak_Msg_t* PureMessage :: GetPureMessage() {
	Ak_Msg_t* _px_msg_allocate;

	ENTRY_CRITICAL();
	_px_msg_allocate = this->pxFreePureMessagePool;

	if (_px_msg_allocate == AK_MSG_NULL) {
		FATAL("MF", 0x30);
	}
	else {
		this->pxFreePureMessagePool = _px_msg_allocate->pxNext;

		++(this->PureMessagePoolUsed);
	}

	_px_msg_allocate->Src_Task_ID = Task :: GetCurrentTaskID();
	_px_msg_allocate->pxNext = NULL;
	EXIT_CRITICAL();

	return (_px_msg_allocate);
}

void PureMessage :: FreePureMessage(Ak_Msg_t *pxMsg) {
	ENTRY_CRITICAL();
	pxMsg->pxNext = this->pxFreePureMessagePool;
	this->pxFreePureMessagePool = pxMsg;

	--(this->PureMessagePoolUsed);
	EXIT_CRITICAL();
}

uint16_t PureMessage :: GetPureMessagePoolUsed() {
	return (this->PureMessagePoolUsed);
}

uint16_t PureMessage :: GetPureMessagePoolFree() {
	return (AK_PURE_MSG_POOL_SIZE - (this->PureMessagePoolUsed));
}


/**
 **************************
 * @class Dynamic message *
 *************************/
DynamicMessage :: DynamicMessage() {
	ENTRY_CRITICAL();
	this->pxFreeDynamicMessagePool = (Ak_Msg_t *)(&this->DynamicMessagePool[0]);
	this->DynamicMessagePoolUsed = 0;

	for (uint16_t iDx = 0; iDx < AK_DYNAMIC_MSG_POOL_SIZE - 1; ++iDx) {
		this->DynamicMessagePool[iDx].Msg_Header.Type = DYNAMIC_MSG_TYPE;
		this->DynamicMessagePool[iDx].Msg_Header.pxNext = (Ak_Msg_t *)(&this->DynamicMessagePool[iDx + 1]);
	}
	this->DynamicMessagePool[AK_DYNAMIC_MSG_POOL_SIZE - 1].Msg_Header.Type = DYNAMIC_MSG_TYPE;
	this->DynamicMessagePool[AK_DYNAMIC_MSG_POOL_SIZE - 1].Msg_Header.pxNext = AK_MSG_NULL;
	EXIT_CRITICAL();
}

Ak_Msg_t* DynamicMessage :: GetDynamicMessage() {
	Ak_Msg_t* _px_msg_allocate;

	ENTRY_CRITICAL();
	_px_msg_allocate = this->pxFreeDynamicMessagePool;

	if (_px_msg_allocate == AK_MSG_NULL) {
		FATAL("MF", 0x31);
	}
	else {
		this->pxFreeDynamicMessagePool = _px_msg_allocate->pxNext;

		++(this->DynamicMessagePoolUsed);
	}

	_px_msg_allocate->Src_Task_ID = Task :: GetCurrentTaskID();
	_px_msg_allocate->pxNext = AK_MSG_NULL;
	EXIT_CRITICAL();

	return (_px_msg_allocate);
}

uint16_t DynamicMessage :: GetLengthOfDataDynamicMessage(Ak_Msg_t *pxMsg) {
	if (pxMsg->Type != DYNAMIC_MSG_TYPE) {
		FATAL("DYNAMIC", 0x01);
	}

	return (((Ak_Msg_Dynamic_t *)pxMsg)->Length_Of_Data);
}

uint8_t* DynamicMessage :: GetDataDynamicMessage(Ak_Msg_t *pxMsg) {
	if (pxMsg == AK_MSG_NULL) {
		FATAL("DYNAMIC", 0x02);
	}

	if (pxMsg->Type != DYNAMIC_MSG_TYPE) {
		FATAL("DYNAMIC", 0x03);
	}

	return (((Ak_Msg_Dynamic_t *)pxMsg)->pxData);
}

uint8_t	DynamicMessage :: SetDataDynamicMessage(Ak_Msg_t *pxMsg, uint8_t *pxData, 
									uint16_t Length_Of_Data) 
{
	if ((pxMsg->Type) != DYNAMIC_MSG_TYPE) {
		FATAL("MF", 0x43);
	}

	((Ak_Msg_Dynamic_t *)pxMsg)->Length_Of_Data = Length_Of_Data;
	((Ak_Msg_Dynamic_t *)pxMsg)->pxData = (uint8_t*)malloc(Length_Of_Data);
	memcpy(((Ak_Msg_Dynamic_t*)pxMsg)->pxData, pxData, Length_Of_Data);

	return AK_MSG_OK;
}

void DynamicMessage :: FreeDynamicMessage(Ak_Msg_t *pxMsg) {
	ENTRY_CRITICAL();
	pxMsg->pxNext = this->pxFreeDynamicMessagePool;
	this->pxFreeDynamicMessagePool = pxMsg;
	--(this->DynamicMessagePoolUsed);

	free(((Ak_Msg_Dynamic_t*)pxMsg)->pxData);

	EXIT_CRITICAL();
}

uint16_t DynamicMessage :: GetDynamicMessagePoolUsed() {
	return this->DynamicMessagePoolUsed;
}

uint16_t DynamicMessage :: GetDynamicMessagePoolFree() {
	return (AK_DYNAMIC_MSG_POOL_SIZE - (this->DynamicMessagePoolUsed));
}

/**
 *************************
 * @class Message   	 *
 *************************/
Message :: Message() : PureMessage(), DynamicMessage() {
}

uint16_t Message :: GetPureMessagePoolMax() {
	return AK_PURE_MSG_POOL_SIZE;
}

uint16_t Message :: GetDynamicMessagePoolMax() {
	return AK_DYNAMIC_MSG_POOL_SIZE;
}

void Message :: SetMessageInformation(Ak_Msg_t *pxMsg, uint8_t Des_Task_ID, uint8_t Signal) {
	pxMsg->Des_Task_ID = Des_Task_ID;
	pxMsg->Signal = Signal;
}

void Message :: Free(Ak_Msg_t *pxMsg) {
	uint8_t MessageType = pxMsg->Type;

	switch(MessageType) {
		case PURE_MSG_TYPE: {
			PureMessage :: FreePureMessage(pxMsg);
		}
		break;

		case DYNAMIC_MSG_TYPE: {
			DynamicMessage :: FreeDynamicMessage(pxMsg);
		}
		break;

		default:
			FATAL("MF", 0x20);
		break;
	}
}
