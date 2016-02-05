include ../../Makefile.Includes
include ../../Makefile.Compilers

OutputLIB          = ../../lib/liboutputwriters.a

OBJS =  OutputWriter.o H5OutputWriter.o
# command_suggestion.o

%.o : %.cpp
	$(CC++) $(C++FLAGS) $(INCLUDES) $(MACHINE_INCLUDES) -c $< -o $@

all:  $(OBJS)
	$(AR) $(ARFLAGS) $(OutputLIB) $(OBJS)

###########################################################################
clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o 

cleanall:
	$(RM) $(RMFLAGS) $(OBJS) *.o 
	$(RM) $(RMFLAGS) $(OutputLIB)

