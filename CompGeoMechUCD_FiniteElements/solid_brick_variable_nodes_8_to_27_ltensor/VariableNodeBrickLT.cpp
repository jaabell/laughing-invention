///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli and Boris Jeremic
// DATE:               May 2013
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



#ifndef VariableNodeBrickLTLT_CPP
#define VariableNodeBrickLTLT_CPP

#include <VariableNodeBrickLT.h>
#include <LTensorDisplay.h>
#include <HDF5_Channel.h>


VariableNodeBrickLT::VariableNodeBrickLT(int element_number,
                                     int int_order,
                                     int node_numb_1,
                                     int node_numb_2,
                                     int node_numb_3,
                                     int node_numb_4,
                                     int node_numb_5,
                                     int node_numb_6,
                                     int node_numb_7,
                                     int node_numb_8,
                                     int node_numb_9,
                                     int node_numb_10,
                                     int node_numb_11,
                                     int node_numb_12,
                                     int node_numb_13,
                                     int node_numb_14,
                                     int node_numb_15,
                                     int node_numb_16,
                                     int node_numb_17,
                                     int node_numb_18,
                                     int node_numb_19,
                                     int node_numb_20,
                                     int node_numb_21,
                                     int node_numb_22,
                                     int node_numb_23,
                                     int node_numb_24,
                                     int node_numb_25,
                                     int node_numb_26,
                                     int node_numb_27,
                                     NDMaterialLT *Globalmmodel)
    : Element(element_number, ELE_TAG_VariableNodeBrickLT),
      connectedExternalNodes(27), node_existance(19)
{
    is_mass_computed = false;

    rho = Globalmmodel->getRho();

    determinant_of_Jacobian = 0.0;

    integration_order = int_order; // Gauss-Legendre integration order
    Num_TotalGaussPts = integration_order * integration_order * integration_order;
    VariableNodeBrickLT_OUTPUT_SIZE=Num_TotalGaussPts*6*3;

    mmodel = Globalmmodel;
    for (int k = 0; k < Num_TotalGaussPts; k++)
    {
        material_array[k] = mmodel->getCopy();
    }

    // GaussCoord = new Vector(Num_TotalGaussPts * 3 + 1);



// # ifndef _PARALLEL_PROCESSING
//     populate();
// # endif



    connectedExternalNodes(0)  = node_numb_1;
    connectedExternalNodes(1)  = node_numb_2;
    connectedExternalNodes(2)  = node_numb_3;
    connectedExternalNodes(3)  = node_numb_4;
    connectedExternalNodes(4)  = node_numb_5;
    connectedExternalNodes(5)  = node_numb_6;
    connectedExternalNodes(6)  = node_numb_7;
    connectedExternalNodes(7)  = node_numb_8;
    connectedExternalNodes(8)  = node_numb_9;
    connectedExternalNodes(9)  = node_numb_10;
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


    // loop nodes 9-27 and :
    //  1) put the right coordinates on place,
    //  2) calculate the total number of nodes
    nodes_in_brick = 8;

    for ( int i = 8 ; i < 27 ; i++ )
    {
        if (connectedExternalNodes(i) == 0 )
        {
            node_existance(i - 8) = 0;
        }
        else
        {
            nodes_in_brick++;
            node_existance(i - 8) = 1;
        }
    }



    //     theNodes = new Node* [nodes_in_brick];
    // theNodes = new Node* [27];

    // zero node pointers
    //     for (int i=0; i<nodes_in_brick; i++)
    for (int i = 0; i < 27; i++)
    {
        theNodes[i] = 0;
    }



    K = new Matrix(nodes_in_brick * 3, nodes_in_brick * 3);

    M = new Matrix(nodes_in_brick * 3, nodes_in_brick * 3);

    P = new Vector(nodes_in_brick * 3);

    Q = new Vector(nodes_in_brick * 3);

    existing_node_numbers = new ID(nodes_in_brick);



    int j = 0;

    for ( int i = 0 ; i < 27 ; i++ )
    {
        if (connectedExternalNodes(i) != 0 )
        {
            (*existing_node_numbers)(j) = connectedExternalNodes(i);
            j++;
        }
    }





    switch(integration_order){
        // integration_order 2
        case 2:{

            short where = 0;
            for (short ii = 0 ; ii < 2 ; ++ii ){
                for (short jj = 0 ; jj < 2 ; ++jj ){
                    for (short kk = 0 ; kk < 2 ; ++kk ){
                        std::vector<short> axis={ii,jj,kk};
                        for(short rst=0;rst<3;++rst){
                            switch(axis[rst]){
                                case 0:{
                                    gp_coords(where, rst) = -0.577350269189626;
                                    gp_weight(where, rst) = 1.0;
                                    break;
                                }
                                case 1:{
                                    gp_coords(where, rst) = 0.577350269189626;
                                    gp_weight(where, rst) = 1.0;
                                    break;
                                }
                            }
                        }
                        where ++;
                    }
                }
            }
            break;

        }

        // integration_order 3
        case 3:{


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
            break;

        }
    }





}


// void VariableNodeBrickLT::populate()
// {


//     if ( Num_TotalGaussPts != 0 )
//     {
//         matpoint  = new MatPoint3D * [Num_TotalGaussPts];
//     }
//     else
//     {
//         matpoint  = 0;
//     }


//     short where = 0;

//     for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//     {
//         double r = get_Gauss_p_c( integration_order, GP_c_r );
//         double rw = get_Gauss_p_w( integration_order, GP_c_r );

//         for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//         {
//             double s = get_Gauss_p_c( integration_order, GP_c_s );
//             double sw = get_Gauss_p_w( integration_order, GP_c_s );

//             for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//             {
//                 double t = get_Gauss_p_c( integration_order, GP_c_t );
//                 double tw = get_Gauss_p_w( integration_order, GP_c_t );

//                 where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;


//                 matpoint[where] = new MatPoint3D(GP_c_r, GP_c_s, GP_c_t,
//                                                  r, s, t,
//                                                  rw, sw, tw,
//                                                  mmodel);

//             }
//         }
//     }

//     initialized = true;


// }



//====================================================================
VariableNodeBrickLT::VariableNodeBrickLT()
    : Element(0, ELE_TAG_VariableNodeBrickLT),
      connectedExternalNodes(27), node_existance(19), rho(0.0), mmodel(0)
{

    integration_order = 3; // Gauss-Legendre integration order

    initialized = false;

    is_mass_computed = false;




    switch(integration_order){
        // integration_order 2
        case 2:{

            short where = 0;
            for (short ii = 0 ; ii < 2 ; ++ii ){
                for (short jj = 0 ; jj < 2 ; ++jj ){
                    for (short kk = 0 ; kk < 2 ; ++kk ){
                        std::vector<short> axis={ii,jj,kk};
                        for(short rst=0;rst<3;++rst){
                            switch(axis[rst]){
                                case 0:{
                                    gp_coords(where, rst) = -0.577350269189626;
                                    gp_weight(where, rst) = 1.0;
                                    break;
                                }
                                case 1:{
                                    gp_coords(where, rst) = 0.577350269189626;
                                    gp_weight(where, rst) = 1.0;
                                    break;
                                }
                            }
                        }
                        where ++;
                    }
                }
            }
            break;
        }

        // integration_order 3
        case 3:{


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
            break;
        }
    }













// # ifndef _PARALLEL_PROCESSING
//     populate();
// # endif


}


VariableNodeBrickLT::~VariableNodeBrickLT()
{

    for (int i = 0; i < Num_TotalGaussPts; i++)
    {
        // Delete the NDMaterialLTs at each integration point
        if ( material_array[i] )
        {
            delete material_array[i];
        }

    }

    // Delete the array of pointers to NDMaterialLT pointer arrays
    // if (material_array)
    // {
    //     delete [] material_array;
    // }

    if (K != 0)
    {
        delete K;
    }

    if (M != 0)
    {
        delete M;
    }

    if (P != 0)
    {
        delete P;
    }


    // Q = new Vector(nodes_in_brick * 3);
    if(Q!=0) delete Q;
    if(existing_node_numbers!=0) delete existing_node_numbers;
    // existing_node_numbers = new ID(nodes_in_brick);


}


// void VariableNodeBrickLT::incremental_Update()
// {

//     double r  = 0.0;
//     double s  = 0.0;
//     double t  = 0.0;

//     short where = 0;

//     int dh_dim[] = {27, 3};
//     tensor dh(2, dh_dim, 0.0);


//     static int disp_dim[] = {nodes_in_brick, 3};


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

//                 where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                 dh = dh_drst_at(r, s, t);

//                 Jacobian = Jacobian_3D(dh);

//                 JacobianINV = Jacobian_3Dinv(dh);

//                 dhGlobal = dh("ij") * JacobianINV("kj");

//                 incremental_strain = (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
//                 incremental_strain.null_indices();
//                 //                 incremental_strain.print("strain");


//                 if ( ( (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)) )
//                 {
//                     cerr << "EightNodeBrick::incremental_Update (tag: " << this->getTag() << "), not converged\n";
//                 }

//             }
//         }
//     }


// }


