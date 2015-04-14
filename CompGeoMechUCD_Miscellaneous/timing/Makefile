#include ../Makefile.Includes
include ../../Makefile.Compilers

# SRCS = $(wildcard *.cpp)
# OBJS = $(SRCS:.cpp=.o)
# TARGETS = $(patsubst %.cpp, % , $(SRCS))

# TARGETS = ESSITimer.o
OBJS = ESSITimer.o 

OutputLIB = ../../lib/libessitiming.a

# %.o: %.cpp
# 	$(CC++) $(C++FLAGS) $(INCLUDES) -c $< -o $@

# %.o : %.cpp
# 	$(CC++) $(C++FLAGS) $(INCLUDES) $(MACHINE_INCLUDES) -c $< -o $@

all:  $(OBJS)
	$(AR) $(ARFLAGS) $(OutputLIB) $(OBJS)

test: test.o ESSITimer.o
	$(CC++) $< ESSITimer.o -o $@

###########################################################################
clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o 

cleanall:
	$(RM) $(RMFLAGS) $(OBJS) *.o 
	$(RM) $(RMFLAGS) $(OutputLIB)
