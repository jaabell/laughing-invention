////////////////////////////////////////////////////////////////////////////////////
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
// VERSION:           001
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// Modified by :       Babak Kamrani to be matched for parallel version
//
// DATE:              September 2010
//
///////////////////////////////////////////////////////////////////////////////


// standard C++ includes
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <StandardStream.h>
#include <FileStream.h>
# include <mpi.h>


// UCD CompGeoMech nDarray (tensor, vector, matrix)
#include "../nDarray/BJtensor.h"
#include "../nDarray/stresst.h"
#include "../nDarray/straint.h"
#include "../nDarray/BJvector.h"
#include "../nDarray/BJmatrix.h"
#include "../nDarray/stresst.h"

//Modified OpenSees Services (MOSS) includes
#include "../ModifiedOpenSeesServices/handler/ConsoleErrorHandler.h"
#include "../ModifiedOpenSeesServices/tagged/storage/ArrayOfTaggedObjects.h"
#include "../ModifiedOpenSeesServices/tagged/storage/MapOfTaggedObjects.h"
#include "../ModifiedOpenSeesServices/tagged/storage/MapOfTaggedObjectsIter.h"
#include "../ModifiedOpenSeesServices/G3Globals.h"

// Domain classes
//#include "../ModifiedOpenSeesServices/domain/domain/Domain.h"
#include "../ModifiedOpenSeesServices/domain/domain/partitioned/PartitionedDomain.h"
#include "../ModifiedOpenSeesServices/domain/node/Node.h"

// got from Nima on 06/26/2012:
#include "../ModifiedOpenSeesServices/domain/pattern/Linear_Force_TimeHistory.h"
//#include "../ModifiedOpenSeesServices/domain/pattern/SineWave_Force_TimeHistory.h"
//#include "../ModifiedOpenSeesServices/domain/pattern/Ricker1st_Force_TimeHistory.h"
//#include "../ModifiedOpenSeesServices/domain/pattern/Ricker2nd_Force_TimeHistory.h"
//#include "../ModifiedOpenSeesServices/domain/pattern/Ormsby_Force_TimeHistory.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathSeries_Force_TimeHistory.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathSeries_Disp_TimeHistory_Penalty.h"

//

// Constraints
#include "../ModifiedOpenSeesServices/domain/constraints/SP_Constraint.h"
#include "../ModifiedOpenSeesServices/domain/constraints/MP_Constraint.h"
#include "../ModifiedOpenSeesServices/domain/domain/SP_ConstraintIter.h"
#include "../ModifiedOpenSeesServices/domain/domain/MP_ConstraintIter.h"

// Constraint Handlers
#include "../ModifiedOpenSeesServices/analysis/handler/PlainHandler.h"
#include "../ModifiedOpenSeesServices/analysis/handler/PenaltyConstraintHandler.h"
#include "../ModifiedOpenSeesServices/analysis/handler/LagrangeConstraintHandler.h"
#include "../ModifiedOpenSeesServices/analysis/handler/TransformationConstraintHandler.h"
// Analysis classes
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
#include "../ModifiedOpenSeesServices/analysis/integrator/DistributedDisplacementControl.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/ArcLength.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/ArcLength1.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/MinUnbalDispNorm.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/Newmark.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/WilsonTheta.h"
#include "../ModifiedOpenSeesServices/analysis/integrator/HHT_alpha_method.h"
#include "../ModifiedOpenSeesServices/analysis/analysis/EigenAnalysis.h"

// numberers
#include "../ModifiedOpenSeesServices/analysis/numberer/PlainNumberer.h"
#include "../ModifiedOpenSeesServices/analysis/numberer/DOF_Numberer.h"
#include "../ModifiedOpenSeesServices/graph/numberer/RCM.h"
#include "../ModifiedOpenSeesServices/analysis/numberer/ParallelNumberer.h"

// convergence tests
#include "../ModifiedOpenSeesServices/convergenceTest/CTestNormUnbalance.h"
#include "../ModifiedOpenSeesServices/convergenceTest/CTestNormDispIncr.h"
#include "../ModifiedOpenSeesServices/convergenceTest/CTestEnergyIncr.h"

// soln algorithms
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/Linear.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/NewtonRaphson.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/NewtonLineSearch.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/ModifiedNewton.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/BFGS.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/KrylovNewton.h"

// line searches
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/BisectionLineSearch.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/InitialInterpolatedLineSearch.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/RegulaFalsiLineSearch.h"
#include "../ModifiedOpenSeesServices/analysis/algorithm/equiSolnAlgo/SecantLineSearch.h"

