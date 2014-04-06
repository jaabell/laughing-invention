///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              VariableNodeBrick.cpp
// CLASS:             VariableNodeBrick
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// DESIGNER:          Nima Tafazzoli and Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli and Boris Jeremic
// DATE:              May 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#ifndef VariableNodeBrick_CPP
#define VariableNodeBrick_CPP

#include <VariableNodeBrick.h>


VariableNodeBrick::VariableNodeBrick(int element_number,
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
                                     NDMaterial *Globalmmodel)
    : Element(element_number, ELE_TAG_VariableNodeBrick),
      connectedExternalNodes(27), node_existance(19)
{

    rho = Globalmmodel->getRho();

    determinant_of_Jacobian = 0.0;

    integration_order = int_order; // Gauss-Legendre integration order


    Num_TotalGaussPts = integration_order * integration_order * integration_order;

    GaussCoord = new Vector(Num_TotalGaussPts * 3 + 1);


    mmodel = Globalmmodel;


# ifndef _PARALLEL_PROCESSING
    populate();
# endif



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
    theNodes = new Node* [27];

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

}


void VariableNodeBrick::populate()
{


    if ( Num_TotalGaussPts != 0 )
    {
        matpoint  = new MatPoint3D * [Num_TotalGaussPts];
    }
    else
    {
        matpoint  = 0;
    }


    short where = 0;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        double r = get_Gauss_p_c( integration_order, GP_c_r );
        double rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            double s = get_Gauss_p_c( integration_order, GP_c_s );
            double sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                double t = get_Gauss_p_c( integration_order, GP_c_t );
                double tw = get_Gauss_p_w( integration_order, GP_c_t );

                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;


                matpoint[where] = new MatPoint3D(GP_c_r, GP_c_s, GP_c_t,
                                                 r, s, t,
                                                 rw, sw, tw,
                                                 mmodel);

            }
        }
    }

    initialized = true;


}



//====================================================================
VariableNodeBrick::VariableNodeBrick()
    : Element(0, ELE_TAG_VariableNodeBrick),
      connectedExternalNodes(27), node_existance(19), rho(0.0), mmodel(0)
{

    integration_order = 3; // Gauss-Legendre integration order

    initialized = false;


# ifndef _PARALLEL_PROCESSING
    populate();
# endif


}


VariableNodeBrick::~VariableNodeBrick()
{

    for (int i = 0; i < Num_TotalGaussPts; i++)
    {
        // Delete the NDMaterials at each integration point
        if (matpoint[i])
        {
            delete matpoint[i];
        }
    }

    // Delete the array of pointers to NDMaterial pointer arrays
    if (matpoint)
    {
        delete [] matpoint;
    }

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

}


void VariableNodeBrick::incremental_Update()
{

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    int dh_dim[] = {27, 3};
    tensor dh(2, dh_dim, 0.0);


    static int disp_dim[] = {nodes_in_brick, 3};


    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    incremental_displacements = incr_disp();

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );

                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                dh = dh_drst_at(r, s, t);

                Jacobian = Jacobian_3D(dh);

                JacobianINV = Jacobian_3Dinv(dh);

                dhGlobal = dh("ij") * JacobianINV("kj");

                incremental_strain = (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();
                //                 incremental_strain.print("strain");


                if ( ( (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)) )
                {
                    cerr << "EightNodeBrick::incremental_Update (tag: " << this->getTag() << "), not converged\n";
                }

            }
        }
    }


}


