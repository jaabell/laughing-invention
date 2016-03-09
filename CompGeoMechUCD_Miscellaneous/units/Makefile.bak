#include ../Makefile.Includes
include ../../Makefile.Compilers

QuantityLIB          = ../../lib/libquantity.a

OBJS =  quantity.o siunit.o 
# command_suggestion.o

%.o : %.cpp
	$(CC++) $(C++FLAGS) $(INCLUDES) $(MACHINE_INCLUDES) -c $< -o $@

all:  quantity.o siunit.o
	$(AR) $(ARFLAGS) $(QuantityLIB) $(OBJS)

quantity: siunit.o

###########################################################################
clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o 

cleanall:
	$(RM) $(RMFLAGS) $(OBJS) *.o 
	$(RM) $(RMFLAGS) $(QuantityLIB)

