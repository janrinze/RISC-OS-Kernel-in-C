cmake_minimum_required(VERSION 3.5)

# choose the cross compiler
set(CMAKE_C_COMPILER arm-linux-gnueabi-gcc-11)

# define project name
project(RISC-OS-Kernel-in-C)

# use flags that setup bare metal code
add_compile_options( -mcpu=cortex-a53 -fno-pic -Os -marm -nostartfiles -nostdlib -fno-zero-initialized-in-bss -static ${CMAKE_SOURCE_DIR}/rom.script )
add_link_options( -fno-pic -nostartfiles -nostdlib -fno-zero-initialized-in-bss -static -g -march=armv8-a -Os -o rom.elf -static -T ${CMAKE_SOURCE_DIR}/rom.script -Os )

# run tool to build ro_rom.o from the binary rom provided by riscosopen
add_custom_command(
  OUTPUT ro_rom.o
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate_ro_rom.sh
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate_ro_rom.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# main target and dependencies
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
    ./processor/ARMv6/processor.c
    ./kernel/kernel.c
    ./boot/generic/boot.c
    ro_rom.o
)

# after building create the ROM from the executable
add_custom_command(TARGET RISC-OS-Kernel-in-C
		POST_BUILD
		COMMAND arm-linux-gnueabi-objcopy -R .ignoring -O binary RISC-OS-Kernel-in-C rom.bin
)

# make sure all headers can be found
target_include_directories(RISC-OS-Kernel-in-C PRIVATE ./Legacy ./debug ./kernel ./boot/generic ./memory/simple ./MMU/ARMv6 ./processor/ARMv6 ./swis ./TaskSlot/simple )
