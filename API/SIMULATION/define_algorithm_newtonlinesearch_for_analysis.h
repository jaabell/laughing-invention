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
// PROGRAMMER:        Jose Abell
//
// DATE:              June 2016
//
///////////////////////////////////////////////////////////////////////////////


int define_algorithm_newtonlinesearch_for_analysis()
{

    if (theConvergenceTest == NULL)
    {
        cerr << "Error: (define_algorithm_newtonlinesearch_for_analysis) the convergence test has not been set to be used by algorithm!" << endl;
        return -1;
    }

    LineSearch* theLineSearch = new InitialInterpolatedLineSearch();
    theAlgorithm = new NewtonLineSearch(*theConvergenceTest, theLineSearch);


    if (theAlgorithm == NULL)
    {
        cerr << "Error: (define_algorithm_newtonlinesearch_for_analysis) memory for defining algorithm can not be allocated!" << endl;
        return -1;
    }

    return 0;

};


