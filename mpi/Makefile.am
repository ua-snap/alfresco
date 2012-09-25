#lib_LTLIBRARIES = libfoobar.la
#libfoobar_la_SOURCES = ../samp.cpp
#libfoobar_la_CPPFLAGS = -I$(top_srcdir)/
CXX=mpicxx
bin_PROGRAMS = fresco-mpi
INCLUDES = -I../include -I../ -I/usr/include/gdal -I/usr/include/Poco $(MPI_CXXFLAGS)
fresco_mpi_SOURCES = ../src/MainCLI.cpp  \
		../src/Global.cpp ../src/demon_me.cpp ../src/Client.cpp ../src/RemoteNotification.cpp ../src/Messenger.cpp ../src/FrescoEngine.cpp \
		../src/StatFile.cpp ../src/StatArray.cpp ../src/Args.cpp
fresco_mpi_LDFLAGS = -L../lib -lFrescoFoundation -lFrescoCustom -lboost_system -lboost_filesystem -lboost_program_options -L/usr/lib64/Poco -lPocoFoundation -lPocoNet -lPocoXML -lgdal -L/usr/lib64/openmpi
fresco_mpi_LDADD = ../lib/libFrescoFoundation.la ../lib/libFrescoCustom.la
fresco_mpi_CPPFLAGS = -I$(top_srcdir)/src
fresco_mpi_CXXFLAGS = -DWITHMPI -DWITHSTATS -O3 -funroll-loops -Wall
