///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               June 2012
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


//! Inputs:
//! - number_of_eigen_values: number of eigenvalues required (The default eigensolver is able to solve only for number_of_eigen_values-1 eigenvalues, where N is the number of inertial DOFs.)




int simulate_using_eigen_analysis(int number_of_eigen_values)
{

#ifdef _PARALLEL_PROCESSING
    cerr << "simulate_using_eigen_analysis():: Not available in _PARALLEL_PROCESSING" << endl;
    return -1;
#else
    if (theEigenSOE != 0)
    {
        if (theTransientAnalysis != 0)
        {
            theTransientAnalysis->setEigenSOE(*theEigenSOE);
        }
    }



    if (theTransientAnalysis == 0)
    {

        if (theAnalysisModel == 0)
        {
            theAnalysisModel = new AnalysisModel();
        }

        if (theAnalysisModel == NULL)
        {
            cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theAnalysisModel!" << endl;
            return -1;
        }

        if (theConvergenceTest == 0)
        {
            theConvergenceTest = new CTestNormUnbalance(1.0e-6, 25, 0);
        }

        if (theConvergenceTest == NULL)
        {
            cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theConvergenceTest!" << endl;
            return -1;
        }

        if (theAlgorithm == 0)
        {
            theAlgorithm = new Linear();
        }

        if (theAlgorithm == NULL)
        {
            cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theAlgorithm!" << endl;
            return -1;
        }

        if (theHandler == 0)
        {
            theHandler = new PlainHandler();
        }

        if (theHandler == NULL)
        {
            cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theHandler!" << endl;
            return -1;
        }

        if (theNumberer == 0)
        {
            theNumberer = new DOF_Numberer(*theRCM);
        }

        if (theNumberer == NULL)
        {
            cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theNumberer!" << endl;
            return -1;
        }

        if (theTransientIntegrator == 0)
        {
            theTransientIntegrator = new Newmark(0.5, 0.25);
        }

        if (theTransientIntegrator == NULL)
        {
            cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theTransientIntegrator!" << endl;
            return -1;
        }

        if (theSOE == 0)
        {
            ProfileSPDLinSolver *theSolver;
            theSolver = new ProfileSPDLinDirectSolver();

            if (theSolver == NULL)
            {
                cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theSolver!" << endl;
                return -1;
            }

            theSOE = new ProfileSPDLinSOE(*theSolver);

            if (theSOE == NULL)
            {
                cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theSOE!" << endl;
                return -1;
            }

        }

        theTransientAnalysis = new DirectIntegrationAnalysis(theDomain,
                *theHandler,
                *theNumberer,
                *theAnalysisModel,
                *theAlgorithm,
                *theSOE,
                *theTransientIntegrator,
                theConvergenceTest);


        if (theTransientAnalysis == NULL)
        {
            cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theTransientAnalysis!" << endl;
            return -1;
        }


    }



    if (theEigenSOE == 0)
    {


        // BandArpackSolver *theEigenSolver = new BandArpackSolver(number_of_eigen_values);
        FullGenEigenSolver *theEigenSolver = new FullGenEigenSolver();
        // theEigenSOE = new BandArpackSOE(*theEigenSolver, *theAnalysisModel);
        theEigenSOE = new FullGenEigenSOE(*theEigenSolver, *theAnalysisModel);


        if (theEigenSOE == NULL)
        {
            cerr << "Error: (simulate_using_eigen_analysis) memory allocation problem for theEigenSOE!" << endl;
            return -1;
        }


        if (theTransientAnalysis != 0)
        {
            theTransientAnalysis->setEigenSOE(*theEigenSOE);
        }

    }



    if (theTransientAnalysis != 0)
    {
        theTransientAnalysis->eigen(number_of_eigen_values);
    }

    /*********************** Added By Sumeet 1st August, 2016 *****************************/
    /* Eigen Analysis Output in Hdf5 file */

    theDomain.number_of_eigen_modes = number_of_eigen_values;

    string filename("");
    if((StageName.compare(""))==0){
        StageName = "Eigen_Mode_Analysis";
        filename = ModelName + "_" + StageName + ".h5.feioutput";
        // int numSteps = 1;
        theDomain.setOutputWriter(filename,
                                  ModelName,
                                  "Eigen_Mode_Analysis",
                                  1);

    }

    theDomain.setNumberOfOutputSteps(0);
    theDomain.output_step();              // Four outputting mesh in HDF5 Output; Also writes initial conditions 
    theDomain.Output_Eigen_Analysis();    // Writing Eigen value resuslts in HDF5 file

    /***************************************************************************************/
    
    //////////////// Not Required (Sumeet 1st August, 2016 ) ///////////////////////
    // const Vector &eigenvalues = theDomain.getEigenvalues();
    // Vector periodvalues(number_of_eigen_values);
    // Vector frequencyvalues(number_of_eigen_values);

    // double Pi = 2 * asin(1.0);


    // for (int i = 0; i < number_of_eigen_values; i++)
    // {
    //     double sqrtEigen = sqrt(eigenvalues(i));
    //     periodvalues(i) = 2 * Pi / sqrtEigen;
    //     frequencyvalues(i) = sqrtEigen / (2 * Pi);
    // }


    // //     for (int i=0; i<number_of_eigen_values; i++)
    // //        {
    // //           cout.flush() << "Period(" << i+1 << ")= " << periodvalues(i) << endl;
    // //           cout.flush() << "Frequency(" << i+1 << ")= " << frequencyvalues(i) << endl;
    // //           cout.flush() << "Eigen Value(" << i+1 << ")= " << eigenvalues(i) << endl;
    // //           cout.flush() << endl;
    // //        }


    // //============================================================

    // ofstream periodsfile;
    // string periodsnamestring = ModelName + "_" + "Periods.feioutput";
    // const char *periodfilename = periodsnamestring.c_str();
    // periodsfile.open(periodfilename , ios::out);


    // ofstream frequenciesfile;
    // string frequenciesnamestring = ModelName + "_" + "Frequencies.feioutput";
    // const char *frequencyfilename = frequenciesnamestring.c_str();
    // frequenciesfile.open(frequencyfilename , ios::out);


    // ofstream eigenvaluefile;
    // string eigenvaluesnamestring = ModelName + "_" + "EigenValue.feioutput";
    // const char *eigenvaluefilename = eigenvaluesnamestring.c_str();
    // eigenvaluefile.open(eigenvaluefilename , ios::out);



    // for (int i = 0; i < number_of_eigen_values; i++)
    // {
    //     periodsfile << periodvalues(i) << endl;
    //     frequenciesfile << frequencyvalues(i) << endl;
    //     eigenvaluefile << eigenvalues(i) << endl;
    // }



    // for (int i = 0; i < number_of_eigen_values; i++)
    // {
    //     // converting integer to string
    //     string integertostring;
    //     stringstream out;
    //     out << i + 1;
    //     integertostring = out.str();

    //     string filenamestring = ModelName + "_" + "EigenMode" + integertostring + ".feioutput";
    //     const char *filename = filenamestring.c_str();
    //     ofstream eigenmodefile;
    //     eigenmodefile.open(filename , ios::out);


    //     Node *nodePtr;
    //     NodeIter &theNodeIter = theDomain.getNodes();

    //     while ((nodePtr = theNodeIter()) != 0)
    //     {
    //         const Matrix &theEigenvectors = nodePtr->getEigenvectors();
    //         int ndof = (*nodePtr).getNumberDOF();

    //         for (int j = 0; j < ndof; j++)
    //         {
    //             eigenmodefile << theEigenvectors(j, i) << "  ";
    //         }

    //         eigenmodefile << endl;
    //     }


    // }


    // // ============================================================


    return 0;
#endif
};


