///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:               September 2016
// UPDATE HISTORY:     Full update history in git repository.
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


#ifndef DirectStrainConstitutiveDriver_CPP
#define DirectStrainConstitutiveDriver_CPP

#include "DirectStrainConstitutiveDriver.h"



DirectStrainConstitutiveDriver::DirectStrainConstitutiveDriver( NDMaterialLT *material_in)
{
    material = material_in;
}

DirectStrainConstitutiveDriver::~DirectStrainConstitutiveDriver()
{

}


void DirectStrainConstitutiveDriver::applyIncrement(const DTensor2& strain_increment)
{
    //Send increment to the material
    material->setTrialStrainIncr(strain_increment);
    material->commitState();
}



//Recover stress from material
const DTensor2 &DirectStrainConstitutiveDriver::getStress() const
{
    return material->getStressTensor();
}


#endif

