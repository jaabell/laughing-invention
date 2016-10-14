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

// #################################################
// Coupled Elements   [Sumeet July, 2016]
// #################################################

#include "./Contact_Elements/HardContact.h"
#include "./Contact_Elements/SoftContact.h"

//#################################################### 
// General Brick Elements  [Sumeet September, 2016]
//####################################################

#include "./Solid_Bricks/solid_brick_8_nodes/EightNodeBrick.h"
#include "./Solid_Bricks/solid_brick_20_nodes/TwentyNodeBrick.h"
#include "./Solid_Bricks/solid_brick_27_nodes/TwentySevenNodeBrick.h"
#include "./Solid_Bricks/solid_brick_variable_nodes/VariableNodeBrickLT.h"

// Eight Node Brick
#include "./Solid_Bricks/solid_brick_8_nodes/EightNodeBrickOrderOne.h"
#include "./Solid_Bricks/solid_brick_8_nodes/EightNodeBrickOrderTwo.h"
#include "./Solid_Bricks/solid_brick_8_nodes/EightNodeBrickOrderThree.h"
#include "./Solid_Bricks/solid_brick_8_nodes/EightNodeBrickOrderFour.h"
#include "./Solid_Bricks/solid_brick_8_nodes/EightNodeBrickOrderFive.h"
#include "./Solid_Bricks/solid_brick_8_nodes/EightNodeBrickOrderSix.h"
// Twenty Node Brick
#include "./Solid_Bricks/solid_brick_20_nodes/TwentyNodeBrickOrderOne.h"
#include "./Solid_Bricks/solid_brick_20_nodes/TwentyNodeBrickOrderTwo.h"
#include "./Solid_Bricks/solid_brick_20_nodes/TwentyNodeBrickOrderThree.h"
#include "./Solid_Bricks/solid_brick_20_nodes/TwentyNodeBrickOrderFour.h"
#include "./Solid_Bricks/solid_brick_20_nodes/TwentyNodeBrickOrderFive.h"
#include "./Solid_Bricks/solid_brick_20_nodes/TwentyNodeBrickOrderSix.h"
// Twenty Seven Node Brick
#include "./Solid_Bricks/solid_brick_27_nodes/TwentySevenNodeBrickOrderOne.h"
#include "./Solid_Bricks/solid_brick_27_nodes/TwentySevenNodeBrickOrderTwo.h"
#include "./Solid_Bricks/solid_brick_27_nodes/TwentySevenNodeBrickOrderThree.h"
#include "./Solid_Bricks/solid_brick_27_nodes/TwentySevenNodeBrickOrderFour.h"
#include "./Solid_Bricks/solid_brick_27_nodes/TwentySevenNodeBrickOrderFive.h"
#include "./Solid_Bricks/solid_brick_27_nodes/TwentySevenNodeBrickOrderSix.h"
// Variable Node Brick
#include "./Solid_Bricks/solid_brick_variable_nodes/VariableNodeBrickOrderOne.h"
#include "./Solid_Bricks/solid_brick_variable_nodes/VariableNodeBrickOrderTwo.h"
#include "./Solid_Bricks/solid_brick_variable_nodes/VariableNodeBrickOrderThree.h"
#include "./Solid_Bricks/solid_brick_variable_nodes/VariableNodeBrickOrderFour.h"
#include "./Solid_Bricks/solid_brick_variable_nodes/VariableNodeBrickOrderFive.h"
#include "./Solid_Bricks/solid_brick_variable_nodes/VariableNodeBrickOrderSix.h"


// ####################################################################
// Coupled upU Elements   [Sumeet September, 2016]
// ####################################################################

#include "./Solid_Bricks_upU/solid_brick_8_nodes_upU/EightNodeBrick_upU.h"
#include "./Solid_Bricks_upU/solid_brick_20_nodes_upU/TwentyNodeBrick_upU.h"
#include "./Solid_Bricks_upU/solid_brick_27_nodes_upU/TwentySevenNodeBrick_upU.h"
#include "./Solid_Bricks_upU/solid_brick_variable_nodes_upU/VariableNodeBrick_upU.h"

