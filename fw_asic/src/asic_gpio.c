#include <stdint.h>

#include "asic_common.h"
#include "asic_gpio.h"
#include "asic_regs.h"
#include "asic_spi.h"

/**
 * @brief Enable Asics GPIO outputs
 *
 * @param gpio_bit_mask [in] (4 bits) Can be made from ORing GpioChanReg enums
 * @return asicState
 */
asicState asic_gpio_output_enable(uint16_t gpio_bit_mask) {
  return asic_write(REG_GPIO_OE, gpio_bit_mask);
}

/**
 * @brief Select what the GPIO output represents
 *
 * @param gpio_channel [in] 0 - 3 GPIO channel
 * @param outsel [in] Output selection
 * @return asicState
 */
asicState asic_gpio_output_select(uint8_t gpio_channel, GpioOutSel outsel) {
  if (gpio_channel > 3) {
    return kAsiceERR;
  }

  uint16_t gpio_channel_mask = (uint16_t)0xf << (4 * gpio_channel);
  uint16_t shifted_outsel = ((uint16_t)outsel & 0xf) << (4 * gpio_channel);
  uint16_t data;
  asicState state = asic_read(REG_GPIO_OUTSEL, &data);
  if (kAsiceSuccess > state) {
    return state;
  }

  data &= ~gpio_channel_mask;
  data |= shifted_outsel;
  return asic_write(REG_GPIO_OUTSEL, data);
}

/**
 * @brief State of outputs (only when outsel is GPIO (0))
 *
 * @param gpio_chan_reg [in] Output values (Can be made from ORing GpioChanReg)
 * @return asicState
 */
asicState asic_gpio_write(uint16_t gpio_chan_reg) {
  return asic_write(REG_GPIO_OUT, gpio_chan_reg);
}

void asic_gpio_init(void) {
  asic_gpio_write(kGPIO_None);
  for (uint8_t gpio = 0; gpio < 4; gpio++) {
    asic_gpio_output_select(gpio, kGPIOOutsel_GPIO);
  }
  asic_gpio_output_enable(kGPIO_All);
}

/**
 * @brief Read GPIO states
 *
 * @param data [in/out] GPIO states pointer
 * @return asicState
 */
asicState asic_gpio_read(uint16_t* data) {
  return asic_read(REG_GPIO_IN, data);
}

/**
 * @brief Configure pwm channel on output (only when outsel is PWM_GPIOx)
 *
 * @param gpio_channel [in] 0 - 3 GPIO Channel
 * @param channel [in] 0 - 15 PWM Channel
 * @return asicState
 */
asicState asic_pwm_mux_select(uint8_t gpio_channel, uint16_t channel) {
  uint16_t gpio_channel_mask = (uint16_t)0xf << (4 * gpio_channel);
  uint16_t shifted_mux_sel = ((uint16_t)channel & 0xf) << (4 * gpio_channel);
  uint16_t data;
  asicState state = asic_read(REG_PWM_GPIO_OUTSEL, &data);
  if (kAsiceSuccess > state) {
    return state;
  }

  data &= ~gpio_channel_mask;
  data |= shifted_mux_sel;
  return asic_write(REG_PWM_GPIO_OUTSEL, data);
}