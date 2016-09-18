///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli and Boris Jeremic && Sumeet 
// DATE:               NOv. 2013
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
// Adapted by Sumeet, for EightNodeBrickOrderSix to use the LTensor library for
// tensorial representation. [September, 2016]
/////////////////////////////////////////////////////////////////////////////////////


#ifndef EightNodeBrickOrderSix_CPP
#define EightNodeBrickOrderSix_CPP

#include "EightNodeBrickOrderSix.h"
#include <LTensorDisplay.h>
#include <HDF5_Channel.h>
#include <ESSITimer.h>
#include <unistd.h>

#define Order 6
#define Num_IntegrationPts Order
#define Num_TotalGaussPts Num_IntegrationPts*Num_IntegrationPts*Num_IntegrationPts
 

DTensor2 EightNodeBrickOrderSix::gp_coords(Num_TotalGaussPts, 3, 0.0);
DTensor2 EightNodeBrickOrderSix::gp_weight(Num_TotalGaussPts, 3, 0.0);
Matrix EightNodeBrickOrderSix::gauss_points(Num_TotalGaussPts, 3);
vector<float> EightNodeBrickOrderSix::Gauss_Output_Vector(Num_TotalGaussPts*18);

Matrix EightNodeBrickOrderSix::K( 24, 24);

Vector EightNodeBrickOrderSix::ShapeFunctionValues_in_function( 4 );  // Nima added for surface load (July 2012)
Vector EightNodeBrickOrderSix::J_vector_in_function( 3 );             // Nima added for surface load (July 2012)
double EightNodeBrickOrderSix::SurfaceLoadValues_in_function;

