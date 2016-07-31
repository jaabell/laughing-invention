///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Zhaohui Yang  and Xiaoyan Wu
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


#ifndef TWENTYNODEBRICKLTLT_CPP
#define TWENTYNODEBRICKLTLT_CPP

#include "TwentyNodeBrickLT.h"
#include <LTensorDisplay.h>
#include <HDF5_Channel.h>


double TwentyNodeBrickLT::SurfaceLoadValues_in_function; // Nima added for surface load (July 2012)

DTensor2 TwentyNodeBrickLT::gp_coords(27, 3, 0.0);
DTensor2 TwentyNodeBrickLT::gp_weight(27, 3, 0.0);
Matrix TwentyNodeBrickLT::gauss_points(27, 3);
Vector TwentyNodeBrickLT::outputVector(TwentyNodeBrickLT_OUTPUT_SIZE);

Vector TwentyNodeBrickLT::ShapeFunctionValues_in_function(8); // Nima added for surface load (July 2012)
Vector TwentyNodeBrickLT::J_vector_in_function(3); // Nima added for surface load (July 2012)


Matrix TwentyNodeBrickLT::K(60, 60);



//====================================================================
// Constructor
//====================================================================

TwentyNodeBrickLT::TwentyNodeBrickLT(int element_number,
                                 int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
                                 int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
                                 int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
                                 int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
                                 int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
                                 NDMaterialLT *Globalmmodel)

    : Element(element_number, ELE_TAG_TwentyNodeBrickLT ),
      rho(0.0), connectedExternalNodes(20),
      Ki(0), Q(60), bf(3),
      M( 60, 60), P( 60 )
{
    this->setMaterialTag(Globalmmodel->getTag());

    rho = Globalmmodel->getRho();

    determinant_of_Jacobian = 0.0;
    mmodel = Globalmmodel;


    // Generate 27 NDMaterialLT for use at each Gauss point.
    for (int k = 0; k < 27; k++)
    {
        material_array[k] = mmodel->getCopy();
    }


    // 20NodeBrick also has 27 Gauss Points, like 27NodeBrick. 
    short where = 0;
    for (short ii = 0 ; ii < 3 ; ++ii ){
        for (short jj = 0 ; jj < 3 ; ++jj ){
            for (short kk = 0 ; kk < 3 ; ++kk ){
                std::vector<short> axis={ii,jj,kk};
                for(short rst=0;rst<3;++rst){
                    switch(axis[rst]){
                        case 0:{
                            gp_coords(where, rst) = -0.774596669241483;
                            gp_weight(where, rst) = 0.555555555555556;
                            break;
                        }
                        case 1:{
                            gp_coords(where, rst) = 0.0;
                            gp_weight(where, rst) = 0.888888888888889;
                            break;
                        }
                        case 2:{
                            gp_coords(where, rst) = 0.774596669241483;
                            gp_weight(where, rst) = 0.555555555555556;
                            break;
                        }
                    }
                }
                where ++;
            }
        }
    }

    is_mass_computed = false;

    nodes_in_brick = 20;

    // Set connected external node IDs
    connectedExternalNodes( 0) = node_numb_1;
    connectedExternalNodes( 1) = node_numb_2;
    connectedExternalNodes( 2) = node_numb_3;
    connectedExternalNodes( 3) = node_numb_4;
    connectedExternalNodes( 4) = node_numb_5;
    connectedExternalNodes( 5) = node_numb_6;
    connectedExternalNodes( 6) = node_numb_7;
    connectedExternalNodes( 7) = node_numb_8;
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

    for (int i = 0; i < 20; i++)
    {
        theNodes[i] = 0;
    }

    this->setNumberOfBoundaryNodes(8);

}




//====================================================================
TwentyNodeBrickLT::TwentyNodeBrickLT (): Element(0, ELE_TAG_TwentyNodeBrickLT ),
    rho(0.0), connectedExternalNodes(20), 
    Ki(0), mmodel(0), Q(60),
    bf(3), M( 60, 60), P( 60 )
{
    is_mass_computed = false;
    nodes_in_brick = 20;

    for (int i = 0; i < 20; i++){
        theNodes[i] = 0;
    }
    this->setNumberOfBoundaryNodes(8);

        // 20NodeBrick also has 27 Gauss Points, like 27NodeBrick. 
    short where = 0;
    for (short ii = 0 ; ii < 3 ; ++ii ){
        for (short jj = 0 ; jj < 3 ; ++jj ){
            for (short kk = 0 ; kk < 3 ; ++kk ){
                std::vector<short> axis={ii,jj,kk};
                for(short rst=0;rst<3;++rst){
                    switch(axis[rst]){
                        case 0:{
                            gp_coords(where, rst) = -0.774596669241483;
                            gp_weight(where, rst) = 0.555555555555556;
                            break;
                        }
                        case 1:{
                            gp_coords(where, rst) = 0.0;
                            gp_weight(where, rst) = 0.888888888888889;
                            break;
                        }
                        case 2:{
                            gp_coords(where, rst) = 0.774596669241483;
                            gp_weight(where, rst) = 0.555555555555556;
                            break;
                        }
                    }
                }
                where ++;
            }
        }
    }
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TwentyNodeBrickLT::~TwentyNodeBrickLT ()
{

    for (int i = 0; i < 27; i++)
    {
        // Delete the NDMaterials at each integration point
        if (material_array[i])
        {
            delete material_array[i];
        }
    }



    if (Ki != 0)
    {
        delete Ki;
    }

}













//#############################################################################
//#############################################################################
//***************************************************************
const DTensor2 &TwentyNodeBrickLT::H_3D(double r1, double r2, double r3) const
{

    static DTensor2 H( 60, 3, 0.0);


    // influence of the node number 20
    H( 57, 0) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) * 0.25;
    H( 58, 1) = H( 57, 0); //(1.0+r1)*(1.0-r2)*(1.0-r3*r3)*0.25;
    H( 59, 2) = H( 57, 0); //(1.0+r1)*(1.0-r2)*(1.0-r3*r3)*0.25;
    // influence of the node number 19
    H( 54, 0) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) * 0.25;
    H( 55, 1) = H( 54, 0); //(1.0-r1)*(1.0-r2)*(1.0-r3*r3)*0.25;
    H( 56, 2) = H( 54, 0); //(1.0-r1)*(1.0-r2)*(1.0-r3*r3)*0.25;
    // influence of the node number 18
    H( 51, 0) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) * 0.25;
    H( 52, 1) = H( 51, 0); //(1.0-r1)*(1.0+r2)*(1.0-r3*r3)*0.25;
    H( 53, 2) = H( 51, 0); //(1.0-r1)*(1.0+r2)*(1.0-r3*r3)*0.25;
    // influence of the node number 17
    H( 48, 0) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) * 0.25;
    H( 49, 1) = H( 48, 0); //(1.0+r1)*(1.0+r2)*(1.0-r3*r3)*0.25;
    H( 50, 2) = H( 48, 0); //(1.0+r1)*(1.0+r2)*(1.0-r3*r3)*0.25;

    // influence of the node number 16
    H( 45, 0) = (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) * 0.25;
    H( 46, 1) = H( 45, 0); //(1.0+r1)*(1.0-r2*r2)*(1.0-r3)*0.25;
    H( 47, 2) = H( 45, 0); //(1.0+r1)*(1.0-r2*r2)*(1.0-r3)*0.25;
    // influence of the node number 15
    H( 42, 0) = (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) * 0.25;
    H( 43, 1) = H( 42, 0); //(1.0-r1*r1)*(1.0-r2)*(1.0-r3)*0.25;
    H( 44, 2) = H( 42, 0); //(1.0-r1*r1)*(1.0-r2)*(1.0-r3)*0.25;
    // influence of the node number 14
    H( 39, 0) = (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) * 0.25;
    H( 40, 1) = H( 39, 0); //(1.0-r1)*(1.0-r2*r2)*(1.0-r3)*0.25;
    H( 41, 2) = H( 39, 0); //(1.0-r1)*(1.0-r2*r2)*(1.0-r3)*0.25;
    // influence of the node number 13
    H( 36, 0) = (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) * 0.25;
    H( 37, 1) = H( 36, 0); //(1.0-r1*r1)*(1.0+r2)*(1.0-r3)*0.25;
    H( 38, 2) = H( 36, 0); //(1.0-r1*r1)*(1.0+r2)*(1.0-r3)*0.25;

    // influence of the node number 12
    H( 33, 0) = (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) * 0.25;
    H( 34, 1) = H( 33, 0); //(1.0+r1)*(1.0-r2*r2)*(1.0+r3)*0.25;
    H( 35, 2) = H( 33, 0); //(1.0+r1)*(1.0-r2*r2)*(1.0+r3)*0.25;
    // influence of the node number 11
    H( 30, 0) = (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) * 0.25;
    H( 31, 1) = H( 30, 0); //(1.0-r1*r1)*(1.0-r2)*(1.0+r3)*0.25;
    H( 32, 2) = H( 30, 0); //(1.0-r1*r1)*(1.0-r2)*(1.0+r3)*0.25;
    // influence of the node number 10
    H( 27, 0) = (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) * 0.25;
    H( 28, 1) = H( 27, 0); //(1.0-r1)*(1.0-r2*r2)*(1.0+r3)*0.25;
    H( 29, 2) = H( 27, 0); //(1.0-r1)*(1.0-r2*r2)*(1.0+r3)*0.25;
    // influence of the node number 9
    H( 24, 0) = (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) * 0.25;
    H( 25, 1) = H( 24, 0); //(1.0-r1*r1)*(1.0+r2)*(1.0+r3)*0.25;
    H( 26, 2) = H( 24, 0); //(1.0-r1*r1)*(1.0+r2)*(1.0+r3)*0.25;


    // 9-20 nodes

    // influence of the node number 8
    H( 21, 0) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) * 0.125 - (H( 42, 0) + H( 47, 2) + H( 59, 2)) * 0.5;
    H( 22, 1) = H( 21, 0); //(1.0+r1)*(1.0-r2)*(1.0-r3)/8.0 - (H( 42,0)+H( 47,2)+H( 59,2))/2.0;
    H( 23, 2) = H( 21, 0); //(1.0+r1)*(1.0-r2)*(1.0-r3)/8.0 - (H( 42,0)+H( 47,2)+H( 59,2))/2.0;
    // influence of the node number 7
    H( 18, 0) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) * 0.125 - (H( 41, 2) + H( 42, 0) + H( 56, 2)) * 0.5;
    H( 19, 1) = H( 18, 0); //(1.0-r1)*(1.0-r2)*(1.0-r3)/8.0 - (H( 41,2)+H( 42,0)+H( 56,2))/2.0;
    H( 20, 2) = H( 18, 0); //(1.0-r1)*(1.0-r2)*(1.0-r3)/8.0 - (H( 41,2)+H( 42,0)+H( 56,2))/2.0;
    // influence of the node number 6
    H( 15, 0) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) * 0.125 - (H( 38, 2) + H( 41, 2) + H( 53, 2)) * 0.5;
    H( 16, 1) = H( 15, 0); //(1.0-r1)*(1.0+r2)*(1.0-r3)/8.0 - (H( 38,2)+H( 41,2)+H( 53,2))/2.0;
    H( 17, 2) = H( 15, 0); //(1.0-r1)*(1.0+r2)*(1.0-r3)/8.0 - (H( 38,2)+H( 41,2)+H( 53,2))/2.0;
    // influence of the node number 5
    H( 12, 0) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) * 0.125 - (H( 38, 2) + H( 47, 2) + H( 50, 2)) * 0.5;
    H( 13, 1) = H( 12, 0); //(1.0+r1)*(1.0+r2)*(1.0-r3)/8.0 - (H( 38,2)+H( 47,2)+H( 50,2))/2.0;
    H( 14, 2) = H( 12, 0); //(1.0+r1)*(1.0+r2)*(1.0-r3)/8.0 - (H( 38,2)+H( 47,2)+H( 50,2))/2.0;

    // influence of the node number 4
    H( 9, 0) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) * 0.125 - (H( 32, 2) + H( 35, 2) + H( 59, 2)) * 0.5;
    H( 10, 1) = H( 9, 0); //(1.0+r1)*(1.0-r2)*(1.0+r3)/8.0 - (H( 32,2)+H( 35,2)+H( 59,2))/2.0;
    H( 11, 2) = H( 9, 0); //(1.0+r1)*(1.0-r2)*(1.0+r3)/8.0 - (H( 32,2)+H( 35,2)+H( 59,2))/2.0;
    // influence of the node number 3
    H( 6, 0) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) * 0.125 - (H( 29, 2) + H( 32, 2) + H( 56, 2)) * 0.5;
    H( 7, 1) = H( 6, 0); //(1.0-r1)*(1.0-r2)*(1.0+r3)/8.0 - (H( 29,2)+H( 32,2)+H( 56,2))/2.0;
    H( 8, 2) = H( 6, 0); //(1.0-r1)*(1.0-r2)*(1.0+r3)/8.0 - (H( 29,2)+H( 32,2)+H( 56,2))/2.0;
    // influence of the node number 2
    H( 3, 0) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) * 0.125 - (H( 29, 2) + H( 53, 2) + H( 26, 2)) * 0.5;
    H( 4, 1) = H( 3, 0); //(1.0-r1)*(1.0+r2)*(1.0+r3)/8.0 - (H( 29,2)+H( 53,2)+H( 26,2))/2.0;
    H( 5, 2) = H( 3, 0); //(1.0-r1)*(1.0+r2)*(1.0+r3)/8.0 - (H( 29,2)+H( 53,2)+H( 26,2))/2.0;
    // influence of the node number 1
    H( 0, 0) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) * 0.125 - (H( 35, 2) + H( 50, 2) + H( 26, 2)) * 0.5;
    H( 1, 1) = H( 0, 0); //(1.0+r1)*(1.0+r2)*(1.0+r3)/8.0 - (H( 35,2)+H( 50,2)+H( 26,2))/2.0;
    H( 2, 2) = H( 0, 0); //(1.0+r1)*(1.0+r2)*(1.0+r3)/8.0 - (H( 35,2)+H( 50,2)+H( 26,2))/2.0;



    return H;
}






