

#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#include "main.h"
#include <string.h>
#include <stdio.h>

#define USBD_MAX_NUM_INTERFACES     1U
#define USBD_MAX_NUM_CONFIGURATION  1U
#define USBD_MAX_STR_DESC_SIZ       512U
#define USBD_DEBUG_LEVEL            0U
#define USBD_LPM_ENABLED            0U
#define USBD_SELF_POWERED           1U

/* MSC类配置 */
#define MSC_MEDIA_PACKET            (32 * 1024U)
#define MSC_STORAGE_LUN_NBR         3U

/* 设备ID定义 */
#define DEVICE_HS                   0

/* 内存管理相关宏定义 */
#define USBD_malloc                 (void *)USBD_static_malloc
#define USBD_free                   USBD_static_free
#define USBD_memset                 memset

/* 调试相关宏定义 */
#if (USBD_DEBUG_LEVEL > 0U)
#define USBD_UsrLog(...)    do {                        \
                                printf(__VA_ARGS__);    \
                                printf("\n");           \
                            } while (0)
#else
#define USBD_UsrLog(...)    do {} while (0)
#endif
#if (USBD_DEBUG_LEVEL > 1U)
#define USBD_ErrLog(...)    do {                        \
                                printf("ERROR: ") ;     \
                                printf(__VA_ARGS__);    \
                                printf("\n"); \
                            } while (0)
#else
#define USBD_ErrLog(...)    do {} while (0)
#endif
#if (USBD_DEBUG_LEVEL > 2U)
#define USBD_DbgLog(...)    do {                        \
                                printf("DEBUG : ") ;    \
                                printf(__VA_ARGS__);    \
                                printf("\n");           \
                            } while (0)
#else
#define USBD_DbgLog(...)    do {} while (0)
#endif

/* 导出相关变量 */
extern uint8_t g_usb_conn_state;

/* 函数声明 */
void *USBD_static_malloc(uint32_t size);
void USBD_static_free(void *p);

#endif
