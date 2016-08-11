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

// $Revision: 1.61 $
// $Date: 2004/11/24 22:34:27 $
// $Source: /usr/local/cvs/OpenSees/SRC/classTags.h,v $

// Written: fmk
// Revision: A
//
// Purpose: This file contains the declarations of all class tags used.
//
// What: "@(#) classTags.h, revA"

#ifndef classTags_h
#define classTags_h

#define intType    1
#define doubleType 2
#define idType     3
#define vectorType 4
#define matrixType 5

#define ACTOR_TAGS_SUBDOMAIN 1

#define DMG_TAG_HystereticEnergy 1
#define DMG_TAG_ParkAng          2
#define DMG_TAG_Kratzig          3
#define DMG_TAG_Mehanny          4
#define DMG_TAG_NormalizedPeak   5

#define EigenSOE_TAGS_BandArpackSOE     1
#define EigenSOE_TAGS_SymArpackSOE  2
#define EigenSOE_TAGS_SymBandEigenSOE   3
#define EigenSOE_TAGS_FullGenEigenSOE   4

#define EigenSOLVER_TAGS_BandArpackSolver   1
#define EigenSOLVER_TAGS_SymArpackSolver    2
#define EigenSOLVER_TAGS_SymBandEigenSolver     3
#define EigenSOLVER_TAGS_FullGenEigenSolver  4

#define EigenALGORITHM_TAGS_Frequency 1
#define EigenALGORITHM_TAGS_Standard  2

#define EigenINTEGRATOR_TAGS_Eigen 1

#define CONVERGENCE_TEST_CTestNormUnbalance      1
#define CONVERGENCE_TEST_CTestNormDispIncr       2
#define CONVERGENCE_TEST_CTestEnergyIncr         3
#define CONVERGENCE_TEST_CTestRelativeNormUnbalance      4
#define CONVERGENCE_TEST_CTestRelativeNormDispIncr       5
#define CONVERGENCE_TEST_CTestRelativeEnergyIncr         6

#define GRND_TAG_ElCentroGroundMotion                 1
#define GROUND_MOTION_TAG_GroundMotionRecord          2
#define GROUND_MOTION_TAG_InterpolatedGroundMotion    3
#define GROUND_MOTION_TAG_GroundMotion                4

#define REGION_TAG_MeshRegion      1

#define TIMESERIES_INTEGRATOR_TAG_Trapezoidal 1

#define SECT_TAG_Section         1

#define TSERIES_TAG_LinearSeries         1
#define TSERIES_TAG_RectangularSeries          2
#define TSERIES_TAG_PathTimeSeries       3
#define TSERIES_TAG_PathSeries       4
#define TSERIES_TAG_ConstantSeries       5
#define TSERIES_TAG_DiscretizedRandomProcessSeries 7
#define TSERIES_TAG_SimulatedRandomProcessSeries 8


#define MAT_TAG_ElasticMaterial         1
#define MAT_TAG_ElasticPPMaterial       2
#define MAT_TAG_ParallelMaterial        3
#define MAT_TAG_Concrete01              4
#define MAT_TAG_Steel01                 5
#define MAT_TAG_Hardening               6
#define MAT_TAG_Hysteretic              7
#define MAT_TAG_EPPGap                  8
#define MAT_TAG_Viscous                 9
#define MAT_TAG_Backbone                10
#define MAT_TAG_PathIndependent         11
#define MAT_TAG_SeriesMaterial          12
#define MAT_TAG_CableMaterial          13
#define MAT_TAG_ENTMaterial             14
#define MAT_TAG_Penalty                 15
#define MAT_TAG_MinMax                  16
#define MAT_TAG_BoucWen                 17
#define MAT_TAG_Pinching4                   18
#define MAT_TAG_BarSlip                 19
#define MAT_TAG_Fatigue         20
// Terje:
#define MAT_TAG_SmoothSteel01           21
#define MAT_TAG_SmoothConcrete01        22



