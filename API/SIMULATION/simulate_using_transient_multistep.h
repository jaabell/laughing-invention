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
// DATE:              October 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Transient (Dynamic) Analysis</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of transient (dynamic) analysis.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - dT: time-step increment
//! - numSteps: number of iterations performed at each step



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int simulate_using_transient_multistep(double dT,
                                       int numSteps)
{
#ifdef _PARALLEL_PROCESSING // Parallel processing
    cout << "\n";
    cout << "Starting parallel transient multistep analysis\n";
    cout << "====================================================================================================\n";
#else
    cout << "\n";
    cout << "Starting sequential transient multistep analysis\n";
    cout << "====================================================================================================\n";
#endif

    int result = 0;

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

    cout << "Checking transient integration handler.........................................................";
    if (theTransientIntegrator == NULL)
    {
        cout << "Fail!\n";
        return -1;
    }
    cout << "Pass!\n";




    cout << "Setting up the DirectIntegrationAnalysis.......................................................";
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
        cout << "Fail!\n";
        return -1;
    }
    cout << "Done!\n";



    //=====================================================================================
    // Parallel Dynamic Analysis
    //=====================================================================================
#ifdef _PARALLEL_PROCESSING // Parallel processing

    cout << "Setting domain decomposition analysis..........................................................";
    if (OPS_PARTITIONED == true && OPS_NUM_SUBDOMAINS > 1)
    {
        DomainDecompositionAnalysis *theSubAnalysis;
        SubdomainIter &theSubdomains = theDomain.getSubdomains();
        Subdomain *theSub = 0;
        // create the appropriate domain decomposition analysis
        while ((theSub = theSubdomains()) != 0)
        {
            theSubAnalysis = new TransientDomainDecompositionAnalysis(*theSub,
                    *theHandler,
                    *theNumberer,
                    *theAnalysisModel,
                    *theAlgorithm,
                    *theSOE,
                    *theTransientIntegrator,
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
            cout << "  Subdomain # " << i << " created. \n";
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

                theSubAnalysis = new TransientDomainDecompositionAnalysis(*theSub,
                        *theHandler,
                        *theNumberer,
                        *theAnalysisModel,
                        *theAlgorithm,
                        *theSOE,
                        *theTransientIntegrator,
                        theConvergenceTest,
                        false);

                theSub->setDomainDecompAnalysis(*theSubAnalysis);
            }
            OPS_REDEFINE_ANALYSIS = false;
        }

        result = theTransientAnalysis->analyze(1, dT);

        cout << "Analysis step # " << i  << " of " << numSteps << "\n";
    }
    cout << "> Analysis End -------------------------------------------------------------------------------";




    //=====================================================================================
    // Sequential Dynamic Analysis
    //=====================================================================================
#else
    // Analysis is sequential
    cout << "\n\n\n";
    cout << "> Analysis Start -----------------------------------------------------------------------------";
    // for (int i = 1; i <= numSteps; i++)
    // {
    theTransientAnalysis->analyze(numSteps, dT);
    // cout << "\nAnalysis step # " << i  << " of " << numSteps << " finished!\n";
    // }
    cout << "> Analysis End -------------------------------------------------------------------------------\n";

#endif
    //=====================================================================================

    return result;
};
