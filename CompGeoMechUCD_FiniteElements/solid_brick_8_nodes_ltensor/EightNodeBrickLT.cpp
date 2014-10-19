///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              EightNodeBrickLTLT.cpp
// CLASS:             EightNodeBrickLTLT
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic, Zhaohui Yang, Xiaoyan Wu, Nima Tafazzoli, & Jose Abell
// PROGRAMMER:        Jose Abell
// DATE:              NOv. 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#ifndef EightNodeBrickLTLT_CPP
#define EightNodeBrickLTLT_CPP

#include "EightNodeBrickLT.h"
#include <LTensorDisplay.h>
#include <HDF5_Channel.h>



double EightNodeBrickLT::SurfaceLoadValues_in_function;         // Nima added for surface load (July 2012)

DTensor2 EightNodeBrickLT::gp_coords(8, 3, 0.0);
DTensor1 EightNodeBrickLT::gp_weight(8, 0.0);

Matrix EightNodeBrickLT::K( 24, 24);
Matrix EightNodeBrickLT::M( 24, 24);

Vector EightNodeBrickLT::P( 24 );
Vector EightNodeBrickLT::ShapeFunctionValues_in_function( 4 );  // Nima added for surface load (July 2012)
Vector EightNodeBrickLT::J_vector_in_function( 3 );             // Nima added for surface load (July 2012)

// These are for returning Information from the element
// Vector Info_Stress(8 * 6 + 1); // Stress 8*6+1  2X2X2
// Vector Info_GaussCoordinates(8 * 3 + 1);      //Gauss point coordinates
// Vector Info_Strain(8 * 6 + 1);


EightNodeBrickLT::EightNodeBrickLT( int element_number,
                                    int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                                    int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                                    NDMaterialLT *Globalmmodel)

    : Element( element_number, ELE_TAG_EightNodeBrickLT ),
      rho( 0.0 ), connectedExternalNodes( 8 ),
      Ki( 0 ), Q( 24 ), bf(3), gauss_points(8, 3), outputVector(EightNodeBrickLT_OUTPUT_SIZE)
{

    rho = Globalmmodel->getRho();
    determinant_of_Jacobian = 0.0;
    mmodel = Globalmmodel;



    // Generate 8 NDMaterialLT for use at each Gauss point.
    for (int k = 0; k < 8; k++)
    {
        material_array[k] = mmodel->getCopy();
    }

    //GP coordinates and weights. ====================================================
    // This initializes class wide members gp_coords and gp_weights .
    // Since LTensor does not provide initializer lists, this is the only way to
    // do this.

    short where = 0;
    for ( short ii = 0 ; ii < 2 ; ii++ )
    {
        for ( short jj = 0 ; jj < 2 ; jj++ )
        {
            for ( short kk = 0 ; kk < 2 ; kk++ )
            {
                gp_coords(where, 0) = (ii == 0) ? -0.577350269189626 : 0.577350269189626;
                gp_coords(where, 1) = (jj == 0) ? -0.577350269189626 : 0.577350269189626;
                gp_coords(where, 2) = (kk == 0) ? -0.577350269189626 : 0.577350269189626;
                where ++;
            }
        }
    }

    gp_weight(0) = 1.0;
    gp_weight(1) = 1.0;
    gp_weight(2) = 1.0;
    gp_weight(3) = 1.0;
    gp_weight(4) = 1.0;
    gp_weight(5) = 1.0;
    gp_weight(6) = 1.0;
    gp_weight(7) = 1.0;
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

    nodes_in_brick = 8;


    for ( int i = 0; i < 8; i++ )
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(4);

}





//====================================================================
EightNodeBrickLT::EightNodeBrickLT(): Element( 0, ELE_TAG_EightNodeBrickLT ),
    rho( 0.0 ), connectedExternalNodes( 8 ) , Ki( 0 ), mmodel( 0 ), Q( 24 ), bf(3),  gauss_points(8, 3), outputVector(EightNodeBrickLT_OUTPUT_SIZE)
{
    is_mass_computed = false;

    //GP coordinates and weights. ====================================================
    // This initializes class wide members gp_coords and gp_weights .
    // Since LTensor does not provide initializer lists, this is the only way to
    // do this.
    short where = 0;
    for ( short ii = 0 ; ii < 2 ; ii++ )
    {
        for ( short jj = 0 ; jj < 2 ; jj++ )
        {
            for ( short kk = 0 ; kk < 2 ; kk++ )
            {
                gp_coords(where, 0) = (ii == 0) ? -0.577350269189626 : 0.577350269189626;
                gp_coords(where, 1) = (jj == 0) ? -0.577350269189626 : 0.577350269189626;
                gp_coords(where, 2) = (kk == 0) ? -0.577350269189626 : 0.577350269189626;
                where ++;
            }
        }
    }

    gp_weight(0) = 1.0;
    gp_weight(1) = 1.0;
    gp_weight(2) = 1.0;
    gp_weight(3) = 1.0;
    gp_weight(4) = 1.0;
    gp_weight(5) = 1.0;
    gp_weight(6) = 1.0;
    gp_weight(7) = 1.0;
    // =============================================================================

    is_mass_computed = false;

    nodes_in_brick = 8;

    // zero node pointers
    for ( int i = 0; i < 8; i++ )
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(4);

}




