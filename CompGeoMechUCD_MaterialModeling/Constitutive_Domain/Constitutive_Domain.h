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
// PURPOSE:           Domain for Constitutive Testing
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              December 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <ArrayOfTaggedObjects.h>
#include <ArrayOfTaggedObjectsIter.h>
#include <MapOfTaggedObjects.h>
#include <MapOfTaggedObjectsIter.h>
#include <TaggedObjectStorage.h>
#include <NDMaterial.h>

class Constitutive_Domain
{
    public:

        Constitutive_Domain();

        virtual ~Constitutive_Domain();

        virtual   int addNDMaterial(NDMaterial& theMaterial);

        virtual void clearAll(void);
        virtual  NDMaterial*  getNDMaterial(int tag);


    private:

        TaggedObjectStorage* theNDMaterials;
};



