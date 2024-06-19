#include <stdbool.h>
#include <stdint.h>

#include "asic_adc.h"
#include "asic_common.h"
#include "asic_regs.h"
#include "asic_spi.h"

/**
 * @brief Initialise ADC bits
 *
 * @return asicState
 */
static asicState init_tbit(void) {
  /*
   * TBIT_SET_BIT_STOP <12:15> = 10
   * TBIT_SET_BIT_START <8:11> = 10
   * TBIT_CMP_EN         <4:7> = 10
   * TBIT_BIT_LEN        <0:3> = 10
   */
  static const uint16_t TBIT_CONFIG = 0xAAAA;
  asicState state = asic_write(REG_ADC_TBIT_CONFIG, TBIT_CONFIG);
  if (kAsiceSuccess != state) {
    return state;
  }

  /*
   * TBIT_ENABLE_OP <0:3> = 10
   * TBIT_LTCH      <4:7> = 10
   */
  static const uint16_t TBIT_START_TIMES = 0x00AA;
  return asic_write(REG_ADC_TBIT_START_TIMES, TBIT_START_TIMES);
}

/**
 * @brief Initialise pulse config
 *
 * @return asicState
 */
static asicState init_tconf(void) {
  asicState state = kAsiceERR;
  do {
    /*
     * PULSE_SH_START1 <0:5> = 16
     * PULSE_SH_STOP1 <6:11> = 31
     * PULSE_SH_START2 <0:5> = 36
     * PULSE_SH_STOP2 <6:11> = 51
     */
    static const uint16_t pulseStartStop1 = 0x07D0;
    static const uint16_t pulseStartStop2 = 0x1476;
    if ((kAsiceSuccess != asic_write(REG_ADC_PULSE_START_STOP1, pulseStartStop1)) ||
        (kAsiceSuccess != asic_write(REG_ADC_PULSE_START_STOP2, pulseStartStop2))) {
      break;
    }

    /*
     * PULSE_SH_SMALL_START1 <0:5> = 16
     * PULSE_SH_SMALL_STOP1 <6:11> = 32
     * PULSE_SH_SMALL_START2 <0:5> = 36
     * PULSE_SH_SMALL_STOP2 <6:11> = 52
     */
    static const uint16_t pulseSmallStartStop1 = 0x0810;
    static const uint16_t pulseSmallStartStop2 = 0x0D24;
    if ((kAsiceSuccess != asic_write(REG_ADC_PULSE_SMALL_START_STOP1, pulseSmallStartStop1)) ||
        (kAsiceSuccess != asic_write(REG_ADC_PULSE_SMALL_START_STOP2, pulseSmallStartStop2))) {
      break;
    }

    /*
     * PULSE_SHRT_IP_START1 <0:5> = 0
     * PULSE_SHRT_IP_STOP1 <6:11> = 33
     * PULSE_SHRT_IP_START2 <0:5> = 0
     * PULSE_SHRT_IP_STOP2 <6:11> = 0
     */
    static const uint16_t pulseShortIpStartStop1 = 0x0840;
    static const uint16_t pulseShortIpStartStop2 = 0x0000;
    if ((kAsiceSuccess != asic_write(REG_ADC_PULSE_SMALL_START_STOP1, pulseShortIpStartStop1)) ||
        (kAsiceSuccess != asic_write(REG_ADC_PULSE_SMALL_START_STOP2, pulseShortIpStartStop2))) {
      break;
    }

    /*
     * PULSE_AZ1_START1 <0:5> = 0
     * PULSE_AZ1_STOP1 <6:11> = 34
     * PULSE_AZ1_START2 <0:5> = 0
     * PULSE_AZ1_STOP2 <6:11> = 0
     */
    static const uint16_t pulseAz1StartStop1 = 0x0880;
    static const uint16_t pulseAz1StartStop2 = 0x0000;
    if ((kAsiceSuccess != asic_write(REG_ADC_PULSE_AZ1_START_STOP1, pulseAz1StartStop1)) ||
        (kAsiceSuccess != asic_write(REG_ADC_PULSE_AZ1_START_STOP2, pulseAz1StartStop2))) {
      break;
    }

    /*
     * PULSE_AZ2_START1 <0:5> = 0
     * PULSE_AZ2_STOP1 <6:11> = 35
     * PULSE_AZ2_START2 <0:5> = 0
     * PULSE_AZ2_STOP2 <6:11> = 0
     */
    static const uint16_t pulseAz2StartStop1 = 0x08C0;
    static const uint16_t pulseAz2StartStop2 = 0x0000;
    if ((kAsiceSuccess != asic_write(REG_ADC_PULSE_AZ2_START_STOP1, pulseAz2StartStop1)) ||
        (kAsiceSuccess != asic_write(REG_ADC_PULSE_AZ2_START_STOP2, pulseAz2StartStop2))) {
      break;
    }

    /*
     * PULSE_RESET_BIT_START1 <0:5> = 0
     * PULSE_RESET_BIT_STOP1 <6:11> = 2
     * PULSE_RESET_BIT_START2 <0:5> = 0
     * PULSE_RESET_BIT_STOP2 <6:11> = 0
     */
    static const uint16_t pulseResetBitStartStop1 = 0x0080;
    static const uint16_t pulseResetBitStartStop2 = 0x0000;
    if ((kAsiceSuccess != asic_write(REG_ADC_PULSE_RST_BIT_START_STOP1, pulseResetBitStartStop1)) ||
        (kAsiceSuccess != asic_write(REG_ADC_PULSE_RST_BIT_START_STOP2, pulseResetBitStartStop2))) {
      break;
    }

    /*
     * PULSE_RESET_LS_HALF_START1 <0:5> = 16
     * PULSE_RESET_LS_HALF_STOP1 <6:11> = 31
     * PULSE_RESET_LS_HALF_START2 <0:5> = 36
     * PULSE_RESET_LS_HALF_STOP2 <6:11> = 51
     */
    static const uint16_t pulseResetLsHalfStartStop1 = 0x07D0;
    static const uint16_t pulseResetLsHalfStartStop2 = 0x0CE4;
    if ((kAsiceSuccess !=
         asic_write(REG_ADC_PULSE_RST_HALF_START_STOP1, pulseResetLsHalfStartStop1)) ||
        (kAsiceSuccess !=
         asic_write(REG_ADC_PULSE_RST_HALF_START_STOP2, pulseResetLsHalfStartStop2))) {
      break;
    }

    /* Must be 53 (0x35) or less */
    static const uint16_t bitStart = 0x0035;
    if (kAsiceSuccess != asic_write(REG_ADC_BIT_START, bitStart)) {
      break;
    }

    state = kAsiceSuccess;
  } while (0);
  return state;
}