// solvers
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectThreadSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectSkypackSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinDirectSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSOE.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/bandSPD/BandSPDLinSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/bandSPD/BandSPDLinSOE.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/bandSPD/BandSPDLinLapackSolver.h"
// #include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN3.1/UmfpackGenLinSOE.h"
// #include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN3.1/UmfpackGenLinSolver.h"
// #include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSOE.h"
// #include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSolver.h"
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/petsc/PetscSolver.h" //Babak added
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/petsc/PetscSOE.h"   //Babak added
#include "../ModifiedOpenSeesServices/system_of_eqn/linearSOE/petsc/PetscSparseSeqSolver.h"//Babak added

// recorders
#include "../ModifiedOpenSeesServices/recorder/NodeRecorder.h"
#include "../ModifiedOpenSeesServices/recorder/ElementRecorder.h"
#include "../ModifiedOpenSeesServices/matrix/ID.h"
#include "../ModifiedOpenSeesServices/handler/DataOutputHandler.h"
#include "../ModifiedOpenSeesServices/handler/DataOutputFileHandler.h"

//utility
#include "../ModifiedOpenSeesServices/utility/Timer.h"
//******************** Rayleigh Damping ***********************
#include "../ModifiedOpenSeesServices/domain/damping/Damping.h"
#include "../ModifiedOpenSeesServices/domain/damping/Rayleigh.h"
#include "../ModifiedOpenSeesServices/domain/damping/Caughey3rd.h"
#include "../ModifiedOpenSeesServices/domain/damping/Caughey4th.h"

// //***************************************************************

using std::ios;
using namespace std;
using std::ofstream; // Babak added
using std::endl; // Babak added

StandardStream sserr;
OPS_Stream& opserr = sserr;
OPS_Stream* opserrPtr;

ErrorHandler* g3ErrorHandler = 0;
double        ops_Dt = 0.0;


//************************ Analysis Objects *******************************

static AnalysisModel* theAnalysisModel = 0;
static EquiSolnAlgo* theAlgorithm = 0;
//static ConstraintHandler *theConstraintHandler =0; // Out by Babak at 10/02/2011
static DOF_Numberer* theNumberer = 0;
static LinearSOE* theSOE = 0;
static StaticAnalysis* theStaticAnalysis = 0;
static DirectIntegrationAnalysis* theTransientAnalysis = 0;
static VariableTimeStepDirectIntegrationAnalysis* theVariableTimeStepTransientAnalysis = 0;
static StaticIntegrator* theStaticIntegrator = 0;
static TransientIntegrator* theTransientIntegrator = 0;
static ConvergenceTest* theConvergenceTest = 0;
static ConstraintHandler* theHandler = 0;



static bool builtModel = false;

static EigenAnalysis* theEigenAnalysis = 0;

static char* resDataPtr = 0;
static int resDataSize = 0;
static Timer* theTimer = 0;

FE_Datastore* theDatabase  = 0;

//Domain       *ops_TheActiveDomain = 0;
Element*      ops_TheActiveElement = 0;

PartitionedDomain theDomain;
int OPS_PARALLEL_PROCESSING = 0;
int OPS_NUM_SUBDOMAINS      = 0;
bool OPS_PARTITIONED        = false;
bool OPS_REDEFINE_ANALYSIS = true; //Guanzhou
DomainPartitioner* OPS_DOMAIN_PARTITIONER = 0;
//GraphPartitioner  *OPS_GRAPH_PARTITIONER =0;
//  LoadBalancer      *OPS_BALANCER = 0;
//  FEM_ObjectBroker  *OPS_OBJECT_BROKER;
//  MachineBroker     *OPS_MACHINE;
Channel**          OPS_theChannels = 0;

#ifdef _BABAK_DEBUG
LoadPattern* DRM_LOAD_PATTERN = 0 ; //Pornter to DRM Load pattern to be available ftom everywhere ... added by Babak
#endif






