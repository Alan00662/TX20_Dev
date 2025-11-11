
#ifndef __SPI_SDCARD_H
#define __SPI_SDCARD_H

#include "main.h"

/* SPI定义 */
#define SDNAND_SPI                          SPI4
#define SDNAND_SPI_CLK_ENABLE()             do { __HAL_RCC_SPI4_CLK_ENABLE(); } while (0)
#define SDNAND_SPI_CLK_DISABLE()            do { __HAL_RCC_SPI4_CLK_DISABLE(); } while (0)
#define SDNAND_SPI_SCK_GPIO_PORT            GPIOE
#define SDNAND_SPI_SCK_GPIO_PIN             GPIO_PIN_12
#define SDNAND_SPI_SCK_GPIO_AF              GPIO_AF5_SPI4
#define SDNAND_SPI_SCK_GPIO_CLK_ENABLE()    do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0)
#define SDNAND_SPI_MOSI_GPIO_PORT           GPIOE
#define SDNAND_SPI_MOSI_GPIO_PIN            GPIO_PIN_14
#define SDNAND_SPI_MOSI_GPIO_AF             GPIO_AF5_SPI4
#define SDNAND_SPI_MOSI_GPIO_CLK_ENABLE()   do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0)
#define SDNAND_SPI_MISO_GPIO_PORT           GPIOE
#define SDNAND_SPI_MISO_GPIO_PIN            GPIO_PIN_13
#define SDNAND_SPI_MISO_GPIO_AF             GPIO_AF5_SPI4
#define SDNAND_SPI_MISO_GPIO_CLK_ENABLE()   do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0)

/* 引脚定义 */
#define SDNAND_CS_GPIO_PORT                 GPIOE
#define SDNAND_CS_GPIO_PIN                  GPIO_PIN_4
#define SDNAND_CS_GPIO_CLK_ENABLE()         do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0)

/* IO操作 */
#define SDNAND_CS(x)                        do { (x) ?                                                                      \
                                                HAL_GPIO_WritePin(SDNAND_CS_GPIO_PORT, SDNAND_CS_GPIO_PIN, GPIO_PIN_SET):   \
                                                HAL_GPIO_WritePin(SDNAND_CS_GPIO_PORT, SDNAND_CS_GPIO_PIN, GPIO_PIN_RESET); \
                                            } while (0)

/* SD NAND块大小定义 */
#define SDNAND_BLOCK_SIZE                   (512UL)

/* SD NAND信息类型定义 */
typedef struct {
    uint8_t csd[16];
    uint32_t chip_size;
    uint16_t block_size;
    uint32_t block_num;
    uint32_t logic_block_size;
    uint32_t logic_block_num;
} sdnand_info_t;

/* SD NAND信息导出 */
extern sdnand_info_t sdnand_info;

/* 函数声明 */
uint8_t spi_sdcard_init(void);                                                          /* 初始化SD NAND */
uint8_t sdnand_read_disk(uint8_t *data, uint32_t block_index, uint32_t block_num);  /* 读SD NAND指定数量块的数据 */
uint8_t sdnand_write_disk(uint8_t *data, uint32_t block_index, uint32_t block_num); /* 写SD NAND指定数量块的数据 */
void show_spi_sdcard_info(void);
#endif