#define MAT_TAG_FedeasMaterial         1000
#define MAT_TAG_FedeasBond1       1001
#define MAT_TAG_FedeasBond2       1002
#define MAT_TAG_FedeasConcrete1       1003
#define MAT_TAG_FedeasConcrete2       1004
#define MAT_TAG_FedeasConcrete3       1005
#define MAT_TAG_FedeasHardening       1006
#define MAT_TAG_FedeasHysteretic1       1007
#define MAT_TAG_FedeasHysteretic2       1008
#define MAT_TAG_FedeasSteel1       1009
#define MAT_TAG_FedeasSteel2       1010
#define MAT_TAG_Concrete04 1011
#define MAT_TAG_Steel02 1012
#define MAT_TAG_Steel03 1014
#define MAT_TAG_Concrete02 1013
#define MAT_TAG_ReinforcingSteel 1015

#define MAT_TAG_DrainMaterial       2000
#define MAT_TAG_DrainHardening      2001
#define MAT_TAG_DrainBilinear       2002
#define MAT_TAG_DrainClough1        2003
#define MAT_TAG_DrainClough2        2004
#define MAT_TAG_DrainPinch1         2005
#define MAT_TAG_DrainPinch2         2006

#define MAT_TAG_SnapMaterial        3000
#define MAT_TAG_SnapBilinear        3001
#define MAT_TAG_SnapClough      3002
#define MAT_TAG_SnapPinch       3003
#define MAT_TAG_SnapCloughDamage    3004
#define MAT_TAG_SnapPinchingDamage  3005

#define MAT_TAG_Clough1 201
#define MAT_TAG_Clough2 202
#define MAT_TAG_Pinch1  203
#define MAT_TAG_BiLinear    204
#define MAT_TAG_Pinching    205


#define SEC_TAG_Elastic3d   4
#define SEC_TAG_Generic1d   5
#define SEC_TAG_GenericNd   6
#define SEC_TAG_Aggregator  7
#define SEC_TAG_Fiber       8
#define SEC_TAG_FiberSection3d      10
#define SEC_TAG_FiberSectionGJ      11
#define SEC_TAG_BeamFiberSection    12
#define SEC_TAG_ElasticPlateSection 13
#define SEC_TAG_ElasticMembranePlateSection 14
#define SEC_TAG_MembranePlateFiberSection   15
#define SEC_TAG_Bidirectional   16
#define SEC_TAG_Isolator2spring 17

#define ND_TAG_ElasticIsotropic                 10
#define ND_TAG_ElasticIsotropicAxiSymm          13
#define ND_TAG_ElasticIsotropicPlateFiber       14
#define ND_TAG_ElasticIsotropicBeamFiber        15
#define ND_TAG_ElasticIsotropic3D               16
#define ND_TAG_ElasticCrossAnisotropic3D        17
#define ND_TAG_J2PlaneStrain                  3005
#define ND_TAG_J2PlaneStress                  3006
#define ND_TAG_J2AxiSymm                      3007
#define ND_TAG_J2ThreeDimensional             3009
#define ND_TAG_J2PlateFiber                 3010
#define ND_TAG_J2BeamFiber                  3011
#define ND_TAG_PressureDependentElastic3D       22
#define ND_TAG_Template3Dep                     31
#define ND_TAG_NewTemplate3Dep    32
#define ND_TAG_Pisano_model         33
#define ND_TAG_TwoStageEquivalentElasticIsotropic3D 34

//Guanzhou added for parallel processing
#define MP_TAG 3201
#define ES_TAG_DM04 3202
#define ES_TAG_elnpElastic 3203
#define ES_TAG_PressureDependentElastic 3204
#define ES_TAG_IsotropicElastic 3205
#define YF_TAG_VM 3206
#define YF_TAG_CC 3207
#define YF_TAG_DP 3208
#define YF_TAG_DM04 3209
#define PF_TAG_VM 3210
#define PF_TAG_CC 3211
#define PF_TAG_DP 3212
#define PF_TAG_DM04 3213
#define SE_TAG_Linear_Eeq 3214
#define SE_TAG_CC 3215
#define TE_TAG_Linear_Eij 3216
#define TE_TAG_AF 3217
#define TE_TAG_DM04_alpha_Eij 3218
#define TE_TAG_DM04_z_Eij 3219
#define YF_TAG_RMC 3220
#define PF_TAG_RMC 3221
// Mahdi Taiebat and Boris Jeremic, 06May2007
#define ES_TAG_SANISAND 3230
#define YF_TAG_SANISAND 3231
#define PF_TAG_SANISAND 3232
#define SE_TAG_SANISAND_p0_bar 3233
#define TE_TAG_SANISAND_alpha_Eij 3234
#define TE_TAG_SANISAND_z_Eij 3235