// Contact elements
//#ifdef Contact_Elements
#include "../CompGeoMechUCD_FiniteElements/Contact_Elements/ContactElement_3DOF_3DOF.h"
#include "../CompGeoMechUCD_FiniteElements/Contact_Elements/ContactElement_Nonlinear_3DOF_3DOF.h"
// #include "../CompGeoMechUCD_FiniteElements/Contact_Elements/ContactElement_3DOF_6DOF.h"
// #include "../CompGeoMechUCD_FiniteElements/Contact_Elements/ContactElement_6DOF_6DOF.h"
// #include "../CompGeoMechUCD_FiniteElements/Contact_Elements/Nonlinear_two_functions_ContactElement_3DOF_3DOF.h"
// #include "../CompGeoMechUCD_FiniteElements/Contact_Elements/Nonlinear_two_functions_ContactElement_3DOF_6DOF.h"
// #include "../CompGeoMechUCD_FiniteElements/Contact_Elements/Nonlinear_two_functions_ContactElement_6DOF_6DOF.h"

// DSLs
#include "./MODELING/add_element_contact_3dof_to_3dof.h"
#include "./MODELING/add_element_contact_nonlinear_3dof_to_3dof.h"
//#include "add_element_contact_3dof_to_6dof.h"
//#include "add_element_contact_6dof_to_6dof.h"
//#include "add_element_nonlinear_two_functions_contact_3dof_to_3dof.h"
//#include "add_element_nonlinear_two_functions_contact_3dof_to_6dof.h"
//#include "add_element_nonlinear_two_functions_contact_6dof_to_6dof.h"



// // UCD CompGeoMech Material Modeling Out by Babak Kamrani 10/11/2011
#include "../CompGeoMechUCD_MaterialModeling/NDMaterial.h"

#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/NewTemplate3Dep.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/MaterialParameter.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/ElasticState.h"
#include "../CompGeoMechUCD_MaterialModeling/ElasticIsotropic3D.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Isotropic_Elastic.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/elnp_Elastic.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_Elastic.h"
#include "../CompGeoMechUCD_MaterialModeling/ElasticCrossAnisotropic.h"

#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/YieldFunction.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/PlasticFlow.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/ScalarEvolution.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Linear_Eeq.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/TensorEvolution.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Linear_Eij.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/AF_Eij.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/ElasticState.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Isotropic_Elastic.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/elnp_Elastic.h"

#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DP_YF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/VM_YF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/RMC_YF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/CC_YF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_YF.h"

#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DP_PF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/VM_PF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/RMC_PF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/CC_PF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_PF.h"

#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Linear_Eeq.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/CC_Ev.h"

#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Linear_Eij.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/AF_Eij.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_alpha_Eij.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_z_Eij.h"

#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_p0_bar.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_alpha_Eij.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_PF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_YF.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_Elastic.h"
#include "../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_z_Eij.h"
//

//   // UCD CompGeoMech Material Modeling Modified by Babak KAmrani for using Template3D instead of NewTemplate3D
// #include "../CompGeoMechUCD_MaterialModeling/NDMaterial.h"
//
#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/Template3Dep.h"
// //#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/MaterialParameter.h"
// //#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/ElasticState.h"
// #include "../CompGeoMechUCD_MaterialModeling/ElasticIsotropic3D.h"
// //#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/Isotropic_Elastic.h"
// //#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/elnp_Elastic.h"
// //#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/DM04_Elastic.h"
// #include "../CompGeoMechUCD_MaterialModeling/ElasticCrossAnisotropic.h"
//
// //#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/YieldFunction.h"
// //#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/PlasticFlow.h"
// //#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/ScalarEvolution.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/CAM_PS.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/CAM_YS.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/DP_PS01.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/DP_PS.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/DP_YS01.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/DP_YS.h"
#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/EL_LEeq.h"
//
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/EL_NLEij.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/EL_NLEijMD.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/EL_NLEp.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/EL_S.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/EL_T.h"
//
#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/EPState.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/MatPoint3D.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/MD_EL.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/MD_PS01.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/MD_PS.h"
//
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/MD_YS.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/PS.h"
//
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/RMC01.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/RMC01_PS.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/RMC01_YS.h"
// #include "../CompGeoMechUCD_MaterialModeling/Template3Dep/Tri_a_fail_crit_YS.h"
//
#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/VM_PS.h"
#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/VM_YS.h"
#include "../CompGeoMechUCD_MaterialModeling/Template3Dep/YS.h"


