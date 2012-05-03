lib_LTLIBRARIES = libFrescoFoundation.la libFrescoCustom.la
INCLUDES = -I../include -I../ -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS)
libFrescoFoundation_la_LDFLAGS = -shared
libFrescoFoundation_la_SOURCES = ../src/PreCompiled.cpp ../src/Fresco.cpp ../src/Interface.cpp ../src/Fire.cpp ../src/Frame.cpp ../src/Except.cpp ../src/Species.cpp ../src/NoVeg.cpp ../src/Landscape.cpp ../src/Climate.cpp ../src/Stat.cpp ../src/RasterIO.cpp
libFrescoCustom_la_LDFLAGS = -shared
libFrescoCustom_la_CXXFLAGS = -DWITHSTATS
libFrescoCustom_la_SOURCES = ../src/CustomLandscape.cpp ../src/CustomFresco.cpp ../src/Decid.cpp ../src/BSpruce.cpp ../src/WSpruce.cpp ../src/Grassland.cpp ../src/Tundra.cpp ../src/ShrubTundra.cpp ../src/GraminoidTundra.cpp ../src/WetlandTundra.cpp
