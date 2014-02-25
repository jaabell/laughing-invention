
#
# Makefile to create libMOSS.a 
# MOSS -> ModifiedOpenSeesServices
#
# Boris Jeremic, March 2009, UCD
#
# updated by:
# Nima Tafazzoli, November 2009, UCD
#
#

#include ../Makefile.Includes
include ../Makefile.Compilers



MOSS_LIBRARY = ../lib/libMOSS.a
WIPE_LIBS   = $(MOSS_LIBRARY)


# %---------------------------------------------------------%


all:
	$(RM) $(RMFLAGS) $(MOSS_LIBRARY);
	$(CD) matrix; $(MAKE);
	$(CD) database; $(MAKE);
	$(CD) system_of_eqn; $(MAKE);
	$(CD) tagged; $(MAKE);
	$(CD) material; $(MAKE);
	$(CD) analysis; $(MAKE);
	$(CD) graph; $(MAKE);
	$(CD) convergenceTest; $(MAKE);
	$(CD) domain; $(MAKE);
	$(CD) handler; $(MAKE);
	$(CD) actor; $(MAKE);
	$(CD) element; $(MAKE);
	$(CD) utility; $(MAKE);
	$(CD) response; $(MAKE);
	@$(ECHO) "------------------"
	$(AR) $(ARFLAGS) \
            $(MOSS_LIBRARY) \
            $(FMK_LIBS) \
            $(LAW_LIBS) \
            $(GGD_LIBS) \
            $(RECORDER_LIBS) \
            $(DATABASE_LIBS) \
            $(MATRIX_LIBS) \
            $(TAGGED_LIBS) \
            $(UTILITY_LIBS) \
            $(GRAPH_LIBS) \
            $(ELE_LIBS) \
            $(MATERIAL_LIBS) \
            $(SECTION_LIBS) \
            $(SysOfEqn_LIBS) \
            $(DOMAIN_LIBS) \
            $(ANALYSIS_LIBS) \
            $(ACTOR_LIBS) \
            $(ERRORHANDLER_LIBS) \
            $(RESPONSE_LIBS)

	# $(CD) recorder; $(MAKE);
#	$(CD) machine; $(MAKE);
   

FMK_LIBS = ./database/FE_Datastore.o



LAW_LIBS = ./system_of_eqn/eigenSOE/EigenSOE.o \
           ./system_of_eqn/eigenSOE/EigenSolver.o \
           ./system_of_eqn/eigenSOE/SymBandEigenSOE.o \
           ./system_of_eqn/eigenSOE/SymBandEigenSolver.o \
           ./analysis/analysis/EigenAnalysis.o \
           ./analysis/integrator/EigenIntegrator.o \
           ./analysis/algorithm/eigenAlgo/EigenAlgorithm.o \
           ./analysis/algorithm/eigenAlgo/FrequencyAlgo.o \
           ./analysis/algorithm/eigenAlgo/StandardEigenAlgo.o \
           ./system_of_eqn/eigenSOE/FullGenEigenSOE.o \
           ./system_of_eqn/eigenSOE/FullGenEigenSolver.o



GGD_LIBS = 
# ./material/yieldSurface/yieldSurfaceBC/Orbison2D.o \
#            ./material/yieldSurface/yieldSurfaceBC/YieldSurface_BC2D.o \
#            ./material/yieldSurface/yieldSurfaceBC/YieldSurface_BC.o \
#            ./material/yieldSurface/yieldSurfaceBC/NullYS2D.o \
#            ./material/yieldSurface/evolution/BkStressLimSurface2D.o \
#            ./material/yieldSurface/evolution/CombinedIsoKin2D01.o \
#            ./material/yieldSurface/evolution/Isotropic2D01.o \
#            ./material/yieldSurface/evolution/Kinematic2D02.o \
#            ./material/yieldSurface/evolution/PeakOriented2D01.o \
#            ./material/yieldSurface/evolution/PlasticHardening2D.o \
#            ./material/yieldSurface/evolution/YS_Evolution.o \
#            ./material/yieldSurface/evolution/BoundingSurface2D.o \
#            ./material/yieldSurface/evolution/CombinedIsoKin2D02.o \
#            ./material/yieldSurface/evolution/Kinematic2D01.o \
#            ./material/yieldSurface/evolution/NullEvolution.o \
#            ./material/yieldSurface/evolution/PeakOriented2D02.o \
#            ./material/yieldSurface/evolution/YS_Evolution2D.o \
#            ./material/yieldSurface/plasticHardeningMaterial/ExponReducing.o \
#            ./material/yieldSurface/plasticHardeningMaterial/MultiLinearKp.o \
#            ./material/yieldSurface/plasticHardeningMaterial/NullPlasticMaterial.o \
#            ./material/yieldSurface/plasticHardeningMaterial/PlasticHardeningMaterial.o



