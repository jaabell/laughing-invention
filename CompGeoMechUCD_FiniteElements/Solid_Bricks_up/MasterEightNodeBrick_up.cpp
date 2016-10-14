///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Zhaohui Yang, Xiaoyan Wu, Zhao Cheng & Sumeet Kumar Sinha
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
// Sumeet Kumar Sinha [September, 2016]
// This is a Header file that is taken from MasterEightNodeBrick_upU.h
// The macros (#define) that go into this master BrickHeaderFile are 
//
// BRICK_TYPE -> define the element type like "EightNodeBrick_upU"
// BRICK_ELEMENT_NAME -> Name of the brick in string Eg:- "EightNodeBrick_upU"
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
#define Num_Nodes 8
#define Num_Dim 3
#define Num_Dof 4
#define Num_ElemDof Num_Nodes*Num_Dof

DTensor2 BRICK_TYPE::gp_coords(Num_TotalGaussPts, 3, 0.0);
DTensor2 BRICK_TYPE::gp_weight(Num_TotalGaussPts, 3, 0.0);
Matrix BRICK_TYPE::gauss_points(Num_TotalGaussPts, 3);
vector<float> BRICK_TYPE::Gauss_Output_Vector(Num_TotalGaussPts*18);

DTensor4 BRICK_TYPE::TensorKep(Num_Nodes,3,3,Num_Nodes,0.0);
DTensor3 BRICK_TYPE::TensorQ(Num_Nodes, 3, Num_Nodes, 0.0);;
DTensor2 BRICK_TYPE::TensorH(Num_Nodes, Num_Nodes, 0.0);
DTensor2 BRICK_TYPE::TensorMs(Num_Nodes, Num_Nodes, 0.0);

Matrix BRICK_TYPE::K( Num_ElemDof, Num_ElemDof);
Vector BRICK_TYPE::P( Num_ElemDof );

Vector BRICK_TYPE::ShapeFunctionValues_in_function( 4 );  // Nima added for surface load (July 2012)
Vector BRICK_TYPE::J_vector_in_function( 3 );             // Nima added for surface load (July 2012)
double BRICK_TYPE::SurfaceLoadValues_in_function;

