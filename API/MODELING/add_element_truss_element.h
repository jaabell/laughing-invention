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
// DATE:              August 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Truss Element</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of truss element.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - ElementNumber: unique element object tag
//! - dimension: number of dimensions of the beam
//! - iNode , jNode: end nodes
//! - MaterialNumber: tag of the uniaxial material to be used
//! - sectionarea: section area of the truss element
//! - rho: density





//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n





void add_element_truss_element(int, int, int, int, int, double , double);
void add_element_truss_element(int ElementNumber,
                               int dimension,
                               int iNode,
                               int jNode,
                               int MaterialNumber,
                               double sectionarea,
                               double rho)
{
    UniaxialMaterial* uniaxialmaterial = 0;
    uniaxialmaterial = theDomain.getUniaxialMaterial(MaterialNumber);

    Element* theElement = 0;
    theElement = new Truss(ElementNumber, dimension, iNode, jNode, *uniaxialmaterial, sectionarea, rho);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_truss_element) memory allocation problem for theElement!" << endl;
        abort();
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_truss_element) could not add element to the domain\n";
        cerr << "Element element: " << ElementNumber << endln;
    }
};





