/*
    Copyright (C) 2021 UCM-237

    Licensed under the GPL License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.gnu.org/licenses/old-licenses/gpl-2.0.html

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/* Board identifier. */
#define BOARD_DWM1001_DEV
#define BOARD_NAME              "DWM1001-DEV"

/* Board oscillators-related settings. */
#define NRF5_XTAL_VALUE        32000000
#define NRF5_HFCLK_SOURCE      NRF5_HFCLK_HFXO
#define NRF5_LFCLK_SOURCE      NRF5_LFCLK_XTAL

#define NRF5_HFCLK_HFINT       0
#define NRF5_HFCLK_HFXO        1

#define NRF5_LFCLK_RC          0
#define NRF5_LFCLK_XTAL        1
#define NRF5_LFCLK_SYNTH       2

/*
 * GPIO pins.
 */

#define SW1              21U
#define SW2               2U
#define GREEN_LED_D9     30U
#define BLUE_LED_D10     31U
#define RED_LED_D11      22U
#define RED_LED_D12      14U
#define DW_IRQ           19U
#define DW_RST           24U
#define SPI_SCK          16U
#define SPI_MOSI         20U
#define SPI_MISO         18U
#define SPI_SS           17U
#define UART_TX			  5U
#define UART_RX          11U
#define I2C_SCL           8U
#define I2C_SDA          15U


/*
 * IO pins assignments.
 */

#define IOPORT1_SW1            21U
#define IOPORT1_SW2             2U
#define IOPORT1_GREEN_LED_D9   30U
#define IOPORT1_BLUE_LED_D10   31U
#define IOPORT1_RED_LED_D11    22U
#define IOPORT1_RED_LED_D12    14U
#define IOPORT1_DW_IRQ         19U
#define IOPORT1_DW_RST         24U
#define IOPORT1_SPI_SCK        16U
#define IOPORT1_SPI_MOSI       20U
#define IOPORT1_SPI_MISO       18U
#define IOPORT1_SPI_SS         17U
#define IOPORT1_UART_TX         5U
#define IOPORT1_UART_RX        11U
#define IOPORT1_I2C_SCL         8U
#define IOPORT1_I2C_SDA        15U

/*
 * IO lines assignments.
 */

#define LINE_SW1              PAL_LINE(IOPORT1, IOPORT1_SW1)
#define LINE_SW2              PAL_LINE(IOPORT1, IOPORT1_SW2)
#define LINE_GREEN_LED_D9     PAL_LINE(IOPORT1, IOPORT1_GREEN_LED_D9)
#define LINE_BLUE_LED_D10     PAL_LINE(IOPORT1, IOPORT1_BLUE_LED_D10)
#define LINE_RED_LED_D11      PAL_LINE(IOPORT1, IOPORT1_RED_LED_D11)
#define LINE_RED_LED_D12      PAL_LINE(IOPORT1, IOPORT1_RED_LED_D12)
#define LINE_DW_IRQ           PAL_LINE(IOPORT1, IOPORT1_DW_IRQ)
#define LINE_DW_RST           PAL_LINE(IOPORT1, IOPORT1_DW_RST)
#define LINE_SPI_SCK          PAL_LINE(IOPORT1, IOPORT1_SPI_SCK)
#define LINE_SPI_MOSI         PAL_LINE(IOPORT1, IOPORT1_SPI_MOSI)
#define LINE_SPI_MISO         PAL_LINE(IOPORT1, IOPORT1_SPI_MISO)
#define LINE_SPI_SS           PAL_LINE(IOPORT1, IOPORT1_SPI_SS)
#define LINE_UART_TX          PAL_LINE(IOPORT1, IOPORT1_UART_TX)
#define LINE_UART_RX          PAL_LINE(IOPORT1, IOPORT1_UART_RX)
#define LINE_I2C_SCL          PAL_LINE(IOPORT1, IOPORT1_I2C_SCL)
#define LINE_I2C_SDA          PAL_LINE(IOPORT1, IOPORT1_I2C_SDA)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */