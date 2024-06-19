#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>

#include "asic_spi.h"

asic_spi_struct g_spi_struct = {0};
ARM_DRIVER_SPI* g_spi = {0};
/* ---------------------------------- Mocks --------------------------------- */

/* ---------------------------------- Tests --------------------------------- */

static void test_asic_spi_init(void** state) {
  (void)state; /* Unused */

  /* Null pointers */
  asicState status = asic_initSPI(NULL, NULL);
  assert_int_equal(status, kAsiceERR);

  status = asic_initSPI(&g_spi_struct, NULL);
  assert_int_equal(status, kAsiceERR);

  g_spi_struct.spi = g_spi;
  status = asic_initSPI(&g_spi_struct, NULL);
  assert_int_equal(status, kAsiceSuccess);
}

static void test_asic_spi_basic_write(void** state) {
  (void)state; /* Unused */
}

static void test_asic_spi_basic_read(void** state) {
  (void)state; /* Unused */
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_asic_spi_init),
      cmocka_unit_test(test_asic_spi_basic_write),
      cmocka_unit_test(test_asic_spi_basic_read),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