//#############################################################################
//***************************************************************
const DTensor1 &TwentyNodeBrickLT::interp_poli_at(double r1, double r2, double r3) const
{

    static DTensor1 h( 20, 0.0);

    // influence of the node number 20
    //    h( 19 )=(1.0+r1)*(1.0-r2)*(1.0-r3*r3)/4.0;
    h( 19 ) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) * 0.25;
    // influence of the node number 19
    h( 18 ) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) * 0.25;
    // influence of the node number 18
    h( 17 ) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) * 0.25;
    // influence of the node number 17
    h( 16 ) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) * 0.25;

    // influence of the node number 16
    h( 15 ) = (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) * 0.25;
    // influence of the node number 15
    h( 14 ) = (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) * 0.25;
    // influence of the node number 14
    h( 13 ) = (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) * 0.25;
    // influence of the node number 13
    h( 12 ) = (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) * 0.25;

    // influence of the node number 12
    h( 11 ) = (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) * 0.25;
    // influence of the node number 11
    h( 10 ) = (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) * 0.25;
    // influence of the node number 10
    h( 9 ) = (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) * 0.25;
    // influence of the node number 9
    h( 8 ) = (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) * 0.25;

    // influence of the node number 8
    //h( 7 )=(1.0+r1)*(1.0-r2)*(1.0-r3)/8.0 - (h( 14 )+h( 15 )+h( 19 ))/2.0;
    h( 7 ) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) * 0.125 - (h( 14 ) + h( 15 ) + h( 19 )) * 0.5;
    // influence of the node number 7
    h( 6 ) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) * 0.125 - (h( 13 ) + h( 14 ) + h( 18 )) * 0.5;
    // influence of the node number 6
    h( 5 ) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) * 0.125 - (h( 12 ) + h( 13 ) + h( 17 )) * 0.5;
    // influence of the node number 5
    h( 4 ) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) * 0.125 - (h( 12 ) + h( 15 ) + h( 16 )) * 0.5;

    // influence of the node number 4
    h( 3 ) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) * 0.125 - (h( 10 ) + h( 11 ) + h( 19 )) * 0.5;
    // influence of the node number 3
    h( 2 ) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) * 0.125 - (h( 9 ) + h( 10 ) + h( 18 )) * 0.5;
    // influence of the node number 2
    h( 1 ) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) * 0.125 - (h( 9 ) + h( 17 ) + h( 8 )) * 0.5;
    // influence of the node number 1
    h( 0 ) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) * 0.125 - (h( 11 ) + h( 16 ) + h( 8 )) * 0.5;
    //    printf("r1 = %lf, r2 = %lf, r3 = %lf\n", r1, r2, r3);
    //    h.print("h");

    return h;
}










