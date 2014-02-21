#  Written by: Nima Tafazzoli June 1st 2010
# Added for compilation of ltensor_utilities in Feb 2014 (Jose Abell)

include ../Makefile.Includes
include ../Makefile.Compilers

#all:  Element_renumber_GiD_to_FEI_Nima 
all: 
	$(CD) ltensor_utilities; $(MAKE);

Element_renumber_GiD_to_FEI:  Element_renumber_GiD_to_FEI.o
				gcc Element_renumber_GiD_to_FEI.o -o Element_renumber_GiD_to_FEI

Element_renumber_GiD_to_FEI_Nima:  Element_renumber_GiD_to_FEI_Nima.o
				gcc Element_renumber_GiD_to_FEI_Nima.o -o Element_renumber_GiD_to_FEI_Nima

TimeHistory_Column_to_Row:  TimeHistory_Column_to_Row.o
				g++ TimeHistory_Column_to_Row.o -o TimeHistory_Column_to_Row

#g++ TimeHistory_Column_to_Row.cpp -o TimeHistory_Column_to_Row

clean:
	$(RM) $(RMFLAGS) $(OBJS) *.o core.* *.err cachegrind* callgrind.* massif.* *~
	$(RM) $(RMFLAGS) TimeHistory_Column_to_Row Element_renumber_GiD_to_FEI_Nima Element_renumber_GiD_to_FEI

cleanall: clean
	$(CD) ltensor_utilities; $(MAKE) cleanall;

