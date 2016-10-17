///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral LGPL (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     LGPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Domain Specific Language (DSL)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// DSL PROGRAMMER:    Nima Tafazzoli, Boris Jeremic
// DESIGNER:          Mahdi Taiebat, Boris Jeremic
// DATE:
// UPDATE HISTORY:    December 2009
//
//
///////////////////////////////////////////////////////////////////////////////


//******************************  Related Paper ********************************************

//  obtain  proper  strain  for given test condition from Bardet constraint (see paper:
//  J.P. Bardet and W. Choucair.
//  A linearized integration technique for incremental constitutive equations
//  International Journal for Numerical and Analytical Methods in Geomechanics
//  15(1):1-19, 1991

//********************************************************************************************



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Constitutive Testing for Drained Triaxial Test</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (December 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of constitutive testing for drained triaxial test.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//! \n Reference:
//! \n obtain  proper  strain  for given test condition from Bardet constraint (see paper:
//! \n J.P. Bardet and W. Choucair.
//! \n A linearized integration technique for incremental constitutive equations
//! \n International Journal for Numerical and Analytical Methods in Geomechanics
//! \n 15(1):1-19, 1991



//! Inputs:
//! - increment_size: size of the strain increment in each step
//! - strain_cut: absolute values of final strain (min/max strain)
//! - number_of_reaching_final_strain: number of times reaches minimum or maximum of final strain_cut
//! - MaterialNumber: tag number of previously nDMaterial defined

//! Outputs:
//! - Results.feioutput: -tStrain.val(1,1) , -tStrain.val(1,2) , -tStrain.val(2,2) , -tStrain.val(3,3) , -volstrain , -devstrain , pp , MIT_q , qq
//! - Strain.feioutput : -tStrain.val(1,1) , -tStrain.val(1,2) , -tStrain.val(2,2) , -tStrain.val(3,3) , -volstrain ,  -devstrain
//! - Stress.feioutput : -tStress.val(1,1) , -tStress.val(1,2) , -tStress.val(2,2) , -tStress.val(3,3) , pp , MIT_q , qq
//! - pq.feioutput     : p , q (for all the start_stress, cross_stress, predicted_stress, final_stress)


// #include "../../ltensor/LTensor.h"

int simulate_constitutive_testing_BardetLT_path(int MaterialNumber, int type, double scale_factor, std::string filein, double sxx0, double syy0, double szz0, double sxy0, double sxz0, double syz0, int verbose_output)
{

    BardetConstraintType bardet_type =  CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL;

    // enum BardetConstraintType
    // {
    //     CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL         ,
    //     DRAINED_TRIAXIAL_LOADING_STRESS_CONTROL         ,
    //     DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL            ,
    //     UNDRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL          ,
    //     UNDRAINED_CYCLIC_TRIAXIAL_LOADING_STRESS_CONTROL   ,
    //     UNDRAINED_SIMPLE_SHEAR_LOADING_STRAIN_CONTROL
    // };
    cout << "Bardet Constitutive Driver\n....... ";
    cout << "Test condition is: ";
    switch (type)
    {
    case 0://CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL
        cout << "Constant P Triaxial loading with strain control\n";
        bardet_type = CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL;
        break;

    case 1: //DRAINED_TRIAXIAL_LOADING_STRESS_CONTROL
        cout << "Drained Triaxial Loading with Stress Control\n";
        bardet_type = DRAINED_TRIAXIAL_LOADING_STRESS_CONTROL;
        break;

    case 2: //DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL
        cout << "Drained Triaxial Loading with Strain Control\n";
        bardet_type = DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL;
        break;

    case 3: //UNDRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL
        cout << "Un-Drained Triaxial Loading with Strain Control\n";
        bardet_type = UNDRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL;
        break;

    case 4: //UNDRAINED_TRIAXIAL_LOADING_STRESS_CONTROL
        cout << "Un-Drained Triaxial Loading with Strain Control\n";
        bardet_type = UNDRAINED_TRIAXIAL_LOADING_STRESS_CONTROL;
        break;

    case 5: //UNDRAINED_SIMPLE_SHEAR_LOADING_STRAIN_CONTROL
        cout << "Un-Drained Simple Shear Loading with Strain Control\n";
        bardet_type = UNDRAINED_SIMPLE_SHEAR_LOADING_STRAIN_CONTROL;
        break;
    }

//*************************  Material Models *************************************

    NDMaterialLT* material = theDomain.getNDMaterialLT(MaterialNumber);
    if (material == NULL)
    {
        cerr << "Error: (simulate_constitutive_testing_BardetLT_path) material allocation problem!" << endl;
        REALESSIGLOBAL_SIMULATE_RETURN_FLAG = -1;
        return REALESSIGLOBAL_SIMULATE_RETURN_FLAG;
    }

    DTensor2 s0(3, 3, 0);
    s0(0, 0) = sxx0;
    s0(1, 1) = syy0;
    s0(2, 2) = szz0;
    s0(0, 1) = sxy0;
    s0(1, 0) = sxy0;
    s0(0, 2) = sxz0;
    s0(2, 0) = sxz0;
    s0(1, 2) = syz0;
    s0(2, 1) = syz0;

    material->setStressTensor(s0);

    BardetConstraintLT driver(bardet_type, material);


    ofstream outStress("Stress.feioutput");
    ofstream outStrain("Strain.feioutput");
    ofstream outMaterial("Material_Output.feioutput");

    ifstream infile(filein.c_str());


    int step = 0;
    double increment;
    while (infile >> increment )
    {
        cout << "Step = " << step << " increment = " << increment << endl;
        const DTensor2 & stress = material->getStressTensor();
        const DTensor2 & strain = material->getStrainTensor();
        outStress << stress(0, 0)  << " " << stress(1, 1)  << " " << stress(2, 2)  << " " << stress(0, 1)  << " " << stress(0, 2)  << " " << stress(1, 2) << endl;
        outStrain << strain(0, 0)  << " " << strain(1, 1)  << " " << strain(2, 2)  << " " << strain(0, 1)  << " " << strain(0, 2)  << " " << strain(1, 2) << endl;
        if (verbose_output > 0 && (step % verbose_output) == 0)
        {
            material->Print(outMaterial);
            outMaterial << "\nStep = " << step << endl;
            outMaterial << "-----------------------------" << endl;
        }

        int status = driver.applyIncrement( increment);
        if (status < 0)
        {
            cerr << "simulate_constitutive_testing_BardetLT_path - applyIncrement() failed with error code " << status << endl;
            REALESSIGLOBAL_SIMULATE_RETURN_FLAG =  status;
            return status;
        }
        step++;
    }
    material->Print(outMaterial);
    outStress.close();
    outStrain.close();
    infile.close();

    REALESSIGLOBAL_SIMULATE_RETURN_FLAG = 0;
    return REALESSIGLOBAL_SIMULATE_RETURN_FLAG;
};


