const DTensor2 &TwentyNodeBrickLT::dh_drst_at(double r1, double r2, double r3) const
{

    static DTensor2 dh( 20, 3, 0.0);


    // influence of the node number 20
    dh( 19, 0 ) =   (1.0 - r2) * (1.0 - r3 * r3) * 0.25; ///4.0;
    dh( 19, 1 ) = - (1.0 + r1) * (1.0 - r3 * r3) * 0.25; ///4.0;
    dh( 19, 2 ) = - (1.0 + r1) * (1.0 - r2) * r3 * 0.50; ///2.0;
    // influence of the node number 19
    dh( 18, 0 ) = - (1.0 - r2) * (1.0 - r3 * r3) * 0.25; ///4.0;
    dh( 18, 1 ) = - (1.0 - r1) * (1.0 - r3 * r3) * 0.25; ///4.0;
    dh( 18, 2 ) = - (1.0 - r1) * (1.0 - r2) * r3 * 0.50; ///2.0;
    // influence of the node number 18
    dh( 17, 0 ) = - (1.0 + r2) * (1.0 - r3 * r3) * 0.25; ///4.0;
    dh( 17, 1 ) =   (1.0 - r1) * (1.0 - r3 * r3) * 0.25; ///4.0;
    dh( 17, 2 ) = - (1.0 - r1) * (1.0 + r2) * r3 * 0.50; ///2.0;
    // influence of the node number 17
    dh( 16, 0 ) =   (1.0 + r2) * (1.0 - r3 * r3) * 0.25; ///4.0;
    dh( 16, 1 ) =   (1.0 + r1) * (1.0 - r3 * r3) * 0.25; ///4.0;
    dh( 16, 2 ) = - (1.0 + r1) * (1.0 + r2) * r3 * 0.50; ///2.0;

    // influence of the node number 16
    dh( 15, 0 ) =   (1.0 - r2 * r2) * (1.0 - r3) * 0.25; ///4.0;
    dh( 15, 1 ) = - (1.0 + r1) * r2 * (1.0 - r3) * 0.50; ///2.0;
    dh( 15, 2 ) = - (1.0 + r1) * (1.0 - r2 * r2) * 0.25; ///4.0;
    // influnce of the node number 15
    dh( 14, 0 ) = - r1 * (1.0 - r2) * (1.0 - r3) * 0.50; ///2.0;
    dh( 14, 1 ) = - (1.0 - r1 * r1) * (1.0 - r3) * 0.25; ///4.0;
    dh( 14, 2 ) = - (1.0 - r1 * r1) * (1.0 - r2) * 0.25; ///4.0;
    // influence of the node number 14
    dh( 13, 0 ) = - (1.0 - r2 * r2) * (1.0 - r3) * 0.25; ///4.0;
    dh( 13, 1 ) = - (1.0 - r1) * r2 * (1.0 - r3) * 0.50; ///2.0;
    dh( 13, 2 ) = - (1.0 - r1) * (1.0 - r2 * r2) * 0.25; ///4.0;
    // influence of the node number 13
    dh( 12, 0 ) = - r1 * (1.0 + r2) * (1.0 - r3) * 0.50; ///2.0;
    dh( 12, 1 ) =   (1.0 - r1 * r1) * (1.0 - r3) * 0.25; ///4.0;
    dh( 12, 2 ) = - (1.0 - r1 * r1) * (1.0 + r2) * 0.25; ///4.0;

    // influence of the node number 12
    dh( 11, 0 ) =   (1.0 - r2 * r2) * (1.0 + r3) * 0.25; ///4.0;
    dh( 11, 1 ) = - (1.0 + r1) * r2 * (1.0 + r3) * 0.50; ///2.0;
    dh( 11, 2 ) =   (1.0 + r1) * (1.0 - r2 * r2) * 0.25; ///4.0;
    // influence of the node number 11
    dh( 10, 0 ) = - r1 * (1.0 - r2) * (1.0 + r3) * 0.50; ///2.0;
    dh( 10, 1 ) = - (1.0 - r1 * r1) * (1.0 + r3) * 0.25; ///4.0; // bug discovered 01 aug '95 2.0 -> 4.0
    dh( 10, 2 ) =   (1.0 - r1 * r1) * (1.0 - r2) * 0.25; ///4.0;
    // influence of the node number 10
    dh( 9, 0 ) = - (1.0 - r2 * r2) * (1.0 + r3) * 0.25; ///4.0;
    dh( 9, 1 ) = - (1.0 - r1) * r2 * (1.0 + r3) * 0.50; ///2.0;
    dh( 9, 2 ) =   (1.0 - r1) * (1.0 - r2 * r2) * 0.25; ///4.0;
    // influence of the node number 9
    dh( 8, 0 )  = - r1 * (1.0 + r2) * (1.0 + r3) * 0.50; ///2.0;
    dh( 8, 1 )  =   (1.0 - r1 * r1) * (1.0 + r3) * 0.25; ///4.0;
    dh( 8, 2 )  =   (1.0 - r1 * r1) * (1.0 + r2) * 0.25; ///4.0;

    // influence of the node number 8
    //dh( 7,0 )= (1.0-r2)*(1.0-r3)/8.0 - (dh( 14,0 )+dh( 15,0 )+dh( 19,0 ))/2.0;
    dh( 7, 0 ) = (1.0 - r2) * (1.0 - r3) * 0.125 - (dh( 14, 0 ) + dh( 15, 0 ) + dh( 19, 0 )) * 0.50; ///2.0;
    dh( 7, 1 ) = -(1.0 + r1) * (1.0 - r3) * 0.125 - (dh( 14, 1 ) + dh( 15, 1 ) + dh( 19, 1 )) * 0.50; ///2.0;
    dh( 7, 2 ) = -(1.0 + r1) * (1.0 - r2) * 0.125 - (dh( 14, 2 ) + dh( 15, 2 ) + dh( 19, 2 )) * 0.50; ///2.0;
    // influence of the node number 7
    dh( 6, 0 ) = -(1.0 - r2) * (1.0 - r3) * 0.125 - (dh( 13, 0 ) + dh( 14, 0 ) + dh( 18, 0 )) * 0.50; ///2.0;
    dh( 6, 1 ) = -(1.0 - r1) * (1.0 - r3) * 0.125 - (dh( 13, 1 ) + dh( 14, 1 ) + dh( 18, 1 )) * 0.50; ///2.0;
    dh( 6, 2 ) = -(1.0 - r1) * (1.0 - r2) * 0.125 - (dh( 13, 2 ) + dh( 14, 2 ) + dh( 18, 2 )) * 0.50; ///2.0;
    // influence of the node number 6
    dh( 5, 0 ) = -(1.0 + r2) * (1.0 - r3) * 0.125 - (dh( 12, 0 ) + dh( 13, 0 ) + dh( 17, 0 )) * 0.50; ///2.0;
    dh( 5, 1 ) = (1.0 - r1) * (1.0 - r3) * 0.125 - (dh( 12, 1 ) + dh( 13, 1 ) + dh( 17, 1 )) * 0.50; ///2.0;
    dh( 5, 2 ) = -(1.0 - r1) * (1.0 + r2) * 0.125 - (dh( 12, 2 ) + dh( 13, 2 ) + dh( 17, 2 )) * 0.50; ///2.0;
    // influence of the node number 5
    dh( 4, 0 ) = (1.0 + r2) * (1.0 - r3) * 0.125 - (dh( 12, 0 ) + dh( 15, 0 ) + dh( 16, 0 )) * 0.50; ///2.0;
    dh( 4, 1 ) = (1.0 + r1) * (1.0 - r3) * 0.125 - (dh( 12, 1 ) + dh( 15, 1 ) + dh( 16, 1 )) * 0.50; ///2.0;
    dh( 4, 2 ) = -(1.0 + r1) * (1.0 + r2) * 0.125 - (dh( 12, 2 ) + dh( 15, 2 ) + dh( 16, 2 )) * 0.50; ///2.0;

    // influence of the node number 4
    dh( 3, 0 ) = (1.0 - r2) * (1.0 + r3) * 0.125 - (dh( 10, 0 ) + dh( 11, 0 ) + dh( 19, 0 )) * 0.50; ///2.0;
    dh( 3, 1 ) = -(1.0 + r1) * (1.0 + r3) * 0.125 - (dh( 10, 1 ) + dh( 11, 1 ) + dh( 19, 1 )) * 0.50; ///2.0;
    dh( 3, 2 ) = (1.0 + r1) * (1.0 - r2) * 0.125 - (dh( 10, 2 ) + dh( 11, 2 ) + dh( 19, 2 )) * 0.50; ///2.0;
    // influence of the node number 3
    dh( 2, 0 ) = -(1.0 - r2) * (1.0 + r3) * 0.125 - (dh( 9, 0 ) + dh( 10, 0 ) + dh( 18, 0 )) * 0.50; ///2.0;
    dh( 2, 1 ) = -(1.0 - r1) * (1.0 + r3) * 0.125 - (dh( 9, 1 ) + dh( 10, 1 ) + dh( 18, 1 )) * 0.50; ///2.0;
    dh( 2, 2 ) = (1.0 - r1) * (1.0 - r2) * 0.125 - (dh( 9, 2 ) + dh( 10, 2 ) + dh( 18, 2 )) * 0.50; ///2.0;
    // influence of the node number 2
    dh( 1, 0 ) = -(1.0 + r2) * (1.0 + r3) * 0.125 - (dh( 9, 0 ) + dh( 17, 0 ) + dh(  8, 0 )) * 0.50; ///2.0;
    dh( 1, 1 ) = (1.0 - r1) * (1.0 + r3) * 0.125 - (dh( 9, 1 ) + dh( 17, 1 ) + dh(  8, 1 )) * 0.50; ///2.0;
    dh( 1, 2 ) = (1.0 - r1) * (1.0 + r2) * 0.125 - (dh( 9, 2 ) + dh( 17, 2 ) + dh(  8, 2 )) * 0.50; ///2.0;
    // influence of the node number 1
    dh( 0, 0 ) = (1.0 + r2) * (1.0 + r3) * 0.125 - (dh( 11, 0 ) + dh( 16, 0 ) + dh(  8, 0 )) * 0.50; ///2.0;
    dh( 0, 1 ) = (1.0 + r1) * (1.0 + r3) * 0.125 - (dh( 11, 1 ) + dh( 16, 1 ) + dh(  8, 1 )) * 0.50; ///2.0;
    dh( 0, 2 ) = (1.0 + r1) * (1.0 + r2) * 0.125 - (dh( 11, 2 ) + dh( 16, 2 ) + dh(  8, 2 )) * 0.50; ///2.0;

    return dh;
}










////#############################################################################
const DTensor2 &TwentyNodeBrickLT::Jacobian_3D( const DTensor2 &dh) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    static DTensor2 Jacobian_3D_( 3, 3, 0.0);
    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    return Jacobian_3D_;
}





//#############################################################################
const DTensor2 &TwentyNodeBrickLT::Jacobian_3Dinv(const DTensor2 &dh) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    DTensor2 Jacobian_3D_(3, 3, 0.0);
    static DTensor2 Jacobian_3D_inv(3, 3, 0.0);

    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    Jacobian_3D_inv = Jacobian_3D_.Inv();
    return Jacobian_3D_inv;
}









////#############################################################################
const DTensor2 &TwentyNodeBrickLT::Nodal_Coordinates(void) const
{
    static DTensor2 N_coord( 20, 3, 0.0 );
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            N_coord( i, j ) = (theNodes[i]->getCrds())(j);
        }
    }

    return N_coord;

}

////#############################################################################
const DTensor2 &TwentyNodeBrickLT::incr_disp(void) const
{
    static DTensor2 increment_disp( 20, 3, 0.0 );

    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            increment_disp( i, j ) = (theNodes[i]->getIncrDeltaDisp())(j);
        }
    }


    return increment_disp;
}




////#############################################################################
const DTensor2 &TwentyNodeBrickLT::total_disp(void) const
{
    static DTensor2 total_disp( 20, 3, 0.0 );
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            total_disp( i, j ) = (theNodes[i]->getTrialDisp())(j);
        }
    }

    return total_disp;
}








////#############################################################################
int TwentyNodeBrickLT::get_global_number_of_node(int local_node_number)
{
    return connectedExternalNodes(local_node_number);
}

////#############################################################################
int  TwentyNodeBrickLT::get_Brick_Number(void)
{
    return this->getTag();
}



