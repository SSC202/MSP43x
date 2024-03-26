###################
#   MSP430 系列 GCC 编译下载用 CMakeLists.txt
#   1. https://www.ti.com.cn/tool/cn/MSP430-GCC-OPENSOURCE GCC 编译链
#   2. https://www.ti.com.cn/tool/cn/MSP430-FLASHER?keyMatch=MSP430FLASHER#downloads FLASH 烧录器
#   
#
#   配置以下项
#   1. DEVICE           小写
#   2. DEVICE_DEFINE    __大写__
#   3. DRIVER_LIBRARY   driverlib库路径
#   4. SUPPORT_LIBRARY  gcc头文件路径
#   5. FLASHER          烧录器路径
#
#
#   使用 make 命令进行编译，使用 build 路径下的 cmake --build . --target flash 命令进行烧录
###################
cmake_minimum_required(VERSION 3.12)
set(CMAKE_SYSTEM_NAME Generic)
project(MSP430 LANGUAGES C)

# 设置芯片型号
set(DEVICE "msp430f5529")
set(DEVICE_DEFINE "__MSP430F5529__")

# 设置预处理宏定义
add_definitions("-D${DEVICE_DEFINE}")

# 设置库文件路径
set(DRIVER_LIBRARY "E:/STM32SSC/TI/MSP430/msp430_driverlib/driverlib/MSP430F5xx_6xx")
set(SUPPORT_LIBRARY "D:/ti/msp430-gcc/include")

# 设置编译器
set(CMAKE_C_COMPILER msp430-elf-gcc)
set(CMAKE_OBJCOPY msp430-elf-objcopy) 
set(CMAKE_SIZE msp430-elf-size)
set(CMAKE_GDB msp430-elf-gdb)

# 设置烧录器
set(FLASHER "D:/ti/MSPFlasher_1.3.20/MSP430Flasher.exe")

# 设置编译选项
set(MCU_FLAGS "-mmcu=${DEVICE}")
set(CMAKE_C_FLAGS "${MCU_FLAGS} -w -g -O2")
set(CMAKE_C_FLAGS_DEBUG "-O2 -g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# 设置头文件路径inc
include_directories(
    ${DRIVER_LIBRARY}
    ${SUPPORT_LIBRARY}
)

# 设置源文件
file(GLOB DRIVER_SOURCES "${DRIVER_LIBRARY}/*.c")
file(GLOB SOURCES "src/*.c")

# 设置链接选项
set(LINKER_SCRIPT "${SUPPORT_LIBRARY}/${DEVICE}.ld")
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -L ${SUPPORT_LIBRARY} -T ${LINKER_SCRIPT}")

# 生成可执行文件
add_executable(${PROJECT_NAME}.elf "${DRIVER_SOURCES}" "${SOURCES}")

# .elf 转 .hex
add_custom_command(
    TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} -t ${PROJECT_NAME}.hex
    COMMENT "To hexFile..."
)

# .hex 烧录
add_custom_command(
    OUTPUT flasher
    COMMAND ${FLASHER} -w "${PROJECT_NAME}.hex" -v -g -z [VCC] -n "${DEVICE}"
    COMMENT "To board flash"
)

add_custom_target(flash
    DEPENDS flasher
)