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
// Adapted by Sumeet Kumar Sinha, from TwentyNodeBrick to use the LTensor library for
// tensorial representation.
/////////////////////////////////////////////////////////////////////////////////////


#ifndef TwentySevenNodeBrick_upU_LTLT_CPP
#define TwentySevenNodeBrick_upU_LTLT_CPP

#include "TwentySevenNodeBrick_upU_LT.h"
#include <LTensorDisplay.h>
#include <HDF5_Channel.h>
#include <ESSITimer.h>
#include <unistd.h>


#define Num_IntegrationPts 3
#define Num_TotalGaussPts 27
#define Num_Nodes 27
#define Num_Dim 3
#define Num_Dof 7
#define Num_ElemDof 189

DTensor2 TwentySevenNodeBrick_upU_LT::gp_coords(Num_TotalGaussPts, 3, 0.0);
DTensor2 TwentySevenNodeBrick_upU_LT::gp_weight(Num_TotalGaussPts, 3, 0.0);
DTensor4 TwentySevenNodeBrick_upU_LT::TensorKep(Num_Nodes,3,3,Num_Nodes,0.0);
DTensor3 TwentySevenNodeBrick_upU_LT::TensorG12(Num_Nodes, 3, Num_Nodes, 0.0);;
DTensor4 TwentySevenNodeBrick_upU_LT::TensorC123(Num_Nodes, 3, 3, Num_Nodes, 0.0);
DTensor2 TwentySevenNodeBrick_upU_LT::TensorMsf(Num_Nodes, Num_Nodes, 0.0);
Matrix   TwentySevenNodeBrick_upU_LT::gauss_points(Num_TotalGaussPts, 3);

vector<float> TwentySevenNodeBrick_upU_LT::Gauss_Output_Vector(Num_TotalGaussPts*18);
Matrix TwentySevenNodeBrick_upU_LT::K( Num_ElemDof, Num_ElemDof);

// Vector TwentySevenNodeBrick_upU_LT::ShapeFunctionValues_in_function( 4 );  // Nima added for surface load (July 2012)
// Vector TwentySevenNodeBrick_upU_LT::J_vector_in_function( 3 );             // Nima added for surface load (July 2012)
// double TwentySevenNodeBrick_upU_LT::SurfaceLoadValues_in_function;         // Nima added for surface load (July 2012)



TwentySevenNodeBrick_upU_LT::TwentySevenNodeBrick_upU_LT( int element_number,
        int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
        int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
        int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
        int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
        int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
        int node_numb_21,  int node_numb_22,  int node_numb_23,  int node_numb_24,
        int node_numb_25,  int node_numb_26,  int node_numb_27,
        NDMaterialLT *Globalmmodel,
        double nn,
        double alf,
        double rs,
        double rf,
        double permb_x,
        double permb_y,
        double permb_z,
        double kks,
        double kkf)
    : Element(element_number,
              ELE_TAG_TwentySevenNodeBrick_upU ),
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
      Ki(0),
      P(Num_ElemDof)
{
    
    this->setMaterialTag(Globalmmodel->getTag());
     
    // permeability
    perm(0) = permb_x;
    perm(1) = permb_y;
    perm(2) = permb_z;

    if (perm(0) == 0.0 || perm(1) == 0.0 || perm(2) == 0.0)
    {
        cerr << " Error, TwentyNodeBrick_u_p_U:: permeability (kx/ky/kz) is zero! \n";
        exit(-1);
    }


    for (int i = 0; i < Num_TotalGaussPts; i++)
    {
        material_array[i] = Globalmmodel->getCopy();

    }

    //GP coordinates and weights. ====================================================
    // This initializes class wide members gp_coords and gp_weights .
    // Since LTensor does not provide initializer lists, this is the only way to
    // do this.
    short where = 0;

    for ( short ii = 0 ; ii < 3 ; ii++ )
    {
        for ( short jj = 0 ; jj < 3 ; jj++ )
        {
            for ( short kk = 0 ; kk < 3 ; kk++ )
            {
                if (ii == 0)
                {
                    gp_coords(where, 0) = -0.774596669241483;
                }
                else if (ii == 1)
                {
                    gp_coords(where, 0) = 0.0;
                }
                else
                {
                    gp_coords(where, 0) = 0.774596669241483;
                }

                if (jj == 0)
                {
                    gp_coords(where, 1) = -0.774596669241483;
                }
                else if (jj == 1)
                {
                    gp_coords(where, 1) = 0.0;
                }
                else
                {
                    gp_coords(where, 1) = 0.774596669241483;
                }

                if (kk == 0)
                {
                    gp_coords(where, 2) = -0.774596669241483;
                }
                else if (kk == 1)
                {
                    gp_coords(where, 2) = 0.0;
                }
                else
                {
                    gp_coords(where, 2) = 0.774596669241483;
                }

                where ++;
            }
        }
    }

    where = 0;

    for ( short ii = 0 ; ii < 3 ; ii++ )
    {
        for ( short jj = 0 ; jj < 3 ; jj++ )
        {
            for ( short kk = 0 ; kk < 3 ; kk++ )
            {
                if (ii == 0)
                {
                    gp_weight(where, 0) = 0.555555555555556;
                }
                else if (ii == 1)
                {
                    gp_weight(where, 0) = 0.888888888888889;
                }
                else
                {
                    gp_weight(where, 0) = 0.555555555555556;
                }

                if (jj == 0)
                {
                    gp_weight(where, 1) = 0.555555555555556;
                }
                else if (jj == 1)
                {
                    gp_weight(where, 1) = 0.888888888888889;
                }
                else
                {
                    gp_weight(where, 1) = 0.555555555555556;
                }

                if (kk == 0)
                {
                    gp_weight(where, 2) = 0.555555555555556;
                }
                else if (kk == 1)
                {
                    gp_weight(where, 2) = 0.888888888888889;
                }
                else
                {
                    gp_weight(where, 2) = 0.555555555555556;
                }

                where ++;
            }
        }
    }
    // =============================================================================


    is_mass_computed = false;
    is_damping_computed = false;

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

    // Global_to_Local_Node_Mapping[node_numb_1] =0;
    // Global_to_Local_Node_Mapping[node_numb_2] =1;
    // Global_to_Local_Node_Mapping[node_numb_3] =2;
    // Global_to_Local_Node_Mapping[node_numb_4] =3;
    // Global_to_Local_Node_Mapping[node_numb_5] =4;
    // Global_to_Local_Node_Mapping[node_numb_6] =5;
    // Global_to_Local_Node_Mapping[node_numb_7] =6;
    // Global_to_Local_Node_Mapping[node_numb_8] =7;
    // Global_to_Local_Node_Mapping[node_numb_9] =8;
    // Global_to_Local_Node_Mapping[node_numb_10] =9;
    // Global_to_Local_Node_Mapping[node_numb_11] =10;
    // Global_to_Local_Node_Mapping[node_numb_12] =11;
    // Global_to_Local_Node_Mapping[node_numb_13] =12;
    // Global_to_Local_Node_Mapping[node_numb_14] =13;
    // Global_to_Local_Node_Mapping[node_numb_15] =14;
    // Global_to_Local_Node_Mapping[node_numb_16] =15;
    // Global_to_Local_Node_Mapping[node_numb_17] =16;
    // Global_to_Local_Node_Mapping[node_numb_18] =17;
    // Global_to_Local_Node_Mapping[node_numb_19] =18;
    // Global_to_Local_Node_Mapping[node_numb_20] =19;
    // Global_to_Local_Node_Mapping[node_numb_21] =20;
    // Global_to_Local_Node_Mapping[node_numb_22] =21;
    // Global_to_Local_Node_Mapping[node_numb_23] =22;
    // Global_to_Local_Node_Mapping[node_numb_24] =23;
    // Global_to_Local_Node_Mapping[node_numb_25] =24;
    // Global_to_Local_Node_Mapping[node_numb_26] =25;
    // Global_to_Local_Node_Mapping[node_numb_27] =26;


    for ( int i = 0; i < Num_Nodes; i++ )
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(9);

}