//***************************************************************
const DTensor2 &VariableNodeBrickLT::H_3D(double r1, double r2, double r3) const
{

    static DTensor2 H( 81, 3, 0.0 );
    


    //Shape Functions of Node 27 Along Three Coordinate Directions
    H(78, 0) = node_existance(27 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H(79, 1) = H(78, 0);
    H(80, 2) = H(78, 0);


    //Shape Functions of Node 26 Along Three Coordinate Directions
    H(75, 0) = node_existance(26 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H(76, 1) = H(75, 0);
    H(77, 2) = H(75, 0);


    //Shape Functions of Node 25 Along Three Coordinate Directions
    H(72, 0) = node_existance(25 - 1 - 8) * 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(73, 1) = H(72, 0);
    H(74, 2) = H(72, 0);


    //Shape Functions of Node 24 Along Three Coordinate Directions
    H(69, 0) = node_existance(24 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H(70, 1) = H(69, 0);
    H(71, 2) = H(69, 0);


    //Shape Functions of Node 23 Along Three Coordinate Directions
    H(66, 0) = node_existance(23 - 1 - 8) * 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(67, 1) = H(66, 0);
    H(68, 2) = H(66, 0);


    //Shape Functions of Node 22 Along Three Coordinate Directions
    H(63, 0) = node_existance(22 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H(64, 1) = H(63, 0);
    H(65, 2) = H(63, 0);


    //Shape Functions of Node 21 Along Three Coordinate Directions
    H(60, 0) = node_existance(21 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H(61, 1) = H(60, 0);
    H(62, 2) = H(60, 0);


    //Shape Functions of Node 20 Along Three Coordinate Directions
    H(57, 0) = node_existance(20 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (H(69, 0) + H(72, 0)) - 0.25 * H(60, 0) );
    H(58, 1) = H(57, 0);
    H(59, 2) = H(57, 0);


    //Shape Functions of Node 19 Along Three Coordinate Directions
    H(54, 0) = node_existance(19 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (H(66, 0) + H(69, 0)) - 0.25 * H(60, 0) );
    H(55, 1) = H(54, 0);
    H(56, 2) = H(54, 0);


    //Shape Functions of Node 18 Along Three Coordinate Directions
    H(51, 0) = node_existance(18 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (H(63, 0) + H(66, 0)) - 0.25 * H(60, 0) );
    H(52, 1) = H(51, 0);
    H(53, 2) = H(51, 0);


    //Shape Functions of Node 17 Along Three Coordinate Directions
    H(48, 0) = node_existance(17 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (H(63, 0) + H(72, 0)) - 0.25 * H(60, 0) );
    H(49, 1) = H(48, 0);
    H(50, 2) = H(48, 0);


    //Shape Functions of Node 16 Along Three Coordinate Directions
    H(45, 0) = node_existance(16 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (H(72, 0) + H(78, 0)) - 0.25 * H(60, 0) );
    H(46, 1) = H(45, 0);
    H(47, 2) = H(45, 0);


    //Shape Functions of Node 15 Along Three Coordinate Directions
    H(42, 0) = node_existance(15 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) - 0.5 * (H(69, 0) + H(78, 0)) - 0.25 * H(60, 0) );
    H(43, 1) = H(42, 0);
    H(44, 2) = H(42, 0);


    //Shape Functions of Node 14 Along Three Coordinate Directions
    H(39, 0) = node_existance(14 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (H(66, 0) + H(78, 0)) - 0.25 * H(60, 0) );
    H(40, 1) = H(39, 0);
    H(41, 2) = H(39, 0);


    //Shape Functions of Node 13 Along Three Coordinate Directions
    H(36, 0) = node_existance(13 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) - 0.5 * (H(63, 0) + H(78, 0)) - 0.25 * H(60, 0) );
    H(37, 1) = H(36, 0);
    H(38, 2) = H(36, 0);


    //Shape Functions of Node 12 Along Three Coordinate Directions
    H(33, 0) = node_existance(12 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (H(72, 0) + H(75, 0)) - 0.25 * H(60, 0) );
    H(34, 1) = H(33, 0);
    H(35, 2) = H(33, 0);


    //Shape Functions of Node 11 Along Three Coordinate Directions
    H(30, 0) = node_existance(11 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) - 0.5 * (H(69, 0) + H(75, 0)) - 0.25 * H(60, 0) );
    H(31, 1) = H(30, 0);
    H(32, 2) = H(30, 0);


    //Shape Functions of Node 10 Along Three Coordinate Directions
    H(27, 0) = node_existance(10 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (H(66, 0) + H(75, 0)) - 0.25 * H(60, 0) );
    H(28, 1) = H(27, 0);
    H(29, 2) = H(27, 0);


    //Shape Functions of Node 9 Along Three Coordinate Directions
    H(24, 0) = node_existance(9 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) - 0.5 * (H(63, 0) + H(75, 0)) - 0.25 * H(60, 0) );
    H(25, 1) = H(24, 0);
    H(26, 2) = H(24, 0);


    //Shape Functions of Node 1 Along Three Coordinate Directions
    H(0, 0) = 0.125 * (1 + r1) * (1 + r2) * (1 + r3) - 0.5 * ( H(24, 0) + H(33, 0) + H(48, 0) ) - 0.25 * ( H(63, 0) + H(72, 0) + H(75, 0) ) - 0.125 * H(60, 0);
    H(1, 1) = H(0, 0);
    H(2, 2) = H(0, 0);

    //Shape Functions of Node 2 Along Three Coordinate Directions
    H(3, 0) = 0.125 * (1 - r1) * (1 + r2) * (1 + r3) - 0.5 * ( H(24, 0) + H(27, 0) + H(51, 0) ) - 0.25 * ( H(63, 0) + H(66, 0) + H(75, 0) ) - 0.125 * H(60, 0);
    H(4, 1) = H(3, 0);
    H(5, 2) = H(3, 0);

    //Shape Functions of Node 3 Along Three Coordinate Directions
    H(6, 0) = 0.125 * (1 - r1) * (1 - r2) * (1 + r3) - 0.5 * ( H(27, 0) + H(30, 0) + H(54, 0) ) - 0.25 * ( H(66, 0) + H(69, 0) + H(75, 0) ) - 0.125 * H(60, 0);
    H(7, 1) = H(6, 0);
    H(8, 2) = H(6, 0);

    //Shape Functions of Node 4 Along Three Coordinate Directions
    H(9, 0) = 0.125 * (1 + r1) * (1 - r2) * (1 + r3) - 0.5 * ( H(30, 0) + H(33, 0) + H(57, 0) ) - 0.25 * ( H(69, 0) + H(72, 0) + H(75, 0) ) - 0.125 * H(60, 0);
    H(10, 1) = H(9, 0);
    H(11, 2) = H(9, 0);

    //Shape Functions of Node 5 Along Three Coordinate Directions
    H(12, 0) = 0.125 * (1 + r1) * (1 + r2) * (1 - r3) - 0.5 * ( H(36, 0) + H(45, 0) + H(48, 0) ) - 0.25 * ( H(63, 0) + H(72, 0) + H(78, 0) ) - 0.125 * H(60, 0);
    H(13, 1) = H(12, 0);
    H(14, 2) = H(12, 0);

    //Shape Functions of Node 6 Along Three Coordinate Directions
    H(15, 0) = 0.125 * (1 - r1) * (1 + r2) * (1 - r3) - 0.5 * ( H(36, 0) + H(39, 0) + H(51, 0) ) - 0.25 * ( H(63, 0) + H(66, 0) + H(78, 0) ) - 0.125 * H(60, 0);
    H(16, 1) = H(15, 0);
    H(17, 2) = H(15, 0);

    //Shape Functions of Node 7 Along Three Coordinate Directions
    H(18, 0) = 0.125 * (1 - r1) * (1 - r2) * (1 - r3) - 0.5 * ( H(39, 0) + H(42, 0) + H(54, 0) ) - 0.25 * ( H(66, 0) + H(69, 0) + H(78, 0) ) - 0.125 * H(60, 0);
    H(19, 1) = H(18, 0);
    H(20, 2) = H(18, 0);


    //Shape Functions of Node 8 Along Three Coordinate Directions
    H(21, 0) = 0.125 * (1 + r1) * (1 - r2) * (1 - r3) - 0.5 * ( H(42, 0) + H(45, 0) + H(57, 0) ) - 0.25 * ( H(69, 0) + H(72, 0) + H(78, 0) ) - 0.125 * H(60, 0);
    H(22, 1) = H(21, 0);
    H(23, 2) = H(21, 0);


    //     H.print("H","\n\n H \n");

    return H;

}



const DTensor2 &VariableNodeBrickLT::dh_drst_at(double r1, double r2, double r3) const
{
    static DTensor2 dh( 27, 3, 0.0 );

    // Node 21
    dh(20, 0) = node_existance(21 - 1 - 8) * (-2.0 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(20, 1) = node_existance(21 - 1 - 8) * (1.0 - r1 * r1) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(20, 2) = node_existance(21 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 22
    dh(21, 0) = node_existance(22 - 1 - 8) * (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh(21, 1) = node_existance(22 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh(21, 2) = node_existance(22 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    // Node 23
    dh(22, 0) = node_existance(23 - 1 - 8) * 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(22, 1) = node_existance(23 - 1 - 8) * 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(22, 2) = node_existance(23 - 1 - 8) * 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 24
    dh(23, 0) = node_existance(24 - 1 - 8) * (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh(23, 1) = node_existance(24 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh(23, 2) = node_existance(24 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    // Node 25
    dh(24, 0) = node_existance(25 - 1 - 8) * 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh(24, 1) = node_existance(25 - 1 - 8) * 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh(24, 2) = node_existance(25 - 1 - 8) * 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 26
    dh(25, 0) = node_existance(26 - 1 - 8) * (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(25, 1) = node_existance(26 - 1 - 8) * (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh(25, 2) = node_existance(26 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    // Node 27
    dh(26, 0) = node_existance(27 - 1 - 8) * (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(26, 1) = node_existance(27 - 1 - 8) * (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh(26, 2) = node_existance(27 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);


    // Node 20
    dh(19, 0) = node_existance(20 - 1 - 8) * ( 0.25 * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (dh(23, 0) + dh(24, 0)) - 0.25 * dh(20, 0) );
    dh(19, 1) = node_existance(20 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 + r1) * (1.0 - r3 * r3) - 0.5 * (dh(23, 1) + dh(24, 1)) - 0.25 * dh(20, 1) );
    dh(19, 2) = node_existance(20 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2) * (-2.0 * r3) - 0.5 * (dh(23, 2) + dh(24, 2)) - 0.25 * dh(20, 2) );


    // Node 19
    dh(18, 0) = node_existance(19 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (dh(22, 0) + dh(23, 0)) - 0.25 * dh(20, 0) );
    dh(18, 1) = node_existance(19 - 1 - 8) * ( 0.25 * (1.0 - r1) * (-1.0) * (1.0 - r3 * r3) - 0.5 * (dh(22, 1) + dh(23, 1)) - 0.25 * dh(20, 1) );
    dh(18, 2) = node_existance(19 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2) * (-2.0 * r3) - 0.5 * (dh(22, 2) + dh(23, 2)) - 0.25 * dh(20, 2) );


    // Node 18
    dh(17, 0) = node_existance(18 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (dh(21, 0) + dh(22, 0)) - 0.25 * dh(20, 0) );
    dh(17, 1) = node_existance(18 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r3 * r3) - 0.5 * (dh(21, 1) + dh(22, 1)) - 0.25 * dh(20, 1) );
    dh(17, 2) = node_existance(18 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 + r2) * (-2.0 * r3) - 0.5 * (dh(21, 2) + dh(22, 2)) - 0.25 * dh(20, 2) );


    // Node 17
    dh(16, 0) = node_existance(17 - 1 - 8) * ( 0.25 * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (dh(21, 0) + dh(24, 0)) - 0.25 * dh(20, 0) );
    dh(16, 1) = node_existance(17 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r3 * r3) - 0.5 * (dh(21, 1) + dh(24, 1)) - 0.25 * dh(20, 1) );
    dh(16, 2) = node_existance(17 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 + r2) * (-2.0 * r3) - 0.5 * (dh(21, 2) + dh(24, 2)) - 0.25 * dh(20, 2) );


    // Node 16
    dh(15, 0) = node_existance(16 - 1 - 8) * ( 0.25 * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (dh(24, 0) + dh(26, 0)) - 0.25 * dh(20, 0) );
    dh(15, 1) = node_existance(16 - 1 - 8) * ( 0.25 * (1.0 + r1) * (-2.0 * r2) * (1.0 - r3) - 0.5 * (dh(24, 1) + dh(26, 1)) - 0.25 * dh(20, 1) );
    dh(15, 2) = node_existance(16 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (-1.0) - 0.5 * (dh(24, 2) + dh(26, 2)) - 0.25 * dh(20, 2) );


    // Node 15
    dh(14, 0) = node_existance(15 - 1 - 8) * ( 0.25 * (-2.0 * r1) * (1.0 - r2) * (1.0 - r3) - 0.5 * (dh(23, 0) + dh(26, 0)) - 0.25 * dh(20, 0) );
    dh(14, 1) = node_existance(15 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (-1.0) * (1.0 - r3) - 0.5 * (dh(23, 1) + dh(26, 1)) - 0.25 * dh(20, 1) );
    dh(14, 2) = node_existance(15 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (-1.0) - 0.5 * (dh(23, 2) + dh(26, 2)) - 0.25 * dh(20, 2) );



    // Node 14
    dh(13, 0) = node_existance(14 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (dh(22, 0) + dh(26, 0)) - 0.25 * dh(20, 0) );
    dh(13, 1) = node_existance(14 - 1 - 8) * ( 0.25 * (1.0 - r1) * (-2.0 * r2) * (1.0 - r3) - 0.5 * (dh(22, 1) + dh(26, 1)) - 0.25 * dh(20, 1) );
    dh(13, 2) = node_existance(14 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (-1.0) - 0.5 * (dh(22, 2) + dh(26, 2)) - 0.25 * dh(20, 2) );


    // Node 13
    dh(12, 0) = node_existance(13 - 1 - 8) * ( 0.25 * (-2.0 * r1) * (1.0 + r2) * (1.0 - r3) - 0.5 * (dh(21, 0) + dh(26, 0)) - 0.25 * dh(20, 0) );
    dh(12, 1) = node_existance(13 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r3) - 0.5 * (dh(21, 1) + dh(26, 1)) - 0.25 * dh(20, 1) );
    dh(12, 2) = node_existance(13 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (-1.0) - 0.5 * (dh(21, 2) + dh(26, 2)) - 0.25 * dh(20, 2) );


    // Node 12
    dh(11, 0) = node_existance(12 - 1 - 8) * ( 0.25 * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (dh(24, 0) + dh(25, 0)) - 0.25 * dh(20, 0) );
    dh(11, 1) = node_existance(12 - 1 - 8) * ( 0.25 * (1.0 + r1) * (-2.0 * r2) * (1.0 + r3) - 0.5 * (dh(24, 1) + dh(25, 1)) - 0.25 * dh(20, 1) );
    dh(11, 2) = node_existance(12 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) - 0.5 * (dh(24, 2) + dh(25, 2)) - 0.25 * dh(20, 2) );


    // Node 11
    dh(10, 0) = node_existance(11 - 1 - 8) * ( 0.25 * (-2.0 * r1) * (1.0 - r2) * (1.0 + r3) - 0.5 * (dh(23, 0) + dh(25, 0)) - 0.25 * dh(20, 0) );
    dh(10, 1) = node_existance(11 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (-1.0) * (1.0 + r3) - 0.5 * (dh(23, 1) + dh(25, 1)) - 0.25 * dh(20, 1) );
    dh(10, 2) = node_existance(11 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) - 0.5 * (dh(23, 2) + dh(25, 2)) - 0.25 * dh(20, 2) );


    // Node 10
    dh(9, 0) = node_existance(10 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (dh(22, 0) + dh(25, 0)) - 0.25 * dh(20, 0) );
    dh(9, 1) = node_existance(10 - 1 - 8) * ( 0.25 * (1.0 - r1) * (-2.0 * r2) * (1.0 + r3) - 0.5 * (dh(22, 1) + dh(25, 1)) - 0.25 * dh(20, 1) );
    dh(9, 2) = node_existance(10 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) - 0.5 * (dh(22, 2) + dh(25, 2)) - 0.25 * dh(20, 2) );


    // Node 9
    dh(8, 0) = node_existance(9 - 1 - 8) * ( 0.25 * (-2.0 * r1) * (1.0 + r2) * (1.0 + r3) - 0.5 * (dh(21, 0) + dh(25, 0)) - 0.25 * dh(20, 0) );
    dh(8, 1) = node_existance(9 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r3) - 0.5 * (dh(21, 1) + dh(25, 1)) - 0.25 * dh(20, 1) );
    dh(8, 2) = node_existance(9 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) - 0.5 * (dh(21, 2) + dh(25, 2)) - 0.25 * dh(20, 2) );


    // Node 1
    dh(0, 0) = 0.125 * (1 + r2) * (1 + r3) - 0.5 * ( dh(8, 0) + dh(11, 0) + dh(16, 0) ) - 0.25 * ( dh(21, 0) + dh(24, 0) + dh(25, 0) ) - 0.125 * dh(20, 0);
    dh(0, 1) = 0.125 * (1 + r1) * (1 + r3) - 0.5 * ( dh(8, 1) + dh(11, 1) + dh(16, 1) ) - 0.25 * ( dh(21, 1) + dh(24, 1) + dh(25, 1) ) - 0.125 * dh(20, 1);
    dh(0, 2) = 0.125 * (1 + r1) * (1 + r2) - 0.5 * ( dh(8, 2) + dh(11, 2) + dh(16, 2) ) - 0.25 * ( dh(21, 2) + dh(24, 2) + dh(25, 2) ) - 0.125 * dh(20, 2);


    // Node 2
    dh(1, 0) = 0.125 * (-1.0) * (1 + r2) * (1 + r3) - 0.5 * ( dh(8, 0) + dh(9, 0) + dh(17, 0) ) - 0.25 * ( dh(21, 0) + dh(22, 0) + dh(25, 0) ) - 0.125 * dh(20, 0);
    dh(1, 1) = 0.125 * (1 - r1) * (1 + r3) - 0.5 * ( dh(8, 1) + dh(9, 1) + dh(17, 1) ) - 0.25 * ( dh(21, 1) + dh(22, 1) + dh(25, 1) ) - 0.125 * dh(20, 1);
    dh(1, 2) = 0.125 * (1 - r1) * (1 + r2) - 0.5 * ( dh(8, 2) + dh(9, 2) + dh(17, 2) ) - 0.25 * ( dh(21, 2) + dh(22, 2) + dh(25, 2) ) - 0.125 * dh(20, 2);


    // Node 3
    dh(2, 0) = 0.125 * (-1.0) * (1 - r2) * (1 + r3) - 0.5 * ( dh(9, 0) + dh(10, 0) + dh(18, 0) ) - 0.25 * ( dh(22, 0) + dh(23, 0) + dh(25, 0) ) - 0.125 * dh(20, 0);
    dh(2, 1) = 0.125 * (1 - r1) * (-1.0) * (1 + r3) - 0.5 * ( dh(9, 1) + dh(10, 1) + dh(18, 1) ) - 0.25 * ( dh(22, 1) + dh(23, 1) + dh(25, 1) ) - 0.125 * dh(20, 1);
    dh(2, 2) = 0.125 * (1 - r1) * (1 - r2) - 0.5 * ( dh(9, 2) + dh(10, 2) + dh(18, 2) ) - 0.25 * ( dh(22, 2) + dh(23, 2) + dh(25, 2) ) - 0.125 * dh(20, 2);


    // Node 4
    dh(3, 0) = 0.125 * (1 - r2) * (1 + r3) - 0.5 * ( dh(10, 0) + dh(11, 0) + dh(19, 0) ) - 0.25 * ( dh(23, 0) + dh(24, 0) + dh(25, 0) ) - 0.125 * dh(20, 0);
    dh(3, 1) = 0.125 * (1 + r1) * (-1.0) * (1 + r3) - 0.5 * ( dh(10, 1) + dh(11, 1) + dh(19, 1) ) - 0.25 * ( dh(23, 1) + dh(24, 1) + dh(25, 1) ) - 0.125 * dh(20, 1);
    dh(3, 2) = 0.125 * (1 + r1) * (1 - r2) - 0.5 * ( dh(10, 2) + dh(11, 2) + dh(19, 2) ) - 0.25 * ( dh(23, 2) + dh(24, 2) + dh(25, 2) ) - 0.125 * dh(20, 2);


    // Node 5
    dh(4, 0) = 0.125 * (1 + r2) * (1 - r3) - 0.5 * ( dh(12, 0) + dh(15, 0) + dh(16, 0) ) - 0.25 * ( dh(21, 0) + dh(24, 0) + dh(26, 0) ) - 0.125 * dh(20, 0);
    dh(4, 1) = 0.125 * (1 + r1) * (1 - r3) - 0.5 * ( dh(12, 1) + dh(15, 1) + dh(16, 1) ) - 0.25 * ( dh(21, 1) + dh(24, 1) + dh(26, 1) ) - 0.125 * dh(20, 1);
    dh(4, 2) = 0.125 * (1 + r1) * (1 + r2) * (-1.0) - 0.5 * ( dh(12, 2) + dh(15, 2) + dh(16, 2) ) - 0.25 * ( dh(21, 2) + dh(24, 2) + dh(26, 2) ) - 0.125 * dh(20, 2);


    // Node 6
    dh(5, 0) = 0.125 * (-1.0) * (1 + r2) * (1 - r3) - 0.5 * ( dh(12, 0) + dh(13, 0) + dh(17, 0) ) - 0.25 * ( dh(21, 0) + dh(22, 0) + dh(26, 0) ) - 0.125 * dh(20, 0);
    dh(5, 1) = 0.125 * (1 - r1) * (1 - r3) - 0.5 * ( dh(12, 1) + dh(13, 1) + dh(17, 1) ) - 0.25 * ( dh(21, 1) + dh(22, 1) + dh(26, 1) ) - 0.125 * dh(20, 1);
    dh(5, 2) = 0.125 * (1 - r1) * (1 + r2) * (-1.0) - 0.5 * ( dh(12, 2) + dh(13, 2) + dh(17, 2) ) - 0.25 * ( dh(21, 2) + dh(22, 2) + dh(26, 2) ) - 0.125 * dh(20, 2);


    // Node 7
    dh(6, 0) = 0.125 * (-1.0) * (1 - r2) * (1 - r3) - 0.5 * ( dh(13, 0) + dh(14, 0) + dh(18, 0) ) - 0.25 * ( dh(22, 0) + dh(23, 0) + dh(26, 0) ) - 0.125 * dh(20, 0);
    dh(6, 1) = 0.125 * (1 - r1) * (-1.0) * (1 - r3) - 0.5 * ( dh(13, 1) + dh(14, 1) + dh(18, 1) ) - 0.25 * ( dh(22, 1) + dh(23, 1) + dh(26, 1) ) - 0.125 * dh(20, 1);
    dh(6, 2) = 0.125 * (1 - r1) * (1 - r2) * (-1.0) - 0.5 * ( dh(13, 2) + dh(14, 2) + dh(18, 2) ) - 0.25 * ( dh(22, 2) + dh(23, 2) + dh(26, 2) ) - 0.125 * dh(20, 2);


    // Node 8
    dh(7, 0) = 0.125 * (1 - r2) * (1 - r3) - 0.5 * ( dh(14, 0) + dh(15, 0) + dh(19, 0) ) - 0.25 * ( dh(23, 0) + dh(24, 0) + dh(26, 0) ) - 0.125 * dh(20, 0);
    dh(7, 1) = 0.125 * (1 + r1) * (-1.0) * (1 - r3) - 0.5 * ( dh(14, 1) + dh(15, 1) + dh(19, 1) ) - 0.25 * ( dh(23, 1) + dh(24, 1) + dh(26, 1) ) - 0.125 * dh(20, 1);
    dh(7, 2) = 0.125 * (1 + r1) * (1 - r2) * (-1.0) - 0.5 * ( dh(14, 2) + dh(15, 2) + dh(19, 2) ) - 0.25 * ( dh(23, 2) + dh(24, 2) + dh(26, 2) ) - 0.125 * dh(20, 2);


    //             dh.print("dh");

    return dh;
}

////#############################################################################
VariableNodeBrickLT &VariableNodeBrickLT::operator[](int subscript)
{
    return ( *(this + subscript) );
}


////#############################################################################
const DTensor4 &VariableNodeBrickLT::getStiffnessTensor(void) const
{
    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    // all use 27 Gauss points here in this function.
    // the node_exist cases were discussed in other functions.
    DTensor2 dh_drst( 27, 3, 0.0 ); 
    DTensor2 dhGlobal( 27, 3, 0.0 );
    DTensor2 Jacobian(3, 3, 0.0);
    DTensor2 JacobianINV(3, 3, 0.0);
    DTensor4 E_elpl(3, 3, 3, 3, 0.0);
    DTensor4 Kkt( 27, 3, 3, 27, 0.0);
    static DTensor4 Kk( 27, 3, 3, 27, 0.0);

    Index < 'a' > a;
    Index < 'b' > b;
    Index < 'c' > c;
    Index < 'd' > d;

    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp,0);
        w_s = gp_weight(gp,1);
        w_t = gp_weight(gp,2);

        // derivatives of local coordinates with respect to local coordinates
        dh_drst         = dh_drst_at( r, s, t );
        Jacobian        = Jacobian_3D( dh_drst );
        JacobianINV     = Jacobian.Inv();
        det_of_Jacobian = Jacobian.compute_Determinant();

        // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
        dhGlobal(i, k)  = dh_drst( i, j ) * JacobianINV( k, j );
        weight          = w_r * w_s * w_t * det_of_Jacobian;
        E_elpl          = material_array[gp]->getTangentTensor();

        // FIXME: This can be more efficient if these lines are merged into one (discard the temporary KKt tensor)
        Kkt(i, a, c, j) = dhGlobal(i, b) * E_elpl(a, b, c, d) * dhGlobal(j, d) * weight;
        Kk(i, a, c, j) = Kk(i, a, c, j) + Kkt(i, a, c, j);
    }

    return Kk;


}