//====================================================================
// Constructor
//====================================================================
BRICK_TYPE::BRICK_TYPE(int element_number,
                       int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                       int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                       NDMaterialLT *Globalmmodel,
                       double nn, double alf, double rs, double rf, double permb_x, double permb_y,
                       double permb_z, double kks, double kkf)
    : Element(element_number, ELE_TAG_BRICK_TYPE ),
      connectedExternalNodes(Num_Nodes),
      perm(Num_Dim),
      poro(nn),
      alpha(alf),
      rho_s(rs),
      rho_f(rf),
      ks(kks),
      kf(kkf),
      Q(Num_ElemDof),
      M( Num_ElemDof, Num_ElemDof),
      C( Num_ElemDof, Num_ElemDof),
      KFluid( Num_ElemDof, Num_ElemDof),
      Ki(0)
{
    
    this->setMaterialTag(Globalmmodel->getTag());
    mmodel = Globalmmodel;

    // permeability
    perm(0) = permb_x;
    perm(1) = permb_y;
    perm(2) = permb_z;

    if (perm(0) == 0.0 || perm(1) == 0.0 || perm(2) == 0.0)
    {
       cerr << BRICK_ELEMENT_NAME << "::" << BRICK_ELEMENT_NAME << " permeability (kx/ky/kz) is zero! \n";
        exit(-1);
    }

    material_array = new NDMaterialLT *[Num_TotalGaussPts];

    if (material_array == 0)
    {
       cerr << BRICK_ELEMENT_NAME << "::" << BRICK_ELEMENT_NAME << "-- failed allocate material model pointer\n";
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
    is_damping_computed = false;
    is_KFluid_computed  = false;

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


    for ( int i = 0; i < Num_Nodes; i++ )
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(4);

}

//====================================================================
BRICK_TYPE::BRICK_TYPE(): Element(0, ELE_TAG_BRICK_TYPE ),
      connectedExternalNodes(Num_Nodes), perm(Num_Dim),
      poro(0.0), alpha(1.0), rho_s(0.0), rho_f(0.0), ks(0.0), kf(0.0), 
      Q(Num_ElemDof), M( Num_ElemDof, Num_ElemDof), C( Num_ElemDof, Num_ElemDof), 
      KFluid( Num_ElemDof, Num_ElemDof), Ki(0)
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
    for ( int i = 0; i < Num_Nodes; i++ )
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(4);


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
const DTensor1 &BRICK_TYPE::interp_poli_at( double r1, double r2, double r3 ) const
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



const DTensor2 &BRICK_TYPE::dh_drst_at( double r1, double r2, double r3 ) const
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
    dh( 5, 2 ) = -( 1.0 - r1 ) * ( 1.0 + r2 ) * 0.125; ///8.0;// - (dh(13,3)+dh(14,3)+dh(18,3))/2.0;
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
    dh( 0, 2 ) =  ( 1.0 + r1 ) * ( 1.0 + r2 ) * 0.125; ///8.0;// - (dh(12,3)+dh(17,3)+dh(9,3))/2.0;
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
const DTensor2 &BRICK_TYPE::incr_disp( void ) const
{
    static DTensor2 increment_disp( Num_Nodes, Num_Dof, 0.0 );

    for (int i = 0; i < Num_Nodes; ++i)
    {
        for (int j = 0; j < Num_Dof; ++j)
        {
            increment_disp( i, j ) = (theNodes[i]->getIncrDeltaDisp())(j);
        }
    }


    return increment_disp;
}

////#############################################################################
const DTensor2 &BRICK_TYPE::total_disp( void ) const
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
const Vector &BRICK_TYPE::nodal_forces ()
{
    static Vector Pforce(Num_ElemDof);

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor2 nodal_forces( Num_Nodes, 3, 0.0 );
    static DTensor2 dh( Num_Nodes, 3 , 0.0 );
    static DTensor2 stress_at_GP( 3, 3, 0.0 );
    static DTensor2 Jacobian( 3, 3, 0.0 );
    static DTensor2 JacobianINV( 3, 3, 0.0 );
    static DTensor2 dhGlobal( Num_Nodes, 3 , 0.0 );

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

    Pforce.Zero(); // necessary

    for (int i = 0; i < Num_Nodes; i++)
    {
        for (int j = 0; j < Num_Dim; j++)
        {
            Pforce(i * Num_Dof + j) = nodal_forces(i,j);
        }
    }

    return Pforce;
}


//Compute gauss point coordinates
void BRICK_TYPE::computeGaussPoint()
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    DTensor2 NodalCoord( 3, Num_Nodes, 0.0 );
    DTensor2 material_arrayCoord( 3, Num_TotalGaussPts, 0.0 );
    DTensor2 H( Num_Nodes*3, 3, 0.0 );

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

//======================================================================
int BRICK_TYPE::getNumExternalNodes (void) const
{
    return Num_Nodes;
}

//======================================================================
const ID &BRICK_TYPE::getExternalNodes (void)
{
    return connectedExternalNodes;
}

//======================================================================
Node **BRICK_TYPE::getNodePtrs (void)
{
    return theNodes;
}

//======================================================================
int BRICK_TYPE::getNumDOF (void)
{
    return Num_ElemDof;
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
    }
    else
    {

        for (int i = 0; i < Num_Nodes; ++i)
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
            cerr << "FATAL ERROR " << BRICK_ELEMENT_NAME << " (tag: " << this->getTag() << "), node not found in domain\n";
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

        if ( dofNd1 != Num_Dof || dofNd2 != Num_Dof || dofNd3 != Num_Dof || dofNd4 != Num_Dof ||
                dofNd5 != Num_Dof || dofNd6 != Num_Dof || dofNd7 != Num_Dof || dofNd8 != Num_Dof )
        {
            cerr << "FATAL ERROR " << BRICK_ELEMENT_NAME << " (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit( -1 );
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
        cerr << BRICK_ELEMENT_NAME << " ::commitState () - failed in base class";
    }

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


    //P
    if (ks == 0.0 || kf == 0.0)
    {
        cerr << BRICK_ELEMENT_NAME << " ::getStiffnessTensorP -- solid and/or fluid bulk modulus is zero\n";
        exit(-1);
    }

    K.Zero();  // necessary
    K=getKFluid();

    getStiffnessTensorKep(); // gets current Kep Tensor

    // + Kep
    for (int  i = 0 ; i < Num_Nodes; i++ )
    {
        for (int  j = 0; j < Num_Nodes; j++ )
        {
            for (int  m = 0; m < Num_Dim; m++)
            {
                for ( int n = 0; n < Num_Dim; n++)
                {
                    K(i * Num_Dof + m, j * Num_Dof + n) =TensorKep(i, m, n, j);
                }
            }
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

//======================================================================
void BRICK_TYPE::getStiffnessTensorKep( )
{
    TensorKep*=0;

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor2 incremental_strain(3, 3, 0.0);
    static DTensor2 dh_drst(Num_Nodes, 3, 0.0 );
    static DTensor2 dhGlobal(Num_Nodes, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);
    static DTensor2 JacobianINV(3, 3, 0.0);
    static DTensor4 E_elpl(3, 3, 3, 3, 0.0);

    incremental_strain *= 0;
    dh_drst *= 0;
    dhGlobal *= 0;
    Jacobian *= 0;
    JacobianINV *= 0;
    E_elpl *= 0;

    Index < 'a' > a;
    Index < 'b' > b;
    Index < 'c' > c;
    Index < 'd' > d;

    static DTensor4 Kkt( 8, 3, 3, 8, 0.0);
    Kkt *= 0;

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
        TensorKep(i, a, c, j) = TensorKep(i, a, c, j) + Kkt(i, a, c, j);
    }

}


//======================================================================
void BRICK_TYPE::getStiffnessTensorQ()
{

    TensorQ *= 0;

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;


    static DTensor1 hp(Num_Nodes, 0.0 );
    static DTensor2 dh_drst( Num_Nodes, 3, 0.0 );
    static DTensor2 dhGlobal( Num_Nodes, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);
    static DTensor2 JacobianINV(3, 3, 0.0);

    hp *= 0;
    dh_drst *= 0;
    dhGlobal *= 0;
    Jacobian *= 0;
    JacobianINV *= 0;

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
        hp              = interp_poli_at( r, s, t );
        dh_drst         = dh_drst_at( r, s, t );
        Jacobian        = Jacobian_3D( dh_drst );
        JacobianINV     = Jacobian.Inv();
        det_of_Jacobian = Jacobian.compute_Determinant();

        // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
        dhGlobal(i, k)  = dh_drst( i, j ) * JacobianINV( k, j );
        weight          = w_r * w_s * w_t * det_of_Jacobian;


        //LTensorDisplay::print(E_elpl,"a","Hi there!",1);
        TensorQ(i, j, k) = TensorQ(i, j, k) + dhGlobal(i, j) * hp(k) * weight;
    }

}

//======================================================================
void BRICK_TYPE::getStiffnessTensorH()
{

    TensorH *= 0;

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor1 hp(Num_Nodes, 0.0 );
    static DTensor2 dh_drst( Num_Nodes, 3, 0.0 );
    static DTensor2 dhGlobal( Num_Nodes, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);
    static DTensor2 JacobianINV(3, 3, 0.0);
    static DTensor2 permiability(3, 3, 0.0);

    hp *= 0;
    dh_drst *= 0;
    dhGlobal *= 0;
    Jacobian *= 0;
    JacobianINV *= 0;
    permiability*=0;

    Index < 'a' > a;
    Index < 'b' > b;

    permiability(0, 0) = perm(0);
    permiability(1, 1) = perm(1);
    permiability(2, 2) = perm(2);

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

        // This is the actual integration
        TensorH(i, j) = TensorH(i, j) + dhGlobal(i,a) * permiability(a,b) * dhGlobal(j,b) * weight;
    }

}

//======================================================================
void BRICK_TYPE::getMassTensorMs()
{
    // This is for Ms and Mf -> M_kl
    // Ms = Ms * rho_s

    // Also this is for Compression term S
    // S = Ms * oneOverQ
    //   = Ms * (kf * ks)/(kf + ks)

    TensorMs *= 0;

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor1 hp(Num_Nodes, 0.0 );
    static DTensor2 dh_drst( Num_Nodes, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);

    hp *= 0;
    dh_drst *= 0;
    Jacobian *= 0;

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
        hp              = interp_poli_at( r, s, t );
        dh_drst         = dh_drst_at( r, s, t );
        Jacobian        = Jacobian_3D( dh_drst );
        det_of_Jacobian = Jacobian.compute_Determinant();

        weight          = w_r * w_s * w_t * det_of_Jacobian;

        //LTensorDisplay::print(E_elpl,"a","Hi there!",1);
        TensorMs(i,j) = TensorMs(i, j) +hp(i) * hp(j) * weight;
    }

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
        M.Zero();

        double Mtemp1 = 0.0;
        double rho_0 = (1.0 - poro) * rho_s + poro * rho_f;

        this->getMassTensorMs();

        for (int i = 0 ; i < Num_Nodes; i++ )
        {
            for (int j = 0; j < Num_Nodes; j++ )
            {

                //Ms, Note *rho_0 here!
                Mtemp1 = TensorMs(i, j) *rho_0;

                M(i * Num_Dof + 0, j * Num_Dof + 0) = Mtemp1;
                M(i * Num_Dof + 1, j * Num_Dof + 1) = Mtemp1;
                M(i * Num_Dof + 2, j * Num_Dof + 2) = Mtemp1;

            }
        }

        is_mass_computed = true;
    }

    return M;
}