//####DESTROYER!###############################################################
EightNodeBrickLT::~EightNodeBrickLT ()
{
    for ( int i = 0; i < 8; i++ )
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
const DTensor2 &EightNodeBrickLT::H_3D( double r1, double r2, double r3 ) const
{
    static DTensor2 H( 24, 3, 0.0 );

    H( 21, 0 ) =  ( 1.0 + r1 ) * ( 1.0 - r2 ) * ( 1.0 - r3 ) * 0.125; // - (H(43,1)+H(48,3)+H(60,3))/2.0;
    H( 22, 1 ) = H( 21, 0 ); //(1.0+r1)*(1.0-r2)*(1.0-r3)*0.125;// - (H(43,1)+H(48,3)+H(60,3))/2.0;
    H( 23, 2 ) = H( 21, 0 ); //(1.0+r1)*(1.0-r2)*(1.0-r3)*0.125;// - (H(43,1)+H(48,3)+H(60,3))/2.0;
    H( 18, 0 ) =  ( 1.0 - r1 ) * ( 1.0 - r2 ) * ( 1.0 - r3 ) * 0.125; // - (H(42,3)+H(43,1)+H(57,3))/2.0;
    H( 19, 1 ) = H( 18, 0 ); //(1.0-r1)*(1.0-r2)*(1.0-r3)*0.125;// - (H(42,3)+H(43,1)+H(57,3))/2.0;
    H( 20, 2 ) = H( 18, 0 ); //(1.0-r1)*(1.0-r2)*(1.0-r3)*0.125;// - (H(42,3)+H(43,1)+H(57,3))/2.0;
    H( 15, 0 ) =  ( 1.0 - r1 ) * ( 1.0 + r2 ) * ( 1.0 - r3 ) * 0.125; //- (H(39,3)+H(42,3)+H(54,3))/2.0;
    H( 16, 1 ) = H( 15, 0 ); //(1.0-r1)*(1.0+r2)*(1.0-r3)*0.125;//- (H(39,3)+H(42,3)+H(54,3))/2.0;
    H( 17, 2 ) = H( 15, 0 ); //(1.0-r1)*(1.0+r2)*(1.0-r3)*0.125;//- (H(39,3)+H(42,3)+H(54,3))/2.0;
    H( 12, 0 ) =  ( 1.0 + r1 ) * ( 1.0 + r2 ) * ( 1.0 - r3 ) * 0.125; //- (H(39,3)+H(48,3)+H(51,3))/2.0;
    H( 13, 1 ) = H( 12, 0 ); //(1.0+r1)*(1.0+r2)*(1.0-r3)*0.125;//- (H(39,3)+H(48,3)+H(51,3))/2.0;
    H( 14, 2 ) = H( 12, 0 ); //(1.0+r1)*(1.0+r2)*(1.0-r3)*0.125;//- (H(39,3)+H(48,3)+H(51,3))/2.0;
    H(  9, 0 ) =  ( 1.0 + r1 ) * ( 1.0 - r2 ) * ( 1.0 + r3 ) * 0.125; //- (H(33,3)+H(36,3)+H(60,3))/2.0;
    H( 10, 1 ) = H(  9, 0 ); //(1.0+r1)*(1.0-r2)*(1.0+r3)*0.125;//- (H(33,3)+H(36,3)+H(60,3))/2.0;
    H( 11, 2 ) = H(  9, 0 ); //(1.0+r1)*(1.0-r2)*(1.0+r3)*0.125;//- (H(33,3)+H(36,3)+H(60,3))/2.0;
    H( 6, 0 )  =  ( 1.0 - r1 ) * ( 1.0 - r2 ) * ( 1.0 + r3 ) * 0.125; //- (H(30,3)+H(33,3)+H(57,3))/2.0;
    H( 7, 1 )  = H( 6, 0 ); //(1.0-r1)*(1.0-r2)*(1.0+r3)*0.125;//- (H(30,3)+H(33,3)+H(57,3))/2.0;
    H( 8, 2 )  = H( 6, 0 ); //(1.0-r1)*(1.0-r2)*(1.0+r3)*0.125;//- (H(30,3)+H(33,3)+H(57,3))/2.0;
    H( 3, 0 )  =  ( 1.0 - r1 ) * ( 1.0 + r2 ) * ( 1.0 + r3 ) * 0.125; //- (H(30,3)+H(54,3)+H(27,3))/2.0;
    H( 4, 1 )  = H( 3, 0 ); //(1.0-r1)*(1.0+r2)*(1.0+r3)*0.125;//- (H(30,3)+H(54,3)+H(27,3))/2.0;
    H( 5, 2 )  = H( 3, 0 ); //(1.0-r1)*(1.0+r2)*(1.0+r3)*0.125;//- (H(30,3)+H(54,3)+H(27,3))/2.0;
    H( 0, 0 )  =  ( 1.0 + r1 ) * ( 1.0 + r2 ) * ( 1.0 + r3 ) * 0.125; //- (H(36,3)+H(51,3)+H(27,3))/2.0;
    H( 1, 1 )  = H( 0, 0 ); //(1.0+r1)*(1.0+r2)*(1.0+r3)*0.125;//- (H(36,3)+H(51,3)+H(27,3))/2.0;
    H( 2, 2 )  = H( 0, 0 ); //(1.0+r1)*(1.0+r2)*(1.0+r3)*0.125;//- (H(36,3)+H(51,3)+H(27,3))/2.0;

    return H;
}

//#############################################################################
//***************************************************************
const DTensor1 &EightNodeBrickLT::interp_poli_at( double r1, double r2, double r3 ) const
{
    static DTensor1 h( 8, 0.0 );

    // influence of the node number 8
    h( 7 ) = ( 1.0 + r1 ) * ( 1.0 - r2 ) * ( 1.0 - r3 ) / 8.0; // - (h(15)+h(16)+h(20))/2.0;
    h( 6 ) = ( 1.0 - r1 ) * ( 1.0 - r2 ) * ( 1.0 - r3 ) / 8.0; // - (h(14)+h(15)+h(19))/2.0;
    h( 5 ) = ( 1.0 - r1 ) * ( 1.0 + r2 ) * ( 1.0 - r3 ) / 8.0; // - (h(13)+h(14)+h(18))/2.0;
    h( 4 ) = ( 1.0 + r1 ) * ( 1.0 + r2 ) * ( 1.0 - r3 ) / 8.0; // - (h(13)+h(16)+h(17))/2.0;
    h( 3 ) = ( 1.0 + r1 ) * ( 1.0 - r2 ) * ( 1.0 + r3 ) / 8.0; // - (h(11)+h(12)+h(20))/2.0;
    h( 2 ) = ( 1.0 - r1 ) * ( 1.0 - r2 ) * ( 1.0 + r3 ) / 8.0; // - (h(10)+h(11)+h(19))/2.0;
    h( 1 ) = ( 1.0 - r1 ) * ( 1.0 + r2 ) * ( 1.0 + r3 ) / 8.0; // - (h(10)+h(18)+h(9))/2.0;
    h( 0 ) = ( 1.0 + r1 ) * ( 1.0 + r2 ) * ( 1.0 + r3 ) / 8.0; // - (h(12)+h(17)+h(9))/2.0;

    return h;
}



const DTensor2 &EightNodeBrickLT::dh_drst_at( double r1, double r2, double r3 ) const
{
    static DTensor2 dh( 8, 3, 0.0 );

    dh( 7, 0 ) =  ( 1.0 - r2 ) * ( 1.0 - r3 ) * 0.125; ///8.0;// - (dh(15,0)+dh(16,0)+dh(20,0))/2.0;
    dh( 7, 1 ) = -( 1.0 + r1 ) * ( 1.0 - r3 ) * 0.125; ///8.0;// - (dh(15,2)+dh(16,2)+dh(20,2))/2.0;
    dh( 7, 2 ) = -( 1.0 + r1 ) * ( 1.0 - r2 ) * 0.125; ///8.0;// - (dh(15,3)+dh(16,3)+dh(20,3))/2.0;
    dh( 6, 0 ) = -( 1.0 - r2 ) * ( 1.0 - r3 ) * 0.125; ///8.0;// - (dh(14,0)+dh(15,0)+dh(19,0))/2.0;
    dh( 6, 1 ) = -( 1.0 - r1 ) * ( 1.0 - r3 ) * 0.125; ///8.0;// - (dh(14,2)+dh(15,2)+dh(19,2))/2.0;
    dh( 6, 2 ) = -( 1.0 - r1 ) * ( 1.0 - r2 ) * 0.125; ///8.0;// - (dh(14,3)+dh(15,3)+dh(19,3))/2.0;
    dh( 5, 0 ) = -( 1.0 + r2 ) * ( 1.0 - r3 ) * 0.125; ///8.0;// - (dh(13,0)+dh(14,0)+dh(18,0))/2.0;
    dh( 5, 1 ) =  ( 1.0 - r1 ) * ( 1.0 - r3 ) * 0.125; ///8.0;// - (dh(13,2)+dh(14,2)+dh(18,2))/2.0;
    dh( 5, 2 ) = -( 1.0 - r1 ) * ( 1.0 + r2 ) * 0.125; ///8.0;//- (dh(13,3)+dh(14,3)+dh(18,3))/2.0;
    dh( 4, 0 ) =  ( 1.0 + r2 ) * ( 1.0 - r3 ) * 0.125; ///8.0;// - (dh(13,0)+dh(16,0)+dh(17,0))/2.0;
    dh( 4, 1 ) =  ( 1.0 + r1 ) * ( 1.0 - r3 ) * 0.125; ///8.0;// - (dh(13,2)+dh(16,2)+dh(17,2))/2.0;
    dh( 4, 2 ) = -( 1.0 + r1 ) * ( 1.0 + r2 ) * 0.125; ///8.0;// - (dh(13,3)+dh(16,3)+dh(17,3))/2.0;
    dh( 3, 0 ) =  ( 1.0 - r2 ) * ( 1.0 + r3 ) * 0.125; ///8.0;// - (dh(11,0)+dh(12,0)+dh(20,0))/2.0;
    dh( 3, 1 ) = -( 1.0 + r1 ) * ( 1.0 + r3 ) * 0.125; ///8.0;// - (dh(11,2)+dh(12,2)+dh(20,2))/2.0;
    dh( 3, 2 ) =  ( 1.0 + r1 ) * ( 1.0 - r2 ) * 0.125; ///8.0;// - (dh(11,3)+dh(12,3)+dh(20,3))/2.0;
    dh( 2, 0 ) = -( 1.0 - r2 ) * ( 1.0 + r3 ) * 0.125; ///8.0;// - (dh(10,0)+dh(11,0)+dh(19,0))/2.0;
    dh( 2, 1 ) = -( 1.0 - r1 ) * ( 1.0 + r3 ) * 0.125; ///8.0;// - (dh(10,2)+dh(11,2)+dh(19,2))/2.0;
    dh( 2, 2 ) =  ( 1.0 - r1 ) * ( 1.0 - r2 ) * 0.125; ///8.0;// - (dh(10,3)+dh(11,3)+dh(19,3))/2.0;
    dh( 1, 0 ) = -( 1.0 + r2 ) * ( 1.0 + r3 ) * 0.125; ///8.0;// - (dh(10,0)+dh(18,0)+dh(9,0))/2.0;
    dh( 1, 1 ) =  ( 1.0 - r1 ) * ( 1.0 + r3 ) * 0.125; ///8.0;// - (dh(10,2)+dh(18,2)+dh(9,2))/2.0;
    dh( 1, 2 ) =  ( 1.0 - r1 ) * ( 1.0 + r2 ) * 0.125; ///8.0;// - (dh(10,3)+dh(18,3)+dh(9,3))/2.0;
    dh( 0, 0 ) =  ( 1.0 + r2 ) * ( 1.0 + r3 ) * 0.125; ///8.0;// - (dh(12,0)+dh(17,0)+dh(9,0))/2.0;
    dh( 0, 1 ) =  ( 1.0 + r1 ) * ( 1.0 + r3 ) * 0.125; ///8.0;// - (dh(12,2)+dh(17,2)+dh(9,2))/2.0;
    dh( 0, 2 ) =  ( 1.0 + r1 ) * ( 1.0 + r2 ) * 0.125; ///8.0;//- (dh(12,3)+dh(17,3)+dh(9,3))/2.0;
    return dh;
}




////#############################################################################
const DTensor2 &EightNodeBrickLT::Jacobian_3D( const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    static DTensor2 Jacobian_3D_(3, 3, 0.0);
    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    return Jacobian_3D_;
}

//#############################################################################
const DTensor2  &EightNodeBrickLT::Jacobian_3Dinv( const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    DTensor2 Jacobian_3D_(3, 3, 0.0);
    static DTensor2 Jacobian_3D_inv(3, 3, 0.0);

    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    Jacobian_3D_inv = Jacobian_3D_.Inv();
    return Jacobian_3D_inv;
}


////#############################################################################
const DTensor2 &EightNodeBrickLT::Nodal_Coordinates( void ) const
{
    static DTensor2 N_coord( 8, 3, 0.0 );

    //using node pointers, which come from the Domain
    const Vector &nd1Crds = theNodes[0]->getCrds();
    const Vector &nd2Crds = theNodes[1]->getCrds();
    const Vector &nd3Crds = theNodes[2]->getCrds();
    const Vector &nd4Crds = theNodes[3]->getCrds();
    const Vector &nd5Crds = theNodes[4]->getCrds();
    const Vector &nd6Crds = theNodes[5]->getCrds();
    const Vector &nd7Crds = theNodes[6]->getCrds();
    const Vector &nd8Crds = theNodes[7]->getCrds();

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

    return N_coord;
}

////#############################################################################
const DTensor2 &EightNodeBrickLT::incr_disp( void ) const
{
    static DTensor2 increment_disp( 8, 3, 0.0 );

    //Have to get IncrDeltaDisp, not IncrDisp for cumulation of incr_disp
    const Vector &IncrDis1 = theNodes[0]->getIncrDeltaDisp();
    const Vector &IncrDis2 = theNodes[1]->getIncrDeltaDisp();
    const Vector &IncrDis3 = theNodes[2]->getIncrDeltaDisp();
    const Vector &IncrDis4 = theNodes[3]->getIncrDeltaDisp();
    const Vector &IncrDis5 = theNodes[4]->getIncrDeltaDisp();
    const Vector &IncrDis6 = theNodes[5]->getIncrDeltaDisp();
    const Vector &IncrDis7 = theNodes[6]->getIncrDeltaDisp();
    const Vector &IncrDis8 = theNodes[7]->getIncrDeltaDisp();



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

    return increment_disp;
}

////#############################################################################
const DTensor2 &EightNodeBrickLT::total_disp( void ) const
{
    static DTensor2 total_disp( 8, 3, 0.0 );

    const Vector &TotDis1 = theNodes[0]->getTrialDisp();
    const Vector &TotDis2 = theNodes[1]->getTrialDisp();
    const Vector &TotDis3 = theNodes[2]->getTrialDisp();
    const Vector &TotDis4 = theNodes[3]->getTrialDisp();
    const Vector &TotDis5 = theNodes[4]->getTrialDisp();
    const Vector &TotDis6 = theNodes[5]->getTrialDisp();
    const Vector &TotDis7 = theNodes[6]->getTrialDisp();
    const Vector &TotDis8 = theNodes[7]->getTrialDisp();

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

    return total_disp;
}



////#############################################################################
int EightNodeBrickLT::get_global_number_of_node( int local_node_number )
{
    return connectedExternalNodes( local_node_number );
}

////#############################################################################
int  EightNodeBrickLT::get_Brick_Number( void )
{
    return this->getTag();
}



////#############################################################################
// returns nodal forces for given stress field in an element
const DTensor2 &EightNodeBrickLT::nodal_forces( void ) const
{
    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor2 nodal_forces( 8, 3, 0.0 );
    DTensor2 dh( 8, 3 , 0.0 );
    DTensor2 stress_at_GP( 3, 3, 0.0 );
    DTensor2 Jacobian( 3, 3, 0.0 );
    DTensor2 JacobianINV( 3, 3, 0.0 );
    DTensor2 dhGlobal( 8, 3 , 0.0 );

    //Set nodal_forces to zero (it is only done once since it's a static varible)
    for (DTensor2::literator it = nodal_forces.begin(); it != nodal_forces.end(); it++)
    {
        *it = 0;
    }


    for ( short gp = 0; gp < 8; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp);
        w_s = gp_weight(gp);
        w_t = gp_weight(gp);

        // derivatives of local coordiantes with respect to local coordiantes
        dh = dh_drst_at( r, s, t );

        // Jacobian tensor ( matrix )
        Jacobian = Jacobian_3D( dh );

        // Inverse of Jacobian tensor ( matrix )
        JacobianINV = Jacobian_3Dinv( dh );

        // determinant of Jacobian tensor ( matrix )
        det_of_Jacobian  = Jacobian.compute_Determinant();

        // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
        dhGlobal(i, k) = dh(i, j) * JacobianINV(k, j);

        //weight
        weight = w_r * w_s * w_t * det_of_Jacobian;
        stress_at_GP = material_array[gp]->getStressTensor();

        nodal_forces(i, j) = nodal_forces(i, j) + weight * (dhGlobal( i, k ) * stress_at_GP( j, k ) );

    }

    return nodal_forces;
}



