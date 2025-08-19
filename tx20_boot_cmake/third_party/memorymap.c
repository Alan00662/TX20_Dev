/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    memorymap.c
  * @brief   This file provides code for the configuration
  *          of the MEMORYMAP instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "memorymap.h"

/* USER CODE BEGIN 0 */
#include "xspi.h"
#include "w35t51nwtbie.h"
#include "aps256xx.h"

#include <stdio.h>

#define EXTMEM_LOG(...)
//#define EXTMEM_LOG(...)	printf(__VA_ARGS__)
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
int32_t EXTMEM_Flash_Probe(void)
{
    uint8_t device_id[3];
    int retr = 0;

    if (W35T51NWTBIE_OK != W35T51NWTBIE_ReadID(&hxspi2, W35T51NWTBIE_SPI_MODE, W35T51NWTBIE_STR_TRANSFER, device_id,
                                               W35T51NWTBIE_3BYTES_SIZE))
    {
        EXTMEM_LOG("Read Flash ID Fail\r\n");
        return -1;
    }
    else
    {
        EXTMEM_LOG("Read Flash ID success:0x%.2x 0x%.2x 0x%.2x\r\n", device_id[0], device_id[1], device_id[2]);

        /* stage 1. enter octal mode and set flash register max speed and other configs */
        retr = W35T51NWTBIE_EnterOctal_DTR_Mode(&hxspi2);
        if (retr != W35T51NWTBIE_OK)
        {
            EXTMEM_LOG("Flash Enter Octal mode Fail, errno:%d\r\n", retr);
            return -1;
        }

        if (HAL_XSPI_DeInit(&hxspi2) != HAL_OK)
        {
            EXTMEM_LOG("Flash deinit error\r\n");
            return -1;
        }

        /* stage 2. reinit and use new configs to achieve maxim speed */
        EXTMEM_LOG("Reconfigure Flash clock\r\n");
        MX_XSPI2_Init();

        if (W35T51NWTBIE_OK != W35T51NWTBIE_ReadID(&hxspi2, W35T51NWTBIE_OPI_MODE, W35T51NWTBIE_DTR_TRANSFER, device_id,
                                                   W35T51NWTBIE_4BYTES_SIZE))
        {
            EXTMEM_LOG("Flash Re-Enter octal failed\r\n");
            return -1;
        }
        else
        {
            EXTMEM_LOG("Flash Re-Enter Octal DTR and Read ID success:%.2x %.2x %.2x\r\n", device_id[0], device_id[1],
                   device_id[2]);
            return 0;
        }
    }
}

int32_t EXTMEM_Flash_EnterXIP(void)
{
    if (W35T51NWTBIE_OK != W35T51NWTBIE_EnableMemoryMappedModeDTR(&hxspi2, 0))
    {
        EXTMEM_LOG("Flash XIP failed\r\n");
        return -1;
    }
    else
    {
        EXTMEM_LOG("Flash XIP success\r\n");
        return 0;
    }
}

