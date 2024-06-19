cmake_minimum_required(VERSION 3.10)

set(ENV{GIT_LFS_SKIP_SMUDGE} 1)

set(GITLAB_CI_JOB_TOKEN $ENV{CI_JOB_TOKEN})
if (GITLAB_CI_JOB_TOKEN)
    set(gitlab_root [REDACTED])
else ()
    set(gitlab_root [REDACTED])
endif ()

include(FetchContent)
FetchContent_Declare(
  cmsis
  GIT_REPOSITORY ${gitlab_root}[REDACTED]
  GIT_TAG        5.7.0
  GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(cmsis)
if(NOT cmsis_POPULATED)
  message("Downloading CMSIS dependency...")
  FetchContent_Populate(cmsis)
  set(CMSIS_INC ${cmsis_SOURCE_DIR}/CMSIS/Core/Include)
  set(CMSIS_DRIVER_INC ${cmsis_SOURCE_DIR}/CMSIS/Driver/Include)

  add_library(cmsis INTERFACE) # CMSIS only contains header (.h) files

  target_include_directories(cmsis INTERFACE
  ${CMSIS_INC}
  ${CMSIS_DRIVER_INC}
  )
endif()