//#############################################################################

// void VariableNodeBrickLT::set_strain_stress_tensor(FILE *fp, double *u)
// {

//     int dh_dim[] = {27, 3};
//     tensor dh(2, dh_dim, 0.0);

//     tensor Constitutive;
//     double r  = 0.0;
//     double s  = 0.0;
//     double t  = 0.0;
//     int where = 0;


//     straintensor strain;
//     stresstensor stress;

//     tensor Jacobian;
//     tensor JacobianINV;
//     tensor dhGlobal;


//     static int disp_dim[] = {27, 3};

//     tensor total_displacements(2, disp_dim, 0.0);

//     total_displacements = total_disp(fp, u);

//     ::printf("\n  displacement(x-y-z) at GAUSS pt %d \n\n", where + 1);

//     for (int ii = 1; ii <= nodes_in_brick; ii++)
//     {
//         ::printf("Global# %d Local#%d  %+0.5e %+0.5e %+0.5e\n",

//                  connectedExternalNodes(ii - 1),
//                  ii, total_displacements.val(ii, 1),
//                  total_displacements.val(ii, 2),
//                  total_displacements.val(ii, 3));
//     }

//     for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//     {
//         r = get_Gauss_p_c( integration_order, GP_c_r );

//         for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//         {
//             s = get_Gauss_p_c( integration_order, GP_c_s );