/* Configure the 16-bits Octal RAM memory ***************************/
static XSPI_HSCalTypeDef cal = {0};
int32_t EXTMEM_PSRAM_Probe(void)
{
    uint8_t reg[2];

    if (APS256XX_OK != APS256XX_Reset(&hxspi1))
    {
        EXTMEM_LOG("PSRAM Reset error\r\n");
        return -1;
    }
    HAL_Delay(20);

    if (APS256XX_OK == APS256XX_ReadReg(&hxspi1, APS256XX_MR0_ADDRESS, reg, 5))
    {
        EXTMEM_LOG("PSRAM MR0: 0x%.2x, MR1: 0x%.2x\r\n", reg[0], reg[1]);
    }

    MODIFY_REG(reg[0], ((uint8_t)APS256XX_MR0_LATENCY_TYPE | (uint8_t)APS256XX_MR0_READ_LATENCY_CODE | (uint8_t)APS256XX_MR0_DRIVE_STRENGTH),
               ((uint8_t)APS256XX_MR0_LATENCY_TYPE_FIXED |
                (uint8_t)APS256XX_MR0_RLC_7 |
                (uint8_t)APS256XX_MR0_DS_FULL));
    if (APS256XX_OK == APS256XX_WriteReg(&hxspi1, APS256XX_MR0_ADDRESS, reg[0]))
    {
        EXTMEM_LOG("PSRAM MR0 SET\r\n");
    }

    MODIFY_REG(reg[0], (uint8_t)(APS256XX_MR4_WRITE_LATENCY_CODE | APS256XX_MR4_RF_RATE | APS256XX_MR4_PASR),
               ((uint8_t)APS256XX_MR4_WLC_7 |
                APS256XX_MR4_RF_4X |
                APS256XX_MR4_PASR_FULL));
    if (APS256XX_OK == APS256XX_WriteReg(&hxspi1, APS256XX_MR4_ADDRESS, reg[0]))
    {
        EXTMEM_LOG("PSRAM MR4 SET\r\n");
    }
    reg[0] = 0;
    MODIFY_REG(reg[0], ((uint8_t)APS256XX_MR8_X8_X16 | (uint8_t)APS256XX_MR8_RBX | (uint8_t)APS256XX_MR8_BT | (uint8_t)APS256XX_MR8_BL),
               ((uint8_t)APS256XX_MR8_X16 |
                (uint8_t)APS256XX_MR8_HYBRID_NONE_BURST_WRAP |
                (uint8_t)APS256XX_MR8_RBX_NOT_SUPPORT |
                (uint8_t)APS256XX_MR8_BL_16_BYTES));
    // reg[0] = 0x40;
    if (APS256XX_OK == APS256XX_WriteReg(&hxspi1, APS256XX_MR8_ADDRESS, reg[0]))
    {
        EXTMEM_LOG("PSRAM MR8 SET\r\n");
    }

    HAL_XSPI_GetDelayValue(&hxspi1, &cal);
    EXTMEM_LOG("cal delay: 0x%.2x, fine: 0x%.2x, coarse 0x%.2x, max 0x%.2x\r\n", cal.DelayValueType, cal.FineCalibrationUnit, cal.CoarseCalibrationUnit, cal.MaxCalibration);

    if (APS256XX_OK == APS256XX_ReadReg(&hxspi1, APS256XX_MR0_ADDRESS, reg, 7))
    {
        EXTMEM_LOG("PSRAM MR0: 0x%.2x, MR1: 0x%.2x\r\n", reg[0], reg[1]);
    }
    if (APS256XX_OK == APS256XX_ReadReg(&hxspi1, APS256XX_MR4_ADDRESS, reg, 7))
    {
        EXTMEM_LOG("PSRAM MR4: 0x%.2x, MR5: 0x%.2x\r\n", reg[0], reg[1]);
    }
    if (APS256XX_OK == APS256XX_ReadReg(&hxspi1, APS256XX_MR8_ADDRESS, reg, 7))
    {
        EXTMEM_LOG("PSRAM MR8: 0x%.2x, MR1: 0x%.2x\r\n", reg[0], reg[1]);
    }

    HAL_XSPI_GetDelayValue(&hxspi1, &cal);
    EXTMEM_LOG("cal delay: 0x%.2x, fine: 0x%.2x, coarse 0x%.2x, max 0x%.2x\r\n", cal.DelayValueType, cal.FineCalibrationUnit, cal.CoarseCalibrationUnit, cal.MaxCalibration);
    return 0;
}

int32_t EXTMEM_PSRAM_EnterXIP(void)
{
    uint32_t ReadLatencyVal = 7;
    uint32_t WriteLatencyVal = 7;

    /*Configure Memory Mapped mode*/
    if (APS256XX_OK != APS256XX_EnableMemoryMappedMode(&hxspi1, ReadLatencyVal, WriteLatencyVal, HAL_XSPI_DATA_16_LINES, 0)) // Liner Burst
    {
        EXTMEM_LOG("PSRAM XIP Fail\r\n");
        return -1;
    }
    else
    {
        EXTMEM_LOG("PSRAM XIP Success\r\n");
        return 0;
    }
}
/* USER CODE END 1 */
