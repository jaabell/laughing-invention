///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral LGPL (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     LGPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Domain Specific Language (DSL)
//                    This file should be included in he main file of the example
//                    which are  expected to be run in parallel mode and contains
//                    all of the  needed header files also some of the parameters
//                    are iitialized here and some of the needed objects are defined.
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Babak Kamrani, Boris Jeremic
//
// DATE:              May 2009
// LAST UPDATE:       Feb 2014
//
// Note: Jose Abell merged CPPIncludes.h (in sequential code) with
//  CPPIncludes_Parallel.h from Babak
///////////////////////////////////////////////////////////////////////////////


// =================================================================================================
// Standard C++ includes
// =================================================================================================
#include <fstream>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
//#include <StandardStream.h>
//#include <FileStream.h>

#ifdef _PARALLEL_PROCESSING
# include <mpi.h>
#endif



// =================================================================================================
// Numerical libraries
// =================================================================================================

// LTensor library
//--------------------------------------------------------------------------------------------------
#include "../ltensor/LTensor.h"
#include "../small_utility_programs/ltensor_utilities/LTensorDisplay.h"


// UCD CompGeoMech nDarray (tensor, vector, matrix)
//--------------------------------------------------------------------------------------------------
#include "../nDarray/BJtensor.h"
#include "../nDarray/stresst.h"
#include "../nDarray/straint.h"
#include "../nDarray/BJvector.h"
#include "../nDarray/BJmatrix.h"
#include "../nDarray/stresst.h"



// =================================================================================================
// UCD Materials and Elements
// =================================================================================================
#include "../CompGeoMechUCD_FiniteElements/CompGeoMechUCD_FiniteElements.h"
#include "../CompGeoMechUCD_MaterialModeling/CompGeoMechUCD_MaterialModeling.h"



// =================================================================================================
// Modified OpenSees Services (MOSS) includes
// =================================================================================================
#include "../ModifiedOpenSeesServices/handler/ConsoleErrorHandler.h"
#include "../ModifiedOpenSeesServices/tagged/storage/ArrayOfTaggedObjects.h"
#include "../ModifiedOpenSeesServices/tagged/storage/MapOfTaggedObjects.h"
#include "../ModifiedOpenSeesServices/tagged/storage/MapOfTaggedObjectsIter.h"
#include "../ModifiedOpenSeesServices/G3Globals.h"
#include <../ModifiedOpenSeesServices/actor/objectBroker/FEM_ObjectBroker.h>


// Domain classes
//--------------------------------------------------------------------------------------------------
#ifdef _PARALLEL_PROCESSING
#include "../ModifiedOpenSeesServices/domain/domain/partitioned/PartitionedDomain.h"
#else
#include "../ModifiedOpenSeesServices/domain/domain/Domain.h"
#endif

#include "../ModifiedOpenSeesServices/domain/node/Node.h"
#include "../ModifiedOpenSeesServices/matrix/ID.h"

// Constraints
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/domain/constraints/SP_Constraint.h"
#include "../ModifiedOpenSeesServices/domain/constraints/MP_Constraint.h"
#include "../ModifiedOpenSeesServices/domain/domain/SP_ConstraintIter.h"
#include "../ModifiedOpenSeesServices/domain/domain/MP_ConstraintIter.h"


// Constraint Handlers
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/analysis/handler/PlainHandler.h"
#include "../ModifiedOpenSeesServices/analysis/handler/PenaltyConstraintHandler.h"
#include "../ModifiedOpenSeesServices/analysis/handler/LagrangeConstraintHandler.h"
#include "../ModifiedOpenSeesServices/analysis/handler/TransformationConstraintHandler.h"


// Analysis classes
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/analysis/analysis/StaticAnalysis.h"
#include "../ModifiedOpenSeesServices/analysis/analysis/TransientAnalysis.h"
#include "../ModifiedOpenSeesServices/analysis/analysis/VariableTimeStepDirectIntegrationAnalysis.h"
#include "../ModifiedOpenSeesServices/analysis/analysis/DirectIntegrationAnalysis.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/TransientIntegrator.h"
#include "../ModifiedOpenSeesServices/analysis/model/AnalysisModel.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/LoadControl.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/DisplacementControl.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/ArcLength.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/HSConstraint.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/ArcLength.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/ArcLength1.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/MinUnbalDispNorm.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/Newmark.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/WilsonTheta.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/HHT.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/HHT1.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/HHT_alpha_method.h"
#include "../ModifiedOpenSeesServices/analysis/analysis/EigenAnalysis.h"


