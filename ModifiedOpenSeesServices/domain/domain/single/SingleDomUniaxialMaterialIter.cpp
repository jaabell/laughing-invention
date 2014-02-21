///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              September 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#include "SingleDomUniaxialMaterialIter.h"

#include <UniaxialMaterial.h>
#include <TaggedObjectIter.h>
#include <TaggedObjectStorage.h>



SingleDomUniaxialMaterialIter::SingleDomUniaxialMaterialIter(TaggedObjectStorage* theStorage)
    : myIter(theStorage->getComponents())
{
}

SingleDomUniaxialMaterialIter::~SingleDomUniaxialMaterialIter()
{
}


void
SingleDomUniaxialMaterialIter::reset(void)
{
    myIter.reset();
}


UniaxialMaterial*
SingleDomUniaxialMaterialIter::operator()(void)
{

    TaggedObject* theComponent = myIter();

    if (theComponent == 0)
    {
        return 0;
    }
    else
    {
        UniaxialMaterial* result = (UniaxialMaterial*)theComponent;
        return result;
    }
}