ERRORHANDLER_LIBS = 
#./handler/StandardStream.o \
#                    ./handler/FileStream.o 
#                    ./handler/OPS_Stream.o



RECORDER_LIBS = 
# ./recorder/Recorder.o \
#                 ./recorder/MaxNodeDispRecorder.o \
#                 ./recorder/DatastoreRecorder.o \
#                 ./recorder/NodeRecorder.o \
#                 ./recorder/EnvelopeNodeRecorder.o \
#                 ./recorder/DriftRecorder.o \
#                 ./recorder/PatternRecorder.o \
#                 ./recorder/ElementRecorder.o \
#                 ./recorder/EnvelopeElementRecorder.o \


                # ./recorder/GSA_Recorder.o \
                #./handler/DataOutputHandler.o \
                #./handler/DataOutputDatabaseHandler.o \
                #./handler/DataOutputStreamHandler.o \
                #./handler/DataOutputFileHandler.o 

RESPONSE_LIBS = ./response/Response.o \
                ./response/ElementResponse.o \
                ./response/MaterialResponse.o \
                ./response/FiberResponse.o 



DATABASE_LIBS = ./database/FileDatastore.o \
                ./database/NEESData.o 
#               ./database/MySqlDatastore.o \
#               ./database/MySqlDataRecorder.o


MATRIX_LIBS   = ./matrix/Matrix.o \
                ./matrix/Vector.o \
                ./matrix/ID.o

TAGGED_LIBS =   ./tagged/TaggedObject.o \
                ./tagged/storage/ArrayOfTaggedObjects.o \
                ./tagged/storage/ArrayOfTaggedObjectsIter.o  \
                ./tagged/storage/MapOfTaggedObjects.o \
                ./tagged/storage/MapOfTaggedObjectsIter.o

UTILITY_LIBS = ./utility/NanoTimer.o \
				./utility/Timer.o


ifeq ($(PROGRAMMING_MODE), PARALLEL)
GRAPH_LIBS = ./graph/graph/DOF_Graph.o \
             ./graph/graph/VertexIter.o \
             ./graph/graph/Vertex.o  \
             ./graph/graph/Graph.o \
             ./graph/graph/DOF_GroupGraph.o  \
             ./graph/numberer/RCM.o \
             ./graph/numberer/MyRCM.o \
             ./graph/numberer/GraphNumberer.o \
             ./graph/numberer/SimpleNumberer.o \
             ./graph/partitioner/Metis.o \
             ./graph/partitioner/ParMetis.o
else
GRAPH_LIBS = ./graph/graph/DOF_Graph.o \
             ./graph/graph/VertexIter.o \
             ./graph/graph/Vertex.o  \
             ./graph/graph/Graph.o \
             ./graph/graph/DOF_GroupGraph.o  \
             ./graph/numberer/RCM.o \
             ./graph/numberer/MyRCM.o \
             ./graph/numberer/GraphNumberer.o \
             ./graph/numberer/SimpleNumberer.o
endif

ELE_LIBS   =  ./element/Element.o \
              ./element/Information.o \
              ./element/ElementalLoad.o



