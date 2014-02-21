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


#include <Constitutive_Domain.h>

Constitutive_Domain::Constitutive_Domain()
{
    //    theElements = &theStorage;
    theNDMaterials = new ArrayOfTaggedObjects(32);
}


Constitutive_Domain::~Constitutive_Domain()
{
    if (theNDMaterials != 0)
    {
        delete theNDMaterials;
    }
}


int
Constitutive_Domain::addNDMaterial(NDMaterial& theMaterial)
{
    int materialTag = theMaterial.getTag();

    // check if a Material with a similar tag already exists in the Constitutive_Domain
    TaggedObject* other = theNDMaterials->getComponentPtr(materialTag);

    if (other != 0)
    {
        cerr << "Constitutive_Domain::NDMaterial - material with tag " << materialTag << "already exists in model\n";
        return false;
    }

    // add the element to the container object for the elements
    bool result = theNDMaterials->addComponent(&theMaterial);

    if (result == true)
    {
        return 0;
    }
    else
    {
        cerr << "Constitutive_Domain::NDMaterial - material with tag " << materialTag << "could not be added to container\n";
        return -1;
    }
}



void
Constitutive_Domain::clearAll(void)
{

    theNDMaterials->clearAll();

}


NDMaterial*
Constitutive_Domain::getNDMaterial(int tag)
{
    TaggedObject* mc = theNDMaterials->getComponentPtr(tag);

    if (mc == 0)
    {
        return 0;
    }

    // otherweise we do a cast and return
    NDMaterial* result = (NDMaterial*)mc;
    return result;

}
