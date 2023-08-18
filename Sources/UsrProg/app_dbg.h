#ifndef __APP_DBG_H
#define __APP_DBG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "xprintf.h"

#define APP_DBG_EN      (1)
#define APP_LOG_EN      (1)
#define APP_DBG_SIG_EN  (1)

#define APP_PRINT					xprintf

#if APP_DBG_EN
#define APP_DBG(fmt, ...)       	APP_PRINT(KBLU"[DBG] " fmt "\r\n" KNRM, ##__VA_ARGS__)
#else
#define APP_DBG(fmt, ...)
#endif

#if APP_LOG_EN
#define APP_LOG(tag, fmt, ...)     APP_PRINT("[" tag "] " fmt  "\r\n", ##__VA_ARGS__)
#else
#define APP_LOG(tag, fmt, ...)
#endif

#if APP_DBG_SIG_EN
#define APP_DBG_SIG(tag, fmt, ...)  APP_PRINT(KMAG "[SIG] " KGRN tag " -> " KYEL fmt "\r\n" KNRM, ##__VA_ARGS__)
#else
#define APP_DBG_SIG(tag, fmt, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif