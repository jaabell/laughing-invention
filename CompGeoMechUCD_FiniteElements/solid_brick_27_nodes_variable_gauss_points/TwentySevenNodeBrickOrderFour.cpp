///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli and Boris Jeremic && Sumeet 
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
// Adapted by Sumeet, for TwentySevenNodeBrickOrderFour to use the LTensor library for
// tensorial representation. [September, 2016]
/////////////////////////////////////////////////////////////////////////////////////



#ifndef TwentySevenNodeBrickOrderFour_CPP
#define TwentySevenNodeBrickOrderFour_CPP

#include "TwentySevenNodeBrickOrderFour.h"
#include <HDF5_Channel.h>


#define Order 4
#define Num_IntegrationPts Order
#define Num_TotalGaussPts Num_IntegrationPts*Num_IntegrationPts*Num_IntegrationPts


DTensor2 TwentySevenNodeBrickOrderFour::gp_coords(Num_TotalGaussPts, 3, 0.0);
DTensor2 TwentySevenNodeBrickOrderFour::gp_weight(Num_TotalGaussPts, 3, 0.0);
Matrix TwentySevenNodeBrickOrderFour::gauss_points(Num_TotalGaussPts, 3);
vector<float> TwentySevenNodeBrickOrderFour::Gauss_Output_Vector(Num_TotalGaussPts*18);

Matrix TwentySevenNodeBrickOrderFour::K(81, 81);

Vector TwentySevenNodeBrickOrderFour::ShapeFunctionValues_in_function( 9 );  // Nima added for surface load (July 2012)
Vector TwentySevenNodeBrickOrderFour::J_vector_in_function( 3 );             // Nima added for surface load (July 2012)
double TwentySevenNodeBrickOrderFour::SurfaceLoadValues_in_function;

//====================================================================
// Constructor
//====================================================================

TwentySevenNodeBrickOrderFour::TwentySevenNodeBrickOrderFour( int element_number,
        int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
        int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
        int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
        int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
        int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
        int node_numb_21,  int node_numb_22,  int node_numb_23,  int node_numb_24,
        int node_numb_25,  int node_numb_26,  int node_numb_27,
        NDMaterialLT *Globalmmodel)

    : Element( element_number, ELE_TAG_TwentySevenNodeBrickOrderFour ),
      rho( 0.0 ), connectedExternalNodes( 27 ),
      Ki( 0 ), Q( 81 ), bf(3),
      M(81,81), P( 81 )
{

    this->setMaterialTag(Globalmmodel->getTag());

    rho = Globalmmodel->getRho();

    determinant_of_Jacobian = 0.0;
    mmodel = Globalmmodel;

    material_array = new NDMaterialLT *[Num_TotalGaussPts];

    if (material_array == 0)
    {
        cerr << " TwentySevenNodeBrickOrderFour::TwentySevenNodeBrickOrderFour -- failed allocate material model pointer\n";
        exit(-1);
    }

    // Generate NDMaterialLT for use at each Gauss point.
    for (int k = 0; k < Num_TotalGaussPts; k++)
    {
        material_array[k] = mmodel->getCopy();
    }

    //GP coordinates and weights. ====================================================our
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

    Global_to_Local_Node_Mapping[node_numb_1] =0;
    Global_to_Local_Node_Mapping[node_numb_2] =1;
    Global_to_Local_Node_Mapping[node_numb_3] =2;
    Global_to_Local_Node_Mapping[node_numb_4] =3;
    Global_to_Local_Node_Mapping[node_numb_5] =4;
    Global_to_Local_Node_Mapping[node_numb_6] =5;
    Global_to_Local_Node_Mapping[node_numb_7] =6;
    Global_to_Local_Node_Mapping[node_numb_8] =7;
    Global_to_Local_Node_Mapping[node_numb_9] =8;
    Global_to_Local_Node_Mapping[node_numb_10] =9;
    Global_to_Local_Node_Mapping[node_numb_11] =10;
    Global_to_Local_Node_Mapping[node_numb_12] =11;
    Global_to_Local_Node_Mapping[node_numb_13] =12;
    Global_to_Local_Node_Mapping[node_numb_14] =13;
    Global_to_Local_Node_Mapping[node_numb_15] =14;
    Global_to_Local_Node_Mapping[node_numb_16] =15;
    Global_to_Local_Node_Mapping[node_numb_17] =16;
    Global_to_Local_Node_Mapping[node_numb_18] =17;
    Global_to_Local_Node_Mapping[node_numb_19] =18;
    Global_to_Local_Node_Mapping[node_numb_20] =19;
    Global_to_Local_Node_Mapping[node_numb_21] =20;
    Global_to_Local_Node_Mapping[node_numb_22] =21;
    Global_to_Local_Node_Mapping[node_numb_23] =22;
    Global_to_Local_Node_Mapping[node_numb_24] =23;
    Global_to_Local_Node_Mapping[node_numb_25] =24;
    Global_to_Local_Node_Mapping[node_numb_26] =25;
    Global_to_Local_Node_Mapping[node_numb_27] =26;


    for ( int i = 0; i < 27; i++ )
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(9);

}