//             for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//             {
//                 t = get_Gauss_p_c( integration_order, GP_c_t );

//                 where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                 dh = dh_drst_at(r, s, t);

//                 Jacobian = Jacobian_3D(dh);

//                 JacobianINV = Jacobian_3Dinv(dh);

//                 dhGlobal = dh("ij") * JacobianINV("kj");

//                 strain = (dhGlobal("ib") * total_displacements("ia")).symmetrize11();
//                 strain.null_indices();

//                 Constitutive =  ( matpoint[where]->matmodel)->getTangentTensor();

//                 stress = Constitutive("ijkl") * strain("kl");
//                 stress.null_indices();

//                 ::printf("\n  strain tensor at GAUSS point %d \n", where + 1);
//                 strain.reportshort("");
//                 ::printf("\n  stress tensor at GAUSS point %d \n", where + 1);
//                 stress.reportshort("");


//             }
//         }
//     }

// }



// tensor VariableNodeBrickLT::stiffness_matrix(const tensor &K)
// {

//     matrix Kmatrix(nodes_in_brick * 3, nodes_in_brick * 3, 0.0);

//     int Ki = 0;
//     int Kj = 0;

//     for ( int i = 1 ; i <= nodes_in_brick ; i++)
//     {
//         for ( int j = 1 ; j <= nodes_in_brick ; j++)
//         {
//             for ( int k = 1 ; k <= 3 ; k++)
//             {
//                 for ( int l = 1 ; l <= 3 ; l++)
//                 {
//                     Ki = k + 3 * (i - 1);
//                     Kj = l + 3 * (j - 1);


