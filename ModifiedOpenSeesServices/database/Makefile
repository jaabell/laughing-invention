#  Written: fmk 
#  Created: 10/97
#  updated by:
#  Nima Tafazzoli, Sep 2012, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/..

include ../../Makefile.Includes
include ../../Makefile.Compilers

OBJS       = FE_Datastore.o \
             FileDatastore.o \
             NEESData.o


# Compilation control

all:   $(OBJS) 
#	mysql


MySQL_LIBRARY = MySQL.so



mysql: MySqlDatastore.o MySqlDataRecorder.o
	$(CC++) $(C++FLAGS) -shared -fPIC -Wl,-soname,$(MySQL_LIBRARY) -o $(MySQL_LIBRARY) \
	MySqlDatastore.o MySqlDataRecorder.o /usr/local/lib/libmysqlclient.so -lc



MySqlDatastore.o: MySqlDatastore.cpp MySqlDatastore.h
	$(CC++) $(C++FLAGS) -fPIC $(INCLUDES) $(MACHINE_INCLUDES)  -c MySqlDatastore.cpp


MySqlDataRecorder.o: MySqlDataRecorder.cpp MySqlDataRecorder.h
	$(CC++) $(C++FLAGS) -fPIC $(INCLUDES) $(MACHINE_INCLUDES)  -c MySqlDataRecorder.cpp	


test:  NeesDataTest.o 
	$(LINKER) $(LINKFLAGS) NeesDataTest.o NEESData.o $(FE_LIBRARY) \
	$(FE_LIBRARY) $(MACHINE_LINKLIBS) \
	$(MACHINE_NUMERICAL_LIBS) $(MACHINE_SPECIFIC_LIBS) $(METIS_LIBRARY) \
	 -o test

# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.



