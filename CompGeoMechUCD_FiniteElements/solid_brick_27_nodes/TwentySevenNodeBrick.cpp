///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              TwentySevenNodeBrick.cpp
// CLASS:             TwentySevenNodeBrick
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         all
// DESIGNER:          Boris Jeremic, Guanzhou Jie
// PROGRAMMER:        Guanzhou Jie and Boris Jeremic
// DATE:              Oct. 2003
// UPDATE HISTORY:    May 2004 Guanzhou added update()
//                    September 2010 Nima removed density as an input argument
//                    October 2010 Nima removed pressure as an input argument
//                    November 2011 Nima changed the order of the nodes to be
//                    consistent with 8node and 20node brick elements
//                    July 2012 Nima added the surface load
//
///////////////////////////////////////////////////////////////////////////////


#ifndef TWENTYSEVENNODEBRICK_CPP
#define TWENTYSEVENNODEBRICK_CPP

#include <NDMaterial.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>
#include <Domain.h>
#include <string.h>
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>

#include <TwentySevenNodeBrick.h>
#include <ElementalLoad.h>

Matrix TwentySevenNodeBrick::K(81, 81);
Matrix TwentySevenNodeBrick::C(81, 81);
Matrix TwentySevenNodeBrick::M(81, 81);
Vector TwentySevenNodeBrick::P(81);

Vector TwentySevenNodeBrick::ShapeFunctionValues_in_function(9);
Vector TwentySevenNodeBrick::J_vector_in_function(3);
double TwentySevenNodeBrick::SurfaceLoadValues_in_function;


//====================================================================
// Constructor
//====================================================================

TwentySevenNodeBrick::TwentySevenNodeBrick(int element_number,
        int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
        int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
        int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
        int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
        int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
        int node_numb_21,  int node_numb_22,  int node_numb_23,  int node_numb_24,
        int node_numb_25,  int node_numb_26,  int node_numb_27,
        NDMaterial *Globalmmodel)

    : Element(element_number, ELE_TAG_TwentySevenNodeBrick ),
      connectedExternalNodes(27),
      Ki(0),
      Q(81),
      bf(3),
      rho(0.0),
      gauss_points(TwentySevenNodeBrick_NUMBER_OF_GAUSSPOINTS, 3),
      outputVector(TwentySevenNodeBrick_OUTPUT_SIZE)
{

    rho = Globalmmodel->getRho();
    determinant_of_Jacobian = 0.0;

    matpoint  = new MatPoint3D * [27];


    short where = 0;
    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        double r = get_Gauss_p_c( 3, GP_c_r );
        double rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            double s = get_Gauss_p_c( 3, GP_c_s );
            double sw = get_Gauss_p_w( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                double t = get_Gauss_p_c( 3, GP_c_t );
                double tw = get_Gauss_p_w( 3, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

                matpoint[where] = new MatPoint3D(GP_c_r,
                                                 GP_c_s,
                                                 GP_c_t,
                                                 r, s, t,
                                                 rw, sw, tw,
                                                 Globalmmodel);
            }
        }
    }

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

    connectedExternalNodes(20) = node_numb_21;
    connectedExternalNodes(21) = node_numb_22;
    connectedExternalNodes(22) = node_numb_23;
    connectedExternalNodes(23) = node_numb_24;
    connectedExternalNodes(24) = node_numb_25;
    connectedExternalNodes(25) = node_numb_26;
    connectedExternalNodes(26) = node_numb_27;

    for (int i = 0; i < 27; i++)
    {
        theNodes[i] = 0;
    }

}

//====================================================================
TwentySevenNodeBrick::TwentySevenNodeBrick (): Element(0, ELE_TAG_TwentySevenNodeBrick ),
    connectedExternalNodes(27), Ki(0), Q(81), bf(3), rho(0.0), mmodel(0),
    gauss_points(TwentySevenNodeBrick_NUMBER_OF_GAUSSPOINTS, 3),
    outputVector(TwentySevenNodeBrick_OUTPUT_SIZE)
{
    matpoint  = new MatPoint3D * [27];

    for (int i = 0; i < 27; i++)
    {
        theNodes[i] = 0;
    }
}


//#############################################################################


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TwentySevenNodeBrick::~TwentySevenNodeBrick ()
{

    for (int i = 0; i < 27; i++)
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

    if (Ki != 0)
    {
        delete Ki;
    }

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void TwentySevenNodeBrick::incremental_Update()
{
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    int dh_dim[] = {27, 3};
    tensor dh(2, dh_dim, 0.0);

    static int disp_dim[] = {27, 3};
    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    incremental_displacements = incr_disp();

    short where = 0;
    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );

                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);

                dhGlobal = dh("ij") * JacobianINV("kj");


                // incremental straines at this Gauss point
                // now in Update we know the incremental displacements so let's find
                // the incremental strain
                incremental_strain =
                    (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();

                // here comes the final_stress calculation actually on only needs to copy stresses
                // from the iterative data . . .


                if ( ! ( (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)) )
                {
                    cerr << "TwentySevenNodeBrick::incremental_Update (tag: " << this->getTag() << "), not converged\n";
                }
            }
        }
    }
}



//#############################################################################
//#############################################################################