MATERIAL_LIBS   =  ./material/Material.o \
                   ./material/uniaxial/UniaxialMaterial.o \
                   ./material/uniaxial/ElasticMaterial.o \
                   ./material/uniaxial/ReinforcingSteel.o \
                   ./material/uniaxial/ElasticPPMaterial.o \
                   ./material/uniaxial/HardeningMaterial.o \
                   ./material/uniaxial/ParallelMaterial.o \
                   ./material/uniaxial/SeriesMaterial.o \
                   ./material/uniaxial/Concrete01.o \
                   ./material/uniaxial/Concrete02.o \
                   ./material/uniaxial/Concrete04.o \
                   ./material/uniaxial/Steel01.o \
                   ./material/uniaxial/Steel02.o \
                   ./material/uniaxial/Steel03.o \
                   ./material/uniaxial/HystereticMaterial.o \
                   ./material/uniaxial/EPPGapMaterial.o \
                   ./material/uniaxial/ViscousMaterial.o \
                   ./material/uniaxial/PathIndependentMaterial.o \
                   ./material/uniaxial/MinMaxMaterial.o \
                   ./material/uniaxial/FatigueMaterial.o \
                   ./material/uniaxial/CableMaterial.o \
                   ./material/uniaxial/ENTMaterial.o \
                   ./material/uniaxial/BoucWenMaterial.o 
                   # ./material/uniaxial/BarSlipMaterial.o
#./material/uniaxial/Pinching4Material.o \
#                    ./material/nD/ElasticIsotropicMaterial.o \
#                    ./material/nD/ElasticIsotropicBeamFiber.o \
#                    ./material/nD/PlaneStressMaterial.o \
#                    ./material/nD/BeamFiberMaterial.o
#                    ./material/nD/NDMaterial.o \



SECTION_LIBS = ./material/section/SectionForceDeformation.o \
               ./material/section/ElasticSection3d.o \
               ./material/section/ElasticShearSection3d.o \
               ./material/section/FiberSection3d.o \
               ./material/section/FiberSectionGJ.o \
               ./material/section/SectionAggregator.o \
               ./material/section/GenericSection1d.o \
               ./material/section/ElasticPlateSection.o \
               ./material/section/ElasticMembranePlateSection.o \
               ./material/section/MembranePlateFiberSection.o \
               ./material/section/Bidirectional.o \
               ./material/section/Isolator2spring.o \
               ./material/section/fiber/Fiber.o \
               ./material/section/fiber/UniaxialFiber3d.o \
               ./material/section/repres/patch/Patch.o \
               ./material/section/repres/patch/QuadPatch.o \
               ./material/section/repres/patch/CircPatch.o \
               ./material/section/repres/cell/Cell.o \
               ./material/section/repres/cell/QuadCell.o \
               ./material/section/repres/reinfBar/ReinfBar.o \
               ./material/section/repres/reinfLayer/ReinfLayer.o \
               ./material/section/repres/reinfLayer/StraightReinfLayer.o \
               ./material/section/repres/reinfLayer/CircReinfLayer.o \
               ./material/section/repres/section/SectionRepres.o \
               ./material/section/repres/section/FiberSectionRepr.o \
               ./material/section/matrixutil/MatrixUtil.o



ifeq ($(PROGRAMMING_MODE), THREADS)
SUPER_LU_OBJ = ./system_of_eqn/linearSOE/sparseGEN/ThreadedSuperLU.o
else
SUPER_LU_OBJ = ./system_of_eqn/linearSOE/spar.seGEN/SuperLU.o
endif

ifeq ($(PROGRAMMING_MODE), PARALLEL)
SysOfEqn_LIBS = ./system_of_eqn/SystemOfEqn.o \
                ./system_of_eqn/Solver.o \
                ./system_of_eqn/linearSOE/LinearSOE.o \
                ./system_of_eqn/linearSOE/LinearSOESolver.o \
                ./system_of_eqn/linearSOE/DomainSolver.o \
                ./system_of_eqn/linearSOE/petsc/PetscSOE.o \
                ./system_of_eqn/linearSOE/petsc/PetscSolver.o 