// DSLs
//  #include "./MODELING/add_constitutive_model_NDMaterial_camclay.h"
//#include "./MODELING/add_constitutive_model_NDMaterial_vonmises_perfectly_plastic.h"
//  #include "./MODELING/add_constitutive_model_NDMaterial_accelerated_vonmises_perfectly_plastic.h"
// #include "add_material_NDMaterial_vonmises_kinematic_hardening.h"
//  #include "./MODELING/add_constitutive_model_NDMaterial_vonmises_isotropic_hardening.h"
// #include "add_material_NDMaterial_druckerprager_perfectly_plastic.h"
// #include "add_material_NDMaterial_druckerprager_kinematic_hardening.h"
// #include "add_material_NDMaterial_druckerprager_isotropic_hardening.h"
// #include "add_material_NDMaterial_dafalias_manzari.h"
// #include "add_material_NDMaterial_sanisand.h"
#include "./MODELING/add_material_NDMaterial_linear_elastic_isotropic_3d.h"
#include "./MODELING/add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d.h"
// #include "add_material_NDMaterial_linear_elastic_crossanisotropic.h"
//#include "vonMisesPP.h"
//#include "add_material_constitutive_models_vonmises_perfectly_plastic.h"
//#include "vonMisesIH.h"
// #include "./MODELING/add_constitutive_model_NDMaterial_vonmises_isotropic_hardening.h"
// #include "./MODELING/add_material_NDMaterial_vonmises_isotropic_hardening_TDep.h"
// #include "./MODELING/add_material_NDMaterial_vonmises_perfectly_plastic.h"
// #include "./MODELING/add_material_NDMaterial_vonmises_isotropic_hardening.h"



// Uniaxial material models (concrete and steel)
#include "../ModifiedOpenSeesServices/material/uniaxial/UniaxialMaterial.h"
#include "../ModifiedOpenSeesServices/material/uniaxial/ElasticMaterial.h"
#include "../ModifiedOpenSeesServices/material/uniaxial/Steel01.h"
#include "../ModifiedOpenSeesServices/material/uniaxial/Steel02.h"
#include "../ModifiedOpenSeesServices/material/uniaxial/Concrete02.h"

// DSLs
// #include "add_material_uniaxial_elastic.h"
// #include "add_material_uniaxial_steel01.h"
// #include "add_material_uniaxial_steel02.h"
// #include "add_material_uniaxial_concrete02.h"


// UCD CompGeoMech solid elements
//#ifdef UCD_Brick_Elements


#include "../CompGeoMechUCD_FiniteElements/solid_brick_8_nodes/EightNodeBrick.h"
//#include "../CompGeoMechUCD_FiniteElements/EightNodeBrickElastic/EightNodeBrickElastic.h"
#include "../CompGeoMechUCD_FiniteElements/coupled_u-p-U_brick_8_nodes/EightNodeBrick_u_p_U.h"
#include "../CompGeoMechUCD_FiniteElements/coupled_u-p_brick_8_nodes/EightNode_Brick_u_p.h"
#include "../CompGeoMechUCD_FiniteElements/coupled_u-p_large_deformation_brick_8_nodes/EightNode_LDBrick_u_p.h"
#include "../CompGeoMechUCD_FiniteElements/coupled_u-p-U_brick_20_nodes/TwentyNodeBrick_u_p_U.h"
#include "../CompGeoMechUCD_FiniteElements/solid_brick_20_nodes/TwentyNodeBrick.h"
#include "../CompGeoMechUCD_FiniteElements/solid_brick_27_nodes/TwentySevenNodeBrick.h"
#include "../CompGeoMechUCD_FiniteElements/solid_brick_27_nodes_elastic/TwentySevenNodeBrickElastic.h"
#include "../CompGeoMechUCD_FiniteElements/solid_brick_large_deformation_total_Lagrangian_8_nodes/TotalLagrangianFD8NodeBrick.h"
#include "../CompGeoMechUCD_FiniteElements/solid_brick_large_deformation_total_Lagrangian_20_nodes/TotalLagrangianFD20NodeBrick.h"
#include "../CompGeoMechUCD_FiniteElements/elastic_beam/ElasticBeam.h"
#include "../CompGeoMechUCD_FiniteElements/rank_one_deficient_elastic_pinned_fixed_beam/rank_one_deficient_elastic_pinned_fixed_beam.h" //Babak (May 2013)
#include "../CompGeoMechUCD_FiniteElements/shell_andes/ThreeNodeAndesShell.h"  // Jose Abell added July 22 2012
#include "../CompGeoMechUCD_FiniteElements/shell_andes/FourNodeAndesShell.h"  // Jose Abell added July 22 2012
#include "../CompGeoMechUCD_FiniteElements/penalty_element/PenaltyElement.h"  // Babak added June 2013
#include "../CompGeoMechUCD_FiniteElements/penalty_element_apply_displacement/PenaltyElementApplyDisplacement.h"  // Babak added June 2013


