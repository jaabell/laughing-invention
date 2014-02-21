/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.27 $
// $Date: 2004/11/25 00:18:34 $
// $Source: /usr/local/cvs/OpenSees/SRC/actor/objectBroker/FEM_ObjectBroker.cpp,v $


// Written: fmk
// Revision: A
//
// Purpose: This file contains the class definition for FEM_ObjectBroker.
// FEM_ObjectBroker is is an object broker class for the finite element
// method. All methods are virtual to allow for subclasses; which can be
// used by programmers when introducing new subclasses of the main objects.
//
// What: "@(#) FEM_ObjectBroker.C, revA"

#include <FEM_ObjectBroker.h>

// ActorTypes
#include <ActorSubdomain.h>

// Convergence tests
#include <CTestNormUnbalance.h>
#include <CTestNormDispIncr.h>
#include <CTestEnergyIncr.h>

// graph numbering schemes
#include <RCM.h>
#include <MyRCM.h>
#include <SimpleNumberer.h>


// uniaxial material model header files
#include <ElasticMaterial.h>
#include <ElasticPPMaterial.h>
#include <ParallelMaterial.h>
#include <Concrete01.h>
#include <Steel01.h>
#include <HardeningMaterial.h>
#include <HystereticMaterial.h>
#include <EPPGapMaterial.h>
#include <ViscousMaterial.h>
#include <PathIndependentMaterial.h>
#include <SeriesMaterial.h>
#include <CableMaterial.h>
#include <ENTMaterial.h>
#include <MinMaxMaterial.h>

// Sections
#include <ElasticSection3d.h>
#include <GenericSection1d.h>
#include <SectionAggregator.h>
#include <FiberSection3d.h>
#include <ElasticPlateSection.h>
#include <ElasticMembranePlateSection.h>
#include <MembranePlateFiberSection.h>
#include <Bidirectional.h>

// NDMaterials
#include <ElasticIsotropic3D.h>
#include <PlaneStressMaterial.h>
#include <Template3Dep.h>
#include <NewTemplate3Dep.h>
#include <MaterialParameter.h>
#include <ElasticState.h>
#include <elnp_Elastic.h>
#include <PressureDependent_Elastic.h>
#include <DM04_Elastic.h>
#include <Isotropic_Elastic.h>

#include <YieldFunction.h>
#include <VM_YF.h>
#include <DP_YF.h>
#include <CC_YF.h>
#include <DM04_YF.h>

#include <PlasticFlow.h>
#include <VM_PF.h>
#include <DP_PF.h>
#include <CC_PF.h>
#include <DM04_PF.h>

#include <ScalarEvolution.h>
#include <Linear_Eeq.h>
#include <CC_Ev.h>

#include <TensorEvolution.h>
#include <Linear_Eij.h>
#include <AF_Eij.h>
#include <DM04_alpha_Eij.h>
#include <DM04_z_Eij.h>

//#include <PressureDependentElastic3D.h>
//YieldSurface includes
#include <YS.h>
#include <CAM_YS.h>
#include <DP_YS01.h>
#include <DP_YS.h>
#include <MD_YS.h>
#include <RMC01_YS.h>
#include <VM_YS.h>
#include <Tri_a_fail_crit_YS.h>
//Potential Surface include
#include <PS.h>
#include <CAM_PS.h>
#include <DP_PS.h>
#include <MD_PS01.h>
#include <MD_PS.h>
#include <RMC01_PS.h>
#include <VM_PS.h>
//EvolutionLaw includes
#include <EL_S.h>
#include <EL_T.h>
#include <EL_LEeq.h>
#include <EL_NLEeq.h>
#include <EL_NLEp.h>
#include <EL_LEij.h>
#include <EL_NLEij.h>
#include <EL_NLEijMD.h>

// Boris Jeremic took out 20Mar2008
//#include <FluidSolidPorousMaterial.h>
//#include <PressureDependMultiYield.h>
//#include <PressureIndependMultiYield.h>
//
// Fibers
#include <UniaxialFiber3d.h>
// element header files
#include <Element.h>
// #include <beam3d01.h>
// #include <beam3d02.h>
#include <Truss.h>
#include <TrussSection.h>
#include <CorotTruss.h>
#include <CorotTrussSection.h>
#include <ZeroLength.h>
#include <ZeroLengthSection.h>
//#include <ZeroLengthND.h>
// #include <FourNodeQuad.h>
// #include <EnhancedQuad.h>
// #include <NineNodeMixedQuad.h>
// #include <ConstantPressureVolumeQuad.h>
#include <ElasticBeamLumpedMass.h>
#include <ElasticBeam.h>
#include <BeamWithHinges3d.h>
// #include <NLBeamColumn3d.h>
// #include <ForceBeamColumn3d.h>

#include <DispBeamColumn3d.h>
#include <ShellMITC4.h>
#include <ElementSelfWeight.h>

//Contents of CompGeoMechUCD_FiniteElements (Jose Abell, Fri 31 Jan 2014 02:41:19 PM PST)
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_8_nodes/EightNodeBrick.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_8_nodes_variable_number_of_gauss_points/EightNodeBrickVariableGP.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_8_nodes_elastic/EightNodeBrickElastic.h>
#include <../../../CompGeoMechUCD_FiniteElements/coupled_u-p-U_brick_8_nodes/EightNodeBrick_u_p_U.h>
#include <../../../CompGeoMechUCD_FiniteElements/coupled_u-p_brick_8_nodes/EightNode_Brick_u_p.h>
#include <../../../CompGeoMechUCD_FiniteElements/coupled_u-p_large_deformation_brick_8_nodes/EightNode_LDBrick_u_p.h>
#include <../../../CompGeoMechUCD_FiniteElements/coupled_u-p-U_brick_20_nodes/TwentyNodeBrick_u_p_U.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_20_nodes/TwentyNodeBrick.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_20_nodes_elastic/TwentyNodeBrickElastic.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_20_nodes_variable_number_of_gauss_points/TwentyNodeBrickVariableGP.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_27_nodes_variable_number_of_gauss_points/TwentySevenNodeBrickVariableGP.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_27_nodes/TwentySevenNodeBrick.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_27_nodes_elastic/TwentySevenNodeBrickElastic.h>
#include <../../../CompGeoMechUCD_FiniteElements/penalty_element_apply_displacement/PenaltyElementApplyDisplacement.h>
#include <../../../CompGeoMechUCD_FiniteElements/penalty_element/PenaltyElement.h>
#include <../../../CompGeoMechUCD_FiniteElements/shell_andes/ThreeNodeAndesShell.h>
#include <../../../CompGeoMechUCD_FiniteElements/shell_andes/FourNodeAndesShell.h>
#include <../../../CompGeoMechUCD_FiniteElements/elastic_beam/ElasticBeam.h>
#include <../../../CompGeoMechUCD_FiniteElements/rank_one_deficient_elastic_pinned_fixed_beam/rank_one_deficient_elastic_pinned_fixed_beam.h>
#include <../../../CompGeoMechUCD_FiniteElements/solid_brick_variable_nodes_8_to_27/VariableNodeBrick.h>
#include "../../../CompGeoMechUCD_FiniteElements/Contact_Elements/ContactElement_3DOF_3DOF.h"
#include "../../../CompGeoMechUCD_FiniteElements/Contact_Elements/ContactElement_Nonlinear_3DOF_7DOF.h"
#include "../../../CompGeoMechUCD_FiniteElements/Contact_Elements/ContactElement_Nonlinear_3DOF_3DOF.h"
#include "../../../CompGeoMechUCD_FiniteElements/solid_brick_large_deformation_total_Lagrangian_8_nodes/TotalLagrangianFD8NodeBrick.h"
#include "../../../CompGeoMechUCD_FiniteElements/solid_brick_large_deformation_total_Lagrangian_20_nodes/TotalLagrangianFD20NodeBrick.h"
#include "../../../CompGeoMechUCD_FiniteElements/solid_brick_8_nodes_ltensor/EightNodeBrickLT.h"

