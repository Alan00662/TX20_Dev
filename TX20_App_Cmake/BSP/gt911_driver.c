#include "gt911_driver.h"
#include <stdio.h>
#include "string.h" 
#include "i2c.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#define TP_I2C I2C2

static void set_float(void)
{
	LL_GPIO_SetPinMode(TP_INT_GPIO_Port, TP_INT_Pin, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(TP_INT_GPIO_Port, TP_INT_Pin, LL_GPIO_PULL_NO);
}


static void GT911_WR_Reg_One_byte(uint16_t reg,uint8_t data)
{
	uint8_t temp[3]={0};
	temp[0]=reg>>8;
	temp[1]=reg&0xff;
	temp[2]=data;
	
	LL_I2C_HandleTransfer(TP_I2C, GT_CMD_WR, LL_I2C_ADDRSLAVE_7BIT, 3, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
	
	while(!LL_I2C_IsActiveFlag_TXIS(TP_I2C))
  { 
  }	
	LL_I2C_TransmitData8(TP_I2C,temp[0]);

	while(!LL_I2C_IsActiveFlag_TXIS(TP_I2C))
  { 
  }	
	LL_I2C_TransmitData8(TP_I2C,temp[1]);
		
	while(!LL_I2C_IsActiveFlag_TXIS(TP_I2C))
  { 
  }	
	LL_I2C_TransmitData8(TP_I2C,temp[2]);
		
}

  
static void GT911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i = 0;
	uint8_t temp[2]={0};
	temp[0]=reg>>8;	
	temp[1]=reg&0xff;

	LL_I2C_HandleTransfer(TP_I2C, GT_CMD_WR, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
	while(!LL_I2C_IsActiveFlag_TXIS(TP_I2C))
  { 
  }
	LL_I2C_TransmitData8(TP_I2C,temp[0]);
		
	while(!LL_I2C_IsActiveFlag_TXIS(TP_I2C))
  { 
  }
	LL_I2C_TransmitData8(TP_I2C,temp[1]);
		
	LL_I2C_HandleTransfer(TP_I2C, GT_CMD_WR, LL_I2C_ADDRSLAVE_7BIT, len, LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_READ);
	for(i =0;i<len;i++)
	{
		while(!LL_I2C_IsActiveFlag_RXNE(TP_I2C))
		{		
		}
		*(buf+i) = LL_I2C_ReceiveData8(TP_I2C);
	}

}


void gt911_init(void)
{
	  uint8_t temp[5]={0};	
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
    LL_GPIO_SetPinMode(TP_RST_GPIO_Port, TP_RST_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(TP_RST_GPIO_Port, TP_RST_Pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(TP_RST_GPIO_Port, TP_RST_Pin, LL_GPIO_PULL_NO);
    

    LL_GPIO_ResetOutputPin(TP_RST_GPIO_Port, TP_RST_Pin);
    HAL_Delay(10);
    LL_GPIO_SetOutputPin(TP_INT_GPIO_Port, TP_INT_Pin);
    HAL_Delay(1);
    LL_GPIO_SetOutputPin(TP_RST_GPIO_Port, TP_RST_Pin);
    HAL_Delay(60);
		
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

void touchTask(void* argument)
{
    while(1)
    {
        gt911_get_state(&Touch);
        if(Touch.TouchDetected)
        {
            debug_tx4("X:%d,Y:%d\n",Touch.X,Touch.Y);
        }
        HAL_Delay(10);
    }
}