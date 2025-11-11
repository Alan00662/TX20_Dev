/**
 ****************************************************************************************************
 * @file        usbd_msc_storage.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-05-21
 * @brief       USB MSC应用代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 * 
 * 实验平台:正点原子 H7R7开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 * 
 ****************************************************************************************************
 */

#include "usbd_msc_storage.h"

#include "sdmmc_sdcard.h"
#include "spi_sdcard.h"
/* 磁盘编号定义 */
#define LUN_NOR_FLASH   0   /* NOR Flash */
#define LUN_SD_NAND     1   /* SD NAND */
#define LUN_SD          2   /* SD */

/**
 * NOR Flash区域划分（最大扇区大小扇区对齐）
 *    名称      起始地址    大小
 *     代码区 0x00000000 0x00100000
 * 文件系统区 0x00100000 0x01800000
 *     字库区 0x01900000 0x00604000
 *     用户区 0x01F04000 0x000FC000
 */
#define NORFLASH_MSC_BASE   (0x00100000)
#define NORFLASH_MSC_SIZE   (0x01800000)

/* USB MSC状态标志
 * bit0: 磁盘写入标志
 * bit1: 磁盘读取标志
 * bit2: 磁盘写入出错标志
 * bit3: 磁盘读取错误标志
 * bit4: USB连接标志
 */
uint8_t g_usb_msc_state = 0;

/* USB MSC标准查询数据 */
int8_t  STORAGE_Inquirydata[] = {
    /* LUN_NOR_FLASH */
    0x00,
    0x80,
    0x02,
    0x02,
    (STANDARD_INQUIRY_DATA_LEN - 5),
    0x00,
    0x00,
    0x00,
    'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',
    'N', 'O', 'R', ' ', 'F', 'l', 'a', 's', 'h', ' ',
    'D', 'i', 's', 'k', ' ',
    '1', '.', '0', '0',
    
    /* LUN_SD_NAND */
    0x00,
    0x80,
    0x02,
    0x02,
    (STANDARD_INQUIRY_DATA_LEN - 5),
    0x00,
    0x00,
    0x00,
    'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',
    'S', 'D', ' ', 'N', 'A', 'N', 'D', ' ',
    'D', 'i', 's', 'k', ' ', ' ', ' ', 
    '1', '.', '0', '0',
    
    /* LUN_SD */
    0x00,
    0x80,
    0x02,
    0x02,
    (STANDARD_INQUIRY_DATA_LEN - 5),
    0x00,
    0x00,
    0x00,
    'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',
    'S', 'D', ' ', 'C', 'a', 'r', 'd', ' ',
    'D', 'i', 's', 'k', ' ', ' ', ' ', 
    '1', '.', '0', '0',
};

/**
 * @brief   磁盘初始化
 * @param   lun: 磁盘编号
 * @retval  初始化结果
 * @arg     0: 成功
 * @arg     -1: 失败
 */
int8_t STORAGE_Init(uint8_t lun)
{
    uint8_t res = 0;
    
    switch (lun)
    {
        case LUN_NOR_FLASH:
        {
            break;
        }
        case LUN_SD_NAND:
        {
            break;
        }
        case LUN_SD:
        {
            res = sd_init();
            break;
        }
    }
    
    if (res != 0)
    {
        return -1;
    }
    
    return 0;
}

/**
 * @brief   获取磁盘容量
 * @param   lun: 磁盘编号
 * @param   block_num: 块总量
 * @param   block_size: 块大小
 * @retval  获取结果
 * @arg     0: 成功
 * @arg     -1: 失败
 */
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    
    switch (lun)
    {
        case LUN_NOR_FLASH:
        {
            *block_num = NORFLASH_MSC_SIZE >> 9;
            *block_size = 512;
            break;
        }
        case LUN_SD_NAND:
        {
            *block_num = sdnand_info.logic_block_num - 1;
            *block_size = sdnand_info.logic_block_size;
            break;
        }
        case LUN_SD:
        {
            hal_status = HAL_SD_GetCardInfo(&g_sd_handle, &g_sd_card_info_struct);
            if (hal_status == HAL_OK)
            {
                *block_num = g_sd_card_info_struct.LogBlockNbr - 1;
                *block_size = g_sd_card_info_struct.LogBlockSize;
            }
            break;
        }
    }
    
    if (hal_status != HAL_OK)
    {
        return -1;
    }
    
    return 0;
}

