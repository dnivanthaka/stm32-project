# Makefile for blink demo

#TOOLS = arm-linux-gnu
TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as
# Use the -g flag if you intend to use gdb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb, added -Os flag to optimize code generation
CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb -g -Os

#LD = $(TOOLS)-gcc
LD = $(TOOLS)-ld.bfd
OBJCOPY = $(TOOLS)-objcopy
DUMP = $(TOOLS)-objdump -d
GDB = $(TOOLS)-gdb

OBJS = start.o startup.o gpio.o rcc.o interrupts.o usart.o main.o spi.o i2c.o st7735.o keypad.o

all: main.bin main.dump

main.dump:	main.elf
	$(DUMP) main.elf >main.dump

main.bin:	main.elf
	$(OBJCOPY) main.elf main.bin -O binary

main.elf: 	$(OBJS)
	$(LD) -Aarch=armv7-m -T main.lds -o main.elf $(OBJS)

start.o:	start.s
	$(AS) start.s -o start.o

startup.o:	startup.c
	$(CC) -c startup.c

main.o:	main.c
	$(CC) -c main.c

rcc.o:	rcc.c
	$(CC) -c rcc.c

interrupts.o:	interrupts.c
	$(CC) -c interrupts.c

spi.o:	spi.c
	$(CC) -c spi.c

usart.o: usart.c
	$(CC) -c usart.c

gpio.o:	gpio.c
	$(CC) -c gpio.c

i2c.o:	i2c.c
	$(CC) -c i2c.c

st7735.o:	st7735.c
	$(CC) -c st7735.c

keypad.o:	keypad.c
	$(CC) -c keypad.c

clean:
	rm -f *.o main.elf main.bin main.dump
