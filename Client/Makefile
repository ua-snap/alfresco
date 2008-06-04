#
# Makefile for FrescoClient executable
#

export PROJECT_BASE = $(HOME)/fresco
export POCO_BASE = $(PROJECT_BASE)/Poco
export POCO_BUILD = ../build

include $(POCO_BASE)/build/rules/global

INCLUDE := $(INCLUDE) -I../Foundation/include -I../Custom/include

objects        = RemoteNotification Messenger Main Global FrescoEngine demon_me Client
target         = FrescoClient
target_version = 1
target_libs    = PocoFoundation PocoNet PocoXML FrescoCustom FrescoFoundation

include $(POCO_BASE)/build/rules/exec

