cmake_minimum_required(VERSION 3.5)

# Pull in PICO SDK
include(pico_sdk_import.cmake)

project(nes_emu C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Init SDK
pico_sdk_init()

add_executable(nes_emu
    main.c
    memory.c
)

target_include_directories(nes_emu PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}   #find memory.h in the project root
)

# pull in picostdlib
target_link_libraries(nes_emu 
    pico_stdlib 
    hardware_pwm
)

# create bin file, hex file, etc.
pico_add_extra_outputs(nes_emu)
