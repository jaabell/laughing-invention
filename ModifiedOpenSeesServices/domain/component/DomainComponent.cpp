/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.4 $
// $Date: 2003/03/04 00:48:10 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/component/DomainComponent.cpp,v $


// File: ~/domain/component//DomainComponent.C
//
// Written: fmk 11/95
// Revised:
//
// Purpose: This file contains the class implementation for DomainComponent
//
// What: "@(#) DomainComponent.h, revA"


#include <Domain.h>
#include <DomainComponent.h>
#include <Vector.h>


DomainComponent::DomainComponent(int tag, int clasTag)
    : TaggedObject(tag), MovableObject(clasTag), myDomain(0)
{
    // does nothing else
}


DomainComponent::~DomainComponent()
{
    // does nothing
}


void
DomainComponent::setDomain(Domain *model)
{
    // sets the pointer
    myDomain = model;
}


Domain *
DomainComponent::getDomain(void) const
{
    // returns the current pointer
    return myDomain;
}




int DomainComponent::describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel)
{
    cerr << "DomainComponent::describeSelf -> Element with tag " << this->getTag() << " does not implement describeSelf() ! " << endl;
    return 0;
}