//====================================================================
// Constructor
//====================================================================
EightNodeBrickOrderSix::EightNodeBrickOrderSix( int element_number,
                                    int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                                    int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                                    NDMaterialLT *Globalmmodel)

    : Element( element_number,ELE_TAG_EightNodeBrickOrderSix ),
      rho( 0.0 ), connectedExternalNodes( 8 ),
      Ki( 0 ), Q( 24 ), bf(3),
      M( 24, 24), P( 24 )
{

    this->setMaterialTag(Globalmmodel->getTag());
    
    rho = Globalmmodel->getRho();
    determinant_of_Jacobian = 0.0;
    mmodel = Globalmmodel;


    material_array = new NDMaterialLT *[Num_TotalGaussPts];

    if (material_array == 0)
    {
        cerr << " EightNodeBrickOrderSix::EightNodeBrickOrderSix -- failed allocate material model pointer\n";
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

    connectedExternalNodes(0) = node_numb_1;
    connectedExternalNodes(1) = node_numb_2;
    connectedExternalNodes(2) = node_numb_3;
    connectedExternalNodes(3) = node_numb_4;
    connectedExternalNodes(4) = node_numb_5;
    connectedExternalNodes(5) = node_numb_6;
    connectedExternalNodes(6) = node_numb_7;
    connectedExternalNodes(7) = node_numb_8;

    Global_to_Local_Node_Mapping[node_numb_1]=0;
    Global_to_Local_Node_Mapping[node_numb_2]=1;
    Global_to_Local_Node_Mapping[node_numb_3]=2;
    Global_to_Local_Node_Mapping[node_numb_4]=3;
    Global_to_Local_Node_Mapping[node_numb_5]=4;
    Global_to_Local_Node_Mapping[node_numb_6]=5;
    Global_to_Local_Node_Mapping[node_numb_7]=6;
    Global_to_Local_Node_Mapping[node_numb_8]=7;   

    for ( int i = 0; i < 8; i++ )
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(4);
}





//====================================================================
EightNodeBrickOrderSix::EightNodeBrickOrderSix(): Element( 0, ELE_TAG_EightNodeBrickOrderSix ),
    rho( 0.0 ), connectedExternalNodes( 8 ) ,
    Ki( 0 ), mmodel( 0 ), Q( 24 ), bf(3),
    M( 24, 24), P( 24 )
{
    is_mass_computed = false;

    material_array = new NDMaterialLT *[Num_TotalGaussPts];

    if (material_array == 0)
    {
        cerr << " EightNodeBrickOrderSix::EightNodeBrickOrderSix -- failed allocate material model pointer\n";
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


    is_mass_computed = false;

    // zero node pointers
    for ( int i = 0; i < 8; i++ )
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(4);

}




//####DESTROYER!###############################################################
EightNodeBrickOrderSix::~EightNodeBrickOrderSix ()
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
const DTensor2 &EightNodeBrickOrderSix::H_3D( double r1, double r2, double r3 ) const
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
const DTensor1 &EightNodeBrickOrderSix::interp_poli_at( double r1, double r2, double r3 ) const
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



const DTensor2 &EightNodeBrickOrderSix::dh_drst_at( double r1, double r2, double r3 ) const
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
const DTensor2 &EightNodeBrickOrderSix::Jacobian_3D( const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    static DTensor2 Jacobian_3D_(3, 3, 0.0);
    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    return Jacobian_3D_;
}

//#############################################################################
const DTensor2  &EightNodeBrickOrderSix::Jacobian_3Dinv( const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    DTensor2 Jacobian_3D_(3, 3, 0.0);
    static DTensor2 Jacobian_3D_inv(3, 3, 0.0);

    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    Jacobian_3D_inv = Jacobian_3D_.Inv();
    return Jacobian_3D_inv;
}


////#############################################################################
const DTensor2 &EightNodeBrickOrderSix::Nodal_Coordinates( void ) const
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
const DTensor2 &EightNodeBrickOrderSix::incr_disp( void ) const
{
    static DTensor2 increment_disp( 8, 3, 0.0 );

    const Vector &IncrDis1 = theNodes[0]->getIncrDisp();
    const Vector &IncrDis2 = theNodes[1]->getIncrDisp();
    const Vector &IncrDis3 = theNodes[2]->getIncrDisp();
    const Vector &IncrDis4 = theNodes[3]->getIncrDisp();
    const Vector &IncrDis5 = theNodes[4]->getIncrDisp();
    const Vector &IncrDis6 = theNodes[5]->getIncrDisp();
    const Vector &IncrDis7 = theNodes[6]->getIncrDisp();
    const Vector &IncrDis8 = theNodes[7]->getIncrDisp();

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
const DTensor2 &EightNodeBrickOrderSix::total_disp( void ) const
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
int EightNodeBrickOrderSix::get_global_number_of_node( int local_node_number )
{
    return connectedExternalNodes( local_node_number );
}

////#############################################################################
int  EightNodeBrickOrderSix::get_Brick_Number( void )
{
    return this->getTag();
}



////#############################################################################
// returns nodal forces for given stress field in an element
const DTensor2 &EightNodeBrickOrderSix::nodal_forces( void ) const
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
    static DTensor2 dh( 8, 3 , 0.0 );
    static DTensor2 stress_at_GP( 3, 3, 0.0 );
    static DTensor2 Jacobian( 3, 3, 0.0 );
    static DTensor2 JacobianINV( 3, 3, 0.0 );
    static DTensor2 dhGlobal( 8, 3 , 0.0 );

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
void EightNodeBrickOrderSix::computeGaussPoint()
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    // Matrix gauss_coordinates(8, 3);

    // special case for 8 nodes only
    DTensor2 NodalCoord( 3, 8, 0.0 );
    DTensor2 material_arrayCoord( 3, Num_TotalGaussPts, 0.0 );
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


    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
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

}


////#############################################################################


//=============================================================================
int EightNodeBrickOrderSix::getNumExternalNodes () const
{
    return 8;
}


//=============================================================================
const ID &EightNodeBrickOrderSix::getExternalNodes ()
{
    return connectedExternalNodes;
}

Node **
EightNodeBrickOrderSix::getNodePtrs( void )
{
    return theNodes;
}

//=============================================================================
int EightNodeBrickOrderSix::getNumDOF ()
{
    return 24;
}

//=============================================================================
void EightNodeBrickOrderSix::setDomain ( Domain *theDomain )
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

        for (int i = 0; i < 8; ++i)
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
                theNodes[4] == 0 || theNodes[5] == 0 || theNodes[6] == 0 || theNodes[7] == 0 )
        {
            cerr << "FATAL ERROR EightNodeBrickOrderSix (tag: " << this->getTag() << "), node not found in domain\n";
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
            cerr << "FATAL ERROR EightNodeBrickOrderSix (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit( -1 );
        }

        this->DomainComponent::setDomain( theDomain );
    }

    ComputeVolume();
    computeGaussPoint();
}

