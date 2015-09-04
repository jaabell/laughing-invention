
include ../Makefile.Includes
include ../Makefile.Compilers



OBJS       = \
             NDMaterial.o \
             ElasticIsotropic3D.o \
             ElasticIsotropicMaterial.o \
             ElasticCrossAnisotropic.o \
             BardetConstraint.o \
             BardetConstraintLT.o \
             TwoStageEquivalentElasticIsotropic3D.o \
             ClassicElastoplasticMaterial/ClassicElastoplasticityGlobals.o

LIBRARY_Base_3D_EP  = ../lib/libbase_3dep.a

all: $(OBJS) 
	$(CD) NewTemplate3Dep; $(MAKE);
	$(CD) Template3Dep; $(MAKE);
	$(CD) Constitutive_Domain; $(MAKE);
	$(CD) Accelerated3Dep; $(MAKE);
	$(CD) LTensor_Based; $(MAKE);
	$(CD) ClassicElastoplasticMaterial; $(MAKE);
	$(AR) $(ARFLAGS) $(LIBRARY_Base_3D_EP) $(OBJS)


clean:
	$(CD) NewTemplate3Dep; $(MAKE) clean;
	$(CD) Template3Dep; $(MAKE) clean;
	$(CD) Constitutive_Domain; $(MAKE) clean;
	$(CD) Accelerated3Dep; $(MAKE) clean;
	@$(RM) $(RMFLAGS) $(OBJS) *.o

cleanall:
	$(CD) NewTemplate3Dep; $(MAKE) cleanall;
	$(CD) Template3Dep; $(MAKE) cleanall;
	$(CD) Constitutive_Domain; $(MAKE) cleanall;
	$(CD) Accelerated3Dep; $(MAKE) cleanall;
	$(CD) LTensor_Based; $(MAKE) cleanall;
	$(RM) $(RMFLAGS)  *.o *.bak
	$(RM) $(RMFLAGS) $(LIBRARY_Base_3D_EP)

