#include <stdbool.h>
#include <stdint.h>

#include "asic_common.h"
#include "asic_pwm.h"
#include "asic_regs.h"
#include "asic_spi.h"

/**
 * @brief Force PWM sync signal high
 *
 * @return asicState
 */
asicState asic_pwm_sync(void) {
  static const uint16_t asic_pwm_sync = 1 << 15;
  uint16_t data;
  asicState state = asic_read(REG_PWM_CONFIG, &data);
  if (kAsiceSuccess > state) {
    return state;
  }

  data |= asic_pwm_sync;
  return asic_write(REG_PWM_CONFIG, data);
}

/**
 * @brief Configure short detection
 *
 * @param enable [in] Disable hi-z of transducers when short is detected
 * @param sc_filter [in] Number of PWM clocks to trigger after
 * @return asicState
 */
asicState asic_pwm_short_circuit_protection_control(bool enable, uint16_t sc_filter) {
  static const uint16_t DISABLE_SC_PROTECTION = 1 << 6;
  static const uint16_t sc_mask = 0x003F;
  uint16_t data = (enable ? 0x0000 : DISABLE_SC_PROTECTION);
  data |= (sc_filter & sc_mask);
  return asic_write(REG_SHORT_CONFIG, data);
}

/**
 * @brief Clear short circuit flag
 *
 * @return asicState
 */
asicState asic_pwm_short_circuit_clear(void) {
  static const uint16_t CLEAR_SHORTS = 0xFFFF;
  return asic_write(REG_SHORT_DETECT, CLEAR_SHORTS);
}

/**
 * @brief Get short detections
 *
 * @param shorts [in/out] Pointer to shorts reading
 * @return asicState
 */
asicState asic_pwm_short_circuit_get(uint16_t* shorts) {
  return asic_read(REG_SHORT_DETECT, shorts);
}

/**
 * @brief Set delay on PWM channel
 *
 * @param delay [in] Delay value
 * @param channel [in] PWM channel
 * @param sync [in] Force PWM sync high after writing
 * @return asicState
 */
asicState asic_pwm_delay_set(uint16_t delay, uint16_t channel, bool sync) {
  uint16_t address = REG_PWM0_DELAY + (channel * 2);
  asicState state = asic_write(address, delay);

  if (sync) {
    asic_pwm_sync();
  }
  return state;
}

/**
 * @brief Set duty on PWM channel
 *
 * @param duty [in] Duty value
 * @param channel [in] PWM channel
 * @param sync [in] Force PWM sync high after writing
 * @return asicState
 */
asicState asic_pwm_duty_set(uint16_t duty, uint16_t channel, bool sync) {
  uint16_t address = REG_PWM0_DUTY + (channel * 2);
  asicState state = asic_write(address, duty);

  if (sync) {
    asic_pwm_sync();
  }
  return state;
}

/**
 * @brief Set hi-z state on all channels
 *
 * @param enable_highz [in] Set or clear hi-z
 * @return asicState
 */
asicState asic_pwm_set_highz(bool enable_highz) {
  uint16_t highz_state = (enable_highz ? 0x0000 : 0xFFFF);
  asicState state = asic_write(REG_PWM_OE, highz_state);
  asic_pwm_sync();

  return state;
}

/**
 * @brief Set PWM state on all channels
 *
 * @param enable_PWM [in] Enable PWM state
 * @return asicState
 */
asicState asic_pwm_enable(bool enable_PWM) {
  uint16_t pwm_output_state = (enable_PWM ? 0xFFFF : 0x0000);
  asicState state = asic_write(REG_PWM_EN, pwm_output_state);
  asic_pwm_sync();

  return state;
}

/**
 * @brief Configure PWM
 *
 * @param enable_linear_mode [in] Enable linear mode
 * @param enable_count_from_centre [in] Enable count from centre
 * @return asicState
 */
asicState asic_pwm_set_config(bool enable_linear_mode, bool enable_count_from_centre) {
  static const uint16_t DITHER_SEED_COMMIT = 1 << 11;
  static const uint16_t LINEAR_MODE_DISABLE = 1 << 14;
  static const uint16_t COUNT_FROM_CENTRE_ENABLE = 1 << 12;
  static const uint16_t DITHER_SEED_UPPER = 0x00A9;
  static const uint16_t DITHER_SEED_LOWER = 0x0001;
  uint16_t data = DITHER_SEED_COMMIT | (DITHER_SEED_UPPER << 2) | DITHER_SEED_LOWER;

  if (!enable_linear_mode) {
    data |= LINEAR_MODE_DISABLE;
  }

  if (enable_count_from_centre) {
    data |= COUNT_FROM_CENTRE_ENABLE;
  }

  asicState state = asic_write(REG_PWM_CONFIG, data);
  asic_pwm_sync();
  return state;
}

/**
 * @brief Enable PWM dither
 *
 * @param enable_asic_pwm_dither [in] PWM dither
 * @return asicState
 */
asicState asic_pwm_dither(bool enable_asic_pwm_dither) {
  uint16_t data = (enable_asic_pwm_dither ? 0xFFFF : 0x0000);
  asicState state = asic_write(REG_asic_pwm_dither, data);
  asic_pwm_sync();
  return state;
}

/**
 * @brief Initialise PWM
 *
 * @param enable_sc [in] Enable short circuit detection
 * @param sc_filter [in] Number of PWM clocks for short detection
 * @param enable_linear_mode [in] Enable linear mode
 * @param enable_asic_pwm_dither [in] Enable PWM dither
 * @param enable_count_from_centre [in] Enable count from centre
 * @return asicState
 */
asicState asic_pwm_init(bool enable_sc, uint8_t sc_filter, bool enable_linear_mode,
                        bool enable_asic_pwm_dither, bool enable_count_from_centre) {
  if ((kAsiceSuccess != asic_pwm_short_circuit_protection_control(enable_sc, sc_filter)) ||
      (kAsiceSuccess != asic_pwm_short_circuit_clear()) ||
      (kAsiceSuccess != asic_pwm_set_highz(false)) ||
      (kAsiceSuccess != asic_pwm_set_config(enable_linear_mode, enable_count_from_centre)) ||
      (kAsiceSuccess != asic_pwm_dither(enable_asic_pwm_dither)) ||
      (kAsiceSuccess != asic_pwm_enable(true))) {
    return kAsiceERR;
  }
  return kAsiceSuccess;
}