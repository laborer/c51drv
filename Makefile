SDCC		:= $(if $(shell which sdcc), sdcc, ~/Downloads/sdcc-3.3.0/bin/sdcc)

SDCCBINDIR	:= $(shell $(SDCC) --print-search-dirs | sed -n '/^programs:$$/{n;p}')
PACKIHX		:= $(SDCCBINDIR)/packihx
BUILDDIR	:= build

SDCCFLAGS	:= --less-pedantic --disable-warning 84
ASLINKFLAGS	:=

MODULES		:= common tools uart timer irrc5 irnec ds1820 rom9346 rom2402 lcd1602
MODULES		:= $(MODULES) stc/eeprom
TESTS		:= $(subst /,_,$(MODULES)) stc_wdt 1 2 3 4 5 6
TARGETS		:= $(TESTS:%=$(BUILDDIR)/test/test_%.bin)

# AT89C2051
#ASLINKFLAGS	:= $(ASLINKFLAGS) --code-size 2048 --iram-size 128 --xram-size 0
#SDCCFLAGS	:= $(SDCCFLAGS) -DMICROCONTROLLER_8051
# AT89C51
#ASLINKFLAGS	:= $(ASLINKFLAGS) --code-size 4096 --iram-size 128 --xram-size 0
#SDCCFLAGS	:= $(SDCCFLAGS) -DMICROCONTROLLER_8051
# STC89C52RC
ASLINKFLAGS	:= $(ASLINKFLAGS) --code-size 8192 --iram-size 256 --xram-size 256
SDCCFLAGS	:= $(SDCCFLAGS) -DMICROCONTROLLER_8052
# STC89C54RD+
#ASLINKFLAGS	:= $(ASLINKFLAGS) --code-size 16384 --iram-size 256 --xram-size 1024
#SDCCFLAGS	:= $(SDCCFLAGS) -DMICROCONTROLLER_8052
# STC12C5A16S2
#ASLINKFLAGS	:= $(ASLINKFLAGS) --code-size 16384 --iram-size 256 --xram-size 1024
#SDCCFLAGS	:= $(SDCCFLAGS) -DMICROCONTROLLER_8052

TESTCFLAGS	= $(if $(findstring /test/, $@), -Isrc)

libf		= $(patsubst %,$(BUILDDIR)/%.rel,$(1))
testf		= $(patsubst %,$(BUILDDIR)/test/test_%.ihx,$(1))


.PHONY: all clean

.PRECIOUS: $(BUILDDIR)/%.rel

all: $(TARGETS)

$(BUILDDIR)/%.rel: src/%.c
	@mkdir -p $(@D)
	@$(SDCC) -MM $(SDCCFLAGS) $(TESTCFLAGS) $< | \
	    sed ':a;$$!{N;ba}; s@\\\n@@g; s@^[^:]*: \(.*\)$$@$(@D)/\0\n\1:@' >$(@:%.rel=%.dep)
	$(SDCC) -c $(SDCCFLAGS) $(TESTCFLAGS) $< -o $(@D)/

%.ihx: %.rel
	$(SDCC) $(SDCCFLAGS) $(ASLINKFLAGS) $^ -o $(@D)/

$(call testf, common): 		$(call libf, common)
$(call testf, uart): 		$(call libf, common uart)
$(call testf, timer): 		$(call libf, common uart timer)
$(call testf, irrc5):		$(call libf, common uart timer irrc5)
$(call testf, irnec):		$(call libf, common uart timer irnec)
$(call testf, rom9346):		$(call libf, common uart rom9346)
$(call testf, rom2402): 	$(call libf, common uart rom2402)
$(call testf, ds1820): 		$(call libf, common tools uart ds1820)
$(call testf, lcd1602): 	$(call libf, common uart lcd1602)

$(call testf, stc_wdt): 	$(call libf, common uart)
$(call testf, stc_eeprom): 	$(call libf, common uart stc/eeprom)

$(call testf, 1): 		$(call libf, common)
$(call testf, 2): 		$(call libf, common uart)
$(call testf, 3): 		$(call libf, common uart)
$(call testf, 4): 		$(call libf, common uart timer)
$(call testf, 5): 		$(call libf, common tools uart timer rom9346 ds1820 lcd1602 irnec)
$(call testf, 6): 		$(call libf, common uart)

%.hex: %.ihx
	$(PACKIHX) $< >$@

%.bin: %.ihx
	objcopy -Iihex -Obinary $< $@

clean:
	rm -rf $(BUILDDIR)/*


-include $(MODULES:%=$(BUILDDIR)/%.dep)
-include $(TARGETS:%.bin=%.dep)
