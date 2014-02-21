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
// DATE:              November 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Twenty Seven Node Brick Element</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (November 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of twenty seven node brick element.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - ElementNumber: unique element object tag
//! - node_numb_#: eight node coordinates
//! - MaterialNumber: material tag associated with previsouly-difined NDMaterial object




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_element_brick_27node_elastic(int ElementNumber,
                                     int node_numb_1,
                                     int node_numb_2,
                                     int node_numb_3,
                                     int node_numb_4,
                                     int node_numb_5,
                                     int node_numb_6,
                                     int node_numb_7,
                                     int node_numb_8,
                                     int node_numb_9,
                                     int node_numb_10,
                                     int node_numb_11,
                                     int node_numb_12,
                                     int node_numb_13,
                                     int node_numb_14,
                                     int node_numb_15,
                                     int node_numb_16,
                                     int node_numb_17,
                                     int node_numb_18,
                                     int node_numb_19,
                                     int node_numb_20,
                                     int node_numb_21,
                                     int node_numb_22,
                                     int node_numb_23,
                                     int node_numb_24,
                                     int node_numb_25,
                                     int node_numb_26,
                                     int node_numb_27,
                                     int MaterialNumber)

{
    NDMaterial* ndmaterial = 0;
    ndmaterial = theDomain.getNDMaterial(MaterialNumber);

    if (ndmaterial == NULL)
    {
        cerr << "Error: (add_element_brick_27node) memory allocation problem for ndmaterial!" << endl;
        return -1;
    }


    Element* theElement = 0;
    theElement = new TwentySevenNodeBrickElastic(ElementNumber, node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, node_numb_9, node_numb_10, node_numb_11, node_numb_12, node_numb_13, node_numb_14, node_numb_15, node_numb_16, node_numb_17, node_numb_18, node_numb_19, node_numb_20, node_numb_21, node_numb_22, node_numb_23, node_numb_24, node_numb_25, node_numb_26, node_numb_27, ndmaterial);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_brick_27node) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_brick_27node) could not add element to the domain\n";
        cerr << "Element number: " << ElementNumber << endln;
        return -1;
    }

    return 0;
};





