#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdint.h>
#include <stdlib.h>
#include "port.h"

/*========================================================================
 * #### Message defines  ####
 =========================================================================*/
#define AK_MSG_NULL                 ((Ak_Msg_t*)0)

#define AK_MSG_NG					( 0 )
#define AK_MSG_OK					( 1 )

#ifndef AK_PURE_MSG_POOL_SIZE
#define AK_PURE_MSG_POOL_SIZE       ( 8 )   
#endif

#ifndef AK_DYNAMIC_MSG_POOL_SIZE
#define AK_DYNAMIC_MSG_POOL_SIZE	( 4 )
#endif

#define PURE_MSG_TYPE				( 0x80 )
#define DYNAMIC_MSG_TYPE			( 0xC0 )

typedef struct AK_MESSAGE_STRUCTURE {
	struct AK_MESSAGE_STRUCTURE *pxNext;

	/**
	 * @brief Message info
	 **/
	uint8_t	Src_Task_ID;
	uint8_t	Des_Task_ID;
	uint8_t	Signal;
	uint8_t Type;
} Ak_Msg_t;

typedef struct AK_PURE_MESSAGE_STRUCTURE {
	Ak_Msg_t Msg_Header;
} Ak_Msg_Pure_t;

typedef struct AK_DYNAMIC_MESSAGE_STRUCTURE {
	Ak_Msg_t Msg_Header;
	uint16_t Length_Of_Data;
	uint8_t* pxData;
} Ak_Msg_Dynamic_t;


/**
 *************************
 * @class Pure message   *
 *************************/
class PureMessage {
private:
	Ak_Msg_Pure_t PureMessagePool[AK_PURE_MSG_POOL_SIZE];
	Ak_Msg_t *pxFreePureMessagePool;
	uint16_t PureMessagePoolUsed;

public:
	PureMessage(void);
	uint16_t GetPureMessagePoolUsed(void);
	uint16_t GetPureMessagePoolFree(void);
	Ak_Msg_t *GetPureMessage(void);
	void FreePureMessage(Ak_Msg_t *pxMsg);
};


/**
 **************************
 * @class Dynamci message *
 **************************/
class DynamicMessage {
private:
	Ak_Msg_Dynamic_t DynamicMessagePool[AK_DYNAMIC_MSG_POOL_SIZE];
	Ak_Msg_t *pxFreeDynamicMessagePool;
	uint16_t DynamicMessagePoolUsed;
	
public:
	DynamicMessage(void);
	Ak_Msg_t *GetDynamicMessage(void);
	uint16_t GetDynamicMessagePoolUsed(void);
	uint16_t GetDynamicMessagePoolFree(void);
	uint8_t *GetDataDynamicMessage(Ak_Msg_t *pxMsg);
	uint16_t GetLengthOfDataDynamicMessage(Ak_Msg_t *pxMsg);
	uint8_t	SetDataDynamicMessage(Ak_Msg_t *pxMsg, uint8_t *pxData, 
										uint16_t Length_Of_Data);
	void FreeDynamicMessage(Ak_Msg_t *pxMsg);
};

/**
 **************************
 * @class Message 		  *
 **************************/
class Message : public PureMessage, public DynamicMessage {
public:
	Message(void);
	uint16_t GetPureMessagePoolMax(void);
	uint16_t GetDynamicMessagePoolMax(void);
	void SetMessageInformation(Ak_Msg_t *pxMsg, uint8_t Des_Task_ID, uint8_t Signal);
	void Free(Ak_Msg_t *pxMsg);
};

#endif /* __MESSAGE_H__ */
