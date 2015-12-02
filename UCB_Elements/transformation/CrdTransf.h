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

// $Revision: 1.6 $
// $Date: 2009-08-19 17:53:01 $
// $Source: /usr/local/cvs/OpenSees/SRC/coordTransformation/CrdTransf.h,v $

// Written: Remo Magalhaes de Souza (rmsouza@ce.berkeley.edu)
// Created: 04/2000
// Revision: A
//
// Description: This file contains the class definition for
// CrdTransf.h. CrdTransf provides the abstraction of a frame
// coordinate transformation. It is an abstract base class and
// thus no objects of  it's type can be instatiated. It has pure
// virtual functions which  must be implemented in it's derived classes.
//
// What: "@(#) CrdTransf.h, revA"

#ifndef CrdTransf_h
#define CrdTransf_h

#include <MovableObject.h>
#include <TaggedObject.h>

class Vector;
class Matrix;
class Node;

// class definition

class CrdTransf: public TaggedObject, public MovableObject
{
public:
    CrdTransf(int tag, int classTag);
    CrdTransf();
    virtual ~CrdTransf();

    virtual CrdTransf *getCopy2d(void)
    {
        return 0;
    };
    virtual CrdTransf *getCopy3d(void)
    {
        return 0;
    };
    virtual int getLocalAxes(Vector &xAxis, Vector &yAxis, Vector &zAxis)
    {
        return -1;
    };

    virtual int    initialize(Node *node1Pointer, Node *node2Pointer) = 0;
    virtual int    update(void) = 0;
    virtual double getInitialLength(void) = 0;
    virtual double getDeformedLength(void) = 0;

    virtual int commitState(void) = 0;
    virtual int revertToLastCommit(void) = 0;
    virtual int revertToStart(void) = 0;

    virtual const Vector &getBasicTrialDisp(void) = 0;
    virtual const Vector &getBasicIncrDisp(void) = 0;
    virtual const Vector &getBasicIncrDeltaDisp(void) = 0;
    virtual const Vector &getBasicTrialVel(void) = 0;
    virtual const Vector &getBasicTrialAccel(void) = 0;

    // AddingSensitivity:BEGIN //////////////////////////////////
    // virtual const Vector &getBasicDisplSensitivity(int gradNumber);
    // virtual const Vector &getBasicDisplSensitivity(int gradNumber,int); // used by Quan
    // //virtual const Vector &getGlobalResistingForceShapeSensitivity(const Vector &basicForce, const Vector &uniformLoad);
    // virtual const Vector &getGlobalResistingForceShapeSensitivity(const Vector &pb, const Vector &p0, int gradNumber);
    // virtual const Vector &getGlobalResistingForceShapeSensitivity(const Vector &pb, const Vector &p0);
    // virtual const Vector &getBasicTrialDispShapeSensitivity(void);
    // virtual bool isShapeSensitivity(void) {return false;}
    // virtual double getdLdh(void) {return 0.0;}
    // virtual double getd1overLdh(void) {return 0.0;}
    // AddingSensitivity:END //////////////////////////////////

    virtual const Vector &getGlobalResistingForce(const Vector &basicForce, const Vector &uniformLoad) = 0;
    virtual const Matrix &getGlobalStiffMatrix(const Matrix &basicStiff, const Vector &basicForce) = 0;
    virtual const Matrix &getInitialGlobalStiffMatrix(const Matrix &basicStiff) = 0;

    // method used to rotate consistent mass matrix
    virtual const Matrix &getGlobalMatrixFromLocal(const Matrix &local) = 0;

    // methods used in post-processing only
    virtual const Vector &getPointGlobalCoordFromLocal(const Vector &localCoords) = 0;
    virtual const Vector &getPointGlobalDisplFromBasic(double xi, const Vector &basicDisps) = 0;

protected:

private:
};

// some additional methods related to prototypes created for copy constructors
extern bool       OPS_AddCrdTransf(CrdTransf *newComponent);
extern CrdTransf *OPS_GetCrdTransf(int tag);
extern void       OPS_ClearAllCrdTransf(void);

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
