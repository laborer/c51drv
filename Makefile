# Try to use sdcc in the system.  If sdcc is not installed, find sdcc
# at ~/Downloads/sdcc/bin/sdcc
SDCC		:= $(if $(shell which sdcc), sdcc, ~/Downloads/sdcc/bin/sdcc)

# Find where sdcc is, so we can also locate packihx and makebin there
SDCCBINDIR	:= $(shell $(SDCC) --print-search-dirs | sed -n '/^programs:$$/{n;p}')

# The name of the directory which holds all the compile binaries
BUILDDIR	:= $(CURDIR)/build
MODULESDIR	:= $(BUILDDIR)/modules
DEMOSDIR	:= $(BUILDDIR)/demos

# Set the model name of the target MCU
TARGET		?= STC89C52RC
# TARGET		?= STC12C5A16S2
# TARGET		?= AT89C51

# Specify demos to compile
DEMOS		:= tools uart timer print iic spi irrc5 irnec led7seg kbhost
DEMOS		+= ds1820 rom9346 rom2402 lcd1602 pcf8591 ds1302 dht11 hcsr04
DEMOS		+= stc_wdt stc_gpio stc_adc stc_pca stc_eeprom stc_autoisp stc_spi stc_uart2
DEMOS		+= watchdog thermometer pinlogger uartecho pinmonitor command

# Enable AutoISP for STC MCUs
AUTOISP		?= yes

# Set frequency of the oscillator
# SDCCFLAGS	+= -DFOSC=11059200L

# Disable some unnecessary warnings
SDCCFLAGS	+= --less-pedantic

# Tell C program the name of the target MCU model
SDCCFLAGS	+= -DTARGET_MODEL_$(subst +,_,$(TARGET))

# Header directory
SDCCFLAGS	+= -I$(CURDIR)/modules

# Set flags for certain MCUs
ifneq ($(filter STC%, $(TARGET)), )
    SDCCFLAGS	+= -DMICROCONTROLLER_8052
    # If AutoISP is enabled, register autoisp_check as a UART callback
    # function
    SDCCFLAGS	+= $(if $(filter yes, $(AUTOISP)), -DUART_CALLBACK=autoisp_check)
    # The following modules and test cases only work for non-STC89C
    # series MCUs
    ifneq ($(filter-out STC89%, $(TARGET)), )
        SDCCFLAGS += -DTICKS=1 -DCYCLES_MOV_R_N=2 -DCYCLES_DJNZ_R_E=4
    endif
else
    SDCCFLAGS	+= -DMICROCONTROLLER_8051
    ifneq ($(filter yes, $(AUTOISP)), )
        $(warning AutoISP can only be used on STC microcontrollers)
        AUTOISP	:= no
    endif
endif

# Set memory usage limit for some known MCUs
ifneq ($(filter $(TARGET), AT89C51), )
    ASLINKFLAGS	+= --code-size 4096 --xram-size 128
else ifneq ($(filter $(TARGET), STC89C52RC AT89S52), )
    ASLINKFLAGS	+= --code-size 8192 --xram-size 256
else ifneq ($(filter $(TARGET), STC89C54RD+ STC12C5A16S2), )
    ASLINKFLAGS	+= --code-size 16384 --xram-size 1024
endif

export TARGET
export SDCC
export SDCCFLAGS
export ASLINKFLAGS
export AUTOISP
export MODULESDIR
export DEMOSDIR

.PHONY: all modules $(DEMOS) srcinfo clean

all: $(DEMOS)

modules:
	$(MAKE) -C modules

$(DEMOS): modules
	$(MAKE) -C $(@:%=demos/%)

srcinfo:
	etags $(shell find modules/ -name "*.[hc]")
	etags -a $(shell find demos/ -name "*.[hc]")
	$(SDCC) --print-search-dirs \
	| sed -n '/^includedir:$$/,/^libdir:$$/{/\// {s/^/-I/;p}}' \
	> .clang_complete
	echo -I./modules/ >>.clang_complete

# Clean up
clean:
	rm -rf $(BUILDDIR)/*