//Compute gauss point coordinates
void EightNodeBrickLT::computeGaussPoint()
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    // Matrix gauss_coordinates(8, 3);

    // special case for 8 nodes only
    DTensor2 NodalCoord( 3, 8, 0.0 );
    DTensor2 material_arrayCoord( 3, 8, 0.0 );
    DTensor2 H( 24, 3, 0.0 );

    const Vector &nd1Crds = theNodes[0]->getCrds();
    const Vector &nd2Crds = theNodes[1]->getCrds();
    const Vector &nd3Crds = theNodes[2]->getCrds();
    const Vector &nd4Crds = theNodes[3]->getCrds();
    const Vector &nd5Crds = theNodes[4]->getCrds();
    const Vector &nd6Crds = theNodes[5]->getCrds();
    const Vector &nd7Crds = theNodes[6]->getCrds();
    const Vector &nd8Crds = theNodes[7]->getCrds();

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


    for ( short gp = 0; gp < 8; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);

        // derivatives of local coordinates with respect to local coordinates
        H = H_3D( r, s, t );

        for ( int encount = 0 ; encount < 8 ; encount++ )
        {
            material_arrayCoord( 0, gp ) += NodalCoord( 0, encount ) * H( encount * 3 + 0, 0 );
            material_arrayCoord( 1, gp ) += NodalCoord( 1, encount ) * H( encount * 3 + 1, 1 );
            material_arrayCoord( 2, gp ) += NodalCoord( 2, encount ) * H( encount * 3 + 2, 2 );
        }

        gauss_points( gp, 0) = material_arrayCoord( 0, gp );
        gauss_points( gp, 1 ) = material_arrayCoord( 1, gp );
        gauss_points( gp, 2 ) = material_arrayCoord( 2, gp );
    }
    // return gauss_points;
}