//======================================================================
const Matrix &BRICK_TYPE::getDamp (void)
{
    if( not is_damping_computed)
    {
      
        C.Zero();  // necessary


        this->getStiffnessTensorQ();

        for (int i = 0 ; i < Num_Nodes; i++ )
        {
            for (int j = 0; j < Num_Nodes; j++ )
            {
                for (int m = 0; m < Num_Dim; m++)
                {
                    C(i * Num_Dof + 3, j * Num_Dof + m) = TensorQ(j, m, i);
                }
            }
        }

        double  oneOverQ = (alpha - poro) / ks + poro / kf;
        this->getMassTensorMs();

        for (int i = 0 ; i < Num_Nodes; i++ )
        {
            for (int j = 0; j < Num_Nodes; j++ )
            {
                C(i * Num_Dof + 3, j * Num_Dof + 3) = TensorMs(i, j)*oneOverQ;
            }
        }

    // Commented by Sumeet [Please Fix me whether I am required or not!! September, 2016]

    // if (a1 != 0.0)
    // {
    //     tensor Ks = this->getStiffnessTensorKep();

    //     for ( i = 0 ; i < Num_Nodes; i++ )
    //     {
    //         for ( j = 0; j < Num_Nodes; j++ )
    //         {
    //             for ( m = 0; m < Num_Dim; m++)
    //             {
    //                 for ( n = 0; n < Num_Dim; n++)
    //                 {
    //                     MCK(i * Num_Dof + m, j * Num_Dof + n) += Ks.cval(i + 1, m + 1, j + 1, n + 1) * a1;
    //                 }
    //             }
    //         }
    //     }
    // }

    // if (a0 != 0.0)
    // {
    //     tensor Msf = this->getMassTensorM1();

    //     for ( i = 0 ; i < Num_Nodes; i++ )
    //     {
    //         for ( j = 0; j < Num_Nodes; j++ )
    //         {
    //             dampT = Msf.cval(i + 1, j + 1) * a0;
    //             MCK(i * Num_Dof + 0, j * Num_Dof + 0) += dampT;
    //             MCK(i * Num_Dof + 1, j * Num_Dof + 1) += dampT;
    //             MCK(i * Num_Dof + 2, j * Num_Dof + 2) += dampT;
    //         }
    //     }
    // }

    }

    return C;
}