//====================================================================
TwentySevenNodeBrick_upU_LT::TwentySevenNodeBrick_upU_LT(): Element(0, ELE_TAG_TwentySevenNodeBrick_upU ),
      connectedExternalNodes(Num_Nodes), perm(Num_Dim),
      poro(0.0), alpha(1.0), rho_s(0.0), rho_f(0.0), ks(0.0), kf(0.0), 
      Q(Num_ElemDof), M( Num_ElemDof, Num_ElemDof), C( Num_ElemDof, Num_ElemDof), Ki(0),
      P(Num_ElemDof)
{
    is_mass_computed = false;
    is_damping_computed = false;

    //GP coordinates and weights. ====================================================
    // This initializes class wide members gp_coords and gp_weights .
    // Since LTensor does not provide initializer lists, this is the only way to
    // do this.
    short where = 0;

    for ( short ii = 0 ; ii < 3 ; ii++ )
    {
        for ( short jj = 0 ; jj < 3 ; jj++ )
        {
            for ( short kk = 0 ; kk < 3 ; kk++ )
            {
                if (ii == 0)
                {
                    gp_coords(where, 0) = -0.774596669241483;
                }
                else if (ii == 1)
                {
                    gp_coords(where, 0) = 0.0;
                }
                else
                {
                    gp_coords(where, 0) = 0.774596669241483;
                }

                if (jj == 0)
                {
                    gp_coords(where, 1) = -0.774596669241483;
                }
                else if (jj == 1)
                {
                    gp_coords(where, 1) = 0.0;
                }
                else
                {
                    gp_coords(where, 1) = 0.774596669241483;
                }

                if (kk == 0)
                {
                    gp_coords(where, 2) = -0.774596669241483;
                }
                else if (kk == 1)
                {
                    gp_coords(where, 2) = 0.0;
                }
                else
                {
                    gp_coords(where, 2) = 0.774596669241483;
                }

                where ++;
            }
        }
    }

    where = 0;

    for ( short ii = 0 ; ii < 3 ; ii++ )
    {
        for ( short jj = 0 ; jj < 3 ; jj++ )
        {
            for ( short kk = 0 ; kk < 3 ; kk++ )
            {
                if (ii == 0)
                {
                    gp_weight(where, 0) = 0.555555555555556;
                }
                else if (ii == 1)
                {
                    gp_weight(where, 0) = 0.888888888888889;
                }
                else
                {
                    gp_weight(where, 0) = 0.555555555555556;
                }

                if (jj == 0)
                {
                    gp_weight(where, 1) = 0.555555555555556;
                }
                else if (jj == 1)
                {
                    gp_weight(where, 1) = 0.888888888888889;
                }
                else
                {
                    gp_weight(where, 1) = 0.555555555555556;
                }

                if (kk == 0)
                {
                    gp_weight(where, 2) = 0.555555555555556;
                }
                else if (kk == 1)
                {
                    gp_weight(where, 2) = 0.888888888888889;
                }
                else
                {
                    gp_weight(where, 2) = 0.555555555555556;
                }

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

    this->setNumberOfBoundaryNodes(9);


}

//####DESTROYER!###############################################################
TwentySevenNodeBrick_upU_LT::~TwentySevenNodeBrick_upU_LT ()
{
    for ( int i = 0; i < Num_TotalGaussPts; i++ )
    {
        // Delete the NDMaterials at each integration point
        if ( material_array[i] )
        {
            delete material_array[i];
        }
    }

    for (int j = 0; j < Num_Nodes; j++)
    {
        theNodes[j] = 0;
    }

    if ( Ki != 0 )
    {
        delete Ki;
    }

}

//#############################################################################
//#############################################################################
//***************************************************************
const DTensor2 &TwentySevenNodeBrick_upU_LT::H_3D(double r1, double r2, double r3) const
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
const DTensor1 &TwentySevenNodeBrick_upU_LT::interp_poli_at(double r1, double r2, double r3) const
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




const DTensor2 &TwentySevenNodeBrick_upU_LT::dh_drst_at(double r1, double r2, double r3) const
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
const DTensor2 &TwentySevenNodeBrick_upU_LT::Jacobian_3D( const DTensor2 &dh) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    static DTensor2 Jacobian_3D_( 3, 3, 0.0);
    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    return Jacobian_3D_;
}


//#############################################################################
const DTensor2 &TwentySevenNodeBrick_upU_LT::Jacobian_3Dinv(const DTensor2 &dh) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    DTensor2 Jacobian_3D_(3, 3, 0.0);
    static DTensor2 Jacobian_3D_inv(3, 3, 0.0);

    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    Jacobian_3D_inv = Jacobian_3D_.Inv();
    return Jacobian_3D_inv;
}



////#############################################################################
const DTensor2 &TwentySevenNodeBrick_upU_LT::Nodal_Coordinates(void) const
{
    static DTensor2 N_coord( Num_Nodes, 3, 0.0 );
    for (int i = 0; i < Num_Nodes; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            N_coord( i, j ) = (theNodes[i]->getCrds())(j);
        }
    }

    return N_coord;

}

