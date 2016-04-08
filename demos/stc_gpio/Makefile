MODULES	:= tools uart print
# BINARY := $(BUILDDIR)/main.ihx
# OBJECTS := main

ifeq ($(filter STC%, $(TARGET)), )
    $(warning Cannot build this demo for non-STC microcontroller)
    all:
else ifneq ($(filter STC89%, $(TARGET)), )
    $(warning Cannot build this demo for STC89 series)
    all:
else
    include ../Makefile.common
endif
