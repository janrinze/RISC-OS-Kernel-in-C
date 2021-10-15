cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_C_LINK_FLAGS "-nostartfiles -nostdlib -fno-zero-initialized-in-bss -static -g -march=armv8-a -Os -o rom.elf -static -T ${CMAKE_SOURCE_DIR}/rom.script -Os" )
project(RISC-OS-Kernel-in-C LANGUAGES C)
set(LINKER_SCRIPT "rom.script")

add_compile_options( -fno-pic -Os -marm -nostartfiles -nostdlib -fno-zero-initialized-in-bss -static -march=armv8-a ${CMAKE_SOURCE_DIR}/rom.script )


add_executable(RISC-OS-Kernel-in-C ./MMU/ARMv6/mmu.c
    ./TaskSlot/simple/task_slot.c
    ./Modules/modules.c
    ./swis/swis.c
    ./swis/os_heap.c
    ./memory/simple/memory_manager.c
    ./processor/ARMv6/processor.c
    ./kernel/kernel.c
    ./boot/generic/boot.c ./ro_rom.o
)

#-T ../rom.script -nostartfiles -nostdlib -fno-zero-initialized-in-bss -static -g -march=armv8-a -Os
#add_link_options( -fno-pic -nostartfiles -nostdlib -fno-zero-initialized-in-bss -static -g -march=armv8-a -Os -o rom.elf -static -T ${CMAKE_SOURCE_DIR}/rom.script -Os )
#set_property(TARGET "RISC-OS-Kernel-in-C" APPEND_STRING PROPERTIES LINK_FLAGS "-static -T ${CMAKE_SOURCE_DIR}/rom.script -Os")
target_include_directories(RISC-OS-Kernel-in-C PRIVATE ./kernel ./boot/generic ./memory/simple ./MMU/ARMv6 ./processor/ARMv6 ./swis ./TaskSlot/simple )