////#############################################################################
const DTensor2 &TwentySevenNodeBrick_upU_LT::incr_disp(void) const
{
    static DTensor2 increment_disp( Num_Nodes, 3, 0.0 );

    for (int i = 0; i < Num_Nodes; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            increment_disp( i, j ) = (theNodes[i]->getIncrDeltaDisp())(j);
        }
    }


    return increment_disp;
}

////#############################################################################
const DTensor2 &TwentySevenNodeBrick_upU_LT::total_disp(void) const
{
    static DTensor2 total_disp( Num_Nodes, 3, 0.0 );
    for (int i = 0; i < Num_Nodes; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            total_disp( i, j ) = (theNodes[i]->getTrialDisp())(j);
        }
    }

    return total_disp;
}

////#############################################################################
int TwentySevenNodeBrick_upU_LT::get_global_number_of_node(int local_node_number)
{
    return connectedExternalNodes(local_node_number);
}

////#############################################################################
int  TwentySevenNodeBrick_upU_LT::get_Brick_Number(void)
{
    return this->getTag();
}


//Compute gauss point coordinates
void TwentySevenNodeBrick_upU_LT::computeGaussPoint()
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

        for ( int encount = 0 ; encount < Num_TotalGaussPts ; encount++ )
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

//======================================================================
int TwentySevenNodeBrick_upU_LT::getNumExternalNodes (void) const
{
    return Num_Nodes;
}

//======================================================================
const ID &TwentySevenNodeBrick_upU_LT::getExternalNodes (void)
{
    return connectedExternalNodes;
}

//======================================================================
Node **TwentySevenNodeBrick_upU_LT::getNodePtrs (void)
{
    return theNodes;
}

//======================================================================
int TwentySevenNodeBrick_upU_LT::getNumDOF (void)
{
    return Num_ElemDof;
}

//=============================================================================
void TwentySevenNodeBrick_upU_LT::setDomain ( Domain *theDomain )
{
    // Check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
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
                theNodes[4] == 0 || theNodes[5] == 0 || theNodes[6] == 0 || theNodes[7] == 0 ||
                theNodes[8]  == 0 || theNodes[9] == 0 || theNodes[10] == 0 || theNodes[11] == 0 ||
                theNodes[12] == 0 || theNodes[13] == 0 || theNodes[14] == 0 || theNodes[15] == 0 ||
                theNodes[16] == 0 || theNodes[17] == 0 || theNodes[18] == 0 || theNodes[19] == 0 ||
                theNodes[20] == 0 || theNodes[21] == 0 || theNodes[22] == 0 || theNodes[23] == 0 ||
                theNodes[24] == 0 || theNodes[25] == 0 || theNodes[26] == 0)
        {

            cerr << "FATAL ERROR TwentSevenyNodeBrick_upU_LT (tag: " << this->getTag() << " ), node not found in domain\n";
            exit(-1);
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

        if ( dofNd1 != Num_Dof || dofNd2 != Num_Dof || dofNd3 != Num_Dof || dofNd4 != Num_Dof ||
                dofNd5 != Num_Dof || dofNd6 != Num_Dof || dofNd7 != Num_Dof || dofNd8 != Num_Dof ||
                dofNd9  != Num_Dof || dofNd10 != Num_Dof || dofNd11 != Num_Dof || dofNd12 != Num_Dof ||
                dofNd13 != Num_Dof || dofNd14 != Num_Dof || dofNd15 != Num_Dof || dofNd16 != Num_Dof ||
                dofNd17 != Num_Dof || dofNd18 != Num_Dof || dofNd19 != Num_Dof || dofNd20 != Num_Dof ||
                dofNd21 != Num_Dof || dofNd22 != Num_Dof || dofNd23 != Num_Dof || dofNd24 != Num_Dof ||
                dofNd25 != Num_Dof || dofNd26 != Num_Dof || dofNd27 != Num_Dof)
        {
            cerr << "FATAL ERROR TwentySevenNodeBrick_upU_LT (tag: " << this->getTag() <<
                 "), has differing number of DOFs at its nodes\n";
            exit(-1);
        }

        this->DomainComponent::setDomain(theDomain);
    }

    ComputeVolume();
    computeGaussPoint();
}

//=============================================================================
int TwentySevenNodeBrick_upU_LT::commitState ()
{

    int retVal = 0;

    // call element commitState to do any base class stuff
    if ( ( retVal = this->Element::commitState() ) != 0 )
    {
        cerr << "TwentySevenNodeBrick_upU_LT::commitState () - failed in base class";
    }

    for (int ii = 0; ii < Num_TotalGaussPts; ii++ )
    {
        retVal += material_array[ii]->commitState();
    }

    return retVal;
}


