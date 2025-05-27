cmake_minimum_required(VERSION 3.5)

# Pull in PICO SDK
include(pico_sdk_import.cmake)

project(audio_blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Init SDK
pico_sdk_init()

add_executable(audio_blink audio_blink.c)

# pull in picostdlib
target_link_libraries(audio_blink pico_stdlib hardware_pwm)

# create bin file
pico_add_extra_outputs(audio_blink)