////#############################################################################


//=============================================================================
int EightNodeBrickLT::getNumExternalNodes () const
{
    return 8;
}


//=============================================================================
const ID &EightNodeBrickLT::getExternalNodes ()
{
    return connectedExternalNodes;
}

Node **
EightNodeBrickLT::getNodePtrs( void )
{
    return theNodes;
}

//=============================================================================
int EightNodeBrickLT::getNumDOF ()
{
    return 24;
}

//=============================================================================
void EightNodeBrickLT::setDomain ( Domain *theDomain )
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
    }
    else
    {
        int Nd1 = connectedExternalNodes( 0 );
        int Nd2 = connectedExternalNodes( 1 );
        int Nd3 = connectedExternalNodes( 2 );
        int Nd4 = connectedExternalNodes( 3 );

        int Nd5 = connectedExternalNodes( 4 );
        int Nd6 = connectedExternalNodes( 5 );
        int Nd7 = connectedExternalNodes( 6 );
        int Nd8 = connectedExternalNodes( 7 );

        theNodes[0] = theDomain->getNode( Nd1 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[0] == 0 )
        {
            theNodes[0] = theDomain->getOutsideNode( Nd1 );
        }

# endif
        theNodes[1] = theDomain->getNode( Nd2 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[1] == 0 )
        {
            theNodes[1] = theDomain->getOutsideNode( Nd2 );
        }

# endif
        theNodes[2] = theDomain->getNode( Nd3 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[2] == 0 )
        {
            theNodes[2] = theDomain->getOutsideNode( Nd3 );
        }

# endif
        theNodes[3] = theDomain->getNode( Nd4 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[3] == 0 )
        {
            theNodes[3] = theDomain->getOutsideNode( Nd4 );
        }

# endif

        theNodes[4] = theDomain->getNode( Nd5 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[4] == 0 )
        {
            theNodes[4] = theDomain->getOutsideNode( Nd5 );
        }

# endif
        theNodes[5] = theDomain->getNode( Nd6 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[5] == 0 )
        {
            theNodes[5] = theDomain->getOutsideNode( Nd6 );
        }

# endif
        theNodes[6] = theDomain->getNode( Nd7 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[6] == 0 )
        {
            theNodes[6] = theDomain->getOutsideNode( Nd7 );
        }

# endif
        theNodes[7] = theDomain->getNode( Nd8 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[7] == 0 )
        {
            theNodes[7] = theDomain->getOutsideNode( Nd8 );
        }

# endif

        if ( theNodes[0] == 0 || theNodes[1] == 0 || theNodes[2] == 0 || theNodes[3] == 0 ||
                theNodes[4] == 0 || theNodes[5] == 0 || theNodes[6] == 0 || theNodes[7] == 0 )
        {
            cerr << "FATAL ERROR EightNodeBrickLT (tag: " << this->getTag() << "), node not found in domain\n";
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

        if ( dofNd1 != 3 || dofNd2 != 3 || dofNd3 != 3 || dofNd4 != 3 ||
                dofNd5 != 3 || dofNd6 != 3 || dofNd7 != 3 || dofNd8 != 3 )
        {
            cerr << "FATAL ERROR EightNodeBrickLT (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit( -1 );
        }

        this->DomainComponent::setDomain( theDomain );
    }

    ComputeVolume();
    computeGaussPoint();
}