else
SysOfEqn_LIBS = ./system_of_eqn/SystemOfEqn.o \
                ./system_of_eqn/Solver.o \
                ./system_of_eqn/linearSOE/LinearSOE.o \
                ./system_of_eqn/linearSOE/LinearSOESolver.o \
                ./system_of_eqn/linearSOE/DomainSolver.o \
                ./system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSOE.o \
                ./system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSolver.o \
                ./system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSOE.o \
                ./system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSolver.o \
                ./system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectSolver.o \
                ./system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSubstrSolver.o \
                ./system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectBlockSolver.o \
                ./system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectSkypackSolver.o \
                ./system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSolverGather.o \
                ./system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSOEGather.o \
                ./system_of_eqn/linearSOE/profileSPD/DistributedProfileSPDLinSOE.o \
                ./system_of_eqn/linearSOE/bandSPD/BandSPDLinSOE.o \
                ./system_of_eqn/linearSOE/bandSPD/BandSPDLinSolver.o \
                ./system_of_eqn/linearSOE/bandSPD/BandSPDLinLapackSolver.o \
                ./system_of_eqn/linearSOE/bandSPD/DistributedBandSPDLinSOE.o \
                ./system_of_eqn/linearSOE/bandGEN/BandGenLinLapackSolver.o \
                ./system_of_eqn/linearSOE/bandGEN/BandGenLinSOE.o \
                ./system_of_eqn/linearSOE/bandGEN/BandGenLinSolver.o \
                ./system_of_eqn/linearSOE/bandGEN/DistributedBandGenLinSOE.o
endif



ifeq ($(PROGRAMMING_MODE), PARALLEL)
DOMAIN_LIBS =  ./domain/component/DomainComponent.o \
               ./domain/domain/Domain.o \
               ./domain/domain/single/SingleDomEleIter.o \
               ./domain/domain/single/SingleDomNodIter.o \
               ./domain/domain/single/SingleDomSP_Iter.o \
               ./domain/domain/single/SingleDomAllSP_Iter.o \
               ./domain/domain/single/SingleDomMP_Iter.o \
               ./domain/domain/single/SingleDomUniaxialMaterialIter.o \
               ./domain/domain/single/SingleDomNDMaterialIter.o \
               ./domain/domain/partitioned/PartitionedDomain.o \
               ./domain/domain/partitioned/PartitionedDomainEleIter.o \
               ./domain/domain/partitioned/PartitionedDomainSubIter.o \
               ./domain/partitioner/DomainPartitioner.o \
               ./domain/node/Node.o \
               ./domain/node/NodalLoad.o \
               ./domain/constraints/SP_Constraint.o \
               ./domain/constraints/MP_Constraint.o \
               ./domain/constraints/ImposedMotionSP.o \
               ./domain/constraints/ImposedMotionSP1.o \
               ./domain/constraints/RigidDiaphragm.o \
               ./domain/constraints/RigidRod.o \
               ./domain/constraints/RigidBeam.o \
               ./domain/load/NodalLoadIter.o \
               ./domain/load/ElementalLoadIter.o \
               ./domain/load/Load.o \
               ./domain/load/Beam3dUniformLoad.o \
               ./domain/load/Beam3dPointLoad.o \
               ./domain/load/ElementSelfWeight.o \
               ./domain/load/BrickSurfaceLoad.o \
               ./domain/load/BrickSurfaceLoad_from_Contact_Pore_Pressure.o \
               ./domain/load/AccelerationField.o \
               ./domain/damping/Damping.o \
               ./domain/damping/Rayleigh.o \
               ./domain/damping/Caughey3rd.o \
               ./domain/damping/Caughey4th.o \
               ./domain/pattern/LoadPattern.o \
               ./domain/pattern/EarthquakePattern.o \
               ./domain/pattern/MultiSupportPattern.o \
               ./domain/pattern/UniformExcitation.o \
               ./domain/pattern/LoadPatternIter.o \
               ./domain/pattern/TimeSeries.o \
               ./domain/pattern/LinearSeries.o \
               ./domain/pattern/ConstantSeries.o \
               ./domain/pattern/Linear_Force_TimeHistory.o \
               ./domain/pattern/PathSeries_Force_TimeHistory.o \
               ./domain/pattern/PathTimeSeries_Force_TimeHistory.o \
               ./domain/pattern/Linear_Disp_TimeHistory_Penalty.o \
               ./domain/pattern/PathSeries_Disp_TimeHistory_Penalty.o \
               ./domain/pattern/PathSeries.o \
               ./domain/pattern/PathTimeSeries.o \
               ./domain/pattern/TimeSeriesIntegrator.o \
               ./domain/pattern/TrapezoidalTimeSeriesIntegrator.o \
               ./domain/groundMotion/GroundMotion.o \
               ./domain/groundMotion/GroundMotionRecord.o \
               ./domain/groundMotion/InterpolatedGroundMotion.o \
               ./domain/subdomain/Subdomain.o \
               ./domain/subdomain/ShadowSubdomain.o \
               ./domain/subdomain/ActorSubdomain.o \
               ./domain/subdomain/SubdomainNodIter.o \
               ./analysis/analysis/DomainUser.o \
               ./domain/pattern/Domain_Reduction_Method.o \
               ./domain/pattern/Domain_Reduction_Method_Modified_Input.o \
               ./domain/pattern/Domain_Reduction_Method_Save_Forces.o \
               ./domain/pattern/Domain_Reduction_Method_Restore_Forces.o \
               ./domain/pattern/PBowlLoading.o 
