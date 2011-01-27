#
# Makefile for FrescoCustom library
#

PROJECT_BASE = $(HOME)/fresco
export PROJECT_BASE
POCO_BASE = $(PROJECT_BASE)/poco
export POCO_BASE
POCO_BUILD = ../build
export POCO_BUILD
POCO_ADD_INCLUDE = $(PROJECT_BASE)/gdal
export POCO_ADD_INCLUDE


include $(POCO_BASE)/build/rules/global

INCLUDE := $(INCLUDE) -I../Foundation/include

objects        = WSpruce Tundra Decid CustomLandscape CustomFresco BSpruce Grassland
target         = FrescoCustom
target_version = 1
target_libs    = PocoFoundation FrescoFoundation gdal

include $(POCO_BASE)/build/rules/lib

