#
# Makefile for FrescoCustom library
#

PROJECT_BASE = $(HOME)/fresco
export PROJECT_BASE
POCO_BASE = $(PROJECT_BASE)/Poco
export POCO_BASE
POCO_BUILD = ../build
export POCO_BUILD

include $(POCO_BASE)/build/rules/global

INCLUDE := $(INCLUDE) -I../Foundation/include

objects        = WSpruce Tundra Decid CustomLandscape CustomFresco BSpruce
target         = FrescoCustom
target_version = 1
target_libs    = PocoFoundation FrescoFoundation

include $(POCO_BASE)/build/rules/lib