/**
 * @brief Set the ADC clock divider
 *
 * @param clock_divider[in] (4 bits) clock division
 * @return asicState
 */
asicState adc_set_clock_divider(uint16_t clock_divider) {
  return asic_write(REG_ADC_CLK, clock_divider);
}

/**
 * @brief Set load sense timings
 *
 * @param charge_time [in] Given in pwm pulses
 * @param measure_time [in] Given in pwm pulses
 * @return asicState
 */
asicState asic_adc_set_load_sense_timing(uint16_t charge_time, uint16_t measure_time) {
  uint16_t data = (measure_time << 8) | charge_time;
  return asic_write(REG_ADC_LOAD_SENSE_CONFIG, data);
}

/**
 * @brief Set load sense configuration
 *
 * @param pF_trim [in] Capacitance trim 2-9pF
 * @return asicState
 */
asicState asic_adc_set_load_sense_config(uint16_t pF_trim) {
  if ((pF_trim < 2) || (pF_trim > 9)) {
    return kAsiceERR;
  }
  // capacitance in pF - 2 = capacitance trim value
  pF_trim -= 2;
  return asic_write(REG_ANA_CONFIG_LOAD_SENSE, pF_trim);
}

/**
 * @brief Initialise the ADC.
 *
 * @return asicState
 */
