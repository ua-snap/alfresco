bin_PROGRAMS = fresco
fresco_SOURCES = Main.cpp Fresco.cpp CustomFresco.cpp Landscape.cpp BSpruce.cpp \
		WSpruce.cpp Tundra.cpp ShrubTundra.cpp GrammanoidTundra.cpp WetlandTundra.cpp RasterIO.cpp \
	 	NoVeg.cpp PreCompiled.cpp CustomLandscape.cpp  \
		Frame.cpp Stat.cpp Fire.cpp Interface.cpp Climate.cpp Decid.cpp Grassland.cpp \
		Species.cpp Except.cpp \
		Global.cpp demon_me.cpp Client.cpp RemoteNotification.cpp Messenger.cpp FrescoEngine.cpp \
		StatFile.cpp StatArray.cpp
fresco_LDFLAGS = -L../lib -lboost_system -lboost_filesystem -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal
fresco_CXXFLAGS = -O3 -funroll-loops -Wall
INCLUDES = -I../include -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS)