//=============================================================================
int EightNodeBrickOrderSix::commitState ()
{

    int retVal = 0;

    // call element commitState to do any base class stuff
    if ( ( retVal = this->Element::commitState() ) != 0 )
    {
        cerr << "EightNodeBrickOrderSix::commitState () - failed in base class";
    }

    Vector pp = getResistingForce();

    for (int ii = 0; ii < Num_TotalGaussPts; ii++ )
    {
        retVal += material_array[ii]->commitState();
    }

    return retVal;
}


//=============================================================================
int EightNodeBrickOrderSix::revertToLastCommit ()
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
int EightNodeBrickOrderSix::revertToStart ()
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
const Matrix &EightNodeBrickOrderSix::getTangentStiff()
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

    DTensor4 stifftensor(8, 3, 3, 8, 0.0);

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor2 dh_drst( 8, 3, 0.0 );
    static DTensor2 dhGlobal( 8, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);
    static DTensor2 JacobianINV(3, 3, 0.0);
    static DTensor4 E_elpl(3, 3, 3, 3, 0.0);
    static DTensor4 Kkt( 8, 3, 3, 8, 0.0);

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

    // Using STL-like iterators to linearly transverse the array

    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp, 0);
        w_s = gp_weight(gp, 1);
        w_t = gp_weight(gp, 2);

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



    //Set the stiffness tensor to zero (its static!)
    K.Zero();

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

//=============================================================================
const Matrix &EightNodeBrickOrderSix::getInitialStiff ()
{

    if ( Ki == 0 )
    {
        Ki = new Matrix( this->getTangentStiff() );
    }

    if ( Ki == 0 )
    {
        cerr << "FATAL EightNodeBrickOrderSix::getInitialStiff() -";
        cerr << "ran out of memory\n";
        exit( -1 );
    }

    return *Ki;
}

//=============================================================================

const Matrix &EightNodeBrickOrderSix::getMass ()
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

        DTensor2 dh( 8, 3, 0.0 );
        DTensor2 H(24, 3, 0.0 );
        DTensor2 Mm( 24, 24, 0.0 );
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
void EightNodeBrickOrderSix::zeroLoad( void )
{
    Q.Zero();
}



