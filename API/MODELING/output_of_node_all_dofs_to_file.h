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
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              Nov. 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


// Recording a Node

// Constructor for NodeRecorder:
// NodeRecorder::NodeRecorder(const ID &dofs,
// const ID &nodes, int psensitivity, const char *dataToStore,
// Domain &theDom, DataOutputHandler &theOutputHandler,
// double dT, int startFlag)


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Recording a Node Response</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of recording a node response.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - NodeNumber: integer tag of a node to be recorded
//! - dof: integer tag of dof to be recorded
//! - filename: filename to be used for recorder
//! - dataToStore: type of response to be recorded (disp, vel, accel, incrDisp, incrDeltaDisp, unbalance, eigen)
//! - dT: time interval of recorded response
//! - startFlag



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




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

    if( theDomain.addRecorder(*thenodeRecorder) != 0 )
    {
        cerr << "Error: (define_output_of_node_all_dofs_to_file) Recorder could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};










