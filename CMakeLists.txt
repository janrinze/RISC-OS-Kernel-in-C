cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabi-g++)
set(CMAKE_C_LINK_FLAGS "-nostartfiles -nostdlib -fno-zero-initialized-in-bss -static -g -march=armv8-a -Os -o rom.elf -static -T ${CMAKE_SOURCE_DIR}/rom.script -Os" )
project(RISC-OS-Kernel-in-C LANGUAGES C)
set(LINKER_SCRIPT "rom.script")

add_compile_options( -mcpu=cortex-a53 -fno-pic -Os -marm -nostartfiles -nostdlib -fno-zero-initialized-in-bss -static ${CMAKE_SOURCE_DIR}/rom.script )

add_custom_command(
  OUTPUT ro_rom.o
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate_ro_rom.sh
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate_ro_rom.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
#add_custom_command(
#  OUTPUT rom.bin
#  COMMAND arm-linux-gnueabihf-objcopy -R .ignoring -O binary RISC-OS-Kernel-in-C rom.bin
#  DEPENDS RISC-OS-Kernel-in-C
#  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#)

add_executable(RISC-OS-Kernel-in-C
    ./MMU/ARMv6/mmu.c
    ./TaskSlot/simple/task_slot.c
    ./Modules/modules.c
    ./swis/varvals.c
    ./swis/swis.c
    ./swis/os_heap.c
    ./swis/plot.c
    ./swis/vdu.c
    ./memory/simple/memory_manager.c
    ./BUILD/CMakeFiles/3.21.3/CompilerIdC/CMakeCCompilerId.c
    ./processor/ARMv6/processor.c
    ./kernel/kernel.c
    ./boot/generic/boot.c
    ro_rom.o
)
add_custom_target(rom.bin 
		COMMAND arm-linux-gnueabihf-objcopy -R .ignoring -O binary RISC-OS-Kernel-in-C rom.bin
		DEPENDS RISC-OS-Kernel-in-C
		)

#-T ../rom.script -nostartfiles -nostdlib -fno-zero-initialized-in-bss -static -g -march=armv8-a -Os
add_link_options( -fno-pic -nostartfiles -nostdlib -fno-zero-initialized-in-bss -static -g -march=armv8-a -Os -o rom.elf -static -T ${CMAKE_SOURCE_DIR}/rom.script -Os )
#set_property(TARGET "RISC-OS-Kernel-in-C" APPEND_STRING PROPERTIES LINK_FLAGS "-static -T ${CMAKE_SOURCE_DIR}/rom.script -Os")
target_include_directories(RISC-OS-Kernel-in-C PRIVATE ./Legacy ./debug ./kernel ./boot/generic ./memory/simple ./MMU/ARMv6 ./processor/ARMv6 ./swis ./TaskSlot/simple )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/rom.bin
  DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/rom.bin
  )
