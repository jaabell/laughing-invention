///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell & Boris Jeremic
// DATE:               Jan 2013
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////


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
#include "../ModifiedOpenSeesServices/actor/channel/Channel.h"
#include "../ModifiedOpenSeesServices/actor/channel/HDF5_Channel.h"


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
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/ModifiedNewton.h"




// System of Equations (SOE) Solvers
//--------------------------------------------------------------------------------------------------
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSOE.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSOE.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSolver.h"
#ifdef _PARALLEL_PROCESSING
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/petsc/PetscSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/petsc/PetscSOE.h"
#endif

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
#include "../ModifiedOpenSeesServices/domain/pattern/Domain_Reduction_Method_HDF5_input.h"
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
#include "FullGenEigenSolver.h"
#include "FullGenEigenSOE.h"


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

#include <../CompGeoMechUCD_Miscellaneous/timing/ESSITimer.h>



// =================================================================================================
// Setup program
// =================================================================================================

ErrorHandler *g3ErrorHandler = 0;
double        ops_Dt         = 0.0;

// Pointers to components
static AnalysisModel                              *theAnalysisModel                     = 0;
static EquiSolnAlgo                               *theAlgorithm                         = 0;
static ConstraintHandler                          *theHandler                           = 0;
static DOF_Numberer                               *theNumberer                          = 0;
static LinearSOE                                  *theSOE                               = 0;
static StaticAnalysis                             *theStaticAnalysis                    = 0;
static DirectIntegrationAnalysis                  *theTransientAnalysis                 = 0;
static VariableTimeStepDirectIntegrationAnalysis *theVariableTimeStepTransientAnalysis  = 0;
static StaticIntegrator                           *theStaticIntegrator                  = 0;
static TransientIntegrator                        *theTransientIntegrator               = 0;
static ConvergenceTest                            *theConvergenceTest                   = 0;
static EigenSOE                                   *theEigenSOE                          = 0;
static RCM                                        *theRCM                               = new RCM();  // Not used in parallel


// Store names of model and stages
static string StageName;
static string ModelName;


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


int OPS_PARALLEL_PROCESSING                = 0;
int OPS_NUM_SUBDOMAINS                     = 0;
bool OPS_PARTITIONED                       = false;
bool OPS_REDEFINE_ANALYSIS                 = true;
DomainPartitioner *OPS_DOMAIN_PARTITIONER  = 0;
GraphPartitioner   *OPS_GRAPH_PARTITIONER  = 0;
LoadBalancer       *OPS_BALANCER           = 0;
FEM_ObjectBroker   *OPS_OBJECT_BROKER;
MachineBroker      *OPS_MACHINE;
Element            *ops_TheActiveElement   = 0;
MPI_MachineBroker  *theMachineBroker       = 0;

#else // If not _PARALLEL_PROCESSING

Domain theDomain;

#endif


LoadPattern *DRM_LOAD_PATTERN      = 0 ; //Pointer to DRM Load pattern to be available ftom everywhere ...
Channel          **OPS_theChannels = 0;

std::string profiling_results_filename = ""; //Where to put timing results

// =================================================================================================
// Application Programming Interface (API)
// =================================================================================================
#include "api.h"
