
#include "sdmmc_sdcard.h"
#include "main.h"
#include "usart.h"

/* SD句柄 */
SD_HandleTypeDef g_sd_handle = {0};
HAL_SD_CardInfoTypeDef g_sd_card_info_struct = {0};


uint8_t sd_init(void)
{
    g_sd_handle.Instance = SD_SDMMCX;
    g_sd_handle.Init.ClockEdge = SDMMC_CLOCK_EDGE_FALLING;                      /* 时钟相位 */
    g_sd_handle.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;           /* 节能模式 */
    g_sd_handle.Init.BusWide = SDMMC_BUS_WIDE_1B;                               /* 宽总线模式 */
    g_sd_handle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;  /* 硬件流控 */
    g_sd_handle.Init.ClockDiv = SDMMC_HSPEED_CLK_DIV*2;  //4G卡                         /* 时钟分频系数 */
    HAL_SD_DeInit(&g_sd_handle);
    if (HAL_SD_Init(&g_sd_handle) != HAL_OK) {
       debug_tx4("SD_CARD init Err1 \n");
        return 1;
    }

    if (HAL_SD_ConfigWideBusOperation(&g_sd_handle, SDMMC_BUS_WIDE_4B) !=
        HAL_OK) {
      debug_tx4("SD_CARD init Err2 \n");
        return 1;
    }

    if (HAL_SD_ConfigSpeedBusOperation(&g_sd_handle, SDMMC_SPEED_MODE_HIGH) !=
        HAL_OK) {
      debug_tx4("SD_CARD init Err3 \n");
        return 1;
    }

    if (HAL_SD_GetCardInfo(&g_sd_handle, &g_sd_card_info_struct) != HAL_OK) {
      debug_tx4("SD_CARD init Err4 \n");
        return 1;
    }
    debug_tx4("SD_CARD init OK \n");
    return 0;
}


void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    RCC_PeriphCLKInitTypeDef rcc_periph_clk_init_struct = {0};
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    if (hsd->Instance == SD_SDMMCX)
    {
        /* 配置时钟 */
        rcc_periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC12;
        rcc_periph_clk_init_struct.Sdmmc12ClockSelection = RCC_SDMMC12CLKSOURCE_PLL2S;
        HAL_RCCEx_PeriphCLKConfig(&rcc_periph_clk_init_struct);
        
        /* 使能时钟 */
        SD_SDMMCX_CLK_ENABLE();
        SD_SDMMCX_CK_GPIO_CLK_ENABLE();
        SD_SDMMCX_CMD_GPIO_CLK_ENABLE();
        SD_SDMMCX_D0_GPIO_CLK_ENABLE();
        SD_SDMMCX_D1_GPIO_CLK_ENABLE();
        SD_SDMMCX_D2_GPIO_CLK_ENABLE();
        SD_SDMMCX_D3_GPIO_CLK_ENABLE();
        
        /* 初始化CK引脚 */
        gpio_init_struct.Pin = SD_SDMMCX_CK_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = SD_SDMMCX_CK_GPIO_AF;
        HAL_GPIO_Init(SD_SDMMCX_CK_GPIO_PORT, &gpio_init_struct);
        
        /* 初始化CMD引脚 */
        gpio_init_struct.Pin = SD_SDMMCX_CMD_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = SD_SDMMCX_CMD_GPIO_AF;
        HAL_GPIO_Init(SD_SDMMCX_CMD_GPIO_PORT, &gpio_init_struct);
        
        /* 初始化D0引脚 */
        gpio_init_struct.Pin = SD_SDMMCX_D0_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = SD_SDMMCX_D0_GPIO_AF;
        HAL_GPIO_Init(SD_SDMMCX_D0_GPIO_PORT, &gpio_init_struct);
        
        /* 初始化D1引脚 */
        gpio_init_struct.Pin = SD_SDMMCX_D1_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = SD_SDMMCX_D1_GPIO_AF;
        HAL_GPIO_Init(SD_SDMMCX_D1_GPIO_PORT, &gpio_init_struct);
        
        /* 初始化D2引脚 */
        gpio_init_struct.Pin = SD_SDMMCX_D2_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = SD_SDMMCX_D2_GPIO_AF;
        HAL_GPIO_Init(SD_SDMMCX_D2_GPIO_PORT, &gpio_init_struct);
        
        /* 初始化D3引脚 */
        gpio_init_struct.Pin = SD_SDMMCX_D3_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = SD_SDMMCX_D3_GPIO_AF;
        HAL_GPIO_Init(SD_SDMMCX_D3_GPIO_PORT, &gpio_init_struct);
    }
}


