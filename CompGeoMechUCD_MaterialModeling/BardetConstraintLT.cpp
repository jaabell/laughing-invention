///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:
// CLASS:
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// DESIGNER:          Mahdi Taiebat and  Boris Jeremic
// PROGRAMMER:        Mahdi Taiebat and  Boris Jeremic
// DATE:              January 2009
// UPDATE HISTORY:    Nima Tafazzoli and Boris Jeremic
//                   (took it out of NewTemplate3Dep January 2009)
/////////////////////////////////////////////////////////////////////////////


#ifndef BardetConstraintLT_CPP
#define BardetConstraintLT_CPP

#include "BardetConstraintLT.h"

// // For Matrix based elements
// Matrix NewTemplate3Dep::D(6,6);
// BJmatrix NewTemplate3Dep::TangentMatrix( 6, 6, 0.0);
//

DTensor2 BardetConstraintLT(int type_of_test, double Increment, NDMaterialLT *material)
{
    //================================================================================
    // for more info look at the paper:
    // @article{Bardet91,
    //  author  = { Bardet, J. P. and Choucair, W. },
    //  title   = { A linearized integration technique for incremental constitutive equations },
    //  journal = { International Journal for Numerical and Analytical Methods in Geomechanics },
    //  year    = { 1991 },
    //    volume   = { 15 },
    //    number   = { 1 },
    //    pages    = { 1-19 },
    //    month    = { },
    //    note     = {  },
    //    napomena     = { local eCM1261 ; 18May2007 },
    // }
    //
    // constant_p_triaxial_loading_strain_control_d_epsilon_11         ->  1
    // drained_triaxial_loading_strain_control_d_epsilon_11            ->  2
    // Undrained_triaxial_loading_strain_control_d_epsilon_11          ->  3
    // Undrained_cyclic_triaxial_loading_stress_control_d_epsilon_11   ->  4
    // Undrained_simple_shear_loading_strain_control_d_epsilon_12      ->  5

    double K0 = 1.0;  // to be taken care of

    DTensor2 CM =  material->getTangent();
    //std::cerr << CM;

    DTensor2 CurrentStiffness(6, 6, 0.0);


    CurrentStiffness(0, 0) = CM(0, 0);
    CurrentStiffness(0, 1) = CM(0, 1);
    CurrentStiffness(0, 2) = CM(0, 2);
    CurrentStiffness(0, 3) = CM(0, 3);
    CurrentStiffness(0, 4) = CM(0, 4);
    CurrentStiffness(0, 5) = CM(0, 5);

    CurrentStiffness(1, 0) = CM(1, 0);
    CurrentStiffness(1, 1) = CM(1, 1);
    CurrentStiffness(1, 2) = CM(1, 2);
    CurrentStiffness(1, 3) = CM(1, 3);
    CurrentStiffness(1, 4) = CM(1, 4);
    CurrentStiffness(1, 5) = CM(1, 5);

    CurrentStiffness(2, 0) = CM(2, 0);
    CurrentStiffness(2, 1) = CM(2, 1);
    CurrentStiffness(2, 2) = CM(2, 2);
    CurrentStiffness(2, 3) = CM(2, 3);
    CurrentStiffness(2, 4) = CM(2, 4);
    CurrentStiffness(2, 5) = CM(2, 5);

    CurrentStiffness(3, 0) = CM(3, 0);
    CurrentStiffness(3, 1) = CM(3, 1);
    CurrentStiffness(3, 2) = CM(3, 2);
    CurrentStiffness(3, 3) = CM(3, 3);
    CurrentStiffness(3, 4) = CM(3, 4);
    CurrentStiffness(3, 5) = CM(3, 5);

    CurrentStiffness(4, 0) = CM(4, 0);
    CurrentStiffness(4, 1) = CM(4, 1);
    CurrentStiffness(4, 2) = CM(4, 2);
    CurrentStiffness(4, 3) = CM(4, 3);
    CurrentStiffness(4, 4) = CM(4, 4);
    CurrentStiffness(4, 5) = CM(4, 5);

    CurrentStiffness(5, 0) = CM(5, 0);
    CurrentStiffness(5, 1) = CM(5, 1);
    CurrentStiffness(5, 2) = CM(5, 2);
    CurrentStiffness(5, 3) = CM(5, 3);
    CurrentStiffness(5, 4) = CM(5, 4);
    CurrentStiffness(5, 5) = CM(5, 5);

    //CurrentStiffness.print("CS","CurrentStiffness");

    //    CurrentStiffness.print("C","C");


    double *p_S_values = NULL;
    double *p_E_values = NULL;


    // constant_p_triaxial_loading_strain_control_d_epsilon_11
    if  ( type_of_test == 1 )
    {
        ////////////////////////////////////////////////////////////////////
        // constant_p_triaxial_loading_strain_control_d_epsilon_11
        double S_values[] =
        {
            1.0  ,   1.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   1.0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            1.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };

        p_S_values = S_values;
        p_E_values = E_values;

    }

    else if ( type_of_test == 2 )
    {
        ////////////////////////////////////////////////////////////////////
        // drained_triaxial_loading_strain_control_d_epsilon_11
        double S_values[] =
        {
            0.0  ,   1.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            1.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        p_S_values = S_values;
        p_E_values = E_values;

    }

    else if ( type_of_test == 3 )
    {
        ////////////////////////////////////////////////////////////////////
        // Undrained_triaxial_loading_strain_control_d_epsilon_11
        double S_values[] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   1.0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[] =
        {
            1.0  ,   1.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            1.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        p_S_values = S_values;
        p_E_values = E_values;

    }

    else if ( type_of_test == 4 )
    {
        ////////////////////////////////////////////////////////////////////
        // Undrained_cyclic_triaxial_loading_stress_control_d_epsilon_11
        double S_values[] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   1.0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            1.0  ,  -1.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[] =
        {
            1.0  ,   1.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        p_S_values = S_values;
        p_E_values = E_values;

    }

    else if ( type_of_test == 5 )
    {
        ////////////////////////////////////////////////////////////////////
        // Undrained_simple_shear_loading_strain_control_d_epsilon_12
        double S_values[] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            K0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   1.0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[] =
        {
            1.0  ,   1.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0
        };
        p_S_values = S_values;
        p_E_values = E_values;

    }


    DTensor2 S(6, 6);//, p_S_values);

    //S.print("S","S");

    DTensor2 E(6, 6);//, p_E_values);

    //E.print("E","E");


    //    double number = 0.0;
    // vector constructor
    DTensor1 Y(6, 0.0);
    Y(5) =  Increment;

    //Y.print("Y","Y");
    //    ::fprintf(stderr," line 1 ");
    Index < 'i' > i;
    Index < 'j' > j;
    Index < 'p' > p;
    // Index < 'p' > q;

    DTensor2 S_times_C_plus_E(6, 6, 0.0);
    S_times_C_plus_E(i, j) = S(i, p) * CurrentStiffness(p, j) + E(i, j);

    //S_times_C_plus_E.print("SCE","S_times_C_plus_E");


    //    ::fprintf(stderr," line 2 ");
    double *det_Inverse_S_times_C_plus_E = NULL;
    DTensor2 Inverse_S_times_C_plus_E(6, 6, 0.0);
    Inverse_S_times_C_plus_E = S_times_C_plus_E.Inv(det_Inverse_S_times_C_plus_E);
    //Inverse_S_times_C_plus_E.print("I_SCE","Inverse_S_times_C_plus_E");

    //    ::fprintf(stderr," line 3 ");
    DTensor1 d_epsilon(6, 0.0);
    d_epsilon(i) = Inverse_S_times_C_plus_E(i, j) * Y(j);
    //d_epsilon.print("d_e","d_epsilon");


    DTensor2 d_epsilon_tensor(3, 3, 0.0);
    //d_epsilon_tensor.print("de");

    double sqrthalf = sqrt(0.5);

    // Adopted method from Helnwein (2001):
    d_epsilon_tensor(0, 0) = d_epsilon(0);
    d_epsilon_tensor(1, 1) = d_epsilon(1);
    d_epsilon_tensor(2, 2) = d_epsilon(2);
    d_epsilon_tensor(0, 1) = d_epsilon_tensor(1, 0) = d_epsilon(3) * sqrthalf;
    d_epsilon_tensor(1, 2) = d_epsilon_tensor(2, 1) = d_epsilon(4) * sqrthalf;
    d_epsilon_tensor(0, 2) = d_epsilon_tensor(2, 0) = d_epsilon(5) * sqrthalf;

    //d_epsilon_tensor.print("de");

    //out-  getchar();

    return d_epsilon_tensor;
}

#endif

