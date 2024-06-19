#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "asic_common.h"

asicState asic_pwm_sync(void);
asicState asic_pwm_short_circuit_protection_control(bool enable, uint16_t sc_filter);
asicState asic_pwm_short_circuit_clear(void);
asicState asic_pwm_short_circuit_get(uint16_t* shorts);
asicState asic_pwm_delay_set(uint16_t delay, uint16_t channel, bool sync);
asicState asic_pwm_duty_set(uint16_t duty, uint16_t channel, bool sync);
asicState asic_pwm_set_highz(bool enable_highz);
asicState asic_pwm_enable(bool enable_PWM);
asicState asic_pwm_set_config(bool enable_linear_mode, bool enable_count_from_centre);
asicState asic_pwm_dither(bool enable_asic_pwm_dither);
asicState asic_pwm_init(bool enable_sc, uint8_t sc_filter, bool enable_linear_mode,
                        bool enable_asic_pwm_dither, bool enable_count_from_centre);