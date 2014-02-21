include ../../Makefile.Includes
include ../../Makefile.Compilers


#####################
OBJS = LTensorDisplay.o
LIBRARY_LTensor_Utils  = ../../lib/libltensorutilities.a

#####################


#%.o : %.cpp
	#$(CC++) $(C++FLAGS) $(INCLUDES) $(MACHINE_INCLUDES) -c $< -o $@

all:  $(OBJS)
	$(AR) $(ARFLAGS) $(LIBRARY_LTensor_Utils) $(OBJS)

#$(OBJS):  $(OBJS).o
#	$(LINKER) $(LINKFLAGS) $(OBJS).o $(LIBS) -o $(OBJS)


###########################################################################
clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o core* *.out *.feioutput
	$(RM) $(LIBRARY_LTensor_Utils)

cleanall: clean