// Nima Tafazzoli (January 2010)
#define ND_TAG_vonMisesPP 3236
#define ND_TAG_vonMisesIH 3237
#define ND_TAG_vonMisesKH 3238
#define Constitutive_MP_TAG 3239
#define ND_TAG_DruckerPragerPP 3240
#define ND_TAG_DruckerPragerIH 3241
#define ND_TAG_DruckerPragerKH 3242
#define ND_TAG_CamClay 3243
#define ND_TAG_vonMisesLinearKH 3244

// LTensor Materials
#define ND_TAG_vonMisesPPLT 4001
#define ND_TAG_PisanoLT 4002
#define ND_TAG_NewPisanoLT 4003
#define ND_TAG_ElasticIsotropic3DLT 4004
#define ND_TAG_NonlinearIsotropic3DLT 4005


#define ND_TAG_FluidSolidPorousMaterial        100
#define ND_TAG_PressureDependMultiYield     101
#define ND_TAG_PressureIndependMultiYield       102
#define ND_TAG_PressureDependMultiYield02       103
#define ND_TAG_FeapMaterial                 1000
#define ND_TAG_FeapMaterial01                 1001
#define ND_TAG_FeapMaterial02                 1002
#define ND_TAG_FeapMaterial03                 1003
#define ND_TAG_PlaneStressMaterial          2000
#define ND_TAG_PlateFiberMaterial          2001
#define ND_TAG_BeamFiberMaterial        2002
#define ND_TAG_CompressibleFluid        3001
#define ND_TAG_GeneralizedPlasticity 3002
#define ND_TAG_J2Plasticity02  3003
#define ND_TAG_FiniteDeformationElastic3D   8002
#define ND_TAG_NeoHookeanCompressible3D         8003
#define ND_TAG_FDdecoupledElastic3D         8004
#define ND_TAG_FiniteDeformationEP3D            8005

#define FIBER_TAG_Uniaxial3d        2

//Classic Elastoplastic Materials
#define ND_TAG_CEM_DruckerPragerLinearHardening 5001
#define ND_TAG_CEM_VonMisesLinearHardening 5002
#define ND_TAG_CEM_DruckerPragerVonMisesLinearHardening 5003
#define ND_TAG_CEM_DruckerPragerArmstrongFrederick 5004
#define ND_TAG_CEM_DruckerPragerNonAssociateLinearHardening 5005
#define ND_TAG_CEM_DruckerPragerNonAssociateArmstrongFrederick 5006
#define ND_TAG_CEM_VonMisesArmstrongFrederick 5007




#define PATTERN_TAG_LoadPattern                                 1
#define PATTERN_TAG_MultiSupportPattern                         3
#define PATTERN_TAG_UniformExcitation                           2
#define LOAD_TAG_Beam3dUniformLoad                              5
#define LOAD_TAG_Beam3dPointLoad                                6
#define LOAD_TAG_ElementSelfWeight                              30
#define LOAD_TAG_BrickSurfaceLoad                               8
#define LOAD_TAG_BrickSurfaceLoad_from_Contact_Pore_Pressure    9
#define PATTERN_TAG_PBowlLoading                                26
#define PATTERN_TAG_Domain_Reduction_Method                     11
#define PATTERN_TAG_Domain_Reduction_Method_Modified_Input      12
#define PATTERN_TAG_Domain_Reduction_Method_HDF5_Input          31
#define PATTERN_TAG_Domain_Reduction_Method_Save_Forces         15
#define PATTERN_TAG_Domain_Reduction_Method_Restore_Forces      16
#define Load_TAG_AccelerationField                              13
#define PATTERN_TAG_Linear_Force_TimeHistory                    14
#define PATTERN_TAG_PathSeries_Force_TimeHistory                19
#define PATTERN_TAG_Linear_Disp_TimeHistory_Penalty             21
#define PATTERN_TAG_PathSeries_Disp_TimeHistory_Penalty         25
#define PATTERN_TAG_PathTimeSeries_Force_TimeHistory            27




#define MAT_TAG_IsotropicLinElastic         1001
#define MAT_TAG_IsotropicLinElasticPoint    1002
#define MAT_TAG_OrthotropicLinElastic       1003
#define MAT_TAG_OrthotropicLinElasticPoint  1004