/**
 * @brief   磁盘就绪检查
 * @param   lun: 磁盘编号
 * @retval  就绪状态
 * @arg     0: 就绪
 * @arg     -1: 未就绪
 */
int8_t STORAGE_IsReady(uint8_t lun)
{
    g_usb_msc_state |= (1 << 4);
    
    return 0;
}

/**
 * @brief   磁盘写保护检查
 * @param   lun: 磁盘编号
 * @retval  写保护状态
 * @arg     0: 失能
 * @arg     -1: 使能
 */
int8_t STORAGE_IsWriteProtected(uint8_t lun)
{
    return 0;
}

/**
 * @brief   从磁盘读取数据
 * @param   lun: 磁盘编号
 * @param   buf: 数据
 * @param   blk_addr: 块地址
 * @param   blk_len: 块数量
 * @retval  读取结果
 * @arg     0: 成功
 * @arg     -1: 失败
 */
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    uint8_t res = 0;
    
    g_usb_msc_state |= (1 << 1);
    switch (lun)
    {
        case LUN_NOR_FLASH:
        {
            // norflash_ex_read(NORFLASH_MSC_BASE + (blk_addr << 9), buf, blk_len << 9);
            break;
        }
        case LUN_SD_NAND:
        {
            res = sdnand_read_disk(buf, blk_addr, blk_len);
            break;
        }
        case LUN_SD:
        {
            res = sd_read_disk(buf, blk_addr, blk_len);
            break;
        }
    }
    
    if (res != 0)
    {
        g_usb_msc_state |= (1 << 3);
        return -1;
    }
    
    return 0;
}

/**
 * @brief   往磁盘写入数据
 * @param   lun: 磁盘编号
 * @param   buf: 数据
 * @param   blk_addr: 块地址
 * @param   blk_len: 块数量
 * @retval  写入结果
 * @arg     0: 成功
 * @arg     -1: 失败
 */
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    uint8_t res = 0;
    
    g_usb_msc_state |= (1 << 0);
    switch (lun)
    {
        case LUN_NOR_FLASH:
        {
            // norflash_ex_write(NORFLASH_MSC_BASE + (blk_addr << 9), buf, blk_len << 9);
            break;
        }
        case LUN_SD_NAND:
        {
            res = sdnand_write_disk(buf, blk_addr, blk_len);
            break;
        }
        case LUN_SD:
        {
            res = sd_write_disk(buf, blk_addr, blk_len);
            break;
        }
    }
    
    if (res != 0)
    {
        g_usb_msc_state |= (1 << 2);
        return -1;
    }
    
    return 0;
}

/**
 * @brief   获取最大磁盘编号
 * @param   无
 * @retval  最大磁盘编号
 */
int8_t STORAGE_GetMaxLun(void)
{
    /* 判断SD卡是否正常
     * 如果SD卡不正常，则支持NOR Flash、NAND Flash（0、1）
     * 如果SD卡正常，则支持NOR Flash、NAND Flash、SD（0、1、2）
     */
    if (sd_init() != 0)
    {
        return (MSC_STORAGE_LUN_NBR - 1 - 1);
    }
    else
    {
        return (MSC_STORAGE_LUN_NBR - 1);
    }
}

/* MSC操作函数集合 */
USBD_StorageTypeDef USBD_MSC_fops = {
    STORAGE_Init,
    STORAGE_GetCapacity,
    STORAGE_IsReady,
    STORAGE_IsWriteProtected,
    STORAGE_Read,
    STORAGE_Write,
    STORAGE_GetMaxLun,
    STORAGE_Inquirydata
};
