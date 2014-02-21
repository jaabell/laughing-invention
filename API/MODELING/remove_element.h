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
// DATE:              October 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Removing an Element</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of removing an element.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - ElementNumber: integer tag identifying the element to be removed



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n


int remove_element(int ElementNumber)
{
    Element* theEle = 0;
    theEle = theDomain.removeElement(ElementNumber);

    if (theEle != 0)
    {
        // we also have to remove any elemental loads from the domain
        LoadPatternIter& theLoadPatterns = theDomain.getLoadPatterns();
        LoadPattern* thePattern;

        // go through all load patterns
        while ((thePattern = theLoadPatterns()) != 0)
        {
            ElementalLoadIter theEleLoads = thePattern->getElementalLoads();
            ElementalLoad* theLoad;

            // go through all elemental loads in the pattern
            while ((theLoad = theEleLoads()) != 0)
            {

                // remove & destroy elemental from elemental load if there
                // note - if last element in load, remove the load and delete it

                // *****************
                int numLoadsLeft = theLoad->removeElement(ElementNumber);

                if (numLoadsLeft == 0)
                {
                    thePattern->removeElementalLoad(theLoad->getTag());
                    delete theLoad;
                }

                // *****************
            }
        }

        // finally invoke the destructor on the element
        delete theEle;
    }

    return 0;
};



















