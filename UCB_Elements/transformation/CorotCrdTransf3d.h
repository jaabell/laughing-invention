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

// $Revision: 1.9 $
// $Date: 2006-01-13 01:07:48 $
// $Source: /usr/local/cvs/OpenSees/SRC/coordTransformation/CorotCrdTransf3d.h,v $


// Written: Remo Magalhaes de Souza (rmsouza@ce.berkeley.edu)
// Created: 04/2000
// Revision: A
//
// Description: This file contains the class definition for
// CorotCrdTransf3d.h. CorotCrdTransf3d provides the
// abstraction of a corotation transformation for a spatial frame element

// What: "@(#) CorotCrdTransf3d.h, revA"

#ifndef CorotCrdTransf3d_h
#define CorotCrdTransf3d_h

#include <CrdTransf.h>
#include <Vector.h>
#include <Matrix.h>

class CorotCrdTransf3d: public CrdTransf
{
public:
    CorotCrdTransf3d(int tag, const Vector &vecInLocXZPlane,
                     const Vector &rigJntOffsetI, const Vector &rigJntOffsetJ);

    CorotCrdTransf3d();
    ~CorotCrdTransf3d();

    const char *getClassType() const
    {
        return "CorotCrdTransf3d";
    };

    int initialize(Node *nodeIPointer, Node *nodeJPointer);
    int update(void);
    double getInitialLength(void);
    double getDeformedLength(void);

    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);

    const Vector &getBasicTrialDisp(void);
    const Vector &getBasicIncrDisp(void);
    const Vector &getBasicIncrDeltaDisp(void);
    const Vector &getBasicTrialVel(void);
    const Vector &getBasicTrialAccel(void);

    const Vector &getGlobalResistingForce(const Vector &basicForce, const Vector &p0);
    const Matrix &getGlobalStiffMatrix(const Matrix &basicStiff, const Vector &basicForce);
    const Matrix &getInitialGlobalStiffMatrix(const Matrix &basicStiff);

    CrdTransf *getCopy3d(void);

    int sendSelf(int cTag, Channel &theChannel);
    int receiveSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    void Print(ostream &s, int flag = 0);

    // method used to rotate consistent mass matrix
    const Matrix &getGlobalMatrixFromLocal(const Matrix &local);

    // methods used in post-processing only
    const Vector &getPointGlobalCoordFromLocal(const Vector &localCoords);
    const Vector &getPointGlobalDisplFromBasic(double xi, const Vector &basicDisps);

    int  getLocalAxes(Vector &xAxis, Vector &yAxis, Vector &zAxis);

private:
    void compTransfMatrixBasicGlobal(void);
    void compTransfMatrixBasicGlobalNew(void);
    void compTransfMatrixLocalGlobal(Matrix &Tlg);
    const Vector &getQuaternionFromRotMatrix(const Matrix &RotMatrix) const;
    const Vector &getQuaternionFromPseudoRotVector(const Vector &theta) const;
    const Vector &getTangScaledPseudoVectorFromQuaternion(const Vector &theta) const;
    const Vector &quaternionProduct(const Vector &q1, const Vector &q2) const;
    const Matrix &getRotationMatrixFromQuaternion(const Vector &q) const;
    const Matrix &getRotMatrixFromTangScaledPseudoVector(const Vector &w) const;
    const Matrix &getSkewSymMatrix(const Vector &theta) const;
    const Matrix &getLMatrix(const Vector &ri) const;
    const Matrix &getKs2Matrix(const Vector &ri, const Vector &z) const;

    // internal data
    Vector vAxis;               // Vector that lies in local plane xz
    Vector nodeIOffset, nodeJOffset;    // rigid joint offsets

    Vector xAxis;               // local x axis
    Node *nodeIPtr, *nodeJPtr;  // pointers to the element two endnodes
    Matrix R0;                  // rotation matrix from local to global coordinates

    double L;                   // undeformed element length
    double Ln;                  // deformed element length

    // (the columns of which are the element local axes)
    Vector alphaIq;             // quaternion for node I
    Vector alphaJq;             // quaternion for node I

    Vector alphaIqcommit;       // commited quaternion for node I
    Vector alphaJqcommit;       // commited quaternion for node J
    Vector alphaI;              // last trial rotations end i
    Vector alphaJ;              // last trial rotatations end j

    Vector ulcommit;            // commited local displacements
    Vector ul;                  // local displacements
    Vector ulpr;                // previous local displacements

    static Matrix RI;           // nodal triad for node 1
    static Matrix RJ;           // nodal triad for node 2
    static Matrix Rbar;         // mean nodal triad
    static Matrix e;            // base vectors
    static Matrix Tp;           // transformation matrix to renumber dofs
    static Matrix T;            // transformation matrix from basic to global system
    static Matrix Tlg;          // transformation matrix from global to local system
    static Matrix kg;           // global stiffness matrix
    static Matrix Lr2, Lr3, A;  // auxiliary matrices

    double *nodeIInitialDisp, *nodeJInitialDisp;
    bool initialDispChecked;
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
