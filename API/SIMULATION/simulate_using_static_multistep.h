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
// PROGRAMMER:        Babak Kamrani, Boris Jeremic
//
// DATE:              June 2010
// UPDATE HISTORY:    October (API)
//
///////////////////////////////////////////////////////////////////////////////

// Defining a parallel Static Analysis


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Static Analysis</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of static analysis.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - numSteps: number of steps of analysis


int simulate_using_static_multistep(int numSteps)
{
    int result = 0;


    // Check that all the analysis pointers are set
    if (theAnalysisModel == 0)
    {
        theAnalysisModel = new AnalysisModel();
    }
    if (theAnalysisModel == NULL)
    {
        cerr << "Error: (analyze_static_multistep) memory for analysis model can not be allocated!" << endl;
        return -1;
    }
    if (theHandler == NULL)
    {
        cerr << "Error: (analyze_static_multistep) The Constraint Handler is not set." << endl;
        return -1;
    }
    if (theNumberer == NULL)
    {
        cerr << "Error: (analyze_static_multistep) The Numberer is not set." << endl;
        return -1;
    }
    if (theAlgorithm == NULL)
    {
        cerr << "Error: (analyze_static_multistep) The Algorithm is not set/" << endl;
        return -1;
    }
    if (theSOE == NULL)
    {
        cerr << "Error: (analyze_static_multistep) The SOE is not set." << endl;
        return -1;
    }
    if (theStaticIntegrator == NULL)
    {
        cerr << "Error: (analyze_static_multistep) The Static Integrator is not set." << endl;
        return -1;
    }


    theStaticAnalysis = new StaticAnalysis(theDomain,
                                           *theHandler,
                                           *theNumberer,
                                           *theAnalysisModel,
                                           *theAlgorithm,
                                           *theSOE,
                                           *theStaticIntegrator,
                                           theConvergenceTest);

    if (theStaticAnalysis == NULL)
    {
        cerr << "Error: (analyze_static_multistep) memory for theStaticAnalysis can not be allocated!" << endl;
        return -1;
    }



    //=====================================================================================
    // Parallel Static Analysis
    //=====================================================================================

#ifdef _PARALLEL_PROCESSING

    if (OPS_PARTITIONED == true && OPS_NUM_SUBDOMAINS > 1)
    {
        DomainDecompositionAnalysis *theSubAnalysis;
        SubdomainIter &theSubdomains = theDomain.getSubdomains();
        Subdomain *theSub = 0;

        // create the appropriate domain decomposition analysis
        while ((theSub = theSubdomains()) != 0)
        {
            theSubAnalysis = new StaticDomainDecompositionAnalysis(*theSub,
                    *theHandler,
                    *theNumberer,
                    *theAnalysisModel,
                    *theAlgorithm,
                    *theSOE,
                    *theStaticIntegrator,
                    theConvergenceTest,
                    false);

            theSub->setDomainDecompAnalysis(*theSubAnalysis);
        }
    }

    if (OPS_PARTITIONED == false && OPS_NUM_SUBDOMAINS > 1)
    {
        if (OPS_theChannels != 0)
        {
            delete [] OPS_theChannels;
        }

        OPS_theChannels = new Channel *[OPS_NUM_SUBDOMAINS];

        // create some subdomains
        for (int i = 1; i <= OPS_NUM_SUBDOMAINS; i++)
        {
            ShadowSubdomain *theSubdomain = new ShadowSubdomain(i, *OPS_MACHINE, *OPS_OBJECT_BROKER);
            theDomain.addSubdomain(theSubdomain);
            OPS_theChannels[i - 1] = theSubdomain->getChannelPtr();
        }

        // create a partitioner & partition the domain
        if (OPS_DOMAIN_PARTITIONER == 0)
        {
#ifdef _PDD //Guanzhou added
            OPS_GRAPH_PARTITIONER  = new ParMetis;
#else
            OPS_GRAPH_PARTITIONER  = new Metis;
#endif
            OPS_DOMAIN_PARTITIONER = new DomainPartitioner(*OPS_GRAPH_PARTITIONER);
            theDomain.setPartitioner(OPS_DOMAIN_PARTITIONER);
        }

# ifdef _TIMING
        double start_time = MPI_Wtime();
# endif

        theDomain.partition(OPS_NUM_SUBDOMAINS);

# ifdef _TIMING
        double end_time = MPI_Wtime();
# endif

        OPS_PARTITIONED = true;

    }


    for (int i = 1; i <= numSteps; i++)
    {


        if ( OPS_REDEFINE_ANALYSIS == true )
        {
            DomainDecompositionAnalysis *theSubAnalysis;
            SubdomainIter &theSubdomains = theDomain.getSubdomains();
            Subdomain *theSub = 0;

            while ((theSub = theSubdomains()) != 0)
            {

                // create the appropriate domain decomposition analysis
                theSubAnalysis = new StaticDomainDecompositionAnalysis(*theSub,
                        *theHandler,
                        *theNumberer,
                        *theAnalysisModel,
                        *theAlgorithm,
                        *theSOE,
                        *theStaticIntegrator,
                        theConvergenceTest,
                        false);


                theSub->setDomainDecompAnalysis(*theSubAnalysis);
            }

            OPS_REDEFINE_ANALYSIS = false;
        }

        double start_time = MPI_Wtime();

        int numIncr = 1; // Number of increments in each step. added by Babak KAmrani 10/042011
        result = theStaticAnalysis->analyze(numIncr);
        double end_time = MPI_Wtime();
        cout << "\nAnalysis step " << i << " finished!\n";

    }

    // Out by J.Abell. --> Is done when wipe_model() is invoked.
    // if (theStaticAnalysis != 0)
    // {
    //     theStaticAnalysis->clearAll();
    //     delete theStaticAnalysis;
    // }


    //=====================================================================================
    // Sequential Static Analysis
    //=====================================================================================
#else
    // // bool isOutputBinary = false;
    // bool isTheAnalysisDynamic = false;
    // StateWriter output_writer(ModelName, StageName, isTheAnalysisDynamic);

    result = theStaticAnalysis->analyze(numSteps);


#endif

    // delete channel;

    return result;

}


