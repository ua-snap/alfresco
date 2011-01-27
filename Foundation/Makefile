#
# Makefile for FrescoFoundation library
#

PROJECT_BASE = $(HOME)/fresco
export PROJECT_BASE
POCO_BASE = $(PROJECT_BASE)/poco
export POCO_BASE
POCO_BUILD = ../build
export POCO_BUILD
POCO_ADD_INCLUDE = /usr/include/gdal
export POCO_ADD_INCLUDE


include $(POCO_BASE)/build/rules/global

objects        = PreCompiled Fresco Interface Fire Frame Except Species NoVeg Landscape Climate Stat RasterIO
target         = FrescoFoundation
target_version = 1
target_libs    = PocoFoundation gdal

include $(POCO_BASE)/build/rules/lib