tensor TwentySevenNodeBrick::H_3D(double r1, double r2, double r3)
{

    int dimension[] = {81, 3};

    tensor H(2, dimension, 0.0);

    //Shape Functions of Node 1 Along Three Coordinate Directions
    H.val(1, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    H.val(2, 2) = H.val(1, 1);
    H.val(3, 3) = H.val(1, 1);

    //Shape Functions of Node 2 Along Three Coordinate Directions
    H.val(4, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    H.val(5, 2) = H.val(4, 1);
    H.val(6, 3) = H.val(4, 1);

    //Shape Functions of Node 3 Along Three Coordinate Directions
    H.val(7, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    H.val(8, 2) = H.val(7, 1);
    H.val(9, 3) = H.val(7, 1);

    //Shape Functions of Node 4 Along Three Coordinate Directions
    H.val(10, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    H.val(11, 2) = H.val(10, 1);
    H.val(12, 3) = H.val(10, 1);

    //Shape Functions of Node 5 Along Three Coordinate Directions
    H.val(13, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    H.val(14, 2) = H.val(13, 1);
    H.val(15, 3) = H.val(13, 1);

    //Shape Functions of Node 6 Along Three Coordinate Directions
    H.val(16, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    H.val(17, 2) = H.val(16, 1);
    H.val(18, 3) = H.val(16, 1);

    //Shape Functions of Node 7 Along Three Coordinate Directions
    H.val(19, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    H.val(20, 2) = H.val(19, 1);
    H.val(21, 3) = H.val(19, 1);


    //Shape Functions of Node 8 Along Three Coordinate Directions
    H.val(22, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    H.val(23, 2) = H.val(22, 1);
    H.val(24, 3) = H.val(22, 1);


    //Shape Functions of Node 9 Along Three Coordinate Directions
    H.val(25, 1) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    H.val(26, 2) = H.val(25, 1);
    H.val(27, 3) = H.val(25, 1);


    //Shape Functions of Node 10 Along Three Coordinate Directions
    H.val(28, 1) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H.val(29, 2) = H.val(28, 1);
    H.val(30, 3) = H.val(28, 1);


    //Shape Functions of Node 11 Along Three Coordinate Directions
    H.val(31, 1) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    H.val(32, 2) = H.val(31, 1);
    H.val(33, 3) = H.val(31, 1);


    //Shape Functions of Node 12 Along Three Coordinate Directions
    H.val(34, 1) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H.val(35, 2) = H.val(34, 1);
    H.val(36, 3) = H.val(34, 1);


    //Shape Functions of Node 13 Along Three Coordinate Directions
    H.val(37, 1) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    H.val(38, 2) = H.val(37, 1);
    H.val(39, 3) = H.val(37, 1);


    //Shape Functions of Node 14 Along Three Coordinate Directions
    H.val(40, 1) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H.val(41, 2) = H.val(40, 1);
    H.val(42, 3) = H.val(40, 1);


    //Shape Functions of Node 15 Along Three Coordinate Directions
    H.val(43, 1) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    H.val(44, 2) = H.val(43, 1);
    H.val(45, 3) = H.val(43, 1);


    //Shape Functions of Node 16 Along Three Coordinate Directions
    H.val(46, 1) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H.val(47, 2) = H.val(46, 1);
    H.val(48, 3) = H.val(46, 1);


    //Shape Functions of Node 17 Along Three Coordinate Directions
    H.val(49, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H.val(50, 2) = H.val(49, 1);
    H.val(51, 3) = H.val(49, 1);


    //Shape Functions of Node 18 Along Three Coordinate Directions
    H.val(52, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H.val(53, 2) = H.val(52, 1);
    H.val(54, 3) = H.val(52, 1);


    //Shape Functions of Node 19 Along Three Coordinate Directions
    H.val(55, 1) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H.val(56, 2) = H.val(55, 1);
    H.val(57, 3) = H.val(55, 1);


    //Shape Functions of Node 20 Along Three Coordinate Directions
    H.val(58, 1) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H.val(59, 2) = H.val(58, 1);
    H.val(60, 3) = H.val(58, 1);


    //Shape Functions of Node 21 Along Three Coordinate Directions
    H.val(61, 1) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H.val(62, 2) = H.val(61, 1);
    H.val(63, 3) = H.val(61, 1);


    //Shape Functions of Node 22 Along Three Coordinate Directions
    H.val(64, 1) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    H.val(65, 2) = H.val(64, 1);
    H.val(66, 3) = H.val(64, 1);


    //Shape Functions of Node 23 Along Three Coordinate Directions
    H.val(67, 1) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H.val(68, 2) = H.val(67, 1);
    H.val(69, 3) = H.val(67, 1);


    //Shape Functions of Node 24 Along Three Coordinate Directions
    H.val(70, 1) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    H.val(71, 2) = H.val(70, 1);
    H.val(72, 3) = H.val(70, 1);


    //Shape Functions of Node 25 Along Three Coordinate Directions
    H.val(73, 1) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    H.val(74, 2) = H.val(73, 1);
    H.val(75, 3) = H.val(73, 1);


    //Shape Functions of Node 26 Along Three Coordinate Directions
    H.val(76, 1) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    H.val(77, 2) = H.val(76, 1);
    H.val(78, 3) = H.val(76, 1);


    //Shape Functions of Node 27 Along Three Coordinate Directions
    H.val(79, 1) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    H.val(80, 2) = H.val(79, 1);
    H.val(81, 3) = H.val(79, 1);


    //     H.print("H","\n\n H \n");


    return H;
}

tensor TwentySevenNodeBrick::dh_drst_at(double r1, double r2, double r3)
{

    int dimensions[] = {27, 3}; // Changed from{20,3} to {27,3} Guanzhou Oct. 2003
    tensor dh(2, dimensions, 0.0);


    //Shape Functions of Node 1 Along Three Coordinate Directions
    dh.val(1, 1) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(1, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(1, 3) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 + 1.0);

    //Shape Functions of Node 2 Along Three Coordinate Directions
    dh.val(2, 1) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(2, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(2, 3) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 + 1.0);

    //Shape Functions of Node 3 Along Three Coordinate Directions
    dh.val(3, 1) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(3, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(3, 3) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 + 1.0);

    //Shape Functions of Node 4 Along Three Coordinate Directions
    dh.val(4, 1) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(4, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(4, 3) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 + 1.0);

    //Shape Functions of Node 5 Along Three Coordinate Directions
    dh.val(5, 1) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(5, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(5, 3) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 - 1.0);

    //Shape Functions of Node 6 Along Three Coordinate Directions
    dh.val(6, 1) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(6, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(6, 3) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 - 1.0);

    //Shape Functions of Node 7 Along Three Coordinate Directions
    dh.val(7, 1) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(7, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(7, 3) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 8 Along Three Coordinate Directions
    dh.val(8, 1) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(8, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(8, 3) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 9 Along Three Coordinate Directions
    dh.val(9, 1) = (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(9, 2) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(9, 3) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 10 Along Three Coordinate Directions
    dh.val(10, 1) = 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh.val(10, 2) = 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh.val(10, 3) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 11 Along Three Coordinate Directions
    dh.val(11, 1) = (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(11, 2) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 + 1.0);
    dh.val(11, 3) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 12 Along Three Coordinate Directions
    dh.val(12, 1) = 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh.val(12, 2) = 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh.val(12, 3) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 13 Along Three Coordinate Directions
    dh.val(13, 1) = (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(13, 2) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(13, 3) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 14 Along Three Coordinate Directions
    dh.val(14, 1) = 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh.val(14, 2) = 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh.val(14, 3) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 15 Along Three Coordinate Directions
    dh.val(15, 1) = (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(15, 2) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * 0.5 * r3 * (r3 - 1.0);
    dh.val(15, 3) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 16 Along Three Coordinate Directions
    dh.val(16, 1) = 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh.val(16, 2) = 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh.val(16, 3) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);


    //Shape Functions of Node 17 Along Three Coordinate Directions
    dh.val(17, 1) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh.val(17, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh.val(17, 3) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    //Shape Functions of Node 18 Along Three Coordinate Directions
    dh.val(18, 1) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh.val(18, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh.val(18, 3) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    //Shape Functions of Node 19 Along Three Coordinate Directions
    dh.val(19, 1) = 0.5 * (2.0 * r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh.val(19, 2) = 0.5 * r1 * (r1 - 1.0) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh.val(19, 3) = 0.5 * r1 * (r1 - 1.0) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    //Shape Functions of Node 20 Along Three Coordinate Directions
    dh.val(20, 1) = 0.5 * (2.0 * r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh.val(20, 2) = 0.5 * r1 * (r1 + 1.0) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh.val(20, 3) = 0.5 * r1 * (r1 + 1.0) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    //Shape Functions of Node 21 Along Three Coordinate Directions
    dh.val(21, 1) = (-2.0 * r1) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh.val(21, 2) = (1.0 - r1 * r1) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh.val(21, 3) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * (-2.0 * r3);


    //Shape Functions of Node 22 Along Three Coordinate Directions
    dh.val(22, 1) = (-2.0 * r1) * 0.5 * r2 * (r2 + 1.0) * (1.0 - r3 * r3);
    dh.val(22, 2) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 + 1.0) * (1.0 - r3 * r3);
    dh.val(22, 3) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 + 1.0) * (-2.0 * r3);


    //Shape Functions of Node 23 Along Three Coordinate Directions
    dh.val(23, 1) = 0.5 * (2.0 * r1 - 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh.val(23, 2) = 0.5 * r1 * (r1 - 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh.val(23, 3) = 0.5 * r1 * (r1 - 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    //Shape Functions of Node 24 Along Three Coordinate Directions
    dh.val(24, 1) = (-2.0 * r1) * 0.5 * r2 * (r2 - 1.0) * (1.0 - r3 * r3);
    dh.val(24, 2) = (1.0 - r1 * r1) * 0.5 * (2.0 * r2 - 1.0) * (1.0 - r3 * r3);
    dh.val(24, 3) = (1.0 - r1 * r1) * 0.5 * r2 * (r2 - 1.0) * (-2.0 * r3);


    //Shape Functions of Node 25 Along Three Coordinate Directions
    dh.val(25, 1) = 0.5 * (2.0 * r1 + 1.0) * (1.0 - r2 * r2) * (1.0 - r3 * r3);
    dh.val(25, 2) = 0.5 * r1 * (r1 + 1.0) * (-2.0 * r2) * (1.0 - r3 * r3);
    dh.val(25, 3) = 0.5 * r1 * (r1 + 1.0) * (1.0 - r2 * r2) * (-2.0 * r3);


    //Shape Functions of Node 26 Along Three Coordinate Directions
    dh.val(26, 1) = (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh.val(26, 2) = (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 + 1.0);
    dh.val(26, 3) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 + 1.0);


    //Shape Functions of Node 27 Along Three Coordinate Directions
    dh.val(27, 1) = (-2.0 * r1) * (1.0 - r2 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh.val(27, 2) = (1.0 - r1 * r1) * (-2.0 * r2) * 0.5 * r3 * (r3 - 1.0);
    dh.val(27, 3) = (1.0 - r1 * r1) * (1.0 - r2 * r2) * 0.5 * (2.0 * r3 - 1.0);

    //         dh.print("dh");

    return dh;
}


////#############################################################################
TwentySevenNodeBrick &TwentySevenNodeBrick::operator[](int subscript)
{
    return ( *(this + subscript) );
}


tensor TwentySevenNodeBrick::getStiffnessTensor(void)
{
    int K_dim[] = {27, 3, 3, 27};
    tensor Kk(4, K_dim, 0.0);
    tensor Kkt(4, K_dim, 0.0);

    //debugging
    //    matrix Kmat;

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

    static int disp_dim[] = {27, 3};
    tensor incremental_displacements(2, disp_dim, 0.0); // \Delta u

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor JacobianINVtemp;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );
        rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );
            sw = get_Gauss_p_w( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                tw = get_Gauss_p_w( 3, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);

                // Inverse of Jacobian tensor ( matrix )
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

    return Kk;
}


//#############################################################################

void TwentySevenNodeBrick::set_strain_stress_tensor(FILE *fp, double *u)
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
    tensor total_displacements(2, disp_dim, 0.0); //

    total_displacements = total_disp(fp, u);

    ::printf("\n  displacement(x-y-z) at GAUSS pt %d \n\n", where + 1);

    for (int ii = 1; ii <= 27; ii++)
    {
        ::printf("Global# %d Local#%d  %+0.5e %+0.5e %+0.5e\n",
                 //G_N_numbs[ii-1],
                 connectedExternalNodes(ii - 1),
                 ii, total_displacements.val(ii, 1),
                 total_displacements.val(ii, 2),
                 total_displacements.val(ii, 3));
    }

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);

                // determinant of Jacobian tensor ( matrix )
                dhGlobal = dh("ij") * JacobianINV("kj");

                //weight
                // strains at this Gauss point from displacement
                strain = (dhGlobal("ib") * total_displacements("ia")).symmetrize11();
                strain.null_indices();

                // here comes the final_stress calculation
                // at this Gauss point.

                // if set total displ, then it should be elstic material
                Constitutive =  ( matpoint[where]->matmodel)->getTangentTensor();

                stress = Constitutive("ijkl") * strain("kl");
                stress.null_indices();

            }
        }
    }
}


////#############################################################################
tensor TwentySevenNodeBrick::getMassTensor(void)
{
    int M_dim[] = {81, 81};
    tensor Mm(2, M_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    //     short where = 0;
    double weight = 0.0;

    int dh_dim[] = {27, 3};

    tensor dh(2, dh_dim, 0.0);

    int h_dim[] = {81, 3};
    tensor H(2, h_dim, 0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;

    double RHO;
    RHO = rho;   //global

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );
        rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );
            sw = get_Gauss_p_w( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                tw = get_Gauss_p_w( 3, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's

                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                //                dhGlobal = dh("ij") * JacobianINV("jk");
                // derivatives of local coordinates with respect to local coordinates

                H = H_3D(r, s, t);

                weight = rw * sw * tw * RHO * det_of_Jacobian;

                Mm = Mm + H("ib") * H("kb") * weight;
            }
        }
    }

    return Mm;
}


////#############################################################################

tensor TwentySevenNodeBrick::stiffness_matrix(const tensor &K)
{

    matrix Kmatrix(81, 81, 0.0);

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
                    Kmatrix.val( Ki , Kj ) = K.cval(i, k, l, j);
                }
            }
        }
    }

    return Kmatrix;
}

//#############################################################################

////#############################################################################
tensor TwentySevenNodeBrick::mass_matrix(const tensor &M)
{

    matrix Mmatrix(81, 81, 0.0);

    for ( int i = 1 ; i <= 81 ; i++ )
    {
        for ( int j = 1 ; j <= 81 ; j++ )
        {
            Mmatrix.val( i , j ) = M.cval(i, j);
        }
    }

    return Mmatrix;
}
////#############################################################################

////#############################################################################
tensor TwentySevenNodeBrick::Jacobian_3D(tensor dh)
{
    tensor N_C = Nodal_Coordinates();
    tensor Jacobian_3D = dh("ij") * N_C("ik");
    return Jacobian_3D;
}

//#############################################################################
tensor TwentySevenNodeBrick::Jacobian_3Dinv(tensor dh)
{
    tensor N_C = Nodal_Coordinates();
    tensor Jacobian_3Dinv = (dh("ij") * N_C("ik")).inverse();
    return Jacobian_3Dinv;
}


////#############################################################################
tensor TwentySevenNodeBrick::Nodal_Coordinates(void)
{
    const int dimensions[] = {27, 3};
    tensor N_coord(2, dimensions, 0.0);

    //Zhaohui using node pointers, which come from the Domain
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



    N_coord.val(1, 1) = nd1Crds(0);
    N_coord.val(1, 2) = nd1Crds(1);
    N_coord.val(1, 3) = nd1Crds(2);
    N_coord.val(2, 1) = nd2Crds(0);
    N_coord.val(2, 2) = nd2Crds(1);
    N_coord.val(2, 3) = nd2Crds(2);
    N_coord.val(3, 1) = nd3Crds(0);
    N_coord.val(3, 2) = nd3Crds(1);
    N_coord.val(3, 3) = nd3Crds(2);
    N_coord.val(4, 1) = nd4Crds(0);
    N_coord.val(4, 2) = nd4Crds(1);
    N_coord.val(4, 3) = nd4Crds(2);
    N_coord.val(5, 1) = nd5Crds(0);
    N_coord.val(5, 2) = nd5Crds(1);
    N_coord.val(5, 3) = nd5Crds(2);
    N_coord.val(6, 1) = nd6Crds(0);
    N_coord.val(6, 2) = nd6Crds(1);
    N_coord.val(6, 3) = nd6Crds(2);
    N_coord.val(7, 1) = nd7Crds(0);
    N_coord.val(7, 2) = nd7Crds(1);
    N_coord.val(7, 3) = nd7Crds(2);
    N_coord.val(8, 1) = nd8Crds(0);
    N_coord.val(8, 2) = nd8Crds(1);
    N_coord.val(8, 3) = nd8Crds(2);

    N_coord.val(9 , 1) = nd9Crds(0);
    N_coord.val(9 , 2) = nd9Crds(1);
    N_coord.val(9 , 3) = nd9Crds(2);
    N_coord.val(10, 1) = nd10Crds(0);
    N_coord.val(10, 2) = nd10Crds(1);
    N_coord.val(10, 3) = nd10Crds(2);
    N_coord.val(11, 1) = nd11Crds(0);
    N_coord.val(11, 2) = nd11Crds(1);
    N_coord.val(11, 3) = nd11Crds(2);
    N_coord.val(12, 1) = nd12Crds(0);
    N_coord.val(12, 2) = nd12Crds(1);
    N_coord.val(12, 3) = nd12Crds(2);

    N_coord.val(13, 1) = nd13Crds(0);
    N_coord.val(13, 2) = nd13Crds(1);
    N_coord.val(13, 3) = nd13Crds(2);
    N_coord.val(14, 1) = nd14Crds(0);
    N_coord.val(14, 2) = nd14Crds(1);
    N_coord.val(14, 3) = nd14Crds(2);
    N_coord.val(15, 1) = nd15Crds(0);
    N_coord.val(15, 2) = nd15Crds(1);
    N_coord.val(15, 3) = nd15Crds(2);
    N_coord.val(16, 1) = nd16Crds(0);
    N_coord.val(16, 2) = nd16Crds(1);
    N_coord.val(16, 3) = nd16Crds(2);

    N_coord.val(17, 1) = nd17Crds(0);
    N_coord.val(17, 2) = nd17Crds(1);
    N_coord.val(17, 3) = nd17Crds(2);
    N_coord.val(18, 1) = nd18Crds(0);
    N_coord.val(18, 2) = nd18Crds(1);
    N_coord.val(18, 3) = nd18Crds(2);
    N_coord.val(19, 1) = nd19Crds(0);
    N_coord.val(19, 2) = nd19Crds(1);
    N_coord.val(19, 3) = nd19Crds(2);
    N_coord.val(20, 1) = nd20Crds(0);
    N_coord.val(20, 2) = nd20Crds(1);
    N_coord.val(20, 3) = nd20Crds(2);

    N_coord.val(21, 1) = nd21Crds(0);
    N_coord.val(21, 2) = nd21Crds(1);
    N_coord.val(21, 3) = nd21Crds(2);
    N_coord.val(22, 1) = nd22Crds(0);
    N_coord.val(22, 2) = nd22Crds(1);
    N_coord.val(22, 3) = nd22Crds(2);
    N_coord.val(23, 1) = nd23Crds(0);
    N_coord.val(23, 2) = nd23Crds(1);
    N_coord.val(23, 3) = nd23Crds(2);
    N_coord.val(24, 1) = nd24Crds(0);
    N_coord.val(24, 2) = nd24Crds(1);
    N_coord.val(24, 3) = nd24Crds(2);
    N_coord.val(25, 1) = nd25Crds(0);
    N_coord.val(25, 2) = nd25Crds(1);
    N_coord.val(25, 3) = nd25Crds(2);
    N_coord.val(26, 1) = nd26Crds(0);
    N_coord.val(26, 2) = nd26Crds(1);
    N_coord.val(26, 3) = nd26Crds(2);
    N_coord.val(27, 1) = nd27Crds(0);
    N_coord.val(27, 2) = nd27Crds(1);
    N_coord.val(27, 3) = nd27Crds(2);


    return N_coord;

}

////#############################################################################
tensor TwentySevenNodeBrick::incr_disp(void)
{
    const int dimensions[] = {27, 3};
    tensor increment_disp(2, dimensions, 0.0);

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



    increment_disp.val(1, 1) = IncrDis1(0);
    increment_disp.val(1, 2) = IncrDis1(1);
    increment_disp.val(1, 3) = IncrDis1(2);
    increment_disp.val(2, 1) = IncrDis2(0);
    increment_disp.val(2, 2) = IncrDis2(1);
    increment_disp.val(2, 3) = IncrDis2(2);
    increment_disp.val(3, 1) = IncrDis3(0);
    increment_disp.val(3, 2) = IncrDis3(1);
    increment_disp.val(3, 3) = IncrDis3(2);
    increment_disp.val(4, 1) = IncrDis4(0);
    increment_disp.val(4, 2) = IncrDis4(1);
    increment_disp.val(4, 3) = IncrDis4(2);
    increment_disp.val(5, 1) = IncrDis5(0);
    increment_disp.val(5, 2) = IncrDis5(1);
    increment_disp.val(5, 3) = IncrDis5(2);
    increment_disp.val(6, 1) = IncrDis6(0);
    increment_disp.val(6, 2) = IncrDis6(1);
    increment_disp.val(6, 3) = IncrDis6(2);
    increment_disp.val(7, 1) = IncrDis7(0);
    increment_disp.val(7, 2) = IncrDis7(1);
    increment_disp.val(7, 3) = IncrDis7(2);
    increment_disp.val(8, 1) = IncrDis8(0);
    increment_disp.val(8, 2) = IncrDis8(1);
    increment_disp.val(8, 3) = IncrDis8(2);

    increment_disp.val(9 , 1) = IncrDis9(0);
    increment_disp.val(9 , 2) = IncrDis9(1);
    increment_disp.val(9 , 3) = IncrDis9(2);
    increment_disp.val(10, 1) = IncrDis10(0);
    increment_disp.val(10, 2) = IncrDis10(1);
    increment_disp.val(10, 3) = IncrDis10(2);
    increment_disp.val(11, 1) = IncrDis11(0);
    increment_disp.val(11, 2) = IncrDis11(1);
    increment_disp.val(11, 3) = IncrDis11(2);
    increment_disp.val(12, 1) = IncrDis12(0);
    increment_disp.val(12, 2) = IncrDis12(1);
    increment_disp.val(12, 3) = IncrDis12(2);

    increment_disp.val(13, 1) = IncrDis13(0);
    increment_disp.val(13, 2) = IncrDis13(1);
    increment_disp.val(13, 3) = IncrDis13(2);
    increment_disp.val(14, 1) = IncrDis14(0);
    increment_disp.val(14, 2) = IncrDis14(1);
    increment_disp.val(14, 3) = IncrDis14(2);
    increment_disp.val(15, 1) = IncrDis15(0);
    increment_disp.val(15, 2) = IncrDis15(1);
    increment_disp.val(15, 3) = IncrDis15(2);
    increment_disp.val(16, 1) = IncrDis16(0);
    increment_disp.val(16, 2) = IncrDis16(1);
    increment_disp.val(16, 3) = IncrDis16(2);

    increment_disp.val(17, 1) = IncrDis17(0);
    increment_disp.val(17, 2) = IncrDis17(1);
    increment_disp.val(17, 3) = IncrDis17(2);
    increment_disp.val(18, 1) = IncrDis18(0);
    increment_disp.val(18, 2) = IncrDis18(1);
    increment_disp.val(18, 3) = IncrDis18(2);
    increment_disp.val(19, 1) = IncrDis19(0);
    increment_disp.val(19, 2) = IncrDis19(1);
    increment_disp.val(19, 3) = IncrDis19(2);
    increment_disp.val(20, 1) = IncrDis20(0);
    increment_disp.val(20, 2) = IncrDis20(1);
    increment_disp.val(20, 3) = IncrDis20(2);

    increment_disp.val(21, 1) = IncrDis21(0);
    increment_disp.val(21, 2) = IncrDis21(1);
    increment_disp.val(21, 3) = IncrDis21(2);
    increment_disp.val(22, 1) = IncrDis22(0);
    increment_disp.val(22, 2) = IncrDis22(1);
    increment_disp.val(22, 3) = IncrDis22(2);
    increment_disp.val(23, 1) = IncrDis23(0);
    increment_disp.val(23, 2) = IncrDis23(1);
    increment_disp.val(23, 3) = IncrDis23(2);
    increment_disp.val(24, 1) = IncrDis24(0);
    increment_disp.val(24, 2) = IncrDis24(1);
    increment_disp.val(24, 3) = IncrDis24(2);
    increment_disp.val(25, 1) = IncrDis25(0);
    increment_disp.val(25, 2) = IncrDis25(1);
    increment_disp.val(25, 3) = IncrDis25(2);
    increment_disp.val(26, 1) = IncrDis26(0);
    increment_disp.val(26, 2) = IncrDis26(1);
    increment_disp.val(26, 3) = IncrDis26(2);
    increment_disp.val(27, 1) = IncrDis27(0);
    increment_disp.val(27, 2) = IncrDis27(1);
    increment_disp.val(27, 3) = IncrDis27(2);



    return increment_disp;
}

////#############################################################################
tensor TwentySevenNodeBrick::total_disp(void)
{
    const int dimensions[] = {27, 3};
    tensor total_disp(2, dimensions, 0.0);

    //Zhaohui using node pointers, which come from the Domain
    const Vector &TotDis1 = theNodes[0]->getTrialDisp();
    cerr << "\ntot node " << theNodes[0]->getTag() << " x " << TotDis1(0) << " y " << TotDis1(1) << " z " << TotDis1(2) << endln;
    const Vector &TotDis2 = theNodes[1]->getTrialDisp();
    cerr << "tot node " << theNodes[1]->getTag() << " x " << TotDis2(0) << " y " << TotDis2(1) << " z " << TotDis2(2) << endln;
    const Vector &TotDis3 = theNodes[2]->getTrialDisp();
    cerr << "tot node " << theNodes[2]->getTag() << " x " << TotDis3(0) << " y " << TotDis3(1) << " z " << TotDis3(2) << endln;
    const Vector &TotDis4 = theNodes[3]->getTrialDisp();
    cerr << "tot node " << theNodes[3]->getTag() << " x " << TotDis4(0) << " y " << TotDis4(1) << " z " << TotDis4(2) << endln;
    const Vector &TotDis5 = theNodes[4]->getTrialDisp();
    cerr << "tot node " << theNodes[4]->getTag() << " x " << TotDis5(0) << " y " << TotDis5(1) << " z " << TotDis5(2) << endln;
    const Vector &TotDis6 = theNodes[5]->getTrialDisp();
    cerr << "tot node " << theNodes[5]->getTag() << " x " << TotDis6(0) << " y " << TotDis6(1) << " z " << TotDis6(2) << endln;
    const Vector &TotDis7 = theNodes[6]->getTrialDisp();
    cerr << "tot node " << theNodes[6]->getTag() << " x " << TotDis7(0) << " y " << TotDis7(1) << " z " << TotDis7(2) << endln;
    const Vector &TotDis8 = theNodes[7]->getTrialDisp();
    cerr << "tot node " << theNodes[7]->getTag() << " x " << TotDis8(0) << " y " << TotDis8(1) << " z " << TotDis8(2) << endln;

    const Vector &TotDis9 = theNodes[8]->getTrialDisp();
    cerr << "tot node " << theNodes[8]->getTag() << " x " << TotDis9(0) << " y " << TotDis9(1) << " z " << TotDis9(2) << endln;
    const Vector &TotDis10 = theNodes[9]->getTrialDisp();
    cerr << "tot node " << theNodes[9]->getTag() << " x " << TotDis10(0) << " y " << TotDis10(1) << " z " << TotDis10(2) << endln;
    const Vector &TotDis11 = theNodes[10]->getTrialDisp();
    cerr << "tot node " << theNodes[10]->getTag() << " x " << TotDis11(0) << " y " << TotDis11(1) << " z " << TotDis11(2) << endln;
    const Vector &TotDis12 = theNodes[11]->getTrialDisp();
    cerr << "tot node " << theNodes[11]->getTag() << " x " << TotDis12(0) << " y " << TotDis12(1) << " z " << TotDis12(2) << endln;

    const Vector &TotDis13 = theNodes[12]->getTrialDisp();
    cerr << "tot node " << theNodes[12]->getTag() << " x " << TotDis13(0) << " y " << TotDis13(1) << " z " << TotDis13(2) << endln;
    const Vector &TotDis14 = theNodes[13]->getTrialDisp();
    cerr << "tot node " << theNodes[13]->getTag() << " x " << TotDis14(0) << " y " << TotDis14(1) << " z " << TotDis14(2) << endln;
    const Vector &TotDis15 = theNodes[14]->getTrialDisp();
    cerr << "tot node " << theNodes[14]->getTag() << " x " << TotDis15(0) << " y " << TotDis15(1) << " z " << TotDis15(2) << endln;
    const Vector &TotDis16 = theNodes[15]->getTrialDisp();
    cerr << "tot node " << theNodes[15]->getTag() << " x " << TotDis16(0) << " y " << TotDis16(1) << " z " << TotDis16(2) << endln;

    const Vector &TotDis17 = theNodes[16]->getTrialDisp();
    cerr << "tot node " << theNodes[16]->getTag() << " x " << TotDis17(0) << " y " << TotDis17(1) << " z " << TotDis17(2) << endln;
    const Vector &TotDis18 = theNodes[17]->getTrialDisp();
    cerr << "tot node " << theNodes[17]->getTag() << " x " << TotDis18(0) << " y " << TotDis18(1) << " z " << TotDis18(2) << endln;
    const Vector &TotDis19 = theNodes[18]->getTrialDisp();
    cerr << "tot node " << theNodes[18]->getTag() << " x " << TotDis19(0) << " y " << TotDis19(1) << " z " << TotDis19(2) << endln;
    const Vector &TotDis20 = theNodes[19]->getTrialDisp();
    cerr << "tot node " << theNodes[19]->getTag() << " x " << TotDis20(0) << " y " << TotDis20(1) << " z " << TotDis20(2) << endln;
    const Vector &TotDis21 = theNodes[20]->getTrialDisp();
    cerr << "tot node " << theNodes[20]->getTag() << " x " << TotDis21(0) << " y " << TotDis21(1) << " z " << TotDis21(2) << endln;
    const Vector &TotDis22 = theNodes[21]->getTrialDisp();
    cerr << "tot node " << theNodes[21]->getTag() << " x " << TotDis22(0) << " y " << TotDis22(1) << " z " << TotDis22(2) << endln;
    const Vector &TotDis23 = theNodes[22]->getTrialDisp();
    cerr << "tot node " << theNodes[22]->getTag() << " x " << TotDis23(0) << " y " << TotDis23(1) << " z " << TotDis23(2) << endln;
    const Vector &TotDis24 = theNodes[23]->getTrialDisp();
    cerr << "tot node " << theNodes[23]->getTag() << " x " << TotDis24(0) << " y " << TotDis24(1) << " z " << TotDis24(2) << endln;
    const Vector &TotDis25 = theNodes[24]->getTrialDisp();
    cerr << "tot node " << theNodes[24]->getTag() << " x " << TotDis25(0) << " y " << TotDis25(1) << " z " << TotDis25(2) << endln;
    const Vector &TotDis26 = theNodes[25]->getTrialDisp();
    cerr << "tot node " << theNodes[25]->getTag() << " x " << TotDis26(0) << " y " << TotDis26(1) << " z " << TotDis26(2) << endln;
    const Vector &TotDis27 = theNodes[26]->getTrialDisp();
    cerr << "tot node " << theNodes[26]->getTag() << " x " << TotDis27(0) << " y " << TotDis27(1) << " z " << TotDis27(2) << endln;




    total_disp.val(1, 1) = TotDis1(0);
    total_disp.val(1, 2) = TotDis1(1);
    total_disp.val(1, 3) = TotDis1(2);
    total_disp.val(2, 1) = TotDis2(0);
    total_disp.val(2, 2) = TotDis2(1);
    total_disp.val(2, 3) = TotDis2(2);
    total_disp.val(3, 1) = TotDis3(0);
    total_disp.val(3, 2) = TotDis3(1);
    total_disp.val(3, 3) = TotDis3(2);
    total_disp.val(4, 1) = TotDis4(0);
    total_disp.val(4, 2) = TotDis4(1);
    total_disp.val(4, 3) = TotDis4(2);
    total_disp.val(5, 1) = TotDis5(0);
    total_disp.val(5, 2) = TotDis5(1);
    total_disp.val(5, 3) = TotDis5(2);
    total_disp.val(6, 1) = TotDis6(0);
    total_disp.val(6, 2) = TotDis6(1);
    total_disp.val(6, 3) = TotDis6(2);
    total_disp.val(7, 1) = TotDis7(0);
    total_disp.val(7, 2) = TotDis7(1);
    total_disp.val(7, 3) = TotDis7(2);
    total_disp.val(8, 1) = TotDis8(0);
    total_disp.val(8, 2) = TotDis8(1);
    total_disp.val(8, 3) = TotDis8(2);

    total_disp.val(9, 1) = TotDis9(0);
    total_disp.val(9, 2) = TotDis9(1);
    total_disp.val(9, 3) = TotDis9(2);
    total_disp.val(10, 1) = TotDis10(0);
    total_disp.val(10, 2) = TotDis10(1);
    total_disp.val(10, 3) = TotDis10(2);
    total_disp.val(11, 1) = TotDis11(0);
    total_disp.val(11, 2) = TotDis11(1);
    total_disp.val(11, 3) = TotDis11(2);
    total_disp.val(12, 1) = TotDis12(0);
    total_disp.val(12, 2) = TotDis12(1);
    total_disp.val(12, 3) = TotDis12(2);

    total_disp.val(13, 1) = TotDis13(0);
    total_disp.val(13, 2) = TotDis13(1);
    total_disp.val(13, 3) = TotDis13(2);
    total_disp.val(14, 1) = TotDis14(0);
    total_disp.val(14, 2) = TotDis14(1);
    total_disp.val(14, 3) = TotDis14(2);
    total_disp.val(15, 1) = TotDis15(0);
    total_disp.val(15, 2) = TotDis15(1);
    total_disp.val(15, 3) = TotDis15(2);
    total_disp.val(16, 1) = TotDis16(0);
    total_disp.val(16, 2) = TotDis16(1);
    total_disp.val(16, 3) = TotDis16(2);

    total_disp.val(17, 1) = TotDis17(0);
    total_disp.val(17, 2) = TotDis17(1);
    total_disp.val(17, 3) = TotDis17(2);
    total_disp.val(18, 1) = TotDis18(0);
    total_disp.val(18, 2) = TotDis18(1);
    total_disp.val(18, 3) = TotDis18(2);
    total_disp.val(19, 1) = TotDis19(0);
    total_disp.val(19, 2) = TotDis19(1);
    total_disp.val(19, 3) = TotDis19(2);
    total_disp.val(20, 1) = TotDis20(0);
    total_disp.val(20, 2) = TotDis20(1);
    total_disp.val(20, 3) = TotDis20(2);

    total_disp.val(21, 1) = TotDis21(0);
    total_disp.val(21, 2) = TotDis21(1);
    total_disp.val(21, 3) = TotDis21(2);
    total_disp.val(22, 1) = TotDis22(0);
    total_disp.val(22, 2) = TotDis22(1);
    total_disp.val(22, 3) = TotDis22(2);
    total_disp.val(23, 1) = TotDis23(0);
    total_disp.val(23, 2) = TotDis23(1);
    total_disp.val(23, 3) = TotDis23(2);
    total_disp.val(24, 1) = TotDis24(0);
    total_disp.val(24, 2) = TotDis24(1);
    total_disp.val(24, 3) = TotDis24(2);
    total_disp.val(25, 1) = TotDis25(0);
    total_disp.val(25, 2) = TotDis25(1);
    total_disp.val(25, 3) = TotDis25(2);
    total_disp.val(26, 1) = TotDis26(0);
    total_disp.val(26, 2) = TotDis26(1);
    total_disp.val(26, 3) = TotDis26(2);
    total_disp.val(27, 1) = TotDis27(0);
    total_disp.val(27, 2) = TotDis27(1);
    total_disp.val(27, 3) = TotDis27(2);


    return total_disp;
}


////#############################################################################
tensor TwentySevenNodeBrick::total_disp(FILE *fp, double *u)  //Not implemented yet??? Guanzhou Oct. 2003
{
    const int dimensions[] = {27, 3};
    tensor total_disp(2, dimensions, 0.0);

    const Vector &TotDis1 = theNodes[0]->getTrialDisp();
    const Vector &TotDis2 = theNodes[1]->getTrialDisp();
    const Vector &TotDis3 = theNodes[2]->getTrialDisp();
    const Vector &TotDis4 = theNodes[3]->getTrialDisp();
    const Vector &TotDis5 = theNodes[4]->getTrialDisp();
    const Vector &TotDis6 = theNodes[5]->getTrialDisp();
    const Vector &TotDis7 = theNodes[6]->getTrialDisp();
    const Vector &TotDis8 = theNodes[7]->getTrialDisp();

    total_disp.val(1, 1) = TotDis1(0);
    total_disp.val(1, 2) = TotDis1(1);
    total_disp.val(1, 3) = TotDis1(2);
    total_disp.val(2, 1) = TotDis2(0);
    total_disp.val(2, 2) = TotDis2(1);
    total_disp.val(2, 3) = TotDis2(2);
    total_disp.val(3, 1) = TotDis3(0);
    total_disp.val(3, 2) = TotDis3(1);
    total_disp.val(3, 3) = TotDis3(2);
    total_disp.val(4, 1) = TotDis4(0);
    total_disp.val(4, 2) = TotDis4(1);
    total_disp.val(4, 3) = TotDis4(2);
    total_disp.val(5, 1) = TotDis5(0);
    total_disp.val(5, 2) = TotDis5(1);
    total_disp.val(5, 3) = TotDis5(2);
    total_disp.val(6, 1) = TotDis6(0);
    total_disp.val(6, 2) = TotDis6(1);
    total_disp.val(6, 3) = TotDis6(2);
    total_disp.val(7, 1) = TotDis7(0);
    total_disp.val(7, 2) = TotDis7(1);
    total_disp.val(7, 3) = TotDis7(2);
    total_disp.val(8, 1) = TotDis8(0);
    total_disp.val(8, 2) = TotDis8(1);
    total_disp.val(8, 3) = TotDis8(2);

    return total_disp;
}


////#############################################################################
int TwentySevenNodeBrick::get_global_number_of_node(int local_node_number)
{
    return connectedExternalNodes(local_node_number);
}

////#############################################################################
int  TwentySevenNodeBrick::get_Brick_Number(void)
{
    //return elem_numb;
    return this->getTag();
}

////#############################################################################
// returns nodal forces for given stress field in an element
tensor TwentySevenNodeBrick::nodal_forces(void)
{
    int force_dim[] = {27, 3}; // Guanzhou changed for {20,3} to {27,3} for 27 nodes

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

    straintensor incremental_strain;

    static int disp_dim[] = {27, 3};
    tensor incremental_displacements(2, disp_dim, 0.0); // \Delta u

    incremental_displacements = incr_disp();

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );
        rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );
            sw = get_Gauss_p_w( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                tw = get_Gauss_p_w( 3, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

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

                //weight
                weight = rw * sw * tw * det_of_Jacobian;

                stress_at_GP = matpoint[where]->getStressTensor();


                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;
            }
        }
    }

    return nodal_forces;

}

////#############################################################################
// returns nodal forces for given ITERATIVE stress field in an element
tensor TwentySevenNodeBrick::iterative_nodal_forces(void)
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

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );
        rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );
            sw = get_Gauss_p_w( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                tw = get_Gauss_p_w( 3, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

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

                //weight
                weight = rw * sw * tw * det_of_Jacobian;

                stress_at_GP = matpoint[where]->getStressTensor();
                stress_at_GP.reportshortpqtheta("\n iterative_stress at GAUSS point in iterative_nodal_force\n");

                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;
                //nodal_forces.print("nf","\n TwentySevenNodeBrick::iterative_nodal_forces Nodal Forces ~~~~\n");

            }
        }
    }


    return nodal_forces;

}

////#############################################################################
// returns nodal forces for given constant stress field in the element
tensor TwentySevenNodeBrick::nodal_forces_from_stress(stresstensor &stress)
{
    int force_dim[] = {27, 3};

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

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );
        rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );
            sw = get_Gauss_p_w( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                tw = get_Gauss_p_w( 3, GP_c_t );

                // this short routine is supposed to calculate position of

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();


                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                //dhGlobal = dh("ij") * JacobianINV("jk");// Zhaohui 09-02-2001
                dhGlobal = dh("ij") * JacobianINV("kj");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;



                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress("ab") * weight;

            }
        }
    }

    return nodal_forces;

}