// UCD CompGeoMech Material Modeling
//#ifdef UCD_Materials
#include "../../../CompGeoMechUCD_MaterialModeling/NDMaterial.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/NewTemplate3Dep.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/MaterialParameter.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/ElasticState.h"
#include "../../../CompGeoMechUCD_MaterialModeling/ElasticIsotropic3D.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Isotropic_Elastic.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/elnp_Elastic.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_Elastic.h"
#include "../../../CompGeoMechUCD_MaterialModeling/ElasticCrossAnisotropic.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/YieldFunction.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/PlasticFlow.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/ScalarEvolution.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Linear_Eeq.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/TensorEvolution.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Linear_Eij.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/AF_Eij.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/ElasticState.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Isotropic_Elastic.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/elnp_Elastic.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DP_YF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/VM_YF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/RMC_YF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/CC_YF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_YF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DP_PF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/VM_PF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/RMC_PF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/CC_PF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_PF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Linear_Eeq.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/CC_Ev.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/Linear_Eij.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/AF_Eij.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_alpha_Eij.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/DM04_z_Eij.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_p0_bar.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_alpha_Eij.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_PF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_YF.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_Elastic.h"
#include "../../../CompGeoMechUCD_MaterialModeling/NewTemplate3Dep/SANISAND_z_Eij.h"
#include "../../../CompGeoMechUCD_MaterialModeling/LTensor_Based/NDMaterialLT.h"
#include "../../../CompGeoMechUCD_MaterialModeling/LTensor_Based/NDMaterialLT.h"
#include "../../../CompGeoMechUCD_MaterialModeling/LTensor_Based/vonMisesPPLT.h"
#include "../../../CompGeoMechUCD_MaterialModeling/LTensor_Based/PisanoLT.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/ConstitutiveMaterialParameter.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesPP.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesIH.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesKH.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesLinearKH.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/DruckerPragerPP.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/DruckerPragerIH.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/DruckerPragerKH.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/CamClay.h"
#include "../../../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/Pisano_model.h"






#include <PathSeries_Disp_TimeHistory_Penalty.h>

#include <HingeMidpointBeamIntegration3d.h>
#include <HingeRadauBeamIntegration3d.h>
#include <HingeRadauTwoBeamIntegration3d.h>
#include <LobattoBeamIntegration.h>

// node header files
#include <Node.h>


//#include <DataOutputStreamHandler.h>
//#include <DataOutputFileHandler.h>
//#include <DataOutputDatabaseHandler.h>

// #include <NodeRecorder.h>
// #include <ElementRecorder.h>
// #include <EnvelopeNodeRecorder.h>
// #include <EnvelopeElementRecorder.h>

// Constraints
#include <MP_Constraint.h>
#include <SP_Constraint.h>
#include <ImposedMotionSP.h>

// nodal load header files
#include <NodalLoad.h>

// elemental load header files
#include <ElementalLoad.h>
#include <Beam3dUniformLoad.h>
#include <Beam3dPointLoad.h>
#include <ElementSelfWeight.h> //Added by Babak 12/03/13

// matrix, vector & id header files
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>

// subdomain header files
#include <Subdomain.h>

// constraint handler header files
#include <ConstraintHandler.h>
#include <PlainHandler.h>
#include <PenaltyConstraintHandler.h>
#include <LagrangeConstraintHandler.h>
#include <TransformationConstraintHandler.h>

// dof numberer header files
#include <DOF_Numberer.h>
#include <PlainNumberer.h>

// analysis model header files
#include <AnalysisModel.h>

// equi soln algo header files
#include <EquiSolnAlgo.h>
#include <Linear.h>
#include <NewtonRaphson.h>
#include <ModifiedNewton.h>

// domain decomp soln algo header files
#include <DomainDecompAlgo.h>

// integrator header files
#include <LoadControl.h>
#include <ArcLength.h>
#include <TransientIntegrator.h>
#include <Newmark.h>
#include <DisplacementControl.h>

// system of eqn header files
#include <LinearSOE.h>
#include <DomainSolver.h>




#ifdef _PARALLEL_PROCESSING
#include <DistributedDisplacementControl.h>
#include <PetscSOE.h>
#include <PetscSolver.h>
#include <SparseGenColLinSOE.h>
#include <PetscSparseSeqSolver.h>
#include <DistributedBandSPDLinSOE.h>
#include <DistributedProfileSPDLinSOE.h>
#include <DistributedSparseGenColLinSOE.h>
#include <DistributedBandGenLinSOE.h>
#include <ParallelNumberer.h>
#include <StaticDomainDecompositionAnalysis.h>
#include <TransientDomainDecompositionAnalysis.h>

#else

#include <FullGenLinSOE.h>
#include <FullGenLinLapackSolver.h>
#include <BandGenLinSOE.h>
#include <BandGenLinLapackSolver.h>
#include <BandSPDLinSOE.h>
#include <BandSPDLinLapackSolver.h>
#include <ProfileSPDLinSOE.h>
#include <ProfileSPDLinDirectSolver.h>
#include <ProfileSPDLinSubstrSolver.h>
#include <SparseGenColLinSOE.h>
#include <SuperLU.h>
#include <UmfpackGenLinSOE.h> //Guanzhou added
#include <UmfpackGenLinSolver.h> //Guanzhou added
#endif

#include <DomainDecompositionAnalysis.h>

// load patterns
#include <LoadPattern.h>
#include <UniformExcitation.h>
#include <GroundMotion.h>
#include <MultiSupportPattern.h>
#include <PBowlLoading.h>

// time series
#include <LinearSeries.h>
#include <PathSeries.h>
#include <PathTimeSeries.h>
#include <ConstantSeries.h>

// time series integrators
#include <TrapezoidalTimeSeriesIntegrator.h>


#ifdef _PARALLEL_PROCESSING

#endif



#include <Linear_Force_TimeHistory.h>
#include <PathSeries_Force_TimeHistory.h>
#include <PathTimeSeries_Force_TimeHistory.h>
#include <Linear_Disp_TimeHistory_Penalty.h>

#include <Domain_Reduction_Method_Modified_Input.h>



#include <iostream>
using namespace std;

FEM_ObjectBroker::FEM_ObjectBroker()
    : lastLinearSolver(0), lastDomainSolver(0)
{

}


FEM_ObjectBroker::~FEM_ObjectBroker()
{

}