//=============================================================================
int TwentySevenNodeBrick_upU_LT::revertToLastCommit ()
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
int TwentySevenNodeBrick_upU_LT::revertToStart ()
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
const Matrix &TwentySevenNodeBrick_upU_LT::getTangentStiff()
{

    K.Zero();  // necessary

    //P
    if (ks == 0.0 || kf == 0.0)
    {
        cerr << " Error, TwentyNodeBrick_u_p_U::getStiffnessTensorP -- solid and/or fluid bulk modulus is zero\n";
        exit(-1);
    }


    getStiffnessTensorKep(); // gets current Kep Tensor
    getStiffnessTensorG12(); // gets Stiffness G12 Tensor
    getDampTensorC123();     // gets Damping C123 Tensor
    getMassTensorMsf();     // gets Pressure Tensor


    //G1 and G1^T, Note *(alpha-poro) here!
    for (int i = 0 ; i < Num_Nodes; i++ )
    {
        for (int j = 0; j < Num_Nodes; j++ )
        {
            for (int m = 0; m < Num_Dim; m++)
            {
                K(i * Num_Dof + m, j * Num_Dof + 3) = -TensorG12(i, m, j) * (alpha - poro);
                K(j * Num_Dof + 3, i * Num_Dof + m) = -TensorG12(i, m, j) * (alpha - poro);
            }
        }
    }

    //G2 and G2^T, Note *poro here!
    for (int i = 0 ; i < Num_Nodes; i++ )
    {
        for (int j = 0; j < Num_Nodes; j++ )
        {
            for (int m = 0; m < Num_Dim; m++)
            {
                K(i * Num_Dof + m + 4, j * Num_Dof + 3) = -TensorG12(i, m, j) * poro;
                K(j * Num_Dof + 3, i * Num_Dof + m + 4) = -TensorG12(i, m, j) * poro;
            }
        }
    }

    double  oneOverQ = poro / kf + (alpha - poro) / ks;

    for (int  i = 0 ; i < Num_Nodes; i++ )
    {
        for (int  j = 0; j < Num_Nodes; j++ )
        {
            K(i * Num_Dof + 3, j * Num_Dof + 3) = TensorMsf(i, j) * (-oneOverQ);
        }
    }


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

    return K;
}

//======================================================================
void TwentySevenNodeBrick_upU_LT::getStiffnessTensorKep( )
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
    static DTensor4 Kkt(Num_Nodes, 3, 3,Num_Nodes, 0.0);

    incremental_strain *= 0;
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
        TensorKep(i, a, c, j) = TensorKep(i, a, c, j) + Kkt(i, a, c, j);
    }

}


//======================================================================
void TwentySevenNodeBrick_upU_LT::getStiffnessTensorG12()
{

    TensorG12 *= 0;

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
        TensorG12(i, j, k) = TensorG12(i, j, k) + dhGlobal(i, j) * hp(k) * weight;
    }

}

//======================================================================
void TwentySevenNodeBrick_upU_LT::getDampTensorC123()
{

    // This is for C1, C2 and C3, C1 = C2 = c3
    // Since solid and fluid shape function the same

    TensorC123 *= 0;

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
    static DTensor2 perm_inv(3, 3, 0.0);
    static DTensor2 Jacobian(3, 3, 0.0);

    hp *= 0;
    perm_inv*=0;
    dh_drst *= 0;
    Jacobian *= 0;

    perm_inv(0, 0) = 1.0 / perm(0);
    perm_inv(1, 1) = 1.0 / perm(1);
    perm_inv(2, 2) = 1.0 / perm(2);

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
        TensorC123(i, j, k, l) = TensorC123(i, j, k, l) +hp(i) * perm_inv(j,k) * hp(l) * weight;
    }

}

//======================================================================
void TwentySevenNodeBrick_upU_LT::getMassTensorMsf()
{
    // This is for Ms and Mf -> M_kl
    // Ms = Msf * (1.0-poro)*rho_s
    // Mf = Msf * poro*rho_f

    // Also this is for Compression term Pc
    // Pc = Msf * oneOverQ
    //    = Msf * (poro/kf + (alpha-poro)/ks)

    TensorMsf *= 0;

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
        TensorMsf(i,j) = TensorMsf(i, j) +hp(i) * hp(j) * weight;
    }

}

//=============================================================================
const Matrix &TwentySevenNodeBrick_upU_LT::getInitialStiff ()
{

    if ( Ki == 0 )
    {
        Ki = new Matrix( this->getTangentStiff() );
    }

    if ( Ki == 0 )
    {
        cerr << "FATAL TwentyNodeBrickLT::getInitialStiff() -";
        cerr << "ran out of memory\n";
        exit( -1 );
    }

    return *Ki;
}

//=============================================================================
// Returns the matrix K [Sumeet September, 2016]
const Matrix &TwentySevenNodeBrick_upU_LT::getConstStiff()   
{
    return K;
}

//=============================================================================

const Matrix &TwentySevenNodeBrick_upU_LT::getMass ()
{

    if (not is_mass_computed)
    {
        M.Zero();
        double Mtemp1 = 0.0;
        double Mtemp2 = 0.0;
        this->getMassTensorMsf();

        for (int i = 0 ; i < Num_Nodes; i++ )
        {
            for (int j = 0; j < Num_Nodes; j++ )
            {

                //Ms, Note *(1.0-poro)*rho_s here!
                Mtemp1 = TensorMsf(i, j) * (1.0 - poro) * rho_s;

                M(i * Num_Dof + 0, j * Num_Dof + 0) = Mtemp1;
                M(i * Num_Dof + 1, j * Num_Dof + 1) = Mtemp1;
                M(i * Num_Dof + 2, j * Num_Dof + 2) = Mtemp1;

                //Mf, Note *poro*rho_f here!
                Mtemp2 = TensorMsf(i, j) * poro * rho_f;

                M(i * Num_Dof + 4, j * Num_Dof + 4) = Mtemp2;
                M(i * Num_Dof + 5, j * Num_Dof + 5) = Mtemp2;
                M(i * Num_Dof + 6, j * Num_Dof + 6) = Mtemp2;
            }
        }

        is_mass_computed = true;
    }

    //LTensorDisplay::print(Mm, "Mm", "", 1);

    return M;
}