////#############################################################################
// returns nodal forces for given incremental strain field in an element
// by using the linearized constitutive tensor from the begining of the step !
tensor TwentySevenNodeBrick::linearized_nodal_forces(void)
{
    int force_dim[] = {27, 3};

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

    static int disp_dim[] = {27, 3};

    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    stresstensor final_linearized_stress;
    //    stresstensor incremental_stress;
    // tensor of incremental displacements taken from node objects for this element !
    incremental_displacements = incr_disp();
    //incremental_displacements.print("disp","\n incremental_displacements tensor at GAUSS point in iterative_Update\n");

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );
        rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );
            sw = get_Gauss_p_w( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                tw = get_Gauss_p_w( 3, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("kj");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;

                // incremental straines at this Gauss point
                // now in Update we know the incremental displacements so let's find
                // the incremental strain
                incremental_strain =
                    (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();
                //incremental_strain.reportshort("\n incremental_strain tensor at GAUSS point in iterative_Update\n");

                if ( ! (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)  )
                {
                    cerr << "TwentySevenNodeBrick::linearized_nodal_forces (tag: " << this->getTag() << "), not converged\n";
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



//#############################################################################
//Compute Gauss Point coordinates and store it in global GaussCoord
void TwentySevenNodeBrick::computeGaussPoint(void)
{

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    // special case for 8 nodes only
    static const int dim[] = {3, 27}; // static-> see ARM pp289-290
    static const int dim27[] = {3, 27}; // static-> see ARM pp289-290 Joey found a bug: dimension count is integr. points dependent
    tensor NodalCoord(2, dim, 0.0);
    tensor matpointCoord(2, dim27, 0.0);
    int h_dim[] = {81, 3};
    tensor H(2, h_dim, 0.0);


    //Zhaohui using node pointers, which come from the Domain
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



    NodalCoord.val(1, 1) = nd1Crds( 0);
    NodalCoord.val(2, 1) = nd1Crds( 1);
    NodalCoord.val(3, 1) = nd1Crds( 2);
    NodalCoord.val(1, 2) = nd2Crds( 0);
    NodalCoord.val(2, 2) = nd2Crds( 1);
    NodalCoord.val(3, 2) = nd2Crds( 2);
    NodalCoord.val(1, 3) = nd3Crds( 0);
    NodalCoord.val(2, 3) = nd3Crds( 1);
    NodalCoord.val(3, 3) = nd3Crds( 2);
    NodalCoord.val(1, 4) = nd4Crds( 0);
    NodalCoord.val(2, 4) = nd4Crds( 1);
    NodalCoord.val(3, 4) = nd4Crds( 2);
    NodalCoord.val(1, 5) = nd5Crds( 0);
    NodalCoord.val(2, 5) = nd5Crds( 1);
    NodalCoord.val(3, 5) = nd5Crds( 2);
    NodalCoord.val(1, 6) = nd6Crds( 0);
    NodalCoord.val(2, 6) = nd6Crds( 1);
    NodalCoord.val(3, 6) = nd6Crds( 2);
    NodalCoord.val(1, 7) = nd7Crds( 0);
    NodalCoord.val(2, 7) = nd7Crds( 1);
    NodalCoord.val(3, 7) = nd7Crds( 2);
    NodalCoord.val(1, 8) = nd8Crds( 0);
    NodalCoord.val(2, 8) = nd8Crds( 1);
    NodalCoord.val(3, 8) = nd8Crds( 2);
    NodalCoord.val(1, 9) = nd9Crds( 0);
    NodalCoord.val(2, 9) = nd9Crds( 1);
    NodalCoord.val(3, 9) = nd9Crds( 2);
    NodalCoord.val(1, 10) = nd10Crds(0);
    NodalCoord.val(2, 10) = nd10Crds(1);
    NodalCoord.val(3, 10) = nd10Crds(2);
    NodalCoord.val(1, 11) = nd11Crds(0);
    NodalCoord.val(2, 11) = nd11Crds(1);
    NodalCoord.val(3, 11) = nd11Crds(2);
    NodalCoord.val(1, 12) = nd12Crds(0);
    NodalCoord.val(2, 12) = nd12Crds(1);
    NodalCoord.val(3, 12) = nd12Crds(2);
    NodalCoord.val(1, 13) = nd13Crds(0);
    NodalCoord.val(2, 13) = nd13Crds(1);
    NodalCoord.val(3, 13) = nd13Crds(2);
    NodalCoord.val(1, 14) = nd14Crds(0);
    NodalCoord.val(2, 14) = nd14Crds(1);
    NodalCoord.val(3, 14) = nd14Crds(2);
    NodalCoord.val(1, 15) = nd15Crds(0);
    NodalCoord.val(2, 15) = nd15Crds(1);
    NodalCoord.val(3, 15) = nd15Crds(2);
    NodalCoord.val(1, 16) = nd16Crds(0);
    NodalCoord.val(2, 16) = nd16Crds(1);
    NodalCoord.val(3, 16) = nd16Crds(2);
    NodalCoord.val(1, 17) = nd17Crds(0);
    NodalCoord.val(2, 17) = nd17Crds(1);
    NodalCoord.val(3, 17) = nd17Crds(2);
    NodalCoord.val(1, 18) = nd18Crds(0);
    NodalCoord.val(2, 18) = nd18Crds(1);
    NodalCoord.val(3, 18) = nd18Crds(2);
    NodalCoord.val(1, 19) = nd19Crds(0);
    NodalCoord.val(2, 19) = nd19Crds(1);
    NodalCoord.val(3, 19) = nd19Crds(2);
    NodalCoord.val(1, 20) = nd20Crds(0);
    NodalCoord.val(2, 20) = nd20Crds(1);
    NodalCoord.val(3, 20) = nd20Crds(2);
    NodalCoord.val(1, 21) = nd21Crds(0);
    NodalCoord.val(2, 21) = nd21Crds(1);
    NodalCoord.val(3, 21) = nd21Crds(2);
    NodalCoord.val(1, 22) = nd22Crds(0);
    NodalCoord.val(2, 22) = nd22Crds(1);
    NodalCoord.val(3, 22) = nd22Crds(2);
    NodalCoord.val(1, 23) = nd23Crds(0);
    NodalCoord.val(2, 23) = nd23Crds(1);
    NodalCoord.val(3, 23) = nd23Crds(2);
    NodalCoord.val(1, 24) = nd24Crds(0);
    NodalCoord.val(2, 24) = nd24Crds(1);
    NodalCoord.val(3, 24) = nd24Crds(2);
    NodalCoord.val(1, 25) = nd25Crds(0);
    NodalCoord.val(2, 25) = nd25Crds(1);
    NodalCoord.val(3, 25) = nd25Crds(2);
    NodalCoord.val(1, 26) = nd26Crds(0);
    NodalCoord.val(2, 26) = nd26Crds(1);
    NodalCoord.val(3, 26) = nd26Crds(2);
    NodalCoord.val(1, 27) = nd27Crds(0);
    NodalCoord.val(2, 27) = nd27Crds(1);
    NodalCoord.val(3, 27) = nd27Crds(2);

    //NodalCoord.print("NC");

    int gp = 0;
    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates

                H = H_3D(r, s, t);

                for (int encount = 1 ; encount <= 27; encount++ )
                {

                    matpointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
                    //::printf("-- NO nodal, H_val :%d %+.2e %+.2e %+.5e\n", encount,NodalCoord.val(1,encount),H.val(encount*3-2,1),matpointCoord.val(1,where+1) );
                    matpointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
                    matpointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);
                }

                gauss_points(gp, 0) = matpointCoord.val(1, where + 1);
                gauss_points(gp, 1) = matpointCoord.val(2, where + 1);
                gauss_points(gp, 2) = matpointCoord.val(3, where + 1);

                gp++;
            }
        }
    }

    //return GaussCoord;

}


