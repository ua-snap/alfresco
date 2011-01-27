########################################################
#
#  Makefile
#  This is a parent makefile for the seperate child
#  project makefiles for ALFRESCO as well as the 
#  POCO and GDAL libraries.
#
#  From scratch:
#  1. Extract the fresco archive as ~/fresco
#  2. Extract the poco archive as ~/fresco/poco
#  3. Extract the gdal archive as ~/fresco/gdal
#  4. cd ~/fresco
#  5. make buildgdal
#  6. make installgdal
#  7. make buildpoco
#  8. make installpoco
#  9. make buildfresco
#  10. make installfresco
#
########################################################

PROJECT_BASE = $(HOME)/fresco
export PROJECT_BASE
POCO_BASE = $(HOME)/fresco/poco
export POCO_BASE

include $(POCO_BASE)/build/rules/global

# Directory Variables
GDALDIR = gdal
POCODIR = poco
FOUNDDIR = Foundation
CUSTDIR = Custom
CLIENTDIR = Client
SUBDIRS = $(FOUNDDIR) $(CUSTDIR) $(CLIENTDIR)

BASEDIR = $(shell pwd)
POCOLIBDIR = $(BASEDIR)/$(POCODIR)/lib/$(OSNAME)/$(OSARCH)
FRESCOLIBDIR = $(BASEDIR)/build/lib/$(OSNAME)/$(OSARCH)
FRESCOBINDIR = $(BASEDIR)/build/$(CLIENTDIR)/bin/$(OSNAME)/$(OSARCH)

# Default Target
buildfresco:
	@echo
	@echo "#######################################"
	@echo "#       BUILDING FOUNDATION           #"
	@echo "#######################################"
	@echo
	@cd $(FOUNDDIR) ; make shared_release
	@echo
	@echo
	@echo "#######################################"
	@echo "#       BUILDING CUSTOM               #"
	@echo "#######################################"
	@echo
	@cd $(CUSTDIR) ; make shared_release
	@echo
	@echo
	@echo "#######################################"
	@echo "#       BUILDING CLIENT               #"
	@echo "#######################################"
	@echo
	@cd $(CLIENTDIR) ; make shared_release
	@echo

# Individual Build Targets
foundation: 
	@cd $(FOUNDDIR) ; make shared_release
custom:
	@cd $(CUSTDIR) ; make shared_release
client:
	@cd $(CLIENTDIR) ; make shared_release
buildpoco: 
	@cd $(POCODIR) ; ./configure ; make libexecs 
buildgdal:
	@cd $(GDALDIR) ; ./configure --with-libz=internal --with-geotiff=internal --with-libtiff=internal ; make 
installpoco: 
	@cd $(POCODIR) ; make install
installgdal:
	@cd $(GDALDIR) ; make install


cleanfresco:
	rm -f *~
	for i in $(SUBDIRS) ; do \
	(cd $$i ; make clean) ; \
	done

installfresco:
	mkdir -p /usr/local/bin
	mkdir -p /usr/local/lib
	cp $(FRESCOLIBDIR)/* /usr/local/lib
	cp $(FRESCOBINDIR)/* /usr/local/bin
