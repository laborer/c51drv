MODULES	:= common uart print stc/eeprom
OBJECTS := main

ifeq ($(filter STC%, $(TARGET)), )
    $(warning Cannot build this demo for non-STC microcontroller)
    all:
else
    include ../Makefile.common
endif