//=============================================================================
int EightNodeBrickLT::commitState ()
{

    int retVal = 0;

    // call element commitState to do any base class stuff
    if ( ( retVal = this->Element::commitState() ) != 0 )
    {
        cerr << "EightNodeBrickLT::commitState () - failed in base class";
    }

    Vector pp = getResistingForce();

    for (int ii = 0; ii < 8; ii++ )
    {
        retVal += material_array[ii]->commitState();
    }

    formOutput();

    return retVal;
}


void EightNodeBrickLT::formOutput()
{
    DTensor2 stress(3, 3);
    DTensor2 strain(3, 3);
    DTensor2 plstrain(3, 3);

    int ii = 0;
    for (int gp = 0; gp < 8; gp++)
    {
        strain = material_array[gp]->getStrainTensor();
        plstrain = material_array[gp]->getPlasticStrainTensor();
        stress = material_array[gp]->getStressTensor();

        //Write strain
        outputVector(ii++) = strain(0, 0);
        outputVector(ii++) = strain(1, 1);
        outputVector(ii++) = strain(2, 2);
        outputVector(ii++) = strain(0, 1);
        outputVector(ii++) = strain(0, 2);
        outputVector(ii++) = strain(1, 2);

        //Write plastic strain
        outputVector(ii++) = plstrain(0, 0);
        outputVector(ii++) = plstrain(1, 1);
        outputVector(ii++) = plstrain(2, 2);
        outputVector(ii++) = plstrain(0, 1);
        outputVector(ii++) = plstrain(0, 2);
        outputVector(ii++) = plstrain(1, 2);


        //Write stress
        outputVector(ii++) = stress(0, 0);
        outputVector(ii++) = stress(1, 1);
        outputVector(ii++) = stress(2, 2);
        outputVector(ii++) = stress(0, 1);
        outputVector(ii++) = stress(0, 2);
        outputVector(ii++) = stress(1, 2);
    }
}

//=============================================================================
int EightNodeBrickLT::revertToLastCommit ()
{
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states

    for (int gp = 0; gp < 8; gp++ )
    {
        retVal += material_array[gp]->revertToLastCommit();
    }

    return retVal;
}

//=============================================================================
int EightNodeBrickLT::revertToStart ()
{
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states

    for (int gp = 0; gp < 8; gp++ )
    {
        retVal += material_array[gp]->revertToStart();
    }

    return retVal;
}


//=============================================================================
const Matrix &EightNodeBrickLT::getTangentStiff()
{
    DTensor4 stifftensor(8, 3, 3, 8, 0.0);




    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    DTensor2 incremental_strain(3, 3, 0.0);
    DTensor2 dh_drst( 8, 3, 0.0 );
    DTensor2 dhGlobal( 8, 3, 0.0 );
    DTensor2 Jacobian(3, 3, 0.0);
    DTensor2 JacobianINV(3, 3, 0.0);

    DTensor4 E_elpl(3, 3, 3, 3, 0.0);
    // DTensor4 Kk( 8, 3, 3, 8, 0.0);
    DTensor4 Kkt( 8, 3, 3, 8, 0.0);

    Index < 'a' > a;
    Index < 'b' > b;
    Index < 'c' > c;
    Index < 'd' > d;

    //Set the stiffness tensor to zero (its static!)
    // Using STL-like iterators to linearly transverse the array

    for ( short gp = 0; gp < 8; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp);
        w_s = gp_weight(gp);
        w_t = gp_weight(gp);

        // derivatives of local coordinates with respect to local coordinates
        dh_drst         = dh_drst_at( r, s, t );
        Jacobian        = Jacobian_3D( dh_drst );
        JacobianINV     = Jacobian.Inv();
        det_of_Jacobian = Jacobian.compute_Determinant();

        // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
        dhGlobal(i, k)  = dh_drst( i, j ) * JacobianINV( k, j );
        weight          = w_r * w_s * w_t * det_of_Jacobian;
        E_elpl          = material_array[gp]->getTangentTensor();


        //LTensorDisplay::print(E_elpl,"a","Hi there!",1);

        // This is the actual integration
        // FIXME: This can be more efficient if these lines are merged into one (discard the temporary KKt tensor)
        Kkt(i, a, c, j) = dhGlobal(i, b) * E_elpl(a, b, c, d) * dhGlobal(j, d) * weight;
        stifftensor(i, a, c, j) = stifftensor(i, a, c, j) + Kkt(i, a, c, j);
    }




    int Ki = 0;
    int Kj = 0;

    for ( int ii = 1 ; ii <= 8 ; ii++ )
    {
        for ( int jj = 1 ; jj <= 8 ; jj++ )
        {
            for ( int kk = 1 ; kk <= 3 ; kk++ )
            {
                for ( int ll = 1 ; ll <= 3 ; ll++ )
                {
                    Ki = kk + 3 * ( ii - 1 );
                    Kj = ll + 3 * ( jj - 1 );
                    K( Ki - 1 , Kj - 1 ) = stifftensor( ii - 1, kk - 1, ll - 1, jj - 1 );
                }
            }
        }
    }

    return K;
}



////#############################################################################
const DTensor4 &EightNodeBrickLT::getStiffnessTensor( void ) const
{

    static DTensor4 Kk(8, 3, 3, 8, 0.0);

    // LTensorDisplay::print(Kk,"Kk","\n\n K tensor \n",1);
    return Kk;
}


//=============================================================================
const Matrix &EightNodeBrickLT::getInitialStiff ()
{

    if ( Ki == 0 )
    {
        Ki = new Matrix( this->getTangentStiff() );
    }

    if ( Ki == 0 )
    {
        cerr << "FATAL EightNodeBrickLT::getInitialStiff() -";
        cerr << "ran out of memory\n";
        exit( -1 );
    }

    return *Ki;
}

//=============================================================================

