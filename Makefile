# Try to use sdcc in the system.  If sdcc is not installed, find sdcc
# at ~/Downloads/sdcc-3.3.0/bin/sdcc
SDCC		:= $(if $(shell which sdcc), sdcc, ~/Downloads/sdcc-3.3.0/bin/sdcc)

# Find where sdcc is, so we can also locate packihx and makebin there
SDCCBINDIR	:= $(shell $(SDCC) --print-search-dirs | sed -n '/^programs:$$/{n;p}')
PACKIHX		:= $(SDCCBINDIR)/packihx
MAKEBIN		:= $(SDCCBINDIR)/makebin -p

# The name of the directory which holds all the compile binaries
BUILDDIR	:= build

# Disable some unnecessary warnings
SDCCFLAGS	:= $(SDCCFLAGS) --less-pedantic --disable-warning 84

# Set the model name of the target MCU
# TARGET		:= STC89C51RC

# Tell C program the name of the target MCU model
SDCCFLAGS	:= $(SDCCFLAGS) -DTARGET_MODEL_$(subst +,_,$(TARGET))

# Set frequency of the oscillator
# SDCCFLAGS	:= $(SDCCFLAGS) -DFOSC=11059200L

# Enable AutoISP for STC MCUs
AUTOISP		:= yes

# Specify modules to compile
MODULES		:= common tools uart timer print iic spi irrc5 irnec ds1820 rom9346 rom2402 lcd1602 pcf8591 ds1302

# Include modules and test cases designed for certain MCUs
ifneq ($(findstring ^STC, ^$(TARGET)), )
    SDCCFLAGS	:= $(SDCCFLAGS) -DMICROCONTROLLER_8052
    # If AutoISP is enabled, register autoisp_check as a UART callback
    # function
    SDCCFLAGS	:= $(SDCCFLAGS) $(if $(AUTOISP), -DUART_CALLBACK=autoisp_check) 
    MODULES	:= $(MODULES) stc/eeprom stc/autoisp
    TESTS	:= $(TESTS) stc/wdt
    # The following modules and test cases only work for non-STC89C
    # series MCUs
    ifeq ($(findstring ^STC89, ^$(TARGET)), )
        TESTS	:= $(TESTS) stc/gpio stc/adc stc/pca 
        SDCCFLAGS := $(SDCCFLAGS) -DTICKS=1 -DCYCLES_MOV_R_N=2 -DCYCLES_DJNZ_R_E=4
    endif
else
    SDCCFLAGS	:= $(SDCCFLAGS) -DMICROCONTROLLER_8051
endif

# Build a list of test cases
TESTS		:= $(MODULES) $(TESTS) 1 2 3 4 5 6
TESTS		:= $(subst /,_,$(TESTS))
BINARIES	:= $(TESTS:%=$(BUILDDIR)/test/test_%.bin)

# Set memory usage limit for some known MCUs
ifeq ($(TARGET), STC89C52RC)
    ASLINKFLAGS	:= $(ASLINKFLAGS) --code-size 8192 --xram-size 256
else ifeq ($(TARGET), STC89C54RD+)
    ASLINKFLAGS	:= $(ASLINKFLAGS) --code-size 16384 --xram-size 1024
else ifeq ($(TARGET), STC12C5A16S2)
    ASLINKFLAGS	:= $(ASLINKFLAGS) --code-size 16384 --xram-size 1024
endif

# Tell test cases where to find modules' header files
TESTCFLAGS	= $(if $(findstring /test/, $@), -Isrc)

# Get the file name of a module or a test case
testf		= $(patsubst %,$(BUILDDIR)/test/test_%.ihx,$(1))
libf		= $(patsubst %,$(BUILDDIR)/%.rel,$(1))


.PHONY: all clean

.PRECIOUS: $(BUILDDIR)/%.rel

all: $(BINARIES)

# Generate dependency file for a C source file and compile the source
# file using sdcc
$(BUILDDIR)/%.rel: src/%.c
	@mkdir -p $(@D)
	@$(SDCC) -MM $(SDCCFLAGS) $(TESTCFLAGS) $< |				\
	    sed ':a;$$!{N;ba}; s@\\\n@@g; s@^[^:]*: \(.*\)$$@$(@D)/\0\n\1:@'	\
	    >$(@:%.rel=%.dep)
	$(SDCC) -c $(SDCCFLAGS) $(TESTCFLAGS) $< -o $(@D)/

# Link .rel files
%.ihx: %.rel 
	$(SDCC) $(SDCCFLAGS) $(ASLINKFLAGS) $^ -o $(@D)/

# Link every module in every test case.  This could result large
# binary files
# $(BINARIES:%.bin=%.ihx): $(MODULES:%=$(BUILDDIR)/%.rel)

