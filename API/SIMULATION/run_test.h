// This is to put some arbitrary code inside that can be run
// by executing the command runTest; on the parser.
// should be left clean upon commit!!


int run_test()
{
    cout << "This is a test!\n";

    double vp = 1000;
    double vs = 500;
    double rho = 2000;
    double phi = 30;
    double ha_alpha = 100;
    double cr_alpha = 700;
    double H_k = 0.0;
    double p0 = 100;
    double K0 = 0.5;
    int Nsteps = 100;



    double phirad = M_PI * phi / 180;
    double M = 6 * sin(phirad) / (3 - sin(phirad));
    double G = rho * vs * vs;
    double PWaveModulus = rho * vp * vp;
    double E = G * (3 * PWaveModulus - 4 * G) / (PWaveModulus - G);
    double nu = (PWaveModulus - 2 * G) / (2 * PWaveModulus - 2 * G);

    DruckerPragerArmstrongFrederick mat(1, M, ha_alpha, cr_alpha, H_k, E, nu, rho, p0) ;

    int method = (int) NDMaterialLT_Constitutive_Integration_Method::Backward_Euler;
    double f_relative_tol = 0.005;
    double stress_relative_tol = 1e-3;
    int n_max_iterations = 100;
    mat.set_constitutive_integration_method( method,  f_relative_tol,  stress_relative_tol,  n_max_iterations);

    DTensor2 strain(3, 3, 0.0);
    DTensor2 stress(3, 3, 0.0);

    stress(0, 0) = stress(1, 1)  = -K0 * p0;
    stress(2, 2) = -p0;
    mat.setTrialStress(stress);
    // mat.setTrialStrain(strain);
    mat.commitState();


    for (int step = 1; step < Nsteps ; step++)
    {
        cout << " >>>>>>>>>>>>>>>>>>>> Step " << step << endl;
        strain(0, 2) = 0.00000001 * step;
        mat.setTrialStrain(strain);
        printTensor("stress", stress);
        mat.commitState();
    }
    return 0;
};





