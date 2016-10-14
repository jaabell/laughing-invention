///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli and Boris Jeremic and Jose && Sumeet 
// DATE:               Aug. 2001
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

/////////////////////////////////////////////////////////////////////////////////////
// Sumeet Kumar Sinha [September, 2016]
// This is a Header file that is taken from MasterVariableNodeBrick.h
// The macros (#define) that go into this master BrickHeaderFile are 
//
// BRICK_TYPE -> define the element type like "VarableNodeBrick"
// BRICK_ELEMENT_NAME -> Name of the brick in string Eg:- "VarableNodeBrick"
// NUMBER_OF_NODES    -> Number of nodes in the brick elemnt Eg:- 8 for eight node brick
// ELE_TAG_BRICK_TYPE -> ELE_TAFG for the brick element
// Order              -> Order of intergration
//
// This master header file was created to make the code moduluar and consistent 
// over all brick elements
/////////////////////////////////////////////////////////////////////////////////////

#include <LTensorDisplay.h>
#include <HDF5_Channel.h>
#include <ESSITimer.h>
#include <unistd.h>

#define Num_IntegrationPts Order
#define Num_TotalGaussPts Num_IntegrationPts*Num_IntegrationPts*Num_IntegrationPts


DTensor2 BRICK_TYPE::gp_coords(Num_TotalGaussPts, 3, 0.0);
DTensor2 BRICK_TYPE::gp_weight(Num_TotalGaussPts, 3, 0.0);
Matrix BRICK_TYPE::gauss_points(Num_TotalGaussPts, 3);
vector<float> BRICK_TYPE::Gauss_Output_Vector(Num_TotalGaussPts*18);

Vector BRICK_TYPE::ShapeFunctionValues_in_function( 9 );  // Nima added for surface load (July 2012)
Vector BRICK_TYPE::J_vector_in_function( 3 );             // Nima added for surface load (July 2012)
double BRICK_TYPE::SurfaceLoadValues_in_function;

//====================================================================
// Constructor
//====================================================================

BRICK_TYPE::BRICK_TYPE( int element_number,
        int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
        int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
        int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
        int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
        int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
        int node_numb_21,  int node_numb_22,  int node_numb_23,  int node_numb_24,
        int node_numb_25,  int node_numb_26,  int node_numb_27,
        NDMaterialLT *Globalmmodel)

    : Element( element_number, ELE_TAG_BRICK_TYPE ),
      rho( 0.0 ), connectedExternalNodes( 27 ),nodeExistence(27),
      Ki( 0 )
{

    this->setMaterialTag(Globalmmodel->getTag());

    rho = Globalmmodel->getRho();
    mmodel = Globalmmodel;

    material_array = new NDMaterialLT *[Num_TotalGaussPts];

    if (material_array == 0)
    {
        cerr << BRICK_ELEMENT_NAME << "::" << BRICK_ELEMENT_NAME << " -- failed allocate material model pointer\n";
        exit(-1);
    }

    // Generate NDMaterialLT for use at each Gauss point.
    for (int k = 0; k < Num_TotalGaussPts; k++)
    {
        material_array[k] = mmodel->getCopy();
    }

    //GP coordinates and weights. ====================================================
    // This initializes class wide members gp_coords and gp_weights .
    // Since LTensor does not provide initializer lists, this is the only way to
    // do this.

    Matrix Gauss_coordinates = get_Gauss_p_c();
    Matrix Gauss_weights = get_Gauss_p_w();

    short where = 0;
    for ( short ii = 1 ; ii <=Order ; ii++ )
    {
        for ( short jj = 1 ; jj <=Order ; jj++ )
        {
            for ( short kk = 1 ; kk <=Order ; kk++ )
            {
                gp_coords(where, 0) =Gauss_coordinates(Order,ii);
                gp_weight(where, 0) =Gauss_weights(Order,ii);

                gp_coords(where, 1) =Gauss_coordinates(Order,jj);
                gp_weight(where, 1) =Gauss_weights(Order,jj);

                gp_coords(where, 2) =Gauss_coordinates(Order,kk);
                gp_weight(where, 2) =Gauss_weights(Order,kk);
                where ++;
            }
        }
    }
    // =============================================================================

    is_mass_computed = false;

    connectedExternalNodes( 0 ) = node_numb_1;
    connectedExternalNodes( 1 ) = node_numb_2;
    connectedExternalNodes( 2 ) = node_numb_3;
    connectedExternalNodes( 3 ) = node_numb_4;

    connectedExternalNodes( 4 ) = node_numb_5;
    connectedExternalNodes( 5 ) = node_numb_6;
    connectedExternalNodes( 6 ) = node_numb_7;
    connectedExternalNodes( 7 ) = node_numb_8;

    connectedExternalNodes( 8) = node_numb_9;
    connectedExternalNodes( 9) = node_numb_10;
    connectedExternalNodes(10) = node_numb_11;
    connectedExternalNodes(11) = node_numb_12;

    connectedExternalNodes(12) = node_numb_13;
    connectedExternalNodes(13) = node_numb_14;
    connectedExternalNodes(14) = node_numb_15;
    connectedExternalNodes(15) = node_numb_16;

    connectedExternalNodes(16) = node_numb_17;
    connectedExternalNodes(17) = node_numb_18;
    connectedExternalNodes(18) = node_numb_19;
    connectedExternalNodes(19) = node_numb_20;

    connectedExternalNodes(20) = node_numb_21;
    connectedExternalNodes(21) = node_numb_22;
    connectedExternalNodes(22) = node_numb_23;
    connectedExternalNodes(23) = node_numb_24;
    connectedExternalNodes(24) = node_numb_25;
    connectedExternalNodes(25) = node_numb_26;
    connectedExternalNodes(26) = node_numb_27;

    for ( int i = 0; i < 27; i++ )
    {
        theNodes[i] = 0;
    }

    // loop nodes 1-27 and :
    //  1) fill the nodeExistence ID
    //  2) calculate the total number of nodes
    nodes_in_brick = 0;

    for ( int i = 0 ; i < 27 ; i++ )
    {
        if (connectedExternalNodes(i) == 0 )
        {
            nodeExistence(i) = 0;
        }
        else
        {
            nodes_in_brick++;
            nodeExistence(i) = 1;
            Global_to_Local_Node_Mapping[connectedExternalNodes(i)] = i;
        }
    }

    Q = Vector(nodes_in_brick * 3);
    M = Matrix(nodes_in_brick * 3, nodes_in_brick * 3);
    P = Vector(nodes_in_brick * 3);
    K = Matrix(nodes_in_brick*3,nodes_in_brick*3);

    this->setNumberOfBoundaryNodes(9);
}



//====================================================================
BRICK_TYPE::BRICK_TYPE(): Element( 0, ELE_TAG_BRICK_TYPE ),
    rho( 0.0 ), connectedExternalNodes( 27 ) , nodeExistence(27),
    Ki( 0 ), mmodel(0)
{
   
    material_array = new NDMaterialLT *[Num_TotalGaussPts];

    if (material_array == 0)
    {
        cerr << BRICK_ELEMENT_NAME << "::" << BRICK_ELEMENT_NAME << "-- failed allocate material model pointer\n";
        exit(-1);
    }

    //GP coordinates and weights. ====================================================
    // This initializes class wide members gp_coords and gp_weights .
    // Since LTensor does not provide initializer lists, this is the only way to
    // do this.

    Matrix Gauss_coordinates = get_Gauss_p_c();
    Matrix Gauss_weights = get_Gauss_p_w();

    short where = 0;
    for ( short ii = 1 ; ii <=Order ; ii++ )
    {
        for ( short jj = 1 ; jj <=Order ; jj++ )
        {
            for ( short kk = 1 ; kk <=Order ; kk++ )
            {
                gp_coords(where, 0) =Gauss_coordinates(Order,ii);
                gp_weight(where, 0) =Gauss_weights(Order,ii);

                gp_coords(where, 1) =Gauss_coordinates(Order,jj);
                gp_weight(where, 1) =Gauss_weights(Order,jj);

                gp_coords(where, 2) =Gauss_coordinates(Order,kk);
                gp_weight(where, 2) =Gauss_weights(Order,kk);
                where ++;
            }
        }
    }
    // =============================================================================


    // zero node pointers
    for ( int i = 0; i < 27; i++ )
    {
        theNodes[i] = 0;
    }
    this->setNumberOfBoundaryNodes(9);

}




//####DESTROYER!###############################################################
BRICK_TYPE::~BRICK_TYPE ()
{
    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {
        // Delete the NDMaterials at each integration point
        if ( material_array[i] )
        {
            delete material_array[i];
        }
    }

    if ( Ki != 0 )
    {
        delete Ki;
    }
}


