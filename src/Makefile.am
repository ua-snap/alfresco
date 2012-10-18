CXX=g++
bin_PROGRAMS = fresco
INCLUDES = -I../include -I/usr/include/gdal -I/usr/include/Poco
fresco_SOURCES = ../src/MainCLI.cpp \
		../src/Global.cpp ../src/Client.cpp ../src/RemoteNotification.cpp ../src/Messenger.cpp ../src/FrescoEngine.cpp \
		../src/StatFile.cpp ../src/StatArray.cpp ../src/Args.cpp
fresco_LDFLAGS = -L../lib -lFrescoFoundation -lFrescoCustom -lboost_system -lboost_filesystem -lboost_program_options -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal
fresco_LDADD = ../lib/libFrescoFoundation.la ../lib/libFrescoCustom.la
fresco_CPPFLAGS = -I$(top_srcdir)/src
fresco_CXXFLAGS = -DWITHSTATS -funroll-loops -Wall