//======================================================================
const Matrix &TwentySevenNodeBrick_upU_LT::getDamp (void)
{
    if( not is_damping_computed)
    {
      
        C.Zero();  // necessary
        double Ctemp;
        this->getDampTensorC123();

        for (int i = 0 ; i < Num_Nodes; i++ )
        {
            for (int j = 0; j < Num_Nodes; j++ )
            {
                for (int m = 0; m < Num_Dim; m++)
                {
                    for (int n = 0; n < Num_Dim; n++)
                    {
                        Ctemp = TensorC123(i, m, n, j) * (poro * poro);

                        //C1
                        C(i * Num_Dof + m, j * Num_Dof + n) = Ctemp;

                        //C3
                        C(i * Num_Dof + m + 4, j * Num_Dof + n + 4) = Ctemp;

                        //C2 and C2^T
                        C(i * Num_Dof + m, j * Num_Dof + n + 4) = - Ctemp;
                        C(j * Num_Dof + n + 4, i * Num_Dof + m) = - Ctemp;
                    }
                }
            }
        }
    }

    return C;
}

//=============================================================================
void TwentySevenNodeBrick_upU_LT::zeroLoad( void )
{
    Q.Zero();
}



//======================================================================
const Vector &TwentySevenNodeBrick_upU_LT::getBodyForce( double loadFactor, const Vector &data )
{

    static Vector bforce(Num_ElemDof);

    Vector bf(3);
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

    static DTensor1 hp(Num_Nodes, 0.0 );
    static DTensor1 Pexf(Num_Nodes, 0.0 );
    static DTensor2 dh_drst( Num_Nodes, 3, 0.0 );
    static DTensor2 Jacobian(3, 3, 0.0);

    hp *= 0;
    Pexf *= 0;
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
        Pexf(i) = Pexf(i) +hp(i) * weight;
    }

    bforce.Zero(); // necessary

    for (int i = 0; i < Num_Nodes; i++)
    {
        for (int j = 0; j < Num_Dim; j++)
        {
            bforce(i * Num_Dof + j) = Pexf(i) * bf(j) * (1.0 - poro) * rho_s;
            bforce(i * Num_Dof + j + 4) = Pexf(i) * bf(j) * poro * rho_f;
        }
    }

    return bforce;
}


// //======================================================================
// const Vector &TwentySevenNodeBrick_upU_LT::getSurfaceForce( double loadFactor, const Vector &data )
// {

//     map<int,int> local_nodes_map; int local_nodes[4];
  
//     /////////////////////////////////////////// Edited by Sumeet 30/03/2016 //////////////////////////////
//     // checking if node exists in the element
//     for ( int i =0; i<4 ;i++){
//         std::map<int,int>::iterator it;
//         it=Global_to_Local_Node_Mapping.find(data(i));
//         if (it == Global_to_Local_Node_Mapping.end()){
//             cerr << "\nERROR: Node " <<  data(i) << " defined for the BrickSurfaceLoad does not belong to element \n" ;
//             exit( 1 );
//         }
//         local_nodes_map[it->second]=i;
//         local_nodes[i]=it->second;
//     }

//     //////////////////////////////////////////////////////////////////////////////////////////////////////
//     static const int Node_to_Surface[8][9]={{1,2,3,4,5,1,3,7,4},
//                                             {2,3,0,0,4,5,5,6,2},
//                                             {3,0,1,6,7,3,1,5,6},
//                                             {0,1,2,2,6,7,7,4,0},
//                                             {7,6,5,5,1,0,0,3,7},
//                                             {4,7,6,1,0,4,6,2,1},
//                                             {5,4,7,7,3,2,2,1,5},
//                                             {6,5,4,4,0,3,3,2,6}};
//     ////////////////////////////////////// Edited by Sumeet 30/3/2016 /////////////////////////////////////
//     //  Finding the correct surface nodes order
//     int success =0; int surface_nodes_order[4]={0,0,0,0};
//     for ( int i =0; i<3 ;i++){
//         for( int j=0; j<3;j++){
//             std::map<int,int>::iterator it;
//             int node=Node_to_Surface[local_nodes[0]][3*i+j];
//             it=local_nodes_map.find(node);
//             if (it == local_nodes_map.end()){
//                 success=0;break;
//             }
//             success=success+1;
//             surface_nodes_order[j]= it->second;         
//         }
//         if(success==3) break;
//     }
//     if (success == 0){
//         cerr << "\nERROR: Nodes  defined for the BrickSurfaceLoad does not belong to elements surface  \n" ;
//         exit( 1 );
//     }    

//     ////////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
//     // cout << "surface_nodes_order ";
//     // for ( int i =0; i < 8 ; i++)
//     //     cout << local_nodes[i] << " ";
//     //     // cout << surface_nodes_order[i] << " ";
//     // cout << "\n";
//     // for ( int i =0; i < 8 ; i++)
//     //     cout << local_nodes[surface_nodes_order[i]] << " ";
//     // cout << "\n";
//     ////////////////////////////////////////////////////////////////////////////////////////////////////

//     // get the surface nodal coordinates
//     const Vector &coordnode1 = theNodes[local_nodes[(surface_nodes_order[0])]]->getCrds();
//     const Vector &coordnode2 = theNodes[local_nodes[(surface_nodes_order[1])]]->getCrds();
//     const Vector &coordnode3 = theNodes[local_nodes[(surface_nodes_order[2])]]->getCrds();
//     const Vector &coordnode4 = theNodes[local_nodes[(surface_nodes_order[3])]]->getCrds();

//     double ShapeFunctionValues;
//     double LoadValue;
//     Vector J_vector( 3 );
//     Vector Pressure( 4 );

//     Pressure( 0 ) = data( surface_nodes_order[0]+4) * loadFactor;
//     Pressure( 1 ) = data( surface_nodes_order[1]+4) * loadFactor;
//     Pressure( 2 ) = data( surface_nodes_order[2]+4) * loadFactor;
//     Pressure( 3 ) = data( surface_nodes_order[3]+4) * loadFactor;