//#############################################################################
//#############################################################################
//***************************************************************
const DTensor2 &BRICK_TYPE::H_3D( double r1, double r2, double r3 ) const
{
    static DTensor2 H( 81, 3, 0.0 );

    //Shape Functions of Node 27 Along Three Coordinate Directions
    H(78, 0) = nodeExistence(26) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H(79, 1) = H(78, 0);
    H(80, 2) = H(78, 0);


    //Shape Functions of Node 26 Along Three Coordinate Directions
    H(75, 0) = nodeExistence(25) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H(76, 1) = H(75, 0);
    H(77, 2) = H(75, 0);


    //Shape Functions of Node 25 Along Three Coordinate Directions
    H(72, 0) = nodeExistence(24) * 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(73, 1) = H(72, 0);
    H(74, 2) = H(72, 0);


    //Shape Functions of Node 24 Along Three Coordinate Directions
    H(69, 0) = nodeExistence(23) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H(70, 1) = H(69, 0);
    H(71, 2) = H(69, 0);


    //Shape Functions of Node 23 Along Three Coordinate Directions
    H(66, 0) = nodeExistence(22) * 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(67, 1) = H(66, 0);
    H(68, 2) = H(66, 0);


    //Shape Functions of Node 22 Along Three Coordinate Directions
    H(63, 0) = nodeExistence(21) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H(64, 1) = H(63, 0);
    H(65, 2) = H(63, 0);


    //Shape Functions of Node 21 Along Three Coordinate Directions
    H(60, 0) = nodeExistence(20) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(61, 1) = H(60, 0);
    H(62, 2) = H(60, 0);


    //Shape Functions of Node 20 Along Three Coordinate Directions
    H(57, 0) = nodeExistence(19) * ( 0.25 * (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (H(69, 0) + H(72, 0)) - 0.25 * H(60, 0) );
    H(58, 1) = H(57, 0);
    H(59, 2) = H(57, 0);


    //Shape Functions of Node 19 Along Three Coordinate Directions
    H(54, 0) = nodeExistence(18) * ( 0.25 * (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (H(66, 0) + H(69, 0)) - 0.25 * H(60, 0) );
    H(55, 1) = H(54, 0);
    H(56, 2) = H(54, 0);


    //Shape Functions of Node 18 Along Three Coordinate Directions
    H(51, 0) = nodeExistence(17) * ( 0.25 * (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (H(63, 0) + H(66, 0)) - 0.25 * H(60, 0) );
    H(52, 1) = H(51, 0);
    H(53, 2) = H(51, 0);


    //Shape Functions of Node 17 Along Three Coordinate Directions
    H(48, 0) = nodeExistence(16) * ( 0.25 * (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (H(63, 0) + H(72, 0)) - 0.25 * H(60, 0) );
    H(49, 1) = H(48, 0);
    H(50, 2) = H(48, 0);


    //Shape Functions of Node 16 Along Three Coordinate Directions
    H(45, 0) = nodeExistence(15) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (H(72, 0) + H(78, 0)) - 0.25 * H(60, 0) );
    H(46, 1) = H(45, 0);
    H(47, 2) = H(45, 0);


    //Shape Functions of Node 15 Along Three Coordinate Directions
    H(42, 0) = nodeExistence(14) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) - 0.5 * (H(69, 0) + H(78, 0)) - 0.25 * H(60, 0) );
    H(43, 1) = H(42, 0);
    H(44, 2) = H(42, 0);


    //Shape Functions of Node 14 Along Three Coordinate Directions
    H(39, 0) = nodeExistence(13) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (H(66, 0) + H(78, 0)) - 0.25 * H(60, 0) );
    H(40, 1) = H(39, 0);
    H(41, 2) = H(39, 0);


    //Shape Functions of Node 13 Along Three Coordinate Directions
    H(36, 0) = nodeExistence(12) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) - 0.5 * (H(63, 0) + H(78, 0)) - 0.25 * H(60, 0) );
    H(37, 1) = H(36, 0);
    H(38, 2) = H(36, 0);


    //Shape Functions of Node 12 Along Three Coordinate Directions
    H(33, 0) = nodeExistence(11) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (H(72, 0) + H(75, 0)) - 0.25 * H(60, 0) );
    H(34, 1) = H(33, 0);
    H(35, 2) = H(33, 0);


    //Shape Functions of Node 11 Along Three Coordinate Directions
    H(30, 0) = nodeExistence(10) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) - 0.5 * (H(69, 0) + H(75, 0)) - 0.25 * H(60, 0) );
    H(31, 1) = H(30, 0);
    H(32, 2) = H(30, 0);


    //Shape Functions of Node 10 Along Three Coordinate Directions
    H(27, 0) = nodeExistence( 9) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (H(66, 0) + H(75, 0)) - 0.25 * H(60, 0) );
    H(28, 1) = H(27, 0);
    H(29, 2) = H(27, 0);


    //Shape Functions of Node 9 Along Three Coordinate Directions
    H(24, 0) = nodeExistence( 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) - 0.5 * (H(63, 0) + H(75, 0)) - 0.25 * H(60, 0) );
    H(25, 1) = H(24, 0);
    H(26, 2) = H(24, 0);


    //Shape Functions of Node 1 Along Three Coordinate Directions
    H(0, 0) = nodeExistence( 0) * 0.125 * (1 + r1) * (1 + r2) * (1 + r3) - 0.5 * ( H(24, 0) + H(33, 0) + H(48, 0) ) - 0.25 * ( H(63, 0) + H(72, 0) + H(75, 0) ) - 0.125 * H(60, 0);
    H(1, 1) = H(0, 0);
    H(2, 2) = H(0, 0);

    //Shape Functions of Node 2 Along Three Coordinate Directions
    H(3, 0) = nodeExistence( 1) * 0.125 * (1 - r1) * (1 + r2) * (1 + r3) - 0.5 * ( H(24, 0) + H(27, 0) + H(51, 0) ) - 0.25 * ( H(63, 0) + H(66, 0) + H(75, 0) ) - 0.125 * H(60, 0);
    H(4, 1) = H(3, 0);
    H(5, 2) = H(3, 0);

    //Shape Functions of Node 3 Along Three Coordinate Directions
    H(6, 0) = nodeExistence( 2) * 0.125 * (1 - r1) * (1 - r2) * (1 + r3) - 0.5 * ( H(27, 0) + H(30, 0) + H(54, 0) ) - 0.25 * ( H(66, 0) + H(69, 0) + H(75, 0) ) - 0.125 * H(60, 0);
    H(7, 1) = H(6, 0);
    H(8, 2) = H(6, 0);

    //Shape Functions of Node 4 Along Three Coordinate Directions
    H( 9, 0) = nodeExistence( 3) * 0.125 * (1 + r1) * (1 - r2) * (1 + r3) - 0.5 * ( H(30, 0) + H(33, 0) + H(57, 0) ) - 0.25 * ( H(69, 0) + H(72, 0) + H(75, 0) ) - 0.125 * H(60, 0);
    H(10, 1) = H(9, 0);
    H(11, 2) = H(9, 0);

    //Shape Functions of Node 5 Along Three Coordinate Directions
    H(12, 0) = nodeExistence( 4) * 0.125 * (1 + r1) * (1 + r2) * (1 - r3) - 0.5 * ( H(36, 0) + H(45, 0) + H(48, 0) ) - 0.25 * ( H(63, 0) + H(72, 0) + H(78, 0) ) - 0.125 * H(60, 0);
    H(13, 1) = H(12, 0);
    H(14, 2) = H(12, 0);

    //Shape Functions of Node 6 Along Three Coordinate Directions
    H(15, 0) = nodeExistence( 5) * 0.125 * (1 - r1) * (1 + r2) * (1 - r3) - 0.5 * ( H(36, 0) + H(39, 0) + H(51, 0) ) - 0.25 * ( H(63, 0) + H(66, 0) + H(78, 0) ) - 0.125 * H(60, 0);
    H(16, 1) = H(15, 0);
    H(17, 2) = H(15, 0);

    //Shape Functions of Node 7 Along Three Coordinate Directions
    H(18, 0) = nodeExistence( 6) * 0.125 * (1 - r1) * (1 - r2) * (1 - r3) - 0.5 * ( H(39, 0) + H(42, 0) + H(54, 0) ) - 0.25 * ( H(66, 0) + H(69, 0) + H(78, 0) ) - 0.125 * H(60, 0);
    H(19, 1) = H(18, 0);
    H(20, 2) = H(18, 0);


    //Shape Functions of Node 8 Along Three Coordinate Directions
    H(21, 0) = nodeExistence( 7) * 0.125 * (1 + r1) * (1 - r2) * (1 - r3) - 0.5 * ( H(42, 0) + H(45, 0) + H(57, 0) ) - 0.25 * ( H(69, 0) + H(72, 0) + H(78, 0) ) - 0.125 * H(60, 0);
    H(22, 1) = H(21, 0);
    H(23, 2) = H(21, 0);



    return H;
}