else
DOMAIN_LIBS =  ./domain/component/DomainComponent.o \
               ./domain/domain/Domain.o \
               ./domain/domain/single/SingleDomEleIter.o \
               ./domain/domain/single/SingleDomNodIter.o \
               ./domain/domain/single/SingleDomSP_Iter.o \
               ./domain/domain/single/SingleDomAllSP_Iter.o \
               ./domain/domain/single/SingleDomMP_Iter.o \
               ./domain/domain/single/SingleDomUniaxialMaterialIter.o \
               ./domain/domain/single/SingleDomNDMaterialIter.o \
               ./domain/node/Node.o \
               ./domain/node/NodalLoad.o \
               ./domain/constraints/SP_Constraint.o \
               ./domain/constraints/MP_Constraint.o \
               ./domain/constraints/ImposedMotionSP.o \
               ./domain/constraints/ImposedMotionSP1.o \
               ./domain/constraints/RigidDiaphragm.o \
               ./domain/constraints/RigidRod.o \
               ./domain/constraints/RigidBeam.o \
               ./domain/load/NodalLoadIter.o \
               ./domain/load/ElementalLoadIter.o \
               ./domain/load/Load.o \
               ./domain/load/Beam3dUniformLoad.o \
               ./domain/load/Beam3dPointLoad.o \
               ./domain/load/ElementSelfWeight.o \
               ./domain/load/BrickSurfaceLoad.o \
               ./domain/load/BrickSurfaceLoad_from_Contact_Pore_Pressure.o \
               ./domain/load/AccelerationField.o \
               ./domain/damping/Damping.o \
               ./domain/damping/Rayleigh.o \
               ./domain/damping/Caughey3rd.o \
               ./domain/damping/Caughey4th.o \
               ./domain/pattern/LoadPattern.o \
               ./domain/pattern/EarthquakePattern.o \
               ./domain/pattern/MultiSupportPattern.o \
               ./domain/pattern/UniformExcitation.o \
               ./domain/pattern/LoadPatternIter.o \
               ./domain/pattern/TimeSeries.o \
               ./domain/pattern/LinearSeries.o \
               ./domain/pattern/ConstantSeries.o \
               ./domain/pattern/Linear_Force_TimeHistory.o \
               ./domain/pattern/PathSeries_Force_TimeHistory.o \
               ./domain/pattern/PathTimeSeries_Force_TimeHistory.o \
               ./domain/pattern/Linear_Disp_TimeHistory_Penalty.o \
               ./domain/pattern/PathSeries_Disp_TimeHistory_Penalty.o \
               ./domain/pattern/PathSeries.o \
               ./domain/pattern/PathTimeSeries.o \
               ./domain/pattern/TimeSeriesIntegrator.o \
               ./domain/pattern/TrapezoidalTimeSeriesIntegrator.o \
               ./domain/groundMotion/GroundMotion.o \
               ./domain/groundMotion/GroundMotionRecord.o \
               ./domain/groundMotion/InterpolatedGroundMotion.o \
               ./analysis/analysis/DomainUser.o \
               ./domain/pattern/Domain_Reduction_Method.o \
               ./domain/pattern/Domain_Reduction_Method_Modified_Input.o \
               ./domain/pattern/Domain_Reduction_Method_Save_Forces.o \
               ./domain/pattern/Domain_Reduction_Method_Restore_Forces.o \
               ./domain/pattern/PBowlLoading.o 
