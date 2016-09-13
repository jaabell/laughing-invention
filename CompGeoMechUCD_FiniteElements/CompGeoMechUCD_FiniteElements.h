///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:               Feb 2014
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


#include "./Contact_Elements/HardContact.h"
#include "./Contact_Elements/SoftContact.h"
#include "./solid_brick_8_nodes/EightNodeBrick.h"
#include "./solid_brick_8_nodes_variable_number_of_gauss_points/EightNodeBrickVariableGP.h"
#include "./solid_brick_8_nodes_elastic/EightNodeBrickElastic.h"
#include "./coupled_u-p-U_brick_8_nodes/EightNodeBrick_u_p_U.h"


// Coupled Elements 

// Eight Node Brick
#include "./coupled_u-p-U_bricks_LT/EightNodeBrick_upU_LT.h"
#include "./coupled_u-p-U_bricks_LT/EightNodeBrick_upU_LT_Elastic.h"

// Twenty Node Brick
#include "./coupled_u-p-U_bricks_LT/TwentyNodeBrick_upU_LT.h"
#include "./coupled_u-p-U_bricks_LT/TwentyNodeBrick_upU_LT_Elastic.h"

// Twenty Seven Node Brick
#include "./coupled_u-p-U_bricks_LT/TwentySevenNodeBrick_upU_LT.h"
#include "./coupled_u-p-U_bricks_LT/TwentySevenNodeBrick_upU_LT_Elastic.h"


// General Brick Elements

// Eight Node Brick
#include "./solid_brick_8_nodes_ltensor/EightNodeBrickLT.h"
#include "./solid_brick_8_nodes_ltensor/EightNodeBrickLT_Elastic.h"
#include "./solid_brick_8_nodes_ltensor/EightNodeBrickLT_NoOutput.h"

// Twenty Node Brick
#include "./solid_brick_20_nodes_ltensor/TwentyNodeBrickLT.h"
#include "./solid_brick_20_nodes_ltensor/TwentyNodeBrickLT_Elastic.h"

// Twenty Seven Node Brick
#include "./solid_brick_27_nodes_ltensor/TwentySevenNodeBrickLT.h"
#include "./solid_brick_27_nodes_ltensor/TwentySevenNodeBrickLT_Elastic.h"





#include "./coupled_u-p_brick_8_nodes/EightNode_Brick_u_p.h"
#include "./coupled_u-p_large_deformation_brick_8_nodes/EightNode_LDBrick_u_p.h"
#include "./coupled_u-p-U_brick_20_nodes/TwentyNodeBrick_u_p_U.h"
#include "./solid_brick_20_nodes/TwentyNodeBrick.h"
#include "./solid_brick_20_nodes_elastic/TwentyNodeBrickElastic.h"
#include "./solid_brick_20_nodes_variable_number_of_gauss_points/TwentyNodeBrickVariableGP.h"
#include "./solid_brick_27_nodes_variable_number_of_gauss_points/TwentySevenNodeBrickVariableGP.h"
#include "./solid_brick_27_nodes/TwentySevenNodeBrick.h"
#include "./solid_brick_27_nodes_elastic/TwentySevenNodeBrickElastic.h"
// #include "./penalty_element_apply_displacement/PenaltyElementApplyDisplacement.h"
// #include "./penalty_element/PenaltyElement.h"
#include "./shell_andes/ThreeNodeAndesShell.h"
#include "./shell_andes/FourNodeAndesShell.h"
#include "./elastic_beam/ElasticBeam.h"
#include "./rank_one_deficient_elastic_pinned_fixed_beam/rank_one_deficient_elastic_pinned_fixed_beam.h"
#include "./solid_brick_variable_nodes_8_to_27/VariableNodeBrick.h"
#include "./nonlinear_shear_beam/ShearBeamLT.h"

#include "./solid_brick_large_deformation_total_Lagrangian_8_nodes/TotalLagrangianFD8NodeBrick.h"
#include "./solid_brick_large_deformation_total_Lagrangian_20_nodes/TotalLagrangianFD20NodeBrick.h"



// #include "./coupled_u-p-U_bricks_LT/EightNodeBrick_u_p_U_LT.h"