//                     Kmatrix.val( Ki , Kj ) = K.cval(i, k, l, j);
//                 }
//             }
//         }
//     }

//     return Kmatrix;

// }



// tensor VariableNodeBrickLT::mass_matrix(const tensor &M)
// {

//     matrix Mmatrix(nodes_in_brick * 3, nodes_in_brick * 3, 0.0);

//     for ( int i = 1 ; i <= nodes_in_brick * 3 ; i++)
//     {
//         for ( int j = 1 ; j <= nodes_in_brick * 3 ; j++)
//         {
//             Mmatrix.val(i, j) = M.cval(i, j);

//         }
//     }

//     return Mmatrix;

// }



const DTensor2 &VariableNodeBrickLT::Jacobian_3D(const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    static DTensor2 Jacobian_3D_(3, 3, 0.0);
    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    return Jacobian_3D_;
}


const DTensor2 &VariableNodeBrickLT::Jacobian_3Dinv(const DTensor2 &dh ) const
{
    const DTensor2 &N_C = Nodal_Coordinates();
    DTensor2 Jacobian_3D_(3, 3, 0.0);
    static DTensor2 Jacobian_3D_inv(3, 3, 0.0);

    Jacobian_3D_(j, k) = dh(i, j) * N_C(i, k);
    Jacobian_3D_inv = Jacobian_3D_.Inv();
    return Jacobian_3D_inv;
}



const DTensor2 VariableNodeBrickLT::Nodal_Coordinates(void) const
{
    static DTensor2 N_coord( 27, 3, 0.0 );

    for ( int i = 0 ; i < 27 ; i++)
    {
        if (theNodes[i] != 0)
        {
            N_coord(i , 0) = ( theNodes[i]->getCrds() )(0);
            N_coord(i , 1) = ( theNodes[i]->getCrds() )(1);
            N_coord(i , 2) = ( theNodes[i]->getCrds() )(2);
        }
        else
        {
            N_coord(i , 0) = 0;
            N_coord(i , 1) = 0;
            N_coord(i , 2) = 0;
        }
    }

    return N_coord;
}



////#############################################################################
const DTensor2 &VariableNodeBrickLT::incr_disp(void) const
{

    static DTensor2 increment_disp( 27, 3, 0.0 );

    for ( int i = 0 ; i < 27 ; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            increment_disp(i , 0) = ( theNodes[i]->getIncrDeltaDisp() )(0);
            increment_disp(i , 1) = ( theNodes[i]->getIncrDeltaDisp() )(1);
            increment_disp(i , 2) = ( theNodes[i]->getIncrDeltaDisp() )(2);
        }
        else
        {
            increment_disp(i , 0) = 0;
            increment_disp(i , 1) = 0;
            increment_disp(i , 2) = 0;
        }
    }

    return increment_disp;


}

////#############################################################################
const DTensor2 &VariableNodeBrickLT::total_disp(void) const
{
    static DTensor2 total_disp( nodes_in_brick, 3, 0.0 );

    for ( int i = 0 ; i < nodes_in_brick ; i++)
    {
        total_disp(i , 0) = ( theNodes[i]->getTrialDisp() )(0);
        total_disp(i , 1) = ( theNodes[i]->getTrialDisp() )(1);
        total_disp(i , 2) = ( theNodes[i]->getTrialDisp() )(2);
    }

    return total_disp;

}





////#############################################################################
// const DTensor2 &VariableNodeBrickLT::total_disp(FILE *fp, double *u) const
// {

//     DTensor2 total_disp( nodes_in_brick, 3, 0.0 );

//     for ( int i = 0 ; i < nodes_in_brick ; i++)
//     {
//         total_disp(i, 0) = ( theNodes[i]->getTrialDisp() )(0);
//         total_disp(i, 1) = ( theNodes[i]->getTrialDisp() )(1);
//         total_disp(i, 2) = ( theNodes[i]->getTrialDisp() )(2);
//     }

//     return total_disp;

// }


////#############################################################################
int VariableNodeBrickLT::get_global_number_of_node(int local_node_number)
{

    return (*existing_node_numbers)(local_node_number);

}

////#############################################################################
int  VariableNodeBrickLT::get_Brick_Number(void)
{

    return this->getTag();

}

