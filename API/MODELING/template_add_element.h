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

// This is an example API call to expose a new element into ESSI system.
// The naming convention for element functions is "add_element_[name]".
//
// An element adder API function must call the element constructor and store it in heap
// memory. Then it must use the pointer to the element in the heap memory to add the
// new element to the domain.
//
// Some elements are containers for other objects (notably materials, but also sections, even other
// elements!). So the function might use the domain API (fully available in this context)
// to query the domain class for pre-existing materials/nodes etc.
//
// It must also provide safety against not being able to add the element for whatever reason. If
// element is not added, must result in a negative error flag.

int add_element_[name_goes_here](int ElementNumber,
                                 parameters.....) // Input parameters might be the same as element constructor or not
{

    //Create a new instance of the element
    Element* theElement = 0;
    theElement = new EightNodeBrickLT(ElementNumber, parameters.....);  //Parameters might be different from input parameters

    //Check if we got a pointer
    if (theElement == NULL)
    {
        cerr << "Error: (add_element_[name_goes_here]) memory allocation problem for theElement!" << endl;
        return -1;
    }

    //Add element to domain class, check whether it was possible
    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_[name_goes_here]) could not add element to the domain\n";
        cerr << "Element number: " << ElementNumber << endln;
        return -1;
    }

    return 0;
};
