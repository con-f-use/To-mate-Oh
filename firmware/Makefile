#!/usr/bin make
# Indent: 4 spaces, Encoding: UTF-8, Language: Make (eng), Breaks: linux
###############################################################################
#
# This file is part of To-mate-Oh.
# Copyright © 2014 con-f-use.
# Use under GNU General Public License v3+.
#
####################### Edit this to match your programmer anc micro controller

PROGRAMMER = usbtiny	# Your programmer here
CPU =        attiny25	# Works with attiny45 and 85 as well

####################### Technicalities (you should not have to edit below here)

F_CPU =      8000000UL  # In MHz

FUSE_L = 0xe2
FUSE_H = 0xdf
FUSE_E = 0xff

TARGET= To-mate-Oh
SOURCE= ${TARGET}.c
OBJECT= ${TARGET}.o
ELF=    ${TARGET}.elf
HEX=    ${TARGET}.hex

CC = avr-gcc -Os -Wall -Wextra -DF_CPU=$(F_CPU)

# symbolic targets:
.PHONY: help flash flashfuse getfuse setfuse clean hex

# print a help text
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build hex"
	@echo "make flashfuse . to flash fuses and firmware"
	@echo "make setfuse ... to flash the fuses"
	@echo "make getfuses .. get device current fuse bits"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects"

# flash the micro controller with the program
flash: ${HEX}
	avrdude -c ${PROGRAMMER} -p ${CPU} -U flash:w:${HEX}:i

# read fuesbits from the micro controller
getfuse:
	avrdude -c ${PROGRAMMER} -p ${CPU} -U lfuse:r:-:h 2> /dev/null
	avrdude -c ${PROGRAMMER} -p ${CPU} -U hfuse:r:-:h 2> /dev/null
	avrdude -c ${PROGRAMMER} -p ${CPU} -U efuse:r:-:h 2> /dev/null

# programm fuse bits to the micro controller (dangerous)
setfuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L, FUSE_H and FUSE_E!"; exit 1; }
	avrdude -c ${PROGRAMMER} -p ${CPU} -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

# clean up everyting but the sources
clean:
	rm -rf ${ELF} ${OBJECT} ${HEX}

flashfuse: setfuse flash

${OBJECT}: ${SOURCE} Makefile
	${CC} -mmcu=${CPU} -c ${SOURCE}

${ELF}: ${OBJECT}
	${CC} -mmcu=${CPU} -o ${ELF} ${OBJECT}

hex: ${HEX}

${HEX}: ${ELF}
	avr-objcopy -j .text -j .data -O ihex ${ELF} ${HEX} && \
	avr-size -C --mcu=${CPU} ${ELF}
