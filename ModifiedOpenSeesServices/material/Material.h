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

// $Revision: 1.3 $
// $Date: 2003/02/25 23:33:21 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/Material.h,v $


#ifndef Material_h
#define Material_h

// File: ~/material/Material.h
//
// Written: fmk
// Created: 05/98
// Revision: A
//
// Description: This file contains the class definition for Material.
// Material is an abstract base class and thus no objects of it's type
// can be instantiated. It has pure virtual functions which must be
// implemented in it's derived classes.
//
// What: "@(#) Material.h, revA"

#include <DomainComponent.h>
#include <MovableObject.h>

class Information;
class Response;

class Material : public TaggedObject, public MovableObject
{
public:
    Material(int tag, int classTag);
    virtual ~Material();

    virtual int setVariable(const char* argv);
    virtual int getVariable(int variableID, double& info);

    // methods for sensitivity studies
    virtual int setParameter(const char** argv, int argc, Information& eleInformation);
    virtual int updateParameter(int responseID, Information& eleInformation);

    virtual Response* setResponse(const char** argv, int argc, Information& info);
    virtual int getResponse(int responseID, Information& info);

    virtual int startNewStage();  // Informs material about starting a new stage.

    // method for this material to update itself according to its new parameters
    virtual void update(void)
    {
        return;
    }

protected:

private:
};


#endif

