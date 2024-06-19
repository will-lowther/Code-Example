#include "Driver_SPI.h"
#include "asic_common.h"
#include "asic_regs.h"
#include "asic_spi.h"

static asic_spi_struct* asic_spi_struct = NULL;
static uint32_t asicAddress = 0;

static const uint32_t kAsicSpeed = 15000000;  // Hz
static const uint32_t kSPIConfig = ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA0 |
                                   ARM_SPI_DATA_BITS(29) | ARM_SPI_MSB_LSB |
                                   ARM_SPI_SS_MASTER_UNUSED;
volatile bool busy_flag = false;

static void default_callback(uint32_t event) {
  switch (event) {
    case ARM_SPI_EVENT_TRANSFER_COMPLETE:
      asic_spi_struct->clearCS();
      busy_flag = false;
      break;
    case ARM_SPI_EVENT_DATA_LOST:
      /*  Occurs in slave mode when data is requested/sent by master
          but send/receive/transfer operation has not been started
          and indicates that data is lost. Occurs also in master mode
          when driver cannot transfer data fast enough. */
      while (1) {
        /* Do nothing */
      }
      /* TODO: Add some error handling */
      break;
    case ARM_SPI_EVENT_MODE_FAULT:
      /*  Occurs in master mode when Slave Select is deactivated and
          indicates Master Mode Fault. */
      while (1) {
        /* Do nothing */
      }
      /* TODO: Add some error handling */
      break;
  }
}

static void lock(void) {
  while (busy_flag) {
    /* Do nothing */
  }
  busy_flag = true;
}

static void unlock(void) {
  busy_flag = false;
}

/**
 * @brief Initialise the SPI peripheral
 *
 * @param spi_struct [in] SPI handle
 * @param callback [in] Interrupt callback
 * @return asicState
 */
asicState asic_initSPI(asic_spi_struct* spi_struct, void (*callback)(uint32_t event)) {
  asic_spi_struct = spi_struct;
  if ((NULL == spi_struct) || (NULL == spi_struct->spi)) {
    return kAsiceERR;
  }

  if ((NULL == spi_struct->setCS) || (NULL == spi_struct->clearCS)) {
    /* CS control should be set up externally */
    return kAsiceERR;
  }

  if ((NULL == spi_struct->lockSem) || (NULL == spi_struct->unlockSem)) {
    /* Use non RTOS flag system */
    asic_spi_struct->lockSem = lock;
    asic_spi_struct->unlockSem = unlock;
  }

  if (NULL == callback) {
    callback = default_callback;
  }

  ARM_DRIVER_SPI* spi = asic_spi_struct->spi;
  if ((ARM_DRIVER_OK != spi->Initialize(callback)) ||
      (ARM_DRIVER_OK != spi->PowerControl(ARM_POWER_FULL)) ||
      (ARM_DRIVER_OK != spi->Control(kSPIConfig, kAsicSpeed))) {
    return kAsiceERR;
  }
  return kAsiceSuccess;
}

/**
 * @brief Set address of asic on chain
 *
 * @param address [in] Asic address
 * @return asicState
 */
asicState asic_setAddress(uint32_t address) {
  asicAddress = address;
  return kAsiceSuccess;
}

/**
 * @brief Write to asic register
 *
 * @param reg [in] Asic register
 * @param data [in] Value to write
 * @return asicState
 */
asicState asic_write(asicReg reg, uint16_t data) {
  /*
   * 28:26 - ADev, Device address
   * 25:18 - AReg, Register address
   * 17:16 - SPIOp, SPI read/write mode
   * 15:0 - WD, Write data
   */
  static const uint32_t write_bit = 0x00;
  uint32_t packet = asicAddress;
  packet = (packet << 8) | (uint32_t)reg;
  packet = (packet << 2) | write_bit;
  packet = (packet << 16) | data;

  ARM_DRIVER_SPI* spi = asic_spi_struct->spi;
  int32_t status;
  asic_spi_struct->lockSem();
  /*
   * The asic SPI needs resetting for every transaction. This is achieved by
   * deasserting the CS and sending a couple of clock pulses down sclk.
   * Sadly it seems you can't reconfigure the transaction size without reinit
   * and causing lines to go high and low. Just send a full transaction with CS
   * high.
   */
  status = spi->Send((const void*)&packet, 1);

  asic_spi_struct->lockSem();
  asic_spi_struct->setCS();
  status = spi->Send((const void*)&packet, 1);

  if (ARM_DRIVER_OK != status) {
    return kAsiceERR;
  }
  return kAsiceSuccess;
}

/**
 * @brief Read from asic register
 *
 * @param reg [in] Asic register
 * @param data [in/out] data pointer
 * @return asicState
 */
asicState asic_read(asicReg reg, uint16_t* data) {
  /*
   * 28:26 - ADev, Device address
   * 25:18 - AReg, Register address
   * 17:16 - SPIOp, SPI read/write mode
   * 15:0 - RD, Read data
   */
  static const uint32_t read_bit = 0x01;
  uint32_t packet = asicAddress;
  packet = (packet << 8) | (uint32_t)reg;
  packet = (packet << 2) | read_bit;
  packet = (packet << 16);

  ARM_DRIVER_SPI* spi = asic_spi_struct->spi;
  uint32_t read_data;
  int32_t status;
  asic_spi_struct->lockSem();
  /*
   * The asic SPI needs resetting for every transaction. This is achieved by
   * deasserting the CS and sending a couple of clock pulses down sclk.
   * Sadly it seems you can't reconfigure the transaction size without reinit
   * and causing lines to go high and low. Just send a full transaction with CS
   * high.
   */
  status = spi->Transfer((const void*)&packet, (void*)&read_data, 1);

  asic_spi_struct->lockSem();
  asic_spi_struct->setCS();
  status = spi->Transfer((const void*)&packet, (void*)&read_data, 1);
  asic_spi_struct->lockSem();
  asic_spi_struct->unlockSem();

  if (ARM_DRIVER_OK != status) {
    return kAsiceERR;
  }

  *data = (uint16_t)(read_data & 0xFFFF);
  return kAsiceSuccess;
}
