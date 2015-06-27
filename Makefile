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
ifndef TARGET
    TARGET	:= STC89C52RC
    # TARGET	:= STC12C5A16S2
    # TARGET	:= AT89C51
endif

# # Specify demos to compile
DEMOS		:= common tools uart timer print iic spi irrc5 irnec led7seg kbhost
DEMOS		+= ds1820 rom9346 rom2402 lcd1602 pcf8591 ds1302 dht11 hcsr04
DEMOS		+= stc_wdt stc_gpio stc_adc stc_pca stc_eeprom stc_autoisp stc_spi stc_uart2

# Enable AutoISP for STC MCUs
AUTOISP		:= yes

# Set frequency of the oscillator
# SDCCFLAGS	+= -DFOSC=11059200L

# Disable some unnecessary warnings
SDCCFLAGS	+= --less-pedantic

# Tell C program the name of the target MCU model
SDCCFLAGS	+= -DTARGET_MODEL_$(subst +,_,$(TARGET))

# Header directory
SDCCFLAGS	+= -I$(CURDIR)/src

# $(if $(findstring /test/, $@), -Isrc)

# Set flags for certain MCUs
ifneq ($(filter STC%, $(TARGET)), )
    SDCCFLAGS	+= -DMICROCONTROLLER_8052
    # If AutoISP is enabled, register autoisp_check as a UART callback
    # function
    SDCCFLAGS	+= $(if $(filter yes, $(AUTOISP)), -DUART_CALLBACK=autoisp_check) 
    # MODULES	+= stc/eeprom stc/autoisp
    # TESTS	+= stc/wdt
    # The following modules and test cases only work for non-STC89C
    # series MCUs
    ifneq ($(filter-out STC89%, $(TARGET)), )
        # TESTS	+= stc/gpio stc/adc stc/pca stc/spi stc/uart2
        SDCCFLAGS += -DTICKS=1 -DCYCLES_MOV_R_N=2 -DCYCLES_DJNZ_R_E=4
    endif
else
    SDCCFLAGS	+= -DMICROCONTROLLER_8051
    ifneq ($(filter yes, $(AUTOISP)), )
        $(warning Disable AutoISP for non-STC microcontrollers)
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

# DEMOSLIST	:= $(DEMOS:%=demos/%)

export TARGET
export SDCC
export SDCCFLAGS
export ASLINKFLAGS
export AUTOISP
export MODULESDIR
export DEMOSDIR

# Build a list of test cases
# TESTS		+= $(MODULES) 1 2 3 4 5 6
# TESTS		:= $(subst /,_,$(TESTS))
# BINARIES	:= $(TESTS:%=$(BUILDDIR)/test/test_%.ihx)
# DEMOS		:= common tools

# Tell test cases where to find modules' header files
TESTCFLAGS	= $(if $(findstring /test/, $@), -Isrc)

# Get the file name of a module or a test case
testf		= $(patsubst %,$(BUILDDIR)/test/test_%.ihx,$(1))
libf		= $(patsubst %,$(BUILDDIR)/%.rel,$(1))


.PHONY: all modules $(DEMOS) clean

# .PRECIOUS: $(BUILDDIR)/%.rel

all: $(DEMOS)

modules:
	$(MAKE) -C src

$(DEMOS): modules
	$(MAKE) -C $(@:%=demos/%)


# # Generate dependency file for a C source file and compile the source
# # file using sdcc
# $(BUILDDIR)/%.rel: src/%.c
# 	@mkdir -p $(@D)
# 	@$(SDCC) -MM $(SDCCFLAGS) $(TESTCFLAGS) $< |				\
# 	    sed ':a;$$!{N;ba}; s@\\\n@@g; s@^[^:]*: \(.*\)$$@$(@D)/\0\n\1:@'	\
# 	    >$(@:%.rel=%.dep)
# 	$(SDCC) -c $(SDCCFLAGS) $(TESTCFLAGS) $< -o $(@D)/

# Link .rel files
# %.ihx: %.rel 
# 	$(SDCC) $(SDCCFLAGS) $(ASLINKFLAGS) $^ -o $(@D)/

# Link every module in every test case.  This could result large
# binary files
# $(BINARIES): $(MODULES:%=$(BUILDDIR)/%.rel)

# # AutoISP
# ifneq ($(AUTOISP), )
# ifneq ($(findstring ^STC, ^$(TARGET)), )
# $(BINARIES): $(BUILDDIR)/stc/autoisp.rel
# endif
# endif

# # To build a test case in the left column, we need modules from the
# # right column
$(call testf, common): 		$(call libf, common)
$(call testf, tools): 		$(call libf, common)
$(call testf, uart): 		$(call libf, common uart)
$(call testf, timer): 		$(call libf, common uart print timer)
$(call testf, iic):		$(call libf, common uart print iic)
$(call testf, spi):		$(call libf, common print spi lcd1602)
$(call testf, irrc5):		$(call libf, common uart print timer irrc5)
$(call testf, irnec):		$(call libf, common uart print timer irnec)
$(call testf, led7seg):		$(call libf, common uart print led7seg)
$(call testf, rom9346):		$(call libf, common uart print spi rom9346)
$(call testf, rom2402): 	$(call libf, common uart print iic rom2402)
$(call testf, ds1820): 		$(call libf, common uart print tools ds1820)
$(call testf, lcd1602): 	$(call libf, common uart print lcd1602)
$(call testf, pcf8591): 	$(call libf, common uart print iic pcf8591)
$(call testf, ds1302): 		$(call libf, common uart print ds1302)
$(call testf, print):		$(call libf, common uart print)
$(call testf, kbhost):		$(call libf, common uart print kbhost)
$(call testf, dht11):		$(call libf, common uart print dht11)
$(call testf, hcsr04):		$(call libf, common uart print timer hcsr04)

$(call testf, stc_wdt): 	$(call libf, common uart print)
$(call testf, stc_gpio): 	$(call libf, common uart print)
$(call testf, stc_adc): 	$(call libf, common uart print)
$(call testf, stc_pca): 	$(call libf, common uart print)
$(call testf, stc_eeprom): 	$(call libf, common uart print stc/eeprom)
$(call testf, stc_autoisp): 	$(call libf, common uart print)
$(call testf, stc_spi): 	$(call libf, common uart print stc/spi)
$(call testf, stc_uart2): 	$(call libf, common stc/uart2)

$(call testf, 1): 		$(call libf, common)
$(call testf, 2): 		$(call libf, common uart print)
$(call testf, 3): 		$(call libf, common uart print)
$(call testf, 4): 		$(call libf, common uart print timer)
$(call testf, 5): 		$(call libf, common uart print tools timer spi rom9346 ds1820 lcd1602 irnec)
$(call testf, 6): 		$(call libf, common uart print timer)

# Clean up
clean:
	rm -rf $(BUILDDIR)/*


# Include source code dependency files built from previous rules
-include $(MODULES:%=$(BUILDDIR)/%.dep)
-include $(BINARIES:%.ihx=%.dep)
