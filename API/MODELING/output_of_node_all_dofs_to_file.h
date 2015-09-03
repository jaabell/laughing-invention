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

//! Inputs:
//! - NodeNumber: integer tag of a node to be recorded
//! - dof: integer tag of dof to be recorded
//! - filename: filename to be used for recorder
//! - dataToStore: type of response to be recorded (disp, vel, accel, incrDisp, incrDeltaDisp, unbalance, eigen)
//! - dT: time interval of recorded response
//! - startFlag



int output_of_node_all_dofs_to_file(int NodeNumber,
                                    const char* filename,
                                    const char* dataToStore,
                                    double dT,
                                    int startFlag)
{
    //    int F = 0;
    int numdof = (theDomain.getNode(NodeNumber))->getNumberDOF();

    ID dofs (0, 7);

    for (int i = 0; i < numdof; i++)
    {
        dofs[i] = i;
    }

    //    dofs[F] = dof-1;

    // int startFlag = 0;

    int numNodes = 0;
    ID nodes (0, 16);
    nodes[numNodes] = NodeNumber;

    //    const char *dataToStore;
    const char* fileName = filename;

    echoMode theEMode = NONE;
    openMode theOMode = OVERWRITE;

    DataOutputHandler* theOutputHandler = 0;
    theOutputHandler = new DataOutputFileHandler(fileName, theEMode, theOMode);

    if (theOutputHandler == NULL)
    {
        cerr << "Error: (define_output_of_node_all_dofs_to_file) memory allocation problem for theOutputHandler!" << endl;
        return -1;
    }

    Recorder* thenodeRecorder = 0;
    thenodeRecorder = new NodeRecorder(dofs, nodes, dataToStore, theDomain, *theOutputHandler, dT, startFlag);

    if (thenodeRecorder == NULL)
    {
        cerr << "Error: (define_output_of_node_all_dofs_to_file) memory allocation problem for thenodeRecorder!" << endl;
        return -1;
    }

    if ( theDomain.addRecorder(*thenodeRecorder) != 0 )
    {
        cerr << "Error: (define_output_of_node_all_dofs_to_file) Recorder could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};