////#############################################################################
const DTensor2 VariableNodeBrickLT::nodal_forces(void) const
{

    double r  = 0.0;
    double w_r = 0.0;
    double s  = 0.0;
    double w_s = 0.0;
    double t  = 0.0;
    double w_t = 0.0;
    double det_of_Jacobian = 0.0;
    double weight = 0.0;

    DTensor2 nodal_forces( Num_TotalGaussPts, 3, 0.0 );
    DTensor2 dh( Num_TotalGaussPts, 3 , 0.0 );
    DTensor2 stress_at_GP( 3, 3, 0.0 );
    DTensor2 Jacobian( 3, 3, 0.0 );
    DTensor2 JacobianINV( 3, 3, 0.0 );
    DTensor2 dhGlobal( Num_TotalGaussPts, 3 , 0.0 );

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




// tensor VariableNodeBrickLT::iterative_nodal_forces(void)
// {

//     int force_dim[] = {nodes_in_brick, 3};

//     tensor nodal_forces(2, force_dim, 0.0);

//     double r  = 0.0;
//     double rw = 0.0;
//     double s  = 0.0;
//     double sw = 0.0;
//     double t  = 0.0;
//     double tw = 0.0;

//     short where = 0;
//     double weight = 0.0;

//     int dh_dim[] = {27, 3};

//     tensor dh(2, dh_dim, 0.0);

//     stresstensor stress_at_GP(0.0);

//     double det_of_Jacobian = 0.0;

//     tensor Jacobian;
//     tensor JacobianINV;
//     tensor dhGlobal;

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

//                 // this short routine is supposed to calculate position of
//                 // Gauss point from 3D array of short's
//                 where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                 // derivatives of local coordiantes with respect to local coordiantes
//                 dh = dh_drst_at(r, s, t);

//                 // Jacobian tensor ( matrix )
//                 Jacobian = Jacobian_3D(dh);

//                 // Inverse of Jacobian tensor ( matrix )
//                 JacobianINV = Jacobian_3Dinv(dh);

//                 // determinant of Jacobian tensor ( matrix )
//                 det_of_Jacobian  = Jacobian.determinant();

//                 // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
//                 dhGlobal = dh("ij") * JacobianINV("kj");

//                 weight = rw * sw * tw * det_of_Jacobian;

//                 stress_at_GP = matpoint[where]->getStressTensor();
//                 stress_at_GP.reportshortpqtheta("\n iterative_stress at GAUSS point in iterative_nodal_force\n");

//                 // nodal forces See Zienkievicz part 1 pp 108
//                 nodal_forces = nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;

//             }
//         }
//     }


//     return nodal_forces;

// }




// tensor VariableNodeBrickLT::nodal_forces_from_stress(stresstensor &stress)
// {
//     int force_dim[] = {nodes_in_brick, 3};

//     tensor nodal_forces(2, force_dim, 0.0);

//     double r  = 0.0;
//     double rw = 0.0;
//     double s  = 0.0;
//     double sw = 0.0;
//     double t  = 0.0;
//     double tw = 0.0;

//     double weight = 0.0;

//     int dh_dim[] = {27, 3};

//     tensor dh(2, dh_dim, 0.0);

//     double det_of_Jacobian = 0.0;

//     tensor Jacobian;
//     tensor JacobianINV;
//     tensor dhGlobal;

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

//                 // derivatives of local coordiantes with respect to local coordiantes
//                 dh = dh_drst_at(r, s, t);

//                 // Jacobian tensor ( matrix )
//                 Jacobian = Jacobian_3D(dh);

//                 // Inverse of Jacobian tensor ( matrix )
//                 JacobianINV = Jacobian_3Dinv(dh);

//                 // determinant of Jacobian tensor ( matrix )
//                 det_of_Jacobian  = Jacobian.determinant();

//                 // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
//                 dhGlobal = dh("ij") * JacobianINV("kj");

//                 weight = rw * sw * tw * det_of_Jacobian;


//                 // nodal forces See Zienkievicz part 1 pp 108
//                 nodal_forces = nodal_forces + dhGlobal("ib") * stress("ab") * weight;

//             }
//         }
//     }

//     return nodal_forces;

// }




// tensor VariableNodeBrickLT::linearized_nodal_forces(void)
// {
//     int force_dim[] = {nodes_in_brick, 3};

//     tensor linearized_nodal_forces(2, force_dim, 0.0);

//     double r  = 0.0;
//     double rw = 0.0;
//     double s  = 0.0;
//     double sw = 0.0;
//     double t  = 0.0;
//     double tw = 0.0;

//     short where = 0;
//     double weight = 0.0;

//     int dh_dim[] = {27, 3};

//     tensor dh(2, dh_dim, 0.0);

//     tensor Constitutive( 4, def_dim_4, 0.0);

//     double det_of_Jacobian = 0.0;

//     static int disp_dim[] = {nodes_in_brick, 3};

//     tensor incremental_displacements(2, disp_dim, 0.0);

//     straintensor incremental_strain;

//     tensor Jacobian;
//     tensor JacobianINV;
//     tensor dhGlobal;

//     stresstensor final_linearized_stress;
//     incremental_displacements = incr_disp();

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

//                 // this short routine is supposed to calculate position of
//                 // Gauss point from 3D array of short's
//                 where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                 // derivatives of local coordiantes with respect to local coordiantes
//                 dh = dh_drst_at(r, s, t);

//                 // Jacobian tensor ( matrix )
//                 Jacobian = Jacobian_3D(dh);

//                 // Inverse of Jacobian tensor ( matrix )
//                 JacobianINV = Jacobian_3Dinv(dh);

//                 // determinant of Jacobian tensor ( matrix )
//                 det_of_Jacobian  = Jacobian.determinant();

//                 // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
//                 dhGlobal = dh("ij") * JacobianINV("kj");

//                 weight = rw * sw * tw * det_of_Jacobian;

//                 // incremental straines at this Gauss point
//                 // now in Update we know the incremental displacements so let's find
//                 // the incremental strain
//                 incremental_strain = (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
//                 incremental_strain.null_indices();

//                 if ( ! (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)  )
//                 {
//                     cerr << "VariableNodeBrickLT::linearized_nodal_forces (tag: " << this->getTag() << "), not converged\n";
//                 }

//                 Constitutive = (matpoint[where]->matmodel)->getTangentTensor();

//                 final_linearized_stress =
//                     Constitutive("ijkl") * incremental_strain("kl");

//                 // nodal forces See Zienkievicz part 1 pp 108
//                 linearized_nodal_forces = linearized_nodal_forces +
//                                           dhGlobal("ib") * final_linearized_stress("ab") * weight;

//             }
//         }
//     }


//     return linearized_nodal_forces;

// }












// const DTensor2 &VariableNodeBrickLT::Nodal_Coordinates(void) const
// {
//     DTensor2 N_coord( 27, 3, 0.0 );

//     for ( int i = 0 ; i < 27 ; i++)
//     {
//         if (theNodes[i] != 0)
//         {
//             N_coord(i , 0) = ( theNodes[i]->getCrds() )(0);
//             N_coord(i , 1) = ( theNodes[i]->getCrds() )(1);
//             N_coord(i , 2) = ( theNodes[i]->getCrds() )(2);
//         }
//         else
//         {
//             N_coord(i , 0) = 0;
//             N_coord(i , 1) = 0;
//             N_coord(i , 2) = 0;
//         }
//     }

//     return N_coord;
// }

void VariableNodeBrickLT::computeGaussPoint()
{

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    DTensor2 NodalCoord( 3, nodes_in_brick, 0.0 );
    DTensor2 material_arrayCoord( 3, Num_TotalGaussPts, 0.0 );
    DTensor2 H( 81, 3, 0.0 );
    

    // use this instead of the for loop below?
    // NodalCoord=Nodal_Coordinates()
    for ( int i = 0 ; i < Num_TotalGaussPts ; i++)
    {
        NodalCoord(i, 0) = ( theNodes[i]->getCrds() )(0);
        NodalCoord(i, 1) = ( theNodes[i]->getCrds() )(1);
        NodalCoord(i, 2) = ( theNodes[i]->getCrds() )(2);
    }





    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);

        // derivatives of local coordinates with respect to local coordinates
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




// void VariableNodeBrickLT::reportTensorF(FILE *fp)
// {

//     double r  = 0.0;
//     double s  = 0.0;
//     double t  = 0.0;

//     short where = 0;

//     int count;
//     count = integration_order * integration_order * integration_order;


//     static const int dim[] = {3, nodes_in_brick};
//     static const int dim27[] = {3, count};
//     tensor NodalCoord(2, dim, 0.0);
//     tensor matpointCoord(2, dim27, 0.0);
//     int h_dim[] = {81, 3};
//     tensor H(2, h_dim, 0.0);


//     for ( int i = 0 ; i < nodes_in_brick ; i++)
//     {
//         NodalCoord.val(i + 1, 1) = ( theNodes[i]->getCrds() )(0);
//         NodalCoord.val(i + 1, 2) = ( theNodes[i]->getCrds() )(1);
//         NodalCoord.val(i + 1, 3) = ( theNodes[i]->getCrds() )(2);
//     }



//     for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//     {
//         r = get_Gauss_p_c( integration_order, GP_c_r );

//         for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//         {
//             s = get_Gauss_p_c( integration_order, GP_c_s );

//             for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//             {
//                 t = get_Gauss_p_c( integration_order, GP_c_t );

//                 // this short routine is supposed to calculate position of
//                 // Gauss point from 3D array of short's
//                 where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                 // derivatives of local coordinates with respect to local coordinates

//                 H = H_3D(r, s, t);

//                 for (int encount = 1 ; encount <= nodes_in_brick ; encount++ )
//                 {
//                     matpointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
//                     matpointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
//                     matpointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);
//                 }

//                 fprintf(fp, "gauss point# %d   %+.6e %+.6e %+.6e \n", where + 1,
//                         matpointCoord.val(1, where + 1),
//                         matpointCoord.val(2, where + 1),
//                         matpointCoord.val(3, where + 1));


//             }
//         }
//     }

// }



//=============================================================================
int VariableNodeBrickLT::getNumExternalNodes () const
{
    return nodes_in_brick;
}


//=============================================================================
const ID &VariableNodeBrickLT::getExternalNodes ()
{
    return *existing_node_numbers;
}


Node **
VariableNodeBrickLT::getNodePtrs(void)
{
    return theNodes;
}

//=============================================================================
int VariableNodeBrickLT::getNumDOF ()
{
    return 3 * nodes_in_brick;
}

//=============================================================================
void VariableNodeBrickLT::setDomain (Domain *theDomain)
{
    // Check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        for (int i = 0; i < 27; i++)
        {
            theNodes[i] = 0;
        }
    }
    else
    {
        for (int i = 0; i < 27; i++)
        {
            if (connectedExternalNodes(i) != 0)
            {
                if (theNodes[i] == 0)
                {
                    theNodes[i] = theDomain->getNode( connectedExternalNodes(i) );
                }
            }
        }


# ifdef _PARALLEL_PROCESSING
        for (int i = 0; i < 27; i++)
        {
            if (connectedExternalNodes(i) != 0)
            {
                if (theNodes[i] == 0)
                {
                    theNodes[i] = theDomain->getOutsideNode( connectedExternalNodes(i) );
                }
            }
        }
#endif


        for (int i = 0; i < 27; i++)
        {
            if (connectedExternalNodes(i) != 0)
            {
                if (theNodes[i] == 0)
                {
                    cerr << "VariableNodeBrickLT (tag: " << this->getTag() << " ) does not have node " <<  (*existing_node_numbers)(i) << "\n";
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
                    cerr << "VariableNodeBrickLT (tag: " << this->getTag() << ") has differing number of DOFs at its nodes\n";
                    exit(-1);
                }
            }
        }

        this->DomainComponent::setDomain(theDomain);
    }

    ComputeVolume();
    computeGaussPoint();
}

//=============================================================================
int VariableNodeBrickLT::commitState ()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "VariableNodeBrickLT::commitState () - failed in base class";
    }


    Vector pp = getResistingForce();


    for (int ii = 0; ii < Num_TotalGaussPts; ii++ )
    {
        retVal += material_array[ii]->commitState();
    }


    return retVal;

}

//=============================================================================
int VariableNodeBrickLT::revertToLastCommit ()
{
    int i;
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states

    for (i = 0; i < Num_TotalGaussPts; i++)
    {
        retVal += material_array[i]->revertToLastCommit();
    }


    return retVal;
}

//=============================================================================
int VariableNodeBrickLT::revertToStart ()
{
    int retVal = 0;

    for (int i = 0; i < Num_TotalGaussPts; i++)
    {
        retVal += material_array[i]->revertToStart();
    }


    return retVal;
}


//=============================================================================
const Matrix &VariableNodeBrickLT::getTangentStiff()
{
    
    DTensor4 stifftensor(8, 3, 3, 8, 0.0);

    stifftensor = getStiffnessTensor();


    (*K).Zero();

    int Ki = 0;
    int Kj = 0;

    Matrix K_temp(27 * 3, 27 * 3);
    K_temp.Zero();


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
                    (K_temp)( Ki - 1 , Kj - 1 ) = stifftensor(i-1, k-1, l-1, j-1);
                }
            }
        }
    }



    // filling the reduced size stiffness matrix

    int a = 1;
    int b = 0;


    for ( int m = 1 ; m <= 27 ; m++)
    {

        if (connectedExternalNodes(m - 1) != 0)
        {

            b = 1;

            for (int n = 1; n <= 27; n++)
            {

                if (connectedExternalNodes(n - 1) != 0)
                {
                    int x = 3 * a - 2;
                    int y = 3 * b - 2;
                    int f = 3 * m - 2;
                    int g = 3 * n - 2;

                    (*K)(x - 1 , y - 1) = K_temp(f - 1 , g - 1);
                    (*K)(x - 1 , y - 0) = K_temp(f - 1 , g - 0);
                    (*K)(x - 1 , y + 1) = K_temp(f - 1 , g + 1);

                    (*K)(x - 0 , y - 1) = K_temp(f - 0 , g - 1);
                    (*K)(x - 0 , y - 0) = K_temp(f - 0 , g - 0);
                    (*K)(x - 0 , y + 1) = K_temp(f - 0 , g + 1);

                    (*K)(x + 1 , y - 1) = K_temp(f + 1 , g - 1);
                    (*K)(x + 1 , y - 0) = K_temp(f + 1 , g - 0);
                    (*K)(x + 1 , y + 1) = K_temp(f + 1 , g + 1);

                    b++;


                }

            }

            a++;

        }
    }


    return *K;
}

