#############################################################################
#
# Makefile for librf24 examples on Raspberry Pi
#
# License: GPL (General Public License)
# Author:  gnulnulf <arco@appeltaart.mine.nu>
# Date:    2013/02/07 (version 1.0)
#
# Description:
# ------------
# use make all and make install to install the examples
# You can change the install directory by editing the prefix line
#
# $@   The file name of the target
# $%   The target member name
# $<   The name of the first prerequisite
# $?   The names of all the prerequisites that are newer than the target
# $^   The names of all the prerequisites
#
prefix := /usr/local

# Detect the Raspberry Pi by the existence of the bcm_host.h file
BCMLOC=/opt/vc/include/bcm_host.h

ifneq ("$(wildcard $(BCMLOC))","")
# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
endif

# define all programs
PROGRAMS = lofi_rx
SOURCES = ${PROGRAMS:=.cpp}

all: ${PROGRAMS}

lofi_rx: lofi_rx.cpp RF24.o bcm2835.o spi.o
	g++ -c ${CCFLAGS} -Wall $@.cpp
	g++ -o $@ lofi_rx.o RF24.o bcm2835.o spi.o

#	g++ ${CCFLAGS} -Wall -I../ -lrf24-bcm $@.cpp -o $@

RF24.o: RF24.cpp	
	g++ -Wall -fPIC ${CCFLAGS} -c $^

bcm2835.o: bcm2835.c
	gcc -Wall -fPIC ${CCFLAGS} -c $^

spi.o: spi.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c $^

compatibility.o: $(DRIVER_DIR)/compatibility.c
	gcc -Wall -fPIC  ${CCFLAGS} -c $(DRIVER_DIR)/compatibility.c

gpio.o: $(DRIVER_DIR)/gpio.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c $(DRIVER_DIR)/gpio.cpp
	
tags:
	ctags lofi_rx.cpp RF24.cpp bcm2835.c spi.cpp RF24.h RF24_arch_config.h RF24_config.h bcm2835.h includes.h nRF24L01.h spi.h

clean:
	rm -rf $(PROGRAMS)

install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
	  install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install tags
