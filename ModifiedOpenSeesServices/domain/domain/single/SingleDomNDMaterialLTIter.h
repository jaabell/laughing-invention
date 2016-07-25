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


#ifndef SingleDomNDMaterialLTIter_h
#define SingleDomNDMaterialLTIter_h

#include <NDMaterialLTIter.h>

class TaggedObjectStorage;
class TaggedObjectIter;

class SingleDomNDMaterialLTIter : public NDMaterialLTIter
{
    public:
        SingleDomNDMaterialLTIter(TaggedObjectStorage *theStorage);
        virtual ~SingleDomNDMaterialLTIter();

        virtual void reset(void);
        virtual NDMaterialLT *operator()(void);

    private:
        TaggedObjectIter &myIter;
};

#endif

