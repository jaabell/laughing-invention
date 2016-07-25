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
// PROGRAMMER:        Jose Abell, Boris Jeremic
// DATE:              December 2014
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#include "SingleDomNDMaterialLTIter.h"

#include <NDMaterialLT.h>
#include <TaggedObjectIter.h>
#include <TaggedObjectStorage.h>



SingleDomNDMaterialLTIter::SingleDomNDMaterialLTIter(TaggedObjectStorage *theStorage)
    : myIter(theStorage->getComponents())
{
}

SingleDomNDMaterialLTIter::~SingleDomNDMaterialLTIter()
{
}


void
SingleDomNDMaterialLTIter::reset(void)
{
    myIter.reset();
}


NDMaterialLT *
SingleDomNDMaterialLTIter::operator()(void)
{

    TaggedObject *theComponent = myIter();

    if (theComponent == 0)
    {
        return 0;
    }
    else
    {
        NDMaterialLT *result = (NDMaterialLT *)theComponent;
        return result;
    }
}




