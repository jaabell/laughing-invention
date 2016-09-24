///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell, Nima Tafazzoli  Boris Jeremic
// DATE:               January 2014
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


#ifndef BardetConstraintLT_CPP
#define BardetConstraintLT_CPP

#include "BardetConstraintLT.h"

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

BardetConstraintLT::BardetConstraintLT(BardetConstraintType type_of_test_in, NDMaterialLT *material_in):
    S(6, 6, 0.0),
    E(6, 6, 0.0),
    Y(6, 0.0),
    CurrentStiffness(6, 6, 0.0),
    d_epsilon_tensor(3, 3, 0.0)
{
    material = material_in;
    type_of_test = type_of_test_in;
    double K0 = 1.0;  // to be taken care of


    if  ( type_of_test == CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL )
    {
        cout << "BardetConstraint -- Selected \"constant_p_triaxial_loading_strain_control_d_epsilon_11\" \n";
        ////////////////////////////////////////////////////////////////////
        // constant_p_triaxial_loading_strain_control_d_epsilon_11
        double S_values[36] =
        {
            1.0  ,   1.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   1.0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[36] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            1.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };

        fillSE( S_values,  E_values);

    }

    else if ( type_of_test == DRAINED_TRIAXIAL_LOADING_STRESS_CONTROL)
    {
        cout << "BardetConstraint -- Selected \"drained_triaxial_loading_stress_control_d_sigma_11\" \n";
        ////////////////////////////////////////////////////////////////////
        // drained_triaxial_loading_strain_control_d_epsilon_11
        double S_values[36] =
        {
            0.0  ,   1.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            1.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[36] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        fillSE( S_values,  E_values);

    }

    else if ( type_of_test == DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL )
    {
        cout << "BardetConstraint -- Selected \"drained_triaxial_loading_strain_control_d_epsilon_11\" \n";
        ////////////////////////////////////////////////////////////////////
        // drained_triaxial_loading_strain_control_d_epsilon_11
        double S_values[36] =
        {
            0.0  ,   1.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[36] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            1.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        fillSE( S_values,  E_values);

    }

    else if ( type_of_test == UNDRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL )
    {
        cout << "BardetConstraint -- Selected \"Undrained_triaxial_loading_strain_control_d_epsilon_11\" \n";
        ////////////////////////////////////////////////////////////////////
        // Undrained_triaxial_loading_strain_control_d_epsilon_11
        double S_values[36] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   1.0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[36] =
        {
            1.0  ,   1.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            1.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        fillSE( S_values,  E_values);

    }

    else if ( type_of_test == UNDRAINED_TRIAXIAL_LOADING_STRESS_CONTROL )
    {
        cout << "BardetConstraint -- Selected \"Undrained_triaxial_loading_stress_control_d_epsilon_11\" \n";
        ////////////////////////////////////////////////////////////////////
        // Undrained_cyclic_triaxial_loading_stress_control_d_epsilon_11
        double S_values[36] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   1.0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            1.0  ,  -1.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[36] =
        {
            1.0  ,   1.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        fillSE( S_values,  E_values);

    }

    else if ( type_of_test == UNDRAINED_SIMPLE_SHEAR_LOADING_STRAIN_CONTROL )
    {
        cout << "BardetConstraint -- Selected \"Undrained_simple_shear_loading_strain_control_d_epsilon_12\" \n";
        ////////////////////////////////////////////////////////////////////
        // Undrained_simple_shear_loading_strain_control_d_epsilon_12
        double S_values[36] =
        {
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            K0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   1.0   ,  -1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   1.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   1.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0
        };
        double E_values[36] =
        {
            1.0  ,   1.0   ,   1.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   0.0   ,   0.0   ,   0.0,
            0.0  ,   0.0   ,   0.0   ,   1.0   ,   0.0   ,   0.0
        };
        fillSE( S_values,  E_values);
    }
    else
    {
        cerr << "BardetConstraint -- Unknown test type \n";
    }



}

BardetConstraintLT::~BardetConstraintLT()
{
    // cout << "BardetConstraintLT::~BardetConstraintLT()\n";
    // cout << "&S = " << &S << endl;
    // cout << "   &data = " << (S.data) << endl;
    // cout << "&E = " << &E << endl;
    // cout << "   &data = " << (E.data) << endl;
    // cout << "&Y = " << &Y << endl;
    // cout << "   &data = " << (Y.data) << endl;
    // cout << "&CurrentStiffness = " << &CurrentStiffness << endl;
    // cout << "   &data = " << (CurrentStiffness.data) << endl;
    // cout << "&d_epsilon_tensor = " << &d_epsilon_tensor << endl;
    // cout << "   &data = " << (d_epsilon_tensor.data) << endl;
}

void BardetConstraintLT::fillSE(double * S_values, double * E_values)
{
    //Fill matrices and vector appropriately
    int lindex = 0;
    for (int ii = 0; ii < 6; ii++)
    {
        for (int jj = 0; jj < 6; jj++)
        {
            S(ii, jj) = S_values[lindex];
            E(ii, jj) = E_values[lindex];
            lindex ++;
        }
    }
}

void BardetConstraintLT::applyIncrement(double Increment)
{

    //Get tangent from material and convert it to a format amenable to the formulation
    DTensor4 CM =  material->getTangentTensor();

    CurrentStiffness(0, 0) = CM(0, 0, 0, 0);
    CurrentStiffness(1, 0) = CM(1, 1, 0, 0);
    CurrentStiffness(2, 0) = CM(2, 2, 0, 0);
    CurrentStiffness(3, 0) = CM(0, 1, 0, 0);
    CurrentStiffness(4, 0) = CM(1, 2, 0, 0);
    CurrentStiffness(5, 0) = CM(2, 0, 0, 0);

    CurrentStiffness(0, 1) = CM(0, 0, 1, 1);
    CurrentStiffness(1, 1) = CM(1, 1, 1, 1);
    CurrentStiffness(2, 1) = CM(2, 2, 1, 1);
    CurrentStiffness(3, 1) = CM(0, 1, 1, 1);
    CurrentStiffness(4, 1) = CM(1, 2, 1, 1);
    CurrentStiffness(5, 1) = CM(2, 0, 1, 1);

    CurrentStiffness(0, 2) = CM(0, 0, 2, 2);
    CurrentStiffness(1, 2) = CM(1, 1, 2, 2);
    CurrentStiffness(2, 2) = CM(2, 2, 2, 2);
    CurrentStiffness(3, 2) = CM(0, 1, 2, 2);
    CurrentStiffness(4, 2) = CM(1, 2, 2, 2);
    CurrentStiffness(5, 2) = CM(2, 0, 2, 2);

    CurrentStiffness(0, 3) = 2 * CM(0, 0, 0, 1);
    CurrentStiffness(1, 3) = 2 * CM(1, 1, 0, 1);
    CurrentStiffness(2, 3) = 2 * CM(2, 2, 0, 1);
    CurrentStiffness(3, 3) = 2 * CM(0, 1, 0, 1);
    CurrentStiffness(4, 3) = 2 * CM(1, 2, 0, 1);
    CurrentStiffness(5, 3) = 2 * CM(2, 0, 0, 1);

    CurrentStiffness(0, 4) = 2 * CM(0, 0, 1, 2);
    CurrentStiffness(1, 4) = 2 * CM(1, 1, 1, 2);
    CurrentStiffness(2, 4) = 2 * CM(2, 2, 1, 2);
    CurrentStiffness(3, 4) = 2 * CM(0, 1, 1, 2);
    CurrentStiffness(4, 4) = 2 * CM(1, 2, 1, 2);
    CurrentStiffness(5, 4) = 2 * CM(2, 0, 1, 2);

    CurrentStiffness(0, 5) = 2 * CM(0, 0, 2, 0);
    CurrentStiffness(1, 5) = 2 * CM(1, 1, 2, 0);
    CurrentStiffness(2, 5) = 2 * CM(2, 2, 2, 0);
    CurrentStiffness(3, 5) = 2 * CM(0, 1, 2, 0);
    CurrentStiffness(4, 5) = 2 * CM(1, 2, 2, 0);
    CurrentStiffness(5, 5) = 2 * CM(2, 0, 2, 0);


    Y(5) =  Increment;


    //Some declarations
    DTensor2 S_times_C_plus_E(6, 6, 0.0);
    DTensor2 Inverse_S_times_C_plus_E(6, 6, 0.0);
    DTensor1 d_epsilon(6, 0.0);
    // double *det_Inverse_S_times_C_plus_E = NULL;

    S_times_C_plus_E(i, j)    = S(i, p) * CurrentStiffness(p, j) + E(i, j);
    // Inverse_S_times_C_plus_E  = S_times_C_plus_E.Inv(det_Inverse_S_times_C_plus_E);
    // d_epsilon(i)              = Inverse_S_times_C_plus_E(i, j) * Y(j);


    bool error = false;
    d_epsilon = S_times_C_plus_E.SolvebyGaussElim(Y, &error);
    // d_epsilon = S_times_C_plus_E.SolvebyDGESV(Y, &error);

    if (error)//det_Inverse_S_times_C_plus_E <= 0 )
        // if (det_Inverse_S_times_C_plus_E <= 0 )
    {
        cerr << "BardetConstraintLT::applyIncrement() -- Stiffness is not invertible.\n";

        cout << "S = "  << S << endl;
        cout << "E = "  << E << endl;
        cout << "CurrentStiffness = "  << CurrentStiffness << endl;
        cout << "S_times_C_plus_E = "  << S_times_C_plus_E << endl;
        cout << "Y = "  << Y << endl;
        cout << "d_epsilon = "  << d_epsilon << endl;
        cout << "CM = "  << CM << endl;

        exit(1);
    }

    // cout << "S = "  << S << endl;
    // cout << "E = "  << E << endl;
    // cout << "CurrentStiffness = "  << CurrentStiffness << endl;
    // cout << "S_times_C_plus_E = "  << S_times_C_plus_E << endl;
    // cout << "Y = "  << Y << endl;
    // cout << "d_epsilon = "  << d_epsilon << endl;
    // cout << "CM = "  << CM << endl;

    constexpr double sqrthalf = 0.707106781186548;
    d_epsilon_tensor(0, 0) = d_epsilon(0);
    d_epsilon_tensor(1, 1) = d_epsilon(1);
    d_epsilon_tensor(2, 2) = d_epsilon(2);
    d_epsilon_tensor(0, 1) = d_epsilon(3) * sqrthalf;
    d_epsilon_tensor(1, 0) = d_epsilon(3) * sqrthalf;
    d_epsilon_tensor(1, 2) = d_epsilon(4) * sqrthalf;
    d_epsilon_tensor(2, 1) = d_epsilon(4) * sqrthalf;
    d_epsilon_tensor(0, 2) = d_epsilon(5) * sqrthalf;
    d_epsilon_tensor(2, 0) = d_epsilon(5) * sqrthalf;

    //Send increment to the material
    material->setTrialStrainIncr(d_epsilon_tensor);            // Explicit integration takes place here
    material->commitState();
}



//Recover stress from material
const DTensor2 &BardetConstraintLT::getStress() const
{
    return material->getStressTensor();
}

const DTensor2 &BardetConstraintLT::getStrainIncrement() const
{
    return d_epsilon_tensor;
}

#endif

