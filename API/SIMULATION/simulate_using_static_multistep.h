///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell, Nima Tafazzoli, Boris Jeremic
// DATE:
// UPDATE HISTORY:     June 2010
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
//! - numSteps: number of steps of analysis


int simulate_using_static_multistep(int numSteps)
{
    int result = 0;

#ifdef _PARALLEL_PROCESSING // Parallel processing
    cout << "\n";
    cout << "Starting parallel static multistep analysis\n";
    cout << "====================================================================================================\n";
#else
    cout << "\n";
    cout << "Starting sequential static multistep analysis\n";
    cout << "====================================================================================================\n";
#endif


    cout << "Creating analysis model........................................................................";
    if (theAnalysisModel == NULL)
    {

        theAnalysisModel = new AnalysisModel();
    }
    if (theAnalysisModel == NULL)
    {
        cout << "Fail!\n";
        return -1;
    }
    cout << "Pass!\n";

    cout << "Checking constraint handler....................................................................";
    if (theHandler == NULL)
    {
        cout << "Fail!\n";
        return -1;
    }
    cout << "Pass!\n";

    cout << "Checking numberer..............................................................................";
    if (theNumberer == NULL)
    {
        cout << "Fail!\n";
        return -1;
    }
    cout << "Pass!\n";


    cout << "Checking analysis algorithm....................................................................";
    if (theAlgorithm == NULL)
    {
        cout << "Fail!\n";
        return -1;
    }
    cout << "Pass!\n";

    cout << "Checking system of equation handler............................................................";
    if (theSOE == NULL)
    {
        cout << "Fail!\n";
        return -1;
    }
    cout << "Pass!\n";

    cout << "Checking static integration handler.........................................................";
    if (theStaticIntegrator == NULL)
    {
        cout << "Fail!\n";
        return -1;
    }
    cout << "Pass!\n";


    theDomain.setNumberOfOutputSteps(numSteps);


    theStaticAnalysis = new StaticAnalysis(theDomain,
                                           *theHandler,
                                           *theNumberer,
                                           *theAnalysisModel,
                                           *theAlgorithm,
                                           *theSOE,
                                           *theStaticIntegrator,
                                           theConvergenceTest);

    if (profiling_results_filename.length() > 4)
    {
        globalESSITimer.setReportFileName(profiling_results_filename, ModelName + "_" + StageName);
        // theStaticAnalysis->setTimer(profiling_results_filename, ModelName + "_" + StageName);
    }
    else if (profiling_results_filename.length() > 0)
    {
        cout << "Profiling filename needs to have more than 4 characters. Currently: " << profiling_results_filename << endl;
    }

    if (theStaticAnalysis == NULL)
    {
        cerr << "Error: (simulate_using_static_multistep) memory for theStaticAnalysis can not be allocated!" << endl;
        return -1;
    }



    //=====================================================================================
    // Parallel Static Analysis
    //=====================================================================================

#ifdef _PARALLEL_PROCESSING

    cout << "Setting domain decomposition analysis..........................................................";
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

            if (theSubAnalysis == NULL)
            {
                cout << "Fail!\n";
                return -1;
            }

            theSub->setDomainDecompAnalysis(*theSubAnalysis);
        }
    }
    cout << "Done!\n";


    if (OPS_PARTITIONED == false && OPS_NUM_SUBDOMAINS > 1)
    {
        cout << "Creating subdomains............................................................................";
        cout << "\n";

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
            // cout << "  Subdomain # " << i << " created. \n";
        }
        cout << "...............................................................................................";
        cout << "Done!\n";



        cout << "Partitioning domain into subdomains............................................................";
        // create a partitioner & partition the domain
        if (OPS_DOMAIN_PARTITIONER == 0)
        {

            OPS_GRAPH_PARTITIONER  = new ParMetis;
            OPS_DOMAIN_PARTITIONER = new DomainPartitioner(*OPS_GRAPH_PARTITIONER);
            theDomain.setPartitioner(OPS_DOMAIN_PARTITIONER);
        }
        theDomain.partition(OPS_NUM_SUBDOMAINS);

        OPS_PARTITIONED = true;
        cout << "Done!\n";
    }


    cout << "\n\n\n";
    cout << "> Analysis Start -----------------------------------------------------------------------------";


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



    result = theStaticAnalysis->analyze(numSteps);

    cout << "> Analysis End -------------------------------------------------------------------------------";


    //=====================================================================================
    // Sequential Static Analysis
    //=====================================================================================
#else

    result = theStaticAnalysis->analyze(numSteps);
    cout << "> Analysis End -------------------------------------------------------------------------------\n";

#endif


    globalESSITimer.report();

    return result;

};


