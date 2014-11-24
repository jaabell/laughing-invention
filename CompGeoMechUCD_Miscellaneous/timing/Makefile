#include ../Makefile.Includes
include ../../Makefile.Compilers

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGETS = $(patsubst %.cpp, % , $(SRCS))

OutputLIB = ../../lib/libessitiming.a

# %.o: %.cpp
# 	$(CC++) $(C++FLAGS) $(INCLUDES) -c $< -o $@

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