#define ELE_TAG_CST                         4050

#define ELE_TAG_Subdomain                   1
#define ELE_TAG_ElasticBeamLumpedMass       3000
#define ELE_TAG_beam3d01                    3001
#define ELE_TAG_beam3d02                    3002
#define ELE_TAG_Truss                       4001
#define ELE_TAG_TrussSection                4005
#define ELE_TAG_CorotTruss                  4003
#define ELE_TAG_CorotTrussSection           4004
#define ELE_TAG_fElmt05                     5
#define ELE_TAG_fElmt02                     2
#define ELE_TAG_ZeroLength                  5000
#define ELE_TAG_ZeroLengthSection           5001
#define ELE_TAG_ZeroLengthND                5002

// Nima Tafazzoli added
#define ELE_TAG_ContactElement_3DOF_3DOF 5003
#define ELE_TAG_ContactElement_3DOF_6DOF 5004
#define ELE_TAG_ContactElement_6DOF_6DOF 5005
#define ELE_TAG_ContactElement_Nonlinear_3DOF_7DOF 5013
#define ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF 5014

#define ELE_TAG_Nonlinear_two_functions_ContactElement_3DOF_3DOF 5006
#define ELE_TAG_Nonlinear_two_functions_ContactElement_3DOF_6DOF 5007
#define ELE_TAG_Nonlinear_two_functions_ContactElement_6DOF_6DOF 5008
#define ELE_TAG_FrictionalPenaltyContact 5015  //J.Abell

#define ELE_TAG_PenaltyElement 5009
#define ELE_TAG_PenaltyElementApplyDisplacement 5010

#define ELE_TAG_ElasticBeam 5011         // Nima Tafazzoli, December 2012
#define ELE_TAG_rank_one_deficient_elastic_pinned_fixed_beam 5012  // Nima Tafazzoli, December 2012



#define ELE_TAG_LargeDispBeamColumn3d    6002
#define ELE_TAG_FourNodeQuad     1010
#define ELE_TAG_BeamWithHinges3d  402
#define ELE_TAG_EightNodeBrick   7001
#define ELE_TAG_EightNodeBrickElastic   7015
#define ELE_TAG_EightNodeBrickVariableGP   7019
#define ELE_TAG_TwentyNodeBrick   7002
#define ELE_TAG_TwentyNodeBrickVariableGP   7020
#define ELE_TAG_TwentySevenNodeBrickVariableGP   7021
#define ELE_TAG_TwentyNodeBrickElastic   7017
#define ELE_TAG_VariableNodeBrick   7018
#define ELE_TAG_EightNodeBrick_u_p_U            7003
#define ELE_TAG_EightNodeBrick_u_p_U_LT            7022
#define ELE_TAG_TwentyNodeBrick_u_p_U            7004
#define ELE_TAG_FourNodeQuadUP  7005
#define ELE_TAG_TotalLagrangianFD20NodeBrick 7006 // ZC added
#define ELE_TAG_TotalLagrangianFD8NodeBrick 7007
#define ELE_TAG_EightNode_LDBrick_u_p 7008
#define ELE_TAG_EightNode_Brick_u_p 7009
#define ELE_TAG_TwentySevenNodeBrick   7010
#define ELE_TAG_TwentySevenNodeBrickElastic   7016
#define ELE_TAG_BrickUP         7011
#define ELE_TAG_Nine_Four_Node_QuadUP         7012
#define ELE_TAG_Twenty_Eight_Node_BrickUP         7013
#define ELE_TAG_Twenty_Node_Brick         7014
#define ELE_TAG_PlateMITC4      2023
#define ELE_TAG_ShellMITC4      2024
#define ELE_TAG_NewShellMITC4      2025
#define ELE_TAG_ThreeNodeAndesShell 2026
#define ELE_TAG_ThreeNodeAndesBending 2027
#define ELE_TAG_ThreeNodeAndesMembrane 2028
#define ELE_TAG_FourNodeAndesShell 2029
#define ELE_TAG_Plate1          2022
#define ELE_TAG_Brick                      3458
#define ELE_TAG_BbarBrick                  3457
#define ELE_TAG_EnhancedQuad               3459
#define ELE_TAG_ConstantPressureVolumeQuad 3456
#define ELE_TAG_NineNodeMixedQuad          3359
#define ELE_TAG_DispBeamColumn3d 9871
#define ELE_TAG_HingedBeam3d     9873
#define ELE_TAG_TwoPointHingedBeam3d     9875
#define ELE_TAG_OnePointHingedBeam3d     9877
#define ELE_TAG_BeamColumnJoint3d 4445