const Matrix &EightNodeBrickLT::getMass ()
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
        double RHO;
        RHO = rho;   //global

        DTensor2 dh( 8, 3, 0.0 );
        DTensor2 H(24, 3, 0.0 );
        DTensor2 Mm( 24, 24, 0.0 );
        DTensor2 Jacobian(3, 3);

        for ( short gp = 0; gp < 8; gp++ )
        {
            r = gp_coords(gp, 0);
            s = gp_coords(gp, 1);
            t = gp_coords(gp, 2);
            w_r = gp_weight(gp);
            w_s = gp_weight(gp);
            w_t = gp_weight(gp);
            dh = dh_drst_at( r, s, t );
            Jacobian = Jacobian_3D( dh );
            det_of_Jacobian  = Jacobian.compute_Determinant();
            H = H_3D( r, s, t );

            weight = w_r * w_s * w_t * RHO * det_of_Jacobian;
            Mm(i, j) = Mm(i, j) + H(i, k) * H(j, k) * weight;
        }

        for ( int ii = 1 ; ii <= 24 ; ii++ )
        {
            for ( int jj = 1 ; jj <= 24 ; jj++ )
            {
                M( ii - 1 , jj - 1 ) = Mm( ii - 1, jj - 1 );
            }

        }

        is_mass_computed = true;
    }

    //LTensorDisplay::print(Mm, "Mm", "", 1);

    return M;
}

//=============================================================================
void EightNodeBrickLT::zeroLoad( void )
{
    Q.Zero();
}



//======================================================================
const Vector &EightNodeBrickLT::getBodyForce( double loadFactor, const Vector &data )
{

    static Vector bforce( 24 );
    bforce.Zero();


    // Check for a quick return
    if ( rho == 0.0 )
    {
        return bforce;
    }

    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);

    Vector ba( 24 ), bfx( 3 );
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

    //Form equivalent body force
    this->getMass();

    bforce.addMatrixVector( 0.0, M, ba, 1.0 );

    return bforce;
}


//======================================================================
const Vector &EightNodeBrickLT::getSurfaceForce( double loadFactor, const Vector &data )
{

    int node_exist = 0;
    Vector node_local( 4 );

    // check if the nodes of the surface belong to the element
    for ( int i = 0; i < 4; i++ )
    {

        for ( int j = 0; j < 8; j++ )
        {
            if ( data( i ) == connectedExternalNodes( j ) )
            {
                node_exist = 1;
                node_local( i ) = j;
                break;
            }
        }

        if ( node_exist != 1 )
        {
            cerr << "\nERROR: Node " << data( i ) << " defined for the BrickSurfaceLoad does not belong to element " << this->getTag() << endl;
            exit( 1 );
        }
    }


    int node1_local = node_local( 0 );
    int node2_local = node_local( 1 );
    int node3_local = node_local( 2 );
    int node4_local = node_local( 3 );


    // get the surface nodal coordinates
    const Vector &coordnode1 = theNodes[node1_local]->getCrds();
    const Vector &coordnode2 = theNodes[node2_local]->getCrds();
    const Vector &coordnode3 = theNodes[node3_local]->getCrds();
    const Vector &coordnode4 = theNodes[node4_local]->getCrds();



    double ShapeFunctionValues;
    double LoadValue;
    Vector J_vector( 3 );
    Vector Pressure( 4 );



    Pressure( 0 ) = data( 4 ) * loadFactor;
    Pressure( 1 ) = data( 5 ) * loadFactor;
    Pressure( 2 ) = data( 6 ) * loadFactor;
    Pressure( 3 ) = data( 7 ) * loadFactor;



    static Vector NodalForces( 24 );

    for ( int m = 0; m < 24; m++ )
    {
        NodalForces( m ) = 0;
    }



    double oneOverSquareRoot3 = 1.0 / sqrt( 3.0 );
    Matrix GsPts( 4, 2 );

    GsPts( 0, 0 ) =  oneOverSquareRoot3;
    GsPts( 0, 1 ) =  oneOverSquareRoot3;

    GsPts( 1, 0 ) = -oneOverSquareRoot3;
    GsPts( 1, 1 ) =  oneOverSquareRoot3;

    GsPts( 2, 0 ) = -oneOverSquareRoot3;
    GsPts( 2, 1 ) = -oneOverSquareRoot3;

    GsPts( 3, 0 ) =  oneOverSquareRoot3;
    GsPts( 3, 1 ) = -oneOverSquareRoot3;



    int r = 0;


    // loop over dof
    for ( int k = 0; k < 3; k++ )
    {
        // loop over nodes
        for ( int j = 0; j < 4; j++ )
        {

            for ( int v = 0; v < 8; v++ )
            {
                if ( data( j ) == connectedExternalNodes( v ) )
                {
                    r = v;
                    break;
                }
            }

            // loop over Gauss points
            for ( int i = 0; i < 4; i++ )
            {

                ShapeFunctionValues = SurfaceShapeFunctionValues( GsPts( i, 0 ) , GsPts( i, 1 ), j );
                J_vector = Direction_Weight( GsPts( i, 0 ) , GsPts( i, 1 ), coordnode1, coordnode2, coordnode3, coordnode4 );
                LoadValue = SurfaceLoadValues( GsPts( i, 0 ) , GsPts( i, 1 ), Pressure );


                NodalForces( r * 3 + k ) = NodalForces( r * 3 + k ) + LoadValue * J_vector( k ) * ShapeFunctionValues;
            }
        }
    }

    return NodalForces;
}



//=============================================================================
int EightNodeBrickLT::addLoad( ElementalLoad *theLoad, double loadFactor )
{

    int type;
    const Vector &data = theLoad->getData( type, loadFactor );

    if ( type == LOAD_TAG_ElementSelfWeight )
    {
        Vector Fbody = this->getBodyForce( loadFactor, data );
        Q.addVector( 1.0, Fbody, 1.0 );
    }
    else if ( type == LOAD_TAG_BrickSurfaceLoad )
    {
        Vector Fsurface = this->getSurfaceForce( loadFactor, data );

        Q.addVector( 1.0, Fsurface, 1.0);

    }
    else
    {
        cerr << "EightNodeBrickLT::addLoad() - 8NodeBrickLT " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }


    return 0;
}




//=============================================================================
int EightNodeBrickLT::addInertiaLoadToUnbalance( const Vector &accel )
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

    if ( 3 != Raccel1.Size() || 3 != Raccel2.Size() || 3 != Raccel3.Size() ||
            3 != Raccel4.Size() || 3 != Raccel5.Size() || 3 != Raccel6.Size() ||
            3 != Raccel7.Size() || 3 != Raccel8.Size()  )
    {
        cerr << "EightNodeBrickLT::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }

    Vector ra( 24 );

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

    Q.addMatrixVector( 1.0, M, ra, -1.0 );
    return 0;
}

//=============================================================================
const Vector EightNodeBrickLT::FormEquiBodyForce( void )
{
    Vector bforce( 24 );

    // Check for a quick return
    if ( rho == 0.0 )
    {
        return bforce;
    }

    Vector ba( 24 );


    //Form equivalent body force
    bforce.addMatrixVector( 0.0, M, ba, 1.0 );

    return bforce;
}


//=============================================================================
const Vector &EightNodeBrickLT::getResistingForce()
{
    DTensor2 nodalforces( 8, 3, 0.0 );
    nodalforces = nodal_forces();

    //converting nodalforce tensor to vector
    for ( int i = 0; i < 8; i++ )
        for ( int j = 0; j < 3; j++ )
        {
            P( i * 3 + j ) = nodalforces( i, j );
        }

    P.addVector( 1.0, Q, -1.0 );

    // cout << P;

    return P;
}

