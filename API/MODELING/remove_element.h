///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               October 2009 - Aug 2015
// UPDATE HISTORY:     See git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////

//! Inputs:
//! - ElementNumber: integer tag identifying the element to be removed



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



