////#############################################################################
// returns nodal forces for given stress field in an element
const DTensor2 &TwentyNodeBrickLT::nodal_forces(void) const
{
    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    static DTensor2 nodal_forces( 20, 3, 0.0 );
    DTensor2 dh( 20, 3 , 0.0 );
    DTensor2 stress_at_GP( 3, 3, 0.0 );
    DTensor2 Jacobian( 3, 3, 0.0 );
    DTensor2 JacobianINV( 3, 3, 0.0 );
    DTensor2 dhGlobal( 20, 3 , 0.0 );

    //Set nodal_forces to zero (it is only done once since it's a static varible)
    for (DTensor2::literator it = nodal_forces.begin(); it != nodal_forces.end(); it++)
    {
        *it = 0;
    }


    for ( short gp = 0; gp < 27; gp++ )
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








//#############################################################################
//Compute Gauss Point coordinates and store it in global Gsc
void TwentyNodeBrickLT::computeGaussPoint(void)
{

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    DTensor2 NodalCoord( 3, 20, 0.0 );
    DTensor2 material_arrayCoord( 3, 27, 0.0 );
    DTensor2 H( 60, 3, 0.0 );


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

    NodalCoord( 0, 0) = nd1Crds( 0);
    NodalCoord( 1, 0) = nd1Crds( 1);
    NodalCoord( 2, 0) = nd1Crds( 2);
    NodalCoord( 0, 1) = nd2Crds( 0);
    NodalCoord( 1, 1) = nd2Crds( 1);
    NodalCoord( 2, 1) = nd2Crds( 2);
    NodalCoord( 0, 2) = nd3Crds( 0);
    NodalCoord( 1, 2) = nd3Crds( 1);
    NodalCoord( 2, 2) = nd3Crds( 2);
    NodalCoord( 0, 3) = nd4Crds( 0);
    NodalCoord( 1, 3) = nd4Crds( 1);
    NodalCoord( 2, 3) = nd4Crds( 2);
    NodalCoord( 0, 4) = nd5Crds( 0);
    NodalCoord( 1, 4) = nd5Crds( 1);
    NodalCoord( 2, 4) = nd5Crds( 2);
    NodalCoord( 0, 5) = nd6Crds( 0);
    NodalCoord( 1, 5) = nd6Crds( 1);
    NodalCoord( 2, 5) = nd6Crds( 2);
    NodalCoord( 0, 6) = nd7Crds( 0);
    NodalCoord( 1, 6) = nd7Crds( 1);
    NodalCoord( 2, 6) = nd7Crds( 2);
    NodalCoord( 0, 7) = nd8Crds( 0);
    NodalCoord( 1, 7) = nd8Crds( 1);
    NodalCoord( 2, 7) = nd8Crds( 2);
    NodalCoord( 0, 8) = nd9Crds( 0);
    NodalCoord( 1, 8) = nd9Crds( 1);
    NodalCoord( 2, 8) = nd9Crds( 2);
    NodalCoord( 0, 9) = nd10Crds(0);
    NodalCoord( 1, 9) = nd10Crds(1);
    NodalCoord( 2, 9) = nd10Crds(2);
    NodalCoord( 0, 10) = nd11Crds(0);
    NodalCoord( 1, 10) = nd11Crds(1);
    NodalCoord( 2, 10) = nd11Crds(2);
    NodalCoord( 0, 11) = nd12Crds(0);
    NodalCoord( 1, 11) = nd12Crds(1);
    NodalCoord( 2, 11) = nd12Crds(2);
    NodalCoord( 0, 12) = nd13Crds(0);
    NodalCoord( 1, 12) = nd13Crds(1);
    NodalCoord( 2, 12) = nd13Crds(2);
    NodalCoord( 0, 13) = nd14Crds(0);
    NodalCoord( 1, 13) = nd14Crds(1);
    NodalCoord( 2, 13) = nd14Crds(2);
    NodalCoord( 0, 14) = nd15Crds(0);
    NodalCoord( 1, 14) = nd15Crds(1);
    NodalCoord( 2, 14) = nd15Crds(2);
    NodalCoord( 0, 15) = nd16Crds(0);
    NodalCoord( 1, 15) = nd16Crds(1);
    NodalCoord( 2, 15) = nd16Crds(2);
    NodalCoord( 0, 16) = nd17Crds(0);
    NodalCoord( 1, 16) = nd17Crds(1);
    NodalCoord( 2, 16) = nd17Crds(2);
    NodalCoord( 0, 17) = nd18Crds(0);
    NodalCoord( 1, 17) = nd18Crds(1);
    NodalCoord( 2, 17) = nd18Crds(2);
    NodalCoord( 0, 18) = nd19Crds(0);
    NodalCoord( 1, 18) = nd19Crds(1);
    NodalCoord( 2, 18) = nd19Crds(2);
    NodalCoord( 0, 19) = nd20Crds(0);
    NodalCoord( 1, 19) = nd20Crds(1);
    NodalCoord( 2, 19) = nd20Crds(2);

    for ( short gp = 0; gp < 27; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);

        // derivatives of local coordinates with respect to local coordinates
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
    //return Gsc;

}






//=============================================================================
int TwentyNodeBrickLT::getNumExternalNodes () const
{
    return 20; 
}


//=============================================================================
const ID &TwentyNodeBrickLT::getExternalNodes ()
{
    return connectedExternalNodes;
}

Node **
TwentyNodeBrickLT::getNodePtrs(void)
{
    return theNodes;
}

//=============================================================================
int TwentyNodeBrickLT::getNumDOF ()
{
    return 3 * nodes_in_brick; 
}

//=============================================================================
void TwentyNodeBrickLT::setDomain (Domain *theDomain)
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
        theNodes[8] = 0;
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
    }

    else
    {

        for (int i = 0; i < 20; ++i)
        {
            theNodes[i] = theDomain->getNode( connectedExternalNodes(i) );

# ifdef _PARALLEL_PROCESSING
            if ( theNodes[i] == 0 ){
                theNodes[i] = theDomain->getOutsideNode( connectedExternalNodes(i) );
            }
# endif
        }



        if (theNodes[0]  == 0 || theNodes[1]  == 0 || theNodes[2]  == 0 || theNodes[3]  == 0 ||
                theNodes[4]  == 0 || theNodes[5]  == 0 || theNodes[6]  == 0 || theNodes[7]  == 0 ||
                theNodes[8]  == 0 || theNodes[9] == 0 || theNodes[10] == 0 || theNodes[11] == 0 ||
                theNodes[12] == 0 || theNodes[13] == 0 || theNodes[14] == 0 || theNodes[15] == 0 ||
                theNodes[16] == 0 || theNodes[17] == 0 || theNodes[18] == 0 || theNodes[19] == 0 )
        {

            cerr << "FATAL ERROR TwentyNodeBrickLT (tag: " << this->getTag() << " ), node not found in domain\n";
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

        if (dofNd1  != 3 || dofNd2  != 3 || dofNd3  != 3 || dofNd4  != 3 ||
                dofNd5  != 3 || dofNd6  != 3 || dofNd7  != 3 || dofNd8  != 3 ||
                dofNd9  != 3 || dofNd10 != 3 || dofNd11 != 3 || dofNd12 != 3 ||
                dofNd13 != 3 || dofNd14 != 3 || dofNd15 != 3 || dofNd16 != 3 ||
                dofNd17 != 3 || dofNd18 != 3 || dofNd19 != 3 || dofNd20 != 3 )
        {
            cerr << "FATAL ERROR TwentyNodeBrickLT (tag: " << this->getTag() <<
                 "), has differing number of DOFs at its nodes\n";
            exit(-1);
        }

        // add the number of gauss node and the number of connectivity nodes -- Added by Sumeet 30th July, 2016
        theDomain->add_Gauss_Points(27);
        theDomain->add_Connectivity_Nodes(20);
        ///---------------------------------------------------------------------//
        this->DomainComponent::setDomain(theDomain);
    }

    ComputeVolume();
    computeGaussPoint();
}



















//=============================================================================
int TwentyNodeBrickLT::commitState ()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "TwentyNodeBrickLT::commitState () - failed in base class";
    }


    Vector pp = getResistingForce();





    for (int ii = 0; ii < nodes_in_brick; ii++ )
    {
        retVal += material_array[ii]->commitState();
    }

    return retVal;
}

//=============================================================================
int TwentyNodeBrickLT::revertToLastCommit ()
{
    int retVal = 0;

    for (int gp = 0; gp < 27; gp++ )
    {
        retVal += material_array[gp]->revertToLastCommit();
    }


    return retVal;
}





//=============================================================================
int TwentyNodeBrickLT::revertToStart ()
{
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states
    for (int gp = 0; gp < 27; gp++ )
    {
        retVal += material_array[gp]->revertToStart();
    }


    return retVal;

    // Loop over the integration points and revert to initial material states
}









//=============================================================================
const Matrix &TwentyNodeBrickLT::getTangentStiff ()
{
    DTensor4 stifftensor(20, 3, 3, 20, 0.0);

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;


    // static DTensor2 incremental_strain(3, 3, 0.0);
    DTensor2 dh_drst( nodes_in_brick, 3, 0.0 );
    DTensor2 dhGlobal( nodes_in_brick, 3, 0.0 );
    DTensor2 Jacobian(3, 3, 0.0);
    DTensor2 JacobianINV(3, 3, 0.0);

    DTensor4 E_elpl(3, 3, 3, 3, 0.0);
    DTensor4 Kkt( nodes_in_brick, 3, 3, nodes_in_brick, 0.0);

    Index < 'a' > a;
    Index < 'b' > b;
    Index < 'c' > c;
    Index < 'd' > d;

    for ( short gp = 0; gp < 27; gp++ )
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

    for ( int i = 1 ; i <= nodes_in_brick ; i++ )
    {
        for ( int j = 1 ; j <= nodes_in_brick ; j++ )
        {
            for ( int k = 1 ; k <= 3 ; k++ )
            {
                for ( int l = 1 ; l <= 3 ; l++ )
                {
                    Ki = k + 3 * (i - 1);
                    Kj = l + 3 * (j - 1);
                    K( Ki - 1 , Kj - 1 ) = stifftensor(i-1, k-1, l-1, j-1);
                }
            }
        }
    }

    return K;
}

// ==========
// To improve efficiency, the content in the function below was moved to getTangentStiff.
// Therefore, this function may be useless now if now called by other files
// maybe remove this function
const DTensor4 &TwentyNodeBrickLT::getStiffnessTensor(void) const
{

    static DTensor4 stifftensor(nodes_in_brick, 3, 3, nodes_in_brick, 0.0);
    
    for (auto it = stifftensor.begin(); it != stifftensor.end(); it++) 
    {
        *it = 0.0;
    }

    return stifftensor;
}





