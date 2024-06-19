#pragma once

/**
 * @brief Asic function result
 */
typedef enum { kAsiceERR = -1, kAsiceSuccess } asicState;

asicState asic_setAddress(uint32_t address);
