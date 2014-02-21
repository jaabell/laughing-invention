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
// UPDATE HISTORY:    July 2011
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Creating 8Node Brick Elements from File</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (December 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of creating 8Node brick elements from file.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - inputelementsfile: name of the input file which has the elements information




int add_elements_from_file_contact_3dof_3dof(string inputelementsfile, double Knormal, double Ktangent, double frictionRatio,
        double x_local_1, double x_local_2, double x_local_3)
{

    const char* inputfromfile = inputelementsfile.c_str();

    ifstream inputelements;

    if(! inputelements)
    {
        cerr << "Error: (add_elements_from_file_contact_3dof_3dof) Can not open " << inputelementsfile << "!" << endl;
        return -1;
    }

    inputelements.open(inputfromfile , ios::in);


    while (!inputelements.eof())
    {

        int ElementNumber;
        int iNode;
        int jNode;


        inputelements >> ElementNumber >> iNode >> jNode;

        add_element_contact_3dof_to_3dof(ElementNumber,
                                         iNode,
                                         jNode,
                                         Knormal,
                                         Ktangent,
                                         frictionRatio,
                                         x_local_1,
                                         x_local_2,
                                         x_local_3);

    }

    inputelements.close();

    return 0;

};





