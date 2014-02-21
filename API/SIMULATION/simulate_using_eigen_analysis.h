///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): LGPL
//                 and
//                 Woody's license (adopted by BJ):
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
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              June 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Eigen Analysis</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of eigen analysis.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - number_of_eigen_values: number of eigenvalues required (The default eigensolver is able to solve only for number_of_eigen_values-1 eigenvalues, where N is the number of inertial DOFs.)




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




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


        FullGenEigenSolver *theEigenSolver = new FullGenEigenSolver();
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



    const Vector &eigenvalues = theDomain.getEigenvalues();
    Vector periodvalues(number_of_eigen_values);
    Vector frequencyvalues(number_of_eigen_values);

    double Pi = 2 * asin(1.0);


    for (int i = 0; i < number_of_eigen_values; i++)
    {
        double sqrtEigen = sqrt(eigenvalues(i));
        periodvalues(i) = 2 * Pi / sqrtEigen;
        frequencyvalues(i) = sqrtEigen / (2 * Pi);
    }


    //     for (int i=0; i<number_of_eigen_values; i++)
    //        {
    //           cout.flush() << "Period(" << i+1 << ")= " << periodvalues(i) << endl;
    //           cout.flush() << "Frequency(" << i+1 << ")= " << frequencyvalues(i) << endl;
    //           cout.flush() << "Eigen Value(" << i+1 << ")= " << eigenvalues(i) << endl;
    //           cout.flush() << endl;
    //        }


    //============================================================

    ofstream periodsfile;
    string periodsnamestring = ModelName + "_" + "Periods.feioutput";
    const char *periodfilename = periodsnamestring.c_str();
    periodsfile.open(periodfilename , ios::out);


    ofstream frequenciesfile;
    string frequenciesnamestring = ModelName + "_" + "Frequencies.feioutput";
    const char *frequencyfilename = frequenciesnamestring.c_str();
    frequenciesfile.open(frequencyfilename , ios::out);


    ofstream eigenvaluefile;
    string eigenvaluesnamestring = ModelName + "_" + "EigenValue.feioutput";
    const char *eigenvaluefilename = eigenvaluesnamestring.c_str();
    eigenvaluefile.open(eigenvaluefilename , ios::out);



    for (int i = 0; i < number_of_eigen_values; i++)
    {
        periodsfile << periodvalues(i) << endl;
        frequenciesfile << frequencyvalues(i) << endl;
        eigenvaluefile << eigenvalues(i) << endl;
    }



    for (int i = 0; i < number_of_eigen_values; i++)
    {
        // converting integer to string
        string integertostring;
        stringstream out;
        out << i + 1;
        integertostring = out.str();

        string filenamestring = ModelName + "_" + "EigenMode" + integertostring + ".feioutput";
        const char *filename = filenamestring.c_str();
        ofstream eigenmodefile;
        eigenmodefile.open(filename , ios::out);


        Node *nodePtr;
        NodeIter &theNodeIter = theDomain.getNodes();

        while ((nodePtr = theNodeIter()) != 0)
        {
            const Matrix &theEigenvectors = nodePtr->getEigenvectors();
            int ndof = (*nodePtr).getNumberDOF();

            for (int j = 0; j < ndof; j++)
            {
                eigenmodefile << theEigenvectors(j, i) << "  ";
            }

            eigenmodefile << endl;
        }


    }


    //============================================================


    return 0;
#endif
};