//#############################################################################
//***************************************************************
const DTensor1 &BRICK_TYPE::interp_poli_at( double r1, double r2, double r3 ) const
{
    static DTensor1 h( 27, 0.0 );

    //Shape Functions of Node 1 Along Three Coordinate Directions
    h(0) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 2 Along Three Coordinate Directions
    h(1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 3 Along Three Coordinate Directions
    h(2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 4 Along Three Coordinate Directions
    h(3) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 5 Along Three Coordinate Directions
    h(4) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    //Shape Functions of Node 6 Along Three Coordinate Directions
    h(5) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    //Shape Functions of Node 7 Along Three Coordinate Directions
    h(6) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    //Shape Functions of Node 8 Along Three Coordinate Directions
    h(7) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    //Shape Functions of Node 9 Along Three Coordinate Directions
    h(8) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 10 Along Three Coordinate Directions
    h(9) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 11 Along Three Coordinate Directions
    h(10) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 12 Along Three Coordinate Directions
    h(11) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 13 Along Three Coordinate Directions
    h(12) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    //Shape Functions of Node 14 Along Three Coordinate Directions
    h(13) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    //Shape Functions of Node 15 Along Three Coordinate Directions
    h(14) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    //Shape Functions of Node 16 Along Three Coordinate Directions
    h(15) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    //Shape Functions of Node 17 Along Three Coordinate Directions
    h(16) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    //Shape Functions of Node 18 Along Three Coordinate Directions
    h(17) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    //Shape Functions of Node 19 Along Three Coordinate Directions
    h(18) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    //Shape Functions of Node 20 Along Three Coordinate Directions
    h(19) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    //Shape Functions of Node 21 Along Three Coordinate Directions
    h(20) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    //Shape Functions of Node 22 Along Three Coordinate Directions
    h(21) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    //Shape Functions of Node 23 Along Three Coordinate Directions
    h(22) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    //Shape Functions of Node 24 Along Three Coordinate Directions
    h(23) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    //Shape Functions of Node 25 Along Three Coordinate Directions
    h(24) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    //Shape Functions of Node 26 Along Three Coordinate Directions
    h(25) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    //Shape Functions of Node 27 Along Three Coordinate Directions
    h(26) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);

    return h;

}

const DTensor2 &BRICK_TYPE::dh_drst_at( double r1, double r2, double r3 ) const
{
    static DTensor2 dh( 27, 3, 0.0 );

    // Node 21
    dh(20, 0) = nodeExistence(20) * (-2.0 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(20, 1) = nodeExistence(20) * (1.0 - r1 * r1) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(20, 2) = nodeExistence(20) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 22
    dh(21, 0) = nodeExistence(21) * (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh(21, 1) = nodeExistence(21) * (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh(21, 2) = nodeExistence(21) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    // Node 23
    dh(22, 0) = nodeExistence(22) * 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(22, 1) = nodeExistence(22) * 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(22, 2) = nodeExistence(22) * 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 24
    dh(23, 0) = nodeExistence(23) * (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh(23, 1) = nodeExistence(23) * (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh(23, 2) = nodeExistence(23) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    // Node 25
    dh(24, 0) = nodeExistence(24) * 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(24, 1) = nodeExistence(24) * 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(24, 2) = nodeExistence(24) * 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 26
    dh(25, 0) = nodeExistence(25) * (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(25, 1) = nodeExistence(25) * (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(25, 2) = nodeExistence(25) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    // Node 27
    dh(26, 0) = nodeExistence(26) * (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(26, 1) = nodeExistence(26) * (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(26, 2) = nodeExistence(26) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);


    // Node 20
    dh(19, 0) = nodeExistence(19) * ( 0.25 * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (dh(23, 0) + dh(24, 0)) - 0.25 * dh(20, 0) );
    dh(19, 1) = nodeExistence(19) * ( 0.25 * (-1.0) * (1.0 + r1) * (1.0 - r3 * r3) - 0.5 * (dh(23, 1) + dh(24, 1)) - 0.25 * dh(20, 1) );
    dh(19, 2) = nodeExistence(19) * ( 0.25 * (1.0 + r1) * (1.0 - r2) * (-2.0 * r3) - 0.5 * (dh(23, 2) + dh(24, 2)) - 0.25 * dh(20, 2) );


    // Node 19
    dh(18, 0) = nodeExistence(18) * ( 0.25 * (-1.0) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (dh(22, 0) + dh(23, 0)) - 0.25 * dh(20, 0) );
    dh(18, 1) = nodeExistence(18) * ( 0.25 * (1.0 - r1) * (-1.0) * (1.0 - r3 * r3) - 0.5 * (dh(22, 1) + dh(23, 1)) - 0.25 * dh(20, 1) );
    dh(18, 2) = nodeExistence(18) * ( 0.25 * (1.0 - r1) * (1.0 - r2) * (-2.0 * r3) - 0.5 * (dh(22, 2) + dh(23, 2)) - 0.25 * dh(20, 2) );


    // Node 18
    dh(17, 0) = nodeExistence(17) * ( 0.25 * (-1.0) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (dh(21, 0) + dh(22, 0)) - 0.25 * dh(20, 0) );
    dh(17, 1) = nodeExistence(17) * ( 0.25 * (1.0 - r1) * (1.0 - r3 * r3) - 0.5 * (dh(21, 1) + dh(22, 1)) - 0.25 * dh(20, 1) );
    dh(17, 2) = nodeExistence(17) * ( 0.25 * (1.0 - r1) * (1.0 + r2) * (-2.0 * r3) - 0.5 * (dh(21, 2) + dh(22, 2)) - 0.25 * dh(20, 2) );


    // Node 17
    dh(16, 0) = nodeExistence(16) * ( 0.25 * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (dh(21, 0) + dh(24, 0)) - 0.25 * dh(20, 0) );
    dh(16, 1) = nodeExistence(16) * ( 0.25 * (1.0 + r1) * (1.0 - r3 * r3) - 0.5 * (dh(21, 1) + dh(24, 1)) - 0.25 * dh(20, 1) );
    dh(16, 2) = nodeExistence(16) * ( 0.25 * (1.0 + r1) * (1.0 + r2) * (-2.0 * r3) - 0.5 * (dh(21, 2) + dh(24, 2)) - 0.25 * dh(20, 2) );


    // Node 16
    dh(15, 0) = nodeExistence(15) * ( 0.25 * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (dh(24, 0) + dh(26, 0)) - 0.25 * dh(20, 0) );
    dh(15, 1) = nodeExistence(15) * ( 0.25 * (1.0 + r1) * (-2.0 * r2) * (1.0 - r3) - 0.5 * (dh(24, 1) + dh(26, 1)) - 0.25 * dh(20, 1) );
    dh(15, 2) = nodeExistence(15) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (-1.0) - 0.5 * (dh(24, 2) + dh(26, 2)) - 0.25 * dh(20, 2) );


    // Node 15
    dh(14, 0) = nodeExistence(14) * ( 0.25 * (-2.0 * r1) * (1.0 - r2) * (1.0 - r3) - 0.5 * (dh(23, 0) + dh(26, 0)) - 0.25 * dh(20, 0) );
    dh(14, 1) = nodeExistence(14) * ( 0.25 * (1.0 - r1 * r1) * (-1.0) * (1.0 - r3) - 0.5 * (dh(23, 1) + dh(26, 1)) - 0.25 * dh(20, 1) );
    dh(14, 2) = nodeExistence(14) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (-1.0) - 0.5 * (dh(23, 2) + dh(26, 2)) - 0.25 * dh(20, 2) );



    // Node 14
    dh(13, 0) = nodeExistence(13) * ( 0.25 * (-1.0) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (dh(22, 0) + dh(26, 0)) - 0.25 * dh(20, 0) );
    dh(13, 1) = nodeExistence(13) * ( 0.25 * (1.0 - r1) * (-2.0 * r2) * (1.0 - r3) - 0.5 * (dh(22, 1) + dh(26, 1)) - 0.25 * dh(20, 1) );
    dh(13, 2) = nodeExistence(13) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (-1.0) - 0.5 * (dh(22, 2) + dh(26, 2)) - 0.25 * dh(20, 2) );


    // Node 13
    dh(12, 0) = nodeExistence(12) * ( 0.25 * (-2.0 * r1) * (1.0 + r2) * (1.0 - r3) - 0.5 * (dh(21, 0) + dh(26, 0)) - 0.25 * dh(20, 0) );
    dh(12, 1) = nodeExistence(12) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r3) - 0.5 * (dh(21, 1) + dh(26, 1)) - 0.25 * dh(20, 1) );
    dh(12, 2) = nodeExistence(12) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (-1.0) - 0.5 * (dh(21, 2) + dh(26, 2)) - 0.25 * dh(20, 2) );


    // Node 12
    dh(11, 0) = nodeExistence(11) * ( 0.25 * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (dh(24, 0) + dh(25, 0)) - 0.25 * dh(20, 0) );
    dh(11, 1) = nodeExistence(11) * ( 0.25 * (1.0 + r1) * (-2.0 * r2) * (1.0 + r3) - 0.5 * (dh(24, 1) + dh(25, 1)) - 0.25 * dh(20, 1) );
    dh(11, 2) = nodeExistence(11) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) - 0.5 * (dh(24, 2) + dh(25, 2)) - 0.25 * dh(20, 2) );


    // Node 11
    dh(10, 0) = nodeExistence(10) * ( 0.25 * (-2.0 * r1) * (1.0 - r2) * (1.0 + r3) - 0.5 * (dh(23, 0) + dh(25, 0)) - 0.25 * dh(20, 0) );
    dh(10, 1) = nodeExistence(10) * ( 0.25 * (1.0 - r1 * r1) * (-1.0) * (1.0 + r3) - 0.5 * (dh(23, 1) + dh(25, 1)) - 0.25 * dh(20, 1) );
    dh(10, 2) = nodeExistence(10) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) - 0.5 * (dh(23, 2) + dh(25, 2)) - 0.25 * dh(20, 2) );


    // Node 10
    dh( 9, 0) = nodeExistence( 9) * ( 0.25 * (-1.0) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (dh(22, 0) + dh(25, 0)) - 0.25 * dh(20, 0) );
    dh( 9, 1) = nodeExistence( 9) * ( 0.25 * (1.0 - r1) * (-2.0 * r2) * (1.0 + r3) - 0.5 * (dh(22, 1) + dh(25, 1)) - 0.25 * dh(20, 1) );
    dh( 9, 2) = nodeExistence( 9) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) - 0.5 * (dh(22, 2) + dh(25, 2)) - 0.25 * dh(20, 2) );


    // Node 9
    dh( 8, 0) = nodeExistence( 8) * ( 0.25 * (-2.0 * r1) * (1.0 + r2) * (1.0 + r3) - 0.5 * (dh(21, 0) + dh(25, 0)) - 0.25 * dh(20, 0) );
    dh( 8, 1) = nodeExistence( 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r3) - 0.5 * (dh(21, 1) + dh(25, 1)) - 0.25 * dh(20, 1) );
    dh( 8, 2) = nodeExistence( 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) - 0.5 * (dh(21, 2) + dh(25, 2)) - 0.25 * dh(20, 2) );


    // Node 1
    dh( 0, 0) = nodeExistence( 0) * 0.125 * (1 + r2) * (1 + r3) - 0.5 * ( dh(8, 0) + dh(11, 0) + dh(16, 0) ) - 0.25 * ( dh(21, 0) + dh(24, 0) + dh(25, 0) ) - 0.125 * dh(20, 0);
    dh( 0, 1) = nodeExistence( 0) * 0.125 * (1 + r1) * (1 + r3) - 0.5 * ( dh(8, 1) + dh(11, 1) + dh(16, 1) ) - 0.25 * ( dh(21, 1) + dh(24, 1) + dh(25, 1) ) - 0.125 * dh(20, 1);
    dh( 0, 2) = nodeExistence( 0) * 0.125 * (1 + r1) * (1 + r2) - 0.5 * ( dh(8, 2) + dh(11, 2) + dh(16, 2) ) - 0.25 * ( dh(21, 2) + dh(24, 2) + dh(25, 2) ) - 0.125 * dh(20, 2);


    // Node 2
    dh(1, 0) = nodeExistence( 1) * 0.125 * (-1.0) * (1 + r2) * (1 + r3) - 0.5 * ( dh(8, 0) + dh(9, 0) + dh(17, 0) ) - 0.25 * ( dh(21, 0) + dh(22, 0) + dh(25, 0) ) - 0.125 * dh(20, 0);
    dh(1, 1) = nodeExistence( 1) * 0.125 * (1 - r1) * (1 + r3) - 0.5 * ( dh(8, 1) + dh(9, 1) + dh(17, 1) ) - 0.25 * ( dh(21, 1) + dh(22, 1) + dh(25, 1) ) - 0.125 * dh(20, 1);
    dh(1, 2) = nodeExistence( 1) * 0.125 * (1 - r1) * (1 + r2) - 0.5 * ( dh(8, 2) + dh(9, 2) + dh(17, 2) ) - 0.25 * ( dh(21, 2) + dh(22, 2) + dh(25, 2) ) - 0.125 * dh(20, 2);


    // Node 3
    dh(2, 0) = nodeExistence( 2) * 0.125 * (-1.0) * (1 - r2) * (1 + r3) - 0.5 * ( dh(9, 0) + dh(10, 0) + dh(18, 0) ) - 0.25 * ( dh(22, 0) + dh(23, 0) + dh(25, 0) ) - 0.125 * dh(20, 0);
    dh(2, 1) = nodeExistence( 2) * 0.125 * (1 - r1) * (-1.0) * (1 + r3) - 0.5 * ( dh(9, 1) + dh(10, 1) + dh(18, 1) ) - 0.25 * ( dh(22, 1) + dh(23, 1) + dh(25, 1) ) - 0.125 * dh(20, 1);
    dh(2, 2) = nodeExistence( 2) * 0.125 * (1 - r1) * (1 - r2) - 0.5 * ( dh(9, 2) + dh(10, 2) + dh(18, 2) ) - 0.25 * ( dh(22, 2) + dh(23, 2) + dh(25, 2) ) - 0.125 * dh(20, 2);


    // Node 4
    dh(3, 0) = nodeExistence( 3) * 0.125 * (1 - r2) * (1 + r3) - 0.5 * ( dh(10, 0) + dh(11, 0) + dh(19, 0) ) - 0.25 * ( dh(23, 0) + dh(24, 0) + dh(25, 0) ) - 0.125 * dh(20, 0);
    dh(3, 1) = nodeExistence( 3) * 0.125 * (1 + r1) * (-1.0) * (1 + r3) - 0.5 * ( dh(10, 1) + dh(11, 1) + dh(19, 1) ) - 0.25 * ( dh(23, 1) + dh(24, 1) + dh(25, 1) ) - 0.125 * dh(20, 1);
    dh(3, 2) = nodeExistence( 3) * 0.125 * (1 + r1) * (1 - r2) - 0.5 * ( dh(10, 2) + dh(11, 2) + dh(19, 2) ) - 0.25 * ( dh(23, 2) + dh(24, 2) + dh(25, 2) ) - 0.125 * dh(20, 2);


    // Node 5
    dh(4, 0) = nodeExistence( 4) * 0.125 * (1 + r2) * (1 - r3) - 0.5 * ( dh(12, 0) + dh(15, 0) + dh(16, 0) ) - 0.25 * ( dh(21, 0) + dh(24, 0) + dh(26, 0) ) - 0.125 * dh(20, 0);
    dh(4, 1) = nodeExistence( 4) * 0.125 * (1 + r1) * (1 - r3) - 0.5 * ( dh(12, 1) + dh(15, 1) + dh(16, 1) ) - 0.25 * ( dh(21, 1) + dh(24, 1) + dh(26, 1) ) - 0.125 * dh(20, 1);
    dh(4, 2) = nodeExistence( 4) * 0.125 * (1 + r1) * (1 + r2) * (-1.0) - 0.5 * ( dh(12, 2) + dh(15, 2) + dh(16, 2) ) - 0.25 * ( dh(21, 2) + dh(24, 2) + dh(26, 2) ) - 0.125 * dh(20, 2);


    // Node 6
    dh(5, 0) = nodeExistence( 5) * 0.125 * (-1.0) * (1 + r2) * (1 - r3) - 0.5 * ( dh(12, 0) + dh(13, 0) + dh(17, 0) ) - 0.25 * ( dh(21, 0) + dh(22, 0) + dh(26, 0) ) - 0.125 * dh(20, 0);
    dh(5, 1) = nodeExistence( 5) * 0.125 * (1 - r1) * (1 - r3) - 0.5 * ( dh(12, 1) + dh(13, 1) + dh(17, 1) ) - 0.25 * ( dh(21, 1) + dh(22, 1) + dh(26, 1) ) - 0.125 * dh(20, 1);
    dh(5, 2) = nodeExistence( 5) * 0.125 * (1 - r1) * (1 + r2) * (-1.0) - 0.5 * ( dh(12, 2) + dh(13, 2) + dh(17, 2) ) - 0.25 * ( dh(21, 2) + dh(22, 2) + dh(26, 2) ) - 0.125 * dh(20, 2);


    // Node 7
    dh(6, 0) = nodeExistence( 6) * 0.125 * (-1.0) * (1 - r2) * (1 - r3) - 0.5 * ( dh(13, 0) + dh(14, 0) + dh(18, 0) ) - 0.25 * ( dh(22, 0) + dh(23, 0) + dh(26, 0) ) - 0.125 * dh(20, 0);
    dh(6, 1) = nodeExistence( 6) * 0.125 * (1 - r1) * (-1.0) * (1 - r3) - 0.5 * ( dh(13, 1) + dh(14, 1) + dh(18, 1) ) - 0.25 * ( dh(22, 1) + dh(23, 1) + dh(26, 1) ) - 0.125 * dh(20, 1);
    dh(6, 2) = nodeExistence( 6) * 0.125 * (1 - r1) * (1 - r2) * (-1.0) - 0.5 * ( dh(13, 2) + dh(14, 2) + dh(18, 2) ) - 0.25 * ( dh(22, 2) + dh(23, 2) + dh(26, 2) ) - 0.125 * dh(20, 2);


    // Node 8
    dh(7, 0) = nodeExistence( 7) * 0.125 * (1 - r2) * (1 - r3) - 0.5 * ( dh(14, 0) + dh(15, 0) + dh(19, 0) ) - 0.25 * ( dh(23, 0) + dh(24, 0) + dh(26, 0) ) - 0.125 * dh(20, 0);
    dh(7, 1) = nodeExistence( 7) * 0.125 * (1 + r1) * (-1.0) * (1 - r3) - 0.5 * ( dh(14, 1) + dh(15, 1) + dh(19, 1) ) - 0.25 * ( dh(23, 1) + dh(24, 1) + dh(26, 1) ) - 0.125 * dh(20, 1);
    dh(7, 2) = nodeExistence( 7) * 0.125 * (1 + r1) * (1 - r2) * (-1.0) - 0.5 * ( dh(14, 2) + dh(15, 2) + dh(19, 2) ) - 0.25 * ( dh(23, 2) + dh(24, 2) + dh(26, 2) ) - 0.125 * dh(20, 2);


    return dh;
}