//======================================================================
const Matrix &BRICK_TYPE::getKFluid()
{

    if(not is_KFluid_computed)
    {

        KFluid*=0;

        getStiffnessTensorQ();      // gets Stiffness Q Tensor
        getStiffnessTensorH();      // gets Pressure Tensor

        // Q Here
        for (int i = 0 ; i < Num_Nodes; i++ )
        {
            for (int j = 0; j < Num_Nodes; j++ )
            {
                for (int m = 0; m < Num_Dim; m++)
                {
                    KFluid(i * Num_Dof + m, j * Num_Dof + 3) = -TensorQ(i, m, j) * alpha;
                }
            }
        }

        //H Here
        for (int i = 0 ; i < Num_Nodes; i++ )
        {
            for (int j = 0; j < Num_Nodes; j++ )
            {
                KFluid(i * Num_Dof + 3, j * Num_Dof + 3) = TensorH(i, j);
            }
        }

        is_KFluid_computed = true;
    }

    return KFluid;

}

//=============================================================================
void BRICK_TYPE::zeroLoad( void )
{
    Q.Zero();
}



//======================================================================
const Vector &BRICK_TYPE::getBodyForce( double loadFactor, const Vector &data )
{

    static Vector bforce(Num_ElemDof);

    DTensor1 bf(3,0.0);
    bf(0) = data(0)*loadFactor;
    bf(1) = data(1)*loadFactor;
    bf(2) = data(2)*loadFactor;

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor1 Pexf(Num_Nodes, 0.0 );
    static DTensor1 Pfluid(Num_Nodes, 0.0 );
        
    static DTensor1 hp(Num_Nodes, 0.0 );
    static DTensor2 dh_drst( Num_Nodes, 3, 0.0 );
    static DTensor2 dhGlobal( Num_Nodes, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);
    static DTensor2 JacobianINV(3, 3, 0.0);
    static DTensor2 permiability(3, 3, 0.0);

    double rho_0 = (1.0 - poro) * rho_s + poro * rho_f;

    Pexf *= 0;
    Pfluid *=0;

    hp *= 0;
    dh_drst *= 0;
    Jacobian *= 0;
    JacobianINV *=0;
    permiability*=0;

    permiability(0, 0) = perm(0);
    permiability(1, 1) = perm(1);
    permiability(2, 2) = perm(2);

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
        hp              = interp_poli_at( r, s, t );
        dh_drst         = dh_drst_at( r, s, t );
        Jacobian        = Jacobian_3D( dh_drst );
        JacobianINV     = Jacobian.Inv();
        det_of_Jacobian = Jacobian.compute_Determinant();

        // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
        dhGlobal(i, k)  = dh_drst( i, j ) * JacobianINV( k, j );
        weight          = w_r * w_s * w_t * det_of_Jacobian;

        //LTensorDisplay::print(E_elpl,"a","Hi there!",1);
        Pexf(i) = Pexf(i) +hp(i) * weight;
        Pfluid(i) = Pfluid(i) + dhGlobal(i,j) * permiability(j,k) * bf(k)* rho_f * weight;
    }

    bforce.Zero(); // necessary

    for (int i = 0; i < Num_Nodes; i++)
    {
        for (int j = 0; j < Num_Dim; j++)
        {
            bforce(i * Num_Dof + j) = Pexf(i) * bf(j) * rho_0;
        }
        bforce(i * Num_Dof + 3) = Pfluid(i);
    }

    return bforce;
}