//=============================================================================
const Matrix &TwentyNodeBrickLT::getInitialStiff ()
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





////#############################################################################
//  tensor TwentyNodeBrickLT::mass_tensor(Elastic  mmodel)
const Matrix &TwentyNodeBrickLT::getMass()
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



        DTensor2 dh( 20, 3, 0.0 );
        DTensor2 H(60, 3, 0.0 );
        DTensor2 Mm( 60, 60, 0.0 );
        DTensor2 Jacobian(3, 3);

        double RHO;
        RHO = rho;  //global

        for ( short gp = 0; gp < 27; gp++ )
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

            weight = w_r * w_s * w_t * RHO * det_of_Jacobian;
            Mm(i, j) = Mm(i, j) + H(i, k) * H(j, k) * weight;
        }


        for ( int ii = 1 ; ii <= 60 ; ii++ )
        {
            for ( int jj = 1 ; jj <= 60 ; jj++ )
            {
                M( ii - 1 , jj - 1 ) = Mm( ii - 1, jj - 1 );
            }

        }

        is_mass_computed = true;

    }

    return M;
}







//=============================================================================
void TwentyNodeBrickLT::zeroLoad(void)
{
    Q.Zero();
}


//======================================================================
const Vector &TwentyNodeBrickLT::getBodyForce(double loadFactor, const Vector &data)
{


    static Vector bforce(60);
    bforce.Zero();

    // Check for a quick return
    if (rho == 0.0)
    {
        return bforce;
    }

    Vector bf(3);
    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);

    Vector ba(60), bfx(3);
    bfx(0) = bf(0) * loadFactor;
    bfx(1) = bf(1) * loadFactor;
    bfx(2) = bf(2) * loadFactor;

    ba( 0) = bfx(0);
    ba( 1) = bfx(1);
    ba( 2) = bfx(2);
    ba( 3) = bfx(0);
    ba( 4) = bfx(1);
    ba( 5) = bfx(2);
    ba( 6) = bfx(0);
    ba( 7) = bfx(1);
    ba( 8) = bfx(2);
    ba( 9) = bfx(0);
    ba(10) = bfx(1);
    ba(11) = bfx(2);
    ba(12) = bfx(0);
    ba(13) = bfx(1);
    ba(14) = bfx(2);
    ba(15) = bfx(0);
    ba(16) = bfx(1);
    ba(17) = bfx(2);
    ba(18) = bfx(0);
    ba(19) = bfx(1);
    ba(20) = bfx(2);
    ba(21) = bfx(0);
    ba(22) = bfx(1);
    ba(23) = bfx(2);
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

    //Form equivalent body force
    this->getMass();
    bforce.addMatrixVector(0.0, M, ba, 1.0);


    return bforce;
}