////#############################################################################



//=============================================================================
int TwentySevenNodeBrick::getNumExternalNodes () const
{
    return 27;
}


//=============================================================================
const ID &TwentySevenNodeBrick::getExternalNodes ()
{
    return connectedExternalNodes;
}

Node **
TwentySevenNodeBrick::getNodePtrs(void)
{
    return theNodes;
}

//=============================================================================
int TwentySevenNodeBrick::getNumDOF ()
{
    return 3 * 27;
}

//=============================================================================
void TwentySevenNodeBrick::setDomain (Domain *theDomain)
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
        theNodes[20] = 0;
        theNodes[21] = 0;
        theNodes[22] = 0;
        theNodes[23] = 0;
        theNodes[24] = 0;
        theNodes[25] = 0;
        theNodes[26] = 0;

    }

    //Added if-else for found a bug when trying removeElement from theDomain  07-19-2001 Zhaohui
    else
    {
        int Nd1 = connectedExternalNodes(0);
        int Nd2 = connectedExternalNodes(1);
        int Nd3 = connectedExternalNodes(2);
        int Nd4 = connectedExternalNodes(3);
        //Xiaoyan added 5-8  07/06/00

        int Nd5 = connectedExternalNodes(4);
        int Nd6 = connectedExternalNodes(5);
        int Nd7 = connectedExternalNodes(6);
        int Nd8 = connectedExternalNodes(7);
        int Nd9  = connectedExternalNodes(8);
        int Nd10 = connectedExternalNodes(9);
        int Nd11 = connectedExternalNodes(10);
        int Nd12 = connectedExternalNodes(11);
        int Nd13 = connectedExternalNodes(12);
        int Nd14 = connectedExternalNodes(13);
        int Nd15 = connectedExternalNodes(14);
        int Nd16 = connectedExternalNodes(15);
        int Nd17 = connectedExternalNodes(16);
        int Nd18 = connectedExternalNodes(17);
        int Nd19 = connectedExternalNodes(18);
        int Nd20 = connectedExternalNodes(19);
        int Nd21 = connectedExternalNodes(20);
        int Nd22 = connectedExternalNodes(21);
        int Nd23 = connectedExternalNodes(22);
        int Nd24 = connectedExternalNodes(23);
        int Nd25 = connectedExternalNodes(24);
        int Nd26 = connectedExternalNodes(25);
        int Nd27 = connectedExternalNodes(26);


        theNodes[0] = theDomain->getNode(Nd1);
        theNodes[1] = theDomain->getNode(Nd2);
        theNodes[2] = theDomain->getNode(Nd3);
        theNodes[3] = theDomain->getNode(Nd4);
        theNodes[4] = theDomain->getNode(Nd5);
        theNodes[5] = theDomain->getNode(Nd6);
        theNodes[6] = theDomain->getNode(Nd7);
        theNodes[7] = theDomain->getNode(Nd8);
        theNodes[8] = theDomain->getNode(Nd9);
        theNodes[9] = theDomain->getNode(Nd10);
        theNodes[10] = theDomain->getNode(Nd11);
        theNodes[11] = theDomain->getNode(Nd12);
        theNodes[12] = theDomain->getNode(Nd13);
        theNodes[13] = theDomain->getNode(Nd14);
        theNodes[14] = theDomain->getNode(Nd15);
        theNodes[15] = theDomain->getNode(Nd16);
        theNodes[16] = theDomain->getNode(Nd17);
        theNodes[17] = theDomain->getNode(Nd18);
        theNodes[18] = theDomain->getNode(Nd19);
        theNodes[19] = theDomain->getNode(Nd20);
        theNodes[20] = theDomain->getNode(Nd21);
        theNodes[21] = theDomain->getNode(Nd22);
        theNodes[22] = theDomain->getNode(Nd23);
        theNodes[23] = theDomain->getNode(Nd24);
        theNodes[24] = theDomain->getNode(Nd25);
        theNodes[25] = theDomain->getNode(Nd26);
        theNodes[26] = theDomain->getNode(Nd27);


        if (theNodes[0]  == 0 || theNodes[1]  == 0 || theNodes[2]  == 0 || theNodes[3]  == 0 ||
                theNodes[4]  == 0 || theNodes[5]  == 0 || theNodes[6]  == 0 || theNodes[7]  == 0 ||
                theNodes[8]  == 0 || theNodes[9] == 0 || theNodes[10] == 0 || theNodes[11] == 0 ||
                theNodes[12] == 0 || theNodes[13] == 0 || theNodes[14] == 0 || theNodes[15] == 0 ||
                theNodes[16] == 0 || theNodes[17] == 0 || theNodes[18] == 0 || theNodes[19] == 0 ||
                theNodes[20] == 0 || theNodes[21] == 0 || theNodes[22] == 0 || theNodes[23] == 0 ||
                theNodes[24] == 0 || theNodes[25] == 0 || theNodes[26] == 0)
        {

            cerr << "FATAL ERROR TwentySevenNodeBrick (tag: " << this->getTag() << " ), node not found in domain\n";
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


        if (dofNd1  != 3 || dofNd2  != 3 || dofNd3  != 3 || dofNd4  != 3 ||
                dofNd5  != 3 || dofNd6  != 3 || dofNd7  != 3 || dofNd8  != 3 ||
                dofNd9  != 3 || dofNd10 != 3 || dofNd11 != 3 || dofNd12 != 3 ||
                dofNd13 != 3 || dofNd14 != 3 || dofNd15 != 3 || dofNd16 != 3 ||
                dofNd17 != 3 || dofNd18 != 3 || dofNd19 != 3 || dofNd20 != 3 ||
                dofNd21 != 3 || dofNd22 != 3 || dofNd23 != 3 || dofNd24 != 3 ||
                dofNd25 != 3 || dofNd26 != 3 || dofNd27 != 3)
        {
            cerr << "FATAL ERROR TwentySevenNodeBrick (tag: " << this->getTag() <<
                 "), has differing number of DOFs at its nodes\n";
            exit(-1);
        }

        this->DomainComponent::setDomain(theDomain);
    }

    computeGaussPoint();
}

