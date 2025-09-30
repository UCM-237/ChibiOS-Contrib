/*
    Copyright (C) 2025	Jose Hinojosa

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

/**
 * @file    NRF5/LLD/TWISv1/hal_i2c_lld.c
 * @brief   NRF52 I2C subsystem low level driver source with slave implementation.
 *
 * @addtogroup I2C
 * @{
 */

#include "osal.h"
#include "hal.h"
#include "nrf_delay.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define I2C_PIN_CNF(internal_pullup) \
      ((GPIO_PIN_CNF_SENSE_Disabled  << GPIO_PIN_CNF_SENSE_Pos) \
      | (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos) \
      | (((internal_pullup) ? GPIO_PIN_CNF_PULL_Pullup : GPIO_PIN_CNF_PULL_Disabled)  << GPIO_PIN_CNF_PULL_Pos)  \
      | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos) \
      | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos))

#define I2C_PIN_CNF_CLR(internal_pullup) \
      ((GPIO_PIN_CNF_SENSE_Disabled  << GPIO_PIN_CNF_SENSE_Pos) \
      | (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos) \
      | (((internal_pullup) ? GPIO_PIN_CNF_PULL_Pullup : GPIO_PIN_CNF_PULL_Disabled)  << GPIO_PIN_CNF_PULL_Pos)  \
      | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos) \
      | (GPIO_PIN_CNF_DIR_Output     << GPIO_PIN_CNF_DIR_Pos))

#if NRF5_I2C_USE_I2C0
#define I2C0_IRQ_NUM     SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn
#define I2C0_IRQ_PRI     NRF5_I2C_I2C0_IRQ_PRIORITY
#endif
#if NRF5_I2C_USE_I2C1
#define I2C1_IRQ_NUM     SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn
#define I2C1_IRQ_PRI     NRF5_I2C_I2C1_IRQ_PRIORITY
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   I2C0 driver identifier.
 */
#if NRF5_I2C_USE_I2C0 || defined(__DOXYGEN__)
I2CDriver I2CD1;
#endif

/**
 * @brief   I2C1 driver identifier.
 */
#if NRF5_I2C_USE_I2C1 || defined(__DOXYGEN__)
I2CDriver I2CD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static uint8_t* nrf52_i2cs_txbuf = NULL;
static uint8_t* nrf52_i2cs_rxbuf = NULL;
static size_t nrf52_i2cs_txbytes = 0;
static size_t nrf52_i2cs_rxbytes = 0;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if defined(__GNUC__)
__attribute__((noinline))
#endif
/**
 * @brief   Common IRQ handler.
 * @note    Tries hard to clear all the pending interrupt sources, we don't
 *          want to go through the whole ISR and have another interrupt soon
 *          after.
 *
 * @param[in] i2cp         pointer to an I2CDriver
 */
