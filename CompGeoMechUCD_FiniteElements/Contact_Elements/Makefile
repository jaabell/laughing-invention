#Nima Tafazzoli and Boris Jeremic October 2009


include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS =  FrictionalPenaltyContact.o



LIBRARY_Contact_Elements  = ../../lib/libcontactelements.a

all: $(OBJS)
	$(AR) $(ARFLAGS) $(LIBRARY_Contact_Elements) $(OBJS)


# Miscellaneous
clean:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  $(LIBRARY_Contact_Elements)
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS)  $(LIBRARY_Contact_Elements)
	$(RM) $(RMFLAGS) .inslog*
	$(RM) $(RMFLAGS) tca.map


