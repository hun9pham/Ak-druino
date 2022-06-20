#ifndef __AK_H__
#define __AK_H__

#include <string.h>
#include "task.h"
#include "message.h"
#include "timer.h"
#include "port.h"

/*========================================================================
 * #### Active Kernel defines ####
 =========================================================================*/
#define AK_ENABLE						( 0x01 )
#define AK_DISABLE						( 0x00 )

#define AK_RET_OK						( 0x01 )
#define AK_RET_NG						( 0x00 )

#define AK_USER_DEFINE_SIG				( 10 )

#define TASK_PRI_MAX_SIZE				( 4 )


#endif /* __AK_H__ */
