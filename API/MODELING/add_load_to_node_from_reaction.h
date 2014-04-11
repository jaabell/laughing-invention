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
// PROGRAMMER:        Jose Abell, Boris Jeremic
//
// DATE:              October 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



int add_load_to_node_from_reaction(int LoadNumber, int NodeNumber)
{

    TimeSeries *newTimeSeries;
    newTimeSeries = new ConstantSeries();
    LoadPattern *theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (start_new_stage) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(newTimeSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (start_new_stage) LoadPattern " << LoadNumber << " could not be added to the domain " << endl;
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
        node = theDomain.getNode(NodeNumber);
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