// Eight Node Brick
#include "./Solid_Bricks_upU/solid_brick_8_nodes_upU/EightNodeBrickOrderOne_upU.h"
#include "./Solid_Bricks_upU/solid_brick_8_nodes_upU/EightNodeBrickOrderTwo_upU.h"
#include "./Solid_Bricks_upU/solid_brick_8_nodes_upU/EightNodeBrickOrderThree_upU.h"
#include "./Solid_Bricks_upU/solid_brick_8_nodes_upU/EightNodeBrickOrderFour_upU.h"
#include "./Solid_Bricks_upU/solid_brick_8_nodes_upU/EightNodeBrickOrderFive_upU.h"
#include "./Solid_Bricks_upU/solid_brick_8_nodes_upU/EightNodeBrickOrderSix_upU.h"
// Twenty Node Brick
#include "./Solid_Bricks_upU/solid_brick_20_nodes_upU/TwentyNodeBrickOrderOne_upU.h"
#include "./Solid_Bricks_upU/solid_brick_20_nodes_upU/TwentyNodeBrickOrderTwo_upU.h"
#include "./Solid_Bricks_upU/solid_brick_20_nodes_upU/TwentyNodeBrickOrderThree_upU.h"
#include "./Solid_Bricks_upU/solid_brick_20_nodes_upU/TwentyNodeBrickOrderFour_upU.h"
#include "./Solid_Bricks_upU/solid_brick_20_nodes_upU/TwentyNodeBrickOrderFive_upU.h"
#include "./Solid_Bricks_upU/solid_brick_20_nodes_upU/TwentyNodeBrickOrderSix_upU.h"
// Twenty Seven Node Brick
#include "./Solid_Bricks_upU/solid_brick_27_nodes_upU/TwentySevenNodeBrickOrderOne_upU.h"
#include "./Solid_Bricks_upU/solid_brick_27_nodes_upU/TwentySevenNodeBrickOrderTwo_upU.h"
#include "./Solid_Bricks_upU/solid_brick_27_nodes_upU/TwentySevenNodeBrickOrderThree_upU.h"
#include "./Solid_Bricks_upU/solid_brick_27_nodes_upU/TwentySevenNodeBrickOrderFour_upU.h"
#include "./Solid_Bricks_upU/solid_brick_27_nodes_upU/TwentySevenNodeBrickOrderFive_upU.h"
#include "./Solid_Bricks_upU/solid_brick_27_nodes_upU/TwentySevenNodeBrickOrderSix_upU.h"
// Variable Node Brick
#include "./Solid_Bricks_upU/solid_brick_variable_nodes_upU/VariableNodeBrickOrderOne_upU.h"
#include "./Solid_Bricks_upU/solid_brick_variable_nodes_upU/VariableNodeBrickOrderTwo_upU.h"
#include "./Solid_Bricks_upU/solid_brick_variable_nodes_upU/VariableNodeBrickOrderThree_upU.h"
#include "./Solid_Bricks_upU/solid_brick_variable_nodes_upU/VariableNodeBrickOrderFour_upU.h"
#include "./Solid_Bricks_upU/solid_brick_variable_nodes_upU/VariableNodeBrickOrderFive_upU.h"
#include "./Solid_Bricks_upU/solid_brick_variable_nodes_upU/VariableNodeBrickOrderSix_upU.h"

// ####################################################################
// Coupled up Elements   [Sumeet September, 2016]
// ####################################################################

#include "./Solid_Bricks_up/solid_brick_8_nodes_up/EightNodeBrick_up.h"
#include "./Solid_Bricks_up/solid_brick_20_nodes_up/TwentyNodeBrick_up.h"
#include "./Solid_Bricks_up/solid_brick_27_nodes_up/TwentySevenNodeBrick_up.h"
#include "./Solid_Bricks_up/solid_brick_variable_nodes_up/VariableNodeBrick_up.h"

