#lib_LTLIBRARIES = libfoobar.la
#libfoobar_la_SOURCES = ../samp.cpp
#libfoobar_la_CPPFLAGS = -I$(top_srcdir)/
CXX=mpicxx
bin_PROGRAMS = frescocli
frescocli_SOURCES = ../src/MainCLI.cpp ../src/Fresco.cpp ../src/CustomFresco.cpp ../src/Landscape.cpp ../src/BSpruce.cpp \
		../src/WSpruce.cpp ../src/Tundra.cpp ../src/ShrubTundra.cpp ../src/GrammanoidTundra.cpp ../src/WetlandTundra.cpp ../src/RasterIO.cpp \
		../src/NoVeg.cpp ../src/PreCompiled.cpp ../src/CustomLandscape.cpp  \
		../src/Frame.cpp ../src/Stat.cpp ../src/Fire.cpp ../src/Interface.cpp ../src/Climate.cpp ../src/Decid.cpp ../src/Grassland.cpp \
		../src/Species.cpp ../src/Except.cpp \
		../src/Global.cpp ../src/demon_me.cpp ../src/Client.cpp ../src/RemoteNotification.cpp ../src/Messenger.cpp ../src/FrescoEngine.cpp \
		../src/StatFile.cpp ../src/StatArray.cpp ../src/Args.cpp
frescocli_LDFLAGS = -L../lib -lboost_system -lboost_filesystem -lboost_program_options -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal -L/usr/lib64/openmpi
frescocli_LDFLAGS += -lprofiler
frescocli_CPPFLAGS = -I$(top_srcdir)/src
frescocli_CXXFLAGS = -DWITHMPI -O3 -funroll-loops -Wall
frescocli_CXXFLAGS += -g
INCLUDES = -I../include -I../ -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS)
lib_LTLIBRARIES = libfrescofoundation.la
libfrescofoundation_la_SOURCES = ../src/PreCompiled.cpp ../src/Fresco.cpp ../src/Interface.cpp ../src/Fire.cpp ../src/Frame.cpp ../src/Except.cpp ../src/Species.cpp ../src/NoVeg.cpp ../src/Landscape.cpp ../src/Climate.cpp ../src/Stat.cpp ../src/RasterIO.cpp