////#############################################################################
const DTensor2 &BRICK_TYPE::Jacobian_3D( const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    static DTensor2 Jacobian_3D_(3, 3, 0.0);
    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    return Jacobian_3D_;
}

//#############################################################################
const DTensor2  &BRICK_TYPE::Jacobian_3Dinv( const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    DTensor2 Jacobian_3D_(3, 3, 0.0);
    static DTensor2 Jacobian_3D_inv(3, 3, 0.0);

    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    Jacobian_3D_inv = Jacobian_3D_.Inv();
    return Jacobian_3D_inv;
}


////#############################################################################
const DTensor2 &BRICK_TYPE::Nodal_Coordinates( void ) const
{
    static DTensor2 N_coord( 27, 3, 0.0 );

    for ( int i = 0 ; i < 27 ; i++)
    {
        if (theNodes[i] != 0)
        {
            N_coord(i, 0) = ( theNodes[i]->getCrds() )(0);
            N_coord(i, 1) = ( theNodes[i]->getCrds() )(1);
            N_coord(i, 2) = ( theNodes[i]->getCrds() )(2);
        }
        else
        {
            N_coord(i, 0) = 0;
            N_coord(i, 1) = 0;
            N_coord(i, 2) = 0;
        }
    }

    return N_coord;
}

////#############################################################################
const DTensor2 &BRICK_TYPE::incr_disp( void ) const
{
    static DTensor2 increment_disp( 27, 3, 0.0 );

    for ( int i = 0 ; i < 27 ; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            increment_disp(i,0) = ( theNodes[i]->getIncrDeltaDisp() )(0);
            increment_disp(i,1) = ( theNodes[i]->getIncrDeltaDisp() )(1);
            increment_disp(i,2) = ( theNodes[i]->getIncrDeltaDisp() )(2);
        }
        else
        {
            increment_disp(i,0) = 0;
            increment_disp(i,1) = 0;
            increment_disp(i,2) = 0;
        }
    }

    return increment_disp;
}

////#############################################################################
const DTensor2 &BRICK_TYPE::total_disp( void ) const
{
    static DTensor2 total_disp( 27, 3, 0.0 );

    for ( int i = 0 ; i < 27 ; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            total_disp(i,0) = ( theNodes[i]->getTrialDisp() )(0);
            total_disp(i,1) = ( theNodes[i]->getTrialDisp() )(1);
            total_disp(i,2) = ( theNodes[i]->getTrialDisp() )(2);
        }
        else
        {
            total_disp(i,0) = 0;
            total_disp(i,1) = 0;
            total_disp(i,2) = 0;
        }
    }

    return total_disp;
}



////#############################################################################
int BRICK_TYPE::get_global_number_of_node( int local_node_number )
{
    return connectedExternalNodes( local_node_number );
}

////#############################################################################
int  BRICK_TYPE::get_Brick_Number( void )
{
    return this->getTag();
}



////#############################################################################
// returns nodal forces for given stress field in an element
const Vector &BRICK_TYPE::nodal_forces( void )
{
    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor2 nodal_forces( 27, 3, 0.0 );
    static DTensor2 dh( 27, 3 , 0.0 );
    static DTensor2 stress_at_GP( 3, 3, 0.0 );
    static DTensor2 Jacobian( 3, 3, 0.0 );
    static DTensor2 JacobianINV( 3, 3, 0.0 );
    static DTensor2 dhGlobal( 27, 3 , 0.0 );

    nodal_forces *= 0;
    dh *= 0;
    stress_at_GP *= 0;
    Jacobian *= 0;
    JacobianINV *= 0;
    dhGlobal *= 0;


    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp, 0);
        w_s = gp_weight(gp, 1);
        w_t = gp_weight(gp, 2);

        // derivatives of local coordiantes with respect to local coordiantes
        dh = dh_drst_at( r, s, t );

        // Jacobian tensor ( matrix )
        //Jacobian = Jacobian_3D( dh );

        // Inverse of Jacobian tensor ( matrix )
        JacobianINV = Jacobian_3Dinv( dh );

        // determinant of Jacobian tensor ( matrix )
        det_of_Jacobian  = 1.0 / JacobianINV.compute_Determinant();

        // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
        dhGlobal(i, k) = dh(i, j) * JacobianINV(k, j);

        //weight
        weight = w_r * w_s * w_t * det_of_Jacobian;
        stress_at_GP = material_array[gp]->getStressTensor();

        nodal_forces(i, j) += weight * (dhGlobal( i, k ) * stress_at_GP( j, k ) );

    }

    int a=0;
    //converting nodalforce tensor to vector
    for ( int i = 0; i < 27; i++ )
    {
        if(connectedExternalNodes(i) != 0)
        {  
	        P( a * 3 + 0 ) = nodal_forces( i, 0 );
	        P( a * 3 + 1 ) = nodal_forces( i, 1 );
	        P( a * 3 + 2 ) = nodal_forces( i, 2 );
    	}

    	a++;
    }


    return P;
}



//Compute gauss point coordinates
void BRICK_TYPE::computeGaussPoint()
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    // special case for 27 nodes only
    DTensor2 NodalCoord( 3, nodes_in_brick, 0.0 );
    DTensor2 material_arrayCoord( 3, Num_TotalGaussPts, 0.0 );
    DTensor2 H( 81, 3, 0.0 );

    int a =0;
    for ( int i = 0 ; i < 27 ; i++)
    {
    	if(connectedExternalNodes(i)!=0)
    	{
	        NodalCoord(0,a) = ( theNodes[i]->getCrds() )(0);
	        NodalCoord(1,a) = ( theNodes[i]->getCrds() )(1);
	        NodalCoord(2,a) = ( theNodes[i]->getCrds() )(2);
            a++;
	    }
    }


    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);

        H = H_3D( r, s, t );

        int i=0;

        for ( int encount = 0 ; encount < 27 ; encount++ )
        {
            if(connectedExternalNodes(i)!=0)
            {
                material_arrayCoord( 0, gp ) += NodalCoord( 0, i ) * H( encount * 3 + 0, 0 );
                material_arrayCoord( 1, gp ) += NodalCoord( 1, i ) * H( encount * 3 + 1, 1 );
                material_arrayCoord( 2, gp ) += NodalCoord( 2, i ) * H( encount * 3 + 2, 2 );
                i++;
            }
        }

        gauss_points( gp, 0 ) = material_arrayCoord( 0, gp );
        gauss_points( gp, 1 ) = material_arrayCoord( 1, gp );
        gauss_points( gp, 2 ) = material_arrayCoord( 2, gp );

    }
}


////#############################################################################


//=============================================================================
int BRICK_TYPE::getNumExternalNodes () const
{
    return nodes_in_brick;
}


//=============================================================================
const ID &BRICK_TYPE::getExternalNodes ()
{
    
	ExternalNodes = ID(nodes_in_brick);
	int a =0;

	for(int i=0; i<27; i++)
	{
		if(connectedExternalNodes(i)!=0)
		{
			ExternalNodes(a++) = connectedExternalNodes(i);
		}
	}

    return ExternalNodes;
}

Node **
BRICK_TYPE::getNodePtrs( void )
{
    return theNodes;
}

//=============================================================================
int BRICK_TYPE::getNumDOF ()
{
    return nodes_in_brick*3;
}

//=============================================================================
void BRICK_TYPE::setDomain ( Domain *theDomain )
{
    // Check Domain is not null - invoked when object removed from a domain
    if ( theDomain == 0 )
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
        theNodes[2] = 0;
        theNodes[3] = 0;
        theNodes[4] = 0;
        theNodes[5] = 0;
        theNodes[6] = 0;
        theNodes[7] = 0;
        theNodes[9] = 0;
        theNodes[10] = 0;
        theNodes[11] = 0;
        theNodes[12] = 0;
        theNodes[13] = 0;
        theNodes[14] = 0;
        theNodes[15] = 0;
        theNodes[16] = 0;
        theNodes[17] = 0;
        theNodes[18] = 0;
        theNodes[19] = 0;
        theNodes[20] = 0;
        theNodes[21] = 0;
        theNodes[22] = 0;
        theNodes[23] = 0;
        theNodes[24] = 0;
        theNodes[25] = 0;
        theNodes[26] = 0;
    }
    else
    {


        for (int i = 0; i < 27; i++)
        {

            if (connectedExternalNodes(i) != 0)
            {
            	theNodes[i] = theDomain->getNode( connectedExternalNodes(i) );

# ifdef _PARALLEL_PROCESSING
            	if ( theNodes[i] == 0 )
            	{
                theNodes[i] = theDomain->getOutsideNode( connectedExternalNodes(i) );
            	}
# endif
                if (theNodes[i] == 0)
                {
                     cerr << BRICK_ELEMENT_NAME << ":: tag: " << this->getTag() << " ) does not have node " << connectedExternalNodes(i) << "\n";
                    exit(-1);
                }
            }

        }

        for (int i = 0; i < 27; i++)
        {
            if (connectedExternalNodes(i) != 0)
            {
                if (theNodes[i]->getNumberDOF() != 3)
                {
                     cerr << BRICK_ELEMENT_NAME << ":: tag: " << this->getTag() << ") has differing number of DOFs at its nodes\n";
                    exit(-1);
                }
            }
        }

        this->DomainComponent::setDomain( theDomain );
    }

}

//=============================================================================
int BRICK_TYPE::commitState ()
{

    int retVal = 0;

    // call element commitState to do any base class stuff
    if ( ( retVal = this->Element::commitState() ) != 0 )
    {
        cerr << BRICK_ELEMENT_NAME << "::commitState () - failed in base class";
    }

    Vector pp = getResistingForce();

    for (int ii = 0; ii < Num_TotalGaussPts; ii++ )
    {
        retVal += material_array[ii]->commitState();
    }

    return retVal;
}

//=============================================================================
int BRICK_TYPE::revertToLastCommit ()
{
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states

    for (int gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        retVal += material_array[gp]->revertToLastCommit();
    }

    return retVal;
}

//=============================================================================
int BRICK_TYPE::revertToStart ()
{
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states

    for (int gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        retVal += material_array[gp]->revertToStart();
    }

    return retVal;
}


