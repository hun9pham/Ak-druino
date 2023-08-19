#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "sys_io.h"
#include "sys_cfgCtl.h"

#define FLASH_BLOCK_START_ADDR              (0x00000000)
#define FLASH_BLOCK_SIZE                    (0x10000) /* 64K */
#define FLASH_SECTOR_SIZE                   (0x1000)  /* 4K */

#define FLASH_OK                            (1)
#define FLASH_NG                            (0)

/* Total 512 Blocks (0 - 511) */
#define FLASH_BLOCK_ID_MIN                  (0)
#define FLASH_BLOCK_ID_MAX                  (511)

/* Total 16 Sectors each Block (0 - 15) */
#define FLASH_SECTOR_ID_MIN                 (0)
#define FLASH_SECTOR_ID_MAX                 (15)

#define GET_FLASH_SECTOR_START_ADDR(b, s)   ((uint32_t)(FLASH_BLOCK_START_ADDR + (b * FLASH_BLOCK_SIZE)) + (s * FLASH_SECTOR_SIZE))
#define GET_FLASH_SECTOR_END_ADDR(b, s)     ((GET_FLASH_SECTOR_START_ADDR(b, s) + FLASH_SECTOR_SIZE) - 0x00000001)

#define GET_FLASH_BLOCK_START_ADDR(b)       ((uint32_t)(FLASH_BLOCK_START_ADDR + (b * FLASH_BLOCK_SIZE)))
#define GET_FLASH_BLOCK_END_ADDR(b)         ((GET_FLASH_BLOCK_START_ADDR(b) + FLASH_BLOCK_SIZE) - 0x00000001)

/*----------------------------------------------------------------------------*
 *  DECLARE: User port function 
 *  Note: 
 *----------------------------------------------------------------------------*/
#define FlashDelayUs(Us)            delayMicros(Us)
#define FlashCsHigh()               setFlashCsHigh()
#define FlashCsLow()                setFlashCsLow()
#define FlashTransferData(Data)     transfeFlashSpiData(Data)

/* Function prototypes -------------------------------------------------------*/
extern uint8_t flashEntry4ByteModes();
extern uint8_t flashExit4ByteModes();
extern uint8_t flashRead(uint32_t addr, uint8_t* pBuff, uint32_t NbrByte);
extern uint8_t flashWrite(uint32_t addr, uint8_t* pBuff, uint32_t NbrByte);
extern uint8_t flashEraseSector(uint32_t addr);
extern uint8_t flashEraseBlock32k(uint32_t addr);
extern uint8_t flashEraseBlock64k(uint32_t addr);
extern uint8_t flashEraseFull(void);

class flashClass {
private:

public:

};

extern flashClass EXTFLASH;

#ifdef __cplusplus
}
#endif

#endif