// Numberers
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/analysis/numberer/PlainNumberer.h"
#include "../ModifiedOpenSeesServices/analysis/numberer/DOF_Numberer.h"
#include "../ModifiedOpenSeesServices/graph/numberer/RCM.h"


// Convergence Tests
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/convergenceTest/CTestNormUnbalance.h"
#include "../ModifiedOpenSeesServices/convergenceTest/CTestNormDispIncr.h"
#include "../ModifiedOpenSeesServices/convergenceTest/CTestEnergyIncr.h"


// Solution Algorithms
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/Linear.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/NewtonRaphson.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/NewtonLineSearch.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/ModifiedNewton.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/BFGS.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/KrylovNewton.h"


// Line Searchers
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/BisectionLineSearch.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/InitialInterpolatedLineSearch.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/RegulaFalsiLineSearch.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/SecantLineSearch.h"


// System of Equations (SOE) Solvers
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectThreadSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectSkypackSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSOE.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/bandSPD/BandSPDLinSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/bandSPD/BandSPDLinSOE.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/bandSPD/BandSPDLinLapackSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN3.1/UmfpackGenLinSOE.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN3.1/UmfpackGenLinSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSOE.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSolver.h"
#ifdef _PARALLEL_PROCESSING
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/petsc/PetscSolver.h" //Babak added
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/petsc/PetscSOE.h"   //Babak added
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/petsc/PetscSparseSeqSolver.h"//Babak added
#endif


// Recorders
//--------------------------------------------------------------------------------------------------
// #include "../ModifiedOpenSeesServices/recorder/NodeRecorder.h"
// #include "../ModifiedOpenSeesServices/recorder/ElementRecorder.h"

// #include "../ModifiedOpenSeesServices/handler/DataOutputHandler.h"
// #include "../ModifiedOpenSeesServices/handler/DataOutputFileHandler.h"


// Utilities
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/utility/Timer.h"


// Loading
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/domain/pattern/LoadPattern.h"
#include "../ModifiedOpenSeesServices/domain/pattern/LoadPatternIter.h"
#include "../ModifiedOpenSeesServices/domain/pattern/LinearSeries.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathSeries.h"
#include "../ModifiedOpenSeesServices/domain/pattern/ConstantSeries.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathTimeSeries.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathSeries.h"
#include "../ModifiedOpenSeesServices/domain/node/NodalLoad.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PBowlLoading.h"
#include "../ModifiedOpenSeesServices/domain/pattern/Domain_Reduction_Method.h"
#include "../ModifiedOpenSeesServices/domain/pattern/Domain_Reduction_Method_Modified_Input.h"
#include "../ModifiedOpenSeesServices/domain/pattern/Domain_Reduction_Method_Save_Forces.h"
#include "../ModifiedOpenSeesServices/domain/pattern/Domain_Reduction_Method_Restore_Forces.h"
#include "../ModifiedOpenSeesServices/domain/pattern/UniformExcitation.h"
#include "../ModifiedOpenSeesServices/domain/pattern/MultiSupportPattern.h"
#include "../ModifiedOpenSeesServices/domain/pattern/Linear_Force_TimeHistory.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathSeries_Force_TimeHistory.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathTimeSeries_Force_TimeHistory.h"
#include "../ModifiedOpenSeesServices/domain/pattern/Linear_Disp_TimeHistory_Penalty.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathSeries_Disp_TimeHistory_Penalty.h"
#include "../ModifiedOpenSeesServices/domain/constraints/ImposedMotionSP.h"
#include "../ModifiedOpenSeesServices/domain/groundMotion/GroundMotion.h"
#include "../ModifiedOpenSeesServices/domain/pattern/TimeSeriesIntegrator.h"
#include "../ModifiedOpenSeesServices/domain/groundMotion/GroundMotionRecord.h"
#include "../ModifiedOpenSeesServices/domain/load/ElementalLoadIter.h"
#include "../ModifiedOpenSeesServices/element/ElementalLoad.h"
#include "../ModifiedOpenSeesServices/domain/load/ElementSelfWeight.h"
#include "../ModifiedOpenSeesServices/domain/load/AccelerationField.h"
#include "../ModifiedOpenSeesServices/domain/load/BrickSurfaceLoad.h"
#include "../ModifiedOpenSeesServices/domain/load/BrickSurfaceLoad_from_Contact_Pore_Pressure.h"


