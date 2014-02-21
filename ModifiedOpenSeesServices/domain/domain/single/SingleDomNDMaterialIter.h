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


#ifndef SingleDomNDMaterialIter_h
#define SingleDomNDMaterialIter_h

#include <NDMaterialIter.h>

class TaggedObjectStorage;
class TaggedObjectIter;

class SingleDomNDMaterialIter : public NDMaterialIter
{
    public:
        SingleDomNDMaterialIter(TaggedObjectStorage* theStorage);
        virtual ~SingleDomNDMaterialIter();

        virtual void reset(void);
        virtual NDMaterial* operator()(void);

    private:
        TaggedObjectIter& myIter;
};

#endif

