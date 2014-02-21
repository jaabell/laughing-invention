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
// DATE:              September 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Fiber Section</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of fiber section.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - SectionNumber: unique section object tag
//! - maxNumPatches: maximum number of patches to be used for the specified section
//! - maxNumReinfLayers: maximum number of reinforcing layers to be used for the specified section




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_section_to_model(int SectionNumber,
                         int maxNumPatches,
                         int maxNumReinfLayers)
{
    SectionRepres* fiberSectionRepr = 0;
    fiberSectionRepr = new FiberSectionRepr(SectionNumber, maxNumPatches, maxNumReinfLayers);

    if (fiberSectionRepr == NULL)
    {
        cerr << "Error: (add_section_to_model) memory allocation problem for fiberSectionRepr!" << endl;
        return -1;
    }

    if( theDomain.addSectionRepres(*fiberSectionRepr) != 0 )
    {
        cerr << "Error: (add_section_to_model) Section " << SectionNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};




