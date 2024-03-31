# Jacob Chisholm
# Makefile for compiling and uploading bare metal code
# to STMF446 with BlackMagic Probe + GDB

CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=build/$@.map
SOURCES = main.c
COMPORT = COM5

build: firmware.elf firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary build/$< build/$@

firmware.elf: $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o build/firmware.elf

st-flash: build/firmware.bin
	./Tools/ST-LINK_CLI.exe -P build/firmware.bin 0x08000000 -V -RST
st-reset:
	./Tools/ST-LINK_CLI.exe -RST

# must run "run" command or restart target to start program
flash:
	gdb -ex 'target extended-remote $(COMPORT)' \
	-ex 'set mem inaccessible-by-default off' \
	-ex 'monitor swdp' \
	-ex 'attach 1' \
	-ex 'load firmware.elf' \
	-q

clean:
	rm -rf build/*