#pragma once
#include <stdint.h>

#include "Driver_SPI.h"
#include "asic_common.h"
#include "asic_regs.h"

typedef struct {
  ARM_DRIVER_SPI* spi;
  void (*lockSem)(void);
  void (*unlockSem)(void);
  void (*setCS)(void);
  void (*clearCS)(void);
} asic_spi_struct;

asicState asic_initSPI(asic_spi_struct* spi_struct, void (*callback)(uint32_t event));
asicState asic_write(asicReg reg, uint16_t data);
asicState asic_read(asicReg reg, uint16_t* data);