// DSLs
//#include "./MODELING/add_acceleration_field.h"
//#include "./MODELING/add_element_brick_8node_Parallel.h"
//#include "./MODELING/add_element_brick_8node_elastic.h"


// #include "add_element_brick_8node_up.h"
// #include "add_element_brick_8node_upU.h"
// #include "add_element_brick_20node.h"
// #include "add_element_brick_20node_upU.h"
// #include "./MODELING/add_element_brick_27node_parallel.h"
#include "./MODELING/add_element_brick_27node.h"
#include "./MODELING/add_element_brick_27node_elastic.h"

//#endif


// includes for loads - ground motions
//#ifdef Loads
#include "../ModifiedOpenSeesServices/domain/pattern/LoadPattern.h"
#include "../ModifiedOpenSeesServices/domain/pattern/LoadPatternIter.h"
#include "../ModifiedOpenSeesServices/domain/pattern/LinearSeries.h"
// #include "../ModifiedOpenSeesServices/domain/pattern/TrigSeries.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathSeries.h"
#include "../ModifiedOpenSeesServices/domain/pattern/ConstantSeries.h"
//#include "../ModifiedOpenSeesServices/domain/pattern/RectangularSeries.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathTimeSeries.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PathSeries.h"
#include "../ModifiedOpenSeesServices/domain/node/NodalLoad.h"
#include "../ModifiedOpenSeesServices/domain/pattern/PBowlLoading.h"
#include "../ModifiedOpenSeesServices/domain/pattern/Domain_Reduction_Method.h"
#include "../ModifiedOpenSeesServices/domain/pattern/Domain_Reduction_Method_Modified_Input.h"
#include "../ModifiedOpenSeesServices/domain/pattern/UniformExcitation.h"
#include "../ModifiedOpenSeesServices/domain/pattern/MultiSupportPattern.h"
#include "../ModifiedOpenSeesServices/domain/constraints/ImposedMotionSP.h"
#include "../ModifiedOpenSeesServices/domain/groundMotion/GroundMotion.h"
#include "../ModifiedOpenSeesServices/domain/pattern/TimeSeriesIntegrator.h"
#include "../ModifiedOpenSeesServices/domain/groundMotion/GroundMotionRecord.h"
#include "../ModifiedOpenSeesServices/domain/load/ElementalLoadIter.h"
#include "../ModifiedOpenSeesServices/element/ElementalLoad.h"
//#include "../ModifiedOpenSeesServices/domain/load/BrickSelfWeight.h"
#include "../ModifiedOpenSeesServices/domain/load/ElementSelfWeight.h"

// DSLs
//#include "add_selfweight_to_brick_element.h"
#include "./MODELING/add_load.h"
//#include "add_constant_load_pattern.h"
//#include "./MODELING/add_load_linear_pattern.h"
//#include "./MODELING/add_sinusoidal_load_pattern.h"
// #include "add_path_series_load_pattern.h"
// #include "add_ground_motion_for_dynamic_analysis.h"
// #include "./MODELING/add_sine_wave_for_uniform_excitation.h"
// #include "add_sine_wave_for_dynamic_analysis.h"
// #include "add_multiple_support_excitation_pattern.h"
// #include "add_imposed_motion_for_multisupport_dynamic_analysis.h"
//#include "add_domain_reduction_method_load_pattern.h"
#include "./MODELING/add_load_pattern_domain_reduction_method.h"
#include "./MODELING/add_load_selfweight_to_element.h"
//#include "add_plastic_bowl_method_load_pattern.h"
//#endif
//  #include "./MODELING/add_ormsby_wavelet_for_multisupport_excitation.h"
// #include "./MODELING/add_displacement_time_history_ormsby_wavelet.h"

//got from Nima on 06/26/2012:
#include "./MODELING/add_force_time_history_linear.h"
// #include "./MODELING//*add_force_time_history_sine_wave.h"
//#include "./MODELING/add_force_time_history_ricker1st_wavelet.h"
//#include "./MODELING/add_force_time_history_ricker2nd_wavelet.h"
//#include "./MODELING/add_force_time_history_ormsby_wavelet.h"
#include "./MODELING/add_force_time_history_path_series.h"
// #include "./MODELING/add_acceleration_field.h"
//#include "./MODELING/add_to_node_support.h"
//#include "./MODELING/add_support_to_nodes_from_file.h"
#include "./MODELING/add_support_to_node.h"
//#include "./MODELING/add_elements_8node_brick_from_file.h"
#include "./MODELING/add_support_to_all_dofs_of_node.h"
#include "./MODELING/add_support_from_file_to_nodes.h"
//

