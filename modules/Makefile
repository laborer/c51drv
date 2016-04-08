MODULES		:= tools uart timer print iic spi irrc5 irnec led7seg kbhost
MODULES		+= ds1820 rom9346 rom2402 lcd1602 pcf8591 ds1302 dht11 hcsr04
ifneq ($(filter STC%, $(TARGET)), )
    MODULES	+= stc/eeprom stc/autoisp
    ifneq ($(filter STC89%, $(TARGET)), )
	MODULES	+=
    else ifneq ($(filter STC15F%, $(TARGET)), )
	MODULES += stc/spi
    else
	MODULES += stc/spi stc/uart2
    endif
endif

BUILDDIR	:= $(MODULESDIR)


.PHONY: all

all: $(MODULES:%=$(BUILDDIR)/%.rel)

# Generate dependency file for a C source file and compile the source
# file using sdcc
$(BUILDDIR)/%.rel: %.c
	@mkdir -p $(@D)
	@$(SDCC) -MM $(SDCCFLAGS) $< |						\
	    sed ':a;$$!{N;ba}; s@\\\n@@g; s@^[^:]*: \(.*\)$$@$(@D)/\0\n\1:@'	\
	    >$(@:%.rel=%.dep)
	$(SDCC) -c $(SDCCFLAGS) $< -o $@

# Clean up
clean:
	rm -rf $(BUILDDIR)/*

# The file name of the official STC ISP programmer, we use it to
# compile a list of MCU models, which are then grouped into families
# in modeldb.h
STCISP_EXE	:= ../stc-isp-15xx-v6.85K.exe
stc/modeldb.h:
	test -s $(STCISP_EXE) || exit 1

	echo '#ifndef __MODELDB_H' >$@
	echo '#define __MODELDB_H' >>$@
	echo >>$@
	echo >>$@

	for i in '\(15F10\|15L10\)[[:digit:]]\+   STC15F10'				\
		'\(15F\|15L\|15W\)[[:digit:]]\+   STC15F'				\
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


-include $(MODULES:%=$(BUILDDIR)/%.dep)