//======================================================================
const Vector &TwentyNodeBrickLT::getSurfaceForce(double loadFactor, const Vector &data)
{
    map<int,int> local_nodes_map; int local_nodes[8];
    //note -> the user at least should start with a node on the corner edge (sumeet)
    
    /////////////////////////////////////////// Edited by Sumeet 30/03/2016 //////////////////////////////
    // checking if node exists in the element
    for ( int i =0; i<8 ;i++){
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
    static const int Node_to_Surface_Number[20][4]={{3,0,2,4},{3,0,2,5},{3,0,3,5},{3,0,3,4},
                                                    {3,1,2,4},{3,1,2,5},{3,1,3,5},{3,1,3,4},
                                                    {2,0,2,0},{2,0,5,0},{2,0,3,0},{2,0,4,0},
                                                    {2,1,2,0},{2,1,5,0},{2,1,3,0},{2,1,4,0}};
    static const int Surface_Number_to_Node_order[6][8]={{1,2,3,0,9 ,10,11,8 },
                                                         {5,4,7,6,12,15,14,13},
                                                         {5,1,0,4,17,8 ,16,12},
                                                         {2,6,7,3,18,14,19,10},
                                                         {0,3,7,4,11,19,15,16},
                                                         {1,5,6,2,17,13,18,9 }};    
    ////////////////////////////////////// Edited by Sumeet 30/3/2016 /////////////////////////////////////
    //  Finding the correct surface nodes order
    int success =0; int surface_nodes_order[8]={0,0,0,0,0,0,0,0};
    int number_of_surfaces = Node_to_Surface_Number[local_nodes[0]][0];
    for ( int i =0; i<number_of_surfaces ;i++){
        int surface_no = Node_to_Surface_Number[local_nodes[0]][i+1];
        for( int j=0; j<8;j++){
            std::map<int,int>::iterator it;
            int node=Surface_Number_to_Node_order[surface_no][j];
            it=local_nodes_map.find(node);
            if (it == local_nodes_map.end()){
                success=0;break;
            }
            success=success+1;
            surface_nodes_order[j]= it->second;         
        }
        if(success==8) break;
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
    const Vector &coordnode5 = theNodes[local_nodes[(surface_nodes_order[4])]]->getCrds();
    const Vector &coordnode6 = theNodes[local_nodes[(surface_nodes_order[5])]]->getCrds();
    const Vector &coordnode7 = theNodes[local_nodes[(surface_nodes_order[6])]]->getCrds();
    const Vector &coordnode8 = theNodes[local_nodes[(surface_nodes_order[7])]]->getCrds();

    double ShapeFunctionValues;
    Vector J_vector(3);
    Vector Pressure(8);
    double LoadValue;


    Pressure(0) = data(surface_nodes_order[0]+8) * loadFactor;
    Pressure(1) = data(surface_nodes_order[1]+8) * loadFactor;
    Pressure(2) = data(surface_nodes_order[2]+8) * loadFactor;
    Pressure(3) = data(surface_nodes_order[3]+8) * loadFactor;
    Pressure(4) = data(surface_nodes_order[4]+8) * loadFactor;
    Pressure(5) = data(surface_nodes_order[5]+8) * loadFactor;
    Pressure(6) = data(surface_nodes_order[6]+8) * loadFactor;
    Pressure(7) = data(surface_nodes_order[7]+8) * loadFactor;

    static Vector NodalForces(60);

    for (int m = 0; m < 60; m++){
        NodalForces(m) = 0;
    }

    double Root3Over5 = sqrt(3.0 / 5.0);
    double W5Over9 = 5.0 / 9.0;
    double W8Over9 = 8.0 / 9.0;
    Matrix GsPts(9, 2);
    Matrix Weight(9, 2);


    GsPts(0, 0) = Root3Over5;
    GsPts(0, 1) = Root3Over5;

    GsPts(1, 0) = -Root3Over5;
    GsPts(1, 1) = Root3Over5;

    GsPts(2, 0) = -Root3Over5;
    GsPts(2, 1) = -Root3Over5;

    GsPts(3, 0) = Root3Over5;
    GsPts(3, 1) = -Root3Over5;

    GsPts(4, 0) = 0.0;
    GsPts(4, 1) = Root3Over5;

    GsPts(5, 0) = -Root3Over5;
    GsPts(5, 1) = 0.0;

    GsPts(6, 0) = 0.0;
    GsPts(6, 1) = -Root3Over5;

    GsPts(7, 0) = Root3Over5;
    GsPts(7, 1) = 0.0;

    GsPts(8, 0) = 0.0;
    GsPts(8, 1) = 0.0;


    Weight(0, 0) = W5Over9;
    Weight(0, 1) = W5Over9;
    Weight(1, 0) = W5Over9;
    Weight(1, 1) = W5Over9;
    Weight(2, 0) = W5Over9;
    Weight(2, 1) = W5Over9;
    Weight(3, 0) = W5Over9;
    Weight(3, 1) = W5Over9;
    Weight(4, 0) = W8Over9;
    Weight(4, 1) = W5Over9;
    Weight(5, 0) = W5Over9;
    Weight(5, 1) = W8Over9;
    Weight(6, 0) = W8Over9;
    Weight(6, 1) = W5Over9;
    Weight(7, 0) = W5Over9;
    Weight(7, 1) = W8Over9;
    Weight(8, 0) = W8Over9;
    Weight(8, 1) = W8Over9;

    int r = 0;

    // loop over dof
    for (int k = 0; k < 3; k++)
    {
        // loop over nodes
        for (int j = 0; j < 8; j++)
        {
            r = local_nodes[(surface_nodes_order[j])] ;
            // loop over Gauss points
            for (int i = 0; i < 9; i++)
            {
                ShapeFunctionValues = SurfaceShapeFunctionValues(GsPts(i, 0) , GsPts(i, 1), j);
                J_vector = Direction_Weight(GsPts(i, 0) , GsPts(i, 1),coordnode1, coordnode2, 
                        coordnode3, coordnode4,coordnode5, coordnode6, coordnode7, coordnode8);
                LoadValue = SurfaceLoadValues(GsPts(i, 0) , GsPts(i, 1), Pressure);
                NodalForces(r * 3 + k) = NodalForces(r * 3 + k) + LoadValue * J_vector(k) * ShapeFunctionValues * Weight(i, 0) * Weight(i, 1);
            }
        }
    }

    // ////////////////////////////// For Debugging By Sumeet //////////////////////////////////////////
    // for ( int i =0; i < 20 ; i++)
    //     cout << NodalForces(3*i)<< " " << NodalForces(3*i+1) << " " << NodalForces(3*i+2) <<   "\n";
    // cout << "\n\n******************************************************************************\n\n";
    // /////////////////////////////////////////////////////////////////////////////////////////////////
    return NodalForces;
}


//=============================================================================
int TwentyNodeBrickLT::addLoad(ElementalLoad *theLoad, double loadFactor)
{

    int type;
    const Vector &data = theLoad->getData(type, loadFactor);

    if (type == LOAD_TAG_ElementSelfWeight)
    {

        Vector Fbody = this->getBodyForce(loadFactor, data);

        Q.addVector(1.0, Fbody, 1.0);


    }
    else if (type == LOAD_TAG_BrickSurfaceLoad)
    {

        Vector Fsurface = this->getSurfaceForce(loadFactor, data);

        Q.addVector(1.0, Fsurface, 1.0);


    }
    else
    {
        cerr << "TwentyNodeBrickLT::addLoad() - 20NodeBrick " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }





    return 0;
}










//=============================================================================
int TwentyNodeBrickLT::addInertiaLoadToUnbalance(const Vector &accel)
{
    // Check for a quick return
    if (rho == 0.0)
    {
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1  = theNodes[0]->getRV(accel);
    const Vector &Raccel2  = theNodes[1]->getRV(accel);
    const Vector &Raccel3  = theNodes[2]->getRV(accel);
    const Vector &Raccel4  = theNodes[3]->getRV(accel);
    const Vector &Raccel5  = theNodes[4]->getRV(accel);
    const Vector &Raccel6  = theNodes[5]->getRV(accel);
    const Vector &Raccel7  = theNodes[6]->getRV(accel);
    const Vector &Raccel8  = theNodes[7]->getRV(accel);
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

    if (3 != Raccel1.Size()  || 3 != Raccel2.Size()  || 3 != Raccel3.Size()  || 3 != Raccel4.Size() ||
            3 != Raccel5.Size()  || 3 != Raccel6.Size()  || 3 != Raccel7.Size()  || 3 != Raccel8.Size() ||
            3 != Raccel9.Size()  || 3 != Raccel10.Size() || 3 != Raccel11.Size() || 3 != Raccel12.Size() ||
            3 != Raccel13.Size() || 3 != Raccel14.Size() || 3 != Raccel15.Size() || 3 != Raccel16.Size() ||
            3 != Raccel17.Size() || 3 != Raccel18.Size() || 3 != Raccel19.Size() || 3 != Raccel20.Size()   )
    {
        // Xiaoyan changed 2 to 3 and added Racce15-18  09/27/00
        cerr << "TwentyNodeBrickLT::addInertiaLoadToUnbalance " <<
             "matrix and vector sizes are incompatable\n";
        return -1;
    }

    Vector ra(60);  

    ra( 0) = Raccel1(0);
    ra( 1) = Raccel1(1);
    ra( 2) = Raccel1(2);
    ra( 3) = Raccel2(0);
    ra( 4) = Raccel2(1);
    ra( 5) = Raccel2(2);
    ra( 6) = Raccel3(0);
    ra( 7) = Raccel3(1);
    ra( 8) = Raccel3(2);
    ra( 9) = Raccel4(0);
    ra(10) = Raccel4(1);
    ra(11) = Raccel4(2);
    ra(12) = Raccel5(0);
    ra(13) = Raccel5(1);
    ra(14) = Raccel5(2);
    ra(15) = Raccel6(0);
    ra(16) = Raccel6(1);
    ra(17) = Raccel6(2);
    ra(18) = Raccel7(0);
    ra(19) = Raccel7(1);
    ra(20) = Raccel7(2);
    ra(21) = Raccel8(0);
    ra(22) = Raccel8(1);
    ra(23) = Raccel8(2);
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


    Q.addMatrixVector(1.0, M, ra, -1.0);

    return 0;
}

//=============================================================================
const Vector TwentyNodeBrickLT::FormEquiBodyForce(const Vector &data)
{
    Vector bforce(60);

    // Check for a quick return
    //cerr << "rho " << rho << endln;
    if (rho == 0.0)
    {
        return bforce;
    }


    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);

    Vector ba(60);

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


    bforce.addMatrixVector(0.0, M, ba, 1.0);


    return bforce;
}


//=============================================================================
const Vector &TwentyNodeBrickLT::getResistingForce ()
{
    DTensor2 nodalforces( 20, 3, 0.0 );

    nodalforces = nodal_forces();

    //converting nodalforce tensor to vector
    for (int i = 0; i < nodes_in_brick; i++){
        for (int j = 0; j < 3; j++){
            P(i * 3 + j) = nodalforces( i, j );
        }
    }

    P.addVector( 1.0, Q, -1.0 );

    return P;
}





//=============================================================================
const Vector &TwentyNodeBrickLT::getResistingForceIncInertia ()
{

    this->getResistingForce();

    //
    // now add dynamic terms
    // P += M * a + C * v
    //

    if (rho != 0.0)
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

        static Vector a(60);  

        a( 0) = accel1(0);
        a( 1) = accel1(1);
        a( 2) = accel1(2);
        a( 3) = accel2(0);
        a( 4) = accel2(1);
        a( 5) = accel2(2);
        a( 6) = accel3(0);
        a( 7) = accel3(1);
        a( 8) = accel3(2);
        a( 9) = accel4(0);
        a(10) = accel4(1);
        a(11) = accel4(2);
        a(12) = accel5(0);
        a(13) = accel5(1);
        a(14) = accel5(2);
        a(15) = accel6(0);
        a(16) = accel6(1);
        a(17) = accel6(2);
        a(18) = accel7(0);
        a(19) = accel7(1);
        a(20) = accel7(2);
        a(21) = accel8(0);
        a(22) = accel8(1);
        a(23) = accel8(2);
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

        // P += M * a
        P.addMatrixVector(1.0, M, a, 1.0);

        // add the damping forces if rayleigh damping
        if (a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0){
            P += this->getRayleighDampingForces();
        }

    }else{

        // add the damping forces if rayleigh damping
        if (a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            P += this->getRayleighDampingForces();
        }

    }

    return P;
}

//=============================================================================
int TwentyNodeBrickLT::sendSelf (int commitTag, Channel &theChannel)
{
    //Copied from EightNodeBrickLT (with modifications)    ID idData( 5 );

    ID idData( 5 );


    idData( 0 ) = this->getTag();
    idData( 1 ) = numDOF;
    idData( 2 ) = nodes_in_brick;
    idData( 3 ) = order;
    idData( 4 ) = material_array[0]->getClassTag();

    if ( theChannel.sendID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send ID idData\n";
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
        cerr << "WARNING TwentyNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }


    // Send the nodes

    if ( theChannel.sendID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send ID connectedExternalNodes\n";
        return -1;
    }


    // material objects send themselves

    for ( int i = 0; i < nodes_in_brick; i++ )
    {
        if ( material_array[i]->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "WARNING TwentyNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send material models\n";
            return -1;
        }
    }


    //Send Q
    if ( theChannel.sendVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send its Q\n";
        return -1;
    }

    //Send bf
    if ( theChannel.sendVector( 0, commitTag, bf ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send its bf\n";
        return -1;
    }

    //Send the gauss points
    if ( theChannel.sendMatrix( 0, commitTag, gauss_points ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send its Gauss point coordinates\n";
        return -1;
    }

    //Send outputVector
    if ( theChannel.sendVector( 0, commitTag, outputVector ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send its outputVector\n";
        return -1;
    }

    return 0;
}

//=============================================================================
int TwentyNodeBrickLT::receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
   //Copied from EightNodeBrickLT (with modifications)    // cout << "TwentyNodeBrickLT::receiveSelf() tag = " << this->getTag() << "\n";

    ID idData( 5 );

    if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::receiveSelf() - failed to receive ID\n";
        return -1;
    }

    this->setTag( idData( 0 ) );
    numDOF = idData(1);
    nodes_in_brick = idData(2);
    order = idData(3);
    int matClassTag = idData( 4 );


    // cout << "TwentyNodeBrickLT::receiveSelf() numDOF           = " << numDOF << "\n";
    // cout << "TwentyNodeBrickLT::receiveSelf() nodes_in_brick   = " << nodes_in_brick << "\n";
    // cout << "TwentyNodeBrickLT::receiveSelf() order            = " << order << "\n";
    // cout << "TwentyNodeBrickLT::receiveSelf() materialclasstag = " << idData(4) << "\n";


    Vector floatData(4);
    if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }
    Volume                  = floatData(0) ;
    e_p                     = floatData(1) ;
    determinant_of_Jacobian = floatData(2) ;
    rho                     = floatData(3) ;

    // cout << "TwentyNodeBrickLT::receiveSelf() Volume                  = " << Volume << "\n";
    // cout << "TwentyNodeBrickLT::receiveSelf() e_p                     = " << e_p << "\n";
    // cout << "TwentyNodeBrickLT::receiveSelf() determinant_of_Jacobian = " << determinant_of_Jacobian << "\n";
    // cout << "TwentyNodeBrickLT::receiveSelf() rho                     = " << rho << "\n";


    // Recieve the nodes

    if ( theChannel.receiveID( 0, commitTag, connectedExternalNodes ) < 0 )
    {
        cerr << "WARNING TwentyNodeBrickLT::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
        return -1;
    }

    // cout << "TwentyNodeBrickLT::receiveSelf() connectedExternalNodes = " << connectedExternalNodes << "\n";

    for ( int i = 0; i < nodes_in_brick; i++ )
    {

        // Allocate new material with the sent class tag
        NDMaterialLT *ndmat = theBroker.getNewNDMaterialLT( matClassTag );
        if ( ndmat == 0 )
        {
            cerr << "TwentyNodeBrickLT::receiveSelf() - Broker could not create NDMaterialLT of class type " << matClassTag << "\n";
            return -1;
        }

        // Now receive materials into the newly allocated space
        if ( ( ndmat )->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
        {
            cerr << "TwentyNodeBrickLT::receiveSelf() - material " << i << "failed to recv itself\n";
            return -1;
        }

        material_array[i] = ndmat;
    }


    // Q
    if ( theChannel.receiveVector( 0, commitTag, Q ) < 0 )
    {
        cerr << "TwentyNodeBrickLT::receiveSelf() - failed to recv Q!\n";
        return -1;
    }

    // bf
    if ( theChannel.receiveVector( 0, commitTag, bf ) < 0 )
    {
        cerr << "TwentyNodeBrickLT::receiveSelf() - failed to recv bf!\n";
        return -1;
    }

    // gauss_points
    if ( theChannel.receiveMatrix( 0, commitTag, gauss_points ) < 0 )
    {
        cerr << "TwentyNodeBrickLT::receiveSelf() - failed to recv gauss_points!\n";
        return -1;
    }

    // outputVector
    if ( theChannel.receiveVector( 0, commitTag, outputVector ) < 0 )
    {
        cerr << "TwentyNodeBrickLT::receiveSelf() - failed to recv outputVector!\n";
        return -1;
    }


    return 0;
}



int TwentyNodeBrickLT::getObjectSize()
{

    int size = 0;
    // Just add all sizes of data-members (there is some static local variables too.... but those are not important for transference)
    //
    // bool is_mass_computed;
    size += sizeof(is_mass_computed);
    // int numDOF;
    size += sizeof(numDOF);
    // int nodes_in_brick;
    size += sizeof(nodes_in_brick);
    // int order;
    size += sizeof(order);
    // double Volume;
    size += sizeof(Volume);
    // double e_p;
    size += sizeof(e_p);
    // double determinant_of_Jacobian;
    size += sizeof(determinant_of_Jacobian);
    // double rho;
    size += sizeof(rho);
    // ID  connectedExternalNodes;
    size += sizeof(int) * connectedExternalNodes.Size();
    // DTensor2 stress[8];
    size += sizeof(double) * stress[0].get_size() * 27;
    // Matrix *Ki;
    size += sizeof(Ki);
    // Node *theNodes[8];
    size += sizeof(theNodes) ;
    // NDMaterialLT *mmodel;
    size += sizeof(mmodel);
    // NDMaterialLT *material_array[8];
    size += sizeof(material_array) * 27;
    // Vector Q;
    size += sizeof(double) * Q.Size();
    // Vector bf;
    size += sizeof(double) * bf.Size();
    // Matrix gauss_points   NOT INCLUDED IN MEMORY COUNT... THEY're Static (J. Abell);
    // size += sizeof(gauss_points);
    // // Vector outputVector;
    // size += sizeof(outputVector);
    // Index < 'i' > i;
    size += sizeof(i);
    // Index < 'j' > j;
    size += sizeof(j);
    // Index < 'k' > k;
    size += sizeof(k);
    // Index < 'l' > l;
    size += sizeof(l);



    for ( int i = 0; i < 27; i++ )
    {
        size += material_array[i]->getObjectSize();
    }

    return size;
}






//=============================================================================
void TwentyNodeBrickLT::Print(ostream &s, int flag)
{
    cout << "TwentyNodeBrickLT: LTensor-based 27 node brick." << endl << endl;

    cout << "Element tag: " << getTag() << endl;
    cout << "Connected Nodes: " << endl;


    for ( int node = 0; node < nodes_in_brick; node++)
    {
        cout << "   #" << node + 1 << ": Domain node #" << connectedExternalNodes(node) << endl;
    }

    cout << "K = " << endl;

    for (int ii = 0; ii < 60; ii++)
    {
        for (int jj = 0; jj < 60; jj++)
        {
            cout << K(ii, jj);

            if (jj < 59)
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

    for (int ii = 0; ii < 60; ii++)
    {
        cout << K(ii, ii);

        if (ii < 59)
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

    for (int gp = 0; gp < 27; gp++)
    {
        cout << "   GP # " << gp <<  ": (" << gp_coords(gp, 0) << ","
             << gp_coords(gp, 1) << ","
             << gp_coords(gp, 2) << ")" << endl;
        cout << "        State of stress " << material_array[gp]->getStressTensor() << endl;
    }

}





void TwentyNodeBrickLT::ComputeVolume()
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

    DTensor2 dh( nodes_in_brick, 3, 0.0 );
    DTensor2 Jacobian(3, 3, 0.0);

    for ( short gp = 0; gp < 27; gp++ )
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








int TwentyNodeBrickLT::update() //Added by Guanzhou, May 7 2004
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;



    DTensor2 dh( nodes_in_brick, 3, 0.0 );
    //DTensor2 Jacobian(3,3,0.0);
    DTensor2 JacobianINV(3, 3, 0.0);
    DTensor2 dhGlobal( nodes_in_brick, 3, 0.0 );
    DTensor2 trial_disp( nodes_in_brick, 3, 0.0  );
    DTensor2 total_strain(3, 3, 0.0);
    DTensor2 trial_strain(3, 3, 0.0);

    trial_disp = total_disp();



    for ( short gp = 0; gp < 27; gp++ )
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
            cerr << "TwentyNodeBrickLT::update (tag: " << this->getTag() << "), Update Failed\n";
        }
    }


    return 0;


}







/////////////////////////////////////////////////////////////////////////
// Nima Tafazzoli added for surface load (July 2012)
double  TwentyNodeBrickLT::SurfaceShapeFunctionValues(double Xi , double Eta, int whichcomponent)
{

    ShapeFunctionValues_in_function(0) =  0.25 * (1 + Xi)  * (1 + Eta) * (Xi + Eta - 1);
    ShapeFunctionValues_in_function(1) =  0.25 * (1 - Xi)  * (1 + Eta) * (-Xi + Eta - 1);
    ShapeFunctionValues_in_function(2) = -0.25 * (1 - Xi)  * (1 - Eta) * (Xi + Eta + 1);
    ShapeFunctionValues_in_function(3) =  0.25 * (1 + Xi)  * (1 - Eta) * (Xi - Eta - 1);
    ShapeFunctionValues_in_function(4) =  0.5  * (1 - Xi)  * (1 + Xi)  * (1 + Eta);
    ShapeFunctionValues_in_function(5) =  0.5  * (1 - Eta) * (1 + Eta) * (1 - Xi);
    ShapeFunctionValues_in_function(6) =  0.5  * (1 - Xi)  * (1 + Xi)  * (1 - Eta);
    ShapeFunctionValues_in_function(7) =  0.5  * (1 - Eta) * (1 + Eta) * (1 + Xi);


    return ShapeFunctionValues_in_function(whichcomponent);

}


Vector &TwentyNodeBrickLT::Direction_Weight(double Xi , double Eta,
        Vector coord1, Vector coord2, Vector coord3, Vector coord4,
        Vector coord5, Vector coord6, Vector coord7, Vector coord8)
{
    Vector J1(3);
    Vector J2(3);


    J1(0) = 0.25 * ( (1 + Eta) + (1 + Eta) * 2 * Xi - (1 - Eta * Eta) ) * coord1(0)
            + 0.25 * ( -(1 + Eta) + (1 + Eta) * 2 * Xi + (1 - Eta * Eta) ) * coord2(0)
            + 0.25 * ( -(1 - Eta) + (1 - Eta * Eta) + (1 - Eta) * 2 * Xi ) * coord3(0)
            + 0.25 * ( (1 - Eta) + (1 - Eta) * 2 * Xi - (1 - Eta * Eta) ) * coord4(0)
            - Xi * (1 + Eta) * coord5(0)
            - 0.5 * (1 - Eta * Eta) * coord6(0)
            - Xi * (1 - Eta) * coord7(0)
            + 0.5 * (1 - Eta * Eta) * coord8(0);




    J1(1) = 0.25 * ( (1 + Eta) + (1 + Eta) * 2 * Xi - (1 - Eta * Eta) ) * coord1(1)
            + 0.25 * ( -(1 + Eta) + (1 + Eta) * 2 * Xi + (1 - Eta * Eta) ) * coord2(1)
            + 0.25 * ( -(1 - Eta) + (1 - Eta * Eta) + (1 - Eta) * 2 * Xi ) * coord3(1)
            + 0.25 * ( (1 - Eta) + (1 - Eta) * 2 * Xi - (1 - Eta * Eta) ) * coord4(1)
            - Xi * (1 + Eta) * coord5(1)
            - 0.5 * (1 - Eta * Eta) * coord6(1)
            - Xi * (1 - Eta) * coord7(1)
            + 0.5 * (1 - Eta * Eta) * coord8(1);


    J1(2) = 0.25 * ( (1 + Eta) + (1 + Eta) * 2 * Xi - (1 - Eta * Eta) ) * coord1(2)
            + 0.25 * ( -(1 + Eta) + (1 + Eta) * 2 * Xi + (1 - Eta * Eta) ) * coord2(2)
            + 0.25 * ( -(1 - Eta) + (1 - Eta * Eta) + (1 - Eta) * 2 * Xi ) * coord3(2)
            + 0.25 * ( (1 - Eta) + (1 - Eta) * 2 * Xi - (1 - Eta * Eta) ) * coord4(2)
            - Xi * (1 + Eta) * coord5(2)
            - 0.5 * (1 - Eta * Eta) * coord6(2)
            - Xi * (1 - Eta) * coord7(2)
            + 0.5 * (1 - Eta * Eta) * coord8(2);



    J2(0) = 0.25 * ( (1 + Xi) + (1 + Xi) * 2 * Eta - (1 - Xi * Xi) ) * coord1(0)
            + 0.25 * ( (1 - Xi) + (1 - Xi) * 2 * Eta - (1 - Xi * Xi) ) * coord2(0)
            + 0.25 * ( -(1 - Xi) + (1 - Xi * Xi) + (1 - Xi) * 2 * Eta ) * coord3(0)
            + 0.25 * ( -(1 + Xi) + (1 + Xi) * 2 * Eta + (1 - Xi * Xi) ) * coord4(0)
            + 0.5 * (1 - Xi * Xi) * coord5(0)
            - Eta * (1 - Xi) * coord6(0)
            - 0.5 * (1 - Xi * Xi) * coord7(0)
            - Eta * (1 + Xi) * coord8(0);


    J2(1) = 0.25 * ( (1 + Xi) + (1 + Xi) * 2 * Eta - (1 - Xi * Xi) ) * coord1(1)
            + 0.25 * ( (1 - Xi) + (1 - Xi) * 2 * Eta - (1 - Xi * Xi) ) * coord2(1)
            + 0.25 * ( -(1 - Xi) + (1 - Xi * Xi) + (1 - Xi) * 2 * Eta ) * coord3(1)
            + 0.25 * ( -(1 + Xi) + (1 + Xi) * 2 * Eta + (1 - Xi * Xi) ) * coord4(1)
            + 0.5 * (1 - Xi * Xi) * coord5(1)
            - Eta * (1 - Xi) * coord6(1)
            - 0.5 * (1 - Xi * Xi) * coord7(1)
            - Eta * (1 + Xi) * coord8(1);


    J2(2) = 0.25 * ( (1 + Xi) + (1 + Xi) * 2 * Eta - (1 - Xi * Xi) ) * coord1(2)
            + 0.25 * ( (1 - Xi) + (1 - Xi) * 2 * Eta - (1 - Xi * Xi) ) * coord2(2)
            + 0.25 * ( -(1 - Xi) + (1 - Xi * Xi) + (1 - Xi) * 2 * Eta ) * coord3(2)
            + 0.25 * ( -(1 + Xi) + (1 + Xi) * 2 * Eta + (1 - Xi * Xi) ) * coord4(2)
            + 0.5 * (1 - Xi * Xi) * coord5(2)
            - Eta * (1 - Xi) * coord6(2)
            - 0.5 * (1 - Xi * Xi) * coord7(2)
            - Eta * (1 + Xi) * coord8(2);



    J_vector_in_function(0) =  J1(1) * J2(2) - J1(2) * J2(1) ;
    J_vector_in_function(1) =  J1(2) * J2(0) - J1(0) * J2(2) ;
    J_vector_in_function(2) =  J1(0) * J2(1) - J1(1) * J2(0) ;


    return J_vector_in_function;
}




double TwentyNodeBrickLT::SurfaceLoadValues(double Xi , double Eta, Vector Pressure)
{

    SurfaceLoadValues_in_function =  ( 0.25 * (1 + Xi)  * (1 + Eta) * (Xi + Eta - 1) ) * Pressure(0)
                                     + ( 0.25 * (1 - Xi)  * (1 + Eta) * (-Xi + Eta - 1) ) * Pressure(1)
                                     - ( 0.25 * (1 - Xi)  * (1 - Eta) * (Xi + Eta + 1) ) * Pressure(2)
                                     + ( 0.25 * (1 + Xi)  * (1 - Eta) * (Xi - Eta - 1) ) * Pressure(3)
                                     + ( 0.5  * (1 - Xi)  * (1 + Xi)  * (1 + Eta) ) * Pressure(4)
                                     + ( 0.5  * (1 - Eta) * (1 + Eta) * (1 - Xi) ) * Pressure(5)
                                     + ( 0.5  * (1 - Xi)  * (1 + Xi)  * (1 - Eta) ) * Pressure(6)
                                     + ( 0.5  * (1 - Eta) * (1 + Eta) * (1 + Xi) ) * Pressure(7);

    return SurfaceLoadValues_in_function;

}







// Nima Tafazzoli (September 2012)
int
TwentyNodeBrickLT::CheckMesh(ofstream &checkmesh_file)
{
    bool jacobian_flag = false;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    double det_of_Jacobian = 0.0;

    DTensor2 dh( nodes_in_brick, 3, 0.0 );
    DTensor2 Jacobian(3, 3, 0.0);


    for ( short gp = 0; gp < 27; gp++ )
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
// std::shared_ptr<Vector> 
Vector *
TwentyNodeBrickLT::getStress( void )
{
    DTensor2 stress;
    Vector *stresses = new Vector( 162 );   // FIXME: Who deallocates this guy???
    // auto stresses = std::make_shared<Vector>( 162 );   

    for ( short gp = 0 ; gp < 27 ; gp++ ){
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







Matrix &TwentyNodeBrickLT::getGaussCoordinates(void)
{
    computeGaussPoint();
    return gauss_points;
}

int TwentyNodeBrickLT::getOutputSize() const
{
    return TwentyNodeBrickLT_OUTPUT_SIZE;
}






const Vector &TwentyNodeBrickLT::getOutput() 
{


    int ii = 0;
    for (int gp = 0; gp < 27; gp++)
    {
        const DTensor2 & strain = material_array[gp]->getStrainTensor();
        const DTensor2 & plstrain = material_array[gp]->getPlasticStrainTensor();
        const DTensor2 & stress = material_array[gp]->getStressTensor();

        //Write strain
        outputVector(ii++) = strain(0, 0);
        outputVector(ii++) = strain(1, 1);
        outputVector(ii++) = strain(2, 2);
        outputVector(ii++) = strain(0, 1);
        outputVector(ii++) = strain(0, 2);
        outputVector(ii++) = strain(1, 2);

        //Write strain
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
        // //Cycle material outputs and place them appropriately
        // int nmaterial_output = material_array[0]->getOutputSize();
        // if (nmaterial_output > 0)
        // {
        //     const Vector & matOutput = material_array[gp]->getOutput();
        //     for (int jj = 0; jj < nmaterial_output ; jj++)
        //     {
        //         outputVector(ii++) = matOutput(jj);
        //     }
        // }

    return outputVector;
}










































// /////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////
// void TwentyNodeBrickLT::incremental_Update()
// {
//     double r  = 0.0;
//     // double rw = 0.0;
//     double s  = 0.0;
//     // double sw = 0.0;
//     double t  = 0.0;
//     // double tw = 0.0;

//     short where = 0;
//     //,,,,,    double weight = 0.0;

//     //double this_one_PP = (material_array)->operator[](where).IS_Perfect_Plastic();

//     int dh_dim[] = {20, 3};
//     tensor dh(2, dh_dim, 0.0);


//     static int disp_dim[] = {20, 3};
//     tensor incremental_displacements(2, disp_dim, 0.0);

//     straintensor incremental_strain;

//     tensor Jacobian;
//     tensor JacobianINV;
//     tensor dhGlobal;

//     incremental_displacements = incr_disp();

//     for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//     {
//         r = get_Gauss_p_c( integration_order, GP_c_r );

//         for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//         {
//             s = get_Gauss_p_c( integration_order, GP_c_s );

//             for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//             {
//                 t = get_Gauss_p_c( integration_order, GP_c_t );

//                 where =
//                     ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                 dh = dh_drst_at(r, s, t);

//                 Jacobian = Jacobian_3D(dh);

//                 JacobianINV = Jacobian_3Dinv(dh);

//                 dhGlobal = dh("ij") * JacobianINV("kj");

//                 incremental_strain =
//                     (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
//                 incremental_strain.null_indices();

//                 if ( ! ( (material_array[where]->matmodel)->setTrialStrainIncr( incremental_strain)) )
//                 {
//                     cerr << "TwentyNodeBrickLT::incremental_Update (tag: " << this->getTag() << "), not converged\n";
//                 }

//             }
//         }
//     }
// }





// ////#############################################################################
// TwentyNodeBrickLT &TwentyNodeBrickLT::operator[](int subscript)
// {
//     return ( *(this + subscript) );
// }







// ////#############################################################################
// // returns nodal forces for given incremental strain field in an element
// // by using the linearized constitutive tensor from the begining of the step !
// tensor TwentyNodeBrickLT::linearized_nodal_forces(void)
// {
//     int force_dim[] = {20, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

//     tensor linearized_nodal_forces(2, force_dim, 0.0);

//     double r  = 0.0;
//     double rw = 0.0;
//     double s  = 0.0;
//     double sw = 0.0;
//     double t  = 0.0;
//     double tw = 0.0;

//     short where = 0;
//     double weight = 0.0;

//     int dh_dim[] = {20, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

//     tensor dh(2, dh_dim, 0.0);

//     tensor Constitutive( 4, def_dim_4, 0.0);

//     double det_of_Jacobian = 0.0;

//     static int disp_dim[] = {20, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

//     tensor incremental_displacements(2, disp_dim, 0.0);

//     straintensor incremental_strain;

//     tensor Jacobian;
//     tensor JacobianINV;
//     tensor dhGlobal;

//     stresstensor final_linearized_stress;
//     //    stresstensor incremental_stress;
//     // tensor of incremental displacements taken from node objects for this element !
//     incremental_displacements = incr_disp();
//     //incremental_displacements.print("disp","\n incremental_displacements tensor at GAUSS point in iterative_Update\n");

//     for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//     {
//         r = get_Gauss_p_c( integration_order, GP_c_r );
//         rw = get_Gauss_p_w( integration_order, GP_c_r );

//         for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//         {
//             s = get_Gauss_p_c( integration_order, GP_c_s );
//             sw = get_Gauss_p_w( integration_order, GP_c_s );

//             for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//             {
//                 t = get_Gauss_p_c( integration_order, GP_c_t );
//                 tw = get_Gauss_p_w( integration_order, GP_c_t );

//                 where =
//                     ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                 // derivatives of local coordiantes with respect to local coordiantes
//                 dh = dh_drst_at(r, s, t);

//                 // Jacobian tensor ( matrix )
//                 Jacobian = Jacobian_3D(dh);
//                 //....                Jacobian.print("J");

//                 // Inverse of Jacobian tensor ( matrix )
//                 JacobianINV = Jacobian_3Dinv(dh);
//                 //....                JacobianINV.print("JINV");

//                 // determinant of Jacobian tensor ( matrix )
//                 det_of_Jacobian  = Jacobian.determinant();

//                 dhGlobal = dh("ij") * JacobianINV("kj");

//                 //weight
//                 weight = rw * sw * tw * det_of_Jacobian;

//                 incremental_strain =
//                     (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
//                 incremental_strain.null_indices();

//                 if ( ! (material_array[where]->matmodel)->setTrialStrainIncr( incremental_strain)  )
//                 {
//                     cerr << "TwentyNodeBrickLT::linearized_nodal_forces (tag: " << this->getTag() << "), not converged\n";
//                 }

//                 Constitutive = (material_array[where]->matmodel)->getTangentTensor();


//                 final_linearized_stress =
//                     Constitutive("ijkl") * incremental_strain("kl");

//                 // nodal forces See Zienkievicz part 1 pp 108
//                 linearized_nodal_forces = linearized_nodal_forces +
//                                           dhGlobal("ib") * final_linearized_stress("ab") * weight;
//                 //::::::                   nodal_forces.print("nf","\n\n Nodal Forces \n");

//             }
//         }
//     }


//     return linearized_nodal_forces;

// }



#endif