//     static Vector NodalForces(24);
//     for (int m = 0; m < 24; m++){
//         NodalForces(m) = 0;
//     }

//     double oneOverSquareRoot3 = 1.0 / sqrt( 3.0 );
//     Matrix GsPts( 4, 2 );

//     GsPts( 0, 0 ) =  oneOverSquareRoot3;
//     GsPts( 0, 1 ) =  oneOverSquareRoot3;
//     GsPts( 1, 0 ) = -oneOverSquareRoot3;
//     GsPts( 1, 1 ) =  oneOverSquareRoot3;
//     GsPts( 2, 0 ) = -oneOverSquareRoot3;
//     GsPts( 2, 1 ) = -oneOverSquareRoot3;
//     GsPts( 3, 0 ) =  oneOverSquareRoot3;
//     GsPts( 3, 1 ) = -oneOverSquareRoot3;

//     int r = 0;

//     // loop over dof
//     for ( int k = 0; k < 3; k++ )
//     {
//         // loop over nodes
//         for ( int j = 0; j < 4; j++ )
//         {
//             r = local_nodes[(surface_nodes_order[j])] ;
//             // loop over Gauss points
//             for ( int i = 0; i < 4; i++ )
//             {
//                 ShapeFunctionValues = SurfaceShapeFunctionValues( GsPts( i, 0 ) , GsPts( i, 1 ), j );
//                 J_vector = Direction_Weight( GsPts( i, 0 ) , GsPts( i, 1 ), coordnode1, coordnode2, coordnode3, coordnode4 );
//                 LoadValue = SurfaceLoadValues( GsPts( i, 0 ) , GsPts( i, 1 ), Pressure );
//                 NodalForces( r * 3 + k ) = NodalForces( r * 3 + k ) + LoadValue * J_vector( k ) * ShapeFunctionValues;
//             }
//         }
//     }


//     // ////////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
//     // for ( int i =0; i < 8 ; i++)
//     //     cout << NodalForces(3*i)<< " " << NodalForces(3*i+1) << " " << NodalForces(3*i+2) <<   "\n";
//     // cout << "\n\n******************************************************************************\n\n";
//     // /////////////////////////////////////////////////////////////////////////////////////////////////
//     return NodalForces;
// }



//=============================================================================
int TwentySevenNodeBrick_upU_LT::addLoad( ElementalLoad *theLoad, double loadFactor )
{

    int type;
    const Vector &data = theLoad->getData( type, loadFactor );

    if ( type == LOAD_TAG_ElementSelfWeight )
    {
        Vector Fbody = this->getBodyForce( loadFactor, data );
        Q.addVector( 1.0, Fbody, 1.0 );
    }
    // else if ( type == LOAD_TAG_BrickSurfaceLoad )
    // {
    //     Vector Fsurface = this->getSurfaceForce( loadFactor, data );

    //     Q.addVector( 1.0, Fsurface, 1.0);

    // }
    else if (type < 0)
    {
        //Do nothing
    }
    else
    {
        cerr << "TwentySevenNodeBrick_upU_LT::addLoad() - 8NodeBrickLT " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }


    return 0;
}




