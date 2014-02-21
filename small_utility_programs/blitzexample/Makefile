#  Written: Nima Tafazzoli @ucdavis, Jan2010


CC++            =  g++
LINKER          =  g++
LINKFLAGS       = -g -pg -Wall 
INCLUDES  = -I/home/nima/blitz-0.9

####################################################

.cpp.o:
	$(CC++) $(INCLUDES) -c $<


all:  matmult


matmult:  matmult.o
	$(LINKER) $(LINKFLAGS) matmult.o \
                         -I../blitz-0.9/lib/libblitz.a \
                         -o matmult


clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o core.* *.txt *.out *.err cachegrind* callgrind.* massif.* *~
	$(RM) $(RMFLAGS) matmult