endif


ANALYSIS_LIBS_BASE = ./analysis/analysis/Analysis.o \
                ./analysis/analysis/StaticAnalysis.o \
                ./analysis/analysis/TransientAnalysis.o \
                ./analysis/analysis/DirectIntegrationAnalysis.o \
                ./analysis/analysis/VariableTimeStepDirectIntegrationAnalysis.o \
                ./analysis/analysis/SubstructuringAnalysis.o \
                ./analysis/algorithm/SolutionAlgorithm.o \
                ./analysis/algorithm/equiSolnAlgo/EquiSolnAlgo.o \
                ./analysis/algorithm/equiSolnAlgo/Linear.o \
                ./analysis/algorithm/equiSolnAlgo/NewtonRaphson.o \
                ./analysis/algorithm/equiSolnAlgo/ModifiedNewton.o \
                ./convergenceTest/ConvergenceTest.o \
                ./convergenceTest/CTestNormUnbalance.o \
                ./convergenceTest/CTestNormDispIncr.o \
                ./convergenceTest/CTestEnergyIncr.o \
                ./analysis/integrator/Integrator.o \
                ./analysis/integrator/StaticIntegrator.o \
                ./analysis/integrator/LoadControl.o \
                ./analysis/integrator/LoadPath.o \
                ./analysis/integrator/ArcLength.o \
                ./analysis/integrator/HSConstraint.o \
                ./analysis/integrator/ArcLength1.o \
                ./analysis/integrator/MinUnbalDispNorm.o \
                ./analysis/integrator/DisplacementControl.o \
                ./analysis/integrator/DistributedDisplacementControl.o \
                ./analysis/integrator/TransientIntegrator.o \
                ./analysis/integrator/Newmark.o \
                ./analysis/integrator/HHT.o \
                ./analysis/integrator/HHT1.o \
                ./analysis/integrator/HHT_alpha_method.o \
                ./analysis/integrator/CentralDifference.o \
                ./analysis/integrator/WilsonTheta.o \
                ./analysis/integrator/IncrementalIntegrator.o \
                ./analysis/model/AnalysisModel.o \
                ./analysis/model/simple/SimpleFE_Iter.o \
                ./analysis/model/simple/SimpleDOF_Iter.o \
                ./analysis/handler/ConstraintHandler.o \
                ./analysis/handler/PlainHandler.o \
                ./analysis/handler/PenaltyConstraintHandler.o \
                ./analysis/handler/LagrangeConstraintHandler.o \
                ./analysis/handler/TransformationConstraintHandler.o \
                ./analysis/numberer/DOF_Numberer.o \
                ./analysis/numberer/PlainNumberer.o \
                ./analysis/numberer/ParallelNumberer.o \
                ./analysis/dof_grp/DOF_Group.o \
                ./analysis/dof_grp/LagrangeDOF_Group.o \
                ./analysis/dof_grp/TransformationDOF_Group.o \
                ./analysis/fe_ele/FE_Element.o \
                ./analysis/fe_ele/penalty/PenaltySP_FE.o \
                ./analysis/fe_ele/penalty/PenaltyMP_FE.o \
                ./analysis/fe_ele/lagrange/LagrangeSP_FE.o \
                ./analysis/fe_ele/lagrange/LagrangeMP_FE.o \
                ./analysis/fe_ele/transformation/TransformationFE.o 