//#ifdef Remove_Commands
// DSLs
//#include "remove_element.h"
//#include "remove_load_pattern.h"
//#include "remove_node.h"
//#include "remove_node_support_by_fixity_number.h"
//#include "remove_node_support_by_node_number.h"
//#include "remove_output_to_file.h"
//#include "remove_all_output_to_files.h"
//#endif


//#ifdef Beam_Elements
// Integration Rules
//#include "../ModifiedOpenSeesServices/element/integration/BeamIntegration.h"
//#include "../ModifiedOpenSeesServices/element/integration/LobattoBeamIntegration.h"
//#include "../ModifiedOpenSeesServices/element/integration/LegendreBeamIntegration.h"
//#include "../ModifiedOpenSeesServices/element/integration/RadauBeamIntegration.h"
//#include "../ModifiedOpenSeesServices/element/integration/NewtonCotesBeamIntegration.h"

// beams and trusses
//#include "../ModifiedOpenSeesServices/coordTransformation/LinearCrdTransf3d.h"
//#include "../ModifiedOpenSeesServices/element/dispBeamColumn/DispBeamColumn3d.h"
//#include "../ModifiedOpenSeesServices/element/dispBeamColumn/DispBeamColumn2d.h"
//#include "../ModifiedOpenSeesServices/element/truss/Truss.h"
//#include "../ModifiedOpenSeesServices/element/elasticBeamColumn/ElasticBeam3d.h"
#include "../ModifiedOpenSeesServices/material/section/fiber/UniaxialFiber3d.h"
#include "../ModifiedOpenSeesServices/material/section/FiberSection3d.h"
#include "../ModifiedOpenSeesServices/material/section/FiberSectionGJ.h"
//#include "../ModifiedOpenSeesServices/element/forceBeamColumn/ForceBeamColumn3d.h"
//#include "../ModifiedOpenSeesServices/element/forceBeamColumn/ForceBeamColumn2d.h"
//#include "../ModifiedOpenSeesServices/element/nonlinearBeamColumn/element/NLBeamColumn3d.h"
#include "../ModifiedOpenSeesServices/material/section/repres/cell/Cell.h"
#include "../ModifiedOpenSeesServices/material/section/repres/reinfBar/ReinfBar.h"
// Patches
#include "../ModifiedOpenSeesServices/material/section/repres/section/SectionRepres.h"
#include "../ModifiedOpenSeesServices/material/section/repres/section/FiberSectionRepr.h"
#include "../ModifiedOpenSeesServices/material/section/repres/patch/CircPatch.h"
#include "../ModifiedOpenSeesServices/material/section/repres/patch/QuadPatch.h"
// Reinforcing bars
#include "../ModifiedOpenSeesServices/material/section/repres/reinfLayer/CircReinfLayer.h"
#include "../ModifiedOpenSeesServices/material/section/repres/reinfLayer/StraightReinfLayer.h"

// DSLs
// #include "add_element_beam_elastic_3d.h"
// #include "add_element_truss_element.h"
// #include "add_element_beam_displacement_based_3d.h"
// #include "add_element_beam_displacement_based_2d.h"
// #include "add_element_beam_force_based_3d.h"
// #include "add_element_beam_force_based_2d.h"
// #include "add_linear_3d_transformation_for_beam_element.h"
// #include "add_fiber_to_section.h"
// #include "add_build_section.h"
// #include "add_patch_circle_to_section.h"
// #include "add_patch_quad_to_section.h"
// #include "add_patch_rectangle_to_section.h"
// #include "add_reinforcing_bars_arc_to_section.h"
// #include "add_reinforcing_bars_circle_to_section.h"
// #include "add_reinforcing_bars_layer_to_section.h"
#include "./MODELING/add_element_beam_elastic.h"  // Nima Tafazzoli (Dec. 2012)
//#include "./MODELING/add_element_beam_rank_one_deficient_elastic_pinned_fixed_beam.h" //Babak (May 2013)
#include "./MODELING/add_element_shell_andes_4node.h" // Babak Kamrani (Dec 2012)
//#include "./MODELING/add_elements_from_file_shell_andes_4node.h" //Babak Kamrani (Jan 2012)
//#include "./MODELING/add_elements_from_file_shell_andes_4node_density_Specified.h"
#include "./MODELING/add_element_shell_andes_3node.h"//Babak Kamrani (Feb 2013)
//#include "./MODELING/add_elements_from_file_shell_andes_3node.h" //Babak Kamrani (Feb 2013)
#include "./MODELING/add_element_penalty.h" //Babak Kamrani (June 2013)
//#include "./MODELING/add_elements_from_file_penalty.h" //Babak Kamrani (June 2013)
#include "./MODELING/add_penalty_displacement_time_history_path_series.h"//Babak Kamrani (June 2013)
#include "./MODELING/add_element_penalty_for_applying_generalized_displacement.h" //Babak Kamrani (June 2013)
//#include "./MODELING/add_elements_from_file_penalty_for_applying_generalized_displacement.h"//Babak Kamrani (June 2013)
#include "./MODELING/add_elements_from_file_beam_rank_one_deficient_elastic_pinned_fixed_beam.h" //Babak Kamrani (June 2013)


