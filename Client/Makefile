#
# Makefile for FrescoClient executable
#
# deleted this line: only  added to test the svn repository.

export PROJECT_BASE = $(HOME)/fresco
export POCO_BASE = $(PROJECT_BASE)/poco
export POCO_BUILD = ../build
POCO_ADD_INCLUDE = $(PROJECT_BASE)/gdal
export POCO_ADD_INCLUDE

include $(POCO_BASE)/build/rules/global

INCLUDE := $(INCLUDE) -I../Foundation/include -I../Custom/include

objects        = RemoteNotification Messenger Main Global FrescoEngine demon_me Client
target         = FrescoClient
target_version = 1
target_libs    = PocoFoundation PocoNet PocoXML FrescoCustom FrescoFoundation gdal

include $(POCO_BASE)/build/rules/exec

