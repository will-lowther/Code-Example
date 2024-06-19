cmake_minimum_required(VERSION 3.10)

project(fw_asic C)

add_library(${PROJECT_NAME})

#Bring in project dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CMSIS)

target_include_directories(${PROJECT_NAME} PUBLIC 
"inc"
)

target_sources(${PROJECT_NAME} PRIVATE
"src/asic_adc.c"
"src/asic_gpio.c"
"src/asic_pwm.c"
"src/asic_spi.c"
)

add_dependencies(${PROJECT_NAME} cmsis)
target_link_libraries(${PROJECT_NAME} PRIVATE
cmsis
)

# Enable unit tests
option(UNIT_TESTS "Unit Tests" OFF)
if (UNIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()