//#endif



//***************** Including General DSL header files *****************

#include "./MODELING/add_node.h"
//#include "./MODELING/add_nodes_from_file.h"
#include "./MODELING/add_body_force.h"
#include "./MODELING/add_acceleration_field.h"
//#include "./MODELING/add_elements_8node_brick_from_file.h"
//#include "./MODELING/add_to_node_support.h"
//#include "./MODELING/add_support_to_nodes_from_file.h"
#include "./MODELING/add_equaldof_to_two_nodes.h"
#include "./MODELING/add_equaldof_to_two_nodes_from_file.h"
// #include "add_mass_to_node.h"
/*#include "add_single_point_constraint_to_node.h"*/


//#include "./MODELING/add_support_to_nodes_from_file.h"

// #include "add_equaldof_to_two_nodes.h"

// #include "return_element_mass.h"
// #include "return_node_displacement.h"
// #include "return_nodal_displacements_of_element.h"

// #include "define_output_of_node_to_screen.h"
// #include "define_output_of_element_to_screen.h"
//#include "define_output_of_node_to_file.h"
/*#include "define_output_of_all_nodes_to_file.h"*/
//#include "define_output_of_element_to_file.h"
//#include "define_output_of_all_elements_to_file.h"
//#include "define_output_of_range_of_elements_to_file.h"
//#include "define_output_of_range_of_nodes_to_file.h"

//#include "return_pseudo_time.h"
//#include "return_node_numbers_of_the_element.h"

// #include "define_algorithm_bfgs_for_analysis.h"
#include "./SIMULATION/define_algorithm_linear_for_analysis.h"
//#include "define_algorithm_with_no_convergence_check_for_analysis.h"
//#include "define_algorithm_krylovnewton_for_analysis.h"
#include "./SIMULATION/define_algorithm_newton_for_analysis.h"
//#include "define_algorithm_modifiednewton_for_analysis.h"

// #include "define_convergence_test_energyincrement_for_analysis.h"
#include "./SIMULATION/define_convergence_test_normdisplacementincrement_for_analysis.h"
//#include "define_convergence_test_normunbalance_for_analysis.h"

//#include "define_constraint_handler_lagrange_for_analysis.h"
//#include "./MODELING/define_constraint_handler_penalty_for_analysis.h"
#include "./MODELING/define_constraint_handler_plain_for_analysis.h"
//#include "./MODELING/define_constraint_handler_transformation_for_analysis.h"

#include "./SIMULATION/define_integrator_loadcontrol_for_static_analysis.h"
#include "./SIMULATION/define_static_solution_advancement_integrator_load_control.h"
#include "./SIMULATION/define_static_solution_advancement_integrator_displacement_control.h"
//#include "define_parallel_integrator_displacementcontrol_for_static_analysis.h"
//#include "define_integrator_newmark_for_dynamic_analysis.h"
//#include "define_dynamic_solution_advancement_integrator_newmark.h"
// #include "define_integrator_hht_for_dynamic_analysis.h"

//#include "define_parallel_numberer_rcm_for_analysis.h"
#include "./SIMULATION/define_parallel_numberer_plain_for_analysis.h"
#include"./SIMULATION/define_numberer_plain_for_analysis.h"

// #include "define_solver_umfpack_for_analysis.h"
//#include "define_solver_profilespd_for_analysis.h"

//#include "analyze_static_multistep.h"
// #include "analyze_static_onestep.h"
// #include "./SIMULATION/analyze_transient_multistep.h"
// #include "analyze_transient_onestep.h"
// #include "analyze_transient_variable_multistep.h"
// #include "analyze_transient_variable_onestep.h"



