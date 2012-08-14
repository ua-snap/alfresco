CXX=g++
bin_PROGRAMS = frescocli
INCLUDES = -I../include -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS)
frescocli_SOURCES = ../src/MainCLI.cpp \
		../src/Global.cpp ../src/demon_me.cpp ../src/Client.cpp ../src/RemoteNotification.cpp ../src/Messenger.cpp ../src/FrescoEngine.cpp \
		../src/StatFile.cpp ../src/StatArray.cpp ../src/Args.cpp
frescocli_LDFLAGS = -L../lib -lFrescoFoundation -lFrescoCustom -lboost_system -lboost_filesystem -lboost_program_options -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal -L/usr/lib64/openmpi
frescocli_LDADD = ../lib/libFrescoFoundation.la ../lib/libFrescoCustom.la
frescocli_CPPFLAGS = -I$(top_srcdir)/src
frescocli_CXXFLAGS = -DWITHSTATS -O3 -funroll-loops -Wall
frescocli_CXXFLAGS += -g
