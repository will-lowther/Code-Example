#pragma once
#include "asic_common.h"

/**
 * @brief ADC channels
 *
 */
typedef enum {
  kADCChannel_hv,
  kADCChannel_5V,
  kADCChannel_VTemperature,
  kADCChannel_LoadSense,
  kADCChannel_1V8,
  kADCChannel_Unused,
  kADCChannel_TestLowGain,
  kADCChannel_TestHighGain,

  kADCChannel_Total
} ADCChannels;

asicState asic_adc_set_load_sense_timing(uint16_t charge_time, uint16_t measure_time);
asicState asic_adc_set_load_sense_config(uint16_t cap_trim);
asicState asic_adc_load_sense_sel(ADCChannels channel);
asicState asic_adc_load_sense_hold(void);
asicState asic_adc_sync(void);

asicState asic_adc_set_channel(ADCChannels channel);
asicState asic_adc_start_sample(void);
asicState asic_adc_get_value(uint16_t* reading);
bool asic_adc_ready(void);
asicState asic_adc_init(void);
