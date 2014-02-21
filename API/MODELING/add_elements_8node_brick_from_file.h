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
//! - MaterialNumber: tag number assigned to nDMaterial
//! - BodyForceNumber: bodyforce tag associated with previsouly-defined bodyforce object


//! Format of input file:
//! - The first argument should be the element number and the rest should be connectivity (8 node numbers)


void add_elements_8node_brick_from_file(const char*, int, int);
void add_elements_8node_brick_from_file(const char* inputelementsfile, int MaterialNumber, int BodyForceNumber)
{

    const char* inputfromfile = inputelementsfile;

    ifstream inputelements;

    if(! inputelements)
    {
        cerr << "Error: (add_elements_8node_brick_from_file) Can not open " << inputelementsfile << "!" << endl;
        abort();
    }

    inputelements.open(inputfromfile , ios::in);


    while (!inputelements.eof())
    {

        int ElementNumber;
        int node_numb_1;
        int node_numb_2;
        int node_numb_3;
        int node_numb_4;
        int node_numb_5;
        int node_numb_6;
        int node_numb_7;
        int node_numb_8;


        inputelements >> ElementNumber >> node_numb_1 >> node_numb_2 >> node_numb_3 >> node_numb_4 >>
                      node_numb_5 >> node_numb_6 >> node_numb_7 >> node_numb_8;

        #ifdef _PARALLEL_PROCESSING

        add_element_brick_8node_Parallel(ElementNumber, node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, MaterialNumber, 0.00, 0.00, -9.81, 10.00, 0.00);

        #else

        add_element_brick_8node(ElementNumber, node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, MaterialNumber, BodyForceNumber);
        #endif

    }

    inputelements.close();

};