Actor *
FEM_ObjectBroker::getNewActor(int classTag, Channel *theChannel)
{
    switch (classTag)
    {

#ifdef _PARALLEL_PROCESSING

        case ACTOR_TAGS_SUBDOMAIN:
            return new ActorSubdomain(*theChannel, *this);
#endif

        default:
            cerr << "FEM_ObjectBroker::getNewActor - ";
            cerr << " - no ActorType type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}


PartitionedModelBuilder *
FEM_ObjectBroker::getPtrNewPartitionedModelBuilder(Subdomain &theSubdomain,
        int classTag)
{
    switch (classTag)
    {
            /*
            case PartitionedModelBuilder_TAGS_PartitionedQuick2dFrameModel:
                 return new PartitionedQuick2dFrame(theSubdomain);
                 */

        default:
            cerr << "FEM_ObjectBroker::getPtrNewPartitionedModelBuilder - ";
            cerr << " - no PartitionedModelBuilder type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


GraphNumberer *
FEM_ObjectBroker::getPtrNewGraphNumberer(int classTag)
{
    switch (classTag)
    {
        case GraphNUMBERER_TAG_RCM:
            return new RCM();


        case GraphNUMBERER_TAG_MyRCM:
            return new MyRCM();


        case GraphNUMBERER_TAG_SimpleNumberer:
            return new SimpleNumberer();


        default:
            cerr << "ObjectBroker::getPtrNewGraphNumberer - ";
            cerr << " - no GraphNumberer type exists for class tag " ;
            cerr << classTag << endln;
            return 0;

    }
}

/*****************************************
 *
 * METHODS TO GET NEW MODELLING CLASSES
 *
 *****************************************/








Element *
FEM_ObjectBroker::getNewElement(int classTag)
{
    switch (classTag)
    {
        case ELE_TAG_EightNodeBrick:            //Guanzhou added //temp out by babak
            return new EightNodeBrick();//temp out by babak

        case ELE_TAG_TwentySevenNodeBrick:          //Babak added on 10/18/2012
            return new TwentySevenNodeBrick();

        case ELE_TAG_TwentySevenNodeBrickElastic:           //Babak added on 11/16/2012
            return new TwentySevenNodeBrickElastic();

        case ELE_TAG_ElasticBeam:            //Babak added on 12/14/2012
            return new ElasticBeam();

        case ELE_TAG_FourNodeAndesShell:
            return new FourNodeAndesShell();

        case ELE_TAG_ThreeNodeAndesShell:           //Babak added on 2/11/2013
            return new ThreeNodeAndesShell();

        case ELE_TAG_rank_one_deficient_elastic_pinned_fixed_beam:          //Babak added on 5/8/2013
            return new rank_one_deficient_elastic_pinned_fixed_beam();

        case ELE_TAG_EightNodeBrickElastic:
            return new EightNodeBrickElastic();

        case ELE_TAG_PenaltyElement:            //Babak added on 6/19/13
            return new PenaltyElement();

        case ELE_TAG_PenaltyElementApplyDisplacement:           //Babak added on 6/25/13
            return new PenaltyElementApplyDisplacement();

        case ELE_TAG_ContactElement_3DOF_3DOF:          //Babak added on 11/05/13
            return new ContactElement_3DOF_3DOF();

        case ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF:            //Babak added on 11/25/13
            return new ContactElement_Nonlinear_3DOF_3DOF();


            break;



            break;

        default:
            cerr << "FEM_ObjectBroker::getNewElement - ";
            cerr << " - no Element type exists for class tag " ;
            cerr << classTag << endln;
            return 0;

    }
}

Node *
FEM_ObjectBroker::getNewNode(int classTag)
{
    switch (classTag)
    {
        case NOD_TAG_Node:
            return new Node(classTag);


        default:
            cerr << "FEM_ObjectBroker::getNewNode - ";
            cerr << " - no Node type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


MP_Constraint *
FEM_ObjectBroker::getNewMP(int classTag)
{
    switch (classTag)
    {
        case CNSTRNT_TAG_MP_Constraint:
            return new MP_Constraint( 0 , classTag);

            //      case CNSTRNT_TAG_MP_Joint2D:            // Arash //out by babak oct 2012
            //       return new MP_Joint2D();           // Arash //out by babak oct 2012

        default:
            cerr << "FEM_ObjectBroker::getNewMP - ";
            cerr << " - no SP_Constraint type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


SP_Constraint *
FEM_ObjectBroker::getNewSP(int classTag)
{
    switch (classTag)
    {
        case CNSTRNT_TAG_SP_Constraint:
            return new SP_Constraint(classTag);

        case CNSTRNT_TAG_ImposedMotionSP:
            return new ImposedMotionSP();
            break;

        default:
            cerr << "FEM_ObjectBroker::getNewSP - ";
            cerr << " - no SP_Constraint type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}

NodalLoad *
FEM_ObjectBroker::getNewNodalLoad(int classTag)
{
    switch (classTag)
    {
        case LOAD_TAG_NodalLoad:
            return new NodalLoad(classTag);


        default:
            cerr << "FEM_ObjectBroker::getNewNodalLoad - ";
            cerr << " - no NodalLoad type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


ElementalLoad *
FEM_ObjectBroker::getNewElementalLoad(int classTag)
{
    switch (classTag)
    {


        case LOAD_TAG_Beam3dUniformLoad:
            return new Beam3dUniformLoad();

        case LOAD_TAG_Beam3dPointLoad:
            return new Beam3dPointLoad();

        case LOAD_TAG_ElementSelfWeight:
            return new ElementSelfWeight();

        default:
            cerr << "FEM_ObjectBroker::getNewNodalLoad - ";
            cerr << " - no NodalLoad type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }

    return 0;
}



BeamIntegration *
FEM_ObjectBroker::getNewBeamIntegration(int classTag)
{
    switch (classTag)
    {
        case BEAM_INTEGRATION_TAG_Lobatto:
            return new LobattoBeamIntegration();

            //out by babak on Oct 2012:
            //--------
            //     case BEAM_INTEGRATION_TAG_Lobatto:
            //       return new LobattoBeamIntegration();
            //
            //     case BEAM_INTEGRATION_TAG_HingeMidpoint2d:
            //       return new HingeMidpointBeamIntegration2d();
            //
            //     case BEAM_INTEGRATION_TAG_HingeRadau2d:
            //       return new HingeRadauBeamIntegration2d();
            //
            //     case BEAM_INTEGRATION_TAG_HingeRadauTwo2d:
            //       return new HingeRadauTwoBeamIntegration2d();
            //
            //     case BEAM_INTEGRATION_TAG_HingeMidpoint3d:
            //       return new HingeMidpointBeamIntegration3d();
            //
            //     case BEAM_INTEGRATION_TAG_HingeRadau3d:
            //       return new HingeRadauBeamIntegration3d();
            //
            //     case BEAM_INTEGRATION_TAG_HingeRadauTwo3d:
            //       return new HingeRadauTwoBeamIntegration3d()
            //-----------

        default:
            cerr << "FEM_ObjectBroker::getBeamIntegration - ";
            cerr << " - no BeamIntegration type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}


UniaxialMaterial *
FEM_ObjectBroker::getNewUniaxialMaterial(int classTag)
{
    switch (classTag)
    {
        case MAT_TAG_ElasticMaterial:
            return new ElasticMaterial(); // values set in recvSelf

        case MAT_TAG_ElasticPPMaterial:
            return new ElasticPPMaterial(); // values set in recvSelf

        case MAT_TAG_ParallelMaterial:
            return new ParallelMaterial();

            //  case MAT_TAG_Concrete01:        out by B. Kamrani for debugging purposes
            //       return new Concrete01();       out by B. Kamrani for debugging purposes

        case MAT_TAG_Steel01:
            return new Steel01();

        case MAT_TAG_Hardening:
            return new HardeningMaterial();

            //// Boris Jeremic took out 20Mar2008
            //  //PY springs: RWBoulanger and BJeremic
            //  case MAT_TAG_PySimple1:
            //      return new PySimple1();
            //
            //  case MAT_TAG_PyLiq1:
            //      return new PyLiq1();
            //
            //  case MAT_TAG_TzSimple1:
            //      return new TzSimple1();
            //
            //  case MAT_TAG_TzLiq1:
            //      return new TzLiq1();
            //
            //  case MAT_TAG_QzSimple1:
            //      return new QzSimple1();


        case MAT_TAG_Hysteretic:
            return new HystereticMaterial();

        case MAT_TAG_EPPGap:
            return new EPPGapMaterial();

        case MAT_TAG_Viscous:
            return new ViscousMaterial();

        case MAT_TAG_PathIndependent:
            return new PathIndependentMaterial();

        case MAT_TAG_SeriesMaterial:
            return new SeriesMaterial();

        case MAT_TAG_CableMaterial:
            return new CableMaterial();

        case MAT_TAG_ENTMaterial:
            return new ENTMaterial();

            //  case MAT_TAG_FedeasBond1:
            //      return new FedeasBond1Material();
            //
            //  case MAT_TAG_FedeasBond2:
            //      return new FedeasBond2Material();
            //
            //  case MAT_TAG_FedeasConcrete1:
            //      return new FedeasConcr1Material();
            //
            //  case MAT_TAG_FedeasConcrete2:
            //      return new FedeasConcr2Material();
            //
            //  case MAT_TAG_FedeasConcrete3:
            //      return new FedeasConcr3Material();
            //
            //  case MAT_TAG_FedeasHardening:
            //      return new FedeasHardeningMaterial();
            //
            //  case MAT_TAG_FedeasHysteretic1:
            //      return new FedeasHyster1Material();
            //
            //  case MAT_TAG_FedeasHysteretic2:
            //      return new FedeasHyster2Material();
            //
            //  case MAT_TAG_FedeasSteel1:
            //      return new FedeasSteel1Material();
            //
            //  case MAT_TAG_FedeasSteel2:
            //      return new FedeasSteel2Material();

        //// Boris Jeremic took out 21Mar2008
        //  case MAT_TAG_DrainBilinear:
        //      return new DrainBilinearMaterial();
        //
        //  case MAT_TAG_DrainClough1:
        //      return new DrainClough1Material();
        //
        //  case MAT_TAG_DrainClough2:
        //      return new DrainClough2Material();
        //
        //  case MAT_TAG_DrainPinch1:
        //      return new DrainPinch1Material();
        //
        case MAT_TAG_MinMax:
            return new MinMaxMaterial();

        default:
            cerr << "FEM_ObjectBroker::getNewUniaxialMaterial - ";
            cerr << " - no UniaxialMaterial type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}

SectionForceDeformation *
FEM_ObjectBroker::getNewSection(int classTag)
{
    switch (classTag)
    {

        case SEC_TAG_Elastic3d:
            return new ElasticSection3d();

        case SEC_TAG_Generic1d:
            return new GenericSection1d();

            //case SEC_TAG_GenericNd:
            //return new GenericSectionNd();

        case SEC_TAG_Aggregator:
            return new SectionAggregator();

            //case SEC_TAG_Fiber:
            //return new FiberSection();


        case SEC_TAG_FiberSection3d:
            return new FiberSection3d();

        case SEC_TAG_ElasticPlateSection:
            return new ElasticPlateSection();

        case SEC_TAG_ElasticMembranePlateSection:
            return new ElasticMembranePlateSection();

        case SEC_TAG_MembranePlateFiberSection:
            return new MembranePlateFiberSection();

        case SEC_TAG_Bidirectional:
            return new Bidirectional();

        default:
            cerr << "FEM_ObjectBroker::getNewSection - ";
            cerr << " - no section type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}

NDMaterial *
FEM_ObjectBroker::getNewNDMaterial(int classTag)
{
    switch (classTag)
    {


            //   case ND_TAG_ElasticIsotropicAxiSymm:
            //     return new ElasticIsotropicAxiSymm();

            //  case ND_TAG_ElasticIsotropicPlateFiber:
            //    return new ElasticIsotropicPlateFiber();

        case ND_TAG_ElasticIsotropic3D:
            return new ElasticIsotropic3D();


            //   case ND_TAG_PlaneStressMaterial:
            //     return new PlaneStressMaterial();

            //  case ND_TAG_PlateFiberMaterial:
            //    return new PlateFiberMaterial();

            // Boris Jeremic took out 20mar2008
            //  case ND_TAG_FluidSolidPorousMaterial:
            //    return new FluidSolidPorousMaterial();
            //
            //  case ND_TAG_PressureDependMultiYield:
            //    return new PressureDependMultiYield();
            //
            //  case ND_TAG_PressureIndependMultiYield:
            //    return new PressureIndependMultiYield();

            //Guanzhou case ND_TAG_FeapMaterial03:
            //Guanzhou   return new FeapMaterial03();

        //Guanzhou added
        case ND_TAG_Template3Dep:
            return new Template3Dep();

        //added by Babak November 5, 2012
        case ND_TAG_vonMisesPP:
            return new vonMisesPP();

        //Guanzhou added
        case ND_TAG_NewTemplate3Dep:
            return new NewTemplate3Dep();
            break;

            //   case ND_TAG_PressureDependentElastic3D:
            //return new PressureDependentElastic3D();
            //      break;

        default:
            cerr << "FEM_ObjectBroker::getNewNDMaterial - ";
            cerr << " - no NDMaterial type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}

//Guanzhou added for NewTemplate3Dep
MaterialParameter *
FEM_ObjectBroker::getNewMaterialParameterPtr(void)
{
    return new MaterialParameter();
}


//Guanzhou added for NewTemplate3Dep
ElasticState *
FEM_ObjectBroker::getNewElasticStatePtr(int classTag)
{
    switch (classTag)
    {
        case ES_TAG_DM04:
            return new DM04_Elastic();
            break;

        case ES_TAG_elnpElastic:
            return new elnp_Elastic();
            break;

        case ES_TAG_PressureDependentElastic:
            return new PressureDependent_Elastic();
            break;

        case ES_TAG_IsotropicElastic:
            return new Isotropic_Elastic();
            break;

        default:
            cerr << "FEM_ObjectBroker::getNewElasticStatePtr - ";
            cerr << " - no Elastic State type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}



//Guanzhou added for NewTemplate3Dep
YieldFunction *
FEM_ObjectBroker::getNewYieldFunctionPtr(int classTag)
{
    switch (classTag)
    {
        case YF_TAG_VM:
            return new VM_YF();
            break;

        case YF_TAG_CC:
            return new CC_YF();
            break;

        case YF_TAG_DP:
            return new DP_YF();
            break;

        case YF_TAG_DM04:
            return new DM04_YF();
            break;

        default:
            cerr << "FEM_ObjectBroker::getNewYieldFunctionPtr - ";
            cerr << " - no Yield Function type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}



//Guanzhou added for NewTemplate3Dep
PlasticFlow *
FEM_ObjectBroker::getNewPlasticFlowPtr(int classTag)
{
    switch (classTag)
    {
        case PF_TAG_VM:
            return new VM_PF();
            break;

        case PF_TAG_CC:
            return new CC_PF();
            break;

        case PF_TAG_DP:
            return new DP_PF();
            break;

        case PF_TAG_DM04:
            return new DM04_PF();
            break;

        default:
            cerr << "FEM_ObjectBroker::getNewPlasticFlowPtr - ";
            cerr << " - no Plastic Flow type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}



//Guanzhou added for NewTemplate3Dep
ScalarEvolution *
FEM_ObjectBroker::getNewScalarEvolutionPtr(int classTag)
{
    switch (classTag)
    {
        case SE_TAG_Linear_Eeq:
            return new Linear_Eeq();
            break;

        case SE_TAG_CC:
            return new CC_Ev();
            break;

        default:
            cerr << "FEM_ObjectBroker::getNewScalarEvolutionPtr - ";
            cerr << " - no Scalar Evolution type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}



//Guanzhou added for NewTemplate3Dep
TensorEvolution *
FEM_ObjectBroker::getNewTensorEvolutionPtr(int classTag)
{
    switch (classTag)
    {
        case TE_TAG_Linear_Eij:
            return new Linear_Eij();
            break;

        case TE_TAG_AF:
            return new AF_Eij();
            break;

        case TE_TAG_DM04_alpha_Eij:
            return new DM04_alpha_Eij();
            break;

        case TE_TAG_DM04_z_Eij:
            return new DM04_z_Eij();
            break;

        default:
            cerr << "FEM_ObjectBroker::getNewTensorEvolutionPtr - ";
            cerr << " - no Tensor Evolution type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}



Fiber *
FEM_ObjectBroker::getNewFiber(int classTag)
{
    switch (classTag)
    {

        case FIBER_TAG_Uniaxial3d:
            return new UniaxialFiber3d();

        default:
            cerr << "FEM_ObjectBroker::getNewFiber - ";
            cerr << " - no Fiber type exists for class tag ";
            cerr << classTag << endln;
            return 0;
    }
}

ConvergenceTest *
FEM_ObjectBroker::getNewConvergenceTest(int classTag)
{
    switch (classTag)
    {
        case CONVERGENCE_TEST_CTestNormUnbalance:
            return new CTestNormUnbalance();

        case CONVERGENCE_TEST_CTestNormDispIncr:
            return new CTestNormDispIncr();

        case CONVERGENCE_TEST_CTestEnergyIncr:
            return new CTestEnergyIncr();


        default:
            cerr << "FEM_ObjectBroker::getNewConvergenceTest - ";
            cerr << " - no ConvergenceTest type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


LoadPattern *
FEM_ObjectBroker::getNewLoadPattern(int classTag)
{
    switch (classTag)
    {
        case PATTERN_TAG_LoadPattern:
            return new LoadPattern();

        //Guanzhou added
        case PATTERN_TAG_PBowlLoading:
            return new PBowlLoading();

        case PATTERN_TAG_UniformExcitation:
            return new UniformExcitation();

        case PATTERN_TAG_MultiSupportPattern:
            return new MultiSupportPattern(0);


        //##################################################################################
        //Nima Tafazzoli added
        case  PATTERN_TAG_Linear_Force_TimeHistory:
        {
#ifdef _BABAK_DEBUG
            //Added by Babak Kamrani :
            int numProcesses, processID;
            MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
            MPI_Comm_rank(MPI_COMM_WORLD, &processID);
            cerr << "BABAK@FEM_ObjectBroker::getNewLoadPattern--PID # " << processID << "classTag is: " << classTag << "\n";
#endif
            return new Linear_Force_TimeHistory();
        }

        //--------------------
        //Babak Kamrani added to use PATTERN_TAG_Domain_Reduction_Method_Modified_Input
        //--------------------
        case PATTERN_TAG_Domain_Reduction_Method_Modified_Input:
        {
#ifdef _BABAK_DEBUG
            //Added by Babak Kamrani :
            int numProcesses, processID;
            MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
            MPI_Comm_rank(MPI_COMM_WORLD, &processID);
            cerr << "BABAK@FEM_ObjectBroker::getNewLoadPattern--PID # " << processID << "classTag is: " << classTag << "\n";
#endif
            return new Domain_Reduction_Method_Modified_Input();
        }

        case  PATTERN_TAG_PathSeries_Force_TimeHistory:
            return new PathSeries_Force_TimeHistory();

        case  PATTERN_TAG_PathTimeSeries_Force_TimeHistory:
            return new PathTimeSeries_Force_TimeHistory();

        case  PATTERN_TAG_Linear_Disp_TimeHistory_Penalty:
            return new Linear_Disp_TimeHistory_Penalty();

            //##################################################################################



        default:
            cerr << "FEM_ObjectBroker::getPtrLoadPattern - ";
            cerr << " - no Load type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


GroundMotion *
FEM_ObjectBroker::getNewGroundMotion(int classTag)
{
    switch (classTag)
    {

        case GROUND_MOTION_TAG_GroundMotion:
            return new GroundMotion(GROUND_MOTION_TAG_GroundMotion);

        default:
            cerr << "FEM_ObjectBroker::getPtrGroundMotion - ";
            cerr << " - no Load type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}

TimeSeries *
FEM_ObjectBroker::getNewTimeSeries(int classTag)
{
    switch (classTag)
    {
        case TSERIES_TAG_LinearSeries:
            return new LinearSeries;

        case TSERIES_TAG_PathTimeSeries:
            return new PathTimeSeries;

        case TSERIES_TAG_PathSeries:
            return new PathSeries;

        case TSERIES_TAG_ConstantSeries:
            return new ConstantSeries;


        default:
            cerr << "FEM_ObjectBroker::getPtrTimeSeries - ";
            cerr << " - no Load type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}

TimeSeriesIntegrator *
FEM_ObjectBroker::getNewTimeSeriesIntegrator(int classTag)
{
    switch (classTag)
    {
        case TIMESERIES_INTEGRATOR_TAG_Trapezoidal:
            return new TrapezoidalTimeSeriesIntegrator();

        default:
            cerr << "FEM_ObjectBroker::getPtrTimeSeriesIntegrator - ";
            cerr << " - no Load type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


Matrix *
FEM_ObjectBroker::getPtrNewMatrix(int classTag, int noRows, int noCols)
{
    switch (classTag)
    {
        case MATRIX_TAG_Matrix:
            return new Matrix(noRows, noCols);


        default:
            cerr << "FEM_ObjectBroker::getPtrNewMatrix - ";
            cerr << " - no NodalLoad type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


Vector *
FEM_ObjectBroker::getPtrNewVector(int classTag, int size)
{
    switch (classTag)
    {
        case VECTOR_TAG_Vector:
            return new Vector(size);


        default:
            cerr << "FEM_ObjectBroker::getPtrNewVector - ";
            cerr << " - no Vector type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


ID *
FEM_ObjectBroker::getPtrNewID(int classTag, int size)
{
    switch (classTag)
    {
        case ID_TAG_ID:
            return new ID(size);


        default:
            cerr << "FEM_ObjectBroker::getPtrNewID - ";
            cerr << " - no ID type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}

/*****************************************
 *
 * METHODS TO GET NEW OUTPUT CLASS OBJECTS
 *
 *****************************************/
/*
DataOutputHandler *
FEM_ObjectBroker::getPtrNewDataOutputHandler(int classTag)
{
    switch (classTag)
    {
        case DATAHANDLER_TAGS_DataOutputStreamHandler:
            return new DataOutputStreamHandler();

        case DATAHANDLER_TAGS_DataOutputFileHandler:
            return new DataOutputFileHandler();

        case DATAHANDLER_TAGS_DataOutputDatabaseHandler:
            return new DataOutputDatabaseHandler();

        default:
            cerr << "FEM_ObjectBroker::getPtrNewDataOutputHandler - ";
            cerr << " - no DataOutputHandler type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}
*/
// Recorder *
// FEM_ObjectBroker::getPtrNewRecorder(int classTag)
// {
//     switch (classTag)
//     {
//         case RECORDER_TAGS_ElementRecorder:
//             return new ElementRecorder();

//         case RECORDER_TAGS_NodeRecorder:
//             return new NodeRecorder();

//         case RECORDER_TAGS_EnvelopeNodeRecorder:
//             return new EnvelopeNodeRecorder();

//         case RECORDER_TAGS_EnvelopeElementRecorder:
//             return new EnvelopeElementRecorder();

//         default:
//             cerr << "FEM_ObjectBroker::getNewConstraintHandler - ";
//             cerr << " - no ConstraintHandler type exists for class tag ";
//             cerr << classTag << endln;
//             return 0;

//     }
// }



/*****************************************
 *
 * METHODS TO GET NEW ANALYSIS CLASSES
 *
 *****************************************/

ConstraintHandler *
FEM_ObjectBroker::getNewConstraintHandler(int classTag)
{
    switch (classTag)
    {
        case HANDLER_TAG_PlainHandler:
            return new PlainHandler();

        case HANDLER_TAG_PenaltyConstraintHandler:
            return new PenaltyConstraintHandler(1.0e12, 1.0e12);

        case HANDLER_TAG_LagrangeConstraintHandler:
            return new LagrangeConstraintHandler(1.0, 1.0);

        case HANDLER_TAG_TransformationConstraintHandler:
            return new TransformationConstraintHandler();

        default:
            cerr << "FEM_ObjectBroker::getNewConstraintHandler - ";
            cerr << " - no ConstraintHandler type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


DOF_Numberer *
FEM_ObjectBroker::getNewNumberer(int classTag)
{
    switch (classTag)
    {
        case NUMBERER_TAG_DOF_Numberer:
            return new DOF_Numberer();


        case NUMBERER_TAG_PlainNumberer:
            return new PlainNumberer();


#ifdef _PARALLEL_PROCESSING

        case NUMBERER_TAG_ParallelNumberer:
            return new ParallelNumberer();
#endif

        default:
            cerr << "FEM_ObjectBroker::getNewConstraintHandler - ";
            cerr << " - no ConstraintHandler type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


AnalysisModel *
FEM_ObjectBroker::getNewAnalysisModel(int classTag)
{
    switch (classTag)
    {
        case AnaMODEL_TAGS_AnalysisModel:
            return new AnalysisModel();


        default:
            cerr << "FEM_ObjectBroker::getNewAnalysisModel - ";
            cerr << " - no AnalysisModel type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


EquiSolnAlgo *
FEM_ObjectBroker::getNewEquiSolnAlgo(int classTag)
{
    switch (classTag)
    {
        case EquiALGORITHM_TAGS_Linear:
            return new Linear();


        case EquiALGORITHM_TAGS_NewtonRaphson:
            return new NewtonRaphson();


        case EquiALGORITHM_TAGS_ModifiedNewton:
            return new ModifiedNewton();


        default:
            cerr << "FEM_ObjectBroker::getNewEquiSolnAlgo - ";
            cerr << " - no EquiSolnAlgo type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


DomainDecompAlgo *
FEM_ObjectBroker::getNewDomainDecompAlgo(int classTag)
{
    switch (classTag)
    {
        case DomDecompALGORITHM_TAGS_DomainDecompAlgo:
            return new DomainDecompAlgo();

        default:
            cerr << "FEM_ObjectBroker::getNewDomainDecompAlgo - ";
            cerr << " - no DomainDecompAlgo type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


StaticIntegrator *
FEM_ObjectBroker::getNewStaticIntegrator(int classTag)
{
    switch (classTag)
    {
        case INTEGRATOR_TAGS_LoadControl:
            return new LoadControl(1.0, 1, 1.0, .10); // must recvSelf

#ifdef _PARALLEL_PROCESSING

        case INTEGRATOR_TAGS_DistributedDisplacementControl:
            return new DistributedDisplacementControl(); // must recvSelf
#endif

        case INTEGRATOR_TAGS_ArcLength:
            return new ArcLength(1.0);      // must recvSelf


        default:
            cerr << "FEM_ObjectBroker::getNewStaticIntegrator - ";
            cerr << " - no StaticIntegrator type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


TransientIntegrator *
FEM_ObjectBroker::getNewTransientIntegrator(int classTag)
{
    switch (classTag)
    {
        case INTEGRATOR_TAGS_Newmark:
            return new Newmark();


        default:
            cerr << "FEM_ObjectBroker::getNewTransientIntegrator - ";
            cerr << " - no TransientIntegrator type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


IncrementalIntegrator *
FEM_ObjectBroker::getNewIncrementalIntegrator(int classTag)
{
    switch (classTag)
    {
        case INTEGRATOR_TAGS_LoadControl:
            return new LoadControl(1.0, 1, 1.0, 1.0); // must recvSelf


        case INTEGRATOR_TAGS_ArcLength:
            return new ArcLength(1.0);      // must recvSelf


        case INTEGRATOR_TAGS_Newmark:
            return new Newmark();

#ifdef _PARALLEL_PROCESSING

        case INTEGRATOR_TAGS_DistributedDisplacementControl:
            return new DistributedDisplacementControl(); // must recvSelf
#endif

        default:
            cerr << "FEM_ObjectBroker::getNewIncrementalIntegrator - ";
            cerr << " - no IncrementalIntegrator type exists for class tag ";
            cerr << classTag << endln;
            return 0;

    }
}


LinearSOESolver *
FEM_ObjectBroker::getNewLinearSolver(void)
{
    return lastLinearSolver;
}

LinearSOE *
FEM_ObjectBroker::getNewLinearSOE(int classTagSOE,
                                  int classTagSolver)
{
    LinearSOE *theSOE = 0;
    //    SlowLinearSOESolver *theSlowSolver =0;
    //Guanzhou    FullGenLinSolver *theGenSolver =0;
    //Guanzhou    BandGenLinSolver *theGenBandSolver =0;
    //Guanzhou    BandSPDLinSolver *theBandSPDSolver =0;
    //Guanzhou    ProfileSPDLinSolver *theProfileSPDSolver =0;
    //Guanzhou    SuperLU *theSparseGenLinSolver =0;
    //Guanzhou    UmfpackGenLinSolver *theUmfpackGenSolver = 0; //Guanzhou added for PDD

#ifdef _PETSC
    PetscSolver *thePetscSolver = 0;
#endif


    /*
      case LinSOE_TAGS_SlowLinearSOE:
    if (classTagSolver == SOLVER_TAGS_SlowLinearSOESolver) {
        theSlowSolver = new SlowLinearSOESolver();
        theSOE = new SlowLinearSOE(*theSlowSolver);
        lastLinearSolver = theSlowSolver;
        return theSOE;
    } else {
        cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
        cerr << " - no SlowLinearSOESolver type exists for class tag ";
        cerr << classTagSolver << endln;
        return 0;
    }

    */


    switch (classTagSOE)
    {
            //Guanzhou      case LinSOE_TAGS_FullGenLinSOE:
            //Guanzhou
            //Guanzhou  if (classTagSolver == SOLVER_TAGS_FullGenLinLapackSolver) {
            //Guanzhou      theGenSolver = new FullGenLinLapackSolver();
            //Guanzhou      theSOE = new FullGenLinSOE(*theGenSolver);
            //Guanzhou      lastLinearSolver = theGenSolver;
            //Guanzhou      return theSOE;
            //Guanzhou  } else {
            //Guanzhou      cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou      cerr << " - no FullGenLinSOESolver type exists for class tag ";
            //Guanzhou      cerr << classTagSolver << endln;
            //Guanzhou      return 0;
            //Guanzhou  }
            //Guanzhou
            //Guanzhou
            //Guanzhou      case LinSOE_TAGS_BandGenLinSOE:

            //Guanzhou if (classTagSolver == SOLVER_TAGS_BandGenLinLapackSolver) {
            //Guanzhou     theGenBandSolver = new BandGenLinLapackSolver();
            //Guanzhou     theSOE = new BandGenLinSOE(*theGenBandSolver);
            //Guanzhou     lastLinearSolver = theGenBandSolver;
            //Guanzhou     return theSOE;
            //Guanzhou } else {
            //Guanzhou     cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou     cerr << " - no BandGenLinSolver type exists for class tag ";
            //Guanzhou     cerr << classTagSolver << endln;
            //Guanzhou     return 0;
            //Guanzhou }

            //Guanzhou  case LinSOE_TAGS_BandSPDLinSOE:

            //Guanzhou if (classTagSolver == SOLVER_TAGS_BandSPDLinLapackSolver) {
            //Guanzhou     theBandSPDSolver = new BandSPDLinLapackSolver();
            //Guanzhou     theSOE = new BandSPDLinSOE(*theBandSPDSolver);
            //Guanzhou     lastLinearSolver = theBandSPDSolver;
            //Guanzhou     return theSOE;
            //Guanzhou } else {
            //Guanzhou     cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou     cerr << " - no BandSPDLinSOESolver type exists for class tag ";
            //Guanzhou     cerr << classTagSolver << endln;
            //Guanzhou     return 0;
            //Guanzhou }

            //Guanzhou  case LinSOE_TAGS_ProfileSPDLinSOE:
            //Guanzhou
            //Guanzhou    if (classTagSolver == SOLVER_TAGS_ProfileSPDLinDirectSolver) {
            //Guanzhou        theProfileSPDSolver = new ProfileSPDLinDirectSolver();
            //Guanzhou        theSOE = new ProfileSPDLinSOE(*theProfileSPDSolver);
            //Guanzhou        lastLinearSolver = theProfileSPDSolver;
            //Guanzhou        return theSOE;
            //Guanzhou    } else if (classTagSolver == SOLVER_TAGS_ProfileSPDLinSubstrSolver) {
            //Guanzhou        theProfileSPDSolver = new ProfileSPDLinSubstrSolver();
            //Guanzhou        theSOE = new ProfileSPDLinSOE(*theProfileSPDSolver);
            //Guanzhou        lastLinearSolver = theProfileSPDSolver;
            //Guanzhou        return 0;
            //Guanzhou    }
            //Guanzhou    else {
            //Guanzhou        cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou        cerr << " - no ProfileSPD_LinSolver type exists for class tag ";
            //Guanzhou        cerr << classTagSolver << endln;
            //Guanzhou        return 0;
            //Guanzhou    }
            //Guanzhou
            //Guanzhou  case LinSOE_TAGS_SparseGenColLinSOE:
            //Guanzhou
            //Guanzhou    if (classTagSolver == SOLVER_TAGS_SuperLU) {
            //Guanzhou        theSparseGenLinSolver = new SuperLU();
            //Guanzhou        theSOE = new SparseGenColLinSOE(*theSparseGenLinSolver);
            //Guanzhou        lastLinearSolver = theSparseGenLinSolver;
            //Guanzhou        return theSOE;
            //Guanzhou    } else {
            //Guanzhou        cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou        cerr << " - no SparseGenLinSolverSolver type exists for class tag ";
            //Guanzhou        cerr << classTagSolver << endln;
            //Guanzhou        return 0;
            //Guanzhou    }

            //Guanzhou added for PDD
            //Guanzhou case LinSOE_TAGS_UmfpackGenLinSOE:
            //Guanzhou
            //Guanzhou   if (classTagSolver == SOLVER_TAGS_UmfpackGenLinSolver) {
            //Guanzhou       theUmfpackGenSolver = new UmfpackGenLinSolver();
            //Guanzhou       theSOE = new UmfpackGenLinSOE(*theUmfpackGenSolver);
            //Guanzhou       lastLinearSolver = theUmfpackGenSolver;
            //Guanzhou       return theSOE;
            //Guanzhou   } else {
            //Guanzhou       cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou       cerr << " - no UmfpackGenSolver type exists for class tag ";
            //Guanzhou       cerr << classTagSolver << endln;
            //Guanzhou       return 0;
            //Guanzhou   }

#ifdef _PETSC
        case LinSOE_TAGS_PetscSOE:

            if (classTagSolver == SOLVER_TAGS_PetscSolver)
            {
                thePetscSolver = new PetscSolver();
                theSOE = new PetscSOE(*thePetscSolver);
                return theSOE;
            }
            else
            {
                cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
                cerr << " - no PetscSolver type exists for class tag ";
                cerr << classTagSolver << endln;
                return 0;
            }

#endif

            //Guanzhou#ifdef _PARALLEL_PROCESSING
            //Guanzhou      case LinSOE_TAGS_DistributedBandGenLinSOE:
            //Guanzhou
            //Guanzhou    if (classTagSolver == SOLVER_TAGS_BandGenLinLapackSolver) {
            //Guanzhou        theGenBandSolver = new BandGenLinLapackSolver();
            //Guanzhou        theSOE = new DistributedBandGenLinSOE(*theGenBandSolver);
            //Guanzhou        lastLinearSolver = theGenBandSolver;
            //Guanzhou        return theSOE;
            //Guanzhou    } else {
            //Guanzhou        cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou        cerr << " - no DistributedBandGenLinSolver type exists for class tag ";
            //Guanzhou        cerr << classTagSolver << endln;
            //Guanzhou        return 0;
            //Guanzhou    }
            //Guanzhou
            //Guanzhou        case LinSOE_TAGS_DistributedBandSPDLinSOE:
            //Guanzhou
            //Guanzhou    //Guanzhou if (classTagSolver == SOLVER_TAGS_BandSPDLinLapackSolver) {
            //Guanzhou    //Guanzhou     theBandSPDSolver = new BandSPDLinLapackSolver();
            //Guanzhou    //Guanzhou     theSOE = new DistributedBandSPDLinSOE(*theBandSPDSolver);
            //Guanzhou    //Guanzhou     lastLinearSolver = theBandSPDSolver;
            //Guanzhou    //Guanzhou     return theSOE;
            //Guanzhou    //Guanzhou } else {
            //Guanzhou    //Guanzhou     cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou    //Guanzhou     cerr << " - no DistributedBandSPDLinSolver type exists for class tag ";
            //Guanzhou    //Guanzhou     cerr << classTagSolver << endln;
            //Guanzhou    //Guanzhou     return 0;
            //Guanzhou    //Guanzhou }
            //Guanzhou
            //Guanzhou
            //Guanzhou  case LinSOE_TAGS_DistributedProfileSPDLinSOE:
            //Guanzhou
            //Guanzhou    if (classTagSolver == SOLVER_TAGS_ProfileSPDLinDirectSolver) {
            //Guanzhou        theProfileSPDSolver = new ProfileSPDLinDirectSolver();
            //Guanzhou        theSOE = new DistributedProfileSPDLinSOE(*theProfileSPDSolver);
            //Guanzhou        lastLinearSolver = theProfileSPDSolver;
            //Guanzhou        return theSOE;
            //Guanzhou    } else if (classTagSolver == SOLVER_TAGS_ProfileSPDLinSubstrSolver) {
            //Guanzhou        theProfileSPDSolver = new ProfileSPDLinSubstrSolver();
            //Guanzhou        theSOE = new DistributedProfileSPDLinSOE(*theProfileSPDSolver);
            //Guanzhou        lastLinearSolver = theProfileSPDSolver;
            //Guanzhou        return 0;
            //Guanzhou    }
            //Guanzhou    else {
            //Guanzhou        cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou        cerr << " - no ProfileSPD_LinSolver type exists for class tag ";
            //Guanzhou        cerr << classTagSolver << endln;
            //Guanzhou        return 0;
            //Guanzhou    }


            /*case LinSOE_TAGS_DistributedSparseGenColLinSOE:
              if (classTagSolver == SOLVER_TAGS_SuperLU) {
                  theSparseGenLinSolver = new SuperLU();
                  theSOE = new DistributedSparseGenColLinSOE(*theSparseGenLinSolver);
                  lastLinearSolver = theSparseGenLinSolver;
                  return theSOE;
              } else if (classTagSolver == SOLVER_TAGS_DistributedSuperLU) {
                  theDistributedSparseGenLinSolver = new DistributedSuperLU();
                  theSOE = new DistributedSparseGenColLinSOE(*theDistributedSparseGenLinSolver);
                  lastLinearSolver = theSparseGenLinSolver;
                  return theSOE;
              } else {
                  cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
                  cerr << " - no DistributedSparseGenLinSolverSolver type exists for class tag ";
                  cerr << classTagSolver << endln;
                  return 0;
              }*/
            //Guanzhou
            //Guanzhou#else
            //Guanzhou  case LinSOE_TAGS_SparseGenColLinSOE:
            //Guanzhou
            //Guanzhou    if (classTagSolver == SOLVER_TAGS_SuperLU) {
            //Guanzhou        theSparseGenLinSolver = new SuperLU();
            //Guanzhou        theSOE = new SparseGenColLinSOE(*theSparseGenLinSolver);
            //Guanzhou        lastLinearSolver = theSparseGenLinSolver;
            //Guanzhou        return theSOE;
            //Guanzhou    } else {
            //Guanzhou        cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou        cerr << " - no SparseGenLinSolverSolver type exists for class tag ";
            //Guanzhou        cerr << classTagSolver << endln;
            //Guanzhou        return 0;
            //Guanzhou    }
            //Guanzhou
            //Guanzhou#endif

        default:
            cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            cerr << " - no LinearSOE type exists for class tag ";
            cerr << classTagSOE << endln;
            return 0;


    }
}




DomainSolver *
FEM_ObjectBroker::getNewDomainSolver(void)
{
    return lastDomainSolver;
}

LinearSOE *
FEM_ObjectBroker::getPtrNewDDLinearSOE(int classTagSOE,
                                       int classTagDDSolver)
{
    //Guanzhou    ProfileSPDLinSubstrSolver *theProfileSPDSolver =0;

    switch (classTagSOE)
    {
            //Guanzhou      case LinSOE_TAGS_ProfileSPDLinSOE:
            //Guanzhou
            //Guanzhou  if (classTagDDSolver == SOLVER_TAGS_ProfileSPDLinSubstrSolver) {
            //Guanzhou      theProfileSPDSolver = new ProfileSPDLinSubstrSolver();
            //Guanzhou      LinearSOE *theSOE = new ProfileSPDLinSOE(*theProfileSPDSolver);
            //Guanzhou      lastDomainSolver = theProfileSPDSolver;
            //Guanzhou      return theSOE;
            //Guanzhou  }
            //Guanzhou  else {
            //Guanzhou      cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            //Guanzhou      cerr << " - no ProfileSPD Domain Solver type exists for class tag ";
            //Guanzhou      cerr << classTagDDSolver << endln;
            //Guanzhou      return 0;
            //Guanzhou  }
            //Guanzhou

        default:
            cerr << "FEM_ObjectBroker::getNewLinearSOE - ";
            cerr << " - no LinearSOE type exists for class tag ";
            cerr << classTagSOE << endln;
            return 0;

    }
}


DomainDecompositionAnalysis *
FEM_ObjectBroker::getNewDomainDecompAnalysis(int classTag,
        Subdomain &theSubdomain)
{
    switch (classTag)
    {
        case DomDecompANALYSIS_TAGS_DomainDecompositionAnalysis:
            return new DomainDecompositionAnalysis(theSubdomain);

#ifdef _PARALLEL_PROCESSING

        case ANALYSIS_TAGS_StaticDomainDecompositionAnalysis:
            return new StaticDomainDecompositionAnalysis(theSubdomain);

        case ANALYSIS_TAGS_TransientDomainDecompositionAnalysis:
            return new TransientDomainDecompositionAnalysis(theSubdomain);
#endif

        default:
            cerr << "ObjectBroker::getNewDomainDecompAnalysis ";
            cerr << " - no DomainDecompAnalysis type exists for class tag " ;
            cerr << classTag << endln;
            return 0;

    }
}


Subdomain *
FEM_ObjectBroker::getSubdomainPtr(int classTag)
{
    cerr << "FEM_ObjectBroker: NOT IMPLEMENTED YET";
    return 0;
}


//Guanzhou added below for Template3D material
YieldSurface *
FEM_ObjectBroker::getYieldSurfacePtr(int clsTag)
{
    switch (clsTag)
    {

        case YS_TAG_CAMYieldSurface:
            return new CAMYieldSurface(1.2);

            //      case YS_TAG_DPYieldSurface01:
            //              return new DPYieldSurface01(1.0);

        case YS_TAG_DPYieldSurface:
            return new DPYieldSurface();

        case YS_TAG_MDYieldSurface:
            return new MDYieldSurface();

        case YS_TAG_RMC01YieldSurface:
            return new RMC01YieldSurface();

            //      case YS_TAG_TriFCYieldSurface:
            //              return new TriFCYieldSurface(1.0,1.0,1.0,1.0);

        case YS_TAG_VMYieldSurface:
            return new VMYieldSurface();

        default:
            cerr << "ObjectBroker::getYieldSurfacePtr ";
            cerr << " - no YieldSurface type exists for class tag " ;
            cerr << clsTag << endln;
            return  0;

    }
}

PotentialSurface *
FEM_ObjectBroker::getPotentialSurfacePtr(int clsTag)
{
    switch (clsTag)
    {

        case PS_TAG_CAMPotentialSurface:
            return new CAMPotentialSurface(1.2);

        case PS_TAG_DPPotentialSurface:
            return new DPPotentialSurface(0.2);

            //    case PS_TAG_MDPotentialSurface01:
            //              return new MDPotentialSurface01(1.0);

        case PS_TAG_MDPotentialSurface:
            return new MDPotentialSurface();

        case PS_TAG_RMC01PotentialSurface:
            return new RMC01PotentialSurface();

        case PS_TAG_VMPotentialSurface:
            return new VMPotentialSurface();

        default:
            cerr << "ObjectBroker::getPotentialSurfacePtr ";
            cerr << " - no PotentialSurface type exists for class tag " ;
            cerr << clsTag << endln;
            return  0;

    }
}

EvolutionLaw_S *
FEM_ObjectBroker::getEL_S(int clsTag)
{
    switch (clsTag)
    {

        case ELS_TAG_L_Eeq:
            return new EvolutionLaw_L_Eeq(0.0);

        case ELS_TAG_NL_Eeq:
            return new EvolutionLaw_NL_Eeq();


        case ELS_TAG_NL_Ep:
            return new EvolutionLaw_NL_Ep(0.65, 0.19, 0.06);


        default:
            cerr << "ObjectBroker::getEL_S ";
            cerr << " - no EvolutionType type exists for class tag " ;
            cerr << clsTag << endln;
            return  0;

    }


}


EvolutionLaw_T *
FEM_ObjectBroker::getEL_T(int clsTag)
{

    switch (clsTag)
    {

        case ELT_TAG_L_Eij:
            return new EvolutionLaw_L_Eij(0.0);

        case ELT_TAG_NL_Eij:
            return new EvolutionLaw_NL_Eij(0.0, 0.0);


            //case ELT_TAG_NL_EijMD:
            //          return new EvolutionLaw_NL_EijMD();


        default:
            cerr << "ObjectBroker::getEL_T ";
            cerr << " - no EvolutionType type exists for class tag " ;
            cerr << clsTag << endln;
            return  0;

    }

}