// Eight Node Brick
#include "./Solid_Bricks_up/solid_brick_8_nodes_up/EightNodeBrickOrderOne_up.h"
#include "./Solid_Bricks_up/solid_brick_8_nodes_up/EightNodeBrickOrderTwo_up.h"
#include "./Solid_Bricks_up/solid_brick_8_nodes_up/EightNodeBrickOrderThree_up.h"
#include "./Solid_Bricks_up/solid_brick_8_nodes_up/EightNodeBrickOrderFour_up.h"
#include "./Solid_Bricks_up/solid_brick_8_nodes_up/EightNodeBrickOrderFive_up.h"
#include "./Solid_Bricks_up/solid_brick_8_nodes_up/EightNodeBrickOrderSix_up.h"
// Twenty Node Brick
#include "./Solid_Bricks_up/solid_brick_20_nodes_up/TwentyNodeBrickOrderOne_up.h"
#include "./Solid_Bricks_up/solid_brick_20_nodes_up/TwentyNodeBrickOrderTwo_up.h"
#include "./Solid_Bricks_up/solid_brick_20_nodes_up/TwentyNodeBrickOrderThree_up.h"
#include "./Solid_Bricks_up/solid_brick_20_nodes_up/TwentyNodeBrickOrderFour_up.h"
#include "./Solid_Bricks_up/solid_brick_20_nodes_up/TwentyNodeBrickOrderFive_up.h"
#include "./Solid_Bricks_up/solid_brick_20_nodes_up/TwentyNodeBrickOrderSix_up.h"
// Twenty Seven Node Brick
#include "./Solid_Bricks_up/solid_brick_27_nodes_up/TwentySevenNodeBrickOrderOne_up.h"
#include "./Solid_Bricks_up/solid_brick_27_nodes_up/TwentySevenNodeBrickOrderTwo_up.h"
#include "./Solid_Bricks_up/solid_brick_27_nodes_up/TwentySevenNodeBrickOrderThree_up.h"
#include "./Solid_Bricks_up/solid_brick_27_nodes_up/TwentySevenNodeBrickOrderFour_up.h"
#include "./Solid_Bricks_up/solid_brick_27_nodes_up/TwentySevenNodeBrickOrderFive_up.h"
#include "./Solid_Bricks_up/solid_brick_27_nodes_up/TwentySevenNodeBrickOrderSix_up.h"
// Variable Node Brick
#include "./Solid_Bricks_up/solid_brick_variable_nodes_up/VariableNodeBrickOrderOne_up.h"
#include "./Solid_Bricks_up/solid_brick_variable_nodes_up/VariableNodeBrickOrderTwo_up.h"
#include "./Solid_Bricks_up/solid_brick_variable_nodes_up/VariableNodeBrickOrderThree_up.h"
#include "./Solid_Bricks_up/solid_brick_variable_nodes_up/VariableNodeBrickOrderFour_up.h"
#include "./Solid_Bricks_up/solid_brick_variable_nodes_up/VariableNodeBrickOrderFive_up.h"
#include "./Solid_Bricks_up/solid_brick_variable_nodes_up/VariableNodeBrickOrderSix_up.h"

// ####################################################################
// Structural Elements    [Sumeet September, 2016]
// ####################################################################


// #include "./penalty_element_apply_displacement/PenaltyElementApplyDisplacement.h"
// #include "./penalty_element/PenaltyElement.h"
#include "./shell_andes/ThreeNodeAndesShell.h"
#include "./shell_andes/FourNodeAndesShell.h"
#include "./elastic_beam/ElasticBeam.h"
#include "./rank_one_deficient_elastic_pinned_fixed_beam/rank_one_deficient_elastic_pinned_fixed_beam.h"
#include "./nonlinear_shear_beam/ShearBeam.h"

// ####################################################################
// Large Deformation Elements    [Sumeet September, 2016]
// ####################################################################

#include "./Large_Deformation_Elements/solid_brick_large_deformation_total_Lagrangian_8_nodes/TotalLagrangianFD8NodeBrick.h"
#include "./Large_Deformation_Elements/solid_brick_large_deformation_total_Lagrangian_20_nodes/TotalLagrangianFD20NodeBrick.h"
#include "./Large_Deformation_Elements/coupled_u-p_large_deformation_brick_8_nodes/EightNode_LDBrick_u_p.h"


// ####################################################################
// Depretiated elements (Can be removed abter verification of equivalent new elements )    [Sumeet September, 2016]
// ####################################################################

#include "./solid_brick_variable_nodes_8_to_27/VariableNodeBrick.h"