// Damping models
//--------------------------------------------------------------------------------------------------
#include "Damping.h"
#include "Rayleigh.h"
#include "Caughey3rd.h"
#include "Caughey4th.h"


// Eigen
//--------------------------------------------------------------------------------------------------
#include "FrequencyAlgo.h"
#include "SymBandEigenSolver.h"
#include "EigenIntegrator.h"
#include "FullGenEigenSolver.h"
#include "FullGenEigenSOE.h"


// Database
//--------------------------------------------------------------------------------------------------
// #include <../ModifiedOpenSeesServices/database/FileDatastore.h>
// #include <../ModifiedOpenSeesServices/database/MySqlDatastore.h>
// #include <../ModifiedOpenSeesServices/database/MySqlDataRecorder.h>


// MOSSy Elements
//--------------------------------------------------------------------------------------------------
#include "../UCB_Elements/dispBeamColumn/DispBeamColumn3d.h"
#include "../UCB_Elements/truss/Truss.h"
#include "../UCB_Elements/elasticBeamLumpedMass/ElasticBeamLumpedMass.h"
#include "../UCB_Elements/shell/ShellMITC4.h"
#include "../UCB_Elements/shell/NewShellMITC4.h"


// Integration Rules
//--------------------------------------------------------------------------------------------------
#include "../UCB_Elements/integration/BeamIntegration.h"
#include "../UCB_Elements/integration/LobattoBeamIntegration.h"
#include "../UCB_Elements/integration/LegendreBeamIntegration.h"
#include "../UCB_Elements/integration/RadauBeamIntegration.h"
#include "../UCB_Elements/integration/NewtonCotesBeamIntegration.h"



// Uniaxial material models (concrete and steel)
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/material/uniaxial/UniaxialMaterial.h"
#include "../ModifiedOpenSeesServices/material/uniaxial/ElasticMaterial.h"
#include "../ModifiedOpenSeesServices/material/uniaxial/Steel01.h"
#include "../ModifiedOpenSeesServices/material/uniaxial/Steel02.h"
#include "../ModifiedOpenSeesServices/material/uniaxial/Concrete02.h"


// Materials for fiber based beams and trusses
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/material/section/fiber/UniaxialFiber3d.h"
#include "../ModifiedOpenSeesServices/material/section/FiberSection3d.h"
#include "../ModifiedOpenSeesServices/material/section/FiberSectionGJ.h"
#include "../ModifiedOpenSeesServices/material/section/repres/cell/Cell.h"
#include "../ModifiedOpenSeesServices/material/section/repres/reinfBar/ReinfBar.h"


// Patches (sections treated as materials)
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/material/section/repres/section/SectionRepres.h"
#include "../ModifiedOpenSeesServices/material/section/repres/section/FiberSectionRepr.h"
#include "../ModifiedOpenSeesServices/material/section/repres/patch/CircPatch.h"
#include "../ModifiedOpenSeesServices/material/section/repres/patch/QuadPatch.h"


// Reinforcing bars
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/material/section/repres/reinfLayer/CircReinfLayer.h"
#include "../ModifiedOpenSeesServices/material/section/repres/reinfLayer/StraightReinfLayer.h"
#include "../ModifiedOpenSeesServices/material/section/ElasticSection3d.h"
#include "../ModifiedOpenSeesServices/material/section/ElasticShearSection3d.h"


// For Shells
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/material/section/MembranePlateFiberSection.h"
#include "../ModifiedOpenSeesServices/material/section/ElasticMembranePlateSection.h"
#include "../ModifiedOpenSeesServices/material/section/ElasticPlateSection.h"


// Parallel Libraries
//--------------------------------------------------------------------------------------------------

