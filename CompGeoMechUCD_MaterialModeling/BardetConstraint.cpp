///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Mahdi Taiebat and  Boris Jeremic
// DATE:               January 2009
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


#ifndef BardetConstraint_CPP
#define BardetConstraint_CPP

#include "BardetConstraint.h"

// // For Matrix based elements
// Matrix NewTemplate3Dep::D(6,6);
// BJmatrix NewTemplate3Dep::TangentMatrix( 6, 6, 0.0);
//

straintensor BardetConstraint(int type_of_test, double Increment, NDMaterial* material)
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

    Matrix CM =  material->getTangent();
    //std::cerr << CM;

    BJmatrix CurrentStiffness(6, 6, 0.0);


    CurrentStiffness.val(1, 1) = CM(0, 0);
    CurrentStiffness.val(1, 2) = CM(0, 1);
    CurrentStiffness.val(1, 3) = CM(0, 2);
    CurrentStiffness.val(1, 4) = CM(0, 3);
    CurrentStiffness.val(1, 5) = CM(0, 4);
    CurrentStiffness.val(1, 6) = CM(0, 5);

    CurrentStiffness.val(2, 1) = CM(1, 0);
    CurrentStiffness.val(2, 2) = CM(1, 1);
    CurrentStiffness.val(2, 3) = CM(1, 2);
    CurrentStiffness.val(2, 4) = CM(1, 3);
    CurrentStiffness.val(2, 5) = CM(1, 4);
    CurrentStiffness.val(2, 6) = CM(1, 5);

    CurrentStiffness.val(3, 1) = CM(2, 0);
    CurrentStiffness.val(3, 2) = CM(2, 1);
    CurrentStiffness.val(3, 3) = CM(2, 2);
    CurrentStiffness.val(3, 4) = CM(2, 3);
    CurrentStiffness.val(3, 5) = CM(2, 4);
    CurrentStiffness.val(3, 6) = CM(2, 5);

    CurrentStiffness.val(4, 1) = CM(3, 0);
    CurrentStiffness.val(4, 2) = CM(3, 1);
    CurrentStiffness.val(4, 3) = CM(3, 2);
    CurrentStiffness.val(4, 4) = CM(3, 3);
    CurrentStiffness.val(4, 5) = CM(3, 4);
    CurrentStiffness.val(4, 6) = CM(3, 5);

    CurrentStiffness.val(5, 1) = CM(4, 0);
    CurrentStiffness.val(5, 2) = CM(4, 1);
    CurrentStiffness.val(5, 3) = CM(4, 2);
    CurrentStiffness.val(5, 4) = CM(4, 3);
    CurrentStiffness.val(5, 5) = CM(4, 4);
    CurrentStiffness.val(5, 6) = CM(4, 5);

    CurrentStiffness.val(6, 1) = CM(5, 0);
    CurrentStiffness.val(6, 2) = CM(5, 1);
    CurrentStiffness.val(6, 3) = CM(5, 2);
    CurrentStiffness.val(6, 4) = CM(5, 3);
    CurrentStiffness.val(6, 5) = CM(5, 4);
    CurrentStiffness.val(6, 6) = CM(5, 5);

    //CurrentStiffness.print("CS","CurrentStiffness");

    //    CurrentStiffness.print("C","C");


    double* p_S_values = NULL;
    double* p_E_values = NULL;


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


    BJmatrix S(6, 6, p_S_values);

    //S.print("S","S");

    BJmatrix E(6, 6, p_E_values);

    //E.print("E","E");


    //    double number = 0.0;
    // vector constructor
    BJmatrix Y(6, 1, 0.0);
    Y.val(6) =  Increment;

    //Y.print("Y","Y");
    //    ::fprintf(stderr," line 1 ");

    BJmatrix S_times_C_plus_E = S * CurrentStiffness + E;

    //S_times_C_plus_E.print("SCE","S_times_C_plus_E");


    //    ::fprintf(stderr," line 2 ");
    BJmatrix Inverse_S_times_C_plus_E = S_times_C_plus_E.inverse();
    //Inverse_S_times_C_plus_E.print("I_SCE","Inverse_S_times_C_plus_E");

    //    ::fprintf(stderr," line 3 ");
    BJmatrix d_epsilon = Inverse_S_times_C_plus_E * Y;
    //d_epsilon.print("d_e","d_epsilon");


    straintensor d_epsilon_tensor;
    //d_epsilon_tensor.print("de");

    double sqrthalf = sqrt(0.5);

    // Adopted method from Helnwein (2001):
    d_epsilon_tensor.val(1, 1) = d_epsilon.val(1);
    d_epsilon_tensor.val(2, 2) = d_epsilon.val(2);
    d_epsilon_tensor.val(3, 3) = d_epsilon.val(3);
    d_epsilon_tensor.val(1, 2) = d_epsilon_tensor.val(2, 1) = d_epsilon.val(4) * sqrthalf;
    d_epsilon_tensor.val(2, 3) = d_epsilon_tensor.val(3, 2) = d_epsilon.val(5) * sqrthalf;
    d_epsilon_tensor.val(1, 3) = d_epsilon_tensor.val(3, 1) = d_epsilon.val(6) * sqrthalf;

    //d_epsilon_tensor.print("de");

    //out-  getchar();

    return d_epsilon_tensor;
}

#endif

