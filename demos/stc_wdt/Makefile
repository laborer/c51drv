MODULES	:= common uart print
OBJECTS := main

ifeq ($(filter STC%, $(TARGET)), )
    $(warning Cannot build this demo for non-STC microcontroller)
    all:
else
    include ../Makefile.common
endif