#ifdef _PARALLEL_PROCESSING

#include <../ModifiedOpenSeesServices/analysis/integrator/DistributedDisplacementControl.h>
#include <../ModifiedOpenSeesServices/graph/partitioner/Metis.h>
#include <../ModifiedOpenSeesServices/graph/partitioner/ParMetis.h>
#include <../ModifiedOpenSeesServices/domain/loadBalancer/ShedHeaviest.h>
#include <../ModifiedOpenSeesServices/domain/partitioner/DomainPartitioner.h>
#include <../ModifiedOpenSeesServices/graph/partitioner/GraphPartitioner.h>

#include <../ModifiedOpenSeesServices/actor/machineBroker/MPI_MachineBroker.h>
#include <../ModifiedOpenSeesServices/domain/subdomain/Subdomain.h>
#include <../ModifiedOpenSeesServices/domain/domain/SubdomainIter.h>
#include <../ModifiedOpenSeesServices/actor/machineBroker/MachineBroker.h>
#include <../ModifiedOpenSeesServices/domain/subdomain/ShadowSubdomain.h>

#include <../ModifiedOpenSeesServices/analysis/analysis/StaticDomainDecompositionAnalysis.h>
#include <../ModifiedOpenSeesServices/analysis/analysis/TransientDomainDecompositionAnalysis.h>
#include <../ModifiedOpenSeesServices/analysis/numberer/ParallelNumberer.h>

#endif




// =================================================================================================
// Setup program
// =================================================================================================

ErrorHandler *g3ErrorHandler = 0;
double        ops_Dt         = 0.0;

// Pointers to components
static AnalysisModel *theAnalysisModel                                                 = 0;
static EquiSolnAlgo *theAlgorithm                                                      = 0;
static ConstraintHandler *theHandler                                         = 0;
static DOF_Numberer *theNumberer                                                       = 0;
static LinearSOE *theSOE                                                               = 0;
static StaticAnalysis *theStaticAnalysis                                               = 0;
static DirectIntegrationAnalysis *theTransientAnalysis                                 = 0;
static VariableTimeStepDirectIntegrationAnalysis *theVariableTimeStepTransientAnalysis = 0;
static StaticIntegrator *theStaticIntegrator                                           = 0;
static TransientIntegrator *theTransientIntegrator                                     = 0;
static ConvergenceTest *theConvergenceTest                                             = 0;
static EigenSOE *theEigenSOE                                                           = 0;
static RCM  *theRCM = new RCM();  // Not used in parallel


// Store names of model and stages
static string StageName;
static string ModelName;
static bool binary_output;
static bool exodusii_output;


// Numberers are hard-coded. For parallel processing use the Parallel numberer
// for sequential use RCM.
#ifdef _PARALLEL_PROCESSING
static ParallelNumberer *theParallelNumberer = new ParallelNumberer();
#endif

// The object broker
//FEM_ObjectBroker theBroker;
//Element      *ops_TheActiveElement = 0;

// Initialize Empty Domain

#ifdef _PARALLEL_PROCESSING
PartitionedDomain theDomain;


int OPS_PARALLEL_PROCESSING = 0;
int OPS_NUM_SUBDOMAINS      = 0;
bool OPS_PARTITIONED        = false;
bool OPS_REDEFINE_ANALYSIS = true;
DomainPartitioner *OPS_DOMAIN_PARTITIONER = 0;
GraphPartitioner  *OPS_GRAPH_PARTITIONER = 0;
LoadBalancer      *OPS_BALANCER = 0;
FEM_ObjectBroker  *OPS_OBJECT_BROKER;
MachineBroker     *OPS_MACHINE;
static bool builtModel = false;
static char *resDataPtr = 0;
static int resDataSize = 0;
static Timer *theTimer = 0;
Element      *ops_TheActiveElement = 0;


#else // If not _PARALLEL_PROCESSING
Domain theDomain;


// endif _PARALLEL_PROCESSING
#endif


LoadPattern *DRM_LOAD_PATTERN = 0 ; //Pointer to DRM Load pattern to be available ftom everywhere ... added by Babak
Channel          **OPS_theChannels = 0;



// =================================================================================================
// Application Programming Interface (API)
// =================================================================================================
#include "api.h"