//=============================================================================
const Vector &EightNodeBrickLT::getResistingForceIncInertia ()
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

        Vector a( 24 ); // originally 8

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

    // cout << P;

    return P;

}




int EightNodeBrickLT::sendSelf ( int commitTag, Channel &theChannel )
{
    // cout << "EightNodeBrickLT::sendSelf() tag = " << this->getTag() << "\n";

    ID idData( 5 );

    idData( 0 ) = this->getTag();
    idData( 1 ) = numDOF;
    idData( 2 ) = nodes_in_brick;
    idData( 3 ) = order;
    idData( 4 ) = material_array[0]->getClassTag();

    if ( theChannel.sendID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send ID idData\n";
        return -1;
    }

    // send double data
    Vector floatData(4);
    floatData(0) = Volume;
    floatData(1) = e_p;
    floatData(2) = determinant_of_Jacobian;
    floatData(3) = rho;

    if ( theChannel.sendVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }


    // Send the nodes

    if ( theChannel.sendID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send ID connectedExternalNodes\n";
        return -1;
    }


    // material objects send themselves

    for ( int i = 0; i < 8; i++ )
    {
        if ( material_array[i]->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send material models\n";
            return -1;
        }
    }


    //Send Q
    if ( theChannel.sendVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send its Q\n";
        return -1;
    }

    //Send bf
    if ( theChannel.sendVector( 0, commitTag, bf ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send its bf\n";
        return -1;
    }

    //Send the gauss points
    if ( theChannel.sendMatrix( 0, commitTag, gauss_points ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send its Gauss point coordinates\n";
        return -1;
    }

    //Send outputVector
    if ( theChannel.sendVector( 0, commitTag, outputVector ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send its outputVector\n";
        return -1;
    }




    return 0;

}

int EightNodeBrickLT::receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
{

    ID idData( 5 );

    if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::receiveSelf() - failed to receive ID\n";
        return -1;
    }

    this->setTag( idData( 0 ) );
    numDOF          = idData(1);
    nodes_in_brick  = idData(2);
    order           = idData(3);
    int matClassTag = idData( 4 );

    // cout << "EightNodeBrickLT::receiveSelf() tag = " << this->getTag() << "\n";

    // cout << "EightNodeBrickLT::receiveSelf() numDOF           = " << numDOF << "\n";
    // cout << "EightNodeBrickLT::receiveSelf() nodes_in_brick   = " << nodes_in_brick << "\n";
    // cout << "EightNodeBrickLT::receiveSelf() order            = " << order << "\n";
    // cout << "EightNodeBrickLT::receiveSelf() materialclasstag = " << idData(4) << "\n";


    Vector floatData(4);
    if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }
    Volume                  = floatData(0) ;
    e_p                     = floatData(1) ;
    determinant_of_Jacobian = floatData(2) ;
    rho                     = floatData(3) ;

    // cout << "EightNodeBrickLT::receiveSelf() Volume                  = " << Volume << "\n";
    // cout << "EightNodeBrickLT::receiveSelf() e_p                     = " << e_p << "\n";
    // cout << "EightNodeBrickLT::receiveSelf() determinant_of_Jacobian = " << determinant_of_Jacobian << "\n";
    // cout << "EightNodeBrickLT::receiveSelf() rho                     = " << rho << "\n";


    // Recieve the nodes

    if ( theChannel.receiveID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
        return -1;
    }

    // cout << "EightNodeBrickLT::receiveSelf() connectedExternalNodes = " << connectedExternalNodes << "\n";

    for ( int i = 0; i < 8; i++ )
    {

        // Allocate new material with the sent class tag
        NDMaterialLT *ndmat = theBroker.getNewNDMaterialLT( matClassTag );
        if ( ndmat == 0 )
        {
            cerr << "EightNodeBrickLT::receiveSelf() - Broker could not create NDMaterialLT of class type " << matClassTag << "\n";
            return -1;
        }

        // Now receive materials into the newly allocated space
        if ( ( ndmat )->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
        {
            cerr << "EightNodeBrickLT::receiveSelf() - material " << i << "failed to recv itself\n";
            return -1;
        }

        material_array[i] = ndmat;
    }


    // Q
    if ( theChannel.receiveVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "EightNodeBrickLT::receiveSelf() - failed to recv Q!\n";
        return -1;
    }

    // bf
    if ( theChannel.receiveVector( 0, commitTag, bf ) < 0 )
    {
        cerr << "EightNodeBrickLT::receiveSelf() - failed to recv bf!\n";
        return -1;
    }

    // gauss_points
    if ( theChannel.receiveMatrix( 0, commitTag, gauss_points ) < 0 )
    {
        cerr << "EightNodeBrickLT::receiveSelf() - failed to recv gauss_points!\n";
        return -1;
    }

    // outputVector
    if ( theChannel.receiveVector( 0, commitTag, outputVector ) < 0 )
    {
        cerr << "EightNodeBrickLT::receiveSelf() - failed to recv outputVector!\n";
        return -1;
    }



    return 0;

}


int EightNodeBrickLT::getObjectSize()
{

    int size = sizeof(*this);
    for ( int i = 0; i < 8; i++ )
    {
        size += material_array[i]->getObjectSize();
    }

    cout << "EightNodeBrickLT::getObjectSize() - tag = " << this->getTag()
         << "  size = " << size << "\n";

    return size;
}

//=============================================================================
void EightNodeBrickLT::Print( ostream &s, int flag )
{
    cout << "EightNodeBrickLT: LTensor-based 8 node brick." << endl << endl;

    cout << "Element tag: " << getTag() << endl;
    cout << "Connected Nodes: " << endl;

    for ( int node = 0; node < 8; node++)
    {
        cout << "   #" << node + 1 << ": Domain node #" << connectedExternalNodes(node) << endl;
    }

    cout << "K = " << endl;

    for (int ii = 0; ii < 24; ii++)
    {
        for (int jj = 0; jj < 24; jj++)
        {
            cout << K(ii, jj);

            if (jj < 23)
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

    for (int ii = 0; ii < 24; ii++)
    {
        cout << K(ii, ii);

        if (ii < 23)
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

    for (int gp = 0; gp < 8; gp++)
    {
        cout << "   GP # " << gp <<  ": (" << gp_coords(gp, 0) << ","
             << gp_coords(gp, 1) << ","
             << gp_coords(gp, 2) << ")" << endl;
        cout << "        State of stress " << material_array[gp]->getStressTensor() << endl;
    }

}


void EightNodeBrickLT::ComputeVolume()
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

    DTensor2 dh( 8, 3, 0.0 );
    DTensor2 Jacobian(3, 3, 0.0);

    for ( short gp = 0; gp < 8; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp);
        w_s = gp_weight(gp);
        w_t = gp_weight(gp);

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



int EightNodeBrickLT::update( void )
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;


    DTensor2 dh( 8, 3, 0.0 );
    //DTensor2 Jacobian(3,3,0.0);
    DTensor2 JacobianINV(3, 3, 0.0);
    DTensor2 dhGlobal( 8, 3, 0.0 );
    DTensor2 trial_disp( 8, 3, 0.0  );
    DTensor2 total_strain(3, 3, 0.0);
    DTensor2 trial_strain(3, 3, 0.0);

    trial_disp = total_disp();

    //Debug
    // cout << endl << endl << "EightNodeBrickLT::update()" << endl;
    // LTensorDisplay::print(trial_disp, "trial_disp");

    for ( short gp = 0; gp < 8; gp++ )
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
        //Debug
        trial_strain(i, j) = 0.5 * (dhGlobal( k, i ) * trial_disp( k, j ) + dhGlobal( k, j ) * trial_disp( k, i ));
        // LTensorDisplay::print(trial_strain, "trial_strain");

        if ( ( material_array[gp]->setTrialStrain( trial_strain ) ) )
        {
            cerr << "EightNodeBrickLT::update (tag: " << this->getTag() << "), Update Failed\n";
        }
    }

    return 0;
}



/////////////////////////////////////////////////////////////////////////
//NOTE: Can me templatized to improve performance
double EightNodeBrickLT::SurfaceShapeFunctionValues( double Xi , double Eta, int whichcomponent )
{
    ShapeFunctionValues_in_function( 0 ) = 0.25 * ( 1 + Xi ) * ( 1 + Eta );
    ShapeFunctionValues_in_function( 1 ) = 0.25 * ( 1 - Xi ) * ( 1 + Eta );
    ShapeFunctionValues_in_function( 2 ) = 0.25 * ( 1 - Xi ) * ( 1 - Eta );
    ShapeFunctionValues_in_function( 3 ) = 0.25 * ( 1 + Xi ) * ( 1 - Eta );

    return ShapeFunctionValues_in_function( whichcomponent );
}


Vector &EightNodeBrickLT::Direction_Weight( double Xi , double Eta, Vector coord1, Vector coord2, Vector coord3, Vector coord4 )
{
    Vector J1( 3 );
    Vector J2( 3 );

    J1( 0 ) = 0.25 * ( ( 1 + Eta ) * coord1( 0 ) - ( 1 + Eta ) * coord2( 0 ) - ( 1 - Eta ) * coord3( 0 ) + ( 1 - Eta ) * coord4( 0 ) );
    J1( 1 ) = 0.25 * ( ( 1 + Eta ) * coord1( 1 ) - ( 1 + Eta ) * coord2( 1 ) - ( 1 - Eta ) * coord3( 1 ) + ( 1 - Eta ) * coord4( 1 ) );
    J1( 2 ) = 0.25 * ( ( 1 + Eta ) * coord1( 2 ) - ( 1 + Eta ) * coord2( 2 ) - ( 1 - Eta ) * coord3( 2 ) + ( 1 - Eta ) * coord4( 2 ) );


    J2( 0 ) = 0.25 * ( ( 1 + Xi ) * coord1( 0 ) + ( 1 - Xi ) * coord2( 0 ) - ( 1 - Xi ) * coord3( 0 ) - ( 1 + Xi ) * coord4( 0 ) );
    J2( 1 ) = 0.25 * ( ( 1 + Xi ) * coord1( 1 ) + ( 1 - Xi ) * coord2( 1 ) - ( 1 - Xi ) * coord3( 1 ) - ( 1 + Xi ) * coord4( 1 ) );
    J2( 2 ) = 0.25 * ( ( 1 + Xi ) * coord1( 2 ) + ( 1 - Xi ) * coord2( 2 ) - ( 1 - Xi ) * coord3( 2 ) - ( 1 + Xi ) * coord4( 2 ) );


    J_vector_in_function( 0 ) = fabs( J1( 1 ) * J2( 2 ) - J1( 2 ) * J2( 1 ) );
    J_vector_in_function( 1 ) = fabs( J1( 2 ) * J2( 0 ) - J1( 0 ) * J2( 2 ) );
    J_vector_in_function( 2 ) = fabs( J1( 0 ) * J2( 1 ) - J1( 1 ) * J2( 0 ) );


    return J_vector_in_function;
}



double EightNodeBrickLT::SurfaceLoadValues( double Xi , double Eta, Vector Pressure )
{

    SurfaceLoadValues_in_function =  0.25 * ( 1 + Xi ) * ( 1 + Eta ) * Pressure( 0 )
                                     + 0.25 * ( 1 - Xi ) * ( 1 + Eta ) * Pressure( 1 )
                                     + 0.25 * ( 1 - Xi ) * ( 1 - Eta ) * Pressure( 2 )
                                     + 0.25 * ( 1 + Xi ) * ( 1 - Eta ) * Pressure( 3 );



    return SurfaceLoadValues_in_function;

}


int
EightNodeBrickLT::CheckMesh( ofstream &checkmesh_file )
{
    bool jacobian_flag = false;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    double det_of_Jacobian = 0.0;

    DTensor2 dh( 8, 3, 0.0 );
    DTensor2 Jacobian(3, 3, 0.0);

    for ( short gp = 0; gp < 8; gp++ )
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
            jacobian_flag = true;
            break;
        }

        if ( jacobian_flag == true )
        {
            break;
        }

    }

    return 0;
}



//==================================================================================
Vector *
EightNodeBrickLT::getStress( void )
{
    cout << "EightNodeBrickLT::getStress( void ) got called!\n\n";
    DTensor2 stress;
    Vector *stresses = new Vector( 48 );   // FIXME: Who deallocates this guy???

    for ( short gp = 0 ; gp < 8 ; gp++ )
    {
        stress = material_array[gp]->getStressTensor();
        ( *stresses )( gp * 6 + 0 ) = stress( 1, 1 ); //sigma_xx
        ( *stresses )( gp * 6 + 1 ) = stress( 2, 2 ); //sigma_yy
        ( *stresses )( gp * 6 + 2 ) = stress( 3, 3 ); //sigma_zz
        ( *stresses )( gp * 6 + 3 ) = stress( 1, 2 ); //Assign sigma_xy
        ( *stresses )( gp * 6 + 4 ) = stress( 1, 3 ); //Assign sigma_xz
        ( *stresses )( gp * 6 + 5 ) = stress( 2, 3 ); //Assign sigma_yz
    }

    return stresses;
}




Matrix &EightNodeBrickLT::getGaussCoordinates(void)
{
    return gauss_points;
}

int EightNodeBrickLT::getOutputSize() const
{
    return EightNodeBrickLT_OUTPUT_SIZE;
}



const Vector &EightNodeBrickLT::getOutput() const
{
    return outputVector;
}



#endif