//=============================================================================
int TwentySevenNodeBrick_upU_LT::addInertiaLoadToUnbalance( const Vector &accel )
{

    static Vector avu(Num_ElemDof);
    int ik;

    for (int i = 0; i < Num_Nodes; i++)
    {
        const Vector &RA = theNodes[i]->getRV(accel);

        if ( RA.Size() != Num_Dof )
        {
            cerr << "TwentyNodeBrick_u_p_U::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
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
const Vector &TwentySevenNodeBrick_upU_LT::getResistingForce()
{

    static Vector avu(Num_ElemDof);

    P.Zero();

    // + K0*upU
    for (int i = 0; i < Num_Nodes; i++)
    {
        const Vector &disp = theNodes[i]->getTrialDisp();

        if ( disp.Size() != Num_Dof )
        {
            cerr << "EightNode_Brick_u_p_U::getResistingForce(): matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (int j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = disp(j);
        }
    }

    P.addMatrixVector(1.0, K, avu, 1.0);

    P.addVector(1.0, Q, -1.0);

    return P;
}

//=============================================================================
const Vector &TwentySevenNodeBrick_upU_LT::getResistingForceIncInertia ()
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
            cerr << "EightNode_Brick_u_p_U::getResistingForceIncInertia matrix and vector sizes are incompatable \n";
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
            cerr << "EightNode_Brick_u_p_U::getResistingForceIncInertia matrix and vector sizes are incompatable \n";
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




// int TwentySevenNodeBrick_upU_LT::sendSelf ( int commitTag, Channel &theChannel )
// {
//     // cout << "TwentySevenNodeBrick_upU_LT::sendSelf() tag = " << this->getTag() << "\n";

//     ID idData( 5 );

//     idData( 0 ) = this->getTag();
//     idData( 1 ) = numDOF;
//     idData( 2 ) = nodes_in_brick;
//     idData( 3 ) = order;
//     idData( 4 ) = material_array[0]->getClassTag();

//     if ( theChannel.sendID( 0, commitTag, idData ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::sendSelf() - " << this->getTag() << " failed to send ID idData\n";
//         return -1;
//     }

//     // send double data
//     Vector floatData(8);
//     floatData(0) = Volume;
//     floatData(1) = e_p;
//     floatData(2) = determinant_of_Jacobian;
//     floatData(3) = rho;
//     floatData(4) = a0;
//     floatData(5) = a1;
//     floatData(6) = a2;
//     floatData(7) = a3;

//     if ( theChannel.sendVector( 0, commitTag, floatData ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
//         return -1;
//     }


//     string tmp_string;

//     tmp_string = stiffness_type;
//     if ( theChannel.sendString( 0, commitTag, tmp_string ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::sendSelf() - " << this->getTag() << " failed to send String stiffness_type\n";
//         return -1;
//     }

//     tmp_string = damping_type;
//     if ( theChannel.sendString( 0, commitTag, damping_type ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::sendSelf() - " << this->getTag() << " failed to send String damping_type\n";
//         return -1;
//     }

//     // Send the nodes

//     if ( theChannel.sendID( 0, commitTag, connectedExternalNodes ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::sendSelf() - " << this->getTag() << " failed to send ID connectedExternalNodes\n";
//         return -1;
//     }


//     // material objects send themselves

//     for ( int i = 0; i < 8; i++ )
//     {
//         if ( material_array[i]->sendSelf( commitTag, theChannel ) < 0 )
//         {
//             cerr << "WARNING TwentySevenNodeBrick_upU_LT::sendSelf() - " << this->getTag() << " failed to send material models\n";
//             return -1;
//         }
//     }


//     //Send Q
//     if ( theChannel.sendVector( 0, commitTag, Q ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::sendSelf() - " << this->getTag() << " failed to send its Q\n";
//         return -1;
//     }

//     //Send bf
//     if ( theChannel.sendVector( 0, commitTag, bf ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::sendSelf() - " << this->getTag() << " failed to send its bf\n";
//         return -1;
//     }

//     return 0;

// }

// int TwentySevenNodeBrick_upU_LT::receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
// {

//     ID idData( 5 );

//     if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::receiveSelf() - failed to receive ID\n";
//         return -1;
//     }

//     this->setTag( idData( 0 ) );
//     numDOF          = idData(1);
//     nodes_in_brick  = idData(2);
//     order           = idData(3);
//     int matClassTag = idData( 4 );

//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() tag = " << this->getTag() << "\n";

//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() numDOF           = " << numDOF << "\n";
//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() nodes_in_brick   = " << nodes_in_brick << "\n";
//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() order            = " << order << "\n";
//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() materialclasstag = " << idData(4) << "\n";


//     Vector floatData(8);
//     if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
//         return -1;
//     }
//     Volume                  = floatData(0) ;
//     e_p                     = floatData(1) ;
//     determinant_of_Jacobian = floatData(2) ;
//     rho                     = floatData(3) ;
//     a0 = floatData(4);
//     a1 = floatData(5);
//     a2 = floatData(6);
//     a3 = floatData(7);


//     string tmp_string;
//     if ( theChannel.receiveString( 0, commitTag,  tmp_string) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::receiveSelf() - " << this->getTag() << " failed to recieve String stiffness_type\n";
//         return -1;
//     }
//     stiffness_type = tmp_string;

//     if ( theChannel.receiveString( 0, commitTag, tmp_string ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::receiveSelf() - " << this->getTag() << " failed to recieve String damping_type\n";
//         return -1;
//     }
//     damping_type = tmp_string;

//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() Volume                  = " << Volume << "\n";
//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() e_p                     = " << e_p << "\n";
//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() determinant_of_Jacobian = " << determinant_of_Jacobian << "\n";
//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() rho                     = " << rho << "\n";


//     // Recieve the nodes

//     if ( theChannel.receiveID( 0, commitTag, connectedExternalNodes ) < 0 )
//     {
//         cerr << "WARNING TwentySevenNodeBrick_upU_LT::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
//         return -1;
//     }

//     // cout << "TwentySevenNodeBrick_upU_LT::receiveSelf() connectedExternalNodes = " << connectedExternalNodes << "\n";

//     for ( int i = 0; i < 8; i++ )
//     {

//         // Allocate new material with the sent class tag
//         NDMaterialLT *ndmat = theBroker.getNewNDMaterialLT( matClassTag );
//         if ( ndmat == 0 )
//         {
//             cerr << "TwentySevenNodeBrick_upU_LT::receiveSelf() - Broker could not create NDMaterialLT of class type " << matClassTag << "\n";
//             return -1;
//         }

//         // Now receive materials into the newly allocated space
//         if ( ( ndmat )->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
//         {
//             cerr << "TwentySevenNodeBrick_upU_LT::receiveSelf() - material " << i << "failed to recv itself\n";
//             return -1;
//         }

//         material_array[i] = ndmat;
//     }


//     // Q
//     if ( theChannel.receiveVector( 0, commitTag, Q ) < 0 )
//     {
//         cerr << "TwentySevenNodeBrick_upU_LT::receiveSelf() - failed to recv Q!\n";
//         return -1;
//     }

//     // bf
//     if ( theChannel.receiveVector( 0, commitTag, bf ) < 0 )
//     {
//         cerr << "TwentySevenNodeBrick_upU_LT::receiveSelf() - failed to recv bf!\n";
//         return -1;
//     }

//     return 0;

// }


// int TwentySevenNodeBrick_upU_LT::getObjectSize()
// {

//     int size = 0;
//     // Just add all sizes of data-members (there is some static local variables too.... but those are not important for transference)
//     //
//     // bool is_mass_computed;
//     size += sizeof(is_mass_computed);
//     // int numDOF;
//     size += sizeof(numDOF);
//     // int nodes_in_brick;
//     size += sizeof(nodes_in_brick);
//     // int order;
//     size += sizeof(order);
//     // double Volume;
//     size += sizeof(Volume);
//     // double e_p;
//     size += sizeof(e_p);
//     // double determinant_of_Jacobian;
//     size += sizeof(determinant_of_Jacobian);
//     // double rho;
//     size += sizeof(rho);
//     // ID  connectedExternalNodes;
//     size += sizeof(int) * connectedExternalNodes.Size();
//     // DTensor2 stress[8];
//     size += sizeof(double) * stress[0].get_size() * 8;
//     // Matrix *Ki;
//     size += sizeof(Ki);
//     // Node *theNodes[8];
//     size += sizeof(theNodes) ;
//     // NDMaterialLT *mmodel;
//     size += sizeof(mmodel);
//     // NDMaterialLT *material_array[8];
//     size += sizeof(material_array) * 8;
//     // Vector Q;
//     size += sizeof(double) * Q.Size();
//     // Vector bf;
//     size += sizeof(double) * bf.Size();
//     // Matrix gauss_points   NOT INCLUDED IN MEMORY COUNT... THEY're Static (J. Abell);
//     // size += sizeof(gauss_points);
//     // // Vector Gauss_Output_Vector;
//     // size += sizeof(Gauss_Output_Vector);
//     // Index < 'i' > i;
//     size += sizeof(i);
//     // Index < 'j' > j;
//     size += sizeof(j);
//     // Index < 'k' > k;
//     size += sizeof(k);
//     // Index < 'l' > l;
//     size += sizeof(l);



//     for ( int i = 0; i < 8; i++ )
//     {
//         size += material_array[i]->getObjectSize();
//     }

//     return size;
// }

//=============================================================================
void TwentySevenNodeBrick_upU_LT::Print( ostream &s, int flag )
{
    cout << "TwentySevenNodeBrick_upU_LT: LTensor-based 8 node brick." << endl << endl;

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


void TwentySevenNodeBrick_upU_LT::ComputeVolume()
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



int TwentySevenNodeBrick_upU_LT::update( void )
{
    // cout << "Updating brick #" << this->getTag() << endl;

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
            cerr << "TwentySevenNodeBrick_upU_LT::update (tag: " << this->getTag()
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



// /////////////////////////////////////////////////////////////////////////
// //NOTE: Can me templatized to improve performance
// double TwentySevenNodeBrick_upU_LT::SurfaceShapeFunctionValues( double Xi , double Eta, int whichcomponent )
// {
//     ShapeFunctionValues_in_function( 0 ) = 0.25 * ( 1 + Xi ) * ( 1 + Eta );
//     ShapeFunctionValues_in_function( 1 ) = 0.25 * ( 1 - Xi ) * ( 1 + Eta );
//     ShapeFunctionValues_in_function( 2 ) = 0.25 * ( 1 - Xi ) * ( 1 - Eta );
//     ShapeFunctionValues_in_function( 3 ) = 0.25 * ( 1 + Xi ) * ( 1 - Eta );

//     return ShapeFunctionValues_in_function( whichcomponent );
// }


// Vector &TwentySevenNodeBrick_upU_LT::Direction_Weight( double Xi , double Eta, Vector coord1, Vector coord2, Vector coord3, Vector coord4 )
// {
//     Vector J1( 3 );
//     Vector J2( 3 );

//     J1( 0 ) = 0.25 * ( ( 1 + Eta ) * coord1( 0 ) - ( 1 + Eta ) * coord2( 0 ) - ( 1 - Eta ) * coord3( 0 ) + ( 1 - Eta ) * coord4( 0 ) );
//     J1( 1 ) = 0.25 * ( ( 1 + Eta ) * coord1( 1 ) - ( 1 + Eta ) * coord2( 1 ) - ( 1 - Eta ) * coord3( 1 ) + ( 1 - Eta ) * coord4( 1 ) );
//     J1( 2 ) = 0.25 * ( ( 1 + Eta ) * coord1( 2 ) - ( 1 + Eta ) * coord2( 2 ) - ( 1 - Eta ) * coord3( 2 ) + ( 1 - Eta ) * coord4( 2 ) );


//     J2( 0 ) = 0.25 * ( ( 1 + Xi ) * coord1( 0 ) + ( 1 - Xi ) * coord2( 0 ) - ( 1 - Xi ) * coord3( 0 ) - ( 1 + Xi ) * coord4( 0 ) );
//     J2( 1 ) = 0.25 * ( ( 1 + Xi ) * coord1( 1 ) + ( 1 - Xi ) * coord2( 1 ) - ( 1 - Xi ) * coord3( 1 ) - ( 1 + Xi ) * coord4( 1 ) );
//     J2( 2 ) = 0.25 * ( ( 1 + Xi ) * coord1( 2 ) + ( 1 - Xi ) * coord2( 2 ) - ( 1 - Xi ) * coord3( 2 ) - ( 1 + Xi ) * coord4( 2 ) );


//     J_vector_in_function( 0 ) = J1( 1 ) * J2( 2 ) - J1( 2 ) * J2( 1 ) ;
//     J_vector_in_function( 1 ) = J1( 2 ) * J2( 0 ) - J1( 0 ) * J2( 2 ) ;
//     J_vector_in_function( 2 ) = J1( 0 ) * J2( 1 ) - J1( 1 ) * J2( 0 ) ;


//     return J_vector_in_function;
// }



// double TwentySevenNodeBrick_upU_LT::SurfaceLoadValues( double Xi , double Eta, Vector Pressure )
// {

//     SurfaceLoadValues_in_function =  0.25 * ( 1 + Xi ) * ( 1 + Eta ) * Pressure( 0 )
//                                      + 0.25 * ( 1 - Xi ) * ( 1 + Eta ) * Pressure( 1 )
//                                      + 0.25 * ( 1 - Xi ) * ( 1 - Eta ) * Pressure( 2 )
//                                      + 0.25 * ( 1 + Xi ) * ( 1 - Eta ) * Pressure( 3 );



//     return SurfaceLoadValues_in_function;

// }


int
TwentySevenNodeBrick_upU_LT::CheckMesh( ofstream &checkmesh_file )
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
TwentySevenNodeBrick_upU_LT::getStress( void )
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

Matrix &TwentySevenNodeBrick_upU_LT::getGaussCoordinates(void)
{
    computeGaussPoint();
    return gauss_points;
}

/**********************************************************************************
* Sumeet August, 2016. See classTags.h for class description encoding 
* Returns the output at gauss points.
* NOTE!!! = For each gauss point there should be exactly 18 outputs 
*           6 Total_Strain, 6 Plastic_Strain and 6 Stress
*           Must be consistent with class description esedncoding.
*           Fix the class_desc accordingly based on the encoding formula
***********************************************************************************/
const vector<float> &TwentySevenNodeBrick_upU_LT::getGaussOutput()
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

void TwentySevenNodeBrick_upU_LT::zeroStrain()
{
    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
    {
        material_array[gp]->zeroStrain();
    }

    return ;
}


#endif