//=============================================================================
const Matrix &VariableNodeBrickLT::getInitialStiff ()
{
    if ( Ki == 0 )
    {
        Ki = new Matrix( this->getTangentStiff() );
    }

    if ( Ki == 0 )
    {
        cerr << "FATAL VariableNodeBrickLT::getInitialStiff() -";
        cerr << "ran out of memory\n";
        exit( -1 );
    }

    return *Ki;

    // return this->getTangentStiff();
}


//=============================================================================
const Matrix VariableNodeBrickLT::getMassFullSize()
{
    double r   = 0.0;
    double s   = 0.0;
    double t   = 0.0;
    double w_r = 0.0;
    double w_s = 0.0;
    double w_t = 0.0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    DTensor2 dh( 27, 3, 0.0 );
    DTensor2 H(81, 3, 0.0 );
    DTensor2 Mm( 81, 81, 0.0 );
    DTensor2 Jacobian(3, 3);

    for ( short gp = 0; gp < Num_TotalGaussPts; gp++ )
    {
        r = gp_coords(gp, 0);
        s = gp_coords(gp, 1);
        t = gp_coords(gp, 2);
        w_r = gp_weight(gp,0);
        w_s = gp_weight(gp,1);
        w_t = gp_weight(gp,2);
        dh = dh_drst_at( r, s, t );
        Jacobian = Jacobian_3D( dh );
        det_of_Jacobian  = Jacobian.compute_Determinant();
        H = H_3D( r, s, t );

        weight = w_r * w_s * w_t * rho * det_of_Jacobian;
        Mm(i, j) = Mm(i, j) + H(i, k) * H(j, k) * weight;
    }
    Matrix M_temp(81,81);

    for ( int i = 0 ; i < 27 * 3 ; i++ )
    {
        for ( int j = 0 ; j < 27 * 3 ; j++ )
        {
            M_temp( i , j ) = Mm(i, j);
        }
    }


    return M_temp;

}

//=============================================================================
const Matrix &VariableNodeBrickLT::getMass()
{
    if(is_mass_computed) return *M;
    else{
        Matrix M_temp(27 * 3, 27 * 3);
        M_temp = getMassFullSize();
        // filling the reduced size stiffness matrix
        int a = 1;
        int b = 0;
        for ( int m = 1 ; m <= 27 ; m++)
        {
            if (connectedExternalNodes(m - 1) != 0)
            {
                b = 1;
                for (int n = 1; n <= 27; n++)
                {
                    if (connectedExternalNodes(n - 1) != 0)
                    {
                        int x = 3 * a - 2;
                        int y = 3 * b - 2;
                        int f = 3 * m - 2;
                        int g = 3 * n - 2;

                        (*M)(x - 1 , y - 1) = M_temp(f - 1 , g - 1);
                        (*M)(x - 1 , y - 0) = M_temp(f - 1 , g - 0);
                        (*M)(x - 1 , y + 1) = M_temp(f - 1 , g + 1);

                        (*M)(x - 0 , y - 1) = M_temp(f - 0 , g - 1);
                        (*M)(x - 0 , y - 0) = M_temp(f - 0 , g - 0);
                        (*M)(x - 0 , y + 1) = M_temp(f - 0 , g + 1);

                        (*M)(x + 1 , y - 1) = M_temp(f + 1 , g - 1);
                        (*M)(x + 1 , y - 0) = M_temp(f + 1 , g - 0);
                        (*M)(x + 1 , y + 1) = M_temp(f + 1 , g + 1);

                        b++;
                    }
                }
                a++;
            }
        }
    return *M;
    }
}

//=============================================================================
void VariableNodeBrickLT::zeroLoad(void)
{
    (*Q).Zero();
}



//======================================================================
const Vector VariableNodeBrickLT::getBodyForce(double loadFactor, const Vector &data)
{

    Vector bforce(nodes_in_brick * 3);
    bforce.Zero();

    if (rho == 0.0) return bforce;

    Vector bf(3);
    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);

    Vector ba(nodes_in_brick * 3), bfx(3);
    bfx(0) = bf(0) * loadFactor;
    bfx(1) = bf(1) * loadFactor;
    bfx(2) = bf(2) * loadFactor;

    int n = 0;

    for ( int i = 0 ; i < nodes_in_brick*3; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            ba(n * 3 + 0) = bfx(0);
            ba(n * 3 + 1) = bfx(1);
            ba(n * 3 + 2) = bfx(2);

            n++;
        }
    }

    bforce.addMatrixVector(0.0, this->getMass(), ba, 1.0);

    return bforce;
}


//======================================================================
const Vector VariableNodeBrickLT::getSurfaceForce(double loadFactor, const Vector &data)
{
    cerr << "VariableNodeBrickLT::getSurfaceForce() haven't implemented yet!\n";
    (*tempZero).Zero();
    return *tempZero;
}



//=============================================================================
int VariableNodeBrickLT::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    int type;
    const Vector &data = theLoad->getData(type, loadFactor);

    if (type == LOAD_TAG_ElementSelfWeight)
    {

        Vector Fbody = this->getBodyForce(loadFactor, data);

        (*Q).addVector(1.0, Fbody, 1.0);

    }
    else
    {
        cerr << "VariableNodeBrickLT::addLoad() - 27NodeBrick " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }


    return 0;
}



//=============================================================================
int VariableNodeBrickLT::addInertiaLoadToUnbalance(const Vector &accel)
{
    // Check for a quick return
    if (rho == 0.0)
    {
        return 0;
    }


    static Vector ra(nodes_in_brick * 3);


    for ( int i = 0 ; i < nodes_in_brick ; i++)
    {
        ra(i * 3 + 0) = ( theNodes[i]->getRV(accel) )(0);
        ra(i * 3 + 1) = ( theNodes[i]->getRV(accel) )(1);
        ra(i * 3 + 2) = ( theNodes[i]->getRV(accel) )(2);
    }


    (*Q).addMatrixVector(1.0, this->getMass(), ra, -1.0);

    return 0;

}

//=============================================================================
const Vector VariableNodeBrickLT::FormEquiBodyForce(const Vector &data)
{

    Vector bforce(nodes_in_brick * 3);
    bforce.Zero();

    if (rho == 0.0)
    {
        return bforce;
    }


    Vector bf(3);
    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);

    Vector ba(nodes_in_brick * 3), bfx(3);

    int n = 0;

    for ( int i = 0 ; i < 27; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            ba(n * 3 + 0) = bf(0);
            ba(n * 3 + 1) = bf(1);
            ba(n * 3 + 2) = bf(2);

            n++;

        }

    }


    bforce.addMatrixVector(0.0, this->getMass(), ba, 1.0);

    return bforce;

}


//=============================================================================
const Vector &VariableNodeBrickLT::getResistingForce()
{
    DTensor2 nodalforces( 27, 3, 0.0 );
    nodalforces = nodal_forces();



    (*P).Zero();

    Vector P_temp(27 * 3);
    P_temp.Zero();




    nodalforces = nodal_forces();

    //converting nodalforce tensor to vector
    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            (P_temp)(i * 3 + j) = nodalforces(i , j );
        }
    }




    // filling the reduced size force vector for first 8 nodes
    //      for ( int i=1 ; i<=8*3 ; i++)
    //      {
    //         (*P)( i-1 ) = (P_temp)( i-1 );
    //      }
    //
    //
    //
    //
    // // filling the reduced size stiffness matrix for rest of the existing nodes
    //      int n =9;
    //
    //      for ( int m=8 ; m<27 ; m++)
    //      {
    //         if (connectedExternalNodes(m) != 0)
    //         {
    //
    //
    //            for ( int i=3*n-2 ; i<=3*n ; i++)
    //               {
    //                 (*P)( i-1 ) = (P_temp)( i-1 );
    //               }
    //
    //
    //             n++;
    //         }
    //      }



    int a = 1;


    for ( int m = 1 ; m <= 27 ; m++)
    {

        if (connectedExternalNodes(m - 1) != 0)
        {

            int x = 3 * a - 2;
            int f = 3 * m - 2;

            (*P)(x - 1) = P_temp(f - 1);
            (*P)(x - 1) = P_temp(f - 1);
            (*P)(x - 1) = P_temp(f - 1);

            (*P)(x - 0) = P_temp(f - 0);
            (*P)(x - 0) = P_temp(f - 0);
            (*P)(x - 0) = P_temp(f - 0);

            (*P)(x + 1) = P_temp(f + 1);
            (*P)(x + 1) = P_temp(f + 1);
            (*P)(x + 1) = P_temp(f + 1);

            a++;

        }
    }


    (*P) = (*P) - (*Q);


    //     cerr << "P-Q" << *P;

    //    for ( int i=0 ; i<nodes_in_brick*3 ; i++)
    //       {
    //          cout << "P(" << i+1 << "): " << (*P)(i) << endl;
    //       }


    return *P;
}


//=============================================================================
const Vector &VariableNodeBrickLT::getResistingForceIncInertia()
{

    (*P).Zero();

    this->getResistingForce();

    //
    // now add dynamic terms
    // P += M * a + C * v
    //


    if (rho != 0.0)
    {

        static Vector a(nodes_in_brick * 3);


        int n = 0;

        for ( int i = 0 ; i < 27; i++)
        {
            if (connectedExternalNodes(i) != 0)
            {
                a(n * 3 + 0) = ( theNodes[i]->getTrialAccel() )(0);
                a(n * 3 + 1) = ( theNodes[i]->getTrialAccel() )(1);
                a(n * 3 + 2) = ( theNodes[i]->getTrialAccel() )(2);

                n++;

            }

        }



        // P += M * a
        (*P).addMatrixVector(1.0, this->getMass(), a, 1.0);

    }

    // add the damping forces if rayleigh damping
    if (a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
    {
        (*P) += this->getRayleighDampingForces();
    }


    //     cerr << *P;


    return *P;

}


//=============================================================================

int VariableNodeBrickLT::sendSelf (int commitTag, Channel &theChannel)
{
    cerr << "VariableNodeBrickLT::sendSelf() haven't implemented yet!\n";
    return 0;
}



int VariableNodeBrickLT::receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    cerr << "VariableNodeBrickLT::receiveSelf() haven't implemented yet!\n";
    return 0;
}



