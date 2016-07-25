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


int simulate_constitutive_testing_for_drained_triaxial(double increment_size,
        double strain_cut,
        int number_of_reaching_final_strain,
        int MaterialNumber)
{

    //   cout << "\n*** Bardet Constitutive Testing ***" << "\n";
    cout << "Test condition is: Drained Triaxial \n";

    // Printing the values of increment size, final strain, type of algorithm, number of cycles, test condition
    //   printf("Increment size = %f \n", increment_size);
    //   printf("Final strain = %f \n", strain_cut);
    //   printf("Algorithm = %i \n", Algorithm);
    //   printf("Number of cycles = %i \n", number_of_reaching_final_strain);


    //*************************  Material Models *************************************

    NDMaterial* FTEP = theDomain.getNDMaterial(MaterialNumber);


    if (FTEP == NULL)
    {
        cerr << "Error: (simulate_constitutive_testing_for_drained_triaxial) material allocation problem!" << endl;
        return -1;
    }

    //*************************  Creating the output files *************************************

    ofstream outStress("Results.feioutput", fstream::app);
    ofstream    Strain("Strain.feioutput", fstream::app);
    ofstream    Stress("Stress.feioutput", fstream::app);

    //********************************************************************************************

    // Define how to limit cycles based on:
    //   double strain_cut = 0.03;      //  strain
    //   double  q_cut = 100.0;   //  deviatoric stress


    int current_cycle_counter = 0;

    straintensor Strain_Increment;
    straintensor tStrain;
    stresstensor tStress;

    // Initialize some of the parameters
    double pp = 0.0;
    double qq = 0.0;
    double MIT_q = 0.0;
    double E11 = 0.0;
    double volstrain = 0.0;
    double devstrain = 0.0;


    //=============================================================================================

    tStress = FTEP->getStressTensor();
    tStrain = FTEP->getStrainTensor();

    pp  = tStress.p_hydrostatic();   // mean effective stress
    MIT_q = -tStress.val(1, 1) + tStress.val(2, 2); // deviatoric stress
    qq  = tStress.q_deviatoric();     // deviatoric stress
    volstrain = (tStrain.val(1, 1) +
                 tStrain.val(2, 2) +
                 tStrain.val(3, 3)); // volumetric strain
    devstrain = 0.666 * (tStrain.val(1, 1) - tStrain.val(2, 2)); // deviatoric strain

    outStress << -tStrain.val(1, 1) <<  "  "  << -tStrain.val(1, 2) << "  " << -tStrain.val(2, 2) << "  " << -tStrain.val(3, 3) << "  " << -volstrain <<  "  " << -devstrain << "  " << pp << "  " << MIT_q << "  " << qq << endln;
    Strain    << -tStrain.val(1, 1) <<  "  "  << -tStrain.val(1, 2) << "  " << -tStrain.val(2, 2) << "  " << -tStrain.val(3, 3) << "  " << -volstrain <<  "  " <<  -devstrain << endln;
    Stress    << -tStress.val(1, 1) <<  "  "  << -tStress.val(1, 2) << "  " << -tStress.val(2, 2) << "  " << -tStress.val(3, 3) << "  " << pp << "  " << MIT_q << "  " << qq << endln;

    //=============================================================================================




    do
    {


        //******************************  Choice of Testing Conditions ********************************************

        int test_condition = 2;
        Strain_Increment = BardetConstraint(test_condition, increment_size, FTEP);

        //*********************************************************************************************************

        tStrain = tStrain + Strain_Increment;

        // Do the constitutive integration
        FTEP->setTrialStrain(tStrain);

        //       fprintf(stderr,"%5d\b\b\b\b\b", current_cycle_counter);
        //       fprintf(stderr,".");

        //********************************************************************************************

        // Extract stress results
        tStress = FTEP->getStressTensor();
        // Extract strain results
        tStrain = FTEP->getStrainTensor();


        // Calculate output variables
        E11 = tStrain.val(1, 1);         // strain epsilon_xx
        pp  = tStress.p_hydrostatic();   // mean effective stress
        qq  = tStress.q_deviatoric();     // deviatoric stress
        MIT_q = -tStress.val(1, 1) + tStress.val(2, 2); // deviatoric stress
        volstrain = (tStrain.val(1, 1) +
                     tStrain.val(2, 2) +
                     tStrain.val(3, 3)); // volumetric strain
        devstrain = 0.666 * (tStrain.val(1, 1) - tStrain.val(2, 2)); // deviatoric strain


        // Saving the results in the output files
        //       outStress << -tStrain.val(1,1)  <<  "  "  << -volstrain <<  "  " <<  -devstrain << "  " << pp << "  " << MIT_q << endln;
        outStress << -tStrain.val(1, 1) <<  "  "  << -tStrain.val(1, 2) << "  " << -tStrain.val(2, 2) << "  " << -tStrain.val(3, 3) << "  " << -volstrain <<  "  " << -devstrain << "  " << pp << "  " << MIT_q << "  " << qq << endln;
        Strain    << -tStrain.val(1, 1) <<  "  "  << -tStrain.val(1, 2) << "  " << -tStrain.val(2, 2) << "  " << -tStrain.val(3, 3) << "  " << -volstrain <<  "  " <<  -devstrain << endln;
        Stress    << -tStress.val(1, 1) <<  "  "  << -tStress.val(1, 2) << "  " << -tStress.val(2, 2) << "  " << -tStress.val(3, 3) << "  " << pp << "  " << MIT_q << "  " << qq << endln;

        // Check if the current strain is less than the final strain
        if (fabs(E11) >= strain_cut)
        {
            //if (fabs(qq2) >= q_cut) {
            //if (fabs(tau) >= q_cut) {
            //if (fabs(E11) >= strain_cut) {
            current_cycle_counter++;
            increment_size *= (-1.0);
        }


        //cout << "eps12= " << -tStrain.val(1,2) << "\n";
        //cout << "q= " << qq << "\n";
        //cout << "tau= " << tau << "\n";

    }
    while ( current_cycle_counter < number_of_reaching_final_strain );


    return 0;
};


