ANALYSIS_LIBS_PARALLEL = 
ifeq ($(PROGRAMMING_MODE), PARALLEL)                
ANALYSIS_LIBS_PARALLEL = ./analysis/analysis/DomainDecompositionAnalysis.o \
                ./analysis/analysis/StaticDomainDecompositionAnalysis.o \
                ./analysis/analysis/TransientDomainDecompositionAnalysis.o \
                ./analysis/algorithm/domainDecompAlgo/DomainDecompAlgo.o 
endif

ANALYSIS_LIBS = $(ANALYSIS_LIBS_BASE) $(ANALYSIS_LIBS_PARALLEL)

ifeq ($(PROGRAMMING_MODE), PARALLEL)
ACTOR_LIBS = ./actor/channel/Channel.o \
             ./actor/channel/MPI_Channel.o \
             ./actor/message/Message.o \
             ./actor/machineBroker/MachineBroker.o \
             ./actor/machineBroker/AlphaMachineBroker.o \
             ./actor/machineBroker/MillMachineBroker.o \
             ./actor/machineBroker/DecMachineBroker.o \
             ./actor/objectBroker/FEM_ObjectBroker.o \
             ./actor/actor/Actor.o \
             ./actor/actor/MovableObject.o \
             ./actor/shadow/Shadow.o \
             ./actor/address/ChannelAddress.o \
             ./actor/address/MPI_ChannelAddress.o \
             ./actor/machineBroker/MPI_MachineBroker.o \
             ./actor/channel/Channel.o
else
ACTOR_LIBS = ./actor/actor/MovableObject.o \
             ./actor/objectBroker/FEM_ObjectBroker.o \
             ./actor/message/Message.o \
             ./actor/channel/Channel.o \
             ./actor/actor/MovableObject.o
endif



###############################################################################
###############################################################################
#
# CLEAN

clean:        
	@$(CD) ./material; $(MAKE) wipe;
	@$(CD) ./analysis; $(MAKE) wipe;
	@$(CD) ./graph; $(MAKE) wipe;
	@$(CD) ./actor; $(MAKE) wipe;
	@$(CD) ./utility; $(MAKE) wipe;
	@$(CD) ./system_of_eqn; $(MAKE) wipe;
	@$(CD) ./tagged; $(MAKE) wipe;
	@$(CD) ./domain; $(MAKE) wipe;
	@$(CD) ./matrix; $(MAKE) wipe;
	@$(CD) ./convergenceTest; $(MAKE) wipe;
	@$(CD) ./database; $(MAKE) wipe;
	@$(CD) ./element; $(MAKE) wipe;
	@$(CD) ./response; $(MAKE) wipe;
	# @$(CD) ./recorder; $(MAKE) wipe;
	# @$(CD) ./handler; $(MAKE) wipe;
	# @$(CD) ./machine; $(MAKE) wipe;



cleanall:        
	@$(CD) ./material; $(MAKE) wipe;
	@$(CD) ./analysis; $(MAKE) wipe;
	@$(CD) ./graph; $(MAKE) wipe;
	@$(CD) ./actor; $(MAKE) wipe;
	@$(CD) ./utility; $(MAKE) wipe;
	@$(CD) ./system_of_eqn; $(MAKE) wipe;
	@$(CD) ./tagged; $(MAKE) wipe;
	@$(CD) ./domain; $(MAKE) wipe;
	@$(CD) ./matrix; $(MAKE) wipe;
	@$(CD) ./convergenceTest; $(MAKE) wipe;
	@$(CD) ./database; $(MAKE) wipe;
	@$(CD) ./handler; $(MAKE) wipe;
	@$(CD) ./element; $(MAKE) wipe;
	@$(RM) $(RMFLAGS) $(WIPE_LIBS) *.o *~ core
	@$(CD) ./response; $(MAKE) wipe;
	# @$(CD) ./recorder; $(MAKE) wipe;
	# @$(CD) ./machine; $(MAKE) wipe;


# DO NOT DELETE THIS LINE -- make depend depends on it.