// LTensor Elements
#define ELE_TAG_EightNodeBrickLT   8001
#define ELE_TAG_TwentySevenNodeBrickLT   8002
#define ELE_TAG_ShearBeamLT 8003
#define ELE_TAG_EightNodeBrickLT_NoOutput   8004
#define ELE_TAG_TwentyNodeBrickLT 8005



#define ELE_TAG_Joint3D    9903



#define BEAM_INTEGRATION_TAG_Lobatto         1
#define BEAM_INTEGRATION_TAG_Legendre        2
#define BEAM_INTEGRATION_TAG_Radau           3
#define BEAM_INTEGRATION_TAG_NewtonCotes           4
#define BEAM_INTEGRATION_TAG_Trapezoidal           5
#define BEAM_INTEGRATION_TAG_Midpoint           6
#define BEAM_INTEGRATION_TAG_UserDefined     7
#define BEAM_INTEGRATION_TAG_FixedLocation     8
#define BEAM_INTEGRATION_TAG_LowOrder     9
#define BEAM_INTEGRATION_TAG_MidDistance     40

#define BEAM_INTEGRATION_TAG_HingeMidpoint 10
#define BEAM_INTEGRATION_TAG_HingeEndpoint 11
#define BEAM_INTEGRATION_TAG_HingeRadau    12
#define BEAM_INTEGRATION_TAG_HingeRadauTwo    13
#define BEAM_INTEGRATION_TAG_UserHinge     14
#define BEAM_INTEGRATION_TAG_DistHinge     15
#define BEAM_INTEGRATION_TAG_RegularizedHinge     16

#define BEAM_INTEGRATION_TAG_HingeMidpoint3d 30
#define BEAM_INTEGRATION_TAG_HingeEndpoint3d 31
#define BEAM_INTEGRATION_TAG_HingeRadau3d    32
#define BEAM_INTEGRATION_TAG_HingeRadauTwo3d    33
#define BEAM_INTEGRATION_TAG_UserHinge3d     34
#define BEAM_INTEGRATION_TAG_DistHinge3d     35


#define CRDTR_TAG_LinearCrdTransf3d 4
#define CRDTR_TAG_PDeltaCrdTransf3d 5
#define CRDTR_TAG_CorotCrdTransf3d  6

#define NOD_TAG_Node        1
#define NOD_TAG_DummyNode   2

#define LOAD_TAG_LoadCase   0
#define LOAD_TAG_NodalLoad  1
#define LOAD_TAG_EarthquakeNodalLoad    2
#define LOAD_TAG_SingleExcitation   3
#define LOAD_TAG_RectPulseNodalLoad     4

#define CNSTRNT_TAG_SP_Constraint   1
#define CNSTRNT_TAG_MP_Constraint   2
#define CNSTRNT_TAG_ImposedMotionSP 3
#define CNSTRNT_TAG_ImposedMotionSP1    4
#define CNSTRNT_TAG_MP_Joint3D          7


#define MATRIX_TAG_Matrix   1

#define VECTOR_TAG_Vector   1

#define ID_TAG_ID       1

#define HANDLER_TAG_PlainHandler            1
#define HANDLER_TAG_LagrangeConstraintHandler       2
#define HANDLER_TAG_PenaltyConstraintHandler        3
#define HANDLER_TAG_TransformationConstraintHandler     4
#define HANDLER_TAG_PenaltyHandlerNoHomoSPMultipliers   5

#define NUMBERER_TAG_DOF_Numberer       1
#define NUMBERER_TAG_PlainNumberer  2
#define NUMBERER_TAG_ParallelNumberer   3

#define GraphNUMBERER_TAG_RCM           1
#define GraphNUMBERER_TAG_SimpleNumberer    2
#define GraphNUMBERER_TAG_MyRCM         3
#define GraphNUMBERER_TAG_Metis         4
#define GraphNUMBERER_TAG_ParMetis      5 //Guanzhou added