static void i2c_serve_interrupt(I2CDriver *i2cp) {
  NRF_TWIS_Type *i2c = i2cp->i2c;

  // TODO
  if (i2c->EVENTS_ERROR) 
  {
    uint32_t err = i2c->ERRORSRC;
    i2c->EVENTS_ERROR = 0;
    (void)i2c->EVENTS_ERROR;

    if (err & 0x01)	// nRF52832 Product Specification v1.3 p.314 TWIM_ERRORSRC OVERRUN bit = 0x01
      i2cp->errors |= I2C_OVERRUN;
    if (err & (TWIM_ERRORSRC_ANACK_Msk | TWIM_ERRORSRC_DNACK_Msk))
      i2cp->errors |= I2C_ACK_FAILURE;

    i2c->TASKS_STOP = 1;

    _i2c_wakeup_error_isr(i2cp);
  } 
  else if(i2c->EVENTS_STOPPED) 
  {
    i2c->EVENTS_STOPPED = 0;
    (void)i2c->EVENTS_STOPPED;

    _i2c_wakeup_isr(i2cp);
  }
  else if (i2c->EVENTS_READ) 
  {
    // Read from slave
    if (nrf52_i2cs_txbuf != NULL && nrf52_i2cs_txbytes > 0)
    {
      i2c->TXD.PTR = (uint32_t)nrf52_i2cs_txbuf;
      i2c->TXD.MAXCNT = nrf52_i2cs_txbytes;
      
      i2c->TASKS_PREPARETX = 1;
    
      _i2c_wakeup_isr(i2cp);
    }
  }
  else if (i2c->EVENTS_WRITE) 
  {
    if (nrf52_i2cs_rxbuf != NULL && nrf52_i2cs_rxbytes > 0)
    {
      // Write to slave
      i2c->TXD.PTR = (uint32_t)nrf52_i2cs_rxbuf;
      i2c->TXD.MAXCNT = nrf52_i2cs_rxbytes;
      
      i2c->TASKS_PREPARERX = 1;

      _i2c_wakeup_isr(i2cp);
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if NRF5_I2C_USE_I2C0 || defined(__DOXYGEN__)

OSAL_IRQ_HANDLER(Vector4C) {

  OSAL_IRQ_PROLOGUE();
  i2c_serve_interrupt(&I2CD1);
  OSAL_IRQ_EPILOGUE();
}

#endif

#if NRF5_I2C_USE_I2C1 || defined(__DOXYGEN__)

OSAL_IRQ_HANDLER(Vector50) {

  OSAL_IRQ_PROLOGUE();
  i2c_serve_interrupt(&I2CD2);
  OSAL_IRQ_EPILOGUE();
}

#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {

#if NRF5_I2C_USE_I2C0
  i2cObjectInit(&I2CD1);
  I2CD1.thread = NULL;
  I2CD1.i2c = NRF_TWIS0;
#endif

#if NRF5_I2C_USE_I2C1
  i2cObjectInit(&I2CD2);
  I2CD2.thread = NULL;
  I2CD2.i2c = NRF_TWIS1;
#endif

}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_start(I2CDriver *i2cp) {
  NRF_TWIS_Type *i2c = i2cp->i2c;

  const I2CConfig *cfg = i2cp->config;

  if (i2cp->state != I2C_STOP)
    return;

  osalDbgAssert(i2c->ENABLE == 0, "already in use");

  IOPORT1->PIN_CNF[cfg->scl_pad] = I2C_PIN_CNF(cfg->scl_pullup);
  IOPORT1->PIN_CNF[cfg->sda_pad] = I2C_PIN_CNF(cfg->sda_pullup);

  i2c->SHORTS = 0;

  i2c->EVENTS_STOPPED = 0;
  i2c->EVENTS_ERROR = 0;
  (void)i2c->EVENTS_STOPPED;
  (void)i2c->EVENTS_ERROR;

  i2c->PSEL.SCL = cfg->scl_pad;
  i2c->PSEL.SDA = cfg->sda_pad;

#if NRF5_I2C_USE_I2C0
  nvicEnableVector(I2C0_IRQ_NUM, I2C0_IRQ_PRI);
#endif

#if NRF5_I2C_USE_I2C1
  nvicEnableVector(I2C1_IRQ_NUM, I2C1_IRQ_PRI);
#endif

  i2c->INTENSET = TWIS_INTENSET_STOPPED_Msk | TWIS_INTENSET_ERROR_Msk | TWIS_INTENSET_READ_Msk | TWIS_INTENSET_WRITE_Msk;

  i2c->ENABLE = TWIS_ENABLE_ENABLE_Enabled << TWIS_ENABLE_ENABLE_Pos;
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp) {
  NRF_TWIS_Type *i2c = i2cp->i2c;
  const I2CConfig *cfg = i2cp->config;

  if (i2cp->state != I2C_STOP) {
	i2c->SHORTS = 0;

    i2c->INTENCLR = TWIS_INTENCLR_STOPPED_Msk | TWIS_INTENCLR_ERROR_Msk | TWIS_INTENCLR_READ_Msk | TWIS_INTENCLR_WRITE_Msk;

#if NRF5_I2C_USE_I2C0
    nvicDisableVector(I2C0_IRQ_NUM);
#endif

#if NRF5_I2C_USE_I2C1
    nvicDisableVector(I2C1_IRQ_NUM);
#endif

    i2c->ENABLE = TWIS_ENABLE_ENABLE_Disabled << TWIS_ENABLE_ENABLE_Pos;

    IOPORT1->PIN_CNF[cfg->scl_pad] = I2C_PIN_CNF_CLR(cfg->scl_pullup);
    IOPORT1->PIN_CNF[cfg->sda_pad] = I2C_PIN_CNF_CLR(cfg->sda_pullup);
  }
}

void i2c_lld_slave_handler_set(I2CDriver *i2cp,
	uint8_t *txbuf, size_t txbytes,
	uint8_t *rxbuf, size_t rxbytes) {

  nrf52_i2cs_txbuf = txbuf;
  nrf52_i2cs_rxbuf = rxbuf;
  nrf52_i2cs_txbytes = txbytes;
  nrf52_i2cs_rxbytes = rxbytes;
}

/**
 * @brief   Listen I2C bus for address match.
 * @details Use 7 bit address
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 *
 * @notapi
 */
void i2c_lld_set_addr0(I2CDriver *i2cp, i2caddr_t addr) {

  NRF_TWIS_Type *i2c = i2cp->i2c;
  i2c->ADDRESS[0] = addr;
}


/**
 * @brief   Receives data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
	uint8_t *rxbuf, size_t rxbytes,
	systime_t timeout) {

	return MSG_RESET;
}

/**
* @brief   Transmits data via the I2C bus as master.
*
* @param[in] i2cp      pointer to the @p I2CDriver object
* @param[in] addr      slave device address
* @param[in] txbuf     pointer to the transmit buffer
* @param[in] txbytes   number of bytes to be transmitted
* @param[out] rxbuf    pointer to the receive buffer
* @param[in] rxbytes   number of bytes to be received
* @param[in] timeout   the number of ticks before the operation timeouts,
*                      the following special values are allowed:
*                      - @a TIME_INFINITE no timeout.
*                      .
* @return              The operation status.
* @retval MSG_OK       if the function succeeded.
* @retval MSG_RESET    if one or more I2C errors occurred, the errors can
*                      be retrieved using @p i2cGetErrors().
* @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
*                      timeout the driver must be stopped and restarted
*                      because the bus is in an uncertain state</b>.
*
* @notapi
*/
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
	 const uint8_t *txbuf, size_t txbytes,
	 uint8_t *rxbuf, size_t rxbytes,
	 systime_t timeout) {

	return MSG_RESET;
}

#endif /* HAL_USE_I2C */

/** @} */
