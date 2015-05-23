#Nima Tafazzoli and Boris Jeremic October 2009


include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS =  ContactElement_3DOF_3DOF.o \
        ContactElement_3DOF_6DOF.o \
        ContactElement_6DOF_6DOF.o \
        ContactElement_Nonlinear_3DOF_3DOF.o \
        ContactElement_Nonlinear_3DOF_7DOF.o \
        FrictionalPenaltyContact.o



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