#define AnaMODEL_TAGS_AnalysisModel     1

#define EquiALGORITHM_TAGS_Linear       1
#define EquiALGORITHM_TAGS_NewtonRaphson        2
#define EquiALGORITHM_TAGS_ModifiedNewton   3
#define EquiALGORITHM_TAGS_NewtonLineSearch   4


#define ACCELERATOR_TAGS_Krylov     1
#define ACCELERATOR_TAGS_Secant     2

#define LINESEARCH_TAGS_InitialInterpolatedLineSearch 1
#define LINESEARCH_TAGS_BisectionLineSearch           2
#define LINESEARCH_TAGS_RegulaFalsiLineSearch         3
#define LINESEARCH_TAGS_SecantLineSearch              4


#define INTEGRATOR_TAGS_Newmark             2
#define INTEGRATOR_TAGS_HHT                 3
#define INTEGRATOR_TAGS_HHT1                18
#define INTEGRATOR_TAGS_HHT_alpha_method                10
#define INTEGRATOR_TAGS_WilsonTheta             4
#define INTEGRATOR_TAGS_CentralDifference       5
#define INTEGRATOR_TAGS_LoadControl             6
#define INTEGRATOR_TAGS_DisplacementControl     7
#define INTEGRATOR_TAGS_ArcLength           5
#define INTEGRATOR_TAGS_LoadPath            8
#define INTEGRATOR_TAGS_Newmark1            9
#define INTEGRATOR_TAGS_MinUnbalDispNorm    11
#define INTEGRATOR_TAGS_ArcLength1          12
#define INTEGRATOR_TAGS_StaticSensitivity       13
#define INTEGRATOR_TAGS_HSConstraint            14
#define INTEGRATOR_TAGS_DistributedDisplacementControl  15
#define INTEGRATOR_TAGS_CentralDifferenceAlternative 16
#define INTEGRATOR_TAGS_CentralDifferenceNoDamping 17


#define LinSOE_TAGS_FullGenLinSOE       1
#define LinSOE_TAGS_BandGenLinSOE       2
#define LinSOE_TAGS_BandSPDLinSOE       3
#define LinSOE_TAGS_ProfileSPDLinSOE        4
#define LinSOE_TAGS_SlowLinearSOE       5
#define LinSOE_TAGS_SparseGenColLinSOE      6
#define LinSOE_TAGS_PetscSOE            7
#define LinSOE_TAGS_ShadowPetscSOE      8
#define LinSOE_TAGS_ActorPetscSOE       9
#define LinSOE_TAGS_UmfpackGenLinSOE        10
#define LinSOE_TAGS_SymSparseLinSOE         11
#define LinSOE_TAGS_DiagonalLinSOE         12
#define LinSOE_TAGS_ItpackLinSOE           13
#define LinSOE_TAGS_ProfileSPDLinSOEGather  14
#define LinSOE_TAGS_DistributedBandGenLinSOE        15
#define LinSOE_TAGS_DistributedBandSPDLinSOE        16
#define LinSOE_TAGS_DistributedProfileSPDLinSOE     17
#define LinSOE_TAGS_DistributedSparseGenColLinSOE       18
#define LinSOE_TAGS_SparseGenRowLinSOE 19
#define LinSOE_TAGS_DistributedSparseGenRowLinSOE 20
#define LinSOE_TAGS_DiagonalSOE 21
#define LinSOE_TAGS_DistributedDiagonalSOE 22

#define SOLVER_TAGS_FullGenLinLapackSolver      1
#define SOLVER_TAGS_BandGenLinLapackSolver      2
#define SOLVER_TAGS_BandSPDLinLapackSolver      3
#define SOLVER_TAGS_ProfileSPDLinDirectSolver   4
#define SOLVER_TAGS_ProfileSPDLinSubstrSolver   5
#define SOLVER_TAGS_SlowLinearSOESolver     6
#define SOLVER_TAGS_BandSPDLinThreadSolver      7
#define SOLVER_TAGS_ProfileSPDLinDirectThreadSolver     8
#define SOLVER_TAGS_ProfileSPDLinDirectBlockSolver      9
#define SOLVER_TAGS_ProfileSPDLinDirectSkypackSolver    10
#define SOLVER_TAGS_SuperLU                 11
#define SOLVER_TAGS_ThreadedSuperLU             12
#define SOLVER_TAGS_PetscSolver                 13
#define SOLVER_TAGS_UmfpackGenLinSolver             14
#define SOLVER_TAGS_SymSparseLinSolver 15
#define SOLVER_TAGS_DiagonalLinSolver 16
#define SOLVER_TAGS_Itpack            17
#define SOLVER_TAGS_ProfileSPDLinSolverGather   18
#define SOLVER_TAGS_DistributedSuperLU              19
#define SOLVER_TAGS_DistributedSparseGenRowLinSOE 20
#define SOLVER_TAGS_DiagonalDirectSolver 21
#define SOLVER_TAGS_DistributedDiagonalSolver 22
#define SOLVER_TAGS_PetscSparseSeqSolver 23