//***************************************************************
tensor VariableNodeBrick::H_3D(double r1, double r2, double r3)
{

    int dimension[] = {81, 3};

    tensor H(2, dimension, 0.0);


    //Shape Functions of Node 27 Along Three Coordinate Directions
    H.val(79, 1) = node_existance(27 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H.val(80, 2) = H.val(79, 1);
    H.val(81, 3) = H.val(79, 1);


    //Shape Functions of Node 26 Along Three Coordinate Directions
    H.val(76, 1) = node_existance(26 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H.val(77, 2) = H.val(76, 1);
    H.val(78, 3) = H.val(76, 1);


    //Shape Functions of Node 25 Along Three Coordinate Directions
    H.val(73, 1) = node_existance(25 - 1 - 8) * 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H.val(74, 2) = H.val(73, 1);
    H.val(75, 3) = H.val(73, 1);


    //Shape Functions of Node 24 Along Three Coordinate Directions
    H.val(70, 1) = node_existance(24 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H.val(71, 2) = H.val(70, 1);
    H.val(72, 3) = H.val(70, 1);


    //Shape Functions of Node 23 Along Three Coordinate Directions
    H.val(67, 1) = node_existance(23 - 1 - 8) * 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H.val(68, 2) = H.val(67, 1);
    H.val(69, 3) = H.val(67, 1);


    //Shape Functions of Node 22 Along Three Coordinate Directions
    H.val(64, 1) = node_existance(22 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H.val(65, 2) = H.val(64, 1);
    H.val(66, 3) = H.val(64, 1);


    //Shape Functions of Node 21 Along Three Coordinate Directions
    H.val(61, 1) = node_existance(21 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H.val(62, 2) = H.val(61, 1);
    H.val(63, 3) = H.val(61, 1);


    //Shape Functions of Node 20 Along Three Coordinate Directions
    H.val(58, 1) = node_existance(20 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (H.val(70, 1) + H.val(73, 1)) - 0.25 * H.val(61, 1) );
    H.val(59, 2) = H.val(58, 1);
    H.val(60, 3) = H.val(58, 1);


    //Shape Functions of Node 19 Along Three Coordinate Directions
    H.val(55, 1) = node_existance(19 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (H.val(67, 1) + H.val(70, 1)) - 0.25 * H.val(61, 1) );
    H.val(56, 2) = H.val(55, 1);
    H.val(57, 3) = H.val(55, 1);


    //Shape Functions of Node 18 Along Three Coordinate Directions
    H.val(52, 1) = node_existance(18 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (H.val(64, 1) + H.val(67, 1)) - 0.25 * H.val(61, 1) );
    H.val(53, 2) = H.val(52, 1);
    H.val(54, 3) = H.val(52, 1);


    //Shape Functions of Node 17 Along Three Coordinate Directions
    H.val(49, 1) = node_existance(17 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (H.val(64, 1) + H.val(73, 1)) - 0.25 * H.val(61, 1) );
    H.val(50, 2) = H.val(49, 1);
    H.val(51, 3) = H.val(49, 1);


    //Shape Functions of Node 16 Along Three Coordinate Directions
    H.val(46, 1) = node_existance(16 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (H.val(73, 1) + H.val(79, 1)) - 0.25 * H.val(61, 1) );
    H.val(47, 2) = H.val(46, 1);
    H.val(48, 3) = H.val(46, 1);


    //Shape Functions of Node 15 Along Three Coordinate Directions
    H.val(43, 1) = node_existance(15 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) - 0.5 * (H.val(70, 1) + H.val(79, 1)) - 0.25 * H.val(61, 1) );
    H.val(44, 2) = H.val(43, 1);
    H.val(45, 3) = H.val(43, 1);


    //Shape Functions of Node 14 Along Three Coordinate Directions
    H.val(40, 1) = node_existance(14 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (H.val(67, 1) + H.val(79, 1)) - 0.25 * H.val(61, 1) );
    H.val(41, 2) = H.val(40, 1);
    H.val(42, 3) = H.val(40, 1);


    //Shape Functions of Node 13 Along Three Coordinate Directions
    H.val(37, 1) = node_existance(13 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) - 0.5 * (H.val(64, 1) + H.val(79, 1)) - 0.25 * H.val(61, 1) );
    H.val(38, 2) = H.val(37, 1);
    H.val(39, 3) = H.val(37, 1);


    //Shape Functions of Node 12 Along Three Coordinate Directions
    H.val(34, 1) = node_existance(12 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (H.val(73, 1) + H.val(76, 1)) - 0.25 * H.val(61, 1) );
    H.val(35, 2) = H.val(34, 1);
    H.val(36, 3) = H.val(34, 1);


    //Shape Functions of Node 11 Along Three Coordinate Directions
    H.val(31, 1) = node_existance(11 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) - 0.5 * (H.val(70, 1) + H.val(76, 1)) - 0.25 * H.val(61, 1) );
    H.val(32, 2) = H.val(31, 1);
    H.val(33, 3) = H.val(31, 1);


    //Shape Functions of Node 10 Along Three Coordinate Directions
    H.val(28, 1) = node_existance(10 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (H.val(67, 1) + H.val(76, 1)) - 0.25 * H.val(61, 1) );
    H.val(29, 2) = H.val(28, 1);
    H.val(30, 3) = H.val(28, 1);


    //Shape Functions of Node 9 Along Three Coordinate Directions
    H.val(25, 1) = node_existance(9 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) - 0.5 * (H.val(64, 1) + H.val(76, 1)) - 0.25 * H.val(61, 1) );
    H.val(26, 2) = H.val(25, 1);
    H.val(27, 3) = H.val(25, 1);


    //Shape Functions of Node 1 Along Three Coordinate Directions
    H.val(1, 1) = 0.125 * (1 + r1) * (1 + r2) * (1 + r3) - 0.5 * ( H.val(25, 1) + H.val(34, 1) + H.val(49, 1) ) - 0.25 * ( H.val(64, 1) + H.val(73, 1) + H.val(76, 1) ) - 0.125 * H.val(61, 1);
    H.val(2, 2) = H.val(1, 1);
    H.val(3, 3) = H.val(1, 1);

    //Shape Functions of Node 2 Along Three Coordinate Directions
    H.val(4, 1) = 0.125 * (1 - r1) * (1 + r2) * (1 + r3) - 0.5 * ( H.val(25, 1) + H.val(28, 1) + H.val(52, 1) ) - 0.25 * ( H.val(64, 1) + H.val(67, 1) + H.val(76, 1) ) - 0.125 * H.val(61, 1);
    H.val(5, 2) = H.val(4, 1);
    H.val(6, 3) = H.val(4, 1);

    //Shape Functions of Node 3 Along Three Coordinate Directions
    H.val(7, 1) = 0.125 * (1 - r1) * (1 - r2) * (1 + r3) - 0.5 * ( H.val(28, 1) + H.val(31, 1) + H.val(55, 1) ) - 0.25 * ( H.val(67, 1) + H.val(70, 1) + H.val(76, 1) ) - 0.125 * H.val(61, 1);
    H.val(8, 2) = H.val(7, 1);
    H.val(9, 3) = H.val(7, 1);

    //Shape Functions of Node 4 Along Three Coordinate Directions
    H.val(10, 1) = 0.125 * (1 + r1) * (1 - r2) * (1 + r3) - 0.5 * ( H.val(31, 1) + H.val(34, 1) + H.val(58, 1) ) - 0.25 * ( H.val(70, 1) + H.val(73, 1) + H.val(76, 1) ) - 0.125 * H.val(61, 1);
    H.val(11, 2) = H.val(10, 1);
    H.val(12, 3) = H.val(10, 1);

    //Shape Functions of Node 5 Along Three Coordinate Directions
    H.val(13, 1) = 0.125 * (1 + r1) * (1 + r2) * (1 - r3) - 0.5 * ( H.val(37, 1) + H.val(46, 1) + H.val(49, 1) ) - 0.25 * ( H.val(64, 1) + H.val(73, 1) + H.val(79, 1) ) - 0.125 * H.val(61, 1);
    H.val(14, 2) = H.val(13, 1);
    H.val(15, 3) = H.val(13, 1);

    //Shape Functions of Node 6 Along Three Coordinate Directions
    H.val(16, 1) = 0.125 * (1 - r1) * (1 + r2) * (1 - r3) - 0.5 * ( H.val(37, 1) + H.val(40, 1) + H.val(52, 1) ) - 0.25 * ( H.val(64, 1) + H.val(67, 1) + H.val(79, 1) ) - 0.125 * H.val(61, 1);
    H.val(17, 2) = H.val(16, 1);
    H.val(18, 3) = H.val(16, 1);

    //Shape Functions of Node 7 Along Three Coordinate Directions
    H.val(19, 1) = 0.125 * (1 - r1) * (1 - r2) * (1 - r3) - 0.5 * ( H.val(40, 1) + H.val(43, 1) + H.val(55, 1) ) - 0.25 * ( H.val(67, 1) + H.val(70, 1) + H.val(79, 1) ) - 0.125 * H.val(61, 1);
    H.val(20, 2) = H.val(19, 1);
    H.val(21, 3) = H.val(19, 1);


    //Shape Functions of Node 8 Along Three Coordinate Directions
    H.val(22, 1) = 0.125 * (1 + r1) * (1 - r2) * (1 - r3) - 0.5 * ( H.val(43, 1) + H.val(46, 1) + H.val(58, 1) ) - 0.25 * ( H.val(70, 1) + H.val(73, 1) + H.val(79, 1) ) - 0.125 * H.val(61, 1);
    H.val(23, 2) = H.val(22, 1);
    H.val(24, 3) = H.val(22, 1);


    //     H.print("H","\n\n H \n");

    return H;

}



tensor VariableNodeBrick::dh_drst_at(double r1, double r2, double r3)
{
    int dimensions[] = {27, 3};
    tensor dh(2, dimensions, 0.0);

    // Node 21
    dh.val(21, 1) = node_existance(21 - 1 - 8) * (-2.0 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh.val(21, 2) = node_existance(21 - 1 - 8) * (1.0 - r1 * r1) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh.val(21, 3) = node_existance(21 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 22
    dh.val(22, 1) = node_existance(22 - 1 - 8) * (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh.val(22, 2) = node_existance(22 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh.val(22, 3) = node_existance(22 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    // Node 23
    dh.val(23, 1) = node_existance(23 - 1 - 8) * 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh.val(23, 2) = node_existance(23 - 1 - 8) * 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh.val(23, 3) = node_existance(23 - 1 - 8) * 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 24
    dh.val(24, 1) = node_existance(24 - 1 - 8) * (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh.val(24, 2) = node_existance(24 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh.val(24, 3) = node_existance(24 - 1 - 8) * (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    // Node 25
    dh.val(25, 1) = node_existance(25 - 1 - 8) * 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh.val(25, 2) = node_existance(25 - 1 - 8) * 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh.val(25, 3) = node_existance(25 - 1 - 8) * 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    // Node 26
    dh.val(26, 1) = node_existance(26 - 1 - 8) * (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh.val(26, 2) = node_existance(26 - 1 - 8) * (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh.val(26, 3) = node_existance(26 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    // Node 27
    dh.val(27, 1) = node_existance(27 - 1 - 8) * (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh.val(27, 2) = node_existance(27 - 1 - 8) * (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh.val(27, 3) = node_existance(27 - 1 - 8) * (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);


    // Node 20
    dh.val(20, 1) = node_existance(20 - 1 - 8) * ( 0.25 * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (dh.val(24, 1) + dh.val(25, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(20, 2) = node_existance(20 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 + r1) * (1.0 - r3 * r3) - 0.5 * (dh.val(24, 2) + dh.val(25, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(20, 3) = node_existance(20 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2) * (-2.0 * r3) - 0.5 * (dh.val(24, 3) + dh.val(25, 3)) - 0.25 * dh.val(21, 3) );


    // Node 19
    dh.val(19, 1) = node_existance(19 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 - r2) * (1.0 - r3 * r3) - 0.5 * (dh.val(23, 1) + dh.val(24, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(19, 2) = node_existance(19 - 1 - 8) * ( 0.25 * (1.0 - r1) * (-1.0) * (1.0 - r3 * r3) - 0.5 * (dh.val(23, 2) + dh.val(24, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(19, 3) = node_existance(19 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2) * (-2.0 * r3) - 0.5 * (dh.val(23, 3) + dh.val(24, 3)) - 0.25 * dh.val(21, 3) );


    // Node 18
    dh.val(18, 1) = node_existance(18 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (dh.val(22, 1) + dh.val(23, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(18, 2) = node_existance(18 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r3 * r3) - 0.5 * (dh.val(22, 2) + dh.val(23, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(18, 3) = node_existance(18 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 + r2) * (-2.0 * r3) - 0.5 * (dh.val(22, 3) + dh.val(23, 3)) - 0.25 * dh.val(21, 3) );


    // Node 17
    dh.val(17, 1) = node_existance(17 - 1 - 8) * ( 0.25 * (1.0 + r2) * (1.0 - r3 * r3) - 0.5 * (dh.val(22, 1) + dh.val(25, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(17, 2) = node_existance(17 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r3 * r3) - 0.5 * (dh.val(22, 2) + dh.val(25, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(17, 3) = node_existance(17 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 + r2) * (-2.0 * r3) - 0.5 * (dh.val(22, 3) + dh.val(25, 3)) - 0.25 * dh.val(21, 3) );


    // Node 16
    dh.val(16, 1) = node_existance(16 - 1 - 8) * ( 0.25 * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (dh.val(25, 1) + dh.val(27, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(16, 2) = node_existance(16 - 1 - 8) * ( 0.25 * (1.0 + r1) * (-2.0 * r2) * (1.0 - r3) - 0.5 * (dh.val(25, 2) + dh.val(27, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(16, 3) = node_existance(16 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) * (-1.0) - 0.5 * (dh.val(25, 3) + dh.val(27, 3)) - 0.25 * dh.val(21, 3) );


    // Node 15
    dh.val(15, 1) = node_existance(15 - 1 - 8) * ( 0.25 * (-2.0 * r1) * (1.0 - r2) * (1.0 - r3) - 0.5 * (dh.val(24, 1) + dh.val(27, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(15, 2) = node_existance(15 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (-1.0) * (1.0 - r3) - 0.5 * (dh.val(24, 2) + dh.val(27, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(15, 3) = node_existance(15 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) * (-1.0) - 0.5 * (dh.val(24, 3) + dh.val(27, 3)) - 0.25 * dh.val(21, 3) );



    // Node 14
    dh.val(14, 1) = node_existance(14 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 - r2 * r2) * (1.0 - r3) - 0.5 * (dh.val(23, 1) + dh.val(27, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(14, 2) = node_existance(14 - 1 - 8) * ( 0.25 * (1.0 - r1) * (-2.0 * r2) * (1.0 - r3) - 0.5 * (dh.val(23, 2) + dh.val(27, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(14, 3) = node_existance(14 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) * (-1.0) - 0.5 * (dh.val(23, 3) + dh.val(27, 3)) - 0.25 * dh.val(21, 3) );


    // Node 13
    dh.val(13, 1) = node_existance(13 - 1 - 8) * ( 0.25 * (-2.0 * r1) * (1.0 + r2) * (1.0 - r3) - 0.5 * (dh.val(22, 1) + dh.val(27, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(13, 2) = node_existance(13 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r3) - 0.5 * (dh.val(22, 2) + dh.val(27, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(13, 3) = node_existance(13 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) * (-1.0) - 0.5 * (dh.val(22, 3) + dh.val(27, 3)) - 0.25 * dh.val(21, 3) );


    // Node 12
    dh.val(12, 1) = node_existance(12 - 1 - 8) * ( 0.25 * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (dh.val(25, 1) + dh.val(26, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(12, 2) = node_existance(12 - 1 - 8) * ( 0.25 * (1.0 + r1) * (-2.0 * r2) * (1.0 + r3) - 0.5 * (dh.val(25, 2) + dh.val(26, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(12, 3) = node_existance(12 - 1 - 8) * ( 0.25 * (1.0 + r1) * (1.0 - r2 * r2) - 0.5 * (dh.val(25, 3) + dh.val(26, 3)) - 0.25 * dh.val(21, 3) );


    // Node 11
    dh.val(11, 1) = node_existance(11 - 1 - 8) * ( 0.25 * (-2.0 * r1) * (1.0 - r2) * (1.0 + r3) - 0.5 * (dh.val(24, 1) + dh.val(26, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(11, 2) = node_existance(11 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (-1.0) * (1.0 + r3) - 0.5 * (dh.val(24, 2) + dh.val(26, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(11, 3) = node_existance(11 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 - r2) - 0.5 * (dh.val(24, 3) + dh.val(26, 3)) - 0.25 * dh.val(21, 3) );


    // Node 10
    dh.val(10, 1) = node_existance(10 - 1 - 8) * ( 0.25 * (-1.0) * (1.0 - r2 * r2) * (1.0 + r3) - 0.5 * (dh.val(23, 1) + dh.val(26, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(10, 2) = node_existance(10 - 1 - 8) * ( 0.25 * (1.0 - r1) * (-2.0 * r2) * (1.0 + r3) - 0.5 * (dh.val(23, 2) + dh.val(26, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(10, 3) = node_existance(10 - 1 - 8) * ( 0.25 * (1.0 - r1) * (1.0 - r2 * r2) - 0.5 * (dh.val(23, 3) + dh.val(26, 3)) - 0.25 * dh.val(21, 3) );


    // Node 9
    dh.val(9, 1) = node_existance(9 - 1 - 8) * ( 0.25 * (-2.0 * r1) * (1.0 + r2) * (1.0 + r3) - 0.5 * (dh.val(22, 1) + dh.val(26, 1)) - 0.25 * dh.val(21, 1) );
    dh.val(9, 2) = node_existance(9 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r3) - 0.5 * (dh.val(22, 2) + dh.val(26, 2)) - 0.25 * dh.val(21, 2) );
    dh.val(9, 3) = node_existance(9 - 1 - 8) * ( 0.25 * (1.0 - r1 * r1) * (1.0 + r2) - 0.5 * (dh.val(22, 3) + dh.val(26, 3)) - 0.25 * dh.val(21, 3) );


    // Node 1
    dh.val(1, 1) = 0.125 * (1 + r2) * (1 + r3) - 0.5 * ( dh.val(9, 1) + dh.val(12, 1) + dh.val(17, 1) ) - 0.25 * ( dh.val(22, 1) + dh.val(25, 1) + dh.val(26, 1) ) - 0.125 * dh.val(21, 1);
    dh.val(1, 2) = 0.125 * (1 + r1) * (1 + r3) - 0.5 * ( dh.val(9, 2) + dh.val(12, 2) + dh.val(17, 2) ) - 0.25 * ( dh.val(22, 2) + dh.val(25, 2) + dh.val(26, 2) ) - 0.125 * dh.val(21, 2);
    dh.val(1, 3) = 0.125 * (1 + r1) * (1 + r2) - 0.5 * ( dh.val(9, 3) + dh.val(12, 3) + dh.val(17, 3) ) - 0.25 * ( dh.val(22, 3) + dh.val(25, 3) + dh.val(26, 3) ) - 0.125 * dh.val(21, 3);


    // Node 2
    dh.val(2, 1) = 0.125 * (-1.0) * (1 + r2) * (1 + r3) - 0.5 * ( dh.val(9, 1) + dh.val(10, 1) + dh.val(18, 1) ) - 0.25 * ( dh.val(22, 1) + dh.val(23, 1) + dh.val(26, 1) ) - 0.125 * dh.val(21, 1);
    dh.val(2, 2) = 0.125 * (1 - r1) * (1 + r3) - 0.5 * ( dh.val(9, 2) + dh.val(10, 2) + dh.val(18, 2) ) - 0.25 * ( dh.val(22, 2) + dh.val(23, 2) + dh.val(26, 2) ) - 0.125 * dh.val(21, 2);
    dh.val(2, 3) = 0.125 * (1 - r1) * (1 + r2) - 0.5 * ( dh.val(9, 3) + dh.val(10, 3) + dh.val(18, 3) ) - 0.25 * ( dh.val(22, 3) + dh.val(23, 3) + dh.val(26, 3) ) - 0.125 * dh.val(21, 3);


    // Node 3
    dh.val(3, 1) = 0.125 * (-1.0) * (1 - r2) * (1 + r3) - 0.5 * ( dh.val(10, 1) + dh.val(11, 1) + dh.val(19, 1) ) - 0.25 * ( dh.val(23, 1) + dh.val(24, 1) + dh.val(26, 1) ) - 0.125 * dh.val(21, 1);
    dh.val(3, 2) = 0.125 * (1 - r1) * (-1.0) * (1 + r3) - 0.5 * ( dh.val(10, 2) + dh.val(11, 2) + dh.val(19, 2) ) - 0.25 * ( dh.val(23, 2) + dh.val(24, 2) + dh.val(26, 2) ) - 0.125 * dh.val(21, 2);
    dh.val(3, 3) = 0.125 * (1 - r1) * (1 - r2) - 0.5 * ( dh.val(10, 3) + dh.val(11, 3) + dh.val(19, 3) ) - 0.25 * ( dh.val(23, 3) + dh.val(24, 3) + dh.val(26, 3) ) - 0.125 * dh.val(21, 3);


    // Node 4
    dh.val(4, 1) = 0.125 * (1 - r2) * (1 + r3) - 0.5 * ( dh.val(11, 1) + dh.val(12, 1) + dh.val(20, 1) ) - 0.25 * ( dh.val(24, 1) + dh.val(25, 1) + dh.val(26, 1) ) - 0.125 * dh.val(21, 1);
    dh.val(4, 2) = 0.125 * (1 + r1) * (-1.0) * (1 + r3) - 0.5 * ( dh.val(11, 2) + dh.val(12, 2) + dh.val(20, 2) ) - 0.25 * ( dh.val(24, 2) + dh.val(25, 2) + dh.val(26, 2) ) - 0.125 * dh.val(21, 2);
    dh.val(4, 3) = 0.125 * (1 + r1) * (1 - r2) - 0.5 * ( dh.val(11, 3) + dh.val(12, 3) + dh.val(20, 3) ) - 0.25 * ( dh.val(24, 3) + dh.val(25, 3) + dh.val(26, 3) ) - 0.125 * dh.val(21, 3);


    // Node 5
    dh.val(5, 1) = 0.125 * (1 + r2) * (1 - r3) - 0.5 * ( dh.val(13, 1) + dh.val(16, 1) + dh.val(17, 1) ) - 0.25 * ( dh.val(22, 1) + dh.val(25, 1) + dh.val(27, 1) ) - 0.125 * dh.val(21, 1);
    dh.val(5, 2) = 0.125 * (1 + r1) * (1 - r3) - 0.5 * ( dh.val(13, 2) + dh.val(16, 2) + dh.val(17, 2) ) - 0.25 * ( dh.val(22, 2) + dh.val(25, 2) + dh.val(27, 2) ) - 0.125 * dh.val(21, 2);
    dh.val(5, 3) = 0.125 * (1 + r1) * (1 + r2) * (-1.0) - 0.5 * ( dh.val(13, 3) + dh.val(16, 3) + dh.val(17, 3) ) - 0.25 * ( dh.val(22, 3) + dh.val(25, 3) + dh.val(27, 3) ) - 0.125 * dh.val(21, 3);


    // Node 6
    dh.val(6, 1) = 0.125 * (-1.0) * (1 + r2) * (1 - r3) - 0.5 * ( dh.val(13, 1) + dh.val(14, 1) + dh.val(18, 1) ) - 0.25 * ( dh.val(22, 1) + dh.val(23, 1) + dh.val(27, 1) ) - 0.125 * dh.val(21, 1);
    dh.val(6, 2) = 0.125 * (1 - r1) * (1 - r3) - 0.5 * ( dh.val(13, 2) + dh.val(14, 2) + dh.val(18, 2) ) - 0.25 * ( dh.val(22, 2) + dh.val(23, 2) + dh.val(27, 2) ) - 0.125 * dh.val(21, 2);
    dh.val(6, 3) = 0.125 * (1 - r1) * (1 + r2) * (-1.0) - 0.5 * ( dh.val(13, 3) + dh.val(14, 3) + dh.val(18, 3) ) - 0.25 * ( dh.val(22, 3) + dh.val(23, 3) + dh.val(27, 3) ) - 0.125 * dh.val(21, 3);


    // Node 7
    dh.val(7, 1) = 0.125 * (-1.0) * (1 - r2) * (1 - r3) - 0.5 * ( dh.val(14, 1) + dh.val(15, 1) + dh.val(19, 1) ) - 0.25 * ( dh.val(23, 1) + dh.val(24, 1) + dh.val(27, 1) ) - 0.125 * dh.val(21, 1);
    dh.val(7, 2) = 0.125 * (1 - r1) * (-1.0) * (1 - r3) - 0.5 * ( dh.val(14, 2) + dh.val(15, 2) + dh.val(19, 2) ) - 0.25 * ( dh.val(23, 2) + dh.val(24, 2) + dh.val(27, 2) ) - 0.125 * dh.val(21, 2);
    dh.val(7, 3) = 0.125 * (1 - r1) * (1 - r2) * (-1.0) - 0.5 * ( dh.val(14, 3) + dh.val(15, 3) + dh.val(19, 3) ) - 0.25 * ( dh.val(23, 3) + dh.val(24, 3) + dh.val(27, 3) ) - 0.125 * dh.val(21, 3);


    // Node 8
    dh.val(8, 1) = 0.125 * (1 - r2) * (1 - r3) - 0.5 * ( dh.val(15, 1) + dh.val(16, 1) + dh.val(20, 1) ) - 0.25 * ( dh.val(24, 1) + dh.val(25, 1) + dh.val(27, 1) ) - 0.125 * dh.val(21, 1);
    dh.val(8, 2) = 0.125 * (1 + r1) * (-1.0) * (1 - r3) - 0.5 * ( dh.val(15, 2) + dh.val(16, 2) + dh.val(20, 2) ) - 0.25 * ( dh.val(24, 2) + dh.val(25, 2) + dh.val(27, 2) ) - 0.125 * dh.val(21, 2);
    dh.val(8, 3) = 0.125 * (1 + r1) * (1 - r2) * (-1.0) - 0.5 * ( dh.val(15, 3) + dh.val(16, 3) + dh.val(20, 3) ) - 0.25 * ( dh.val(24, 3) + dh.val(25, 3) + dh.val(27, 3) ) - 0.125 * dh.val(21, 3);


    //             dh.print("dh");

    return dh;
}

////#############################################################################
VariableNodeBrick &VariableNodeBrick::operator[](int subscript)
{
    return ( *(this + subscript) );
}


////#############################################################################
tensor VariableNodeBrick::getStiffnessTensor(void)
{

    int K_dim[] = {27, 3, 3, 27};
    tensor Kk(4, K_dim, 0.0);
    tensor Kkt(4, K_dim, 0.0);


    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {27, 3};
    tensor dh(2, dh_dim, 0.0);


    tensor Constitutive;

    double det_of_Jacobian = 0.0;


    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                where =  ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                dh = dh_drst_at(r, s, t);

                Jacobian = Jacobian_3D(dh);

                JacobianINV = Jacobian_3Dinv(dh);

                det_of_Jacobian  = Jacobian.determinant();

                dhGlobal = dh("ij") * JacobianINV("kj");

                weight = rw * sw * tw * det_of_Jacobian;

                Constitutive = (matpoint[where]->matmodel)->getTangentTensor();

                Kkt = dhGlobal("ib") * Constitutive("abcd");
                Kk = Kk + Kkt("aicd") * dhGlobal("jd") * weight;

            }
        }
    }

    //     Kk.print("K","\n\n K tensor \n");


    return Kk;


}


//#############################################################################

void VariableNodeBrick::set_strain_stress_tensor(FILE *fp, double *u)
{

    int dh_dim[] = {27, 3};
    tensor dh(2, dh_dim, 0.0);

    tensor Constitutive;
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    int where = 0;


    straintensor strain;
    stresstensor stress;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;


    static int disp_dim[] = {27, 3};

    tensor total_displacements(2, disp_dim, 0.0);

    total_displacements = total_disp(fp, u);

    ::printf("\n  displacement(x-y-z) at GAUSS pt %d \n\n", where + 1);

    for (int ii = 1; ii <= nodes_in_brick; ii++)
    {
        ::printf("Global# %d Local#%d  %+0.5e %+0.5e %+0.5e\n",

                 connectedExternalNodes(ii - 1),
                 ii, total_displacements.val(ii, 1),
                 total_displacements.val(ii, 2),
                 total_displacements.val(ii, 3));
    }

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );

                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                dh = dh_drst_at(r, s, t);

                Jacobian = Jacobian_3D(dh);

                JacobianINV = Jacobian_3Dinv(dh);

                dhGlobal = dh("ij") * JacobianINV("kj");

                strain = (dhGlobal("ib") * total_displacements("ia")).symmetrize11();
                strain.null_indices();

                Constitutive =  ( matpoint[where]->matmodel)->getTangentTensor();

                stress = Constitutive("ijkl") * strain("kl");
                stress.null_indices();

                ::printf("\n  strain tensor at GAUSS point %d \n", where + 1);
                strain.reportshort("");
                ::printf("\n  stress tensor at GAUSS point %d \n", where + 1);
                stress.reportshort("");


            }
        }
    }

}



tensor VariableNodeBrick::stiffness_matrix(const tensor &K)
{

    matrix Kmatrix(nodes_in_brick * 3, nodes_in_brick * 3, 0.0);

    int Ki = 0;
    int Kj = 0;

    for ( int i = 1 ; i <= nodes_in_brick ; i++)
    {
        for ( int j = 1 ; j <= nodes_in_brick ; j++)
        {
            for ( int k = 1 ; k <= 3 ; k++)
            {
                for ( int l = 1 ; l <= 3 ; l++)
                {
                    Ki = k + 3 * (i - 1);
                    Kj = l + 3 * (j - 1);


                    Kmatrix.val( Ki , Kj ) = K.cval(i, k, l, j);
                }
            }
        }
    }

    return Kmatrix;

}



tensor VariableNodeBrick::mass_matrix(const tensor &M)
{

    matrix Mmatrix(nodes_in_brick * 3, nodes_in_brick * 3, 0.0);

    for ( int i = 1 ; i <= nodes_in_brick * 3 ; i++)
    {
        for ( int j = 1 ; j <= nodes_in_brick * 3 ; j++)
        {
            Mmatrix.val(i, j) = M.cval(i, j);

        }
    }

    return Mmatrix;

}



tensor VariableNodeBrick::Jacobian_3D(tensor dh)
{
    tensor N_C = Nodal_Coordinates();
    tensor Jacobian_3D = dh("ij") * N_C("ik");
    return Jacobian_3D;
}


tensor VariableNodeBrick::Jacobian_3Dinv(tensor dh)
{
    tensor N_C = Nodal_Coordinates();
    tensor Jacobian_3Dinv = (dh("ij") * N_C("ik")).inverse();
    return Jacobian_3Dinv;
}



tensor VariableNodeBrick::Nodal_Coordinates(void)
{
    const int dimensions[] = {27, 3};
    tensor N_coord(2, dimensions, 0.0);

    for ( int i = 0 ; i < 27 ; i++)
    {
        if (theNodes[i] != 0)
        {
            N_coord.val(i + 1, 1) = ( theNodes[i]->getCrds() )(0);
            N_coord.val(i + 1, 2) = ( theNodes[i]->getCrds() )(1);
            N_coord.val(i + 1, 3) = ( theNodes[i]->getCrds() )(2);
        }
        else
        {
            N_coord.val(i + 1, 1) = 0;
            N_coord.val(i + 1, 2) = 0;
            N_coord.val(i + 1, 3) = 0;
        }
    }

    return N_coord;
}



////#############################################################################
tensor VariableNodeBrick::incr_disp(void)
{

    const int dimensions[] = {27, 3};
    tensor increment_disp(2, dimensions, 0.0);

    for ( int i = 0 ; i < 27 ; i++)
    {
        if (connectedExternalNodes(i) != 0)
        {
            increment_disp.val(i + 1, 1) = ( theNodes[i]->getIncrDeltaDisp() )(0);
            increment_disp.val(i + 1, 2) = ( theNodes[i]->getIncrDeltaDisp() )(1);
            increment_disp.val(i + 1, 3) = ( theNodes[i]->getIncrDeltaDisp() )(2);
        }
        else
        {
            increment_disp.val(i + 1, 1) = 0;
            increment_disp.val(i + 1, 2) = 0;
            increment_disp.val(i + 1, 3) = 0;
        }
    }

    return increment_disp;


}

////#############################################################################
tensor VariableNodeBrick::total_disp(void)
{

    const int dimensions[] = {nodes_in_brick, 3};
    tensor total_disp(2, dimensions, 0.0);

    for ( int i = 0 ; i < nodes_in_brick ; i++)
    {
        total_disp.val(i + 1, 1) = ( theNodes[i]->getTrialDisp() )(0);
        total_disp.val(i + 1, 2) = ( theNodes[i]->getTrialDisp() )(1);
        total_disp.val(i + 1, 3) = ( theNodes[i]->getTrialDisp() )(2);
    }

    return total_disp;

}


////#############################################################################
tensor VariableNodeBrick::total_disp(FILE *fp, double *u)
{


    const int dimensions[] = {nodes_in_brick, 3};
    tensor total_disp(2, dimensions, 0.0);

    for ( int i = 0 ; i < nodes_in_brick ; i++)
    {
        total_disp.val(i + 1, 1) = ( theNodes[i]->getTrialDisp() )(0);
        total_disp.val(i + 1, 2) = ( theNodes[i]->getTrialDisp() )(1);
        total_disp.val(i + 1, 3) = ( theNodes[i]->getTrialDisp() )(2);
    }

    return total_disp;

}


////#############################################################################
int VariableNodeBrick::get_global_number_of_node(int local_node_number)
{

    return (*existing_node_numbers)(local_node_number);

}

////#############################################################################
int  VariableNodeBrick::get_Brick_Number(void)
{

    return this->getTag();

}

////#############################################################################
tensor VariableNodeBrick::nodal_forces(void)
{

    int force_dim[] = {27, 3};

    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {27, 3};

    tensor dh(2, dh_dim, 0.0);

    stresstensor stress_at_GP(0.0);

    double det_of_Jacobian = 0.0;


    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                Jacobian = Jacobian_3D(dh);

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("kj");

                weight = rw * sw * tw * det_of_Jacobian;

                stress_at_GP = matpoint[where]->getStressTensor();

                nodal_forces = nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;

            }
        }
    }

    return nodal_forces;

}




tensor VariableNodeBrick::iterative_nodal_forces(void)
{

    int force_dim[] = {nodes_in_brick, 3};

    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {27, 3};

    tensor dh(2, dh_dim, 0.0);

    stresstensor stress_at_GP(0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("kj");

                weight = rw * sw * tw * det_of_Jacobian;

                stress_at_GP = matpoint[where]->getStressTensor();
                stress_at_GP.reportshortpqtheta("\n iterative_stress at GAUSS point in iterative_nodal_force\n");

                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces = nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;

            }
        }
    }


    return nodal_forces;

}




tensor VariableNodeBrick::nodal_forces_from_stress(stresstensor &stress)
{
    int force_dim[] = {nodes_in_brick, 3};

    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    double weight = 0.0;

    int dh_dim[] = {27, 3};

    tensor dh(2, dh_dim, 0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("kj");

                weight = rw * sw * tw * det_of_Jacobian;


                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces = nodal_forces + dhGlobal("ib") * stress("ab") * weight;

            }
        }
    }

    return nodal_forces;

}




tensor VariableNodeBrick::linearized_nodal_forces(void)
{
    int force_dim[] = {nodes_in_brick, 3};

    tensor linearized_nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {27, 3};

    tensor dh(2, dh_dim, 0.0);

    tensor Constitutive( 4, def_dim_4, 0.0);

    double det_of_Jacobian = 0.0;

    static int disp_dim[] = {nodes_in_brick, 3};

    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    stresstensor final_linearized_stress;
    incremental_displacements = incr_disp();

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("kj");

                weight = rw * sw * tw * det_of_Jacobian;

                // incremental straines at this Gauss point
                // now in Update we know the incremental displacements so let's find
                // the incremental strain
                incremental_strain = (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();

                if ( ! (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)  )
                {
                    cerr << "VariableNodeBrick::linearized_nodal_forces (tag: " << this->getTag() << "), not converged\n";
                }

                Constitutive = (matpoint[where]->matmodel)->getTangentTensor();

                final_linearized_stress =
                    Constitutive("ijkl") * incremental_strain("kl");

                // nodal forces See Zienkievicz part 1 pp 108
                linearized_nodal_forces = linearized_nodal_forces +
                                          dhGlobal("ib") * final_linearized_stress("ab") * weight;

            }
        }
    }


    return linearized_nodal_forces;

}




void VariableNodeBrick::computeGaussPoint()
{
    int count;
    count = integration_order * integration_order * integration_order;
    (*GaussCoord)(0) = count;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    static const int dim[] = {3, nodes_in_brick};
    static const int dim27[] = {3, count};
    tensor NodalCoord(2, dim, 0.0);
    tensor matpointCoord(2, dim27, 0.0);
    int h_dim[] = {81, 3};
    tensor H(2, h_dim, 0.0);


    for ( int i = 0 ; i < nodes_in_brick ; i++)
    {
        NodalCoord.val(i + 1, 1) = ( theNodes[i]->getCrds() )(0);
        NodalCoord.val(i + 1, 2) = ( theNodes[i]->getCrds() )(1);
        NodalCoord.val(i + 1, 3) = ( theNodes[i]->getCrds() )(2);
    }



    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                // derivatives of local coordinates with respect to local coordinates
                H = H_3D(r, s, t);


                for (int encount = 1 ; encount <= nodes_in_brick; encount++ )
                {
                    matpointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
                    matpointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
                    matpointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);
                }


                (*GaussCoord)(where * 3 + 1) = matpointCoord.val(1, where + 1);
                (*GaussCoord)(where * 3 + 2) = matpointCoord.val(2, where + 1);
                (*GaussCoord)(where * 3 + 3) = matpointCoord.val(3, where + 1);

            }
        }
    }

}




void VariableNodeBrick::reportTensorF(FILE *fp)
{

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    int count;
    count = integration_order * integration_order * integration_order;


    static const int dim[] = {3, nodes_in_brick};
    static const int dim27[] = {3, count};
    tensor NodalCoord(2, dim, 0.0);
    tensor matpointCoord(2, dim27, 0.0);
    int h_dim[] = {81, 3};
    tensor H(2, h_dim, 0.0);


    for ( int i = 0 ; i < nodes_in_brick ; i++)
    {
        NodalCoord.val(i + 1, 1) = ( theNodes[i]->getCrds() )(0);
        NodalCoord.val(i + 1, 2) = ( theNodes[i]->getCrds() )(1);
        NodalCoord.val(i + 1, 3) = ( theNodes[i]->getCrds() )(2);
    }



    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                // derivatives of local coordinates with respect to local coordinates

                H = H_3D(r, s, t);

                for (int encount = 1 ; encount <= nodes_in_brick ; encount++ )
                {
                    matpointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
                    matpointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
                    matpointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);
                }

                fprintf(fp, "gauss point# %d   %+.6e %+.6e %+.6e \n", where + 1,
                        matpointCoord.val(1, where + 1),
                        matpointCoord.val(2, where + 1),
                        matpointCoord.val(3, where + 1));


            }
        }
    }

}



//=============================================================================
int VariableNodeBrick::getNumExternalNodes () const
{
    return nodes_in_brick;
}


//=============================================================================
const ID &VariableNodeBrick::getExternalNodes ()
{
    return *existing_node_numbers;
}


Node **
VariableNodeBrick::getNodePtrs(void)
{
    return theNodes;
}

//=============================================================================
int VariableNodeBrick::getNumDOF ()
{
    return 3 * nodes_in_brick;
}

//=============================================================================
void VariableNodeBrick::setDomain (Domain *theDomain)
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
            theNodes[i] = theDomain->getNode( connectedExternalNodes(i) );

            if (connectedExternalNodes(i) != 0)
            {
                if (theNodes[i] == 0)
                {
                    cerr << "VariableNodeBrick (tag: " << this->getTag() << " ) does not have node " <<  (*existing_node_numbers)(i) << "\n";
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
                    cerr << "VariableNodeBrick (tag: " << this->getTag() << ") has differing number of DOFs at its nodes\n";
                    exit(-1);
                }
            }
        }



        this->DomainComponent::setDomain(theDomain);
    }

}

//=============================================================================
int VariableNodeBrick::commitState ()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "VariableNodeBrick::commitState () - failed in base class";
    }


    Vector pp = getResistingForce();

    return retVal;

}

//=============================================================================
int VariableNodeBrick::revertToLastCommit ()
{
    int i;
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states

    for (i = 0; i < Num_TotalGaussPts; i++)
    {
        retVal += matpoint[i]->revertToLastCommit();
    }


    return retVal;
}

//=============================================================================
int VariableNodeBrick::revertToStart ()
{
    int retVal = 0;

    for (int i = 0; i < Num_TotalGaussPts; i++)
    {
        retVal += matpoint[i]->revertToStart();
    }


    return retVal;
}


//=============================================================================
const Matrix &VariableNodeBrick::getTangentStiff()
{
    tensor stifftensor = getStiffnessTensor();
    int Ki = 0;
    int Kj = 0;

    static Matrix K_temp(27 * 3, 27 * 3);
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
                    (K_temp)( Ki - 1 , Kj - 1 ) = stifftensor.cval(i, k, l, j);
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








    //      cerr << " K " << (&K) << endln;
    //      (*K).Output(cerr);

    //    for ( int i=0 ; i<nodes_in_brick*3 ; i++)
    //       {
    //         for ( int j=0 ; j<nodes_in_brick*3 ; j++)
    //             {
    //                cout << "k(" << i+1 << "," << j+1 << "): " << (*K)(i,j) << endl;
    //      }
    //       }



    return *K;
}

//=============================================================================
const Matrix &VariableNodeBrick::getInitialStiff ()
{
    return this->getTangentStiff();
}


//=============================================================================
const Matrix &VariableNodeBrick::getMassFullSize()
{

    int M_dim[] = {81, 81};
    tensor Mm(2, M_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    double weight = 0.0;

    int dh_dim[] = {27, 3};

    tensor dh(2, dh_dim, 0.0);

    int h_dim[] = {81, 3};
    tensor H(2, h_dim, 0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;

    static Matrix M_temp(27 * 3, 27 * 3);
    M_temp.Zero();


    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();

                H = H_3D(r, s, t);

                weight = rw * sw * tw * rho * det_of_Jacobian;

                Mm = Mm + H("ib") * H("kb") * weight;

            }
        }
    }



    for ( int i = 1 ; i <= 27 * 3 ; i++ )
    {
        for ( int j = 1 ; j <= 27 * 3 ; j++ )
        {
            (M_temp)( i - 1 , j - 1 ) = Mm.cval(i, j);
        }
    }


    return M_temp;
}

//=============================================================================
const Matrix &VariableNodeBrick::getMass()
{

    static Matrix M_temp(27 * 3, 27 * 3);
    M_temp.Zero();

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



    //    for ( int i=0 ; i<nodes_in_brick*3 ; i++)
    //       {
    //         for ( int j=0 ; j<nodes_in_brick*3 ; j++)
    //             {
    //                cout << "M(" << i+1 << "," << j+1 << "): " << (*M)(i,j) << endl;
    //      }
    //       }


    return *M;
}

//=============================================================================
void VariableNodeBrick::zeroLoad(void)
{
    (*Q).Zero();
}



//======================================================================
const Vector &VariableNodeBrick::getBodyForce(double loadFactor, const Vector &data)
{

    static Vector bforce(nodes_in_brick * 3);
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
    bfx(0) = bf(0) * loadFactor;
    bfx(1) = bf(1) * loadFactor;
    bfx(2) = bf(2) * loadFactor;


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


    bforce.addMatrixVector(0.0, this->getMass(), ba, 1.0);

    return bforce;

}


//======================================================================
const Vector &VariableNodeBrick::getSurfaceForce(double loadFactor, const Vector &data)
{
    cerr << "VariableNodeBrick::getSurfaceForce() haven't implemented yet!\n";
    (*tempZero).Zero();
    return *tempZero;
}



//=============================================================================
int VariableNodeBrick::addLoad(ElementalLoad *theLoad, double loadFactor)
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
        cerr << "VariableNodeBrick::addLoad() - 27NodeBrick " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }


    return 0;
}



//=============================================================================
int VariableNodeBrick::addInertiaLoadToUnbalance(const Vector &accel)
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
const Vector VariableNodeBrick::FormEquiBodyForce(const Vector &data)
{

    static Vector bforce(nodes_in_brick * 3);
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
const Vector &VariableNodeBrick::getResistingForce()
{

    (*P).Zero();

    static Vector P_temp(27 * 3);
    P_temp.Zero();


    int force_dim[] = {27, 3};
    tensor nodalforces(2, force_dim, 0.0);

    nodalforces = nodal_forces();

    //converting nodalforce tensor to vector
    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            (P_temp)(i * 3 + j) = nodalforces.cval(i + 1, j + 1);
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
const Vector &VariableNodeBrick::getResistingForceIncInertia()
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

int VariableNodeBrick::sendSelf (int commitTag, Channel &theChannel)
{
    cerr << "VariableNodeBrick::sendSelf() haven't implemented yet!\n";
    return 0;
}



int VariableNodeBrick::recvSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    cerr << "VariableNodeBrick::recvSelf() haven't implemented yet!\n";
    return 0;
}



//=============================================================================
void VariableNodeBrick::Print(ostream &s, int flag)
{
    s << "VariableNodeBrick, element id:  " << this->getTag() << endln;
    s << "Connected external nodes:  " << connectedExternalNodes;

    //     int total_number_of_Gauss_points = integration_order*integration_order*integration_order;

    //     if ( total_number_of_Gauss_points != 0 )
    //     {
    //       for (int i=0; i<nodes_in_brick; i++)
    //       {
    //          theNodes[i]->Print(cerr);
    //       }
    //
    //     }

    s << "Element mass density:  " << rho << endln << endln;
    s << "Material model:  " << endln;


    for ( int GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        for ( int GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            for ( int GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                short where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                s << "\n where = " << where << endln;
                s << " GP_c_r= " << GP_c_r << "GP_c_s = " << GP_c_s << " GP_c_t = " << GP_c_t << endln;
                matpoint[where]->report("Material Point\n");

            }
        }
    }

}

//=============================================================================
// Response* VariableNodeBrick::setResponse (const char** argv, int argc, Information& eleInformation)
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
// int VariableNodeBrick::getResponse (int responseID, Information& eleInfo)
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


double VariableNodeBrick::get_Gauss_p_c(short order, short point_numb)
{

    //  Abscissae coefficient of the Gaussian quadrature formula
    // starting from 1 not from 0
    static double Gauss_coordinates[7][7];

    Gauss_coordinates[1][1] = 0.0 ;
    Gauss_coordinates[2][1] = -0.577350269189626;
    Gauss_coordinates[2][2] = -Gauss_coordinates[2][1];
    Gauss_coordinates[3][1] = -0.774596669241483;
    Gauss_coordinates[3][2] = 0.0;
    Gauss_coordinates[3][3] = -Gauss_coordinates[3][1];
    Gauss_coordinates[4][1] = -0.861136311594053;
    Gauss_coordinates[4][2] = -0.339981043584856;
    Gauss_coordinates[4][3] = -Gauss_coordinates[4][2];
    Gauss_coordinates[4][4] = -Gauss_coordinates[4][1];
    Gauss_coordinates[5][1] = -0.906179845938664;
    Gauss_coordinates[5][2] = -0.538469310105683;
    Gauss_coordinates[5][3] = 0.0;
    Gauss_coordinates[5][4] = -Gauss_coordinates[5][2];
    Gauss_coordinates[5][5] = -Gauss_coordinates[5][1];
    Gauss_coordinates[6][1] = -0.932469514203152;
    Gauss_coordinates[6][2] = -0.661509386466265;
    Gauss_coordinates[6][3] = -0.238619188183197;
    Gauss_coordinates[6][4] = -Gauss_coordinates[6][3];
    Gauss_coordinates[6][5] = -Gauss_coordinates[6][2];
    Gauss_coordinates[6][6] = -Gauss_coordinates[6][1];

    return Gauss_coordinates[order][point_numb];

}

double VariableNodeBrick::get_Gauss_p_w(short order, short point_numb)
{

    //  Weight coefficient of the Gaussian quadrature formula
    // starting from 1 not from 0
    static double Gauss_weights[7][7]; // static data ??

    Gauss_weights[1][1] = 2.0;
    Gauss_weights[2][1] = 1.0;
    Gauss_weights[2][2] = 1.0;
    Gauss_weights[3][1] = 0.555555555555556;
    Gauss_weights[3][2] = 0.888888888888889;
    Gauss_weights[3][3] = Gauss_weights[3][1];
    Gauss_weights[4][1] = 0.347854845137454;
    Gauss_weights[4][2] = 0.652145154862546;
    Gauss_weights[4][3] = Gauss_weights[4][2];
    Gauss_weights[4][4] = Gauss_weights[4][1];
    Gauss_weights[5][1] = 0.236926885056189;
    Gauss_weights[5][2] = 0.478628670499366;
    Gauss_weights[5][3] = 0.568888888888889;
    Gauss_weights[5][4] = Gauss_weights[5][2];
    Gauss_weights[5][5] = Gauss_weights[5][1];
    Gauss_weights[6][1] = 0.171324492379170;
    Gauss_weights[6][2] = 0.381761573048139;
    Gauss_weights[6][3] = 0.467913934572691;
    Gauss_weights[6][4] = Gauss_weights[6][3];
    Gauss_weights[6][5] = Gauss_weights[6][2];
    Gauss_weights[6][6] = Gauss_weights[6][1];

    return Gauss_weights[order][point_numb];

}



int VariableNodeBrick::update(void)
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    int dh_dim[] = {27, 3};
    tensor dh(2, dh_dim, 0.0);

    static int disp_dim[] = {nodes_in_brick, 3};
    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    incremental_displacements = incr_disp();

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );

                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                Jacobian = Jacobian_3D(dh);

                JacobianINV = Jacobian_3Dinv(dh);

                dhGlobal = dh("ij") * JacobianINV("kj");

                incremental_strain = (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();

                if ( ( (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)) )
                {
                    cerr << "VariableNodeBrick::update (tag: " << this->getTag() << "), update() failed\n";
                }

            }
        }
    }

    return 0;

}




int
VariableNodeBrick::CheckMesh(ofstream &checkmesh_file)
{

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    int dh_dim[] = {27, 3};
    tensor dh(2, dh_dim, 0.0);


    double det_of_Jacobian = 0.0;
    bool jacobian_flag = false;

    tensor Jacobian;


    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );

                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);


                // Jacobian tensor (matrix)
                Jacobian = Jacobian_3D(dh);


                // determinant of Jacobian tensor (matrix)
                det_of_Jacobian  = Jacobian.determinant();


                if (det_of_Jacobian <= 0)
                {
                    checkmesh_file << "ERROR!!! Element number (" << this->getTag() << "): has negative Jacobian determinant!" << endl;
                    jacobian_flag = true;
                    break;
                }


            }

            if (jacobian_flag == true)
            {
                break;
            }

        }

        if (jacobian_flag == true)
        {
            break;
        }

    }

    return 0;

}

//==================================================================================
Vector *
VariableNodeBrick::getStress(void)
{

    int i = 0;
    int j = 0;
    stresstensor sts;
    Vector *stresses = new Vector(nodes_in_brick * 6);


    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                i = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                sts = matpoint[i]->getStressTensor();
                (*stresses)(j * 6 + 0) = sts.cval(1, 1); //sigma_xx
                (*stresses)(j * 6 + 1) = sts.cval(2, 2); //sigma_yy
                (*stresses)(j * 6 + 2) = sts.cval(3, 3); //sigma_zz
                (*stresses)(j * 6 + 3) = sts.cval(1, 2); //sigma_xy
                (*stresses)(j * 6 + 4) = sts.cval(1, 3); //sigma_xz
                (*stresses)(j * 6 + 5) = sts.cval(2, 3); //sigma_yz

                j++;
            }
        }
    }


    return stresses;

}


#endif