//=============================================================================
int TwentySevenNodeBrick::commitState ()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "TwentySevenNodeBrick::commitState () - failed in base class";
    }
    int i;

    Vector pp = getResistingForce();


    for (i = 0; i < 27; i++)
    {
        retVal += matpoint[i]->commitState();
        stresstensor st;
        stresstensor prin;
        straintensor stn;
        straintensor stnprin;

        st = matpoint[i]->getStressTensor();
        prin = st.principal();
        stn = matpoint[i]->getStrainTensor();
        stnprin = stn.principal();

    }


    //Forming output
    stresstensor stress;
    straintensor strain;
    straintensor plstrain;
    int ii = 0;
    for (int gp = 0; gp < 27; gp++)
    {
        stress = matpoint[gp]->getStressTensor();
        strain = matpoint[gp]->getStrainTensor();
        plstrain = matpoint[gp]->getPlasticStrainTensor();

        //Write strain
        outputVector(ii++) = strain.cval(1, 1);
        outputVector(ii++) = strain.cval(2, 2);
        outputVector(ii++) = strain.cval(3, 3);
        outputVector(ii++) = strain.cval(1, 2);
        outputVector(ii++) = strain.cval(1, 3);
        outputVector(ii++) = strain.cval(2, 3);

        //Write strain
        outputVector(ii++) = plstrain.cval(1, 1);
        outputVector(ii++) = plstrain.cval(2, 2);
        outputVector(ii++) = plstrain.cval(3, 3);
        outputVector(ii++) = plstrain.cval(1, 2);
        outputVector(ii++) = plstrain.cval(1, 3);
        outputVector(ii++) = plstrain.cval(2, 3);


        //Write stress
        outputVector(ii++) = stress.cval(1, 1);
        outputVector(ii++) = stress.cval(2, 2);
        outputVector(ii++) = stress.cval(3, 3);
        outputVector(ii++) = stress.cval(1, 2);
        outputVector(ii++) = stress.cval(1, 3);
        outputVector(ii++) = stress.cval(2, 3);
    }

    return retVal;
}

