#pragma once

#include "asic_common.h"

/**
 * @brief Output for GPIO
 */
typedef enum {
  kGPIOOutsel_GPIO,
  kGPIOOutsel_CRCError,
  kGPIOOutsel_SCDetect,
  // 0x3 = RESERVED
  kGPIOOutsel_PWMSyncToggle = 0x4,
  kGPIOOutsel_ADCSyncToggle,
  kGPIOOutsel_LVDSData,
  // 0x7 = RESERVED
  kGPIOOutsel_LVDSClk = 0x8,
  kGPIOOutsel_SysClk,
  kGPIOOutsel_PWMClk,
  kGPIOOutsel_ADCClk,
  kGPIOOutsel_PWMGPIO0,
  kGPIOOutsel_PWMGPIO1,
  kGPIOOutsel_PWMGPIO2,
  kGPIOOutsel_PWMGPIO3
} GpioOutSel;

/**
 * @brief Bitwise channel selection
 *
 */
typedef enum {
  kGPIO_None = 0x00,
  kGPIO0 = 0x01,
  kGPIO1 = 0x02,
  kGPIO2 = 0x04,
  kGPIO3 = 0x08,
  kGPIO_All = 0x0F
} GpioChanReg;

void asic_gpio_init(void);
asicState asic_gpio_output_enable(uint16_t gpio_bit_mask);
asicState asic_gpio_output_select(uint8_t gpio_channel, GpioOutSel outsel);
asicState asic_gpio_write(uint16_t gpio_chan_reg);
asicState asic_gpio_read(uint16_t* data);
void asic_pwm_mux_select(uint16_t gpio_pwm_channel, uint16_t channel);