//=============================================================================
const Matrix &BRICK_TYPE::getTangentStiff()
{

    /*** Sumeet [September, 2016] check for elastic material [see Element.cpp]*****/
    /* -1 means not an elastic material */
    /*  0 means elastic material but stiffness has not been calculated yet*/
    /*  1 means elastic material and stiffness has been calculated */
    switch(getElasticMaterialStatus()){

        case -1: // not an elastic material
                break;
        case  1: // elstic material and stiffness has allready been calculated 
                return K_Elastiic;
    }


    DTensor4 stifftensor(27, 3, 3, 27, 0.0);

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor2 dh_drst( 27, 3, 0.0 );
    static DTensor2 dhGlobal( 27, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);
    static DTensor2 JacobianINV(3, 3, 0.0);
    static DTensor4 E_elpl(3, 3, 3, 3, 0.0);
    static DTensor4 Kkt( 27, 3, 3, 27, 0.0);

    dh_drst *= 0;
    dhGlobal *= 0;
    Jacobian *= 0;
    JacobianINV *= 0;
    E_elpl *= 0;
    Kkt *= 0;

    Index < 'a' > a;
    Index < 'b' > b;
    Index < 'c' > c;
    Index < 'd' > d;


    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp, 0);
        w_s = gp_weight(gp, 1);
        w_t = gp_weight(gp, 2);

        // derivatives of local coordinates with respect to local coordinates
        dh_drst = dh_drst_at( r, s, t );
        Jacobian = Jacobian_3D( dh_drst );
        JacobianINV = Jacobian.Inv();
        det_of_Jacobian  = Jacobian.compute_Determinant();


        // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
        dhGlobal(i, k) = dh_drst( i, j ) * JacobianINV( k, j );
        weight = w_r * w_s * w_t * det_of_Jacobian;
        E_elpl = material_array[gp]->getTangentTensor();

        // This is the actual integration
        // FIXME: This can be more efficient if these lines are merged into one (discard the temporary KKt tensor)
        Kkt(i, a, c, j) = dhGlobal(i, b) * E_elpl(a, b, c, d) * dhGlobal(j, d) * weight;
        stifftensor(i, a, c, j) = stifftensor(i, a, c, j) + Kkt(i, a, c, j);


    }


    int p = 0;
    int q = 0;
    for(int i=0; i<27; i++)
    {
    	if(connectedExternalNodes(i) != 0)
    	{
    		q =0;
	    	for(int j=0; j<27;j++)
	    	{
		    	if(connectedExternalNodes(j) != 0)
		    	{
		    		for(int k=0; k<3; k++)
		    		{
		    			for(int l=0; l<3; l++)
		    			{
		    				K(p*3+k, q*3+l) = stifftensor(i, k, l, j);
		    			}
		    		}
                    q++;
		    	}
	    	}
	       	p++;
	    }
    }


    /*** Sumeet [September, 2016] check for elastic material [see Element.cpp]*****/
    /* -1 means not an elastic material */
    /*  0 means elastic material but stiffness has not been calculated yet*/
    /*  1 means elastic material and stiffness has been calculated */
    if(getElasticMaterialStatus()==0){
        K_Elastiic = K;
        setElasticMaterialStatus(1);
    }

    return K;
}

//=============================================================================
const Matrix &BRICK_TYPE::getInitialStiff ()
{

    if ( Ki == 0 )
    {
        Ki = new Matrix( this->getTangentStiff() );
    }

    if ( Ki == 0 )
    {
        cerr << BRICK_ELEMENT_NAME << "::getInitialStiff() -";
        cerr << "ran out of memory\n";
        exit( -1 );
    }

    return *Ki;
}

//=============================================================================

const Matrix &BRICK_TYPE::getMass ()
{

    if (not is_mass_computed)
    {
        double r   = 0.0;
        double s   = 0.0;
        double t   = 0.0;
        double w_r = 0.0;
        double w_s = 0.0;
        double w_t = 0.0;
        double weight = 0.0;
        double det_of_Jacobian = 0.0;

        DTensor2 dh(27, 3, 0.0 );
        DTensor2 H(81, 3, 0.0 );
        DTensor2 Mm(81,81, 0.0 );
        DTensor2 Jacobian(3, 3);

        for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
        {
            r = gp_coords(gp, 0);
            s = gp_coords(gp, 1);
            t = gp_coords(gp, 2);
            w_r = gp_weight(gp, 0);
            w_s = gp_weight(gp, 1);
            w_t = gp_weight(gp, 2);

            dh = dh_drst_at( r, s, t );
            Jacobian = Jacobian_3D( dh );
            det_of_Jacobian  = Jacobian.compute_Determinant();
            H = H_3D( r, s, t );

            weight = w_r * w_s * w_t * rho * det_of_Jacobian;
            Mm(i, j) = Mm(i, j) + H(i, k) * H(j, k) * weight;
        }

	    int a = 0;
	    int b = 0;
	    for(int i=0; i<27; i++)
	    {
	    	if(connectedExternalNodes(i) != 0)
	    	{
	    		b =0;
		    	for(int j=0; j<27;j++)
		    	{
			    	if(connectedExternalNodes(j) != 0)
			    	{
			    		for(int k=0; k<3; k++)
			    		{
			    			for(int l=0; l<3; l++)
			    			{
			    				M(a*3+k, b*3+l) = Mm(i*3+k, j*3+l);
			    			}
			    		}
                        b++;
			    	}
		    	}

		       	a++;
		    }
	    }

        is_mass_computed = true;
    }

    return M;
}

//=============================================================================
void BRICK_TYPE::zeroLoad( void )
{
    Q.Zero();
}



//======================================================================
const Vector &BRICK_TYPE::getBodyForce( double loadFactor, const Vector &data )
{
    P.Zero();

    // Check for a quick return
    if ( rho == 0.0 )
    {
        return P;
    }

    Vector ba( 81 ), bfx( 3 );

    bfx( 0 ) = data( 0 ) * loadFactor;
    bfx( 1 ) = data( 1 ) * loadFactor;
    bfx( 2 ) = data( 2 ) * loadFactor;

    int n = 0;

    for ( int i = 0 ; i < 27; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            ba(n * 3 + 0) = bfx(0);
            ba(n * 3 + 1) = bfx(1);
            ba(n * 3 + 2) = bfx(2);

            n++;

        }

    }


    P.addMatrixVector( 0.0, this->getMass(), ba, 1.0 );

    return P;
}


//======================================================================
const Vector &BRICK_TYPE::getSurfaceForce( double loadFactor, const Vector &data )
{

    map<int,int> local_nodes_map; int local_nodes[9];
    ID Surface_nodeExistence(9);
    int nodes_on_surface =0;
    
    //note -> the user at least should start with a node on the corner edge (sumeet)
    /////////////////////////////////////////// Edited by Sumeet 30/03/2016 //////////////////////////////
    // checking if node exists in the element
    for ( int i =0; i<9 ;i++){
        if(data(i)!=0){
            nodes_on_surface ++;
            std::map<int,int>::iterator it;
            it=Global_to_Local_Node_Mapping.find(data(i));
            if (it == Global_to_Local_Node_Mapping.end()){
                cerr << "\n" << BRICK_ELEMENT_NAME << "::getSurfaceForce  Node " <<  data(i) << " defined for the BrickSurfaceLoad does not belong to element \n" ;
                exit( 1 );
            }
            local_nodes_map[it->second]=i;
            local_nodes[i]=it->second;
        }
        else{
            local_nodes[i]=-1;
            local_nodes_map[-1]=-1;
        }
        Surface_nodeExistence(i) =0;
    }
    // //////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    // cout << "Surface_nodeExistence(i) " ;
    // for(int i =0 ; i<9 ;i++ ){
    //     cout << Surface_nodeExistence(i) << " ";
    // }
    // cout << endl <<  "local_nodes[i] " ;
    // for(int i =0 ; i<9 ;i++ ){
    //     cout << local_nodes[i] << " ";
    // }
    // cout << endl << "local_nodes_map[it->second] ";
    // for(int i =0 ; i<9 ;i++ ){
    //     cout << local_nodes_map[local_nodes[i]] << " ";

    // }
    // cout << endl ;
    // //////////////////////////// For Debugging By Sumeet //////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    static const int Node_to_Surface_Number[27][4]={{3,0,2,4},{3,0,2,5},{3,0,3,5},{3,0,3,4},
                                                    {3,1,2,4},{3,1,2,5},{3,1,3,5},{3,1,3,4},
                                                    {2,0,2,0},{2,0,5,0},{2,0,3,0},{2,0,4,0},
                                                    {2,1,2,0},{2,1,5,0},{2,1,3,0},{2,1,4,0},
                                                    {0,0,0,0},{1,2,0,0},{1,5,0,0},{1,3,0,0},
                                                    {1,4,0,0},{1,0,0,0},{1,1,0,0}};
    static const int Surface_Number_to_Node_order[6][9]={{1,2,3,0,9 ,10,11,8 ,25},
                                                         {5,4,7,6,12,15,14,13,26},
                                                         {5,1,0,4,17,8 ,16,12,21},
                                                         {2,6,7,3,18,14,19,10,23},
                                                         {0,3,7,4,11,19,15,16,24},
                                                         {1,5,6,2,17,13,18,9 ,22}};                                            
    ////////////////////////////////////// Edited by Sumeet 30/3/2016 /////////////////////////////////////
    //  Finding the correct surface nodes order
    int success =0; int surface_nodes_order[9]={0,0,0,0,0,0,0,0,0};
    int number_of_surfaces = Node_to_Surface_Number[local_nodes[0]][0];
 
    for ( int i =0; i<number_of_surfaces ;i++){
        int surface_no = Node_to_Surface_Number[local_nodes[0]][i+1];
        for( int j=0; j<9;j++){
            std::map<int,int>::iterator it;
            int node=Surface_Number_to_Node_order[surface_no][j]; 
            it=local_nodes_map.find(node);
            if (it == local_nodes_map.end()){
                break;
            }
            success=success+1;
            surface_nodes_order[j]= it->second;
        }
        if(success==nodes_on_surface) break;
    }
    if (success < nodes_on_surface or success<4){
        cerr << "\n" << BRICK_ELEMENT_NAME << "::getSurfaceForce  Nodes  defined for the BrickSurfaceLoad does not belong to elements surface  \n" ;
        exit( 1 );
    }      

    for(int i=0; i<9;i++)
    {
        if(surface_nodes_order[i]==-1)
        {
            surface_nodes_order[i] = 0; 
        }
        else{
            Surface_nodeExistence((surface_nodes_order[i]))=1;
        }

    }

    //////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    cout << endl <<  "local_nodes[i] " ;
    for(int i =0 ; i<9 ;i++ ){
        cout << local_nodes[i] << " ";
    }
    cout << endl << "surface_nodes_order(i) " ;
    for(int i =0 ; i<9 ;i++ ){
        cout << surface_nodes_order[i] << " ";
    }
    cout << endl << "local_nodes[surface_nodes_order[i]] " ;
    for ( int i =0; i < 9 ; i++){
        cout << local_nodes[surface_nodes_order[i]] << " ";
    }
    cout << endl << "Surface_nodeExistence " ;
    for ( int i =0; i < 9 ; i++)
        cout << Surface_nodeExistence(i) << " ";
    cout << "\n";
    ////////////////////////////////////////////////////////////////////////////////////////////////


    // get the surface nodal coordinates
    const Vector &coordnode1 = theNodes[local_nodes[(surface_nodes_order[0])]]->getCrds();
    const Vector &coordnode2 = theNodes[local_nodes[(surface_nodes_order[1])]]->getCrds();
    const Vector &coordnode3 = theNodes[local_nodes[(surface_nodes_order[2])]]->getCrds();
    const Vector &coordnode4 = theNodes[local_nodes[(surface_nodes_order[3])]]->getCrds();
    const Vector &coordnode5 = theNodes[local_nodes[(surface_nodes_order[4])]]->getCrds();
    const Vector &coordnode6 = theNodes[local_nodes[(surface_nodes_order[5])]]->getCrds();
    const Vector &coordnode7 = theNodes[local_nodes[(surface_nodes_order[6])]]->getCrds();
    const Vector &coordnode8 = theNodes[local_nodes[(surface_nodes_order[7])]]->getCrds();
    const Vector &coordnode9 = theNodes[local_nodes[(surface_nodes_order[8])]]->getCrds();

    static Vector NodalForces(81);
    for (int m = 0; m < 81; m++){
        NodalForces(m) = 0;
    }

    double ShapeFunctionValues;
    double LoadValue;
    Vector J_vector( 3 );
    Vector Pressure( 9 );

    Pressure(0) = data(surface_nodes_order[0]+9) * loadFactor;
    Pressure(1) = data(surface_nodes_order[1]+9) * loadFactor;
    Pressure(2) = data(surface_nodes_order[2]+9) * loadFactor;
    Pressure(3) = data(surface_nodes_order[3]+9) * loadFactor;
    Pressure(4) = data(surface_nodes_order[4]+9) * loadFactor;
    Pressure(5) = data(surface_nodes_order[5]+9) * loadFactor;
    Pressure(6) = data(surface_nodes_order[6]+9) * loadFactor;
    Pressure(7) = data(surface_nodes_order[7]+9) * loadFactor;
    Pressure(8) = data(surface_nodes_order[8]+9) * loadFactor;

    // //////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    // cout << endl << "Pressure[i]] " ;
    // for ( int i =0; i < 9 ; i++)
    //     cout << Pressure(i) << " ";
    // cout << "\n";

    // P.Zero();
    // ////////////////////////////////////////////////////////////////////////////////////////////////


    //GP coordinates and weights for surface. ====================================================
    // This initializes class wide members gp_coords and gp_weights .
    // Since LTensor does not provide initializer lists, this is the only way to
    // do this.

    Matrix Gauss_coordinates = get_Gauss_p_c();
    Matrix Gauss_weights = get_Gauss_p_w();
    Matrix GsPts(Order*Order, 2);
    Matrix Weight(Order*Order, 2);

    short where = 0;
    for ( short ii = 1 ; ii <=Order ; ii++ )
    {
        for ( short jj = 1 ; jj <=Order ; jj++ )
        {
            GsPts(where, 0)  =Gauss_coordinates(Order,ii);
            Weight(where, 0) =Gauss_weights(Order,ii);

            GsPts(where, 1)  =Gauss_coordinates(Order,jj);
            Weight(where, 1) =Gauss_weights(Order,jj);

            where ++;
        }
    }
    // =============================================================================


    int r = 0;

    // loop over dof
    for ( int k = 0; k < 3; k++ )
    {
        // loop over nodes
        for ( int j = 0; j < 9; j++ )
        {
            if(Surface_nodeExistence((surface_nodes_order[j])!=0))
            {
                r = local_nodes[(surface_nodes_order[j])] ;
                // loop over Gauss points
                for ( int i = 0; i < Order*Order; i++ )
                {
                    ShapeFunctionValues = SurfaceShapeFunctionValues( GsPts( i, 0 ) , GsPts( i, 1 ), j, Surface_nodeExistence );
                    J_vector = Direction_Weight( GsPts( i, 0 ) , GsPts( i, 1 ), coordnode1, coordnode2, coordnode3, coordnode4, coordnode5, coordnode6, coordnode7, coordnode8, coordnode9, Surface_nodeExistence );
                    LoadValue = SurfaceLoadValues( GsPts( i, 0 ) , GsPts( i, 1 ), Pressure, Surface_nodeExistence );
                    NodalForces( r * 3 + k ) = NodalForces( r * 3 + k ) + LoadValue * J_vector( k ) * ShapeFunctionValues * Weight(i, 0) * Weight(i, 1);
                }
            }
        }
    }

    cout << NodalForces << endl;

    // ////////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    // for ( int i =0; i < 27 ; i++)
    //     cout << P(3*i)<< " " << P(3*i+1) << " " << P(3*i+2) <<   "\n";
    // cout << "\n\n******************************************************************************\n\n";
    // /////////////////////////////////////////////////////////////////////////////////////////////////


    P.Zero();

    int n = 0;
    for ( int i = 0 ; i < 27; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            P(n * 3 + 0) = NodalForces(i*3 + 0);
            P(n * 3 + 1) = NodalForces(i*3 + 1);
            P(n * 3 + 2) = NodalForces(i*3 + 2);

            n++;

        }

    }

    return P;
}

