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


//Materials (find ../../material -name "*.h" yields this list)
#include <UniaxialMaterial.h>
#include <Concrete04.h>
#include <EPPGapMaterial.h>
#include <Steel02.h>
#include <SeriesMaterial.h>
#include <HardeningMaterial.h>
#include <ElasticPPMaterial.h>
#include <HystereticMaterial.h>
#include <Concrete02.h>
// [J.Abell] These don't compile.  #include <Pinching4Material.h>
#include <Steel03.h>
#include <MinMaxMaterial.h>
#include <ENTMaterial.h>
#include <FatigueMaterial.h>
#include <ElasticMaterial.h>
// [J.Abell] These don't compile. #include <ThreePointCurve.h>
// [J.Abell] These don't compile. #include <AxialCurve.h>
// [J.Abell] These don't compile.  #include <LimitCurve.h>
// [J.Abell] These don't compile.  #include <ShearCurve.h>
// [J.Abell] These don't compile.  #include <LimitStateMaterial.h>
#include <DrainMaterial.h>
#include <Concrete01.h>
#include <ViscousMaterial.h>
#include <Steel01.h>
#include <BarSlipMaterial.h>
#include <PathIndependentMaterial.h>
#include <CableMaterial.h>
#include <ReinforcingSteel.h>
#include <BoucWenMaterial.h>
#include <NewUniaxialMaterial.h>
#include <ParallelMaterial.h>
#include <FiberSection3d.h>
#include <FiberSection.h>
#include <FiberSectionGJ.h>
#include <Isolator2spring.h>
#include <ElasticShearSection3d.h>
#include <GenericSection1d.h>
#include <ElasticMembranePlateSection.h>
#include <ElasticSection3d.h>
#include <ElasticPlateSection.h>
#include <SectionForceDeformation.h>
#include <SectionAggregator.h>
#include <GenericSectionNd.h>
// [J.Abell] These don't compile.  #include <MatrixUtil.h>
#include <MembranePlateFiberSection.h>
// [J.Abell] These don't compile.  #include <YS_Section2D02.h>
// [J.Abell] These don't compile.  #include <YS_Section2D01.h>
#include <Bidirectional.h>
#include <Fiber.h>
#include <UniaxialFiber3d.h>
#include <CircReinfLayer.h>
#include <StraightReinfLayer.h>
#include <ReinfLayer.h>
#include <QuadPatch.h>
#include <CircPatch.h>
#include <Patch.h>
#include <SectionRepres.h>
#include <FiberSectionRepr.h>
#include <Cell.h>
#include <QuadCell.h>
#include <ReinfBar.h>
#include <ElasticIsotropic3D.h>
#include <NDMaterial.h>
#include <PlaneStressMaterial.h>
#include <BeamFiberMaterial.h>
#include <ElasticIsotropicBeamFiber.h>
#include <ElasticIsotropicMaterial.h>
#include <PressureDependentElastic3D.h>
#include <ElasticIsotropicPlateFiber.h>
#include <ElasticCrossAnisotropic.h>
#include <BardetConstraint.h>
#include <ElasticIsotropicAxiSymm.h>
#include <FeapMaterial.h>
#include <PlateFiberMaterial.h>
#include <NullPlasticMaterial.h>
#include <PlasticHardeningMaterial.h>
#include <MultiLinearKp.h>
#include <ExponReducing.h>
#include <PlasticHardening2D.h>
#include <BoundingSurface2D.h>
#include <Kinematic2D01.h>
#include <BkStressLimSurface2D.h>
#include <CombinedIsoKin2D01.h>
#include <PeakOriented2D02.h>
#include <YS_Evolution.h>
#include <Kinematic2D02.h>
#include <YS_Evolution2D.h>
#include <Isotropic2D01.h>
#include <NullEvolution.h>
#include <CombinedIsoKin2D02.h>
#include <PeakOriented2D01.h>
#include <ElTawil2DUnSym.h>
#include <YieldSurface_BC2D.h>
#include <YieldSurface_BC.h>
#include <Orbison2D.h>
#include <NullYS2D.h>

#include "../../../CompGeoMechUCD_MaterialModeling/CompGeoMechUCD_MaterialModeling.h"