void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
    if (hsd->Instance == SD_SDMMCX)
    {
        /* 失能时钟 */
        SD_SDMMCX_CLK_DISABLE();
        
        /* 反初始化引脚 */
        HAL_GPIO_DeInit(SD_SDMMCX_CK_GPIO_PORT, SD_SDMMCX_CK_GPIO_PIN);
        HAL_GPIO_DeInit(SD_SDMMCX_CMD_GPIO_PORT, SD_SDMMCX_CMD_GPIO_PIN);
        HAL_GPIO_DeInit(SD_SDMMCX_D0_GPIO_PORT, SD_SDMMCX_D0_GPIO_PIN);
        HAL_GPIO_DeInit(SD_SDMMCX_D1_GPIO_PORT, SD_SDMMCX_D1_GPIO_PIN);
        HAL_GPIO_DeInit(SD_SDMMCX_D2_GPIO_PORT, SD_SDMMCX_D2_GPIO_PIN);
        HAL_GPIO_DeInit(SD_SDMMCX_D3_GPIO_PORT, SD_SDMMCX_D3_GPIO_PIN);
    }
}

uint8_t sd_read_disk(uint8_t *buf, uint32_t address, uint32_t count)
{
    uint32_t timeout = SD_TIMEOUT;
    
    if (HAL_SD_ReadBlocks(&g_sd_handle, buf, address, count, SD_TIMEOUT) != HAL_OK)
    {
        return 1;
    }
    
    while ((HAL_SD_GetCardState(&g_sd_handle) != HAL_SD_CARD_TRANSFER) && (--timeout != 0));
    
    if (timeout == 0)
    {
        return 1;
    }
    
    return 0;
}


uint8_t sd_write_disk(uint8_t *buf, uint32_t address, uint32_t count)
{
    uint32_t timeout = SD_TIMEOUT;
    
    if (HAL_SD_WriteBlocks(&g_sd_handle, buf, address, count, SD_TIMEOUT) != HAL_OK)
    {
        return 1;
    }
    
    while ((HAL_SD_GetCardState(&g_sd_handle) != HAL_SD_CARD_TRANSFER) && (--timeout != 0));
    
    if (timeout == 0)
    {
        return 1;
    }
    
    return 0;
}
static HAL_SD_CardCIDTypeDef sd_card_cid_struct = {0};
void show_sdcard_info(void)
{
    HAL_SD_GetCardInfo(&g_sd_handle, &g_sd_card_info_struct);
    HAL_SD_GetCardCID(&g_sd_handle, &sd_card_cid_struct);
    
    debug_tx4("Card Type: %s\r\n", (g_sd_card_info_struct.CardType == CARD_SDSC) ? ((g_sd_card_info_struct.CardVersion == CARD_V1_X) ? ("SDSC V1") :
                                                                              ((g_sd_card_info_struct.CardVersion == CARD_V1_X) ? ("SDSC V2") :
                                                                              (""))) :
                                ((g_sd_card_info_struct.CardType == CARD_SDHC_SDXC) ? ("SDHC") :
                                ((g_sd_card_info_struct.CardType == CARD_SECURED) ? ("SECURE") :
                                (""))));
    debug_tx4("Card ManufacturerID: %d\r\n", sd_card_cid_struct.ManufacturerID);
    debug_tx4("Card RCA: %d\r\n", g_sd_card_info_struct.RelCardAdd);
    debug_tx4("LogBlockNbr: %d \r\n", g_sd_card_info_struct.LogBlockNbr);
    debug_tx4("LogBlockSize: %d \r\n", g_sd_card_info_struct.LogBlockSize);
    debug_tx4("Card Capacity: %d MB\r\n", (uint32_t)(((uint64_t)g_sd_card_info_struct.LogBlockNbr * g_sd_card_info_struct.LogBlockSize) >> 20));
    debug_tx4("Card BlockSize: %d\r\n\r\n", g_sd_card_info_struct.BlockSize);
    
}
