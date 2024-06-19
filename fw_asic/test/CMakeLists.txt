cmake_minimum_required(VERSION 3.10)

include(../cmake/cmocka.cmake)

list(APPEND tests_names "test_asic_spi")

# Declare all tests targets
add_cmocka_test(test_asic_spi
                SOURCES test_asic_spi.c
                # COMPILE_OPTIONS
                LINK_LIBRARIES fw_asic cmocka cmsis
                # LINK_OPTIONS
                )

set_tests_properties(test_asic_spi PROPERTIES ENVIRONMENT "CMOCKA_XML_FILE=test_asic_spi.xml;CMOCKA_MESSAGE_OUTPUT=xml")