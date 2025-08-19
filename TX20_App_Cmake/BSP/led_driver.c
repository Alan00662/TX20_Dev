#include "led_driver.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

void ledTask(void* argument)
{
    while(1)
    {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        osDelay(500);
    }
}