//=============================================================================
int TwentySevenNodeBrick::revertToLastCommit ()
{
    int i;
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states
    for (i = 0; i < 27; i++)
    {
        retVal += matpoint[i]->revertToLastCommit();
    }


    return retVal;
}

//=============================================================================
int TwentySevenNodeBrick::revertToStart ()
{
    int i;     // Xiaoyan added k for three dimension
    int retVal = 0;

    for (i = 0; i < 27; i++)
    {
        retVal += matpoint[i]->revertToStart();
    }


    return retVal;

    // Loop over the integration points and revert to initial material states
}


//=============================================================================
const Matrix &TwentySevenNodeBrick::getTangentStiff ()
{
    tensor stifftensor = getStiffnessTensor();
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
                    K( Ki - 1 , Kj - 1 ) = stifftensor.cval(i, k, l, j);
                }
            }
        }
    }

    return K;
}

//=============================================================================
const Matrix &TwentySevenNodeBrick::getInitialStiff ()
{
    return this->getTangentStiff();
}

//=============================================================================
//Get lumped mass
const Matrix &TwentySevenNodeBrick::getConsMass ()
{
    tensor masstensor = getMassTensor();

    double column_mass;

    for ( int i = 1 ; i <= 27 * 3 ; i++ )
    {
        column_mass = 0.0;

        for ( int j = 1 ; j <= 27 * 3 ; j++ )
        {
            column_mass += masstensor.cval(i, j);
            M( i - 1 , j - 1 ) = 0;
        }

        M( i - 1 , i - 1 ) = column_mass;

    }
    return M;
}

//=============================================================================
//Get consistent mass
const Matrix &TwentySevenNodeBrick::getMass ()
{
    tensor masstensor = getMassTensor();

    for ( int i = 1 ; i <= 27 * 3 ; i++ )
    {
        //column_mass = 0.0;
        for ( int j = 1 ; j <= 27 * 3 ; j++ )
        {
            M( i - 1 , j - 1 ) = masstensor.cval(i, j);

        }

    }

    return M;
}

//=============================================================================
void TwentySevenNodeBrick::zeroLoad(void)
{
    Q.Zero();
}


//======================================================================
const Vector &TwentySevenNodeBrick::getBodyForce(double loadFactor, const Vector &data)
{


    static Vector bforce(81);
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


    Vector ba(81), bfx(3);
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
    bforce.addMatrixVector(0.0, M, ba, 1.0);


    return bforce;
}


