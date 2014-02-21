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
//! <h1>DSL of Reinforcement Bars (Circle)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of circular type of reinforcement bars.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!



//! Inputs:
//! - SectionNumber: integer tag of the section
//! - MaterialNumber: material integer tag of the previously-defined UniaxialMaterial object used to represent the stress-strain for the area of the fiber
//! - numReinfBars: number of reinforcing bars along layer
//! - reinfBarArea: area of individual reinforcing bar
//! - yCenter, zCenter: y and z-coordinates of center of reinforcing layer (local coordinate system)
//! - radius: radius of reinforcing layer




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_reinforcing_bars_circle_to_section(int SectionNumber,
        int MaterialNumber,
        int numReinfBars,
        double reinfBarArea,
        double yCenter,
        double zCenter,
        double radius)
{

    // get section

    SectionRepres* sectionRepres = theDomain.getSectionRepres(SectionNumber);

    if (sectionRepres == 0)
    {
        cerr <<  "WARNING: (add_reinforcing_bars_circle_to_section) cannot retrieve section\n";
        return -1;
    }


    FiberSectionRepr* fiberSectionRepr = (FiberSectionRepr*) sectionRepres;


    if (fiberSectionRepr == 0)
    {
        cerr <<  "WARNING: (add_reinforcing_bars_circle_to_section) cannot alocate fiberSectionRepr\n";
        return -1;
    }

    // create the reinforcing layer

    static Vector center(2);

    center(0) = yCenter;
    center(1) = zCenter;

    CircReinfLayer* reinfLayer = 0;


    // Construct circle
    reinfLayer = new CircReinfLayer (MaterialNumber, numReinfBars, reinfBarArea,
                                     center, radius);


    if (!reinfLayer)
    {
        cerr <<  "WARNING: (add_reinforcing_bars_circle_to_section) cannot alocate reinfLayer\n";
        return -1;
    }

    // add reinfLayer to section
    int error = fiberSectionRepr->addReinfLayer(*reinfLayer);
    delete reinfLayer;

    if (error)
    {
        cerr <<  "WARNING: (add_reinforcing_bars_circle_to_section) cannot add reinforcing layer to section\n";
        return -1;
    }

    return 0;

};



