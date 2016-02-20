
default: build

check:
	@cd src;		$(MAKE) -s $@

build:
	@cd src;		$(MAKE) $@

install:
	@cd src;		$(MAKE) $@

clean :
	@cd src;		$(MAKE) $@
	@cd doc;		$(MAKE) $@
#	@cd examples;	$(MAKE) $@
#	@cd tests;		$(MAKE) $@

cleanall: clean

backup: clean
