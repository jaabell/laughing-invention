///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               October 2009 - Aug 2015
// UPDATE HISTORY:     See git repository.
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



int add_load_to_node_from_reaction(int LoadNumber, int NodeNumber)
{

    TimeSeries *newTimeSeries;
    newTimeSeries = new ConstantSeries();
    LoadPattern *theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (add_load_to_node_from_reaction) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(newTimeSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_to_node_from_reaction) LoadPattern " << LoadNumber << " could not be added to the domain " << endl;
        return -1;
    }

    NodeIter &theNodes = theDomain.getNodes();
    Node *node;
    // theNodes.reset();
    NodalLoad *theLoad = 0;

    if (NodeNumber <= 0)
    {
        while ((node = theNodes()) != 0)
        {
            int NodeNumber = node->getTag();
            const Vector &reactions = node->getReaction();

            theLoad = new NodalLoad(LoadNumber, NodeNumber, reactions);

            if (theLoad == NULL)
            {
                cerr << "Error: (add_load) memory allocating for nodal load!" << endl;
                return -1;
            }

            if ( theDomain.addNodalLoad(theLoad, LoadNumber) == false )
            {
                cerr << "Error: (add_load) Nodal load could not be added to the domain " << endl;
                return -1;
            }
        }
    }
    else
    {
#ifdef _PARALLEL_PROCESING
        node = theDomain.getOutsideNode(NodeNumber);
        if (node == NULL)
        {
            cerr << "Error: (add_load) Node with tag " << NodeNumber << " not found" << endl;
            return -1;
        }
#else
        node = theDomain.getNode(NodeNumber);
        if (node == NULL)
        {
            cerr << "Error: (add_load) Node with tag " << NodeNumber << " not found" << endl;
            return -1;
        }
#endif

        const Vector &reactions = node->getReaction();

        theLoad = new NodalLoad(LoadNumber, NodeNumber, reactions);

        if (theLoad == NULL)
        {
            cerr << "Error: (add_load) memory allocating for nodal load!" << endl;
            return -1;
        }

        if ( theDomain.addNodalLoad(theLoad, LoadNumber) == false )
        {
            cerr << "Error: (add_load) Nodal load could not be added to the domain " << endl;
            return -1;
        }

    }
    return 0;
}