//======================================================================
const Vector &EightNodeBrickOrderSix::getBodyForce( double loadFactor, const Vector &data )
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
const Vector &EightNodeBrickOrderSix::getSurfaceForce( double loadFactor, const Vector &data )
{

    map<int,int> local_nodes_map; int local_nodes[4];
  
    /////////////////////////////////////////// Edited by Sumeet 30/03/2016 //////////////////////////////
    // checking if node exists in the element
    for ( int i =0; i<4 ;i++){
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
    static const int Node_to_Surface[8][9]={{1,2,3,4,5,1,3,7,4},
                                            {2,3,0,0,4,5,5,6,2},
                                            {3,0,1,6,7,3,1,5,6},
                                            {0,1,2,2,6,7,7,4,0},
                                            {7,6,5,5,1,0,0,3,7},
                                            {4,7,6,1,0,4,6,2,1},
                                            {5,4,7,7,3,2,2,1,5},
                                            {6,5,4,4,0,3,3,2,6}};
    ////////////////////////////////////// Edited by Sumeet 30/3/2016 /////////////////////////////////////
    //  Finding the correct surface nodes Order
    int success =0; int surface_nodes_order[4]={0,0,0,0};
    for ( int i =0; i<3 ;i++){
        for( int j=0; j<3;j++){
            std::map<int,int>::iterator it;
            int node=Node_to_Surface[local_nodes[0]][3*i+j];
            it=local_nodes_map.find(node);
            if (it == local_nodes_map.end()){
                success=0;break;
            }
            success=success+1;
            surface_nodes_order[j]= it->second;         
        }
        if(success==3) break;
    }
    if (success == 0){
        cerr << "\nERROR: Nodes  defined for the BrickSurfaceLoad does not belong to elements surface  \n" ;
        exit( 1 );
    }    

    ////////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    // cout << "surface_nodes_order ";
    // for ( int i =0; i < 8 ; i++)
    //     cout << local_nodes[i] << " ";
    //     // cout << surface_nodes_order[i] << " ";
    // cout << "\n";
    // for ( int i =0; i < 8 ; i++)
    //     cout << local_nodes[surface_nodes_order[i]] << " ";
    // cout << "\n";
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    // get the surface nodal coordinates
    const Vector &coordnode1 = theNodes[local_nodes[(surface_nodes_order[0])]]->getCrds();
    const Vector &coordnode2 = theNodes[local_nodes[(surface_nodes_order[1])]]->getCrds();
    const Vector &coordnode3 = theNodes[local_nodes[(surface_nodes_order[2])]]->getCrds();
    const Vector &coordnode4 = theNodes[local_nodes[(surface_nodes_order[3])]]->getCrds();

    double ShapeFunctionValues;
    double LoadValue;
    Vector J_vector( 3 );
    Vector Pressure( 4 );

    Pressure( 0 ) = data( surface_nodes_order[0]+4) * loadFactor;
    Pressure( 1 ) = data( surface_nodes_order[1]+4) * loadFactor;
    Pressure( 2 ) = data( surface_nodes_order[2]+4) * loadFactor;
    Pressure( 3 ) = data( surface_nodes_order[3]+4) * loadFactor;

    static Vector NodalForces(24);
    for (int m = 0; m < 24; m++){
        NodalForces(m) = 0;
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
        for ( int j = 0; j < 4; j++ )
        {
            r = local_nodes[(surface_nodes_order[j])] ;
            // loop over Gauss points
            for ( int i = 0; i < Order*Order; i++ )
            {
                ShapeFunctionValues = SurfaceShapeFunctionValues( GsPts( i, 0 ) , GsPts( i, 1 ), j );
                J_vector = Direction_Weight( GsPts( i, 0 ) , GsPts( i, 1 ), coordnode1, coordnode2, coordnode3, coordnode4 );
                LoadValue = SurfaceLoadValues( GsPts( i, 0 ) , GsPts( i, 1 ), Pressure );
                NodalForces( r * 3 + k ) = NodalForces( r * 3 + k ) + LoadValue * J_vector( k ) * ShapeFunctionValues* Weight(i, 0) * Weight(i, 1);
            }
        }
    }


    // ////////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    // for ( int i =0; i < 8 ; i++)
    //     cout << NodalForces(3*i)<< " " << NodalForces(3*i+1) << " " << NodalForces(3*i+2) <<   "\n";
    // cout << "\n\n******************************************************************************\n\n";
    // /////////////////////////////////////////////////////////////////////////////////////////////////
    return NodalForces;
}



//=============================================================================
int EightNodeBrickOrderSix::addLoad( ElementalLoad *theLoad, double loadFactor )
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
    else if (type < 0)
    {
        //Do nothing
    }
    else
    {
        cerr << "EightNodeBrickOrderSix::addLoad() - 8NodeBrickLT " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }


    return 0;
}




//=============================================================================
int EightNodeBrickOrderSix::addInertiaLoadToUnbalance( const Vector &accel )
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
        cerr << "EightNodeBrickOrderSix::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
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
const Vector EightNodeBrickOrderSix::FormEquiBodyForce( void )
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
const Vector &EightNodeBrickOrderSix::getResistingForce()
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
const Vector &EightNodeBrickOrderSix::getResistingForceIncInertia ()
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




