########################################################
#
#  Makefile
#  This is a parent makefile for the seperate child
#  project makefiles.  It also handles installation.
#
########################################################

POCO_BASE = $(HOME)/fresco/Poco
export POCO_BASE

include $(POCO_BASE)/build/rules/global

# Directory Variables
POCODIR = Poco
FOUNDDIR = Foundation
CUSTDIR = Custom
CLIENTDIR = Client
SUBDIRS = $(FOUNDDIR) $(CUSTDIR) $(CLIENTDIR)

BASEDIR = $(shell pwd)
POCOLIBDIR = $(BASEDIR)/$(POCODIR)/lib/$(OSNAME)/$(OSARCH)
FRESCOLIBDIR = $(BASEDIR)/build/lib/$(OSNAME)/$(OSARCH)
FRESCOBINDIR = $(BASEDIR)/build/$(CLIENTDIR)/bin/$(OSNAME)/$(OSARCH)

# Default Target
all: 
	@echo
	@echo "#######################################"
	@echo "#       BUILDING FOUNDATION           #"
	@echo "#######################################"
	@echo
	@cd $(FOUNDDIR) ; make
	@echo
	@echo "#######################################"
	@echo "#       BUILDING CUSTOM               #"
	@echo "#######################################"
	@echo
	@cd $(CUSTDIR) ; make
	@echo
	@echo "#######################################"
	@echo "#       BUILDING CLIENT               #"
	@echo "#######################################"
	@echo
	@cd $(CLIENTDIR) ; make

# Individual Build Targets
foundation: 
	@cd $(FOUNDDIR) ; make
custom:
	@cd $(CUSTDIR) ; make
client:
	@cd $(CLIENTDIR) ; make
poco: 
	@cd $(POCODIR) ; ./configure ; make libexecs

clean:
	rm -f *~
	for i in $(SUBDIRS) ; do \
	(cd $$i ; make clean) ; \
	done

install:
	mkdir -p $(HOME)/fresco/bin
	mkdir -p $(HOME)/fresco/lib
	cp $(POCOLIBDIR)/* $(HOME)/fresco/lib
	cp $(FRESCOLIBDIR)/* $(HOME)/fresco/lib
	cp $(FRESCOBINDIR)/* $(HOME)/fresco/bin
	rm -f $(CLIENTDIR)/scripts/*~	
	cp $(CLIENTDIR)/scripts/* $(HOME)/fresco/bin