//=============================================================================
int BRICK_TYPE::addLoad( ElementalLoad *theLoad, double loadFactor )
{


    int type;
    const Vector &data = theLoad->getData( type, loadFactor );

    if ( type == LOAD_TAG_ElementSelfWeight )
    {
        Vector Fbody = this->getBodyForce( loadFactor , data);
        Q.addVector( 1.0, Fbody, 1.0 );
    }
    // else if ( type == LOAD_TAG_BrickSurfaceLoad )
    // {
    //     Vector Fsurface = this->getSurfaceForce( loadFactor, data );
    //     Q.addVector( 1.0, Fsurface, 1.0 );
    // }
    else
    {
        cerr << BRICK_ELEMENT_NAME << "::addLoad() - " << BRICK_ELEMENT_NAME << this->getTag() << " ,load type " << type << "unknown\n";
        return -1;
    }


    return 0;
}




//=============================================================================
int BRICK_TYPE::addInertiaLoadToUnbalance( const Vector &accel )
{
    // Check for a quick return
    if ( rho == 0.0 )
    {
        return 0;
    }


    Vector ra( nodes_in_brick*3 );

    int n = 0;

    for ( int i = 0 ; i < 27; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            ra(n * 3 + 0) = theNodes[i]->getRV(accel)(0);
            ra(n * 3 + 1) = theNodes[i]->getRV(accel)(1);
            ra(n * 3 + 2) = theNodes[i]->getRV(accel)(2);

            n++;

        }

    }

    //Form equivalent body force
    Q.addMatrixVector( 1.0, this->getMass(), ra, -1.0 );
    return 0;
}


//=============================================================================
const Vector &BRICK_TYPE::getResistingForce()
{
    this->nodal_forces();

    P.addVector( 1.0, Q, -1.0 );

    return P;
}

//=============================================================================
const Vector &BRICK_TYPE::getResistingForceIncInertia ()
{
    // form resisting force
    this->getResistingForce();

    //
    // now add dynamic terms
    // P += M * a + C * v
    //

    if ( rho != 0.0 )
    {


	    Vector a( nodes_in_brick*3 );

	    int n = 0;

	    for ( int i = 0 ; i < 27; i++)
	    {
	        if (connectedExternalNodes(i) != 0)
	        {
	            a(n * 3 + 0) = theNodes[i]->getTrialAccel()(0);
	            a(n * 3 + 1) = theNodes[i]->getTrialAccel()(1);
	            a(n * 3 + 2) = theNodes[i]->getTrialAccel()(2);

	            n++;

	        }

	    }

        // P += M * a
        P.addMatrixVector( 1.0, this->getMass(), a, 1.0 );

        // add the damping forces if rayleigh damping
        if ( a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0 )
        {
            P += this->getRayleighDampingForces();
        }

    }
    else
    {

        // add the damping forces if rayleigh damping
        if ( a1 != 0.0 || a2 != 0.0 || a3 != 0.0 )
        {
            P += this->getRayleighDampingForces();
        }
    }

    return P;

}




int BRICK_TYPE::sendSelf ( int commitTag, Channel &theChannel )
{

    ID idData(4);

    idData( 0 ) = this->getTag();
    idData( 1 ) = material_array[0]->getClassTag();
    idData( 2 ) = getElasticMaterialStatus();
    idData( 3 ) = is_mass_computed;

    if ( theChannel.sendID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send ID idData\n";
        return -1;
    }

    // Sending the Stiffness matrix for elastic material
    if(idData( 2 )==1){
        if ( theChannel.sendMatrix( 0, commitTag, K_Elastiic ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send its K\n";
            return -1;
        }
    }

    // Sending the Mass matrix if it is computed
    if(is_mass_computed){
        if ( theChannel.sendMatrix( 0, commitTag, M ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send its M\n";
            return -1;
        }
    }

    // send double data
    Vector floatData(5);
    floatData(0) = rho;
    floatData(1) = a0;
    floatData(2) = a1;
    floatData(3) = a2;
    floatData(4) = a3;

    if ( theChannel.sendVector( 0, commitTag, floatData ) < 0 )
    {
       cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }

    if ( theChannel.sendString( 0, commitTag, stiffness_type ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send String stiffness_type\n";
        return -1;
    }

    if ( theChannel.sendString( 0, commitTag, damping_type ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send String damping_type\n";
        return -1;
    }

    // Send the nodes
    if ( theChannel.sendID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send ID connectedExternalNodes\n";
        return -1;
    }

    // material objects send themselves
    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {
        if ( material_array[i]->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send material models\n";
            return -1;
        }
    }

    //Send Q
    if ( theChannel.sendVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send its Q\n";
        return -1;
    }

    return 0;

}

int BRICK_TYPE::receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
{

    ID idData( 4 );

    if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - failed to receive ID\n";
        return -1;
    }

    this->setTag( idData( 0 ) );
    int matClassTag = idData( 1 );
    setElasticMaterialStatus(idData(2));
    is_mass_computed = idData(3);

    // Recieve the Stiffness matrix for elastic material
    if(idData(2)==1){
        if ( theChannel.receiveMatrix( 0, commitTag, K_Elastiic ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to recieve its K\n";
            return -1;
        }
    }

    // Recieve the Mass matrix is it is allready calculated
    if(is_mass_computed){
        if ( theChannel.receiveMatrix( 0, commitTag, M ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to recieve its M\n";
            return -1;
        }
    }


    Vector floatData(5);
    if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }
    rho                     = floatData(0) ;
    a0                      = floatData(1) ;
    a1                      = floatData(2) ;
    a2                      = floatData(3) ;
    a3                      = floatData(4) ;

    if ( theChannel.receiveString( 0, commitTag,  stiffness_type) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to recieve String stiffness_type\n";
        return -1;
    }

    if ( theChannel.receiveString( 0, commitTag, damping_type ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to recieve String damping_type\n";
        return -1;
    }
 
    // Recieve the nodes
    if ( theChannel.receiveID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
        return -1;
    }

    // loop nodes 1-27 and :
    //  1) fill the nodeExistence ID
    //  2) calculate the total number of nodes
    nodes_in_brick = 0;

    for ( int i = 0 ; i < 27 ; i++ )
    {
        if (connectedExternalNodes(i) == 0 )
        {
            nodeExistence(i) = 0;
        }
        else
        {
            nodes_in_brick++;
            nodeExistence(i) = 1;
            Global_to_Local_Node_Mapping[connectedExternalNodes(i)] = i;
        }
    }

    Q = Vector(nodes_in_brick * 3);
    M = Matrix(nodes_in_brick * 3, nodes_in_brick * 3);
    P = Vector(nodes_in_brick * 3);
    K = Matrix(nodes_in_brick*3,nodes_in_brick*3);

    // For Surface Load [Sumeet, September, 2016]
    Global_to_Local_Node_Mapping[connectedExternalNodes(0)] =0;
    Global_to_Local_Node_Mapping[connectedExternalNodes(1)] =1;
    Global_to_Local_Node_Mapping[connectedExternalNodes(2)] =2;
    Global_to_Local_Node_Mapping[connectedExternalNodes(3)] =3;
    Global_to_Local_Node_Mapping[connectedExternalNodes(4)] =4;
    Global_to_Local_Node_Mapping[connectedExternalNodes(5)] =5;
    Global_to_Local_Node_Mapping[connectedExternalNodes(6)] =6;
    Global_to_Local_Node_Mapping[connectedExternalNodes(7)] =7;
    Global_to_Local_Node_Mapping[connectedExternalNodes(8)] =8;
    Global_to_Local_Node_Mapping[connectedExternalNodes(9)] =9;
    Global_to_Local_Node_Mapping[connectedExternalNodes(10)] =10;
    Global_to_Local_Node_Mapping[connectedExternalNodes(11)] =11;
    Global_to_Local_Node_Mapping[connectedExternalNodes(12)] =12;
    Global_to_Local_Node_Mapping[connectedExternalNodes(13)] =13;
    Global_to_Local_Node_Mapping[connectedExternalNodes(14)] =14;
    Global_to_Local_Node_Mapping[connectedExternalNodes(15)] =15;
    Global_to_Local_Node_Mapping[connectedExternalNodes(16)] =16;
    Global_to_Local_Node_Mapping[connectedExternalNodes(17)] =17;
    Global_to_Local_Node_Mapping[connectedExternalNodes(18)] =18;
    Global_to_Local_Node_Mapping[connectedExternalNodes(19)] =19;
    Global_to_Local_Node_Mapping[connectedExternalNodes(20)] =20;
    Global_to_Local_Node_Mapping[connectedExternalNodes(21)] =21;
    Global_to_Local_Node_Mapping[connectedExternalNodes(22)] =22;
    Global_to_Local_Node_Mapping[connectedExternalNodes(23)] =23;
    Global_to_Local_Node_Mapping[connectedExternalNodes(24)] =24;
    Global_to_Local_Node_Mapping[connectedExternalNodes(25)] =25;
    Global_to_Local_Node_Mapping[connectedExternalNodes(26)] =26;  


    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {

        // Allocate new material with the sent class tag
        NDMaterialLT *ndmat = theBroker.getNewNDMaterialLT( matClassTag );
        if ( ndmat == 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - Broker could not create NDMaterialLT of class type " << matClassTag << "\n";
            return -1;
        }

        // Now receive materials into the newly allocated space
        if ( ( ndmat )->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - material " << i << "failed to recv itself\n";
            return -1;
        }

        material_array[i] = ndmat;
    }


    // Q
    if ( theChannel.receiveVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - failed to recv Q!\n";
        return -1;
    }

    return 0;

}


int BRICK_TYPE::getObjectSize()
{

    int size = 0;
    // Just add all sizes of data-members (there is some static local variables too.... but those are not important for transference)

    size += sizeof(is_mass_computed);
    size += sizeof(Volume);
    size += sizeof(rho);
    size += sizeof(int) * connectedExternalNodes.Size();
    size += sizeof(int) * nodeExistence.Size();
    size += sizeof(int) * ExternalNodes.Size();
    size += sizeof(nodes_in_brick);
    size += sizeof(Ki);
    size += sizeof(theNodes) ;
    size += sizeof(mmodel);
    size += sizeof(material_array) * Num_TotalGaussPts;
    size += sizeof(double) * Q.Size();
    size += sizeof(double) * M.noRows()*M.noCols();
    size += sizeof(double) * K_Elastiic.noRows()*K_Elastiic.noCols();
    size += sizeof(i);
    size += sizeof(j);
    size += sizeof(k);
    size += sizeof(l);

    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {
        size += material_array[i]->getObjectSize();
    }

    return size;
}

//=============================================================================
void BRICK_TYPE::Print( ostream &s, int flag )
{
    cout<< BRICK_ELEMENT_NAME << ":: LTensor-based 8_27 node brick." << endl << endl;

    cout << "Element tag: " << getTag() << endl;
    cout << "Connected Nodes: " << endl;

    for ( int node = 0; node < 27; node++)
    {
        cout << "   #" << node + 1 << ": Domain node #" << connectedExternalNodes(node) << endl;
    }

    cout << "K = " << endl;

    int ndofs = nodes_in_brick*3;

    for (int ii = 0; ii < ndofs; ii++)
    {
        for (int jj = 0; jj < ndofs; jj++)
        {
            cout << K(ii, jj);

            if (jj < ndofs-1)
            {
                cout << ", ";
            }
            else
            {
                cout << endl;
            }
        }
    }

    cout << endl;

    cout << "diag(K) = " << endl;

    for (int ii = 0; ii < ndofs; ii++)
    {
        cout << K(ii, ii);

        if (ii < ndofs-1)
        {
            cout << ", ";
        }
        else
        {
            cout << endl;
        }
    }

    cout << endl;

    cout << "Gauss point information:" << endl;

    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
    {
        cout << "   GP # " << gp <<  ": (" << gp_coords(gp, 0) << ","
             << gp_coords(gp, 1) << ","
             << gp_coords(gp, 2) << ")" << endl;
        cout << "        State of stress " << material_array[gp]->getStressTensor() << endl;
    }

}



void BRICK_TYPE::ComputeVolume()
{
    Volume = 0.0;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    double w_r = 0.0;
    double w_s = 0.0;
    double w_t = 0.0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    static DTensor2 dh( 27, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);

    dh *= 0;
    Jacobian *= 0;

    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp, 0);
        w_s = gp_weight(gp, 1);
        w_t = gp_weight(gp, 2);

        // derivatives of local coordiantes with respect to local coordiantes
        dh = dh_drst_at( r, s, t );

        // Jacobian tensor ( matrix )
        Jacobian = Jacobian_3D( dh );

        det_of_Jacobian  = Jacobian.compute_Determinant();

        //weight
        weight = w_r * w_s * w_t * det_of_Jacobian;
        Volume += 8.0 * weight;
    }
}



int BRICK_TYPE::update( void )
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;


    static DTensor2 dh( 27, 3, 0.0 );
    static DTensor2 JacobianINV(3, 3, 0.0);
    static DTensor2 dhGlobal( 27, 3, 0.0 );
    static DTensor2 trial_disp( 27, 3, 0.0  );
    static DTensor2 total_strain(3, 3, 0.0);
    static DTensor2 trial_strain(3, 3, 0.0);


    dh *= 0;
    JacobianINV *= 0;
    dhGlobal *= 0;
    trial_disp *= 0;
    total_strain *= 0;
    trial_strain *= 0;

    trial_disp = total_disp();

    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);

        // derivatives of local coordiantes with respect to local coordiantes
        dh = dh_drst_at( r, s, t );

        // Inverse of Jacobian tensor ( matrix )
        JacobianINV = Jacobian_3Dinv( dh );

        dhGlobal(i, k) = dh(i, j) * JacobianINV(k, j);

        //trial_strain(j,k) = dhGlobal( i,j ) * trial_disp( i,k );
        trial_strain(i, j) = 0.5 * (dhGlobal( k, i ) * trial_disp( k, j ) + dhGlobal( k, j ) * trial_disp( k, i ));

        if ( ( material_array[gp]->setTrialStrain( trial_strain ) ) )
        {
             cerr << BRICK_ELEMENT_NAME << "::update (tag: " << this->getTag() << "), Update Failed\n";
            return -1;
        }
    }


    return 0;
}



