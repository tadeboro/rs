### CONFIGURATION ######################################################

STM_COMMON := /home/tadej/studij/faks/s2-racunalniski-sistemi/STM32F4-Discovery_FW_V1.1.0

# Toolchain
CC      := arm-none-eabi-gcc
AS      := arm-none-eabi-as
OBJCOPY := arm-none-eabi-objcopy

# Common flags for compiler and linker
FLAGS  = -g -O2 -Wall
FLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
FLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Compiler flags
CFLAGS  = $(FLAGS) -DUSE_STDPERIPH_DRIVER -DSTM32F4XX
CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -I.

# Linker flags
LDFLAGS = $(FLAGS) -T../common/stm32_flash.ld

# Sources and objects
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
OBJS += ../common/startup_stm32f4xx.o

### RULES ##############################################################

all: $(NAME).elf

# Create binary image
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

# Create hex dump
%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

# Create elf file
%.elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

### CLEANUP AND FLASH COMMANDS #########################################

.PHONY: clean
clean:
	rm -f *.hex *.bin *.elf $(OBJS) .gdbinit

.PHONY: burn
burn: $(NAME).bin
	st-flash write $(NAME).bin 0x8000000

.PHONY: gdb
gdb: $(NAME).elf $(NAME).bin .gdbinit
	st-util &> /tmp/st-util.log &
	gdb

.gdbinit:
	sed -e 's/FNAME/$(NAME).elf/' ../common/gdbinit > $@