# AutoISP
ifneq ($(AUTOISP), )
ifneq ($(findstring ^STC, ^$(TARGET)), )
$(BINARIES:%.bin=%.ihx): $(BUILDDIR)/stc/autoisp.rel
endif
endif

# To build a test case in the left column, we need modules from the
# right column
$(call testf, common): 		$(call libf, common)
$(call testf, tools): 		$(call libf, common)
$(call testf, uart): 		$(call libf, common uart)
$(call testf, timer): 		$(call libf, common uart print timer)
$(call testf, iic):		$(call libf, common uart iic)
$(call testf, spi):		$(call libf, common uart spi)
$(call testf, irrc5):		$(call libf, common uart timer irrc5)
$(call testf, irnec):		$(call libf, common uart timer irnec)
$(call testf, rom9346):		$(call libf, common uart spi rom9346)
$(call testf, rom2402): 	$(call libf, common uart iic rom2402)
$(call testf, ds1820): 		$(call libf, common uart tools ds1820)
$(call testf, lcd1602): 	$(call libf, common uart lcd1602)
$(call testf, pcf8591): 	$(call libf, common uart iic pcf8591)
$(call testf, ds1302): 		$(call libf, common uart ds1302)
$(call testf, print):		$(call libf, common uart print)

$(call testf, stc_wdt): 	$(call libf, common uart)
$(call testf, stc_gpio): 	$(call libf, common uart)
$(call testf, stc_adc): 	$(call libf, common uart)
$(call testf, stc_pca): 	$(call libf, common uart)
$(call testf, stc_eeprom): 	$(call libf, common uart stc/eeprom)
$(call testf, stc_autoisp): 	$(call libf, common uart)

$(call testf, 1): 		$(call libf, common)
$(call testf, 2): 		$(call libf, common uart)
$(call testf, 3): 		$(call libf, common uart)
$(call testf, 4): 		$(call libf, common uart timer)
$(call testf, 5): 		$(call libf, common uart print tools timer spi rom9346 ds1820 lcd1602 irnec)
$(call testf, 6): 		$(call libf, common uart timer)

# Pack .ihx file to .hex.  By default, this makefile only generates
# .bin file, and this rule is not used
%.hex: %.ihx
	$(PACKIHX) $< >$@

# Convert .ihx file to .bin
%.bin: %.ihx
	$(MAKEBIN) $< $@

# Clean up
clean:
	rm -rf $(BUILDDIR)/*

# The file name of the official STC ISP programmer, we use it to
# compile a list of MCU models, which are then grouped into families
# in modeldb.h
STCISP_EXE	:= stc-isp-15xx-v6.61.exe
src/stc/modeldb.h:
	test -s $(STCISP_EXE) || exit 1

	echo '#ifndef __MODELDB_H' >$@
	echo '#define __MODELDB_H' >>$@
	echo >>$@
	echo >>$@

	for i in '\(15F\|15L\|15W\)[[:digit:]]\+  STC15F'				\
		'\(12C52\|12LE52\)[[:digit:]]\+   STC12C52'				\
		'\(12C5A\|12LE5A\)[[:digit:]]\+   STC12C5A'				\
		'\(10F\|10L\)[[:digit:]]\+        STC10F'				\
		'\(11F\|11L\)[[:digit:]]\+        STC11F'				\
		'\(12C54\|12LE54\)[[:digit:]]\+   STC12C54'				\
		'\(12C56\|12LE56\)[[:digit:]]\+   STC12C56'				\
		'\(12C\|12LE\)[[:digit:]]\+052    STC12Cx052'				\
		'\(90C\|90LE\)[[:digit:]]\+AD     STC90CxAD'				\
		'\(90C\|90LE\)[[:digit:]]\+R[CD]  STC90CxR'				\
		'\(89C\|89LE\)5[[:digit:]]\+      STC89C';				\
	do										\
		strings $(STCISP_EXE) |							\
		grep -o '^\(STC\|IAP\)'$${i%% *}'[[:digit:][:upper:]+]*' |		\
		sort -u |								\
		sed 's/+/_/; s/^/ defined TARGET_MODEL_/; 1s/^/#if/; 1!s/^/    ||/' |	\
		sed '$$!s/$$/                                            /' |		\
		sed '$$!s/^\(.\{64\}\).*$$/\1\\/' >>$@;					\
		echo '#define TARGET_FAMILY_'$${i##* } >>$@;				\
		echo '#endif' >>$@;							\
		echo >>$@;								\
	done

	echo >>$@
	echo '#endif /* __MODELDB_H */' >>$@


# Include source code dependency files built from previous rules
-include $(MODULES:%=$(BUILDDIR)/%.dep)
-include $(BINARIES:%.bin=%.dep)