/////////////////////////////////////////////////////////////////////////
//NOTE: Can me templatized to improve performance
double BRICK_TYPE::SurfaceShapeFunctionValues( double Xi , double Eta, int whichcomponent, ID Surface_nodeExistence )
{
    ShapeFunctionValues_in_function(8) = Surface_nodeExistence(8) * ((1 - Eta * Eta) * (1 - Xi * Xi));
    ShapeFunctionValues_in_function(7) = Surface_nodeExistence(7) * (0.5 * (1 - Eta * Eta) * (1 + Xi) - ShapeFunctionValues_in_function(8)/2);
    ShapeFunctionValues_in_function(6) = Surface_nodeExistence(6) * (0.5 * (1 - Xi * Xi) * (1 - Eta)  - ShapeFunctionValues_in_function(8)/2);
    ShapeFunctionValues_in_function(5) = Surface_nodeExistence(5) * (0.5 * (1 - Eta * Eta) * (1 - Xi) - ShapeFunctionValues_in_function(8)/2);
    ShapeFunctionValues_in_function(4) = Surface_nodeExistence(4) * (0.5 * (1 - Xi * Xi) * (1 + Eta)  - ShapeFunctionValues_in_function(8)/2);
    ShapeFunctionValues_in_function(3) = Surface_nodeExistence(3) * (0.25 * (1 + Xi) * (1 - Eta) - ( ShapeFunctionValues_in_function(6) + ShapeFunctionValues_in_function(7) )/2 - ShapeFunctionValues_in_function(8)/4);
    ShapeFunctionValues_in_function(2) = Surface_nodeExistence(2) * (0.25 * (1 - Xi) * (1 - Eta) - ( ShapeFunctionValues_in_function(5) + ShapeFunctionValues_in_function(6) )/2 - ShapeFunctionValues_in_function(8)/4);
    ShapeFunctionValues_in_function(1) = Surface_nodeExistence(1) * (0.25 * (1 - Xi) * (1 + Eta) - ( ShapeFunctionValues_in_function(4) + ShapeFunctionValues_in_function(5) )/2 - ShapeFunctionValues_in_function(8)/4);
    ShapeFunctionValues_in_function(0) = Surface_nodeExistence(0) * (0.25 * (1 + Xi) * (1 + Eta) - ( ShapeFunctionValues_in_function(4) + ShapeFunctionValues_in_function(7) )/2 - ShapeFunctionValues_in_function(8)/4);
    
    return ShapeFunctionValues_in_function( whichcomponent );
}


Vector &BRICK_TYPE::Direction_Weight( double Xi , double Eta, Vector coord1, Vector coord2,
        Vector coord3, Vector coord4, Vector coord5,
        Vector coord6, Vector coord7, Vector coord8,
        Vector coord9, ID Surface_nodeExistence  )
{
    static Vector J1( 3 );
    static Vector J2( 3 );

    static DTensor2 dh( 9, 2, 0.0 );
 
     // Node 9
     dh(8, 0) = Surface_nodeExistence(8) * ((1 - Eta * Eta) * (-2* Xi));
     dh(8, 1) = Surface_nodeExistence(8) * ((1 - Xi * Xi) * (-2* Eta));
 
     // Node 8
     dh(7, 0) = Surface_nodeExistence(7) * (0.5 * (1 - Eta * Eta)      - dh(8,0)/2);
     dh(7, 1) = Surface_nodeExistence(7) * (0.5 * (-2* Eta) * (1 + Xi) - dh(8,1)/2);
 
     // Node 7
     dh(6, 0) = Surface_nodeExistence(6) * (0.5 * (-2* Xi) * (1 - Eta) - dh(8,0)/2);
     dh(6, 1) = Surface_nodeExistence(6) * (0.5 * (1 - Xi * Xi) * (-1) - dh(8,1)/2);
 
     // Node 6
     dh(5, 0) = Surface_nodeExistence(5) * (0.5 * (1 - Eta * Eta)* (-1)- dh(8,0)/2);
     dh(5, 1) = Surface_nodeExistence(5) * (0.5 * (-2* Eta) * (1 - Xi) - dh(8,1)/2);
 
     // Node 5
     dh(4, 0) = Surface_nodeExistence(4) * (0.5 * (-2* Xi) * (1 + Eta) - dh(8,0)/2);
     dh(4, 1) = Surface_nodeExistence(4) * (0.5 * (1 - Xi * Xi)        - dh(8,1)/2);

     // Node 4
     dh(3, 0) = Surface_nodeExistence(3) * (0.25 *       (1 - Eta) - ( dh(6,0) + dh(7,0) )/2 - dh(8,0)/4);
     dh(3, 1) = Surface_nodeExistence(3) * (0.25 * (1 + Xi) * (-1) - ( dh(6,1) + dh(7,1) )/2 - dh(8,1)/4);

     // Node 3
     dh(2, 0) = Surface_nodeExistence(2) * (0.25 * (-1) *(1 - Eta) - ( dh(6,0) + dh(7,0) )/2 - dh(8,0)/4);
     dh(2, 1) = Surface_nodeExistence(2) * (0.25 * (1 - Xi) * (-1) - ( dh(6,1) + dh(7,1) )/2 - dh(8,1)/4);

     // Node 2
     dh(1, 0) = Surface_nodeExistence(1) * (0.25 * (-1) *(1 + Eta) - ( dh(6,0) + dh(7,0) )/2 - dh(8,0)/4);
     dh(1, 1) = Surface_nodeExistence(1) * (0.25 * (1 - Xi)        - ( dh(6,1) + dh(7,1) )/2 - dh(8,1)/4);

     // Node 1
     dh(0, 0) = Surface_nodeExistence(0) * (0.25 *       (1 + Eta) - ( dh(6,0) + dh(7,0) )/2 - dh(8,0)/4);
     dh(0, 1) = Surface_nodeExistence(0) * (0.25 * (1 + Xi)        - ( dh(6,1) + dh(7,1) )/2 - dh(8,1)/4);

     J1(0) = dh(0,0)* coord1(0) +
             dh(1,0)* coord2(0) +
             dh(2,0)* coord3(0) +
             dh(3,0)* coord4(0) +
             dh(4,0)* coord5(0) +
             dh(5,0)* coord6(0) +
             dh(6,0)* coord7(0) +
             dh(7,0)* coord8(0) +
             dh(8,0)* coord9(0) ;
 
     J1(1) = dh(0,0)* coord1(1) +
             dh(1,0)* coord2(1) +
             dh(2,0)* coord3(1) +
             dh(3,0)* coord4(1) +
             dh(4,0)* coord5(1) +
             dh(5,0)* coord6(1) +
             dh(6,0)* coord7(1) +
             dh(7,0)* coord8(1) +
             dh(8,0)* coord9(1) ;


     J1(2) = dh(0,0)* coord1(2) +
             dh(1,0)* coord2(2) +
             dh(2,0)* coord3(2) +
             dh(3,0)* coord4(2) +
             dh(4,0)* coord5(2) +
             dh(5,0)* coord6(2) +
             dh(6,0)* coord7(2) +
             dh(7,0)* coord8(2) +
             dh(8,0)* coord9(2) ;

     J2(0) = dh(0,1)* coord1(0) +
             dh(1,1)* coord2(0) +
             dh(2,1)* coord3(0) +
             dh(3,1)* coord4(0) +
             dh(4,1)* coord5(0) +
             dh(5,1)* coord6(0) +
             dh(6,1)* coord7(0) +
             dh(7,1)* coord8(0) +
             dh(8,1)* coord9(0) ;
 
     J2(1) = dh(0,1)* coord1(1) +
             dh(1,1)* coord2(1) +
             dh(2,1)* coord3(1) +
             dh(3,1)* coord4(1) +
             dh(4,1)* coord5(1) +
             dh(5,1)* coord6(1) +
             dh(6,1)* coord7(1) +
             dh(7,1)* coord8(1) +
             dh(8,1)* coord9(1) ;


     J2(2) = dh(0,1)* coord1(2) +
             dh(1,1)* coord2(2) +
             dh(2,1)* coord3(2) +
             dh(3,1)* coord4(2) +
             dh(4,1)* coord5(2) +
             dh(5,1)* coord6(2) +
             dh(6,1)* coord7(2) +
             dh(7,1)* coord8(2) +
             dh(8,1)* coord9(2) ;

    J_vector_in_function( 0 ) =  J1( 1 ) * J2( 2 ) - J1( 2 ) * J2( 1 ) ;
    J_vector_in_function( 1 ) =  J1( 2 ) * J2( 0 ) - J1( 0 ) * J2( 2 ) ;
    J_vector_in_function( 2 ) =  J1( 0 ) * J2( 1 ) - J1( 1 ) * J2( 0 ) ;


    return J_vector_in_function;
}



