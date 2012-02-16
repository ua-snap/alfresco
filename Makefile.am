AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = frescocli fresco
fresco_SOURCES = src/Main.cpp src/Fresco.cpp src/CustomFresco.cpp src/Landscape.cpp src/BSpruce.cpp \
		src/WSpruce.cpp src/Tundra.cpp src/RasterIO.cpp src/NoVeg.cpp src/PreCompiled.cpp src/CustomLandscape.cpp  \
		src/Frame.cpp src/Stat.cpp src/Fire.cpp src/Interface.cpp src/Climate.cpp src/Decid.cpp src/Grassland.cpp \
		src/Species.cpp src/Except.cpp \
		src/Global.cpp src/demon_me.cpp src/Client.cpp src/RemoteNotification.cpp src/Messenger.cpp src/FrescoEngine.cpp \
		src/StatFile.cpp src/StatArray.cpp
frescocli_SOURCES = src/MainCLI.cpp src/Fresco.cpp src/CustomFresco.cpp src/Landscape.cpp src/BSpruce.cpp \
		src/WSpruce.cpp src/Tundra.cpp src/RasterIO.cpp src/NoVeg.cpp src/PreCompiled.cpp src/CustomLandscape.cpp  \
		src/Frame.cpp src/Stat.cpp src/Fire.cpp src/Interface.cpp src/Climate.cpp src/Decid.cpp src/Grassland.cpp \
		src/Species.cpp src/Except.cpp \
		src/Global.cpp src/demon_me.cpp src/Client.cpp src/RemoteNotification.cpp src/Messenger.cpp src/FrescoEngine.cpp \
		src/StatFile.cpp src/StatArray.cpp src/Args.cpp
fresco_LDFLAGS = -Llib -lboost_system -lboost_filesystem -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal
frescocli_LDFLAGS = -Llib -lboost_system -lboost_filesystem -lboost_program_options -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal -L/usr/lib64/openmpi
fresco_CXXFLAGS = -O3 -funroll-loops
frescocli_CXXFLAGS = -DWITHMPI -O3 -funroll-loops
INCLUDES = -Iinclude -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS)

