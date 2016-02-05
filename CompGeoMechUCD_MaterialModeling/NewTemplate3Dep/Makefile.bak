
include ../../Makefile.Includes
include ../../Makefile.Compilers


OBJS =  MaterialParameter.o \
        ElasticState.o \
        Isotropic_Elastic.o \
        elnp_Elastic.o \
        PressureDependent_Elastic.o \
        DM04_Elastic.o \
        YieldFunction.o \
        VM_YF.o \
        RMC_YF.o \
        DP_YF.o \
        CC_YF.o \
        DM04_YF.o \
        PlasticFlow.o \
        VM_PF.o \
        RMC_PF.o \
        DP_PF.o \
        CC_PF.o \
        DM04_PF.o \
        ScalarEvolution.o \
        Linear_Eeq.o \
        CC_Ev.o \
        TensorEvolution.o \
        Linear_Eij.o \
        AF_Eij.o \
        DM04_alpha_Eij.o \
        DM04_z_Eij.o \
        NewTemplate3Dep.o \
        SANISAND_YF.o \
        SANISAND_Elastic.o \
        SANISAND_alpha_Eij.o \
        SANISAND_PF.o \
        SANISAND_p0_bar.o \
        SANISAND_z_Eij.o 


LIBRARY_NewTemplate_3D_EP  = ../../lib/libnewtemplate3dep.a


################### TARGETS ########################
all: $(OBJS) 
	$(AR) $(ARFLAGS) $(LIBRARY_NewTemplate_3D_EP) $(OBJS)


clean:
	$(RM) $(RMFLAGS)  *.o *.bak

cleanall:
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS) $(LIBRARY_NewTemplate_3D_EP)



# DO NOT DELETE THIS LINE -- make depend depends on it.