//======================================================================
const Vector &TwentySevenNodeBrick::getSurfaceForce(double loadFactor, const Vector &data)
{


    int node_exist = 0;
    Vector node_local(9);

    // check if the nodes of the surface belong to the element
    for (int i = 0; i < 9; i++)
    {

        for (int j = 0; j < 27; j++)
        {
            if (data(i) == connectedExternalNodes(j))
            {
                node_exist = 1;
                node_local(i) = j;
                break;
            }
        }

        if (node_exist != 1)
        {
            cerr << "\nERROR: Node " << data(i) << " defined for the BrickSurfaceLoad does not belong to element " << this->getTag() << endl;
            exit(1);
        }
    }


    int node1_local = node_local(0);
    int node2_local = node_local(1);
    int node3_local = node_local(2);
    int node4_local = node_local(3);
    int node5_local = node_local(4);
    int node6_local = node_local(5);
    int node7_local = node_local(6);
    int node8_local = node_local(7);
    int node9_local = node_local(8);


    // get the surface nodal coordinates
    const Vector &coordnode1 = theNodes[node1_local]->getCrds();
    const Vector &coordnode2 = theNodes[node2_local]->getCrds();
    const Vector &coordnode3 = theNodes[node3_local]->getCrds();
    const Vector &coordnode4 = theNodes[node4_local]->getCrds();
    const Vector &coordnode5 = theNodes[node5_local]->getCrds();
    const Vector &coordnode6 = theNodes[node6_local]->getCrds();
    const Vector &coordnode7 = theNodes[node7_local]->getCrds();
    const Vector &coordnode8 = theNodes[node8_local]->getCrds();
    const Vector &coordnode9 = theNodes[node9_local]->getCrds();



    Vector ShapeFunctionValues(9);
    double LoadValue;
    Vector J_vector(3);
    Vector Pressure(9);


    Pressure(0) = data(9)  * loadFactor;
    Pressure(1) = data(10) * loadFactor;
    Pressure(2) = data(11) * loadFactor;
    Pressure(3) = data(12) * loadFactor;
    Pressure(4) = data(13) * loadFactor;
    Pressure(5) = data(14) * loadFactor;
    Pressure(6) = data(15) * loadFactor;
    Pressure(7) = data(16) * loadFactor;
    Pressure(8) = data(17) * loadFactor;




    static Vector NodalForces(81);

    for (int m = 0; m < 81; m++)
    {
        NodalForces(m) = 0;
    }



    double Root3Over5 = sqrt(3.0 / 5.0);
    double W5Over9 = 5.0 / 9.0;
    double W8Over9 = 8.0 / 9.0;
    Matrix GsPts(9, 2);
    Matrix Weight(9, 2);


    GsPts(0, 0) =  Root3Over5;
    GsPts(0, 1) =  Root3Over5;
    GsPts(1, 0) = -Root3Over5;
    GsPts(1, 1) =  Root3Over5;
    GsPts(2, 0) = -Root3Over5;
    GsPts(2, 1) = -Root3Over5;
    GsPts(3, 0) =  Root3Over5;
    GsPts(3, 1) = -Root3Over5;
    GsPts(4, 0) =  0.0;
    GsPts(4, 1) =  Root3Over5;
    GsPts(5, 0) = -Root3Over5;
    GsPts(5, 1) =  0.0;
    GsPts(6, 0) =  0.0;
    GsPts(6, 1) = -Root3Over5;
    GsPts(7, 0) =  Root3Over5;
    GsPts(7, 1) =  0.0;
    GsPts(8, 0) =  0.0;
    GsPts(8, 1) =  0.0;


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
        for (int j = 0; j < 9; j++)
        {

            for (int v = 0; v < 27; v++)
            {
                if (data(j) == connectedExternalNodes(v))
                {
                    r = v;
                    break;
                }
            }

            // loop over Gauss points
            for (int i = 0; i < 9; i++)
            {

                ShapeFunctionValues = SurfaceShapeFunctionValues(GsPts(i, 0) , GsPts(i, 1));

                J_vector = Direction_Weight(GsPts(i, 0) , GsPts(i, 1),
                                            coordnode1, coordnode2, coordnode3, coordnode4,
                                            coordnode5, coordnode6, coordnode7, coordnode8, coordnode9);
                LoadValue = SurfaceLoadValues(GsPts(i, 0) , GsPts(i, 1), Pressure);

                NodalForces(r * 3 + k) = NodalForces(r * 3 + k) + LoadValue * J_vector(k) * ShapeFunctionValues(j) * Weight(i, 0) * Weight(i, 1);
            }
        }
    }
    return NodalForces;
}




//=============================================================================
int
TwentySevenNodeBrick::addLoad(ElementalLoad *theLoad, double loadFactor)
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
        cerr << "TwentySevenNodeBrick::addLoad() - 27NodeBrick " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }

    return 0;
}


//=============================================================================
int TwentySevenNodeBrick::addInertiaLoadToUnbalance(const Vector &accel)
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
    const Vector &Raccel21 = theNodes[20]->getRV(accel);
    const Vector &Raccel22 = theNodes[21]->getRV(accel);
    const Vector &Raccel23 = theNodes[22]->getRV(accel);
    const Vector &Raccel24 = theNodes[23]->getRV(accel);
    const Vector &Raccel25 = theNodes[24]->getRV(accel);
    const Vector &Raccel26 = theNodes[25]->getRV(accel);
    const Vector &Raccel27 = theNodes[26]->getRV(accel);


    if (3 != Raccel1.Size()  || 3 != Raccel2.Size()  || 3 != Raccel3.Size()  || 3 != Raccel4.Size() ||
            3 != Raccel5.Size()  || 3 != Raccel6.Size()  || 3 != Raccel7.Size()  || 3 != Raccel8.Size() ||
            3 != Raccel9.Size()  || 3 != Raccel10.Size() || 3 != Raccel11.Size() || 3 != Raccel12.Size() ||
            3 != Raccel13.Size() || 3 != Raccel14.Size() || 3 != Raccel15.Size() || 3 != Raccel16.Size() ||
            3 != Raccel17.Size() || 3 != Raccel18.Size() || 3 != Raccel19.Size() || 3 != Raccel20.Size() ||
            3 != Raccel21.Size() || 3 != Raccel22.Size() || 3 != Raccel23.Size() || 3 != Raccel24.Size() ||
            3 != Raccel25.Size() || 3 != Raccel26.Size() || 3 != Raccel27.Size())
    {
        // Xiaoyan changed 2 to 3 and added Racce15-18  09/27/00
        cerr << "TwentySevenNodeBrick::addInertiaLoadToUnbalance " <<
             "matrix and vector sizes are incompatable\n";
        return -1;
    }

    static Vector ra(81);  // Changed form 60 to 81  Guanzhou Oct. 2003

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


    Q.addMatrixVector(1.0, M, ra, -1.0);

    return 0;
}

//=============================================================================
const Vector TwentySevenNodeBrick::FormEquiBodyForce(const Vector &data)
{
    Vector bforce(81);

    // Check for a quick return
    //cerr << "rho " << rho << endln;
    if (rho == 0.0)
    {
        return bforce;
    }


    Vector bf(3);
    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);

    Vector ba(81);

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
    bforce.addMatrixVector(0.0, M, ba, 1.0);

    //cerr << " @@@@@ FormEquiAccelerationField  " << bforce;

    return bforce;
}



//=============================================================================
const Vector &TwentySevenNodeBrick::getResistingForce ()
{
    int force_dim[] = {27, 3};
    tensor nodalforces(2, force_dim, 0.0);

    nodalforces = nodal_forces();

    //converting nodalforce tensor to vector
    for (int i = 0; i < 27; i++)
        for (int j = 0; j < 3; j++)
        {
            P(i * 3 + j) = nodalforces.cval(i + 1, j + 1);
        }

    //cerr << "P" << P;
    //cerr << "Q" << Q;

    P = P - Q;

    //     cerr << "P-Q" << P;
    return P;
}

//=============================================================================
const Vector &TwentySevenNodeBrick::getResistingForceIncInertia ()
{

    this->getResistingForce();

    //     cerr << P;


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
        const Vector &accel21 = theNodes[20]->getTrialAccel();
        const Vector &accel22 = theNodes[21]->getTrialAccel();
        const Vector &accel23 = theNodes[22]->getTrialAccel();
        const Vector &accel24 = theNodes[23]->getTrialAccel();
        const Vector &accel25 = theNodes[24]->getTrialAccel();
        const Vector &accel26 = theNodes[25]->getTrialAccel();
        const Vector &accel27 = theNodes[26]->getTrialAccel();

        static Vector a(81);  // originally 8

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


        //        cerr << a;

        // P += M * a
        P.addMatrixVector(1.0, M, a, 1.0);

        // add the damping forces if rayleigh damping
        if (a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            P += this->getRayleighDampingForces();
        }
    }
    else
    {
        // add the damping forces if rayleigh damping
        if (a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            P += this->getRayleighDampingForces();
        }
    }

    return P;
}

//=============================================================================
//implemented by Babak Kamrani on 10/18/2012
int TwentySevenNodeBrick::sendSelf (int commitTag, Channel &theChannel)
{
    int dataTag = this->getDbTag();
    int matDbTag;
    static ID idData(30);

    idData(29) = this->getTag();

    idData(0) = (matpoint[0]->matmodel)->getClassTag();

    for (int i = 0; i < 27; i++)
    {
        matDbTag = (matpoint[i]->matmodel)->getDbTag();
        if ( matDbTag == 0 )
        {
            matDbTag = theChannel.getDbTag();
            if ( matDbTag != 0 )
            {
                (matpoint[i]->matmodel)->setDbTag(matDbTag);
            }
        }
    }

    idData(1) = matDbTag;

    idData(2)  = connectedExternalNodes(0);
    idData(3)  = connectedExternalNodes(1);
    idData(4)  = connectedExternalNodes(2);
    idData(5)  = connectedExternalNodes(3);
    idData(6)  = connectedExternalNodes(4);
    idData(7)  = connectedExternalNodes(5);
    idData(8)  = connectedExternalNodes(6);
    idData(9)  = connectedExternalNodes(7);
    idData(10) = connectedExternalNodes(8);
    idData(11) = connectedExternalNodes(9);
    idData(12) = connectedExternalNodes(10);
    idData(13) = connectedExternalNodes(11);
    idData(14) = connectedExternalNodes(12);
    idData(15) = connectedExternalNodes(13);
    idData(16) = connectedExternalNodes(14);
    idData(17) = connectedExternalNodes(15);
    idData(18) = connectedExternalNodes(16);
    idData(19) = connectedExternalNodes(17);
    idData(20) = connectedExternalNodes(18);
    idData(21) = connectedExternalNodes(19);
    idData(22) = connectedExternalNodes(20);
    idData(23) = connectedExternalNodes(21);
    idData(24) = connectedExternalNodes(22);
    idData(25) = connectedExternalNodes(23);
    idData(26) = connectedExternalNodes(24);
    idData(27) = connectedExternalNodes(25);
    idData(28) = connectedExternalNodes(26);


    if (theChannel.sendID(dataTag, commitTag, idData) < 0)
    {
        std::cerr << "WARNING TwentySevenNodeBrick::sendSelf() - " << this->getTag() << " failed to send ID\n";
        return -1;
    }
    // Finally, quad asks its material objects to send themselves
    for (int i = 0; i < 27; i++)
    {
        if ((matpoint[i]->matmodel)->sendSelf(commitTag, theChannel) < 0)
        {
            std::cerr << "WARNING TwentySevenNodeBrick::sendSelf() - " << this->getTag() << " failed to send material models\n";
            return -1;
        }
    }



    static Vector matProp(4);
    matProp(0) = rho;
    matProp(1) = bf(0);
    matProp(2) = bf(1);
    matProp(3) = bf(2);

    if (theChannel.sendVector(dataTag, commitTag, matProp) < 0)
    {
        std::cerr << "WARNING TwentySevenNodeBrick::sendSelf() - " << this->getTag() << " failed to send its Material Property\n";
        return -1;
    }
    return 0;
}