double BRICK_TYPE::SurfaceLoadValues( double Xi , double Eta, Vector Pressure, ID Surface_nodeExistence )
{
    ShapeFunctionValues_in_function(8) = Surface_nodeExistence(8) * ((1 - Eta * Eta) * (1 - Xi * Xi));
    ShapeFunctionValues_in_function(7) = Surface_nodeExistence(7) * (0.5 * (1 - Eta * Eta) * (1 + Xi) - ShapeFunctionValues_in_function(8)/2);
    ShapeFunctionValues_in_function(6) = Surface_nodeExistence(6) * (0.5 * (1 - Xi * Xi) * (1 - Eta)  - ShapeFunctionValues_in_function(8)/2);
    ShapeFunctionValues_in_function(5) = Surface_nodeExistence(5) * (0.5 * (1 - Eta * Eta) * (1 - Xi) - ShapeFunctionValues_in_function(8)/2);
    ShapeFunctionValues_in_function(4) = Surface_nodeExistence(4) * (0.5 * (1 - Xi * Xi) * (1 + Eta)  - ShapeFunctionValues_in_function(8)/2);
    ShapeFunctionValues_in_function(3) = Surface_nodeExistence(3) * (0.25 * (1 + Xi) * (1 - Eta) - ( ShapeFunctionValues_in_function(6) + ShapeFunctionValues_in_function(7) )/2 - ShapeFunctionValues_in_function(8)/4);
    ShapeFunctionValues_in_function(2) = Surface_nodeExistence(2) * (0.25 * (1 - Xi) * (1 - Eta) - ( ShapeFunctionValues_in_function(5) + ShapeFunctionValues_in_function(6) )/2 - ShapeFunctionValues_in_function(8)/4);
    ShapeFunctionValues_in_function(1) = Surface_nodeExistence(1) * (0.25 * (1 - Xi) * (1 + Eta) - ( ShapeFunctionValues_in_function(4) + ShapeFunctionValues_in_function(5) )/2 - ShapeFunctionValues_in_function(8)/4);
    ShapeFunctionValues_in_function(0) = Surface_nodeExistence(0) * (0.25 * (1 + Xi) * (1 + Eta) - ( ShapeFunctionValues_in_function(4) + ShapeFunctionValues_in_function(7) )/2 - ShapeFunctionValues_in_function(8)/4);

    SurfaceLoadValues_in_function = ShapeFunctionValues_in_function(8) * Pressure(8) +
                                    ShapeFunctionValues_in_function(7) * Pressure(7) +
                                    ShapeFunctionValues_in_function(6) * Pressure(6) +
                                    ShapeFunctionValues_in_function(5) * Pressure(5) +
                                    ShapeFunctionValues_in_function(4) * Pressure(4) +
                                    ShapeFunctionValues_in_function(3) * Pressure(3) +
                                    ShapeFunctionValues_in_function(2) * Pressure(2) +
                                    ShapeFunctionValues_in_function(1) * Pressure(1) +
                                    ShapeFunctionValues_in_function(0) * Pressure(0);

    return SurfaceLoadValues_in_function;

}


int
BRICK_TYPE::CheckMesh( ofstream &checkmesh_file )
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    double det_of_Jacobian = 0.0;

    static DTensor2 dh( 27, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);
    dh *= 0;
    Jacobian *= 0;

    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);

        // derivatives of local coordinates with respect to local coordinates
        dh = dh_drst_at( r, s, t );

        // Jacobian tensor (matrix)
        Jacobian = Jacobian_3D( dh );

        // determinant of Jacobian tensor (matrix)
        det_of_Jacobian  = Jacobian.compute_Determinant();


        if ( det_of_Jacobian <= 0 )
        {
            checkmesh_file << "ERROR!!! Element number (" << this->getTag() << "): has negative Jacobian determinant!" << endl;
        }

    }

    return 0;
}

//==================================================================================
Vector *
BRICK_TYPE::getStress( void )
{
    DTensor2 stress(3, 3, 0.0);
    Vector *stresses = new Vector( Num_TotalGaussPts*6 );   // FIXME: Who deallocates this guy???

    for ( short gp = 0 ; gp < Num_TotalGaussPts ; gp++ )
    {
        stress = material_array[gp]->getStressTensor();
        ( *stresses )( gp * 6 + 0 ) = stress( 0, 0 ); //sigma_xx
        ( *stresses )( gp * 6 + 1 ) = stress( 1, 1 ); //sigma_yy
        ( *stresses )( gp * 6 + 2 ) = stress( 2, 2 ); //sigma_zz
        ( *stresses )( gp * 6 + 3 ) = stress( 0, 1 ); //Assign sigma_xy
        ( *stresses )( gp * 6 + 4 ) = stress( 0, 2 ); //Assign sigma_xz
        ( *stresses )( gp * 6 + 5 ) = stress( 1, 2 ); //Assign sigma_yz
    }

    return stresses;
}

Matrix &BRICK_TYPE::getGaussCoordinates(void)
{
    computeGaussPoint();
    return gauss_points;
}

Matrix &BRICK_TYPE::get_Gauss_p_c( )
{
    //  Abscissae coefficient of the Gaussian quadrature formula
    // starting from 1 not from 0
    static Matrix Gauss_coordinates(7,7);

    Gauss_coordinates(1,1) = 0.0 ;
    Gauss_coordinates(2,1) = -0.577350269189626;
    Gauss_coordinates(2,2) = -Gauss_coordinates(2,1);
    Gauss_coordinates(3,1) = -0.774596669241483;
    Gauss_coordinates(3,2) = 0.0;
    Gauss_coordinates(3,3) = -Gauss_coordinates(3,1);
    Gauss_coordinates(4,1) = -0.861136311594053;
    Gauss_coordinates(4,2) = -0.339981043584856;
    Gauss_coordinates(4,3) = -Gauss_coordinates(4,2);
    Gauss_coordinates(4,4) = -Gauss_coordinates(4,1);
    Gauss_coordinates(5,1) = -0.906179845938664;
    Gauss_coordinates(5,2) = -0.538469310105683;
    Gauss_coordinates(5,3) = 0.0;
    Gauss_coordinates(5,4) = -Gauss_coordinates(5,2);
    Gauss_coordinates(5,5) = -Gauss_coordinates(5,1);
    Gauss_coordinates(6,1) = -0.932469514203152;
    Gauss_coordinates(6,2) = -0.661209386466265;
    Gauss_coordinates(6,3) = -0.238619186083197;
    Gauss_coordinates(6,4) = -Gauss_coordinates(6,3);
    Gauss_coordinates(6,5) = -Gauss_coordinates(6,2);
    Gauss_coordinates(6,6) = -Gauss_coordinates(6,1);

    return Gauss_coordinates;
}

Matrix &BRICK_TYPE::get_Gauss_p_w()
{
    //  Weight coefficient of the Gaussian quadrature formula
    // starting from 1 not from 0
    static Matrix Gauss_weights(7,7);

    Gauss_weights(1,1) = 2.0;
    Gauss_weights(2,1) = 1.0;
    Gauss_weights(2,2) = 1.0;
    Gauss_weights(3,1) = 0.555555555555556;
    Gauss_weights(3,2) = 0.888888888888889;
    Gauss_weights(3,3) = Gauss_weights(3,1);
    Gauss_weights(4,1) = 0.347854845137454;
    Gauss_weights(4,2) = 0.652145154862546;
    Gauss_weights(4,3) = Gauss_weights(4,2);
    Gauss_weights(4,4) = Gauss_weights(4,1);
    Gauss_weights(5,1) = 0.236926885056189;
    Gauss_weights(5,2) = 0.478628670499366;
    Gauss_weights(5,3) = 0.568888888888889;
    Gauss_weights(5,4) = Gauss_weights(5,2);
    Gauss_weights(5,5) = Gauss_weights(5,1);
    Gauss_weights(6,1) = 0.171324492379170;
    Gauss_weights(6,2) = 0.360761573048139;
    Gauss_weights(6,3) = 0.467913934572691;
    Gauss_weights(6,4) = Gauss_weights(6,3);
    Gauss_weights(6,5) = Gauss_weights(6,2);
    Gauss_weights(6,6) = Gauss_weights(6,1);

    return Gauss_weights;
}

/**********************************************************************************
* Sumeet August, 2016. See classTags.h for class description encoding 
* Returns the output at gauss points.
* NOTE!!! = For each gauss point there should be exactly 18 outputs 
*           6 Total_Strain, 6 Plastic_Strain and 6 Stress
*           Must be consistent with class description esedncoding.
*           Fix the class_desc accordingly based on the encoding formula
***********************************************************************************/
const vector<float> &BRICK_TYPE::getGaussOutput()
{
    DTensor2 stress(3, 3);
    DTensor2 strain(3, 3);
    DTensor2 plstrain(3, 3);

    int ii = 0;
    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
    {
        strain = material_array[gp]->getStrainTensor();
        plstrain = material_array[gp]->getPlasticStrainTensor();
        stress = material_array[gp]->getStressTensor();

        //Write strain
        Gauss_Output_Vector[ii++] = strain(0, 0);
        Gauss_Output_Vector[ii++] = strain(1, 1);
        Gauss_Output_Vector[ii++] = strain(2, 2);
        Gauss_Output_Vector[ii++] = strain(0, 1);
        Gauss_Output_Vector[ii++] = strain(0, 2);
        Gauss_Output_Vector[ii++] = strain(1, 2);

        //Write strain
        Gauss_Output_Vector[ii++] = plstrain(0, 0);
        Gauss_Output_Vector[ii++] = plstrain(1, 1);
        Gauss_Output_Vector[ii++] = plstrain(2, 2);
        Gauss_Output_Vector[ii++] = plstrain(0, 1);
        Gauss_Output_Vector[ii++] = plstrain(0, 2);
        Gauss_Output_Vector[ii++] = plstrain(1, 2);

        //Write stress
        Gauss_Output_Vector[ii++] = stress(0, 0);
        Gauss_Output_Vector[ii++] = stress(1, 1);
        Gauss_Output_Vector[ii++] = stress(2, 2);
        Gauss_Output_Vector[ii++] = stress(0, 1);
        Gauss_Output_Vector[ii++] = stress(0, 2);
        Gauss_Output_Vector[ii++] = stress(1, 2);
    }

    return Gauss_Output_Vector;
}


void BRICK_TYPE::zeroStrain()
{
    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
    {
        material_array[gp]->zeroStrain();
    }

    return ;
}


