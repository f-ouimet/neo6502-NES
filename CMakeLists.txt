cmake_minimum_required(VERSION 3.5)

# Pull in PICO SDK
include(pico_sdk_import.cmake)

project(nes_emu C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(DEFINED ENV{PICO_TINYUSB_PATH})
  set(tinyusb_src_dir $ENV{PICO_TINYUSB_PATH})
endif()
set(PICO_TINYUSB_PATH ${tinyusb_src_dir})

# Init SDK
pico_sdk_init()

add_executable(nes_emu src/main.c src/mem.c)

add_subdirectory(lib/picodvi/software/libdvi) # PICODVI LIBRARY

# pull in picostdlib and others
target_link_libraries(
  nes_emu
  PUBLIC pico_stdlib
         hardware_pwm
         tinyusb_host
         hardware_dma
         hardware_pio
         libdvi
         tinyusb_board)

add_library(fatfs INTERFACE)
target_sources(
  fatfs
  INTERFACE ${tinyusb_src_dir}/lib/fatfs/source/ff.c
            ${tinyusb_src_dir}/lib/fatfs/source/ffsystem.c
            ${tinyusb_src_dir}/lib/fatfs/source/ffunicode.c)
target_include_directories(fatfs INTERFACE ${tinyusb_src_dir}/lib/fatfs/source/)

target_include_directories(
  nes_emu PRIVATE src/ # find mem.h in the project root
)
pico_enable_stdio_uart(nes_emu 0)

# create bin file, hex file, etc.
pico_add_extra_outputs(nes_emu)