//=============================================================================
//implemented by Babak Kamrani on 10/18/2012
int TwentySevenNodeBrick::receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    int dataTag = this->getDbTag();

    static ID idData(30);
    // Quad now receives the tags of its four external nodes
    if (theChannel.receiveID(dataTag, commitTag, idData) < 0)
    {
        std::cerr << "WARNING TwentySevenNodeBrick::receiveSelf() - " << this->getTag() << " failed to receive ID\n";
        return -1;
    }

    this->setTag(idData(29));

    connectedExternalNodes(0)  = idData(2);
    connectedExternalNodes(1)  = idData(3);
    connectedExternalNodes(2)  = idData(4);
    connectedExternalNodes(3)  = idData(5);
    connectedExternalNodes(4)  = idData(6);
    connectedExternalNodes(5)  = idData(7);
    connectedExternalNodes(6)  = idData(8);
    connectedExternalNodes(7)  = idData(9);
    connectedExternalNodes(8)  = idData(10);
    connectedExternalNodes(9)  = idData(11);
    connectedExternalNodes(10) = idData(12);
    connectedExternalNodes(11) = idData(13);
    connectedExternalNodes(12) = idData(14);
    connectedExternalNodes(13) = idData(15);
    connectedExternalNodes(14) = idData(16);
    connectedExternalNodes(15) = idData(17);
    connectedExternalNodes(16) = idData(18);
    connectedExternalNodes(17) = idData(19);
    connectedExternalNodes(18) = idData(20);
    connectedExternalNodes(19) = idData(21);
    connectedExternalNodes(20) = idData(22);
    connectedExternalNodes(21) = idData(23);
    connectedExternalNodes(22) = idData(24);
    connectedExternalNodes(23) = idData(25);
    connectedExternalNodes(24) = idData(26);
    connectedExternalNodes(25) = idData(27);
    connectedExternalNodes(26) = idData(28);

    int matClassTag = idData(0);
    int matDbTag = idData(1);


    determinant_of_Jacobian = 0.0;

    short where = 0;

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        double r = get_Gauss_p_c( 3, GP_c_r );
        double rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            double s = get_Gauss_p_c( 3, GP_c_s );
            double sw = get_Gauss_p_w( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                double t = get_Gauss_p_c( 3, GP_c_t );
                double tw = get_Gauss_p_w( 3, GP_c_t );

                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

                matpoint[where] = new MatPoint3D(GP_c_r,
                                                 GP_c_s,
                                                 GP_c_t,
                                                 r, s, t,
                                                 rw, sw, tw,

                                                 mmodel); //Guanzhou changed to point to global material model!

            }
        }
    }

    if (matpoint[0]->matmodel == 0)
    {
        for (int i = 0; i < 27; i++)
        {
            // Allocate new material with the sent class tag
            NDMaterial *ndmat = theBroker.getNewNDMaterial(matClassTag);
            if (ndmat == 0)
            {
                std::cerr << "TwentySevenNodeBrick::receiveSelf() - Broker could not create NDMaterial of class type " << matClassTag << endln;
                return -1;
            }
            // Now receive materials into the newly allocated space
            ndmat->setDbTag(matDbTag);
            if ((ndmat)->receiveSelf(commitTag, theChannel, theBroker) < 0)
            {
                std::cerr << "TwentySevenNodeBrick::receiveSelf() - material " << i << "failed to recv itself\n";
                return -1;
            }
            matpoint[i]->matmodel = ndmat;

        }
    }
    // materials exist , ensure materials of correct type and receiveSelf on them
    else
    {
        for (int i = 0; i < 27; i++)
        {
            NDMaterial *ndmat = theBroker.getNewNDMaterial(matClassTag);;
            // Check that material is of the right type; if not,
            // delete it and create a new one of the right type
            if ((matpoint[i]->matmodel)->getClassTag() != matClassTag)
            {
                delete matpoint[i]->matmodel;
                //        ndmat = theBroker.getNewNDMaterial(matClassTag);
                if (ndmat ==  0)
                {
                    std::cerr << "TwentySevenNodeBrick::receiveSelf() - Broker could not create NDMaterial of class type " << matClassTag << endln;
                    return -1;
                }
                ndmat->setDbTag(matDbTag);
            }
            // Receive the material
            if ((ndmat)->receiveSelf(commitTag, theChannel, theBroker) < 0)
            {
                std::cerr << "TwentySevenNodeBrick::receiveSelf() - material " << i << "failed to recv itself\n";
                return -1;
            }
            matpoint[i]->matmodel = ndmat;

        }
    }
    static Vector matProp(4);
    if ( theChannel.receiveVector(dataTag, commitTag, matProp) < 0 )
    {
        std::cerr << "TwentySevenNodeBrick::receiveSelf() - failed to recv rho!\n";
        return -1;
    }

    rho = matProp(0);
    bf(0) = matProp(1);
    bf(1) = matProp(2);
    bf(2) = matProp(3);
    return 0;
}
//Babak added for PDD
int TwentySevenNodeBrick::getObjectSize()
{
    int size = 0;
    size += 11 * sizeof(int);
    size += 4 * sizeof(double);
    for (int i = 0; i < 8; i++)
    {
        size += (matpoint[i]->matmodel)->getObjectSize();
    }
    //size for resisting force vector
    //    size += 24*sizeof(double);
    return size;
}



//=============================================================================
void TwentySevenNodeBrick::Print(ostream &s, int flag)
{
    //report(" TwentySevenNodeBrick ");
    s << "TwentySevenNodeBrick, element id:  " << this->getTag() << endln;
    s << "Connected external nodes:  " << connectedExternalNodes;

    s << "Element mass density:  " << rho << endln << endln;
    s << "Material model:  " << endln;

    for ( int GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        for ( int GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            for ( int GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                short where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

                s << "\n where = " << where << endln;
                s << " GP_c_r= " << GP_c_r << "GP_c_s = " << GP_c_s << " GP_c_t = " << GP_c_t << endln;
                matpoint[where]->report("Material Point\n");

            }
        }
    }
}



////#############################################################################
Vector TwentySevenNodeBrick::getWeightofGP(void)
{
    Vector Weight( 9 );

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    //     short where = 0;
    short rs = 0;
    double tmp = 0;

    double weight = 0.0;

    int dh_dim[] = {27, 3};

    tensor dh(2, dh_dim, 0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );
        rw = get_Gauss_p_w( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );
            sw = get_Gauss_p_w( 3, GP_c_s );

            rs = (GP_c_r - 1) * 3 + GP_c_s - 1;
            Weight(rs) = 0;

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                tw = get_Gauss_p_w( 3, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                //where =

                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                det_of_Jacobian  = Jacobian.determinant();

                weight = rw * sw * tw * det_of_Jacobian;
                Weight(rs) += weight;//Volume calculation

            }
            tmp += Weight(rs);
        }
    }

    return Weight;
}



double TwentySevenNodeBrick::get_Gauss_p_c(short order, short point_numb)
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

double TwentySevenNodeBrick::get_Gauss_p_w(short order, short point_numb)
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

int TwentySevenNodeBrick::update()  //Guanzhou added May 6, 2004
{
    double r  = 0.0;
    // double rw = 0.0;
    double s  = 0.0;
    // double sw = 0.0;
    double t  = 0.0;
    // double tw = 0.0;

    short where = 0;
    //,,,,,    double weight = 0.0;

    int dh_dim[] = {27, 3};
    tensor dh(2, dh_dim, 0.0);


    static int disp_dim[] = {27, 3};
    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    incremental_displacements = incr_disp();

    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );

        //--        rw = get_Gauss_p_w( 3, GP_c_r );
        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );

            //--            sw = get_Gauss_p_w( 3, GP_c_s );
            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );
                //--                tw = get_Gauss_p_w( 3, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;
                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");
                // determinant of Jacobian tensor ( matrix )
                //--                det_of_Jacobian  = Jacobian.determinant();
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                //dhGlobal = dh("ij") * JacobianINV("jk"); // Zhaohui 09-02-2001
                dhGlobal = dh("ij") * JacobianINV("kj");

                // incremental straines at this Gauss point
                // now in Update we know the incremental displacements so let's find
                // the incremental strain
                incremental_strain =
                    (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();
                //incremental_strain.reportshort("\n incremental_strain tensor at GAUSS point\n");

                // here comes the final_stress calculation actually on only needs to copy stresses
                // from the iterative data . . .
                //(GPstress+where)->reportshortpqtheta("\n stress START GAUSS \n");

                if ( ( (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)) )
                {
                    cerr << "TwentySevenNodeBrick::update (tag: " << this->getTag() << "), update() failed\n";
                }
            }
        }
    }

    return 0;
}



/////////////////////////////////////////////////////////////////////////
// Nima Tafazzoli added for surface load (July 2012)
Vector &TwentySevenNodeBrick::SurfaceShapeFunctionValues(double Xi , double Eta)
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

    return ShapeFunctionValues_in_function;

}


Vector &TwentySevenNodeBrick::Direction_Weight(double Xi , double Eta,
        Vector coord1, Vector coord2, Vector coord3, Vector coord4,
        Vector coord5, Vector coord6, Vector coord7, Vector coord8,
        Vector coord9)
{
    Vector J1(3);
    Vector J2(3);


    J1(0) = 0.25 * ( (1 + Eta) + (1 + Eta) * 2 * Xi - (1 - Eta * Eta) - 2 * Xi * (1 - Eta * Eta) ) * coord1(0)
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


    J_vector_in_function(0) =  J1(1) * J2(2) - J1(2) * J2(1) ;
    J_vector_in_function(1) =  J1(2) * J2(0) - J1(0) * J2(2) ;
    J_vector_in_function(2) =  J1(0) * J2(1) - J1(1) * J2(0) ;



    return J_vector_in_function;
}




double &TwentySevenNodeBrick::SurfaceLoadValues(double Xi , double Eta, Vector Pressure)
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


// Nima Tafazzoli (September 2012)
int
TwentySevenNodeBrick::CheckMesh(ofstream &checkmesh_file)
{


    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    int dh_dim[] = {27, 3};
    tensor dh(2, dh_dim, 0.0);


    double det_of_Jacobian = 0.0;
    bool jacobian_flag = false;

    tensor Jacobian;


    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        r = get_Gauss_p_c( 3, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            s = get_Gauss_p_c( 3, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                t = get_Gauss_p_c( 3, GP_c_t );

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
TwentySevenNodeBrick::getStress(void)
{

    int i = 0;
    int j = 0;
    stresstensor sts;
    Vector *stresses = new Vector(27 * 6);


    for ( short GP_c_r = 1 ; GP_c_r <= 3 ; GP_c_r++ )
    {
        for ( short GP_c_s = 1 ; GP_c_s <= 3 ; GP_c_s++ )
        {
            for ( short GP_c_t = 1 ; GP_c_t <= 3 ; GP_c_t++ )
            {
                i = ((GP_c_r - 1) * 3 + GP_c_s - 1) * 3 + GP_c_t - 1;

                sts = matpoint[i]->getStressTensor();
                (*stresses)(j * 6 + 0) = sts.cval(1, 1); //sigma_xx
                (*stresses)(j * 6 + 1) = sts.cval(2, 2); //sigma_yy
                (*stresses)(j * 6 + 2) = sts.cval(3, 3); //sigma_zz
                (*stresses)(j * 6 + 3) = sts.cval(1, 2); //Assign sigma_xy
                (*stresses)(j * 6 + 4) = sts.cval(1, 3); //Assign sigma_xz
                (*stresses)(j * 6 + 5) = sts.cval(2, 3); //Assign sigma_yz

                j++;
            }
        }
    }


    return stresses;

}




Matrix &TwentySevenNodeBrick::getGaussCoordinates(void)
{
    return gauss_points;
}



int TwentySevenNodeBrick::getOutputSize() const
{
    return TwentySevenNodeBrick_OUTPUT_SIZE;
}



const Vector &TwentySevenNodeBrick::getOutput() const
{


    return outputVector;
}


#endif
