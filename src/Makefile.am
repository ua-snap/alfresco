CXX=g++
#bin_PROGRAMS = fresco
bin_PROGRAMS = fresco frescocli
fresco_SOURCES = Main.cpp Fresco.cpp CustomFresco.cpp Landscape.cpp BSpruce.cpp \
		WSpruce.cpp Tundra.cpp ShrubTundra.cpp GraminoidTundra.cpp WetlandTundra.cpp RasterIO.cpp \
	 	NoVeg.cpp PreCompiled.cpp CustomLandscape.cpp  \
		Frame.cpp Stat.cpp Fire.cpp Interface.cpp Climate.cpp Decid.cpp Grassland.cpp \
		Species.cpp Except.cpp \
		Global.cpp demon_me.cpp Client.cpp RemoteNotification.cpp Messenger.cpp FrescoEngine.cpp \
		StatFile.cpp StatArray.cpp
fresco_LDFLAGS = -L../lib -lboost_system -lboost_filesystem -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal
fresco_CXXFLAGS = -O3 -funroll-loops -Wall
INCLUDES = -I../include -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS)
frescocli_SOURCES = ../src/MainCLI.cpp ../src/Fresco.cpp ../src/CustomFresco.cpp ../src/Landscape.cpp ../src/BSpruce.cpp \
		../src/WSpruce.cpp ../src/Tundra.cpp ../src/ShrubTundra.cpp ../src/GraminoidTundra.cpp ../src/WetlandTundra.cpp ../src/RasterIO.cpp \
		../src/NoVeg.cpp ../src/PreCompiled.cpp ../src/CustomLandscape.cpp  \
		../src/Frame.cpp ../src/Stat.cpp ../src/Fire.cpp ../src/Interface.cpp ../src/Climate.cpp ../src/Decid.cpp ../src/Grassland.cpp \
		../src/Species.cpp ../src/Except.cpp \
		../src/Global.cpp ../src/demon_me.cpp ../src/Client.cpp ../src/RemoteNotification.cpp ../src/Messenger.cpp ../src/FrescoEngine.cpp \
		../src/StatFile.cpp ../src/StatArray.cpp ../src/Args.cpp
frescocli_LDFLAGS = -L../lib -lboost_system -lboost_filesystem -lboost_program_options -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal -L/usr/lib64/openmpi
#frescocli_LDFLAGS += -lprofiler
frescocli_CPPFLAGS = -I$(top_srcdir)/src
frescocli_CXXFLAGS = -DWITHSTATS -O3 -funroll-loops -Wall
#frescocli_CXXFLAGS += -g