//======================================================================
const Vector &BRICK_TYPE::getSurfaceForce( double loadFactor, const Vector &data )
{

    map<int,int> local_nodes_map; int local_nodes[4];
  
    /////////////////////////////////////////// Edited by Sumeet 30/03/2016 //////////////////////////////
    // checking if node exists in the element
    for ( int i =0; i<4 ;i++){
        std::map<int,int>::iterator it;
        it=Global_to_Local_Node_Mapping.find(data(i));
        if (it == Global_to_Local_Node_Mapping.end()){
            cerr << "\n" << BRICK_ELEMENT_NAME << "::getSurfaceForce  Node " <<  data(i) << " defined for the BrickSurfaceLoad does not belong to element \n" ;
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
    //  Finding the correct surface nodes order
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
        cerr << "\n" << BRICK_ELEMENT_NAME << "::getSurfaceForce  Nodes  defined for the BrickSurfaceLoad does not belong to elements surface  \n" ;
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

    static Vector NodalForces(Num_ElemDof);
    for (int m = 0; m < Num_ElemDof; m++){
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
                NodalForces( r * Num_Dof + k ) = NodalForces( r * Num_Dof + k ) + (alpha - poro) * LoadValue * J_vector( k ) * ShapeFunctionValues * Weight(i, 0) * Weight(i, 1); // solid part
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
int BRICK_TYPE::addLoad( ElementalLoad *theLoad, double loadFactor )
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
        cerr << BRICK_ELEMENT_NAME << "::addLoad() - " << BRICK_ELEMENT_NAME  << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }


    return 0;
}




//=============================================================================
int BRICK_TYPE::addInertiaLoadToUnbalance( const Vector &accel )
{

    static Vector avu(Num_ElemDof);
    int ik;

    for (int i = 0; i < Num_Nodes; i++)
    {
        const Vector &RA = theNodes[i]->getRV(accel);

        if ( RA.Size() != Num_Dof )
        {
           cerr << BRICK_ELEMENT_NAME << "::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
            return (-1);
        }

        ik = i * Num_Dof;

        avu(ik + 0) = RA(0);
        avu(ik + 1) = RA(1);
        avu(ik + 2) = RA(2);
        avu(ik + 3) = 0.0;
        avu(ik + 4) = RA(4);
        avu(ik + 5) = RA(5);
        avu(ik + 6) = RA(6);
    }

    this->getMass();
    Q.addMatrixVector(1.0, M, avu, -1.0);

    return 0;
}

//=============================================================================
const Vector &BRICK_TYPE::getResistingForce()
{

    static Vector avu(Num_ElemDof);

    P.Zero();

    P = this->nodal_forces();

    // + K0*upU
    for (int i = 0; i < Num_Nodes; i++)
    {
        const Vector &disp = theNodes[i]->getTrialDisp();

        if ( disp.Size() != Num_Dof )
        {
            cerr << BRICK_ELEMENT_NAME << "::getResistingForce(): matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (int j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = disp(j);
        }
    }

    this->getKFluid();

    P.addMatrixVector(1.0, KFluid, avu, 1.0);

    P.addVector(1.0, Q, -1.0);

    return P;
}

//=============================================================================
const Vector &BRICK_TYPE::getResistingForceIncInertia ()
{
    static Vector avu(Num_ElemDof);

    this->getResistingForce();

    // + M*a
    int i, j;

    for (int i = 0; i < Num_Nodes; i++)
    {
        const Vector &acc = theNodes[i]->getTrialAccel();

        if ( acc.Size() != Num_Dof )
        {
            cerr << BRICK_ELEMENT_NAME << "::getResistingForceIncInertia matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (int j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = acc(j);
        }
    }

    this->getMass();
    P.addMatrixVector(1.0, M, avu, 1.0);

    // + C*v
    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &vel = theNodes[i]->getTrialVel();

        if ( vel.Size() != Num_Dof )
        {
            cerr << BRICK_ELEMENT_NAME << "::getResistingForceIncInertia matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = vel(j);
        }
    }

    this->getDamp();
    P.addMatrixVector(1.0, C, avu, 1.0);

    return P;
}




int BRICK_TYPE::sendSelf ( int commitTag, Channel &theChannel )
{
 
    ID idData(6);

    idData( 0 ) = this->getTag();
    idData( 1 ) = material_array[0]->getClassTag();
    idData( 2 ) = getElasticMaterialStatus();
    idData( 3 ) = is_mass_computed;
    idData( 4 ) = is_damping_computed;
    idData( 5 ) = is_KFluid_computed;


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

    // Sending the Damping matrix if it is computed
    if(is_damping_computed){
        if ( theChannel.sendMatrix( 0, commitTag, C ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send its C\n";
            return -1;
        }
    }

    // Sending the K_Fluid matrix if it is computed
    if(is_KFluid_computed){
        if ( theChannel.sendMatrix( 0, commitTag, KFluid ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send its KFluid\n";
            return -1;
        }
    }

    // send double data
    Vector floatData(9);
    floatData(0) = perm(0);
    floatData(1) = perm(1);
    floatData(2) = perm(2);
    floatData(3) = poro;
    floatData(4) = alpha;
    floatData(5) = rho_s;
    floatData(6) = rho_f;
    floatData(7) = ks;
    floatData(8) = kf;

    if ( theChannel.sendVector( 0, commitTag, floatData ) < 0 )
    {
       cerr << "WARNING " << BRICK_ELEMENT_NAME << "::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
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

    ID idData( 6 );

    if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - failed to receive ID\n";
        return -1;
    }

    this->setTag( idData( 0 ) );
    int matClassTag = idData( 1 );
    setElasticMaterialStatus(idData(2));
    is_mass_computed = idData(3);
    is_damping_computed = idData(4);
    is_KFluid_computed  = idData(5);

    // Sending the Stiffness matrix for elastic material
    if(idData( 2 )==1){
        if ( theChannel.receiveMatrix( 0, commitTag, K_Elastiic ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to send its K\n";
            return -1;
        }
    }

    // Sending the Mass matrix if it is computed
    if(is_mass_computed){
        if ( theChannel.receiveMatrix( 0, commitTag, M ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to send its M\n";
            return -1;
        }
    }

    // Sending the Damping matrix if it is computed
    if(is_damping_computed){
        if ( theChannel.receiveMatrix( 0, commitTag, C ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to send its C\n";
            return -1;
        }
    }

    // Sending the K_Fluid matrix if it is computed
    if(is_KFluid_computed){
        if ( theChannel.receiveMatrix( 0, commitTag, KFluid ) < 0 )
        {
            cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to send its KFluid\n";
            return -1;
        }
    }

    Vector floatData(9);
    if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }
    perm(0) = floatData(0);
    perm(1) = floatData(1);
    perm(2) = floatData(2);
    poro    = floatData(3);
    alpha   = floatData(4);
    rho_s   = floatData(5);
    rho_f   = floatData(6);
    ks      = floatData(7);
    kf      = floatData(8);

    // Recieve the nodes
    if ( theChannel.receiveID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING " << BRICK_ELEMENT_NAME << "::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
        return -1;
    }

    // For Surface Load [Sumeet, September, 2016]
    Global_to_Local_Node_Mapping[ connectedExternalNodes(0)]=0;
    Global_to_Local_Node_Mapping[ connectedExternalNodes(1)]=1;
    Global_to_Local_Node_Mapping[ connectedExternalNodes(2)]=2;
    Global_to_Local_Node_Mapping[ connectedExternalNodes(3)]=3;
    Global_to_Local_Node_Mapping[ connectedExternalNodes(4)]=4;
    Global_to_Local_Node_Mapping[ connectedExternalNodes(5)]=5;
    Global_to_Local_Node_Mapping[ connectedExternalNodes(6)]=6;
    Global_to_Local_Node_Mapping[ connectedExternalNodes(7)]=7;  


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
    size += sizeof(is_damping_computed);
    size += sizeof(is_KFluid_computed);
    size += sizeof(Volume);
    size += sizeof(double)*perm.Size();
    size += sizeof(poro);
    size += sizeof(alpha);
    size += sizeof(rho_s);
    size += sizeof(rho_f);
    size += sizeof(ks);
    size += sizeof(kf);
    size += sizeof(int) * connectedExternalNodes.Size();
    size += sizeof(theNodes) ;
    size += sizeof(mmodel);
    size += sizeof(material_array) * Num_TotalGaussPts;
    size += sizeof(double) * Q.Size();
    size += sizeof(double) * M.noRows()*M.noCols();
    size += sizeof(double) * C.noRows()*C.noCols();
    size += sizeof(double) * KFluid.noRows()*KFluid.noCols();
    size += sizeof(Ki);
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
    cout<< BRICK_ELEMENT_NAME << ":: LTensor-based 8 node brick." << endl << endl;

    cout << "Element tag: " << getTag() << endl;
    cout << "Connected Nodes: " << endl;

    for ( int node = 0; node < Num_Nodes; node++)
    {
        cout << "   #" << node + 1 << ": Domain node #" << connectedExternalNodes(node) << endl;
    }

    cout << "K = " << endl;

    for (int ii = 0; ii < Num_ElemDof; ii++)
    {
        for (int jj = 0; jj < Num_ElemDof; jj++)
        {
            cout << K(ii, jj);

            if (jj < Num_ElemDof-1)
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

    for (int ii = 0; ii < Num_ElemDof; ii++)
    {
        cout << K(ii, ii);

        if (ii < Num_ElemDof-1)
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
        cout << "   GP # " << gp <<  ": (" 
             << gp_coords(gp, 0) << ","
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

    static DTensor2 dh( Num_Nodes, 3, 0.0 );
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

    //DTensor2 Jacobian(3,3,0.0);
    static DTensor2 dh( Num_Nodes, 3, 0.0 );
    static DTensor2 JacobianINV(3, 3, 0.0);
    static DTensor2 dhGlobal( Num_Nodes, 3, 0.0 );
    static DTensor2 trial_disp( Num_Nodes, 3, 0.0  );
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
        trial_strain(i, j) = 0.5 * (dhGlobal( k, i ) * trial_disp( k, j ) + dhGlobal( k, j ) * trial_disp( k, i ));
        // LTensorDisplay::print(trial_strain, "trial_strain");

        // if ( ( material_array[gp]->setTrialStrain( trial_strain ) ) )
        // Get back by Yuan to solve the zero elastic strain problems.
        material_array[gp]->setTrialStrain( trial_strain );
        if ( ( material_array[gp]->setTrialStrainIncr( trial_strain ) ) )
        {
            Matrix &gps = getGaussCoordinates();
             cerr << BRICK_ELEMENT_NAME << "::update (tag: " << this->getTag()
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
double BRICK_TYPE::SurfaceShapeFunctionValues( double Xi , double Eta, int whichcomponent )
{
    ShapeFunctionValues_in_function( 0 ) = 0.25 * ( 1 + Xi ) * ( 1 + Eta );
    ShapeFunctionValues_in_function( 1 ) = 0.25 * ( 1 - Xi ) * ( 1 + Eta );
    ShapeFunctionValues_in_function( 2 ) = 0.25 * ( 1 - Xi ) * ( 1 - Eta );
    ShapeFunctionValues_in_function( 3 ) = 0.25 * ( 1 + Xi ) * ( 1 - Eta );

    return ShapeFunctionValues_in_function( whichcomponent );
}


Vector &BRICK_TYPE::Direction_Weight( double Xi , double Eta, Vector coord1, Vector coord2, Vector coord3, Vector coord4 )
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


double BRICK_TYPE::SurfaceLoadValues( double Xi , double Eta, Vector Pressure )
{

    SurfaceLoadValues_in_function =  0.25 * ( 1 + Xi ) * ( 1 + Eta ) * Pressure( 0 )
                                     + 0.25 * ( 1 - Xi ) * ( 1 + Eta ) * Pressure( 1 )
                                     + 0.25 * ( 1 - Xi ) * ( 1 - Eta ) * Pressure( 2 )
                                     + 0.25 * ( 1 + Xi ) * ( 1 - Eta ) * Pressure( 3 );



    return SurfaceLoadValues_in_function;

}

int
BRICK_TYPE::CheckMesh( ofstream &checkmesh_file )
{
    bool jacobian_flag = false;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    double det_of_Jacobian = 0.0;

    DTensor2 dh( Num_Nodes, 3, 0.0 );
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
BRICK_TYPE::getStress( void )
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
    //Form the output vector
    int ii = 0;
    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
    {
        const DTensor2 & strain = material_array[gp]->getStrainTensor();
        const DTensor2 & plstrain = material_array[gp]->getPlasticStrainTensor();
        const DTensor2 & stress = material_array[gp]->getStressTensor();

        //Write strain
        Gauss_Output_Vector[ii++] = strain(0, 0);
        Gauss_Output_Vector[ii++] = strain(1, 1);
        Gauss_Output_Vector[ii++] = strain(2, 2);
        Gauss_Output_Vector[ii++] = strain(0, 1);
        Gauss_Output_Vector[ii++] = strain(0, 2);
        Gauss_Output_Vector[ii++] = strain(1, 2);

        //Write plastic strain
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

