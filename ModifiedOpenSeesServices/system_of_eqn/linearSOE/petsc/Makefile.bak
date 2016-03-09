# Makefile for fe objects

# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../../..

include ../../../../Makefile.Includes
include ../../../../Makefile.Compilers


PROGRAM       = test


#OBJS       = PetscSOE.o PetscSolver.o ShadowPetscSOE.o ActorPetscSOE.o
OBJS       = PetscSOE.o \
             PetscSolver.o 
#             PetscSparseSeqSolver.o

all:    $(OBJS)

test:    main.o $(OBJS)
	$(LINKER) $(LINKFLAGS) $(OBJS) main.o  $(MATRIX_LIBS) $(ERROR_LIBS) $(GRAPH_LIBS)\
        $(SysOfEqn_LIBS) $(MACHINE_NUMERICAL_LIBS) $(PETSC_LIB) -o $(PROGRAM)

ERROR_LIBS = ../../../handler/StandardStream.o \
	../../../FileStream.o \
	../../../OPS_Stream.o

GRAPH_LIBS   = ../../../graph/graph/Graph.o \
	../../../graph/Vertex.o \
	../../../graph/VertexIter.o \
	../../../tagged/TaggedObject.o \
	../../../tagged/storage/ArrayOfTaggedObjects.o \
	../../../tagged/storage/ArrayOfTaggedObjectsIter.o 	

MATRIX_LIBS   = $(FE)/matrix/Matrix.o \
        ../../../matrix/Vector.o \
        ../../../matrix/ID.o \
	../../../../nDarray/basics.o \
	../../../../nDarray.o \
	../../../../BJmatrix.o \
	../../../../BJvector.o \
	../../../../BJtensor.o \
	../../../../straint.o  \
	../../../../stresst.o

SysOfEqn_LIBS = ../../SystemOfEqn.o \
		../../linearSOE/LinearSOE.o \
		../../linearSOE/DomainSolver.o \
		../../linearSOE/LinearSOESolver.o \
		../../linearSOE/sparseGEN/SparseGenRowLinSOE.o \
		../../linearSOE/sparseGEN/SparseGenRowLinSolver.o \
		../../linearSOE/sparseGEN/SparseGenColLinSOE.o \
		../../linearSOE/sparseGEN/SparseGenColLinSolver.o \
		../../linearSOE/sparseGEN/SuperLU.o \
		../../Solver.o \
		../../../actor/actor/MovableObject.o 

# Miscellaneous

tidy:	
	@rm -f Makefile.bak *~ #*# core

clean: tidy
	@rm -f $(OBJS) *.o

spotless: clean
	@rm -f $(PROGRAM) 

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.


