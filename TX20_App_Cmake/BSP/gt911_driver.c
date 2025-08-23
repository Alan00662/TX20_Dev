#include "gt911_driver.h"
#include <stdio.h>
#include "string.h" 
#include "i2c.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#define TP_I2C hi2c1


static void set_float(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOF_CLK_ENABLE();
  GPIO_InitStruct.Pin = TP_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TP_INT_GPIO_Port, &GPIO_InitStruct);
}


static void GT911_WR_Reg_One_byte(uint16_t reg,uint8_t data)
{
	uint8_t temp[3]={0};
	temp[0]=reg>>8;
	temp[1]=reg&0xff;
	temp[2]=data;
	if(HAL_I2C_Master_Transmit(&TP_I2C,GT_CMD_WR,temp,3,0xff)!=HAL_OK)
	{
//		printf("write reg one byte error\r\n");
	}
		
}

  
static void GT911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t temp[2]={0};
	temp[1]=reg&0xff;
	temp[0]=reg>>8;
	if(HAL_I2C_Master_Transmit(&TP_I2C,GT_CMD_WR,temp,2,0xff)!=HAL_OK)
	{
//		printf("read reg error\r\n");
	}
	if(HAL_I2C_Master_Receive(&TP_I2C,GT_CMD_RD,buf,len,0xff)!=HAL_OK)
	{
//		printf("read reg error\r\n");
	}

}


void gt911_init(void)
{
	  uint8_t temp[5]={0};	

    HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin,GPIO_PIN_RESET);
    	debug_tx4("HAL_Delay1\n");
    HAL_Delay(10);
    	debug_tx4("HAL_Delay2\n");
    HAL_GPIO_WritePin(TP_INT_GPIO_Port, TP_INT_Pin,GPIO_PIN_RESET);
    HAL_Delay(1);
    debug_tx4("HAL_Delay3\n");
    HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin,GPIO_PIN_SET);
    HAL_Delay(60);
	debug_tx4("HAL_Delay4\n");	
	set_float();

	temp[4]=0;
	GT911_RD_Reg(GT_PID_REG,temp,4);
	debug_tx4("TOUCH ID:%s\r\n",temp);
	temp[0]=0X02;			
	GT911_WR_Reg_One_byte(GT_CTRL_REG,temp[0]);
	GT911_RD_Reg(GT_CFGS_REG,temp,1);

	
}


void gt911_get_state(TS_StateTypeDef* state)
{
	static uint8_t counts=0;
	uint8_t flag=0;
	uint8_t buf[4]={0};
	
	GT911_RD_Reg(GT_GSTID_REG,&flag,1);
	if(flag&0x80)																		
	{

		GT911_WR_Reg_One_byte(GT_GSTID_REG,0);
		if(flag&0xf)																	
		{

			GT911_RD_Reg(GT_TP1_REG,buf,4);
			state->X=(uint16_t)((buf[1]<<8)|buf[0]);
			state->Y=(uint16_t)((buf[3]<<8)|buf[2]);
			state->TouchDetected=1;
			GT911_WR_Reg_One_byte(GT_GSTID_REG,0);
		}
		else
			state->TouchDetected=0;
	}
	else
		state->TouchDetected=0;
	
	counts++;
}


TS_StateTypeDef Touch ={0};
