/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the UWB Mini3sPlus Module with STM32F103T8U6
 * Based on YCHIOT UWB Mini3sPlus datasheet
 */

/*
 * Board identifier.
 */
#define BOARD_UWB_MINI3SPLUS
#define BOARD_NAME              "UWB Mini3sPlus Module"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 * STM32F103T8U6 is a 64KB Flash variant
 * Only xB (128KB Flash) is defined, but it's identical to the
 * x8 version (64KB Flash) except for the Flash region size in the
 * linker script. For x8 parts use xB here and change to the x8 linker
 * script in the project Makefile.
 */
#define STM32F103xB

/*
 * IO pins assignments for UWB Mini3sPlus
 * Based on the datasheet IO allocation table
 */

/* DW1000 UWB chip connections */
#define GPIOA_DW_RSTn           0       // DW1000 reset
#define GPIOA_USB_EN            3       // USB enable
#define GPIOA_DW_NSS            4       // DW1000 SPI NSS
#define GPIOA_DW_SCK            5       // DW1000 SPI SCK
#define GPIOA_DW_MISO           6       // DW1000 SPI MISO
#define GPIOA_DW_MOSI           7       // DW1000 SPI MOSI
#define GPIOA_DW_EXTON          8       // DW1000 external clock
#define GPIOA_USART1_TX         9       // USART1 TX
#define GPIOA_USART1_RX         10      // USART1 RX
#define GPIOA_USBDM             11      // USB D-
#define GPIOA_USBDP             12      // USB D+
#define GPIOA_SWDIO             13      // SWD interface
#define GPIOA_SWCLK             14      // SWD interface

#define GPIOB_DW_WUP            0       // DW1000 wakeup
#define GPIOB_BOOT1             2       // Boot1 pin
#define GPIOB_DW_IRQN           5       // DW1000 interrupt
#define GPIOB_LED1              6       // LED1 (controllable)


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 * PA0  - DW_RSTn (output push-pull 2MHz)
 * PA1  - Input with pull-up
 * PA2  - Input with pull-up  
 * PA3  - USB_EN (output push-pull 2MHz)
 * PA4  - DW_NSS (output push-pull 50MHz)
 * PA5  - DW_SCK (alternate push-pull 50MHz)
 * PA6  - DW_MISO (Digital input with PullUp or PullDown resistor)
 * PA7  - DW_MOSI (alternate push-pull 50MHz)
 * PA8  - DW_EXTON (output push-pull 2MHz)
 * PA9  - USART1_TX (alternate push-pull 50MHz)
 * PA10 - USART1_RX (input with pull-up)
 * PA11 - USB_DM (input with pull-up)
 * PA12 - USB_DP (input with pull-up)
 * PA13 - SWDIO (input with pull-up)
 * PA14 - SWCLK (input with pull-up)
 * PA15 - Input with pull-up
 */
#define VAL_GPIOACRL            0xB4B38833      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x888888B8      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFFFFFF

/*
 * Port B setup.
 * PB0  - DW_WUP (output push-pull 2MHz)
 * PB1  - Input with pull-up
 * PB2  - BOOT1 (input with pull-up)
 * PB3  - Input with pull-up
 * PB4  - Input with pull-up
 * PB5  - DW_IRQN (input with pull-up)
 * PB6  - LED1 (output push-pull 2MHz)
 * PB7  - Input with pull-up
 * PB8-PB15 - Input with pull-up
 */
#define VAL_GPIOBCRL            0x82888882      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0x88888888      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFFFFFF

/*
 * Port C setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOCCRL            0x88888888      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x88888888      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFFFF

/*
 * Port D setup.
 * Everything input with pull-up except:
 * PD0  - Normal input (XTAL).
 * PD1  - Normal input (XTAL).
 */
#define VAL_GPIODCRL            0x88888844      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x88888888      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x88888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

/*
 * USB bus activation macro, required by the USB driver.
 */
#define usb_lld_connect_bus(usbp)	/* always connected */

/*
 * USB bus de-activation macro, required by the USB driver.
 */
#define usb_lld_disconnect_bus(usbp)	/* always connected */

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
