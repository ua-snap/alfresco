lib_LTLIBRARIES = libFrescoFoundation.la libFrescoCustom.la
if AIEM
lib_LTLIBRARIES += libFrescoAIEM.la
endif
INCLUDES = -I../include -I../ -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS) -I/home/UA/apbennett/include
if AIEM
INCLUDES += -I/home/UA/apbennett/aiem/include
INCLUDES += -I/home/apbennett/aiem/include
endif
libFrescoFoundation_la_LDFLAGS = -shared
libFrescoFoundation_la_CXXFLAGS = -DWITHSTATS -g
libFrescoFoundation_la_SOURCES = ../src/FrescoClimate.cpp \
				../src/Except.cpp \
				../src/Fire.cpp \
				../src/Frame.cpp \
				../src/Fresco.cpp \
				../src/Landscape.cpp \
				../src/NoVeg.cpp \
				../src/PreCompiled.cpp \
				../src/RasterIO.cpp \
				../src/Species.cpp \
				../src/Stat.cpp \
				../src/FIF.cpp
libFrescoCustom_la_LDFLAGS = -shared
libFrescoCustom_la_CXXFLAGS = -DWITHSTATS -g
libFrescoCustom_la_SOURCES = 	../src/BarrenLichenMoss.cpp \
				../src/BSpruce.cpp \
				../src/CustomFresco.cpp \
				../src/CustomLandscape.cpp \
				../src/Decid.cpp \
				../src/GraminoidTundra.cpp \
				../src/Grassland.cpp \
				../src/ShrubTundra.cpp \
				../src/TemperateRainforest.cpp \
				../src/Tundra.cpp \
				../src/WetlandTundra.cpp \
				../src/WSpruce.cpp
if AIEM
libFrescoAIEM_la_LDFLAGS = -shared
libFrescoAIEM_la_CXXFLAGS = -DWITHSTATS -DAIEM_MODEL -g
libFrescoAIEM_la_SOURCES = 	../src/BarrenLichenMoss.cpp \
				../src/BSpruce.cpp \
				../src/CustomFresco.cpp \
				../src/CustomLandscape.cpp \
				../src/Decid.cpp \
				../src/GraminoidTundra.cpp \
				../src/Grassland.cpp \
				../src/ShrubTundra.cpp \
				../src/TemperateRainforest.cpp \
				../src/Tundra.cpp \
				../src/WetlandTundra.cpp \
				../src/WSpruce.cpp \
				../src/FrescoClimate.cpp \
				../src/Except.cpp \
				../src/FIF.cpp \
				../src/Fire.cpp \
				../src/Frame.cpp \
				../src/Fresco.cpp \
				../src/Landscape.cpp \
				../src/NoVeg.cpp \
				../src/PreCompiled.cpp \
				../src/RasterIO.cpp \
				../src/Species.cpp \
				../src/Stat.cpp
endif