asicState asic_adc_init(void) {
  static const uint16_t ADC_CLK_DIV = 0x0006;
  static const uint16_t CHARGE_TIME = 20;
  static const uint16_t MEASURE_TIME = 20;
  static const uint16_t CAP_TRIM = 9;

  if ((kAsiceSuccess != adc_set_clock_divider(ADC_CLK_DIV)) || (kAsiceSuccess != init_tbit()) ||
      (kAsiceSuccess != asic_adc_set_load_sense_timing(CHARGE_TIME, MEASURE_TIME)) ||
      (kAsiceSuccess != init_tconf()) ||
      (kAsiceSuccess != asic_adc_set_load_sense_config(CAP_TRIM))) {
    return kAsiceERR;
  }
  return kAsiceSuccess;
}

/**
 * @brief Start the ADC sampling
 *
 * @return asicState
 */
asicState asic_adc_start_sample(void) {
  static const uint16_t ADC_EN = 1 << 3;
  uint16_t data;
  asicState state = asic_read(REG_ADC_STATE, &data);
  if (kAsiceSuccess > state) {
    return state;
  }

  data |= ADC_EN;
  return asic_write(REG_ADC_STATE, data);
}

/**
 * @brief Generate ADC sync pulse
 *
 * @return asicState
 */
asicState asic_adc_sync(void) {
  static const uint16_t asic_adc_sync = 1 << 6;
  uint16_t data;
  asicState state = asic_read(REG_ADC_STATE, &data);
  if (kAsiceSuccess > state) {
    return state;
  }

  data |= asic_adc_sync;
  return asic_write(REG_ADC_STATE, data);
}

/**
 * @brief Sets the ADC Channel
 *
 * @param adc_channel [in] (3 bits) Requested channel
 * @return asicState
 */
asicState asic_adc_set_channel(ADCChannels adc_channel) {
  static const uint16_t channel_mask = 0x7;
  uint16_t data;
  asicState state = asic_read(REG_ADC_STATE, &data);
  if (kAsiceSuccess > state) {
    return state;
  }

  data &= ~channel_mask;
  data |= (uint16_t)adc_channel;
  return asic_write(REG_ADC_STATE, data);
}

/**
 * @brief Load sense hold
 *
 * @return asicState
 */
asicState asic_adc_load_sense_hold(void) {
  uint16_t data;
  asicState state = asic_read(REG_ADC_LOAD_SENSE_CONFIG, &data);
  if (kAsiceSuccess > state) {
    return state;
  }

  static const uint16_t BYTE_MASK = 0x00FF;
  uint16_t charge_time = data & BYTE_MASK;
  uint16_t measure_time = (data >> 8) & BYTE_MASK;

  /* TODO: I don't know what this is doing. It was part of the ESP code */
  uint16_t pulse_number = charge_time + measure_time + 3;
  for (uint16_t pulse = 0; pulse < pulse_number; pulse++) {
    if (kAsiceSuccess != asic_adc_sync()) {
      return kAsiceERR;
    }
  }
  return kAsiceSuccess;
}

/**
 * @brief ADC ready?
 *
 * @return true yes
 * @return false no
 */
bool asic_adc_ready(void) {
  uint16_t data = 0;
  asic_read(REG_ADC_STATE, &data);

  static const uint16_t ADC_DONE = 0x10;
  return (0 != (data & ADC_DONE));
}

/**
 * @brief Get the ADC reading
 *
 * @param reading [in/out] reading value
 * @return asicState
 */
asicState asic_adc_get_value(uint16_t* reading) {
  asicState state = asic_read(REG_ADC_VAL, reading);
  if (kAsiceSuccess > state) {
    return state;
  }

  return kAsiceSuccess;
}

/**
 * @brief Load sense channel
 *
 * @param channel [in] 0-7 Channel to load
 * @return asicState
 */
asicState asic_adc_load_sense_sel(ADCChannels channel) {
  /*
   * Only one bit must be set at a time to
   * avoid damaging the LOAD_SENSE block
   */
  return asic_write(REG_ADC_LOAD_SENSE, (1 << (uint16_t)channel));
}
