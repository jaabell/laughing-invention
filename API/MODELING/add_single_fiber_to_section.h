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
// PURPOSE:           C Binding Commands
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              Novermber 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


int add_single_fiber_to_section(int SectionNumber,
                                int MaterialNumber,
                                double yLocation,
                                double zLocation,
                                double Area)
{

    SectionRepres* sectionRepres = 0;
    sectionRepres = theDomain.getSectionRepres(SectionNumber);


    FiberSectionRepr* fiberSectionRepr = (FiberSectionRepr*) sectionRepres;
    int numFibers = fiberSectionRepr->getNumFibers();


    UniaxialMaterial* uniaxialmaterial = theDomain.getUniaxialMaterial(MaterialNumber);


    static Vector fiberPosition(2);
    fiberPosition(0) = yLocation;
    fiberPosition(1) = zLocation;


    Fiber* theFiber = 0;
    theFiber = new UniaxialFiber3d(numFibers, *uniaxialmaterial, Area, fiberPosition);

    if (theFiber == 0)
    {
        cerr.flush() <<  "add_single_fiber_to_section: WARNING unable to allocate fiber \n";
        return -1;
    }


    int error = fiberSectionRepr->addFiber(*theFiber);

    if (error)
    {
        cerr.flush() <<  "add_single_fiber_to_section: WARNING cannot add patch to section\n";
        return -1;
    }


    return 0;

};