//Within MOSS
#include <Element.h>
#include <UniaxialFiber3d.h>
#include <Truss.h>
#include <TrussSection.h>
#include <CorotTruss.h>
#include <CorotTrussSection.h>
#include <ZeroLength.h>
#include <ZeroLengthSection.h>
#include <ElasticBeamLumpedMass.h>
#include <ElasticBeam.h>
#include <BeamWithHinges3d.h>
#include <DispBeamColumn3d.h>
#include <ShellMITC4.h>
#include <ElementSelfWeight.h>
#include <HingeMidpointBeamIntegration3d.h>
#include <HingeRadauBeamIntegration3d.h>
#include <HingeRadauTwoBeamIntegration3d.h>
#include <LobattoBeamIntegration.h>



//Contents of CompGeoMechUCD_FiniteElements (Jose Abell, Fri 31 Jan 2014 02:41:19 PM PST)
#include "../../../CompGeoMechUCD_FiniteElements/CompGeoMechUCD_FiniteElements.h"


//MOSS
#include <Node.h>
#include <PathSeries_Disp_TimeHistory_Penalty.h>





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

        //         case ELE_TAG_PenaltyElementApplyDisplacement:           //Babak added on 6/25/13
        //             return new PenaltyElementApplyDisplacement();

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

#ifdef _PETSC
    PetscSolver *thePetscSolver = 0;
#endif


    switch (classTagSOE)
    {


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
    switch (classTagSOE)
    {

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


YieldSurface *
FEM_ObjectBroker::getYieldSurfacePtr(int clsTag)
{
    switch (clsTag)
    {
        // [J.Abell] These are located in Template3DEP inside the CompGeoMech_Material_Modeling folder
        // case YS_TAG_CAMYieldSurface:
        //     return new CAMYieldSurface(1.2);

        // //      case YS_TAG_DPYieldSurface01:
        // //              return new DPYieldSurface01(1.0);

        // case YS_TAG_DPYieldSurface:
        //     return new DPYieldSurface();

        // case YS_TAG_MDYieldSurface:
        //     return new MDYieldSurface();

        // case YS_TAG_RMC01YieldSurface:
        //     return new RMC01YieldSurface();

        // //      case YS_TAG_TriFCYieldSurface:
        // //              return new TriFCYieldSurface(1.0,1.0,1.0,1.0);

        // case YS_TAG_VMYieldSurface:
        //     return new VMYieldSurface();

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
        // [J.Abell]  These are located in Template3DEP inside the CompGeoMech_Material_Modeling folder

        // case PS_TAG_CAMPotentialSurface:
        //     return new CAMPotentialSurface(1.2);

        // case PS_TAG_DPPotentialSurface:
        //     return new DPPotentialSurface(0.2);

        // //    case PS_TAG_MDPotentialSurface01:
        // //              return new MDPotentialSurface01(1.0);

        // case PS_TAG_MDPotentialSurface:
        //     return new MDPotentialSurface();

        // case PS_TAG_RMC01PotentialSurface:
        //     return new RMC01PotentialSurface();

        // case PS_TAG_VMPotentialSurface:
        //     return new VMPotentialSurface();

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
        // [J.Abell] These are located in Template3DEP inside the CompGeoMech_Material_Modeling folder

        // case ELS_TAG_L_Eeq:
        //     return new EvolutionLaw_L_Eeq(0.0);

        // case ELS_TAG_NL_Eeq:
        //     return new EvolutionLaw_NL_Eeq();


        // case ELS_TAG_NL_Ep:
        //     return new EvolutionLaw_NL_Ep(0.65, 0.19, 0.06);


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
        // [J.Abell] Thes[J.Abell] e are located in Template3DEP inside the CompGeoMech_Material_Modeling folder

        // case ELT_TAG_L_Eij:
        //     return new EvolutionLaw_L_Eij(0.0);

        // case ELT_TAG_NL_Eij:
        //     return new EvolutionLaw_NL_Eij(0.0, 0.0);


        //case ELT_TAG_NL_EijMD:
        //          return new EvolutionLaw_NL_EijMD();


        default:
            cerr << "ObjectBroker::getEL_T ";
            cerr << " - no EvolutionType type exists for class tag " ;
            cerr << clsTag << endln;
            return  0;

    }

}

