#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif
	
#include "main.h"
void led_on(uint8_t num);
void led_off(uint8_t num);
void led_loopbink(void);
#ifdef __cplusplus
}
#endif

#endif