int EightNodeBrickOrderSix::sendSelf ( int commitTag, Channel &theChannel )
{

    ID idData(3);

    idData( 0 ) = this->getTag();
    idData( 1 ) = material_array[0]->getClassTag();
    idData( 2 ) = getElasticMaterialStatus();

    if ( theChannel.sendID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send ID idData\n";
        return -1;
    }

    // Sending the Stiffness matrix for elastic material
    if(idData( 2 )==1){
        if ( theChannel.sendMatrix( 0, commitTag, K ) < 0 )
        {
            cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send its K\n";
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
        cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }

    string tmp_string;

    tmp_string = stiffness_type;
    if ( theChannel.sendString( 0, commitTag, tmp_string ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send String stiffness_type\n";
        return -1;
    }

    tmp_string = damping_type;
    if ( theChannel.sendString( 0, commitTag, damping_type ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send String damping_type\n";
        return -1;
    }

    // Send the nodes

    cout << this->getTag() << " -> EightNodeBrickOrderSix::sendSelf() connectedExternalNodes = " << connectedExternalNodes << "\n";


    if ( theChannel.sendID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send ID connectedExternalNodes\n";
        return -1;
    }



    // material objects send themselves

    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {
        if ( material_array[i]->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send material models\n";
            return -1;
        }
    }


    //Send Q
    if ( theChannel.sendVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send its Q\n";
        return -1;
    }

    //Send bf
    if ( theChannel.sendVector( 0, commitTag, bf ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::sendSelf() - " << this->getTag() << " failed to send its bf\n";
        return -1;
    }

    return 0;

}

int EightNodeBrickOrderSix::receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
{

    ID idData( 3 );

    if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::receiveSelf() - failed to receive ID\n";
        return -1;
    }

    this->setTag( idData( 0 ) );
    int matClassTag = idData( 1 );
    setElasticMaterialStatus(idData(2));

    // Recieve the Stiffness matrix for elastic material
    if(idData(2)){
        if ( theChannel.sendMatrix( 0, commitTag, K ) < 0 )
        {
            cerr << "WARNING EightNodeBrickOrderSix::receiveSelf() - " << this->getTag() << " failed to recieve its K\n";
            return -1;
        }
    }

    Vector floatData(7);
    if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }
    Volume                  = floatData(0) ;
    determinant_of_Jacobian = floatData(1) ;
    rho                     = floatData(2) ;
    a0                      = floatData(3) ;
    a1                      = floatData(4) ;
    a2                      = floatData(5) ;
    a3                      = floatData(6) ;


    string tmp_string;
    if ( theChannel.receiveString( 0, commitTag,  tmp_string) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::receiveSelf() - " << this->getTag() << " failed to recieve String stiffness_type\n";
        return -1;
    }
    stiffness_type = tmp_string;

    if ( theChannel.receiveString( 0, commitTag, tmp_string ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::receiveSelf() - " << this->getTag() << " failed to recieve String damping_type\n";
        return -1;
    }
    damping_type = tmp_string;


    // Recieve the nodes

    if ( theChannel.receiveID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING EightNodeBrickOrderSix::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
        return -1;
    }

    cout << this->getTag() << " -> EightNodeBrickOrderSix::receiveSelf() connectedExternalNodes = " << connectedExternalNodes << "\n";

    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {

        // Allocate new material with the sent class tag
        NDMaterialLT *ndmat = theBroker.getNewNDMaterialLT( matClassTag );
        if ( ndmat == 0 )
        {
            cerr << "EightNodeBrickOrderSix::receiveSelf() - Broker could not create NDMaterialLT of class type " << matClassTag << "\n";
            return -1;
        }

        // Now receive materials into the newly allocated space
        if ( ( ndmat )->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
        {
            cerr << "EightNodeBrickOrderSix::receiveSelf() - material " << i << "failed to recv itself\n";
            return -1;
        }

        material_array[i] = ndmat;
    }


    // Q
    if ( theChannel.receiveVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "EightNodeBrickOrderSix::receiveSelf() - failed to recv Q!\n";
        return -1;
    }

    // bf
    if ( theChannel.receiveVector( 0, commitTag, bf ) < 0 )
    {
        cerr << "EightNodeBrickOrderSix::receiveSelf() - failed to recv bf!\n";
        return -1;
    }

    return 0;

}


int EightNodeBrickOrderSix::getObjectSize()
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
void EightNodeBrickOrderSix::Print( ostream &s, int flag )
{
    cout << "EightNodeBrickOrderSix: LTensor-based 8 node brick." << endl << endl;

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

    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
    {
        cout << "   GP # " << gp <<  ": (" << gp_coords(gp, 0) << ","
             << gp_coords(gp, 1) << ","
             << gp_coords(gp, 2) << ")" << endl;
        cout << "        State of stress " << material_array[gp]->getStressTensor() << endl;
    }

}


void EightNodeBrickOrderSix::ComputeVolume()
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

    static DTensor2 dh( 8, 3, 0.0 );
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



int EightNodeBrickOrderSix::update( void )
{
    // cout << "Updating brick #" << this->getTag() << endl;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    //DTensor2 Jacobian(3,3,0.0);
    static DTensor2 dh( 8, 3, 0.0 );
    static DTensor2 JacobianINV(3, 3, 0.0);
    static DTensor2 dhGlobal( 8, 3, 0.0 );
    static DTensor2 trial_disp( 8, 3, 0.0  );
    static DTensor2 total_strain(3, 3, 0.0);
    static DTensor2 trial_strain(3, 3, 0.0);

    dh *= 0;
    JacobianINV *= 0;
    dhGlobal *= 0;
    trial_disp *= 0;
    total_strain *= 0;
    trial_strain *= 0;

    // trial_disp = total_disp();  ///////!!!!!!!!!!!!!!  Jose did this. Might break a few material models that are not correctly programmed.
    trial_disp = incr_disp();

    //Debug
    // cout << endl << endl << "EightNodeBrickOrderSix::update()" << endl;
    // LTensorDisplay::print(trial_disp, "trial_disp");

    ESSITimer::tic();

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
        //Debug
        trial_strain(i, j) = 0.5 * (dhGlobal( k, i ) * trial_disp( k, j ) + dhGlobal( k, j ) * trial_disp( k, i ));
        // LTensorDisplay::print(trial_strain, "trial_strain");

        // if ( ( material_array[gp]->setTrialStrain( trial_strain ) ) )
        // Get back by Yuan to solve the zero elastic strain problems.
        material_array[gp]->setTrialStrain( trial_strain );
        if ( ( material_array[gp]->setTrialStrainIncr( trial_strain ) ) )
        {
            Matrix &gps = getGaussCoordinates();
            cerr << "EightNodeBrickOrderSix::update (tag: " << this->getTag()
                 << "), Update Failed on Gauss Point # " << gp
                 << " located @ ( " << gps(gp, 0) << ","
                 << gps(gp, 1) << ","
                 << gps(gp, 2) << ")\n";
            return -1;
        }
    }

    update_time_taken = ESSITimer::toc(); // :)

    return 0;
}



/////////////////////////////////////////////////////////////////////////
//NOTE: Can me templatized to improve performance
double EightNodeBrickOrderSix::SurfaceShapeFunctionValues( double Xi , double Eta, int whichcomponent )
{
    ShapeFunctionValues_in_function( 0 ) = 0.25 * ( 1 + Xi ) * ( 1 + Eta );
    ShapeFunctionValues_in_function( 1 ) = 0.25 * ( 1 - Xi ) * ( 1 + Eta );
    ShapeFunctionValues_in_function( 2 ) = 0.25 * ( 1 - Xi ) * ( 1 - Eta );
    ShapeFunctionValues_in_function( 3 ) = 0.25 * ( 1 + Xi ) * ( 1 - Eta );

    return ShapeFunctionValues_in_function( whichcomponent );
}


Vector &EightNodeBrickOrderSix::Direction_Weight( double Xi , double Eta, Vector coord1, Vector coord2, Vector coord3, Vector coord4 )
{
    Vector J1( 3 );
    Vector J2( 3 );

    J1( 0 ) = 0.25 * ( ( 1 + Eta ) * coord1( 0 ) - ( 1 + Eta ) * coord2( 0 ) - ( 1 - Eta ) * coord3( 0 ) + ( 1 - Eta ) * coord4( 0 ) );
    J1( 1 ) = 0.25 * ( ( 1 + Eta ) * coord1( 1 ) - ( 1 + Eta ) * coord2( 1 ) - ( 1 - Eta ) * coord3( 1 ) + ( 1 - Eta ) * coord4( 1 ) );
    J1( 2 ) = 0.25 * ( ( 1 + Eta ) * coord1( 2 ) - ( 1 + Eta ) * coord2( 2 ) - ( 1 - Eta ) * coord3( 2 ) + ( 1 - Eta ) * coord4( 2 ) );


    J2( 0 ) = 0.25 * ( ( 1 + Xi ) * coord1( 0 ) + ( 1 - Xi ) * coord2( 0 ) - ( 1 - Xi ) * coord3( 0 ) - ( 1 + Xi ) * coord4( 0 ) );
    J2( 1 ) = 0.25 * ( ( 1 + Xi ) * coord1( 1 ) + ( 1 - Xi ) * coord2( 1 ) - ( 1 - Xi ) * coord3( 1 ) - ( 1 + Xi ) * coord4( 1 ) );
    J2( 2 ) = 0.25 * ( ( 1 + Xi ) * coord1( 2 ) + ( 1 - Xi ) * coord2( 2 ) - ( 1 - Xi ) * coord3( 2 ) - ( 1 + Xi ) * coord4( 2 ) );


    J_vector_in_function( 0 ) = J1( 1 ) * J2( 2 ) - J1( 2 ) * J2( 1 ) ;
    J_vector_in_function( 1 ) = J1( 2 ) * J2( 0 ) - J1( 0 ) * J2( 2 ) ;
    J_vector_in_function( 2 ) = J1( 0 ) * J2( 1 ) - J1( 1 ) * J2( 0 ) ;


    return J_vector_in_function;
}



double EightNodeBrickOrderSix::SurfaceLoadValues( double Xi , double Eta, Vector Pressure )
{

    SurfaceLoadValues_in_function =  0.25 * ( 1 + Xi ) * ( 1 + Eta ) * Pressure( 0 )
                                     + 0.25 * ( 1 - Xi ) * ( 1 + Eta ) * Pressure( 1 )
                                     + 0.25 * ( 1 - Xi ) * ( 1 - Eta ) * Pressure( 2 )
                                     + 0.25 * ( 1 + Xi ) * ( 1 - Eta ) * Pressure( 3 );



    return SurfaceLoadValues_in_function;

}


int
EightNodeBrickOrderSix::CheckMesh( ofstream &checkmesh_file )
{

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    double det_of_Jacobian = 0.0;

    DTensor2 dh( 8, 3, 0.0 );
    DTensor2 Jacobian(3, 3, 0.0);

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
EightNodeBrickOrderSix::getStress( void )
{
    DTensor2 stress(3, 3, 0.0);
    Vector *stresses = new Vector( Num_TotalGaussPts*6 );   // FIXME: Who deallocates this guy???

    for ( short gp = 0 ; gp < Num_TotalGaussPts ; gp++ )
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

Matrix &EightNodeBrickOrderSix::getGaussCoordinates(void)
{
    computeGaussPoint();
    return gauss_points;
}

Matrix &EightNodeBrickOrderSix::get_Gauss_p_c( )
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

Matrix &EightNodeBrickOrderSix::get_Gauss_p_w()
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
const vector<float> &EightNodeBrickOrderSix::getGaussOutput()
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

void EightNodeBrickOrderSix::zeroStrain()
{
    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
    {
        material_array[gp]->zeroStrain();
    }

    return ;
}

#endif
