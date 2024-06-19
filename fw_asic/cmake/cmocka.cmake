include(FetchContent)

FetchContent_Declare(
  cmocka_lib
  GIT_REPOSITORY https://gitlab.com/cmocka/cmocka.git
  GIT_TAG          master # 22264569de1a894991b8876caf420b09bab1aa46 Commit has support for disabling building shared lib option
)

FetchContent_GetProperties(cmocka_lib)
if(NOT cmocka_lib_POPULATED)
  FetchContent_Populate(cmocka_lib)

  # No harm in having static library for host with shared lib support.
  option(BUILD_SHARED_LIBS "Shared library won't work for no OS ELF embedded targets" OFF)
  # option(WITH_STATIC_LIB "Static library for embedded targets" On)
  option(WITH_EXAMPLES "Don't need the cmocka examples built" Off)

  if(CMAKE_CROSSCOMPILING)
  option(HAVE_SIGLONGJMP "Not implemented on target" OFF)
  option(HAVE_GCC_THREAD_LOCAL_STORAGE "Not implemented on target" OFF)
  option(HAVE_CLOCK_GETTIME "Not implemented on target" OFF)
  endif()

  add_subdirectory(${cmocka_lib_SOURCE_DIR} ${cmocka_lib_BINARY_DIR})

  # Contains useful function for adding a CMocka test
  include(${cmocka_lib_SOURCE_DIR}/cmake/Modules/AddCMockaTest.cmake)
endif()