#define OPS_STREAM_TAGS_FileStream      1
#define OPS_STREAM_TAGS_StandardStream      2


#define DATAHANDLER_TAGS_DataOutputStreamHandler        1
#define DATAHANDLER_TAGS_DataOutputFileHandler      2
#define DATAHANDLER_TAGS_DataOutputDatabaseHandler      3

#define DomDecompALGORITHM_TAGS_DomainDecompAlgo 1

#define DomDecompANALYSIS_TAGS_DomainDecompositionAnalysis 1
#define ANALYSIS_TAGS_StaticDomainDecompositionAnalysis 2
#define ANALYSIS_TAGS_TransientDomainDecompositionAnalysis 3


#define RANDOM_VARIABLE_beta                1
#define RANDOM_VARIABLE_chisquare           2
#define RANDOM_VARIABLE_exponential         3
#define RANDOM_VARIABLE_gamma               4
#define RANDOM_VARIABLE_gumbel              5
#define RANDOM_VARIABLE_laplace             6
#define RANDOM_VARIABLE_lognormal           7
#define RANDOM_VARIABLE_normal              8
#define RANDOM_VARIABLE_pareto              9
#define RANDOM_VARIABLE_rayleigh            10
#define RANDOM_VARIABLE_shiftedexponential  11
#define RANDOM_VARIABLE_shiftedrayleigh     12
#define RANDOM_VARIABLE_type1largestvalue   13
#define RANDOM_VARIABLE_type1smallestvalue  14
#define RANDOM_VARIABLE_type2largestvalue   15
#define RANDOM_VARIABLE_type3smallestvalue  16
#define RANDOM_VARIABLE_uniform             17
#define RANDOM_VARIABLE_weibull             18
#define RANDOM_VARIABLE_userdefined             19
#define RANDOM_VARIABLE_caughey3rd          20
#define RANDOM_VARIABLE_caughey4th          21

#define RANDOM_VARIABLE_POSITIONER        1
#define PARAMETER_POSITIONER              2

#define CORRELATION_COEFFICIENT           1

#define LIMIT_STATE_FUNCTION              1

#define MODULATING_FUNCTION_gamma         1
#define MODULATING_FUNCTION_constant      2
#define MODULATING_FUNCTION_trapezoidal   3

#define FILTER_standardLinearOscillator   1

#define SPECTRUM_jonswap                  1
#define SPECTRUM_constant                 2
#define SPECTRUM_points                   3


//Guanzhou added below
#define YS_TAG_CAMYieldSurface          8001
#define YS_TAG_DPYieldSurface01         8002
#define YS_TAG_DPYieldSurface           8003
#define YS_TAG_MDYieldSurface           8004
#define YS_TAG_RMC01YieldSurface        8005
#define YS_TAG_TriFCYieldSurface        8006
#define YS_TAG_VMYieldSurface           8007

#define PS_TAG_CAMPotentialSurface         8008
#define PS_TAG_DPPotentialSurface          8009
#define PS_TAG_MDPotentialSurface01        8010
#define PS_TAG_MDPotentialSurface           8011
#define PS_TAG_RMC01PotentialSurface        8012
#define PS_TAG_VMPotentialSurface           8013

#define TAG_EPState             8014

#define ELT_TAG_L_Eij 8050
#define ELT_TAG_NL_Eij 8051
#define ELT_TAG_NL_EijMD 8052
#define ELS_TAG_L_Eeq 8053
#define ELS_TAG_NL_Eeq 8054
#define ELS_TAG_NL_Ep 8055



#endif