//************************************************************
//********* DSL from Files ***************

#include "./MODELING/add_nodes_from_file.h"
#include "./MODELING/add_elements_from_file_27node_brick.h"
#include "./MODELING/add_elements_from_file_27node_brick_elastic.h"
#include "./MODELING/add_elements_from_GIDfile_27node_brick_elastic.h" //Added by Babak May 2013
#include "./MODELING/add_elements_from_GIDfile_27node_brick_elastic_material_number_specified_in_file.h" //Added by Babak Aug 2013
#include "./MODELING/add_elements_from_GIDfile_27node_brick.h"
// #include "DSL_From_File/add_support_to_nodes_from_file.h"
// #include "DSL_From_File/add_equaldof_to_two_nodes_from_file.h"
// #include "DSL_From_File/add_materials_from_file.h"


//******************** New Material Templates ***********************
// //#include "vonMisesPP.h"
// #include "add_material_constitutive_models_vonmises_perfectly_plastic.h"
// #include "vonMisesIH.h"
// #include "add_material_constitutive_models_vonmises_isotropic_hardening.h"

//******************** Rayleigh Damping ***********************
//#include "add_rayleigh_damping_initial_stiffness.h"
//#include "add_rayleigh_damping_current_stiffness.h"
#include "./MODELING/add_damping_rayleigh.h"
#include "./MODELING/add_damping_to_element.h"
// #include "./MODELING/add_raileigh_damping_to_element_from_file.h"



//---------------------Needed library for parallel analysis-----
#include <../ModifiedOpenSeesServices/analysis/integrator/DistributedDisplacementControl.h>
#include <../ModifiedOpenSeesServices/graph/partitioner/Metis.h>
#include <../ModifiedOpenSeesServices/graph/partitioner/ParMetis.h>
#include <../ModifiedOpenSeesServices/domain/loadBalancer/ShedHeaviest.h>
#include <../ModifiedOpenSeesServices/domain/partitioner/DomainPartitioner.h>
#include <../ModifiedOpenSeesServices/graph/partitioner/GraphPartitioner.h>


#include <../ModifiedOpenSeesServices/actor/objectBroker/FEM_ObjectBroker.h>
#include <../ModifiedOpenSeesServices/actor/machineBroker/MPI_MachineBroker.h>
#include <../ModifiedOpenSeesServices/domain/subdomain/Subdomain.h>
#include <../ModifiedOpenSeesServices/domain/domain/SubdomainIter.h>
#include <../ModifiedOpenSeesServices/actor/machineBroker/MachineBroker.h>
#include <../ModifiedOpenSeesServices/domain/subdomain/ShadowSubdomain.h>

#include <../ModifiedOpenSeesServices/analysis/analysis/StaticDomainDecompositionAnalysis.h>
#include <../ModifiedOpenSeesServices/analysis/analysis/TransientDomainDecompositionAnalysis.h>
#include <../ModifiedOpenSeesServices/analysis/numberer/ParallelNumberer.h>
GraphPartitioner*  OPS_GRAPH_PARTITIONER = 0;
LoadBalancer*      OPS_BALANCER = 0;
FEM_ObjectBroker*  OPS_OBJECT_BROKER;
MachineBroker*     OPS_MACHINE;



#include "./SIMULATION/define_solver_petsc_for_analysis.h"
#include "./SIMULATION/define_solver_parallel_for_analysis.h"
#include "./MODELING/parallel_start_new_stage.h"

#include "./MODELING/parallel_analyze_static_multistep.h"
// #include "./SIMULATION/analyze_static_multistep.h"
// #include "./SIMULATION/analyze_transient_multistep.h"
#include "./SIMULATION/simulate_using_transient_multistep.h"



//******************** Accelerated Constitutive Models ***********************
//#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/ConstitutiveMaterialParameter.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesPP.h"
//#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesIH.h"
//#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesKH.h"
//#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/DruckerPragerKH.h"

#include "MODELING/add_constitutive_model_NDMaterial_accelerated_vonmises_perfectly_plastic.h"
// #include "MODELING/add_constitutive_model_NDMaterial_accelerated_vonmises_isotropic_hardening.h"
// #include "MODELING/add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening.h"
// #include "MODELING/add_constitutive_model_NDMaterial_accelerated_druckerprager_kinematic_hardening.h"


//****************************************************************************
