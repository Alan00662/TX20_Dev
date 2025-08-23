#include "led_driver.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

void led_on(uint8_t num) {
  switch (num) {
  case 0:
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,GPIO_PIN_SET);
    break;
  case 1:
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_SET);
    break;
  case 2:
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_SET);
    break;
  }
}

void led_off(uint8_t num) {
  switch (num) {
  case 0:
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,GPIO_PIN_RESET);
    break;
  case 1:
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_RESET);
    break;
  case 2:
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_RESET);
    break;
  }
}

void led_loopbink(void)
{
	static uint8_t bink_flg = 0;
		switch(bink_flg)
		{
			case 0:
				led_on(0);
				led_off(1);
				led_off(2);
				bink_flg =1;
				break;
			case 1:
				led_on(1);
				led_off(0);
				led_off(2);
				bink_flg =2;
				break;
			case 2:
				led_on(2);
				led_off(1);
				led_off(0);
				bink_flg =0;
				break;
		}			

}	