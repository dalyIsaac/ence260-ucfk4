# File:   Makefile
# Author: Isaac Daly (idd17@uclive.ac.nz)
# Author: Divyean Sivarman (dsi3@uclive.ac.nz)
# Date:   2018-10-16
# Descr:  Makefile for game
# Version: 0.3

# Definitions.
CC = avr-gcc
CFLAGS = -std=c99 -mmcu=atmega32u2 -Os -Wall -Werror -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ../../drivers/avr/pio.h ../../drivers/avr/system.h  ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

board.o: board.c ../../drivers/avr/pio.h ../../drivers/avr/system.h  
	$(CC) -c $(CFLAGS) $< -o $@

puck.o: puck.c  ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

ball.o: ball.c  ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/display.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../utils/task.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
game.out: game.o board.o puck.o ball.o ledmat.o display.o pio.o system.o timer.o navswitch.o task.o tinygl.o font.o pacer.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


