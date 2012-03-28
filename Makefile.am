AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = mpi src
EXTRA_DIST = autogen.sh

bin_PROGRAMS = fresco
fresco_SOURCES = src/Main.cpp src/Fresco.cpp src/CustomFresco.cpp src/Landscape.cpp src/BSpruce.cpp \
		src/WSpruce.cpp src/Tundra.cpp src/ShrubTundra.cpp src/GrammanoidTundra.cpp src/WetlandTundra.cpp src/RasterIO.cpp \
	 	src/NoVeg.cpp src/PreCompiled.cpp src/CustomLandscape.cpp  \
		src/Frame.cpp src/Stat.cpp src/Fire.cpp src/Interface.cpp src/Climate.cpp src/Decid.cpp src/Grassland.cpp \
		src/Species.cpp src/Except.cpp \
		src/Global.cpp src/demon_me.cpp src/Client.cpp src/RemoteNotification.cpp src/Messenger.cpp src/FrescoEngine.cpp \
		src/StatFile.cpp src/StatArray.cpp
fresco_LDFLAGS = -Llib -lboost_system -lboost_filesystem -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal
fresco_CXXFLAGS = -O3 -funroll-loops -Wall
INCLUDES = -Iinclude -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS)

