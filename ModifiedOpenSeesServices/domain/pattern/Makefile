# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include ../../../Makefile.Includes
include ../../../Makefile.Compilers

OBJS       =   EarthquakePattern.o \
               Domain_Reduction_Method.o \
               Domain_Reduction_Method_Modified_Input.o \
               Domain_Reduction_Method_HDF5_input.o \
               Domain_Reduction_Method_Save_Forces.o \
               Domain_Reduction_Method_Restore_Forces.o \
               LinearSeries.o \
               LoadPattern.o \
               LoadPatternIter.o \
               PathSeries.o \
               PathTimeSeries.o \
               TimeSeries.o \
               ConstantSeries.o \
               TimeSeriesIntegrator.o \
               TrapezoidalTimeSeriesIntegrator.o \
               MultiSupportPattern.o \
               Linear_Force_TimeHistory.o \
               PathSeries_Force_TimeHistory.o \
               PathTimeSeries_Force_TimeHistory.o \
               Linear_Disp_TimeHistory_Penalty.o \
               PathSeries_Disp_TimeHistory_Penalty.o \
               PBowlLoading.o \
               UniformExcitation.o
#               TclSeriesCommand.o \
#               TclSeriesIntegratorCommand.o
#               TclPatternCommand.o \

#Boris Jeremic took out 19Mar2008
#                DiscretizedRandomProcessSeries.o 
#               SimulatedRandomProcessSeries.o \

# Compilation control

all:         $(OBJS)

# Miscellaneous
tidy:   
	@$(RM) $(RMFLAGS) Makefile.bak *~ #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o

spotless: clean

wipe: spotless


# DO NOT DELETE THIS LINE -- make depend depends on it.
