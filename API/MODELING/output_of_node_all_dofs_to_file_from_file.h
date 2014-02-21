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
// DATE:              January 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Constructing a Node</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (December 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of constructing a node.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - inputelementsfile: name of the input file which has the elements information

//! Format of input file:
//! - The first argument should be the type of the elements which could be
//! one of the followings: Truss / EightNodeBrick / EightNodeBrickupU /


void output_of_node_all_dofs_to_file_from_file(const char* inputnoderecorderfile)
{

    const char* inputfromfile = inputnoderecorderfile;

    ifstream inputnoderecorder;

    if(! inputnoderecorder)
    {
        cerr << "Error: (define_output_of_node_all_dofs_to_file_from_file) Can not open " << inputnoderecorderfile << "!" << endl;
        abort();
    }

    inputnoderecorder.open(inputfromfile , ios::in);


    int NodeNumber;
    string filenamestr;
    string dataToStorestr;
    const char* filename;
    const char* dataToStore;
    double dT;
    int startFlag;


    while (inputnoderecorder >> NodeNumber >> filenamestr >> dataToStorestr >> dT >> startFlag)
    {

        filename = filenamestr.c_str();
        dataToStore = dataToStorestr.c_str();

        output_of_node_all_dofs_to_file(NodeNumber, filename, dataToStore, dT, startFlag);
    }


    inputnoderecorder.close();

};



