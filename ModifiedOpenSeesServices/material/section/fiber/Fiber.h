/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** See Copyright end of file.                                         **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// Written: Remo Magalhaes de Souza
// Created: 10/98

//
// Description: This file contains the class definition for
// Fiber. Fiber is an abstract base class and thus no objects of
// it's type can be instatiated. It has pure virtual functions which
// must be implemented in it's derived classes.
//
// What: "@(#) Fiber.h, revA"


#ifndef Fiber_h
#define Fiber_h

#include <DomainComponent.h>
#include <MovableObject.h>
#include <Vector.h>

class Matrix;
class ID;
class UniaxialMaterial;
class NDMaterial;
class Information;
class Response;

class Fiber : public TaggedObject, public MovableObject
{
public:
    Fiber (int tag, int classTag);
    virtual ~Fiber();

    virtual int    setTrialFiberStrain(const Vector &vs) = 0;
    virtual Vector &getFiberStressResultants(void) = 0;
    virtual Matrix &getFiberTangentStiffContr(void) = 0;

    virtual int    commitState(void) = 0;
    virtual int    revertToLastCommit(void) = 0;
    virtual int    revertToStart(void) = 0;

    virtual Fiber *getCopy(void) = 0;
    virtual int getOrder(void) = 0;
    virtual const ID &getType(void) = 0;

    virtual Response *setResponse(const char **argv, int argc, ostream &s);
    virtual int getResponse(int responseID, Information &info);

    virtual void getFiberLocation(double &y, double &z) = 0;
    virtual double getArea(void) = 0;

    virtual UniaxialMaterial *getMaterial(void)
    {
        return 0;
    }
    virtual NDMaterial *getNDMaterial(void)
    {
        return 0;
    }

    virtual const Vector &getFiberSensitivity(int gradNumber, bool cond);
    virtual int commitSensitivity(const Vector &dedh, int gradNumber,
                                  int numGrads);

protected:
    Vector *sDefault;
    Matrix *fDefault;

private:
};

/*
Copyright @ 1999,2000 The Regents of the University of California (The Regents).
All Rights Reserved.

The Regents grants permission, without fee and without a written license agreement,
for (a) use, reproduction, modification, and distribution of this software and its
documentation by educational, research, and non-profit entities for noncommercial
purposes only; and (b) use, reproduction and modification of this software by other
entities for internal purposes only. The above copyright notice, this paragraph and
the following three paragraphs must appear in all copies and modifications of the
software and/or documentation.

Permission to incorporate this software into products for commercial distribution
may be obtained
by contacting the University of California
Office of Technology Licensing
2150 Shattuck Avenue #510,
Berkeley, CA 94720-1620,
(510) 643-7201.

This software program and documentation are copyrighted by The Regents of the University
of California. The Regents does not warrant that the operation of the program will be
uninterrupted or error-free. The end-user understands that the program was developed
for research purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE
AND ITS DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
REGENTS GRANTS NO EXPRESS OR IMPLIED LICENSE IN ANY PATENT RIGHTS OF REGENTS BUT HAS
IMPLEMENTED AN INDIVIDUAL CONTRIBUTOR LICENSE AGREEMENT FOR THE OPENSEES PROJECT AT THE
UNIVERISTY OF CALIFORNIA, BERKELEY TO BENEFIT THE END USER.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND
ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS
NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#endif