//====================================================================
TwentySevenNodeBrickOrderFour::TwentySevenNodeBrickOrderFour(): Element( 0, ELE_TAG_TwentySevenNodeBrickOrderFour ),
    rho( 0.0 ), connectedExternalNodes( 27 ) , 
    Ki( 0 ), mmodel(0), Q( 81 ), bf(3),
    M(81,81), P( 81 )
{

    is_mass_computed = false;
    
    material_array = new NDMaterialLT *[Num_TotalGaussPts];

    if (material_array == 0)
    {
        cerr << " TwentySevenNodeBrickOrderFour::TwentySevenNodeBrickOrderFour -- failed allocate material model pointer\n";
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
TwentySevenNodeBrickOrderFour::~TwentySevenNodeBrickOrderFour ()
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
const DTensor2 &TwentySevenNodeBrickOrderFour::H_3D( double r1, double r2, double r3 ) const
{
    static DTensor2 H( 81, 3, 0.0 );

    //Shape Functions of Node 1 Along Three Coordinate Directions
    H(0, 0) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    H(1, 1) = H(0, 0);
    H(2, 2) = H(0, 0);

    //Shape Functions of Node 2 Along Three Coordinate Directions
    H(3, 0) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    H(4, 1) = H(3, 0);
    H(5, 2) = H(3, 0);

    //Shape Functions of Node 3 Along Three Coordinate Directions
    H(6, 0) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    H(7, 1) = H(6, 0);
    H(8, 2) = H(6, 0);

    //Shape Functions of Node 4 Along Three Coordinate Directions
    H(9, 0) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    H(10, 1) = H(9, 0);
    H(11, 2) = H(9, 0);

    //Shape Functions of Node 5 Along Three Coordinate Directions
    H(12, 0) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    H(13, 1) = H(12, 0);
    H(14, 2) = H(12, 0);

    //Shape Functions of Node 6 Along Three Coordinate Directions
    H(15, 0) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    H(16, 1) = H(15, 0);
    H(17, 2) = H(15, 0);

    //Shape Functions of Node 7 Along Three Coordinate Directions
    H(18, 0) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    H(19, 1) = H(18, 0);
    H(20, 2) = H(18, 0);


    //Shape Functions of Node 8 Along Three Coordinate Directions
    H(21, 0) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    H(22, 1) = H(21, 0);
    H(23, 2) = H(21, 0);


    //Shape Functions of Node 9 Along Three Coordinate Directions
    H(24, 0) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    H(25, 1) = H(24, 0);
    H(26, 2) = H(24, 0);


    //Shape Functions of Node 10 Along Three Coordinate Directions
    H(27, 0) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H(28, 1) = H(27, 0);
    H(29, 2) = H(27, 0);


    //Shape Functions of Node 11 Along Three Coordinate Directions
    H(30, 0) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    H(31, 1) = H(30, 0);
    H(32, 2) = H(30, 0);


    //Shape Functions of Node 12 Along Three Coordinate Directions
    H(33, 0) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H(34, 1) = H(33, 0);
    H(35, 2) = H(33, 0);


    //Shape Functions of Node 13 Along Three Coordinate Directions
    H(36, 0) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    H(37, 1) = H(36, 0);
    H(38, 2) = H(36, 0);


    //Shape Functions of Node 14 Along Three Coordinate Directions
    H(39, 0) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H(40, 1) = H(39, 0);
    H(41, 2) = H(39, 0);


    //Shape Functions of Node 15 Along Three Coordinate Directions
    H(42, 0) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    H(43, 1) = H(42, 0);
    H(44, 2) = H(42, 0);


    //Shape Functions of Node 16 Along Three Coordinate Directions
    H(45, 0) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H(46, 1) = H(45, 0);
    H(47, 2) = H(45, 0);


    //Shape Functions of Node 17 Along Three Coordinate Directions
    H(48, 0) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H(49, 1) = H(48, 0);
    H(50, 2) = H(48, 0);


    //Shape Functions of Node 18 Along Three Coordinate Directions
    H(51, 0) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H(52, 1) = H(51, 0);
    H(53, 2) = H(51, 0);


    //Shape Functions of Node 19 Along Three Coordinate Directions
    H(54, 0) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H(55, 1) = H(54, 0);
    H(56, 2) = H(54, 0);


    //Shape Functions of Node 20 Along Three Coordinate Directions
    H(57, 0) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H(58, 1) = H(57, 0);
    H(59, 2) = H(57, 0);


    //Shape Functions of Node 21 Along Three Coordinate Directions
    H(60, 0) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(61, 1) = H(60, 0);
    H(62, 2) = H(60, 0);


    //Shape Functions of Node 22 Along Three Coordinate Directions
    H(63, 0) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H(64, 1) = H(63, 0);
    H(65, 2) = H(63, 0);


    //Shape Functions of Node 23 Along Three Coordinate Directions
    H(66, 0) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(67, 1) = H(66, 0);
    H(68, 2) = H(66, 0);


    //Shape Functions of Node 24 Along Three Coordinate Directions
    H(69, 0) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H(70, 1) = H(69, 0);
    H(71, 2) = H(69, 0);


    //Shape Functions of Node 25 Along Three Coordinate Directions
    H(72, 0) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(73, 1) = H(72, 0);
    H(74, 2) = H(72, 0);


    //Shape Functions of Node 26 Along Three Coordinate Directions
    H(75, 0) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H(76, 1) = H(75, 0);
    H(77, 2) = H(75, 0);


    //Shape Functions of Node 27 Along Three Coordinate Directions
    H(78, 0) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H(79, 1) = H(78, 0);
    H(80, 2) = H(78, 0);

    return H;
}

//#############################################################################
//***************************************************************
const DTensor1 &TwentySevenNodeBrickOrderFour::interp_poli_at( double r1, double r2, double r3 ) const
{
    static DTensor1 h( 27, 0.0 );

    h( 0 ) = ( 1.0 + r1 ) * ( 1.0 + r2 ) * ( 1.0 + r3 ) * (r1) * (r2) * (r3) / 8.0;
    h( 1 ) = ( 1.0 - r1 ) * ( 1.0 + r2 ) * ( 1.0 + r3 ) * (-r1) * (r2) * (r3) / 8.0;
    h( 2 ) = ( 1.0 - r1 ) * ( 1.0 - r2 ) * ( 1.0 + r3 ) * (-r1) * (-r2) * (r3) / 8.0;
    h( 3 ) = ( 1.0 + r1 ) * ( 1.0 - r2 ) * ( 1.0 + r3 ) * (r1) * (-r2) * (r3) / 8.0;
    h( 4 ) = ( 1.0 + r1 ) * ( 1.0 + r2 ) * ( 1.0 - r3 ) * (r1) * (r2) * (-r3) / 8.0;
    h( 5 ) = ( 1.0 - r1 ) * ( 1.0 + r2 ) * ( 1.0 - r3 ) * (-r1) * (r2) * (-r3) / 8.0;
    h( 6 ) = ( 1.0 - r1 ) * ( 1.0 - r2 ) * ( 1.0 - r3 ) * (-r1) * (-r2) * (-r3) / 8.0;
    h( 7 ) = ( 1.0 + r1 ) * ( 1.0 - r2 ) * ( 1.0 - r3 ) * (r1) * (-r2) * (-r3) / 8.0;


    h( 8 ) = ( 1.0 ) * ( 1.0 + r2 ) * ( 1.0 + r3 ) * (r2) * (r3) / 4.0;
    h( 9 ) = ( 1.0 - r1 ) * ( 1.0 ) * ( 1.0 + r3 ) * (-r1) * (r3) / 4.0;
    h( 10 ) = ( 1.0 ) * ( 1.0 - r2 ) * ( 1.0 + r3 ) * (-r2) * (r3) / 4.0;
    h( 11 ) = ( 1.0 + r1 ) * ( 1.0 ) * ( 1.0 + r3 ) * (r1) * (r3) / 4.0;
    h( 12 ) = ( 1.0 ) * ( 1.0 + r2 ) * ( 1.0 - r3 ) * (r2) * (-r3) / 4.0;
    h( 13 ) = ( 1.0 - r1 ) * ( 1.0 ) * ( 1.0 - r3 ) * (-r1) * (-r3) / 4.0;
    h( 14 ) = ( 1.0 ) * ( 1.0 - r2 ) * ( 1.0 - r3 ) * (-r2) * (-r3) / 4.0;
    h( 15 ) = ( 1.0 + r1 ) * ( 1.0 ) * ( 1.0 - r3 ) * (r1) * (-r3) / 4.0;
    h( 16 ) = ( 1.0 + r1 ) * ( 1.0 + r2 ) * ( 1.0 ) * (r1) * (r2) / 4.0;
    h( 17 ) = ( 1.0 - r1 ) * ( 1.0 + r2 ) * ( 1.0 ) * (-r1) * (r2) / 4.0;
    h( 18 ) = ( 1.0 - r1 ) * ( 1.0 - r2 ) * ( 1.0 ) * (-r1) * (-r2) / 4.0;
    h( 19 ) = ( 1.0 + r1 ) * ( 1.0 - r2 ) * ( 1.0 ) * (r1) * (-r2) / 4.0;

    h( 20 ) = ( 1.0 ) * ( 1.0 ) * ( 1.0 );
    h( 21 ) = ( 1.0 ) * ( 1.0 + r2 ) * ( 1.0 ) * (r2) / 2.0;
    h( 22 ) = ( 1.0 - r1 ) * ( 1.0 ) * ( 1.0 ) * (-r1) / 2.0;
    h( 23 ) = ( 1.0 ) * ( 1.0 - r2 ) * ( 1.0 ) * (-r2) / 2.0;
    h( 24 ) = ( 1.0 + r1 ) * ( 1.0 ) * ( 1.0 ) * (r1) / 2.0;
    h( 25 ) = ( 1.0 ) * ( 1.0 ) * ( 1.0 + r3 ) * (r3) / 2.0;
    h( 26 ) = ( 1.0 ) * ( 1.0 ) * ( 1.0 - r3 ) * (-r3) / 2.0;

    return h;
}



const DTensor2 &TwentySevenNodeBrickOrderFour::dh_drst_at( double r1, double r2, double r3 ) const
{
    static DTensor2 dh( 27, 3, 0.0 );

    //Shape Functions of Node 1 Along Three Coordinate Directions
    dh(0, 0) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(0, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(0, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 + 1.0);

    //Shape Functions of Node 2 Along Three Coordinate Directions
    dh(1, 0) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(1, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(1, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 + 1.0);

    //Shape Functions of Node 3 Along Three Coordinate Directions
    dh(2, 0) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(2, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(2, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 + 1.0);

    //Shape Functions of Node 4 Along Three Coordinate Directions
    dh(3, 0) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(3, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(3, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 + 1.0);

    //Shape Functions of Node 5 Along Three Coordinate Directions
    dh(4, 0) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(4, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(4, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 - 1.0);

    //Shape Functions of Node 6 Along Three Coordinate Directions
    dh(5, 0) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(5, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(5, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 - 1.0);

    //Shape Functions of Node 7 Along Three Coordinate Directions
    dh(6, 0) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(6, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(6, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 8 Along Three Coordinate Directions
    dh(7, 0) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(7, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(7, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 9 Along Three Coordinate Directions
    dh(8, 0) = (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(8, 1) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(8, 2) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 10 Along Three Coordinate Directions
    dh(9, 0) = 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(9, 1) = 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(9, 2) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 11 Along Three Coordinate Directions
    dh(10, 0) = (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(10, 1) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh(10, 2) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 12 Along Three Coordinate Directions
    dh(11, 0) = 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(11, 1) = 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(11, 2) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 13 Along Three Coordinate Directions
    dh(12, 0) = (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(12, 1) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(12, 2) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 14 Along Three Coordinate Directions
    dh(13, 0) = 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(13, 1) = 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(13, 2) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 15 Along Three Coordinate Directions
    dh(14, 0) = (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(14, 1) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh(14, 2) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 16 Along Three Coordinate Directions
    dh(15, 0) = 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(15, 1) = 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(15, 2) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 17 Along Three Coordinate Directions
    dh(16, 0) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh(16, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh(16, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    //Shape Functions of Node 18 Along Three Coordinate Directions
    dh(17, 0) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh(17, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh(17, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    //Shape Functions of Node 19 Along Three Coordinate Directions
    dh(18, 0) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh(18, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh(18, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    //Shape Functions of Node 20 Along Three Coordinate Directions
    dh(19, 0) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh(19, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh(19, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    //Shape Functions of Node 21 Along Three Coordinate Directions
    dh(20, 0) = (-2.0 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(20, 1) = (1.0 - r1 * r1) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(20, 2) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * (-2.0 * r3);


    //Shape Functions of Node 22 Along Three Coordinate Directions
    dh(21, 0) = (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh(21, 1) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh(21, 2) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    //Shape Functions of Node 23 Along Three Coordinate Directions
    dh(22, 0) = 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(22, 1) = 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(22, 2) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    //Shape Functions of Node 24 Along Three Coordinate Directions
    dh(23, 0) = (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh(23, 1) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh(23, 2) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    //Shape Functions of Node 25 Along Three Coordinate Directions
    dh(24, 0) = 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(24, 1) = 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(24, 2) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    //Shape Functions of Node 26 Along Three Coordinate Directions
    dh(25, 0) = (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(25, 1) = (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(25, 2) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 27 Along Three Coordinate Directions
    dh(26, 0) = (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(26, 1) = (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(26, 2) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);

    return dh;
}

////#############################################################################
const DTensor2 &TwentySevenNodeBrickOrderFour::Jacobian_3D( const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    static DTensor2 Jacobian_3D_(3, 3, 0.0);
    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    return Jacobian_3D_;
}

//#############################################################################
const DTensor2  &TwentySevenNodeBrickOrderFour::Jacobian_3Dinv( const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    DTensor2 Jacobian_3D_(3, 3, 0.0);
    static DTensor2 Jacobian_3D_inv(3, 3, 0.0);

    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    Jacobian_3D_inv = Jacobian_3D_.Inv();
    return Jacobian_3D_inv;
}


////#############################################################################
const DTensor2 &TwentySevenNodeBrickOrderFour::Nodal_Coordinates( void ) const
{
    static DTensor2 N_coord( 27, 3, 0.0 );

    //using node pointers, which come from the Domain
    const Vector &nd1Crds = theNodes[0]->getCrds();
    const Vector &nd2Crds = theNodes[1]->getCrds();
    const Vector &nd3Crds = theNodes[2]->getCrds();
    const Vector &nd4Crds = theNodes[3]->getCrds();
    const Vector &nd5Crds = theNodes[4]->getCrds();
    const Vector &nd6Crds = theNodes[5]->getCrds();
    const Vector &nd7Crds = theNodes[6]->getCrds();
    const Vector &nd8Crds = theNodes[7]->getCrds();

    const Vector &nd9Crds  =  theNodes[8]->getCrds();
    const Vector &nd10Crds = theNodes[9]->getCrds();
    const Vector &nd11Crds = theNodes[10]->getCrds();
    const Vector &nd12Crds = theNodes[11]->getCrds();

    const Vector &nd13Crds = theNodes[12]->getCrds();
    const Vector &nd14Crds = theNodes[13]->getCrds();
    const Vector &nd15Crds = theNodes[14]->getCrds();
    const Vector &nd16Crds = theNodes[15]->getCrds();


    const Vector &nd17Crds = theNodes[16]->getCrds();
    const Vector &nd18Crds = theNodes[17]->getCrds();
    const Vector &nd19Crds = theNodes[18]->getCrds();
    const Vector &nd20Crds = theNodes[19]->getCrds();

    const Vector &nd21Crds = theNodes[20]->getCrds();
    const Vector &nd22Crds = theNodes[21]->getCrds();
    const Vector &nd23Crds = theNodes[22]->getCrds();
    const Vector &nd24Crds = theNodes[23]->getCrds();
    const Vector &nd25Crds = theNodes[24]->getCrds();
    const Vector &nd26Crds = theNodes[25]->getCrds();
    const Vector &nd27Crds = theNodes[26]->getCrds();

    N_coord( 0, 0 ) = nd1Crds( 0 );
    N_coord( 0, 1 ) = nd1Crds( 1 );
    N_coord( 0, 2 ) = nd1Crds( 2 );
    N_coord( 1, 0 ) = nd2Crds( 0 );
    N_coord( 1, 1 ) = nd2Crds( 1 );
    N_coord( 1, 2 ) = nd2Crds( 2 );
    N_coord( 2, 0 ) = nd3Crds( 0 );
    N_coord( 2, 1 ) = nd3Crds( 1 );
    N_coord( 2, 2 ) = nd3Crds( 2 );
    N_coord( 3, 0 ) = nd4Crds( 0 );
    N_coord( 3, 1 ) = nd4Crds( 1 );
    N_coord( 3, 2 ) = nd4Crds( 2 );
    N_coord( 4, 0 ) = nd5Crds( 0 );
    N_coord( 4, 1 ) = nd5Crds( 1 );
    N_coord( 4, 2 ) = nd5Crds( 2 );
    N_coord( 5, 0 ) = nd6Crds( 0 );
    N_coord( 5, 1 ) = nd6Crds( 1 );
    N_coord( 5, 2 ) = nd6Crds( 2 );
    N_coord( 6, 0 ) = nd7Crds( 0 );
    N_coord( 6, 1 ) = nd7Crds( 1 );
    N_coord( 6, 2 ) = nd7Crds( 2 );
    N_coord( 7, 0 ) = nd8Crds( 0 );
    N_coord( 7, 1 ) = nd8Crds( 1 );
    N_coord( 7, 2 ) = nd8Crds( 2 );

    N_coord(8 , 0) = nd9Crds(0);
    N_coord(8 , 1) = nd9Crds(1);
    N_coord(8 , 2) = nd9Crds(2);
    N_coord(9, 0) = nd10Crds(0);
    N_coord(9, 1) = nd10Crds(1);
    N_coord(9, 2) = nd10Crds(2);
    N_coord(10, 0) = nd11Crds(0);
    N_coord(10, 1) = nd11Crds(1);
    N_coord(10, 2) = nd11Crds(2);
    N_coord(11, 0) = nd12Crds(0);
    N_coord(11, 1) = nd12Crds(1);
    N_coord(11, 2) = nd12Crds(2);

    N_coord(12, 0) = nd13Crds(0);
    N_coord(12, 1) = nd13Crds(1);
    N_coord(12, 2) = nd13Crds(2);
    N_coord(13, 0) = nd14Crds(0);
    N_coord(13, 1) = nd14Crds(1);
    N_coord(13, 2) = nd14Crds(2);
    N_coord(14, 0) = nd15Crds(0);
    N_coord(14, 1) = nd15Crds(1);
    N_coord(14, 2) = nd15Crds(2);
    N_coord(15, 0) = nd16Crds(0);
    N_coord(15, 1) = nd16Crds(1);
    N_coord(15, 2) = nd16Crds(2);

    N_coord(16, 0) = nd17Crds(0);
    N_coord(16, 1) = nd17Crds(1);
    N_coord(16, 2) = nd17Crds(2);
    N_coord(17, 0) = nd18Crds(0);
    N_coord(17, 1) = nd18Crds(1);
    N_coord(17, 2) = nd18Crds(2);
    N_coord(18, 0) = nd19Crds(0);
    N_coord(18, 1) = nd19Crds(1);
    N_coord(18, 2) = nd19Crds(2);
    N_coord(19, 0) = nd20Crds(0);
    N_coord(19, 1) = nd20Crds(1);
    N_coord(19, 2) = nd20Crds(2);

    N_coord(20, 0) = nd21Crds(0);
    N_coord(20, 1) = nd21Crds(1);
    N_coord(20, 2) = nd21Crds(2);
    N_coord(21, 0) = nd22Crds(0);
    N_coord(21, 1) = nd22Crds(1);
    N_coord(21, 2) = nd22Crds(2);
    N_coord(22, 0) = nd23Crds(0);
    N_coord(22, 1) = nd23Crds(1);
    N_coord(22, 2) = nd23Crds(2);
    N_coord(23, 0) = nd24Crds(0);
    N_coord(23, 1) = nd24Crds(1);
    N_coord(23, 2) = nd24Crds(2);
    N_coord(24, 0) = nd25Crds(0);
    N_coord(24, 1) = nd25Crds(1);
    N_coord(24, 2) = nd25Crds(2);
    N_coord(25, 0) = nd26Crds(0);
    N_coord(25, 1) = nd26Crds(1);
    N_coord(25, 2) = nd26Crds(2);
    N_coord(26, 0) = nd27Crds(0);
    N_coord(26, 1) = nd27Crds(1);
    N_coord(26, 2) = nd27Crds(2);

    return N_coord;
}

////#############################################################################
const DTensor2 &TwentySevenNodeBrickOrderFour::incr_disp( void ) const
{
    static DTensor2 increment_disp( 27, 3, 0.0 );

    //Have to get IncrDeltaDisp, not IncrDisp for cumulation of incr_disp
    const Vector &IncrDis1 = theNodes[0]->getIncrDeltaDisp();
    const Vector &IncrDis2 = theNodes[1]->getIncrDeltaDisp();
    const Vector &IncrDis3 = theNodes[2]->getIncrDeltaDisp();
    const Vector &IncrDis4 = theNodes[3]->getIncrDeltaDisp();
    const Vector &IncrDis5 = theNodes[4]->getIncrDeltaDisp();
    const Vector &IncrDis6 = theNodes[5]->getIncrDeltaDisp();
    const Vector &IncrDis7 = theNodes[6]->getIncrDeltaDisp();
    const Vector &IncrDis8 = theNodes[7]->getIncrDeltaDisp();

    const Vector &IncrDis9  = theNodes[8]->getIncrDeltaDisp();
    const Vector &IncrDis10 = theNodes[9]->getIncrDeltaDisp();
    const Vector &IncrDis11 = theNodes[10]->getIncrDeltaDisp();
    const Vector &IncrDis12 = theNodes[11]->getIncrDeltaDisp();

    const Vector &IncrDis13 = theNodes[12]->getIncrDeltaDisp();
    const Vector &IncrDis14 = theNodes[13]->getIncrDeltaDisp();
    const Vector &IncrDis15 = theNodes[14]->getIncrDeltaDisp();
    const Vector &IncrDis16 = theNodes[15]->getIncrDeltaDisp();

    const Vector &IncrDis17 = theNodes[16]->getIncrDeltaDisp();
    const Vector &IncrDis18 = theNodes[17]->getIncrDeltaDisp();
    const Vector &IncrDis19 = theNodes[18]->getIncrDeltaDisp();
    const Vector &IncrDis20 = theNodes[19]->getIncrDeltaDisp();

    const Vector &IncrDis21 = theNodes[20]->getIncrDeltaDisp();
    const Vector &IncrDis22 = theNodes[21]->getIncrDeltaDisp();
    const Vector &IncrDis23 = theNodes[22]->getIncrDeltaDisp();
    const Vector &IncrDis24 = theNodes[23]->getIncrDeltaDisp();
    const Vector &IncrDis25 = theNodes[24]->getIncrDeltaDisp();
    const Vector &IncrDis26 = theNodes[25]->getIncrDeltaDisp();
    const Vector &IncrDis27 = theNodes[26]->getIncrDeltaDisp();



    increment_disp( 0, 0 ) = IncrDis1( 0 );
    increment_disp( 0, 1 ) = IncrDis1( 1 );
    increment_disp( 0, 2 ) = IncrDis1( 2 );
    increment_disp( 1, 0 ) = IncrDis2( 0 );
    increment_disp( 1, 1 ) = IncrDis2( 1 );
    increment_disp( 1, 2 ) = IncrDis2( 2 );
    increment_disp( 2, 0 ) = IncrDis3( 0 );
    increment_disp( 2, 1 ) = IncrDis3( 1 );
    increment_disp( 2, 2 ) = IncrDis3( 2 );
    increment_disp( 3, 0 ) = IncrDis4( 0 );
    increment_disp( 3, 1 ) = IncrDis4( 1 );
    increment_disp( 3, 2 ) = IncrDis4( 2 );
    increment_disp( 4, 0 ) = IncrDis5( 0 );
    increment_disp( 4, 1 ) = IncrDis5( 1 );
    increment_disp( 4, 2 ) = IncrDis5( 2 );
    increment_disp( 5, 0 ) = IncrDis6( 0 );
    increment_disp( 5, 1 ) = IncrDis6( 1 );
    increment_disp( 5, 2 ) = IncrDis6( 2 );
    increment_disp( 6, 0 ) = IncrDis7( 0 );
    increment_disp( 6, 1 ) = IncrDis7( 1 );
    increment_disp( 6, 2 ) = IncrDis7( 2 );
    increment_disp( 7, 0 ) = IncrDis8( 0 );
    increment_disp( 7, 1 ) = IncrDis8( 1 );
    increment_disp( 7, 2 ) = IncrDis8( 2 );

    increment_disp(8 , 0) = IncrDis9(0);
    increment_disp(8 , 1) = IncrDis9(1);
    increment_disp(8 , 2) = IncrDis9(2);
    increment_disp(9, 0) = IncrDis10(0);
    increment_disp(9, 1) = IncrDis10(1);
    increment_disp(9, 2) = IncrDis10(2);
    increment_disp(10, 0) = IncrDis11(0);
    increment_disp(10, 1) = IncrDis11(1);
    increment_disp(10, 2) = IncrDis11(2);
    increment_disp(11, 0) = IncrDis12(0);
    increment_disp(11, 1) = IncrDis12(1);
    increment_disp(11, 2) = IncrDis12(2);

    increment_disp(12, 0) = IncrDis13(0);
    increment_disp(12, 1) = IncrDis13(1);
    increment_disp(12, 2) = IncrDis13(2);
    increment_disp(13, 0) = IncrDis14(0);
    increment_disp(13, 1) = IncrDis14(1);
    increment_disp(13, 2) = IncrDis14(2);
    increment_disp(14, 0) = IncrDis15(0);
    increment_disp(14, 1) = IncrDis15(1);
    increment_disp(14, 2) = IncrDis15(2);
    increment_disp(15, 0) = IncrDis16(0);
    increment_disp(15, 1) = IncrDis16(1);
    increment_disp(15, 2) = IncrDis16(2);

    increment_disp(16, 0) = IncrDis17(0);
    increment_disp(16, 1) = IncrDis17(1);
    increment_disp(16, 2) = IncrDis17(2);
    increment_disp(17, 0) = IncrDis18(0);
    increment_disp(17, 1) = IncrDis18(1);
    increment_disp(17, 2) = IncrDis18(2);
    increment_disp(18, 0) = IncrDis19(0);
    increment_disp(18, 1) = IncrDis19(1);
    increment_disp(18, 2) = IncrDis19(2);
    increment_disp(19, 0) = IncrDis20(0);
    increment_disp(19, 1) = IncrDis20(1);
    increment_disp(19, 2) = IncrDis20(2);

    increment_disp(20, 0) = IncrDis21(0);
    increment_disp(20, 1) = IncrDis21(1);
    increment_disp(20, 2) = IncrDis21(2);
    increment_disp(21, 0) = IncrDis22(0);
    increment_disp(21, 1) = IncrDis22(1);
    increment_disp(21, 2) = IncrDis22(2);
    increment_disp(22, 0) = IncrDis23(0);
    increment_disp(22, 1) = IncrDis23(1);
    increment_disp(22, 2) = IncrDis23(2);
    increment_disp(23, 0) = IncrDis24(0);
    increment_disp(23, 1) = IncrDis24(1);
    increment_disp(23, 2) = IncrDis24(2);
    increment_disp(24, 0) = IncrDis25(0);
    increment_disp(24, 1) = IncrDis25(1);
    increment_disp(24, 2) = IncrDis25(2);
    increment_disp(25, 0) = IncrDis26(0);
    increment_disp(25, 1) = IncrDis26(1);
    increment_disp(25, 2) = IncrDis26(2);
    increment_disp(26, 0) = IncrDis27(0);
    increment_disp(26, 1) = IncrDis27(1);
    increment_disp(26, 2) = IncrDis27(2);

    return increment_disp;
}

////#############################################################################
const DTensor2 &TwentySevenNodeBrickOrderFour::total_disp( void ) const
{
    static DTensor2 total_disp( 27, 3, 0.0 );

    const Vector &TotDis1 = theNodes[0]->getTrialDisp();
    const Vector &TotDis2 = theNodes[1]->getTrialDisp();
    const Vector &TotDis3 = theNodes[2]->getTrialDisp();
    const Vector &TotDis4 = theNodes[3]->getTrialDisp();
    const Vector &TotDis5 = theNodes[4]->getTrialDisp();
    const Vector &TotDis6 = theNodes[5]->getTrialDisp();
    const Vector &TotDis7 = theNodes[6]->getTrialDisp();
    const Vector &TotDis8 = theNodes[7]->getTrialDisp();

    const Vector &TotDis9 = theNodes[8]->getTrialDisp();
    const Vector &TotDis10 = theNodes[9]->getTrialDisp();
    const Vector &TotDis11 = theNodes[10]->getTrialDisp();
    const Vector &TotDis12 = theNodes[11]->getTrialDisp();

    const Vector &TotDis13 = theNodes[12]->getTrialDisp();
    const Vector &TotDis14 = theNodes[13]->getTrialDisp();
    const Vector &TotDis15 = theNodes[14]->getTrialDisp();
    const Vector &TotDis16 = theNodes[15]->getTrialDisp();

    const Vector &TotDis17 = theNodes[16]->getTrialDisp();
    const Vector &TotDis18 = theNodes[17]->getTrialDisp();
    const Vector &TotDis19 = theNodes[18]->getTrialDisp();
    const Vector &TotDis20 = theNodes[19]->getTrialDisp();

    const Vector &TotDis21 = theNodes[20]->getTrialDisp();
    const Vector &TotDis22 = theNodes[21]->getTrialDisp();
    const Vector &TotDis23 = theNodes[22]->getTrialDisp();
    const Vector &TotDis24 = theNodes[23]->getTrialDisp();
    const Vector &TotDis25 = theNodes[24]->getTrialDisp();
    const Vector &TotDis26 = theNodes[25]->getTrialDisp();
    const Vector &TotDis27 = theNodes[26]->getTrialDisp();


    total_disp( 0, 0 ) = TotDis1( 0 );
    total_disp( 0, 1 ) = TotDis1( 1 );
    total_disp( 0, 2 ) = TotDis1( 2 );
    total_disp( 1, 0 ) = TotDis2( 0 );
    total_disp( 1, 1 ) = TotDis2( 1 );
    total_disp( 1, 2 ) = TotDis2( 2 );
    total_disp( 2, 0 ) = TotDis3( 0 );
    total_disp( 2, 1 ) = TotDis3( 1 );
    total_disp( 2, 2 ) = TotDis3( 2 );
    total_disp( 3, 0 ) = TotDis4( 0 );
    total_disp( 3, 1 ) = TotDis4( 1 );
    total_disp( 3, 2 ) = TotDis4( 2 );
    total_disp( 4, 0 ) = TotDis5( 0 );
    total_disp( 4, 1 ) = TotDis5( 1 );
    total_disp( 4, 2 ) = TotDis5( 2 );
    total_disp( 5, 0 ) = TotDis6( 0 );
    total_disp( 5, 1 ) = TotDis6( 1 );
    total_disp( 5, 2 ) = TotDis6( 2 );
    total_disp( 6, 0 ) = TotDis7( 0 );
    total_disp( 6, 1 ) = TotDis7( 1 );
    total_disp( 6, 2 ) = TotDis7( 2 );
    total_disp( 7, 0 ) = TotDis8( 0 );
    total_disp( 7, 1 ) = TotDis8( 1 );
    total_disp( 7, 2 ) = TotDis8( 2 );

    total_disp(8, 0) = TotDis9(0);
    total_disp(8, 1) = TotDis9(1);
    total_disp(8, 2) = TotDis9(2);
    total_disp(9, 0) = TotDis10(0);
    total_disp(9, 1) = TotDis10(1);
    total_disp(9, 2) = TotDis10(2);
    total_disp(10, 0) = TotDis11(0);
    total_disp(10, 1) = TotDis11(1);
    total_disp(10, 2) = TotDis11(2);
    total_disp(11, 0) = TotDis12(0);
    total_disp(11, 1) = TotDis12(1);
    total_disp(11, 2) = TotDis12(2);

    total_disp(12, 0) = TotDis13(0);
    total_disp(12, 1) = TotDis13(1);
    total_disp(12, 2) = TotDis13(2);
    total_disp(13, 0) = TotDis14(0);
    total_disp(13, 1) = TotDis14(1);
    total_disp(13, 2) = TotDis14(2);
    total_disp(14, 0) = TotDis15(0);
    total_disp(14, 1) = TotDis15(1);
    total_disp(14, 2) = TotDis15(2);
    total_disp(15, 0) = TotDis16(0);
    total_disp(15, 1) = TotDis16(1);
    total_disp(15, 2) = TotDis16(2);

    total_disp(16, 0) = TotDis17(0);
    total_disp(16, 1) = TotDis17(1);
    total_disp(16, 2) = TotDis17(2);
    total_disp(17, 0) = TotDis18(0);
    total_disp(17, 1) = TotDis18(1);
    total_disp(17, 2) = TotDis18(2);
    total_disp(18, 0) = TotDis19(0);
    total_disp(18, 1) = TotDis19(1);
    total_disp(18, 2) = TotDis19(2);
    total_disp(19, 0) = TotDis20(0);
    total_disp(19, 1) = TotDis20(1);
    total_disp(19, 2) = TotDis20(2);

    total_disp(20, 0) = TotDis21(0);
    total_disp(20, 1) = TotDis21(1);
    total_disp(20, 2) = TotDis21(2);
    total_disp(21, 0) = TotDis22(0);
    total_disp(21, 1) = TotDis22(1);
    total_disp(21, 2) = TotDis22(2);
    total_disp(22, 0) = TotDis23(0);
    total_disp(22, 1) = TotDis23(1);
    total_disp(22, 2) = TotDis23(2);
    total_disp(23, 0) = TotDis24(0);
    total_disp(23, 1) = TotDis24(1);
    total_disp(23, 2) = TotDis24(2);
    total_disp(24, 0) = TotDis25(0);
    total_disp(24, 1) = TotDis25(1);
    total_disp(24, 2) = TotDis25(2);
    total_disp(25, 0) = TotDis26(0);
    total_disp(25, 1) = TotDis26(1);
    total_disp(25, 2) = TotDis26(2);
    total_disp(26, 0) = TotDis27(0);
    total_disp(26, 1) = TotDis27(1);
    total_disp(26, 2) = TotDis27(2);

    return total_disp;
}



////#############################################################################
int TwentySevenNodeBrickOrderFour::get_global_number_of_node( int local_node_number )
{
    return connectedExternalNodes( local_node_number );
}

////#############################################################################
int  TwentySevenNodeBrickOrderFour::get_Brick_Number( void )
{
    return this->getTag();
}



////#############################################################################
// returns nodal forces for given stress field in an element
const DTensor2 &TwentySevenNodeBrickOrderFour::nodal_forces( void ) const
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

    return nodal_forces;
}



//Compute gauss point coordinates
void TwentySevenNodeBrickOrderFour::computeGaussPoint()
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    // special case for 27 nodes only
    DTensor2 NodalCoord( 3, 27, 0.0 );
    DTensor2 material_arrayCoord( 3, Num_TotalGaussPts, 0.0 );
    DTensor2 H( 81, 3, 0.0 );

    const Vector &nd1Crds = theNodes[0]->getCrds();
    const Vector &nd2Crds = theNodes[1]->getCrds();
    const Vector &nd3Crds = theNodes[2]->getCrds();
    const Vector &nd4Crds = theNodes[3]->getCrds();
    const Vector &nd5Crds = theNodes[4]->getCrds();
    const Vector &nd6Crds = theNodes[5]->getCrds();
    const Vector &nd7Crds = theNodes[6]->getCrds();
    const Vector &nd8Crds = theNodes[7]->getCrds();

    const Vector &nd9Crds  = theNodes[8]->getCrds();
    const Vector &nd10Crds = theNodes[9]->getCrds();
    const Vector &nd11Crds = theNodes[10]->getCrds();
    const Vector &nd12Crds = theNodes[11]->getCrds();

    const Vector &nd13Crds = theNodes[12]->getCrds();
    const Vector &nd14Crds = theNodes[13]->getCrds();
    const Vector &nd15Crds = theNodes[14]->getCrds();
    const Vector &nd16Crds = theNodes[15]->getCrds();

    const Vector &nd17Crds = theNodes[16]->getCrds();
    const Vector &nd18Crds = theNodes[17]->getCrds();
    const Vector &nd19Crds = theNodes[18]->getCrds();
    const Vector &nd20Crds = theNodes[19]->getCrds();

    const Vector &nd21Crds = theNodes[20]->getCrds();
    const Vector &nd22Crds = theNodes[21]->getCrds();
    const Vector &nd23Crds = theNodes[22]->getCrds();
    const Vector &nd24Crds = theNodes[23]->getCrds();
    const Vector &nd25Crds = theNodes[24]->getCrds();
    const Vector &nd26Crds = theNodes[25]->getCrds();
    const Vector &nd27Crds = theNodes[26]->getCrds();


    NodalCoord( 0, 0 ) = nd1Crds( 0 );
    NodalCoord( 1, 0 ) = nd1Crds( 1 );
    NodalCoord( 2, 0 ) = nd1Crds( 2 );

    NodalCoord( 0, 1 ) = nd2Crds( 0 );
    NodalCoord( 1, 1 ) = nd2Crds( 1 );
    NodalCoord( 2, 1 ) = nd2Crds( 2 );

    NodalCoord( 0, 2 ) = nd3Crds( 0 );
    NodalCoord( 1, 2 ) = nd3Crds( 1 );
    NodalCoord( 2, 2 ) = nd3Crds( 2 );

    NodalCoord( 0, 3 ) = nd4Crds( 0 );
    NodalCoord( 1, 3 ) = nd4Crds( 1 );
    NodalCoord( 2, 3 ) = nd4Crds( 2 );

    NodalCoord( 0, 4 ) = nd5Crds( 0 );
    NodalCoord( 1, 4 ) = nd5Crds( 1 );
    NodalCoord( 2, 4 ) = nd5Crds( 2 );

    NodalCoord( 0, 5 ) = nd6Crds( 0 );
    NodalCoord( 1, 5 ) = nd6Crds( 1 );
    NodalCoord( 2, 5 ) = nd6Crds( 2 );

    NodalCoord( 0, 6 ) = nd7Crds( 0 );
    NodalCoord( 1, 6 ) = nd7Crds( 1 );
    NodalCoord( 2, 6 ) = nd7Crds( 2 );

    NodalCoord( 0, 7 ) = nd8Crds( 0 );
    NodalCoord( 1, 7 ) = nd8Crds( 1 );
    NodalCoord( 2, 7 ) = nd8Crds( 2 );

    NodalCoord(0, 8) = nd9Crds( 0);
    NodalCoord(1, 8) = nd9Crds( 1);
    NodalCoord(2, 8) = nd9Crds( 2);

    NodalCoord(0, 9) = nd10Crds(0);
    NodalCoord(1, 9) = nd10Crds(1);
    NodalCoord(2, 9) = nd10Crds(2);

    NodalCoord(0, 10) = nd11Crds(0);
    NodalCoord(1, 10) = nd11Crds(1);
    NodalCoord(2, 10) = nd11Crds(2);

    NodalCoord(0, 11) = nd12Crds(0);
    NodalCoord(1, 11) = nd12Crds(1);
    NodalCoord(2, 11) = nd12Crds(2);

    NodalCoord(0, 12) = nd13Crds(0);
    NodalCoord(1, 12) = nd13Crds(1);
    NodalCoord(2, 12) = nd13Crds(2);

    NodalCoord(0, 13) = nd14Crds(0);
    NodalCoord(1, 13) = nd14Crds(1);
    NodalCoord(2, 13) = nd14Crds(2);

    NodalCoord(0, 14) = nd15Crds(0);
    NodalCoord(1, 14) = nd15Crds(1);
    NodalCoord(2, 14) = nd15Crds(2);

    NodalCoord(0, 15) = nd16Crds(0);
    NodalCoord(1, 15) = nd16Crds(1);
    NodalCoord(2, 15) = nd16Crds(2);

    NodalCoord(0, 16) = nd17Crds(0);
    NodalCoord(1, 16) = nd17Crds(1);
    NodalCoord(2, 16) = nd17Crds(2);

    NodalCoord(0, 17) = nd18Crds(0);
    NodalCoord(1, 17) = nd18Crds(1);
    NodalCoord(2, 17) = nd18Crds(2);

    NodalCoord(0, 18) = nd19Crds(0);
    NodalCoord(1, 18) = nd19Crds(1);
    NodalCoord(2, 18) = nd19Crds(2);

    NodalCoord(0, 19) = nd20Crds(0);
    NodalCoord(1, 19) = nd20Crds(1);
    NodalCoord(2, 19) = nd20Crds(2);

    NodalCoord(0, 20) = nd21Crds(0);
    NodalCoord(1, 20) = nd21Crds(1);
    NodalCoord(2, 20) = nd21Crds(2);

    NodalCoord(0, 21) = nd22Crds(0);
    NodalCoord(1, 21) = nd22Crds(1);
    NodalCoord(2, 21) = nd22Crds(2);

    NodalCoord(0, 22) = nd23Crds(0);
    NodalCoord(1, 22) = nd23Crds(1);
    NodalCoord(2, 22) = nd23Crds(2);

    NodalCoord(0, 23) = nd24Crds(0);
    NodalCoord(1, 23) = nd24Crds(1);
    NodalCoord(2, 23) = nd24Crds(2);

    NodalCoord(0, 24) = nd25Crds(0);
    NodalCoord(1, 24) = nd25Crds(1);
    NodalCoord(2, 24) = nd25Crds(2);

    NodalCoord(0, 25) = nd26Crds(0);
    NodalCoord(1, 25) = nd26Crds(1);
    NodalCoord(2, 25) = nd26Crds(2);

    NodalCoord(0, 26) = nd27Crds(0);
    NodalCoord(1, 26) = nd27Crds(1);
    NodalCoord(2, 26) = nd27Crds(2);


    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);

        H = H_3D( r, s, t );

        for ( int encount = 0 ; encount < 27 ; encount++ )
        {
            material_arrayCoord( 0, gp ) += NodalCoord( 0, encount ) * H( encount * 3 + 0, 0 );
            material_arrayCoord( 1, gp ) += NodalCoord( 1, encount ) * H( encount * 3 + 1, 1 );
            material_arrayCoord( 2, gp ) += NodalCoord( 2, encount ) * H( encount * 3 + 2, 2 );
        }

        gauss_points( gp, 0 ) = material_arrayCoord( 0, gp );
        gauss_points( gp, 1 ) = material_arrayCoord( 1, gp );
        gauss_points( gp, 2 ) = material_arrayCoord( 2, gp );

    }
}


////#############################################################################


//=============================================================================
int TwentySevenNodeBrickOrderFour::getNumExternalNodes () const
{
    return 27;
}


//=============================================================================
const ID &TwentySevenNodeBrickOrderFour::getExternalNodes ()
{
    return connectedExternalNodes;
}

Node **
TwentySevenNodeBrickOrderFour::getNodePtrs( void )
{
    return theNodes;
}

//=============================================================================
int TwentySevenNodeBrickOrderFour::getNumDOF ()
{
    return 81;
}

//=============================================================================
void TwentySevenNodeBrickOrderFour::setDomain ( Domain *theDomain )
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

        for (int i = 0; i < 27; ++i)
        {
            theNodes[i] = theDomain->getNode( connectedExternalNodes(i) );

# ifdef _PARALLEL_PROCESSING
            if ( theNodes[i] == 0 )
            {
                theNodes[i] = theDomain->getOutsideNode( connectedExternalNodes(i) );
            }
# endif
        }


        if ( theNodes[0] == 0 || theNodes[1] == 0 || theNodes[2] == 0 || theNodes[3] == 0 ||
                theNodes[4] == 0 || theNodes[5] == 0 || theNodes[6] == 0 || theNodes[7] == 0 ||
                theNodes[8]  == 0 || theNodes[9] == 0 || theNodes[10] == 0 || theNodes[11] == 0 ||
                theNodes[12] == 0 || theNodes[13] == 0 || theNodes[14] == 0 || theNodes[15] == 0 ||
                theNodes[16] == 0 || theNodes[17] == 0 || theNodes[18] == 0 || theNodes[19] == 0 ||
                theNodes[20] == 0 || theNodes[21] == 0 || theNodes[22] == 0 || theNodes[23] == 0 ||
                theNodes[24] == 0 || theNodes[25] == 0 || theNodes[26] == 0)
        {
            cerr << "FATAL ERROR TwentySevenNodeBrickOrderFour (tag: " << this->getTag() << "), node not found in domain\n";
            exit( -1 );
        }

        int dofNd1 = theNodes[0]->getNumberDOF();
        int dofNd2 = theNodes[1]->getNumberDOF();
        int dofNd3 = theNodes[2]->getNumberDOF();
        int dofNd4 = theNodes[3]->getNumberDOF();

        int dofNd5 = theNodes[4]->getNumberDOF();
        int dofNd6 = theNodes[5]->getNumberDOF();
        int dofNd7 = theNodes[6]->getNumberDOF();
        int dofNd8 = theNodes[7]->getNumberDOF();

        int dofNd9 = theNodes[8]->getNumberDOF();
        int dofNd10 = theNodes[9]->getNumberDOF();
        int dofNd11 = theNodes[10]->getNumberDOF();
        int dofNd12 = theNodes[11]->getNumberDOF();

        int dofNd13 = theNodes[12]->getNumberDOF();
        int dofNd14 = theNodes[13]->getNumberDOF();
        int dofNd15 = theNodes[14]->getNumberDOF();
        int dofNd16 = theNodes[15]->getNumberDOF();

        int dofNd17 = theNodes[16]->getNumberDOF();
        int dofNd18 = theNodes[17]->getNumberDOF();
        int dofNd19 = theNodes[18]->getNumberDOF();
        int dofNd20 = theNodes[19]->getNumberDOF();

        int dofNd21 = theNodes[20]->getNumberDOF();
        int dofNd22 = theNodes[21]->getNumberDOF();
        int dofNd23 = theNodes[22]->getNumberDOF();
        int dofNd24 = theNodes[23]->getNumberDOF();
        int dofNd25 = theNodes[24]->getNumberDOF();
        int dofNd26 = theNodes[25]->getNumberDOF();
        int dofNd27 = theNodes[26]->getNumberDOF();

        if ( dofNd1 != 3 || dofNd2 != 3 || dofNd3 != 3 || dofNd4 != 3 ||
                dofNd5 != 3 || dofNd6 != 3 || dofNd7 != 3 || dofNd8 != 3 ||
                dofNd9  != 3 || dofNd10 != 3 || dofNd11 != 3 || dofNd12 != 3 ||
                dofNd13 != 3 || dofNd14 != 3 || dofNd15 != 3 || dofNd16 != 3 ||
                dofNd17 != 3 || dofNd18 != 3 || dofNd19 != 3 || dofNd20 != 3 ||
                dofNd21 != 3 || dofNd22 != 3 || dofNd23 != 3 || dofNd24 != 3 ||
                dofNd25 != 3 || dofNd26 != 3 || dofNd27 != 3)
        {
            cerr << "FATAL ERROR TwentySevenNodeBrickOrderFour (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit( -1 );
        }

        this->DomainComponent::setDomain( theDomain );
    }

    ComputeVolume();
    computeGaussPoint();

}

//=============================================================================
int TwentySevenNodeBrickOrderFour::commitState ()
{

    int retVal = 0;

    // call element commitState to do any base class stuff
    if ( ( retVal = this->Element::commitState() ) != 0 )
    {
        cerr << "TwentySevenNodeBrickOrderFour::commitState () - failed in base class";
    }

    Vector pp = getResistingForce();

    for (int ii = 0; ii < Num_TotalGaussPts; ii++ )
    {
        retVal += material_array[ii]->commitState();
    }

    return retVal;
}

//=============================================================================
int TwentySevenNodeBrickOrderFour::revertToLastCommit ()
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
int TwentySevenNodeBrickOrderFour::revertToStart ()
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
const Matrix &TwentySevenNodeBrickOrderFour::getTangentStiff()
{

    /*** Sumeet [September, 2016] check for elastic material [see Element.cpp]*****/
    /* -1 means not an elastic material */
    /*  0 means elastic material but stiffness has not been calculated yet*/
    /*  1 means elastic material and stiffness has been calculated */
    switch(getElasticMaterialStatus()){

        case -1: // not an elastic material
                break;
        case 0: // elastic material but stiffness not calculated
                setElasticMaterialStatus(1);
                break;
        case 1: // elstic material and stiffness has allready been calculated 
                return K;
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

    //Set the stiffness tensor to zero (its static!)
    K.Zero();
    int Ki = 0;
    int Kj = 0;

    for ( int i = 1 ; i <= 27 ; i++ )
    {
        for ( int j = 1 ; j <= 27 ; j++ )
        {
            for ( int k = 1 ; k <= 3 ; k++ )
            {
                for ( int l = 1 ; l <= 3 ; l++ )
                {
                    Ki = k + 3 * (i - 1);
                    Kj = l + 3 * (j - 1);
                    K( Ki - 1 , Kj - 1 ) = stifftensor(i - 1, k - 1, l - 1, j - 1);
                }
            }
        }
    }

     return K;
}

//=============================================================================
const Matrix &TwentySevenNodeBrickOrderFour::getInitialStiff ()
{

    if ( Ki == 0 )
    {
        Ki = new Matrix( this->getTangentStiff() );
    }

    if ( Ki == 0 )
    {
        cerr << "FATAL TwentySevenNodeBrickOrderFour::getInitialStiff() -";
        cerr << "ran out of memory\n";
        exit( -1 );
    }

    return *Ki;
}

//=============================================================================

const Matrix &TwentySevenNodeBrickOrderFour::getMass ()
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

        for ( int ii = 1 ; ii <= 81 ; ii++ )
        {
            for ( int jj = 1 ; jj <= 81 ; jj++ )
            {
                M( ii - 1 , jj - 1 ) = Mm( ii - 1 , jj - 1 );
            }

        }

        is_mass_computed = true;
    }

    return M;
}

//=============================================================================
void TwentySevenNodeBrickOrderFour::zeroLoad( void )
{
    Q.Zero();
}



//======================================================================
const Vector &TwentySevenNodeBrickOrderFour::getBodyForce( double loadFactor, const Vector &data )
{

    static Vector bforce( 81 );
    bforce.Zero();


    // Check for a quick return
    if ( rho == 0.0 )
    {
        return bforce;
    }

    Vector ba( 81 ), bfx( 3 );

    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);


    bfx( 0 ) = bf( 0 ) * loadFactor;
    bfx( 1 ) = bf( 1 ) * loadFactor;
    bfx( 2 ) = bf( 2 ) * loadFactor;

    ba( 0 ) =  bfx( 0 );
    ba( 1 ) =  bfx( 1 );
    ba( 2 ) =  bfx( 2 );
    ba( 3 ) =  bfx( 0 );
    ba( 4 ) =  bfx( 1 );
    ba( 5 ) =  bfx( 2 );
    ba( 6 ) =  bfx( 0 );
    ba( 7 ) =  bfx( 1 );
    ba( 8 ) =  bfx( 2 );
    ba( 9 ) =  bfx( 0 );
    ba( 10 ) = bfx( 1 );
    ba( 11 ) = bfx( 2 );
    ba( 12 ) = bfx( 0 );
    ba( 13 ) = bfx( 1 );
    ba( 14 ) = bfx( 2 );
    ba( 15 ) = bfx( 0 );
    ba( 16 ) = bfx( 1 );
    ba( 17 ) = bfx( 2 );
    ba( 18 ) = bfx( 0 );
    ba( 19 ) = bfx( 1 );
    ba( 20 ) = bfx( 2 );
    ba( 21 ) = bfx( 0 );
    ba( 22 ) = bfx( 1 );
    ba( 23 ) = bfx( 2 );
    ba(24) = bfx(0);
    ba(25) = bfx(1);
    ba(26) = bfx(2);
    ba(27) = bfx(0);
    ba(28) = bfx(1);
    ba(29) = bfx(2);
    ba(30) = bfx(0);
    ba(31) = bfx(1);
    ba(32) = bfx(2);
    ba(33) = bfx(0);
    ba(34) = bfx(1);
    ba(35) = bfx(2);
    ba(36) = bfx(0);
    ba(37) = bfx(1);
    ba(38) = bfx(2);
    ba(39) = bfx(0);
    ba(40) = bfx(1);
    ba(41) = bfx(2);
    ba(42) = bfx(0);
    ba(43) = bfx(1);
    ba(44) = bfx(2);
    ba(45) = bfx(0);
    ba(46) = bfx(1);
    ba(47) = bfx(2);
    ba(48) = bfx(0);
    ba(49) = bfx(1);
    ba(50) = bfx(2);
    ba(51) = bfx(0);
    ba(52) = bfx(1);
    ba(53) = bfx(2);
    ba(54) = bfx(0);
    ba(55) = bfx(1);
    ba(56) = bfx(2);
    ba(57) = bfx(0);
    ba(58) = bfx(1);
    ba(59) = bfx(2);
    ba(60) = bfx(0);
    ba(61) = bfx(1);
    ba(62) = bfx(2);
    ba(63) = bfx(0);
    ba(64) = bfx(1);
    ba(65) = bfx(2);
    ba(66) = bfx(0);
    ba(67) = bfx(1);
    ba(68) = bfx(2);
    ba(69) = bfx(0);
    ba(70) = bfx(1);
    ba(71) = bfx(2);
    ba(72) = bfx(0);
    ba(73) = bfx(1);
    ba(74) = bfx(2);
    ba(75) = bfx(0);
    ba(76) = bfx(1);
    ba(77) = bfx(2);
    ba(78) = bfx(0);
    ba(79) = bfx(1);
    ba(80) = bfx(2);

    //Form equivalent body force
    this->getMass();

    bforce.addMatrixVector( 0.0, M, ba, 1.0 );

    return bforce;
}


//======================================================================
const Vector &TwentySevenNodeBrickOrderFour::getSurfaceForce( double loadFactor, const Vector &data )
{

    map<int,int> local_nodes_map; int local_nodes[9];
    //note -> the user at least should start with a node on the corner edge (sumeet)
    
    /////////////////////////////////////////// Edited by Sumeet 30/03/2016 //////////////////////////////
    // checking if node exists in the element
    for ( int i =0; i<9 ;i++){
        std::map<int,int>::iterator it;
        it=Global_to_Local_Node_Mapping.find(data(i));
        if (it == Global_to_Local_Node_Mapping.end()){
            cerr << "\nERROR: Node " <<  data(i) << " defined for the BrickSurfaceLoad does not belong to element \n" ;
            exit( 1 );
        }
        local_nodes_map[it->second]=i;
        local_nodes[i]=it->second;
    }

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
                success=0;break;
            }
            success=success+1;
            surface_nodes_order[j]= it->second;
        }
        if(success==9) break;
    }
    if (success == 0){
        cerr << "\nERROR: Nodes  defined for the BrickSurfaceLoad does not belong to elements surface  \n" ;
        exit( 1 );
    }      
    // //////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    // cout << "surface_nodes_order ";
    // for ( int i =0; i < 9 ; i++)
    //     cout << local_nodes[i] << " ";
    //     // cout << surface_nodes_order[i] << " ";
    // cout << "\n";
    // for ( int i =0; i < 9 ; i++)
    //     cout << local_nodes[surface_nodes_order[i]] << " ";
    // cout << "\n";
    //////////////////////////////////////////////////////////////////////////////////////////////////

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

    static Vector NodalForces( 81 );

    for ( int m = 0; m < 81; m++ )
    {
        NodalForces( m ) = 0;
    }

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
            r = local_nodes[(surface_nodes_order[j])] ;
            // loop over Gauss points
            for ( int i = 0; i < Order*Order; i++ )
            {
                ShapeFunctionValues = SurfaceShapeFunctionValues( GsPts( i, 0 ) , GsPts( i, 1 ), j );
                J_vector = Direction_Weight( GsPts( i, 0 ) , GsPts( i, 1 ), coordnode1, coordnode2, coordnode3, coordnode4, coordnode5, coordnode6, coordnode7, coordnode8, coordnode9 );
                LoadValue = SurfaceLoadValues( GsPts( i, 0 ) , GsPts( i, 1 ), Pressure );
                NodalForces( r * 3 + k ) = NodalForces( r * 3 + k ) + LoadValue * J_vector( k ) * ShapeFunctionValues * Weight(i, 0) * Weight(i, 1);
            }
        }
    }

    // ////////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    // for ( int i =0; i < 27 ; i++)
    //     cout << NodalForces(3*i)<< " " << NodalForces(3*i+1) << " " << NodalForces(3*i+2) <<   "\n";
    // cout << "\n\n******************************************************************************\n\n";
    // /////////////////////////////////////////////////////////////////////////////////////////////////


    return NodalForces;
}

//=============================================================================
int TwentySevenNodeBrickOrderFour::addLoad( ElementalLoad *theLoad, double loadFactor )
{


    int type;
    const Vector &data = theLoad->getData( type, loadFactor );

    if ( type == LOAD_TAG_ElementSelfWeight )
    {
        Vector Fbody = this->getBodyForce( loadFactor , data);
        Q.addVector( 1.0, Fbody, 1.0 );
    }
    else if ( type == LOAD_TAG_BrickSurfaceLoad )
    {
        Vector Fsurface = this->getSurfaceForce( loadFactor, data );
        Q.addVector( 1.0, Fsurface, 1.0 );
    }
    else
    {
        cerr << "TwentySevenNodeBrickOrderFour::addLoad() - 8NodeBrick " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }


    return 0;
}




//=============================================================================
int TwentySevenNodeBrickOrderFour::addInertiaLoadToUnbalance( const Vector &accel )
{
    // Check for a quick return
    if ( rho == 0.0 )
    {
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV( accel );
    const Vector &Raccel2 = theNodes[1]->getRV( accel );
    const Vector &Raccel3 = theNodes[2]->getRV( accel );
    const Vector &Raccel4 = theNodes[3]->getRV( accel );
    const Vector &Raccel5 = theNodes[4]->getRV( accel );
    const Vector &Raccel6 = theNodes[5]->getRV( accel );
    const Vector &Raccel7 = theNodes[6]->getRV( accel );
    const Vector &Raccel8 = theNodes[7]->getRV( accel );
    const Vector &Raccel9  = theNodes[8]->getRV(accel);
    const Vector &Raccel10 = theNodes[9]->getRV(accel);
    const Vector &Raccel11 = theNodes[10]->getRV(accel);
    const Vector &Raccel12 = theNodes[11]->getRV(accel);
    const Vector &Raccel13 = theNodes[12]->getRV(accel);
    const Vector &Raccel14 = theNodes[13]->getRV(accel);
    const Vector &Raccel15 = theNodes[14]->getRV(accel);
    const Vector &Raccel16 = theNodes[15]->getRV(accel);
    const Vector &Raccel17 = theNodes[16]->getRV(accel);
    const Vector &Raccel18 = theNodes[17]->getRV(accel);
    const Vector &Raccel19 = theNodes[18]->getRV(accel);
    const Vector &Raccel20 = theNodes[19]->getRV(accel);
    const Vector &Raccel21 = theNodes[20]->getRV(accel);
    const Vector &Raccel22 = theNodes[21]->getRV(accel);
    const Vector &Raccel23 = theNodes[22]->getRV(accel);
    const Vector &Raccel24 = theNodes[23]->getRV(accel);
    const Vector &Raccel25 = theNodes[24]->getRV(accel);
    const Vector &Raccel26 = theNodes[25]->getRV(accel);
    const Vector &Raccel27 = theNodes[26]->getRV(accel);


    if ( 3 != Raccel1.Size() || 3 != Raccel2.Size() || 3 != Raccel3.Size() ||
            3 != Raccel4.Size() || 3 != Raccel5.Size() || 3 != Raccel6.Size() ||
            3 != Raccel7.Size() || 3 != Raccel8.Size() ||
            3 != Raccel9.Size()  || 3 != Raccel10.Size() || 3 != Raccel11.Size() || 3 != Raccel12.Size() ||
            3 != Raccel13.Size() || 3 != Raccel14.Size() || 3 != Raccel15.Size() || 3 != Raccel16.Size() ||
            3 != Raccel17.Size() || 3 != Raccel18.Size() || 3 != Raccel19.Size() || 3 != Raccel20.Size() ||
            3 != Raccel21.Size() || 3 != Raccel22.Size() || 3 != Raccel23.Size() || 3 != Raccel24.Size() ||
            3 != Raccel25.Size() || 3 != Raccel26.Size() || 3 != Raccel27.Size() )
    {
        cerr << "TwentySevenNodeBrickOrderFour::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }

    static Vector ra( 81 );

    ra( 0 ) =  Raccel1( 0 );
    ra( 1 ) =  Raccel1( 1 );
    ra( 2 ) =  Raccel1( 2 );
    ra( 3 ) =  Raccel2( 0 );
    ra( 4 ) =  Raccel2( 1 );
    ra( 5 ) =  Raccel2( 2 );
    ra( 6 ) =  Raccel3( 0 );
    ra( 7 ) =  Raccel3( 1 );
    ra( 8 ) =  Raccel3( 2 );
    ra( 9 ) =  Raccel4( 0 );
    ra( 10 ) = Raccel4( 1 );
    ra( 11 ) = Raccel4( 2 );
    ra( 12 ) = Raccel5( 0 );
    ra( 13 ) = Raccel5( 1 );
    ra( 14 ) = Raccel5( 2 );
    ra( 15 ) = Raccel6( 0 );
    ra( 16 ) = Raccel6( 1 );
    ra( 17 ) = Raccel6( 2 );
    ra( 18 ) = Raccel7( 0 );
    ra( 19 ) = Raccel7( 1 );
    ra( 20 ) = Raccel7( 2 );
    ra( 21 ) = Raccel8( 0 );
    ra( 22 ) = Raccel8( 1 );
    ra( 23 ) = Raccel8( 2 );
    ra(24) = Raccel9(0);
    ra(25) = Raccel9(1);
    ra(26) = Raccel9(2);
    ra(27) = Raccel10(0);
    ra(28) = Raccel10(1);
    ra(29) = Raccel10(2);
    ra(30) = Raccel11(0);
    ra(31) = Raccel11(1);
    ra(32) = Raccel11(2);
    ra(33) = Raccel12(0);
    ra(34) = Raccel12(1);
    ra(35) = Raccel12(2);
    ra(36) = Raccel13(0);
    ra(37) = Raccel13(1);
    ra(38) = Raccel13(2);
    ra(39) = Raccel14(0);
    ra(40) = Raccel14(1);
    ra(41) = Raccel14(2);
    ra(42) = Raccel15(0);
    ra(43) = Raccel15(1);
    ra(44) = Raccel15(2);
    ra(45) = Raccel16(0);
    ra(46) = Raccel16(1);
    ra(47) = Raccel16(2);
    ra(48) = Raccel17(0);
    ra(49) = Raccel17(1);
    ra(50) = Raccel17(2);
    ra(51) = Raccel18(0);
    ra(52) = Raccel18(1);
    ra(53) = Raccel18(2);
    ra(54) = Raccel19(0);
    ra(55) = Raccel19(1);
    ra(56) = Raccel19(2);
    ra(57) = Raccel20(0);
    ra(58) = Raccel20(1);
    ra(59) = Raccel20(2);
    ra(60) = Raccel21(0);
    ra(61) = Raccel21(1);
    ra(62) = Raccel21(2);
    ra(63) = Raccel22(0);
    ra(64) = Raccel22(1);
    ra(65) = Raccel22(2);
    ra(66) = Raccel23(0);
    ra(67) = Raccel23(1);
    ra(68) = Raccel23(2);
    ra(69) = Raccel24(0);
    ra(70) = Raccel24(1);
    ra(71) = Raccel24(2);
    ra(72) = Raccel25(0);
    ra(73) = Raccel25(1);
    ra(74) = Raccel25(2);
    ra(75) = Raccel26(0);
    ra(76) = Raccel26(1);
    ra(77) = Raccel26(2);
    ra(78) = Raccel27(0);
    ra(79) = Raccel27(1);
    ra(80) = Raccel27(2);

    Q.addMatrixVector( 1.0, M, ra, -1.0 );
    return 0;
}

//=============================================================================
const Vector TwentySevenNodeBrickOrderFour::FormEquiBodyForce(const Vector &data)
{
    static Vector bforce( 81 );
    bforce.Zero();

    // Check for a quick return
    if ( rho == 0.0 )
    {
        return bforce;
    }

    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);
    Vector ba( 81 );

    ba( 0) = bf(0);
    ba( 1) = bf(1);
    ba( 2) = bf(2);
    ba( 3) = bf(0);
    ba( 4) = bf(1);
    ba( 5) = bf(2);
    ba( 6) = bf(0);
    ba( 7) = bf(1);
    ba( 8) = bf(2);
    ba( 9) = bf(0);
    ba(10) = bf(1);
    ba(11) = bf(2);
    ba(12) = bf(0);
    ba(13) = bf(1);
    ba(14) = bf(2);
    ba(15) = bf(0);
    ba(16) = bf(1);
    ba(17) = bf(2);
    ba(18) = bf(0);
    ba(19) = bf(1);
    ba(20) = bf(2);
    ba(21) = bf(0);
    ba(22) = bf(1);
    ba(23) = bf(2);
    ba(24) = bf(0);
    ba(25) = bf(1);
    ba(26) = bf(2);
    ba(27) = bf(0);
    ba(28) = bf(1);
    ba(29) = bf(2);
    ba(30) = bf(0);
    ba(31) = bf(1);
    ba(32) = bf(2);
    ba(33) = bf(0);
    ba(34) = bf(1);
    ba(35) = bf(2);
    ba(36) = bf(0);
    ba(37) = bf(1);
    ba(38) = bf(2);
    ba(39) = bf(0);
    ba(40) = bf(1);
    ba(41) = bf(2);
    ba(42) = bf(0);
    ba(43) = bf(1);
    ba(44) = bf(2);
    ba(45) = bf(0);
    ba(46) = bf(1);
    ba(47) = bf(2);
    ba(48) = bf(0);
    ba(49) = bf(1);
    ba(50) = bf(2);
    ba(51) = bf(0);
    ba(52) = bf(1);
    ba(53) = bf(2);
    ba(54) = bf(0);
    ba(55) = bf(1);
    ba(56) = bf(2);
    ba(57) = bf(0);
    ba(58) = bf(1);
    ba(59) = bf(2);
    ba(60) = bf(0);
    ba(61) = bf(1);
    ba(62) = bf(2);
    ba(63) = bf(0);
    ba(64) = bf(1);
    ba(65) = bf(2);
    ba(66) = bf(0);
    ba(67) = bf(1);
    ba(68) = bf(2);
    ba(69) = bf(0);
    ba(70) = bf(1);
    ba(71) = bf(2);
    ba(72) = bf(0);
    ba(73) = bf(1);
    ba(74) = bf(2);
    ba(75) = bf(0);
    ba(76) = bf(1);
    ba(77) = bf(2);
    ba(78) = bf(0);
    ba(79) = bf(1);
    ba(80) = bf(2);

    //Form equivalent body force
    bforce.addMatrixVector( 0.0, M, ba, 1.0 );

    return bforce;
}


//=============================================================================
const Vector &TwentySevenNodeBrickOrderFour::getResistingForce()
{
    static DTensor2 nodalforces( 27, 3, 0.0 );
    nodalforces *= 0;
    nodalforces = nodal_forces();

    //converting nodalforce tensor to vector
    for ( int i = 0; i < 27; i++ )
        for ( int j = 0; j < 3; j++ )
        {
            P( i * 3 + j ) = nodalforces( i, j );
        }

    P.addVector( 1.0, Q, -1.0 );

    return P;
}

//=============================================================================
const Vector &TwentySevenNodeBrickOrderFour::getResistingForceIncInertia ()
{
    // form resisting force
    this->getResistingForce();

    //
    // now add dynamic terms
    // P += M * a + C * v
    //

    if ( rho != 0.0 )
    {

        // form the mass matrix
        this->getMass();

        const Vector &accel1 = theNodes[0]->getTrialAccel();
        const Vector &accel2 = theNodes[1]->getTrialAccel();
        const Vector &accel3 = theNodes[2]->getTrialAccel();
        const Vector &accel4 = theNodes[3]->getTrialAccel();
        const Vector &accel5 = theNodes[4]->getTrialAccel();
        const Vector &accel6 = theNodes[5]->getTrialAccel();
        const Vector &accel7 = theNodes[6]->getTrialAccel();
        const Vector &accel8 = theNodes[7]->getTrialAccel();
        const Vector &accel9 = theNodes[8]->getTrialAccel();
        const Vector &accel10 = theNodes[9]->getTrialAccel();
        const Vector &accel11 = theNodes[10]->getTrialAccel();
        const Vector &accel12 = theNodes[11]->getTrialAccel();
        const Vector &accel13 = theNodes[12]->getTrialAccel();
        const Vector &accel14 = theNodes[13]->getTrialAccel();
        const Vector &accel15 = theNodes[14]->getTrialAccel();
        const Vector &accel16 = theNodes[15]->getTrialAccel();
        const Vector &accel17 = theNodes[16]->getTrialAccel();
        const Vector &accel18 = theNodes[17]->getTrialAccel();
        const Vector &accel19 = theNodes[18]->getTrialAccel();
        const Vector &accel20 = theNodes[19]->getTrialAccel();
        const Vector &accel21 = theNodes[20]->getTrialAccel();
        const Vector &accel22 = theNodes[21]->getTrialAccel();
        const Vector &accel23 = theNodes[22]->getTrialAccel();
        const Vector &accel24 = theNodes[23]->getTrialAccel();
        const Vector &accel25 = theNodes[24]->getTrialAccel();
        const Vector &accel26 = theNodes[25]->getTrialAccel();
        const Vector &accel27 = theNodes[26]->getTrialAccel();

        static Vector a( 81 ); // originally 8

        a( 0 ) =  accel1( 0 );
        a( 1 ) =  accel1( 1 );
        a( 2 ) =  accel1( 2 );
        a( 3 ) =  accel2( 0 );
        a( 4 ) =  accel2( 1 );
        a( 5 ) =  accel2( 2 );
        a( 6 ) =  accel3( 0 );
        a( 7 ) =  accel3( 1 );
        a( 8 ) =  accel3( 2 );
        a( 9 ) =  accel4( 0 );
        a( 10 ) = accel4( 1 );
        a( 11 ) = accel4( 2 );
        a( 12 ) = accel5( 0 );
        a( 13 ) = accel5( 1 );
        a( 14 ) = accel5( 2 );
        a( 15 ) = accel6( 0 );
        a( 16 ) = accel6( 1 );
        a( 17 ) = accel6( 2 );
        a( 18 ) = accel7( 0 );
        a( 19 ) = accel7( 1 );
        a( 20 ) = accel7( 2 );
        a( 21 ) = accel8( 0 );
        a( 22 ) = accel8( 1 );
        a( 23 ) = accel8( 2 );
        a(24) = accel9(0);
        a(25) = accel9(1);
        a(26) = accel9(2);
        a(27) = accel10(0);
        a(28) = accel10(1);
        a(29) = accel10(2);
        a(30) = accel11(0);
        a(31) = accel11(1);
        a(32) = accel11(2);
        a(33) = accel12(0);
        a(34) = accel12(1);
        a(35) = accel12(2);
        a(36) = accel13(0);
        a(37) = accel13(1);
        a(38) = accel13(2);
        a(39) = accel14(0);
        a(40) = accel14(1);
        a(41) = accel14(2);
        a(42) = accel15(0);
        a(43) = accel15(1);
        a(44) = accel15(2);
        a(45) = accel16(0);
        a(46) = accel16(1);
        a(47) = accel16(2);
        a(48) = accel17(0);
        a(49) = accel17(1);
        a(50) = accel17(2);
        a(51) = accel18(0);
        a(52) = accel18(1);
        a(53) = accel18(2);
        a(54) = accel19(0);
        a(55) = accel19(1);
        a(56) = accel19(2);
        a(57) = accel20(0);
        a(58) = accel20(1);
        a(59) = accel20(2);
        a(60) = accel21(0);
        a(61) = accel21(1);
        a(62) = accel21(2);
        a(63) = accel22(0);
        a(64) = accel22(1);
        a(65) = accel22(2);
        a(66) = accel23(0);
        a(67) = accel23(1);
        a(68) = accel23(2);
        a(69) = accel24(0);
        a(70) = accel24(1);
        a(71) = accel24(2);
        a(72) = accel25(0);
        a(73) = accel25(1);
        a(74) = accel25(2);
        a(75) = accel26(0);
        a(76) = accel26(1);
        a(77) = accel26(2);
        a(78) = accel27(0);
        a(79) = accel27(1);
        a(80) = accel27(2);

        // P += M * a
        P.addMatrixVector( 1.0, M, a, 1.0 );

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




int TwentySevenNodeBrickOrderFour::sendSelf ( int commitTag, Channel &theChannel )
{

    ID idData(3);

    idData( 0 ) = this->getTag();
    idData( 1 ) = material_array[0]->getClassTag();
    idData( 2 ) = getElasticMaterialStatus();

    if ( theChannel.sendID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send ID idData\n";
        return -1;
    }

    // Sending the Stiffness matrix for elastic material
    if(idData( 2 )==1){
        if ( theChannel.sendMatrix( 0, commitTag, K ) < 0 )
        {
            cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send its K\n";
            return -1;
        }
    }

    // send double data
    Vector floatData(7);
    floatData(0) = Volume;
    floatData(1) = determinant_of_Jacobian;
    floatData(2) = rho;
    floatData(3) = a0;
    floatData(4) = a1;
    floatData(5) = a2;
    floatData(6) = a3;

    if ( theChannel.sendVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }


    string tmp_string;

    tmp_string = stiffness_type;
    if ( theChannel.sendString( 0, commitTag, tmp_string ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send String stiffness_type\n";
        return -1;
    }

    tmp_string = damping_type;
    if ( theChannel.sendString( 0, commitTag, damping_type ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send String damping_type\n";
        return -1;
    }


    // Send the nodes

    if ( theChannel.sendID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send ID connectedExternalNodes\n";
        return -1;
    }


    // material objects send themselves

    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {
        if ( material_array[i]->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send material models\n";
            return -1;
        }
    }


    //Send Q
    if ( theChannel.sendVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send its Q\n";
        return -1;
    }

    //Send bf
    if ( theChannel.sendVector( 0, commitTag, bf ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::sendSelf() - " << this->getTag() << " failed to send its bf\n";
        return -1;
    }

    return 0;
}

int TwentySevenNodeBrickOrderFour::receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
{
    ID idData( 3 );

    if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrick::receiveSelf() - failed to receive ID\n";
        return -1;
    }

    this->setTag( idData( 0 ) );
    int matClassTag = idData( 1 );
    setElasticMaterialStatus(idData(2));

    // Recieve the Stiffness matrix for elastic material
    if(idData(2)){
        if ( theChannel.sendMatrix( 0, commitTag, K ) < 0 )
        {
            cerr << "WARNING TwentyNodeBrick::receiveSelf() - " << this->getTag() << " failed to recieve its K\n";
            return -1;
        }
    }


    Vector floatData(7);
    if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }
    Volume                  = floatData(0) ;
    determinant_of_Jacobian = floatData(1) ;
    rho                     = floatData(2) ;
    a0 = floatData(3);
    a1 = floatData(4);
    a2 = floatData(5);
    a3 = floatData(6);

    string tmp_string;
    if ( theChannel.receiveString( 0, commitTag,  tmp_string) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::receiveSelf() - " << this->getTag() << " failed to recieve String stiffness_type\n";
        return -1;
    }
    stiffness_type = tmp_string;

    if ( theChannel.receiveString( 0, commitTag, tmp_string ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::receiveSelf() - " << this->getTag() << " failed to recieve String damping_type\n";
        return -1;
    }
    damping_type = tmp_string;


    // Recieve the nodes

    if ( theChannel.receiveID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING TwentySevenNodeBrickOrderFour::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
        return -1;
    }

    // cout << "TwentySevenNodeBrickOrderFour::receiveSelf() connectedExternalNodes = " << connectedExternalNodes << "\n";

    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {

        // Allocate new material with the sent class tag
        NDMaterialLT *ndmat = theBroker.getNewNDMaterialLT( matClassTag );
        if ( ndmat == 0 )
        {
            cerr << "TwentySevenNodeBrickOrderFour::receiveSelf() - Broker could not create NDMaterialLT of class type " << matClassTag << "\n";
            return -1;
        }

        // Now receive materials into the newly allocated space
        if ( ( ndmat )->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
        {
            cerr << "TwentySevenNodeBrickOrderFour::receiveSelf() - material " << i << "failed to recv itself\n";
            return -1;
        }

        material_array[i] = ndmat;
    }


    // Q
    if ( theChannel.receiveVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "TwentySevenNodeBrickOrderFour::receiveSelf() - failed to recv Q!\n";
        return -1;
    }

    // bf
    if ( theChannel.receiveVector( 0, commitTag, bf ) < 0 )
    {
        cerr << "TwentySevenNodeBrickOrderFour::receiveSelf() - failed to recv bf!\n";
        return -1;
    }


    return 0;
}


int TwentySevenNodeBrickOrderFour::getObjectSize()
{
    int size = 0;
    // Just add all sizes of data-members (there is some static local variables too.... but those are not important for transference)
    //
    // bool is_mass_computed;
    size += sizeof(is_mass_computed);
    // double Volume;
    size += sizeof(Volume);
    // double determinant_of_Jacobian;
    size += sizeof(determinant_of_Jacobian);
    // double rho;
    size += sizeof(rho);
    // ID  connectedExternalNodes;
    size += sizeof(int) * connectedExternalNodes.Size();
    // Matrix *Ki;
    size += sizeof(Ki);
    // Node *theNodes[8];
    size += sizeof(theNodes) ;
    // NDMaterialLT *mmodel;
    size += sizeof(mmodel);
    // NDMaterialLT *material_array[8];
    size += sizeof(material_array) * Num_TotalGaussPts;
    // Vector Q;
    size += sizeof(double) * Q.Size();
    // Vector bf;
    size += sizeof(double) * bf.Size();
    // Matrix gauss_points   NOT INCLUDED IN MEMORY COUNT... THEY're Static (J. Abell);
    // size += sizeof(gauss_points);
    // // Vector Gauss_Output_Vector;
    // size += sizeof(Gauss_Output_Vector);
    // Index < 'i' > i;
    size += sizeof(i);
    // Index < 'j' > j;
    size += sizeof(j);
    // Index < 'k' > k;
    size += sizeof(k);
    // Index < 'l' > l;
    size += sizeof(l);

    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {
        size += material_array[i]->getObjectSize();
    }

    return size;
}

//=============================================================================
void TwentySevenNodeBrickOrderFour::Print( ostream &s, int flag )
{
    cout << "TwentySevenNodeBrickOrderFour: LTensor-based 27 node brick." << endl << endl;

    cout << "Element tag: " << getTag() << endl;
    cout << "Connected Nodes: " << endl;

    for ( int node = 0; node < 27; node++)
    {
        cout << "   #" << node + 1 << ": Domain node #" << connectedExternalNodes(node) << endl;
    }

    cout << "K = " << endl;

    for (int ii = 0; ii < 81; ii++)
    {
        for (int jj = 0; jj < 81; jj++)
        {
            cout << K(ii, jj);

            if (jj < 80)
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

    for (int ii = 0; ii < 81; ii++)
    {
        cout << K(ii, ii);

        if (ii < 80)
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



void TwentySevenNodeBrickOrderFour::ComputeVolume()
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



int TwentySevenNodeBrickOrderFour::update( void )
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
            cerr << "TwentySevenNodeBrickOrderFour::update (tag: " << this->getTag() << "), Update Failed\n";
            return -1;
        }
    }


    return 0;
}



/////////////////////////////////////////////////////////////////////////
//NOTE: Can me templatized to improve performance
double TwentySevenNodeBrickOrderFour::SurfaceShapeFunctionValues( double Xi , double Eta, int whichcomponent )
{
    ShapeFunctionValues_in_function(0) = 0.25 * (1 + Xi) * (1 + Eta) - 0.25 * (1 - Xi * Xi) * (1 + Eta)  - 0.25 * (1 - Eta * Eta) * (1 + Xi)  + 0.25 * (1 - Eta * Eta) * (1 - Xi * Xi);
    ShapeFunctionValues_in_function(1) = 0.25 * (1 - Xi) * (1 + Eta) - 0.25 * (1 - Xi * Xi) * (1 + Eta)  - 0.25 * (1 - Eta * Eta) * (1 - Xi)  + 0.25 * (1 - Eta * Eta) * (1 - Xi * Xi);
    ShapeFunctionValues_in_function(2) = 0.25 * (1 - Xi) * (1 - Eta) - 0.25 * (1 - Eta * Eta) * (1 - Xi) - 0.25 * (1 - Xi * Xi) * (1 - Eta)   + 0.25 * (1 - Eta * Eta) * (1 - Xi * Xi);
    ShapeFunctionValues_in_function(3) = 0.25 * (1 + Xi) * (1 - Eta) - 0.25 * (1 - Xi * Xi) * (1 - Eta)  - 0.25 * (1 - Eta * Eta) * (1 + Xi) + 0.25 * (1 - Eta * Eta) * (1 - Xi * Xi);
    ShapeFunctionValues_in_function(4) = 0.5 * (1 - Xi * Xi) * (1 + Eta)  - 0.5 * (1 - Eta * Eta) * (1 - Xi * Xi);
    ShapeFunctionValues_in_function(5) = 0.5 * (1 - Eta * Eta) * (1 - Xi) - 0.5 * (1 - Eta * Eta) * (1 - Xi * Xi);
    ShapeFunctionValues_in_function(6) = 0.5 * (1 - Xi * Xi) * (1 - Eta)  - 0.5 * (1 - Eta * Eta) * (1 - Xi * Xi);
    ShapeFunctionValues_in_function(7) = 0.5 * (1 - Eta * Eta) * (1 + Xi) - 0.5 * (1 - Eta * Eta) * (1 - Xi * Xi);
    ShapeFunctionValues_in_function(8) = (1 - Eta * Eta) * (1 - Xi * Xi);

    return ShapeFunctionValues_in_function( whichcomponent );
}


Vector &TwentySevenNodeBrickOrderFour::Direction_Weight( double Xi , double Eta, Vector coord1, Vector coord2,
        Vector coord3, Vector coord4, Vector coord5,
        Vector coord6, Vector coord7, Vector coord8,
        Vector coord9 )
{
    static Vector J1( 3 );
    static Vector J2( 3 );

    J1( 0 ) = 0.25 * ( (1 + Eta) + (1 + Eta) * 2 * Xi - (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta) ) * coord1(0)
              + 0.25 * ( -(1 + Eta) + (1 + Eta) * 2 * Xi + (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta) ) * coord2(0)
              + 0.25 * ( -(1 - Eta) + (1 - Eta * Eta) + (1 - Eta) * 2 * Xi - 2 * Xi * (1 - Eta * Eta) ) * coord3(0)
              + 0.25 * ( (1 - Eta) + (1 - Eta) * 2 * Xi - (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta)  ) * coord4(0)
              + 0.5  * ( -2 * Xi * (1 + Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord5(0)
              + 0.5  * ( -(1 - Eta * Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord6(0)
              + 0.5  * ( -2 * Xi * (1 - Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord7(0)
              + 0.5 * ( (1 - Eta * Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord8(0)
              - 2 * Xi * (1 - Eta * Eta) * coord9(0);


    J1(1) = 0.25 * ( (1 + Eta) + (1 + Eta) * 2 * Xi - (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta) ) * coord1(1)
            + 0.25 * ( -(1 + Eta) + (1 + Eta) * 2 * Xi + (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta) ) * coord2(1)
            + 0.25 * ( -(1 - Eta) + (1 - Eta * Eta) + (1 - Eta) * 2 * Xi - 2 * Xi * (1 - Eta * Eta) ) * coord3(1)
            + 0.25 * ( (1 - Eta) + (1 - Eta) * 2 * Xi - (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta)  ) * coord4(1)
            + 0.5  * ( -2 * Xi * (1 + Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord5(1)
            + 0.5  * ( -(1 - Eta * Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord6(1)
            + 0.5  * ( -2 * Xi * (1 - Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord7(1)
            + 0.5 * ( (1 - Eta * Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord8(1)
            - 2 * Xi * (1 - Eta * Eta) * coord9(1);


    J1(2) = 0.25 * ( (1 + Eta) + (1 + Eta) * 2 * Xi - (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta) ) * coord1(2)
            + 0.25 * ( -(1 + Eta) + (1 + Eta) * 2 * Xi + (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta) ) * coord2(2)
            + 0.25 * ( -(1 - Eta) + (1 - Eta * Eta) + (1 - Eta) * 2 * Xi - 2 * Xi * (1 - Eta * Eta) ) * coord3(2)
            + 0.25 * ( (1 - Eta) + (1 - Eta) * 2 * Xi - (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta)  ) * coord4(2)
            + 0.5  * ( -2 * Xi * (1 + Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord5(2)
            + 0.5  * ( -(1 - Eta * Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord6(2)
            + 0.5  * ( -2 * Xi * (1 - Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord7(2)
            + 0.5 * ( (1 - Eta * Eta) + 2 * Xi * (1 - Eta * Eta) ) * coord8(2)
            - 2 * Xi * (1 - Eta * Eta) * coord9(2);




    J2(0) = 0.25 * (  (1 + Xi) + (1 + Xi) * 2 * Eta - (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord1(0)
            + 0.25 * (  (1 - Xi) + (1 - Xi) * 2 * Eta - (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord2(0)
            + 0.25 * ( -(1 - Xi) + (1 - Xi) * 2 * Eta + (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord3(0)
            + 0.25 * ( -(1 + Xi) + (1 + Xi) * 2 * Eta + (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord4(0)
            + 0.5  * ( (1 - Xi * Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord5(0)
            + 0.5  * (-2 * Eta * (1 - Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord6(0)
            + 0.5  * ( -(1 - Xi * Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord7(0)
            + 0.5  * (-2 * Eta * (1 + Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord8(0)
            - 2 * Eta * (1 - Xi * Xi) * coord9(0);




    J2(1) = 0.25 * (  (1 + Xi) + (1 + Xi) * 2 * Eta - (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord1(1)
            + 0.25 * (  (1 - Xi) + (1 - Xi) * 2 * Eta - (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord2(1)
            + 0.25 * ( -(1 - Xi) + (1 - Xi) * 2 * Eta + (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord3(1)
            + 0.25 * ( -(1 + Xi) + (1 + Xi) * 2 * Eta + (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord4(1)
            + 0.5  * ( (1 - Xi * Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord5(1)
            + 0.5  * (-2 * Eta * (1 - Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord6(1)
            + 0.5  * ( -(1 - Xi * Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord7(1)
            + 0.5  * (-2 * Eta * (1 + Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord8(1)
            - 2 * Eta * (1 - Xi * Xi) * coord9(1);


    J2(2) = 0.25 * (  (1 + Xi) + (1 + Xi) * 2 * Eta - (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord1(2)
            + 0.25 * (  (1 - Xi) + (1 - Xi) * 2 * Eta - (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord2(2)
            + 0.25 * ( -(1 - Xi) + (1 - Xi) * 2 * Eta + (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord3(2)
            + 0.25 * ( -(1 + Xi) + (1 + Xi) * 2 * Eta + (1 - Xi * Xi) - 2 * Eta * (1 - Xi * Xi) ) * coord4(2)
            + 0.5  * ( (1 - Xi * Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord5(2)
            + 0.5  * (-2 * Eta * (1 - Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord6(2)
            + 0.5  * ( -(1 - Xi * Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord7(2)
            + 0.5  * (-2 * Eta * (1 + Xi) + 2 * Eta * (1 - Xi * Xi) ) * coord8(2)
            - 2 * Eta * (1 - Xi * Xi) * coord9(2);


    J_vector_in_function( 0 ) =  J1( 1 ) * J2( 2 ) - J1( 2 ) * J2( 1 ) ;
    J_vector_in_function( 1 ) =  J1( 2 ) * J2( 0 ) - J1( 0 ) * J2( 2 ) ;
    J_vector_in_function( 2 ) =  J1( 0 ) * J2( 1 ) - J1( 1 ) * J2( 0 ) ;


    return J_vector_in_function;
}



double TwentySevenNodeBrickOrderFour::SurfaceLoadValues( double Xi , double Eta, Vector Pressure )
{

    SurfaceLoadValues_in_function =  ( 0.25 * (1 + Xi) * (1 + Eta) - 0.25 * (1 - Xi * Xi) * (1 + Eta)  - 0.25 * (1 - Eta * Eta) * (1 + Xi)  + 0.25 * (1 - Eta * Eta) * (1 - Xi * Xi) ) * Pressure(0)
                                     + ( 0.25 * (1 - Xi) * (1 + Eta) - 0.25 * (1 - Xi * Xi) * (1 + Eta)  - 0.25 * (1 - Eta * Eta) * (1 - Xi)  + 0.25 * (1 - Eta * Eta) * (1 - Xi * Xi) ) * Pressure(1)
                                     + ( 0.25 * (1 - Xi) * (1 - Eta) - 0.25 * (1 - Eta * Eta) * (1 - Xi) - 0.25 * (1 - Xi * Xi) * (1 - Eta)   + 0.25 * (1 - Eta * Eta) * (1 - Xi * Xi) ) * Pressure(2)
                                     + ( 0.25 * (1 + Xi) * (1 - Eta) - 0.25 * (1 - Xi * Xi) * (1 - Eta)  - 0.25 * (1 - Eta * Eta) * (1 + Xi)  + 0.25 * (1 - Eta * Eta) * (1 - Xi * Xi) ) * Pressure(3)
                                     + ( 0.5 * (1 - Xi * Xi) * (1 + Eta)  - 0.5 * (1 - Eta * Eta) * (1 - Xi * Xi) ) * Pressure(4)
                                     + ( 0.5 * (1 - Eta * Eta) * (1 - Xi) - 0.5 * (1 - Eta * Eta) * (1 - Xi * Xi) ) * Pressure(5)
                                     + ( 0.5 * (1 - Xi * Xi) * (1 - Eta)  - 0.5 * (1 - Eta * Eta) * (1 - Xi * Xi) ) * Pressure(6)
                                     + ( 0.5 * (1 - Eta * Eta) * (1 + Xi) - 0.5 * (1 - Eta * Eta) * (1 - Xi * Xi) ) * Pressure(7)
                                     + (1 - Eta * Eta) * (1 - Xi * Xi) * Pressure(8);



    return SurfaceLoadValues_in_function;

}


int
TwentySevenNodeBrickOrderFour::CheckMesh( ofstream &checkmesh_file )
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
TwentySevenNodeBrickOrderFour::getStress( void )
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

Matrix &TwentySevenNodeBrickOrderFour::getGaussCoordinates(void)
{
    computeGaussPoint();
    return gauss_points;
}

Matrix &TwentySevenNodeBrickOrderFour::get_Gauss_p_c( )
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

Matrix &TwentySevenNodeBrickOrderFour::get_Gauss_p_w()
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
const vector<float> &TwentySevenNodeBrickOrderFour::getGaussOutput()
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


void TwentySevenNodeBrickOrderFour::zeroStrain()
{
    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
    {
        material_array[gp]->zeroStrain();
    }

    return ;
}

#endif