//=============================================================================
// void VariableNodeBrickLT::Print(ostream &s, int flag)
// {
    

//     s << "VariableNodeBrickLT, element id:  " << this->getTag() << endln;
//     s << "Connected external nodes:  " << connectedExternalNodes;

//     //     int total_number_of_Gauss_points = integration_order*integration_order*integration_order;

//     //     if ( total_number_of_Gauss_points != 0 )
//     //     {
//     //       for (int i=0; i<nodes_in_brick; i++)
//     //       {
//     //          theNodes[i]->Print(cerr);
//     //       }
//     //
//     //     }

//     s << "Element mass density:  " << rho << endln << endln;
//     s << "Material model:  " << endln;


//     for ( int GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//     {
//         for ( int GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//         {
//             for ( int GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//             {
//                 // this short routine is supposed to calculate position of
//                 // Gauss point from 3D array of short's
//                 short where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                 s << "\n where = " << where << endln;
//                 s << " GP_c_r= " << GP_c_r << "GP_c_s = " << GP_c_s << " GP_c_t = " << GP_c_t << endln;
//                 material_array[where]->report("Material Point\n");

//             }
//         }
//     }

// }

//=============================================================================
// Response* VariableNodeBrickLT::setResponse (const char** argv, int argc, Information& eleInformation)
// {

//     //=====================================================================================
//     if (strcmp(argv[0], "stiff") == 0 || strcmp(argv[0], "stiffness") == 0)
//     {
//         return new ElementResponse(this, 5, *K);
//     }
//     //=====================================================================================
//     else if (strcmp(argv[0], "stress") == 0 || strcmp(argv[0], "stresses") == 0)
//     {
//         return new ElementResponse(this, 4, Vector(Num_TotalGaussPts * 6 + 1) );
//     }
//     //=====================================================================================
//     else if ( (strcmp(argv[0], "strains") == 0) || (strcmp(argv[0], "strain") == 0) )
//     {
//         return new ElementResponse(this, 7, Vector(Num_TotalGaussPts * 6 + 1) );
//     }
//     //=====================================================================================
//     else
//     {
//         return 0;
//     }

// }

// //=============================================================================
// int VariableNodeBrickLT::getResponse (int responseID, Information& eleInfo)
// {

//     switch (responseID)
//     {

//         case 4:
//             {
//                 static Vector stresses(Num_TotalGaussPts * 6 + 1);

//                 stresses(0) = Num_TotalGaussPts;

//                 stresstensor sigma;


//                 for (int i = 0; i < Num_TotalGaussPts; i++)
//                 {
//                     sigma = matpoint[i]->getStressTensor();
//                     stresses(i * 6 + 1) = sigma.cval(1, 1); //xx
//                     stresses(i * 6 + 2) = sigma.cval(2, 2); //yy
//                     stresses(i * 6 + 3) = sigma.cval(3, 3); //zz
//                     stresses(i * 6 + 4) = sigma.cval(1, 2); //xy
//                     stresses(i * 6 + 5) = sigma.cval(1, 3); //xz
//                     stresses(i * 6 + 6) = sigma.cval(2, 3); //yz
//                 }


//                 return eleInfo.setVector(stresses);
//             }

//         case 5:
//             return eleInfo.setMatrix(this->getTangentStiff());


//         case 7:
//             {
//                 static Vector strains(Num_TotalGaussPts * 6 + 1);

//                 strains(0) = Num_TotalGaussPts;

//                 straintensor epsilon;

//                 for (int i = 0; i < Num_TotalGaussPts; i++)
//                 {
//                     epsilon = matpoint[i]->getStrainTensor();
//                     strains(i * 6 + 1) = epsilon.cval(1, 1); //xx
//                     strains(i * 6 + 2) = epsilon.cval(2, 2); //yy
//                     strains(i * 6 + 3) = epsilon.cval(3, 3); //zz
//                     strains(i * 6 + 4) = epsilon.cval(1, 2); //xy
//                     strains(i * 6 + 5) = epsilon.cval(1, 3); //xz
//                     strains(i * 6 + 6) = epsilon.cval(2, 3); //yz
//                 }

//                 return eleInfo.setVector(strains);
//             }


//         default:
//             return -1;
//     }


// }


// double VariableNodeBrickLT::get_Gauss_p_c(short order, short point_numb)
// {

//     //  Abscissae coefficient of the Gaussian quadrature formula
//     // starting from 1 not from 0
//     static double Gauss_coordinates[7][7];

//     Gauss_coordinates[1][1] = 0.0 ;
//     Gauss_coordinates[2][1] = -0.577350269189626;
//     Gauss_coordinates[2][2] = -Gauss_coordinates[2][1];
//     Gauss_coordinates[3][1] = -0.774596669241483;
//     Gauss_coordinates[3][2] = 0.0;
//     Gauss_coordinates[3][3] = -Gauss_coordinates[3][1];
//     Gauss_coordinates[4][1] = -0.861136311594053;
//     Gauss_coordinates[4][2] = -0.339981043584856;
//     Gauss_coordinates[4][3] = -Gauss_coordinates[4][2];
//     Gauss_coordinates[4][4] = -Gauss_coordinates[4][1];
//     Gauss_coordinates[5][1] = -0.906179845938664;
//     Gauss_coordinates[5][2] = -0.538469310105683;
//     Gauss_coordinates[5][3] = 0.0;
//     Gauss_coordinates[5][4] = -Gauss_coordinates[5][2];
//     Gauss_coordinates[5][5] = -Gauss_coordinates[5][1];
//     Gauss_coordinates[6][1] = -0.932469514203152;
//     Gauss_coordinates[6][2] = -0.661509386466265;
//     Gauss_coordinates[6][3] = -0.238619188183197;
//     Gauss_coordinates[6][4] = -Gauss_coordinates[6][3];
//     Gauss_coordinates[6][5] = -Gauss_coordinates[6][2];
//     Gauss_coordinates[6][6] = -Gauss_coordinates[6][1];

//     return Gauss_coordinates[order][point_numb];

// }

// double VariableNodeBrickLT::get_Gauss_p_w(short order, short point_numb)
// {

//     //  Weight coefficient of the Gaussian quadrature formula
//     // starting from 1 not from 0
//     static double Gauss_weights[7][7]; 

//     Gauss_weights[1][1] = 2.0;
//     Gauss_weights[2][1] = 1.0;
//     Gauss_weights[2][2] = 1.0;
//     Gauss_weights[3][1] = 0.555555555555556;
//     Gauss_weights[3][2] = 0.888888888888889;
//     Gauss_weights[3][3] = Gauss_weights[3][1];
//     Gauss_weights[4][1] = 0.347854845137454;
//     Gauss_weights[4][2] = 0.652145154862546;
//     Gauss_weights[4][3] = Gauss_weights[4][2];
//     Gauss_weights[4][4] = Gauss_weights[4][1];
//     Gauss_weights[5][1] = 0.236926885056189;
//     Gauss_weights[5][2] = 0.478628670499366;
//     Gauss_weights[5][3] = 0.568888888888889;
//     Gauss_weights[5][4] = Gauss_weights[5][2];
//     Gauss_weights[5][5] = Gauss_weights[5][1];
//     Gauss_weights[6][1] = 0.171324492379170;
//     Gauss_weights[6][2] = 0.381761573048139;
//     Gauss_weights[6][3] = 0.467913934572691;
//     Gauss_weights[6][4] = Gauss_weights[6][3];
//     Gauss_weights[6][5] = Gauss_weights[6][2];
//     Gauss_weights[6][6] = Gauss_weights[6][1];

//     return Gauss_weights[order][point_numb];

// }

int VariableNodeBrickLT::update(void)
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    DTensor2 dh( Num_TotalGaussPts, 3, 0.0 );
    DTensor2 JacobianINV(3, 3, 0.0);
    DTensor2 dhGlobal( Num_TotalGaussPts, 3, 0.0 );
    DTensor2 trial_disp( nodes_in_brick, 3, 0.0  );
    DTensor2 total_strain(3, 3, 0.0);
    DTensor2 trial_strain(3, 3, 0.0);

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
            cerr << "VariableNodeBrickLT::update (tag: " << this->getTag() << "), Update Failed\n";
            return -1;
        }
    }
    return 0;
}




int
VariableNodeBrickLT::CheckMesh(ofstream &checkmesh_file)
{
    bool jacobian_flag = false;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    double det_of_Jacobian = 0.0;

    DTensor2 dh( 27, 3, 0.0 );
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
VariableNodeBrickLT::getStress(void)
{


    
    DTensor2 stress;
    Vector *stresses = new Vector( Num_TotalGaussPts*6 );

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

int VariableNodeBrickLT::getOutputSize() const
{
    return VariableNodeBrickLT_OUTPUT_SIZE ;
}


const Vector VariableNodeBrickLT::getOutput() 
{
    Vector outputVector(VariableNodeBrickLT_OUTPUT_SIZE );
    //Form the output vector
    int ii = 0;
    for (int gp = 0; gp < Num_TotalGaussPts; gp++)
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

    outputVector(ii++) = update_time_taken;
    return outputVector;
}




#endif



