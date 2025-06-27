cmake_minimum_required(VERSION 3.5)

# Pull in PICO SDK
include(pico_sdk_import.cmake)

project(nes_emu C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Init SDK
pico_sdk_init()

add_executable(nes_emu src/main.c src/mem.c)

target_include_directories(
  nes_emu PRIVATE src/ # find mem.h in the project root
)

# pull in picostdlib and others
target_link_libraries(nes_emu pico_stdlib hardware_pwm)

pico_enable_stdio_usb(nes_emu 1)
pico_enable_stdio_uart(nes_emu 0)

# create bin file, hex file, etc.
pico_add_extra_outputs(nes_emu)
