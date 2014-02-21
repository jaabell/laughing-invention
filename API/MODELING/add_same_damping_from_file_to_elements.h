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
// DATE:              July 2011
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Assigning Same Damping to Elements from File</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (December 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of assigning same damping to elements from file.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - inputelementsdampingfile: name of the input file which has the element numbers in it
//! - DampingNumber: damping number to be assigned to elements


//! Format of input file:
//! - In each line the element number should be written



int add_same_damping_from_file_to_elements(string inputelementsdampingfile, int DampingNumber)
{

    const char* inputfromfile = inputelementsdampingfile.c_str();

    ifstream inputdampingelements;

    if(! inputdampingelements)
    {
        cerr << "Error: (add_same_damping_from_file_to_elements) Can not open " << inputelementsdampingfile << "!" << endl;
        return -1;
    }

    inputdampingelements.open(inputfromfile , ios::in);


    while (!inputdampingelements.eof())
    {

        int ElementNumber;

        inputdampingelements >> ElementNumber;

        add_damping_to_element(ElementNumber, DampingNumber);

    }

    inputdampingelements.close();

    return 0;

};















