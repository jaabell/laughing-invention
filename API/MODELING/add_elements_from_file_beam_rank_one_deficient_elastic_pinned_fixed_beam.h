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
// PROGRAMMER:        Babak Kamrani, Boris Jeremic
//
// DATE:              January 2011
// UPDATE HISTORY:
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
//! - AccelerationFieldNumber: acceleration field tag associated with previsouly-defined accelerationfield object
//! - rho: Material Density
//! - thickness: Element Thickness


//! Format of input file:
//! - The first argument should be the element number and the rest should be connectivity (4 node numbers)
//! Do not put any blank line at the end of the input file.


int add_elements_from_file_beam_rank_one_deficient_elastic_pinned_fixed_beam(string inputelementsfile, double A,
        double E,
        double G,
        double Jx,
        double Iy,
        double Iz,
        double rho,
        double vecxzPlane_X, double vecxzPlane_Y, double vecxzPlane_Z,
        double jntOffsetI_X, double jntOffsetI_Y, double jntOffsetI_Z,
        double jntOffsetJ_X, double jntOffsetJ_Y, double jntOffsetJ_Z)


{

    const char* inputfromfile = inputelementsfile.c_str();

    ifstream inputelements;

    if(! inputelements)
    {
        cerr << "Error: (add_elements_from_file_beam_rank_one_deficient_elastic_pinned_fixed_beam) Can not open " << inputelementsfile << "!" << endl;
        return -1;
    }

    inputelements.open(inputfromfile , ios::in);


    while (!inputelements.eof())
    {

        int ElementNumber;
        int node_numb_1; //6DOF node
        int node_numb_2; //3DOF node
        int iNode ;
        int jNode ;

        inputelements >> ElementNumber >>  node_numb_1 >> node_numb_2 ; //GMSH Format

        iNode = node_numb_2;
        jNode = node_numb_1;

        add_element_beam_rank_one_deficient_elastic_pinned_fixed_beam(ElementNumber,
                A,
                E,
                G,
                Jx,
                Iy,
                Iz,
                iNode,
                jNode,
                rho,
                vecxzPlane_X, vecxzPlane_Y, vecxzPlane_Z,
                jntOffsetI_X, jntOffsetI_Y, jntOffsetI_Z,
                jntOffsetJ_X, jntOffsetJ_Y,  jntOffsetJ_Z);

    }

    inputelements.close();

    return 0;
};




