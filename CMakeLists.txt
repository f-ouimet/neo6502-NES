cmake_minimum_required(VERSION 3.13)

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

# internal c execs
add_executable(nes_emu src/main.c src/mem.c)

# add Luke Wren's PicoDVI project to lib/picodvi (clone as picodvi/)
add_subdirectory(lib/picodvi/software/libdvi) # PICODVI LIBRARY

# fat fs is added using INTERFACE, make sure to define your TINYUSB folder
# environment variable
add_library(fatfs INTERFACE)
target_sources(
  fatfs
  INTERFACE ${tinyusb_src_dir}/lib/fatfs/source/ff.c
            ${tinyusb_src_dir}/lib/fatfs/source/ffsystem.c
            ${tinyusb_src_dir}/lib/fatfs/source/ffunicode.c)
target_include_directories(fatfs INTERFACE ${tinyusb_src_dir}/lib/fatfs/source/)

target_include_directories(
  nes_emu PRIVATE src/ include/ # find header files in src
)

# pull in picostdlib and other libraries
target_link_libraries(
  nes_emu
  PUBLIC pico_stdlib
         hardware_pwm
         hardware_clocks
         pico_multicore
         tinyusb_host
         hardware_dma
         hardware_pio
         libdvi
         tinyusb_board
         fatfs)

# debug over UART since tinyusb takes control of usb
pico_enable_stdio_uart(nes_emu 0)

# create bin file, hex file, etc.
pico_add_extra_outputs(nes_emu)
