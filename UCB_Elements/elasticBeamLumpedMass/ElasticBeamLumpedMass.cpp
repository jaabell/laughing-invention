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

//============================================================
// Nima Tafazzoli (Nov. 2012)
// Moving linear transformation functions inside the
// ElasticBeamLumpedMass class
//============================================================


// $Revision: 1.15 $
// $Date: 2004/10/30 00:05:32 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/elasticBeamColumn/ElasticBeamLumpedMass.cpp,v $


// File: ~/model/ElasticBeamLumpedMass.C
//
// Written: fmk 11/95
// Revised:
//
// Purpose: This file contains the class definition for ElasticBeamLumpedMass.
// ElasticBeamLumpedMass is a 3d beam element. As such it can only
// connect to a node with 6-dof.

#include <ElasticBeamLumpedMass.h>
#include <Domain.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>

#include <Information.h>
// #include <ElementResponse.h>
#include <ElementalLoad.h>
// #include <Renderer.h>
#include <SectionForceDeformation.h>
#include <ID.h>
#include <math.h>
#include <stdlib.h>

Matrix ElasticBeamLumpedMass::K(12, 12);
Vector ElasticBeamLumpedMass::P(12);
Matrix ElasticBeamLumpedMass::kb(6, 6);

ElasticBeamLumpedMass::ElasticBeamLumpedMass()
    : Element(0, ELE_TAG_ElasticBeamLumpedMass),
      A(0), E(0), G(0), Jx(0), Iy(0), Iz(0), rho(0.0), R(3, 3),
      Q(12), q(6), connectedExternalNodes(2), nodeIOffset(0), nodeJOffset(0), L(0),
      nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false)
{
    // does nothing
    q0[0] = 0.0;
    q0[1] = 0.0;
    q0[2] = 0.0;
    q0[3] = 0.0;
    q0[4] = 0.0;

    p0[0] = 0.0;
    p0[1] = 0.0;
    p0[2] = 0.0;
    p0[3] = 0.0;
    p0[4] = 0.0;

    // set node pointers to NULL
    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }

    R.Zero();


}

ElasticBeamLumpedMass::ElasticBeamLumpedMass(int tag, double a, double e, double g,
        double jx, double iy, double iz, int Nd1, int Nd2,
        double r, int sectTag,
        double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
        double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
        double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z)
    : Element(tag, ELE_TAG_ElasticBeamLumpedMass),
      A(a), E(e), G(g), Jx(jx), Iy(iy), Iz(iz), rho(r), sectionTag(sectTag), nodeIOffset(0), nodeJOffset(0),
      R(3, 3),
      Q(12), q(6), connectedExternalNodes(2), L(0), nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false)
{
    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;


    q0[0] = 0.0;
    q0[1] = 0.0;
    q0[2] = 0.0;
    q0[3] = 0.0;
    q0[4] = 0.0;

    p0[0] = 0.0;
    p0[1] = 0.0;
    p0[2] = 0.0;
    p0[3] = 0.0;
    p0[4] = 0.0;

    // set node pointers to NULL
    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }


    R(2, 0) = vecInLocXZPlane_x;
    R(2, 1) = vecInLocXZPlane_y;
    R(2, 2) = vecInLocXZPlane_z;



    double normI = sqrt(rigJntOffset1_x * rigJntOffset1_x + rigJntOffset1_y * rigJntOffset1_y + rigJntOffset1_z * rigJntOffset1_z);

    if (normI > 0)
    {
        nodeIOffset = new Vector(3);
        (*nodeIOffset)(0) = rigJntOffset1_x;
        (*nodeIOffset)(1) = rigJntOffset1_y;
        (*nodeIOffset)(2) = rigJntOffset1_z;
    }


    double normJ = sqrt(rigJntOffset2_x * rigJntOffset2_x + rigJntOffset2_y * rigJntOffset2_y + rigJntOffset2_z * rigJntOffset2_z);

    if (normJ > 0)
    {
        nodeJOffset = new Vector(3);
        (*nodeJOffset)(0) = rigJntOffset2_x;
        (*nodeJOffset)(1) = rigJntOffset2_y;
        (*nodeJOffset)(2) = rigJntOffset2_z;
    }


}


ElasticBeamLumpedMass::ElasticBeamLumpedMass(int tag, int Nd1, int Nd2, SectionForceDeformation *section,
        double r,
        double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
        double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
        double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z)
    : Element(tag, ELE_TAG_ElasticBeamLumpedMass),
      R(3, 3),
      Q(12), q(6), connectedExternalNodes(2), L(0), nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false)
{
    if (section != 0)
    {
        sectionTag = section->getTag();
        E = 1.0;
        G = 1.0;
        Jx = 0.0;
        rho = r;

        const Matrix &sectTangent = section->getSectionTangent();
        const ID &sectCode = section->getType();

        for (int i = 0; i < sectCode.Size(); i++)
        {
            int code = sectCode(i);

            switch (code)
            {
                case SECTION_RESPONSE_P:
                    A = sectTangent(i, i);
                    break;

                case SECTION_RESPONSE_MZ:
                    Iz = sectTangent(i, i);
                    break;

                case SECTION_RESPONSE_MY:
                    Iy = sectTangent(i, i);
                    break;

                case SECTION_RESPONSE_T:
                    Jx = sectTangent(i, i);
                    break;

                default:
                    break;
            }
        }
    }

    if (Jx == 0.0)
    {
        std::cerr << "ElasticBeamLumpedMass::ElasticBeamLumpedMass -- no torsion in section -- setting GJ = 1.0e10\n";
        Jx = 1.0e10;
    }

    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;


    q0[0] = 0.0;
    q0[1] = 0.0;
    q0[2] = 0.0;
    q0[3] = 0.0;
    q0[4] = 0.0;

    p0[0] = 0.0;
    p0[1] = 0.0;
    p0[2] = 0.0;
    p0[3] = 0.0;
    p0[4] = 0.0;

    // set node pointers to NULL
    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }


    R(2, 0) = vecInLocXZPlane_x;
    R(2, 1) = vecInLocXZPlane_y;
    R(2, 2) = vecInLocXZPlane_z;


    double normI = sqrt(rigJntOffset1_x * rigJntOffset1_x + rigJntOffset1_y * rigJntOffset1_y + rigJntOffset1_z * rigJntOffset1_z);

    if (normI > 0)
    {
        nodeIOffset = new Vector(3);
        (*nodeIOffset)(0) = rigJntOffset1_x;
        (*nodeIOffset)(1) = rigJntOffset1_y;
        (*nodeIOffset)(2) = rigJntOffset1_z;
    }


    double normJ = sqrt(rigJntOffset2_x * rigJntOffset2_x + rigJntOffset2_y * rigJntOffset2_y + rigJntOffset2_z * rigJntOffset2_z);

    if (normJ > 0)
    {
        nodeJOffset = new Vector(3);
        (*nodeJOffset)(0) = rigJntOffset2_x;
        (*nodeJOffset)(1) = rigJntOffset2_y;
        (*nodeJOffset)(2) = rigJntOffset2_z;
    }



}

ElasticBeamLumpedMass::~ElasticBeamLumpedMass()
{

}

int
ElasticBeamLumpedMass::getNumExternalNodes(void) const
{
    return 2;
}

const ID &
ElasticBeamLumpedMass::getExternalNodes(void)
{
    return connectedExternalNodes;
}

Node **
ElasticBeamLumpedMass::getNodePtrs(void)
{
    return theNodes;
}

int
ElasticBeamLumpedMass::getNumDOF(void)
{
    return 12;
}

void
ElasticBeamLumpedMass::setDomain(Domain *theDomain)
{
    if (theDomain == 0)
    {
        std::cerr << "ElasticBeamLumpedMass::setDomain -- Domain is null\n";
        exit(-1);
    }

    theNodes[0] = theDomain->getNode(connectedExternalNodes(0));
    theNodes[1] = theDomain->getNode(connectedExternalNodes(1));


    if (theNodes[0] == 0)
    {
        std::cerr << "ElasticBeamLumpedMass::setDomain -- Node 1: " << connectedExternalNodes(0) << " does not exist\n";
        exit(-1);
    }

    if (theNodes[1] == 0)
    {
        std::cerr << "ElasticBeamLumpedMass::setDomain -- Node 2: " << connectedExternalNodes(1) << " does not exist\n";
        exit(-1);
    }

    int dofNd1 = theNodes[0]->getNumberDOF();
    int dofNd2 = theNodes[1]->getNumberDOF();

    if (dofNd1 != 6)
    {
        std::cerr << "ElasticBeamLumpedMass::setDomain -- Node 1: " << connectedExternalNodes(0)
                  << " has incorrect number of DOF\n";
        exit(-1);
    }

    if (dofNd2 != 6)
    {
        std::cerr << "ElasticBeamLumpedMass::setDomain -- Node 2: " << connectedExternalNodes(1)
                  << " has incorrect number of DOF\n";
        exit(-1);
    }

    this->DomainComponent::setDomain(theDomain);

    if (this->initialize() != 0)
    {
        std::cerr << "ElasticBeamLumpedMass::setDomain -- Error initializing coordinate transformation\n";
        exit(-1);
    }

}

int
ElasticBeamLumpedMass::commitState()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        std::cerr << "ElasticBeamLumpedMass::commitState () - failed in base class";
    }

    return retVal;
}

int
ElasticBeamLumpedMass::revertToLastCommit()
{
    return 0;
}

int
ElasticBeamLumpedMass::revertToStart()
{
    return 0;
}

int
ElasticBeamLumpedMass::update(void)
{
    return 0;
}

const Matrix &
ElasticBeamLumpedMass::getTangentStiff(void)
{
    const Vector &v = this->getBasicTrialDisp();

    double oneOverL = 1.0 / L;
    double EoverL   = E * oneOverL;
    double EAoverL  = A * EoverL;         // EA/L
    double EIzoverL2 = 2.0 * Iz * EoverL; // 2EIz/L
    double EIzoverL4 = 2.0 * EIzoverL2;   // 4EIz/L
    double EIyoverL2 = 2.0 * Iy * EoverL; // 2EIy/L
    double EIyoverL4 = 2.0 * EIyoverL2;   // 4EIy/L
    double GJoverL = G * Jx * oneOverL;     // GJ/L

    kb(0, 0) = EAoverL;
    kb(1, 1) = kb(2, 2) = EIzoverL4;
    kb(2, 1) = kb(1, 2) = EIzoverL2;
    kb(3, 3) = kb(4, 4) = EIyoverL4;
    kb(4, 3) = kb(3, 4) = EIyoverL2;
    kb(5, 5) = GJoverL;

    return this->getGlobalStiffMatrix(kb);
}


const Matrix &
ElasticBeamLumpedMass::getInitialStiff(void)
{

    double oneOverL = 1.0 / L;
    double EoverL   = E * oneOverL;
    double EAoverL  = A * EoverL;         // EA/L
    double EIzoverL2 = 2.0 * Iz * EoverL; // 2EIz/L
    double EIzoverL4 = 2.0 * EIzoverL2;   // 4EIz/L
    double EIyoverL2 = 2.0 * Iy * EoverL; // 2EIy/L
    double EIyoverL4 = 2.0 * EIyoverL2;   // 4EIy/L
    double GJoverL = G * Jx * oneOverL;     // GJ/L

    kb(0, 0) = EAoverL;
    kb(1, 1) = kb(2, 2) = EIzoverL4;
    kb(2, 1) = kb(1, 2) = EIzoverL2;
    kb(3, 3) = kb(4, 4) = EIyoverL4;
    kb(4, 3) = kb(3, 4) = EIyoverL2;
    kb(5, 5) = GJoverL;

    return this->getInitialGlobalStiffMatrix(kb);
}

const Matrix &
ElasticBeamLumpedMass::getMass(void)
{
    K.Zero();

    if (rho == 0.0)
    {
        return K;
    }


    //=====================================================
    // LUMPED MASS MATRIX
    // Nima Tafazzoli (Nov. 2012)
    // changing density from per length to per volume

    double m = 0.5 * rho * L * A;

    K(0, 0) = m;
    K(1, 1) = m;
    K(2, 2) = m;

    K(6, 6) = m;
    K(7, 7) = m;
    K(8, 8) = m;

    return K;

}

void
ElasticBeamLumpedMass::zeroLoad(void)
{
    Q.Zero();

    q0[0] = 0.0;
    q0[1] = 0.0;
    q0[2] = 0.0;
    q0[3] = 0.0;
    q0[4] = 0.0;

    p0[0] = 0.0;
    p0[1] = 0.0;
    p0[2] = 0.0;
    p0[3] = 0.0;
    p0[4] = 0.0;

    return;
}

int
ElasticBeamLumpedMass::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    int type;
    const Vector &data = theLoad->getData(type, loadFactor);

    if (type == LOAD_TAG_Beam3dUniformLoad)
    {
        double wy = data(0) * loadFactor; // Transverse
        double wz = data(1) * loadFactor; // Transverse
        double wx = data(2) * loadFactor; // Axial (+ve from node I to J)

        double Vy = 0.5 * wy * L;
        double Mz = Vy * L / 6.0; // wy*L*L/12
        double Vz = 0.5 * wz * L;
        double My = Vz * L / 6.0; // wz*L*L/12
        double P = wx * L;

        // Reactions in basic system
        p0[0] -= P;
        p0[1] -= Vy;
        p0[2] -= Vy;
        p0[3] -= Vz;
        p0[4] -= Vz;

        // Fixed end forces in basic system
        q0[0] -= 0.5 * P;
        q0[1] -= Mz;
        q0[2] += Mz;
        q0[3] += My;
        q0[4] -= My;
    }
    else if (type == LOAD_TAG_Beam3dPointLoad)
    {
        double Py = data(0) * loadFactor;
        double Pz = data(1) * loadFactor;
        double N  = data(2) * loadFactor;
        double aOverL = data(3);

        if (aOverL < 0.0 || aOverL > 1.0)
        {
            return 0;
        }

        double a = aOverL * L;
        double b = L - a;

        // Reactions in basic system
        p0[0] -= N;
        double V1, V2;
        V1 = Py * (1.0 - aOverL);
        V2 = Py * aOverL;
        p0[1] -= V1;
        p0[2] -= V2;
        V1 = Pz * (1.0 - aOverL);
        V2 = Pz * aOverL;
        p0[3] -= V1;
        p0[4] -= V2;

        double L2 = 1.0 / (L * L);
        double a2 = a * a;
        double b2 = b * b;

        // Fixed end forces in basic system
        q0[0] -= N * aOverL;
        double M1, M2;
        M1 = -a * b2 * Py * L2;
        M2 = a2 * b * Py * L2;
        q0[1] += M1;
        q0[2] += M2;
        M1 = -a * b2 * Pz * L2;
        M2 = a2 * b * Pz * L2;
        q0[3] -= M1;
        q0[4] -= M2;
    }
    else
    {
        std::cerr << "ElasticBeamLumpedMass::addLoad()  -- load type unknown for element with tag: " << this->getTag() << endln;
        return -1;
    }

    return 0;
}


int
ElasticBeamLumpedMass::addInertiaLoadToUnbalance(const Vector &accel)
{
    if (rho == 0.0)
    {
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV(accel);
    const Vector &Raccel2 = theNodes[1]->getRV(accel);

    if (6 != Raccel1.Size() || 6 != Raccel2.Size())
    {
        std::cerr << "ElasticBeamLumpedMass::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }

    // Want to add ( - fact * M R * accel ) to unbalance
    // Take advantage of lumped mass matrix

    // Nima Tafazzoli (Nov. 2012)
    // changing density from per length to per volume
    double m = 0.5 * rho * L * A;

    Q(0) -= m * Raccel1(0);
    Q(1) -= m * Raccel1(1);
    Q(2) -= m * Raccel1(2);

    Q(6) -= m * Raccel2(0);
    Q(7) -= m * Raccel2(1);
    Q(8) -= m * Raccel2(2);

    return 0;
}



const Vector &
ElasticBeamLumpedMass::getResistingForceIncInertia()
{
    P = this->getResistingForce();

    // add the damping forces if rayleigh damping
    if (a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
    {
        P += this->getRayleighDampingForces();
    }

    if (rho == 0.0)
    {
        return P;
    }

    else
    {
        const Vector &accel1 = theNodes[0]->getTrialAccel();
        const Vector &accel2 = theNodes[1]->getTrialAccel();


        // Nima Tafazzoli (Nov. 2012)
        // changing density from per length to per volume
        double m = 0.5 * rho * L * A;

        P(0) += m * accel1(0);
        P(1) += m * accel1(1);
        P(2) += m * accel1(2);

        P(6) += m * accel2(0);
        P(7) += m * accel2(1);
        P(8) += m * accel2(2);

        return P;
    }
}


const Vector &
ElasticBeamLumpedMass::getResistingForce()
{
    const Vector &v = this->getBasicTrialDisp();

    double oneOverL = 1.0 / L;
    double EoverL   = E * oneOverL;
    double EAoverL  = A * EoverL;         // EA/L
    double EIzoverL2 = 2.0 * Iz * EoverL; // 2EIz/L
    double EIzoverL4 = 2.0 * EIzoverL2;   // 4EIz/L
    double EIyoverL2 = 2.0 * Iy * EoverL; // 2EIy/L
    double EIyoverL4 = 2.0 * EIyoverL2;   // 4EIy/L
    double GJoverL = G * Jx * oneOverL;     // GJ/L

    q(0) = EAoverL * v(0);
    q(1) = EIzoverL4 * v(1) + EIzoverL2 * v(2);
    q(2) = EIzoverL2 * v(1) + EIzoverL4 * v(2);
    q(3) = EIyoverL4 * v(3) + EIyoverL2 * v(4);
    q(4) = EIyoverL2 * v(3) + EIyoverL4 * v(4);
    q(5) = GJoverL * v(5);

    q(0) += q0[0];
    q(1) += q0[1];
    q(2) += q0[2];
    q(3) += q0[3];
    q(4) += q0[4];

    Vector p0Vec(p0, 5);

    //  std::cerr << q;

    P = this->getGlobalResistingForce(q, p0Vec);

    // std::cerr << P;

    // P = P - Q;
    P.addVector(1.0, Q, -1.0);

    return P;
}

int
ElasticBeamLumpedMass::sendSelf(int cTag, Channel &theChannel)
{
    int res = 0;

    static Vector data(12);

    data(0) = A;
    data(1) = E;
    data(2) = G;
    data(3) = Jx;
    data(4) = Iy;
    data(5) = Iz;
    data(6) = rho;
    data(7) = this->getTag();
    data(8) = connectedExternalNodes(0);
    data(9) = connectedExternalNodes(1);
    //     data(10) = theCoordTransf->getClassTag();

    //  int dbTag = theCoordTransf->getDbTag();
    //
    //  if (dbTag == 0) {
    //      dbTag = theChannel.getDbTag();
    //      if (dbTag != 0)
    //          theCoordTransf->setDbTag(dbTag);
    //  }
    //
    //  data(11) = dbTag;

    // Send the data vector
    res += theChannel.sendVector(this->getDbTag(), cTag, data);

    if (res < 0)
    {
        std::cerr << "ElasticBeamLumpedMass::sendSelf -- could not send data Vector\n";
        return res;
    }

    // Ask the CoordTransf to send itself
    //     res += theCoordTransf->sendSelf(cTag, theChannel);
    //     if (res < 0) {
    //       std::cerr << "ElasticBeamLumpedMass::sendSelf -- could not send CoordTransf\n";
    //       return res;
    //     }

    return res;
}

int
ElasticBeamLumpedMass::recvSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    int res = 0;

    static Vector data(12);

    res += theChannel.recvVector(this->getDbTag(), cTag, data);

    if (res < 0)
    {
        std::cerr << "ElasticBeamLumpedMass::recvSelf -- could not receive data Vector\n";
        return res;
    }

    A = data(0);
    E = data(1);
    G = data(2);
    Jx = data(3);
    Iy = data(4);
    Iz = data(5);
    rho = data(6);
    this->setTag((int)data(7));
    connectedExternalNodes(0) = (int)data(8);
    connectedExternalNodes(1) = (int)data(9);

    // Check if the CoordTransf is null; if so, get a new one
    //     int crdTag = (int)data(10);
    //     if (theCoordTransf == 0) {
    //       theCoordTransf = theBroker.getNewCrdTransf3d(crdTag);
    //       if (theCoordTransf == 0) {
    //  std::cerr << "ElasticBeamLumpedMass::recvSelf -- could not get a CrdTransf\n";
    //  exit(-1);
    //       }
    //     }

    // Check that the CoordTransf is of the right type; if not, delete
    // the current one and get a new one of the right type
    //     if (theCoordTransf->getClassTag() != crdTag) {
    //       delete theCoordTransf;
    //       theCoordTransf = theBroker.getNewCrdTransf3d(crdTag);
    //       if (theCoordTransf == 0) {
    //  std::cerr << "ElasticBeamLumpedMass::recvSelf -- could not get a CrdTransf\n";
    //  exit(-1);
    //       }
    //     }

    // Now, receive the CoordTransf
    //     theCoordTransf->setDbTag((int)data(11));
    //     res += theCoordTransf->recvSelf(cTag, theChannel, theBroker);
    //     if (res < 0) {
    //       std::cerr << "ElasticBeamLumpedMass::recvSelf -- could not receive CoordTransf\n";
    //       return res;
    //     }

    // Revert the crdtrasf to its last committed state
    //     theCoordTransf->revertToLastCommit();

    return res;
}

void
ElasticBeamLumpedMass::Print(ostream &s, int flag)
{
    if (flag == -1)
    {
        int eleTag = this->getTag();
        s << "EL_BEAM\t" << eleTag << "\t";
        s << sectionTag << "\t" << sectionTag;
        s  << "\t" << connectedExternalNodes(0) << "\t" << connectedExternalNodes(1);
        s << "\t0\t0.0000000\n";
    }
    else if (flag < -1)
    {
        int counter = (flag + 1) * -1;
        int eleTag = this->getTag();
        const Vector &force = this->getResistingForce();

        double P, MZ1, MZ2, VY, MY1, MY2, VZ, T;

        double oneOverL = 1.0 / L;

        P   = q(0);
        MZ1 = q(1);
        MZ2 = q(2);
        VY  = (MZ1 + MZ2) * oneOverL;
        MY1 = q(3);
        MY2 = q(4);
        VZ  = (MY1 + MY2) * oneOverL;
        T   = q(5);

        s << "FORCE\t" << eleTag << "\t" << counter << "\t0";
        s << "\t" << -P + p0[0] << "\t"  <<  VY + p0[1] << "\t"  << -VZ + p0[3]  << endln;
        s << "FORCE\t" << eleTag << "\t" << counter << "\t1";
        s << "\t"  << P  << ' '  << -VY + p0[2] << ' ' << VZ + p0[4] << endln;
        s << "MOMENT\t" << eleTag << "\t" << counter << "\t0";
        s << "\t" << -T << "\t"  << MY1 << "\t" << MZ1 << endln;
        s << "MOMENT\t" << eleTag << "\t" << counter << "\t1";
        s << "\t" << T << ' ' << MY2 << ' '  <<  MZ2 << endln;

    }

    else if (flag == 2)
    {
        this->getResistingForce(); // in case linear algo

        static Vector xAxis(3);
        static Vector yAxis(3);
        static Vector zAxis(3);

        this->getLocalAxes(xAxis, yAxis, zAxis);

        s << "#ElasticBeamColumn3D\n";
        s << "#LocalAxis " << xAxis(0) << " " << xAxis(1) << " " << xAxis(2);
        s << " " << yAxis(0) << " " << yAxis(1) << " " << yAxis(2) << " ";
        s << zAxis(0) << " " << zAxis(1) << " " << zAxis(2) << endln;

        const Vector &node1Crd = theNodes[0]->getCrds();
        const Vector &node2Crd = theNodes[1]->getCrds();
        const Vector &node1Disp = theNodes[0]->getDisp();
        const Vector &node2Disp = theNodes[1]->getDisp();

        s << "#NODE " << node1Crd(0) << " " << node1Crd(1) << " " << node1Crd(2)
          << " " << node1Disp(0) << " " << node1Disp(1) << " " << node1Disp(2)
          << " " << node1Disp(3) << " " << node1Disp(4) << " " << node1Disp(5) << endln;

        s << "#NODE " << node2Crd(0) << " " << node2Crd(1) << " " << node2Crd(2)
          << " " << node2Disp(0) << " " << node2Disp(1) << " " << node2Disp(2)
          << " " << node2Disp(3) << " " << node2Disp(4) << " " << node2Disp(5) << endln;

        double N, Mz1, Mz2, Vy, My1, My2, Vz, T;

        double oneOverL = 1.0 / L;

        N   = q(0);
        Mz1 = q(1);
        Mz2 = q(2);
        Vy  = (Mz1 + Mz2) * oneOverL;
        My1 = q(3);
        My2 = q(4);
        Vz  = -(My1 + My2) * oneOverL;
        T   = q(5);

        s << "#END_FORCES " << -N + p0[0] << ' ' <<  Vy + p0[1] << ' ' << Vz + p0[3] << ' '
          << -T << ' ' << My1 << ' ' <<  Mz1 << endln;
        s << "#END_FORCES " <<  N << ' ' << -Vy + p0[2] << ' ' << -Vz + p0[4] << ' '
          << T << ' ' << My2 << ' ' << Mz2 << endln;
    }
    else
    {

        this->getResistingForce(); // in case linear algo

        s << "\nElasticBeamLumpedMass: " << this->getTag() << endln;
        s << "\tConnected Nodes: " << connectedExternalNodes ;

        double N, Mz1, Mz2, Vy, My1, My2, Vz, T;

        double oneOverL = 1.0 / L;

        N   = q(0);
        Mz1 = q(1);
        Mz2 = q(2);
        Vy  = (Mz1 + Mz2) * oneOverL;
        My1 = q(3);
        My2 = q(4);
        Vz  = -(My1 + My2) * oneOverL;
        T   = q(5);

        s << "\tEnd 1 Forces (P Mz Vy My Vz T): "
          << -N + p0[0] << ' ' << Mz1 << ' ' <<  Vy + p0[1] << ' ' << My1 << ' ' <<  Vz + p0[3] << ' ' << -T << endln;
        s << "\tEnd 2 Forces (P Mz Vy My Vz T): "
          <<  N << ' ' << Mz2 << ' ' << -Vy + p0[2] << ' ' << My2 << ' ' << -Vz + p0[4] << ' ' <<  T << endln;
    }
}

// int
// ElasticBeamLumpedMass::displaySelf(Renderer &theViewer, int displayMode, float fact)
// {
//     // first determine the end points of the quad based on
//     // the display factor (a measure of the distorted image)
//     const Vector &end1Crd = theNodes[0]->getCrds();
//     const Vector &end2Crd = theNodes[1]->getCrds();
//
//     const Vector &end1Disp = theNodes[0]->getDisp();
//     const Vector &end2Disp = theNodes[1]->getDisp();
//
//     static Vector v1(3);
//     static Vector v2(3);
//
//     for (int i = 0; i < 3; i++) {
//       v1(i) = end1Crd(i) + end1Disp(i)*fact;
//       v2(i) = end2Crd(i) + end2Disp(i)*fact;
//     }
//
//     return theViewer.drawLine (v1, v2, 1.0, 1.0);
// }

// Response*
// ElasticBeamLumpedMass::setResponse(const char** argv, int argc, Information& info)
// {
//     // stiffness
//     if (strcmp(argv[0], "stiffness") == 0)
//     {
//         return new ElementResponse(this, 1, K);
//     }

//     // global forces
//     else if (strcmp(argv[0], "force") == 0 || strcmp(argv[0], "forces") == 0 ||
//              strcmp(argv[0], "globalForce") == 0 || strcmp(argv[0], "globalForces") == 0)
//     {
//         return new ElementResponse(this, 2, P);
//     }

//     // local forces
//     else if (strcmp(argv[0], "localForce") == 0 || strcmp(argv[0], "localForces") == 0)
//     {
//         return new ElementResponse(this, 3, P);
//     }

//     else
//     {
//         return 0;
//     }
// }

// int
// ElasticBeamLumpedMass::getResponse (int responseID, Information& eleInfo)
// {
//     double N, V, M1, M2, T;
//     double oneOverL = 1.0 / L;

//     switch (responseID)
//     {
//         case 1: // stiffness
//             return eleInfo.setMatrix(this->getTangentStiff());

//         case 2: // global forces
//             return eleInfo.setVector(this->getResistingForce());

//         case 3: // local forces
//             // Axial
//             N = q(0);
//             P(6) =  N;
//             P(0) = -N + p0[0];

//             // Torsion
//             T = q(5);
//             P(9) =  T;
//             P(3) = -T;

//             // Moments about z and shears along y
//             M1 = q(1);
//             M2 = q(2);
//             P(5)  = M1;
//             P(11) = M2;
//             V = (M1 + M2) * oneOverL;
//             P(1) =  V + p0[1];
//             P(7) = -V + p0[2];

//             // Moments about y and shears along z
//             M1 = q(3);
//             M2 = q(4);
//             P(4)  = M1;
//             P(10) = M2;
//             V = -(M1 + M2) * oneOverL;
//             P(2) = -V + p0[3];
//             P(8) =  V + p0[4];

//             return eleInfo.setVector(P);

//         default:
//             return -1;
//     }
// }



//====================================================================
// Nima Tafazzoli (Nov. 2012) Moving from transformation ro element

int
ElasticBeamLumpedMass::initialize()
{
    int error;

    // see if there is some initial displacements at nodes
    if (initialDispChecked == false)
    {
        const Vector &nodeIDisp = theNodes[0]->getDisp();
        const Vector &nodeJDisp = theNodes[1]->getDisp();

        for (int i = 0; i < 6; i++)
            if (nodeIDisp(i) != 0.0)
            {
                nodeIInitialDisp = new Vector(6);

                for (int j = 0; j < 6; j++)
                {
                    (*nodeIInitialDisp)(j) = nodeIDisp(j);
                }

                i = 6;
            }

        for (int j = 0; j < 6; j++)
            if (nodeJDisp(j) != 0.0)
            {
                nodeJInitialDisp = new Vector(6);

                for (int i = 0; i < 6; i++)
                {
                    (*nodeJInitialDisp)(i) = nodeJDisp(i);
                }

                j = 6;
            }

        initialDispChecked = true;
    }

    // get element length and orientation
    if ((error = this->computeElemtLengthAndOrient()))
    {
        return error;
    }

    static Vector XAxis(3);
    static Vector YAxis(3);
    static Vector ZAxis(3);

    // get 3by3 rotation matrix
    if ((error = this->getLocalAxes(XAxis, YAxis, ZAxis)))
    {
        return error;
    }

    return 0;
}




int
ElasticBeamLumpedMass::computeElemtLengthAndOrient()
{
    // element projection
    static Vector dx(3);

    const Vector &ndICoords = theNodes[0]->getCrds();
    const Vector &ndJCoords = theNodes[1]->getCrds();

    dx(0) = ndJCoords(0) - ndICoords(0);
    dx(1) = ndJCoords(1) - ndICoords(1);
    dx(2) = ndJCoords(2) - ndICoords(2);

    if (nodeJOffset != 0)
    {
        dx(0) += (*nodeJOffset)(0);
        dx(1) += (*nodeJOffset)(1);
        dx(2) += (*nodeJOffset)(2);
    }


    if (nodeIOffset != 0)
    {
        dx(0) -= (*nodeIOffset)(0);
        dx(1) -= (*nodeIOffset)(1);
        dx(2) -= (*nodeIOffset)(2);
    }


    if (nodeIInitialDisp != 0)
    {
        dx(0) -= (*nodeIInitialDisp)(0);
        dx(1) -= (*nodeIInitialDisp)(1);
        dx(2) -= (*nodeIInitialDisp)(2);
    }


    if (nodeJInitialDisp != 0)
    {
        dx(0) += (*nodeJInitialDisp)(0);
        dx(1) += (*nodeJInitialDisp)(1);
        dx(2) += (*nodeJInitialDisp)(2);
    }


    // calculate the element length
    L = dx.Norm();

    if (L == 0.0)
    {
        std::cerr << "\ElasticBeamLumpedMass::computeElemtLengthAndOrien: 0 length\n";
        return -2;
    }

    // calculate the element local x axis components (direction cossines)
    // wrt to the global coordinates
    R(0, 0) = dx(0) / L;
    R(0, 1) = dx(1) / L;
    R(0, 2) = dx(2) / L;

    return 0;
}




int
ElasticBeamLumpedMass::getLocalAxes(Vector &XAxis, Vector &YAxis, Vector &ZAxis)
{
    // Compute y = v cross x
    // Note: v(i) is stored in R(2,i]
    static Vector vAxis(3);
    vAxis(0) = R(2, 0);
    vAxis(1) = R(2, 1);
    vAxis(2) = R(2, 2);

    static Vector xAxis(3);
    xAxis(0) = R(0, 0);
    xAxis(1) = R(0, 1);
    xAxis(2) = R(0, 2);
    XAxis(0) = xAxis(0);
    XAxis(1) = xAxis(1);
    XAxis(2) = xAxis(2);

    static Vector yAxis(3);
    yAxis(0) = vAxis(1) * xAxis(2) - vAxis(2) * xAxis(1);
    yAxis(1) = vAxis(2) * xAxis(0) - vAxis(0) * xAxis(2);
    yAxis(2) = vAxis(0) * xAxis(1) - vAxis(1) * xAxis(0);

    double ynorm = yAxis.Norm();

    if (ynorm == 0)
    {
        cerr.flush() << "\ElasticBeam::getLocalAxes";
        cerr.flush() << "\nvector v that defines plane xz is parallel to x axis\n";
        return -3;
    }

    yAxis /= ynorm;

    YAxis(0) = yAxis(0);
    YAxis(1) = yAxis(1);
    YAxis(2) = yAxis(2);

    // Compute z = x cross y
    static Vector zAxis(3);

    zAxis(0) = xAxis(1) * yAxis(2) - xAxis(2) * yAxis(1);
    zAxis(1) = xAxis(2) * yAxis(0) - xAxis(0) * yAxis(2);
    zAxis(2) = xAxis(0) * yAxis(1) - xAxis(1) * yAxis(0);
    ZAxis(0) = zAxis(0);
    ZAxis(1) = zAxis(1);
    ZAxis(2) = zAxis(2);

    // Fill in transformation matrix
    R(1, 0) = yAxis(0);
    R(1, 1) = yAxis(1);
    R(1, 2) = yAxis(2);

    R(2, 0) = zAxis(0);
    R(2, 1) = zAxis(1);
    R(2, 2) = zAxis(2);

    return 0;
}




const Vector &
ElasticBeamLumpedMass::getBasicTrialDisp (void)
{
    // determine global displacements
    const Vector &disp1 = theNodes[0]->getTrialDisp();
    const Vector &disp2 = theNodes[1]->getTrialDisp();

    static Vector ug(12);

    for (int i = 0; i < 6; i++)
    {
        ug(i)   = disp1(i);
        ug(i + 6) = disp2(i);
    }

    if (nodeIInitialDisp != 0)
    {
        for (int j = 0; j < 6; j++)
        {
            ug(j) -= (*nodeIInitialDisp)(j);
        }
    }

    if (nodeJInitialDisp != 0)
    {
        for (int j = 0; j < 6; j++)
        {
            ug[j + 6] -= (*nodeJInitialDisp)(j);
        }
    }

    double oneOverL = 1.0 / L;

    static Vector ub(6);

    static Vector ul(12);

    ul(0)  = R(0, 0) * ug(0) + R(0, 1) * ug(1) + R(0, 2) * ug(2);
    ul(1)  = R(1, 0) * ug(0) + R(1, 1) * ug(1) + R(1, 2) * ug(2);
    ul(2)  = R(2, 0) * ug(0) + R(2, 1) * ug(1) + R(2, 2) * ug(2);

    ul(3)  = R(0, 0) * ug(3) + R(0, 1) * ug(4) + R(0, 2) * ug(5);
    ul(4)  = R(1, 0) * ug(3) + R(1, 1) * ug(4) + R(1, 2) * ug(5);
    ul(5)  = R(2, 0) * ug(3) + R(2, 1) * ug(4) + R(2, 2) * ug(5);

    ul(6)  = R(0, 0) * ug(6) + R(0, 1) * ug(7) + R(0, 2) * ug(8);
    ul(7)  = R(1, 0) * ug(6) + R(1, 1) * ug(7) + R(1, 2) * ug(8);
    ul(8)  = R(2, 0) * ug(6) + R(2, 1) * ug(7) + R(2, 2) * ug(8);

    ul(9)  = R(0, 0) * ug(9) + R(0, 1) * ug(10) + R(0, 2) * ug(11);
    ul(10) = R(1, 0) * ug(9) + R(1, 1) * ug(10) + R(1, 2) * ug(11);
    ul(11) = R(2, 0) * ug(9) + R(2, 1) * ug(10) + R(2, 2) * ug(11);

    static Vector Wu(3);

    if (nodeIOffset)
    {
        Wu(0) =  (*nodeIOffset)(2) * ug(4) - (*nodeIOffset)(1) * ug(5);
        Wu(1) = -(*nodeIOffset)(2) * ug[3] + (*nodeIOffset)(0) * ug(5);
        Wu(2) =  (*nodeIOffset)(1) * ug[3] - (*nodeIOffset)(0) * ug(4);

        ul(0) += R(0, 0) * Wu(0) + R(0, 1) * Wu(1) + R(0, 2) * Wu(2);
        ul(1) += R(1, 0) * Wu(0) + R(1, 1) * Wu(1) + R(1, 2) * Wu(2);
        ul(2) += R(2, 0) * Wu(0) + R(2, 1) * Wu(1) + R(2, 2) * Wu(2);
    }

    if (nodeJOffset)
    {
        Wu(0) =  (*nodeJOffset)(2) * ug(10) - (*nodeJOffset)(1) * ug(11);
        Wu(1) = -(*nodeJOffset)(2) * ug(9)  + (*nodeJOffset)(0) * ug(11);
        Wu(2) =  (*nodeJOffset)(1) * ug(9)  - (*nodeJOffset)(0) * ug(10);

        ul(6) += R(0, 0) * Wu(0) + R(0, 1) * Wu(1) + R(0, 2) * Wu(2);
        ul(7) += R(1, 0) * Wu(0) + R(1, 1) * Wu(1) + R(1, 2) * Wu(2);
        ul(8) += R(2, 0) * Wu(0) + R(2, 1) * Wu(1) + R(2, 2) * Wu(2);
    }

    ub(0) = ul(6) - ul(0);
    double tmp;
    tmp = oneOverL * (ul(1) - ul(7));
    ub(1) = ul(5) + tmp;
    ub(2) = ul(11) + tmp;
    tmp = oneOverL * (ul(8) - ul(2));
    ub(3) = ul(4) + tmp;
    ub(4) = ul(10) + tmp;
    ub(5) = ul(9) - ul(3);

    return ub;
}




const Matrix &
ElasticBeamLumpedMass::getGlobalStiffMatrix(const Matrix &KB)
{
    static Matrix kg(12, 12);   // Global stiffness for return
    static Matrix kb(6, 6);     // Basic stiffness
    static Matrix kl(12, 12);   // Local stiffness
    static Matrix tmp(12, 12);  // Temporary storage

    double oneOverL = 1.0 / L;

    int i, j;

    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++)
        {
            kb(i, j) = KB(i, j);
        }

    // Transform basic stiffness to local system
    // First compute kb*T_{bl}
    for (i = 0; i < 6; i++)
    {
        tmp(i, 0)  = -kb(i, 0);
        tmp(i, 1)  =  oneOverL * (kb(i, 1) + kb(i, 2));
        tmp(i, 2)  = -oneOverL * (kb(i, 3) + kb(i, 4));
        tmp(i, 3)  = -kb(i, 5);
        tmp(i, 4)  =  kb(i, 3);
        tmp(i, 5)  =  kb(i, 1);
        tmp(i, 6)  =  kb(i, 0);
        tmp(i, 7)  = -tmp(i, 1);
        tmp(i, 8)  = -tmp(i, 2);
        tmp(i, 9)  =  kb(i, 5);
        tmp(i, 10) =  kb(i, 4);
        tmp(i, 11) =  kb(i, 2);
    }

    // Now compute T'_{bl}*(kb*T_{bl})
    for (i = 0; i < 12; i++)
    {
        kl(0, i)  = -tmp(0, i);
        kl(1, i)  =  oneOverL * (tmp(1, i) + tmp(2, i));
        kl(2, i)  = -oneOverL * (tmp(3, i) + tmp(4, i));
        kl(3, i)  = -tmp(5, i);
        kl(4, i)  =  tmp(3, i);
        kl(5, i)  =  tmp(1, i);
        kl(6, i)  =  tmp(0, i);
        kl(7, i)  = -kl(1, i);
        kl(8, i)  = -kl(2, i);
        kl(9, i)  =  tmp(5, i);
        kl(10, i) =  tmp(4, i);
        kl(11, i) =  tmp(2, i);
    }

    static Matrix RWI(3, 3);


    if (nodeIOffset)
    {
        // Compute RWI
        RWI(0, 0) = -R(0, 1) * (*nodeIOffset)(2) + R(0, 2) * (*nodeIOffset)(1);
        RWI(1, 0) = -R(1, 1) * (*nodeIOffset)(2) + R(1, 2) * (*nodeIOffset)(1);
        RWI(2, 0) = -R(2, 1) * (*nodeIOffset)(2) + R(2, 2) * (*nodeIOffset)(1);

        RWI(0, 1) =  R(0, 0) * (*nodeIOffset)(2) - R(0, 2) * (*nodeIOffset)(0);
        RWI(1, 1) =  R(1, 0) * (*nodeIOffset)(2) - R(1, 2) * (*nodeIOffset)(0);
        RWI(2, 1) =  R(2, 0) * (*nodeIOffset)(2) - R(2, 2) * (*nodeIOffset)(0);

        RWI(0, 2) = -R(0, 0) * (*nodeIOffset)(1) + R(0, 1) * (*nodeIOffset)(0);
        RWI(1, 2) = -R(1, 0) * (*nodeIOffset)(1) + R(1, 1) * (*nodeIOffset)(0);
        RWI(2, 2) = -R(2, 0) * (*nodeIOffset)(1) + R(2, 1) * (*nodeIOffset)(0);
    }


    static Matrix RWJ(3, 3);


    if (nodeJOffset)
    {
        // Compute RWJ
        RWJ(0, 0) = -R(0, 1) * (*nodeJOffset)(2) + R(0, 2) * (*nodeJOffset)(1);
        RWJ(1, 0) = -R(1, 1) * (*nodeJOffset)(2) + R(1, 2) * (*nodeJOffset)(1);
        RWJ(2, 0) = -R(2, 1) * (*nodeJOffset)(2) + R(2, 2) * (*nodeJOffset)(1);

        RWJ(0, 1) =  R(0, 0) * (*nodeJOffset)(2) - R(0, 2) * (*nodeJOffset)(0);
        RWJ(1, 1) =  R(1, 0) * (*nodeJOffset)(2) - R(1, 2) * (*nodeJOffset)(0);
        RWJ(2, 1) =  R(2, 0) * (*nodeJOffset)(2) - R(2, 2) * (*nodeJOffset)(0);

        RWJ(0, 2) = -R(0, 0) * (*nodeJOffset)(1) + R(0, 1) * (*nodeJOffset)(0);
        RWJ(1, 2) = -R(1, 0) * (*nodeJOffset)(1) + R(1, 1) * (*nodeJOffset)(0);
        RWJ(2, 2) = -R(2, 0) * (*nodeJOffset)(1) + R(2, 1) * (*nodeJOffset)(0);
    }



    // Transform local stiffness to global system
    // First compute kl*T_{lg}
    int m;

    for (m = 0; m < 12; m++)
    {
        tmp(m, 0) = kl(m, 0) * R(0, 0) + kl(m, 1) * R(1, 0)  + kl(m, 2) * R(2, 0);
        tmp(m, 1) = kl(m, 0) * R(0, 1) + kl(m, 1) * R(1, 1)  + kl(m, 2) * R(2, 1);
        tmp(m, 2) = kl(m, 0) * R(0, 2) + kl(m, 1) * R(1, 2)  + kl(m, 2) * R(2, 2);

        tmp(m, 3) = kl(m, 3) * R(0, 0) + kl(m, 4) * R(1, 0)  + kl(m, 5) * R(2, 0);
        tmp(m, 4) = kl(m, 3) * R(0, 1) + kl(m, 4) * R(1, 1)  + kl(m, 5) * R(2, 1);
        tmp(m, 5) = kl(m, 3) * R(0, 2) + kl(m, 4) * R(1, 2)  + kl(m, 5) * R(2, 2);


        if (nodeIOffset)
        {
            tmp(m, 3)  += kl(m, 0) * RWI(0, 0)  + kl(m, 1) * RWI(1, 0)  + kl(m, 2) * RWI(2, 0);
            tmp(m, 4)  += kl(m, 0) * RWI(0, 1)  + kl(m, 1) * RWI(1, 1)  + kl(m, 2) * RWI(2, 1);
            tmp(m, 5)  += kl(m, 0) * RWI(0, 2)  + kl(m, 1) * RWI(1, 2)  + kl(m, 2) * RWI(2, 2);
        }


        tmp(m, 6) = kl(m, 6) * R(0, 0) + kl(m, 7) * R(1, 0)  + kl(m, 8) * R(2, 0);
        tmp(m, 7) = kl(m, 6) * R(0, 1) + kl(m, 7) * R(1, 1)  + kl(m, 8) * R(2, 1);
        tmp(m, 8) = kl(m, 6) * R(0, 2) + kl(m, 7) * R(1, 2)  + kl(m, 8) * R(2, 2);

        tmp(m, 9)  = kl(m, 9) * R(0, 0) + kl(m, 10) * R(1, 0) + kl(m, 11) * R(2, 0);
        tmp(m, 10) = kl(m, 9) * R(0, 1) + kl(m, 10) * R(1, 1) + kl(m, 11) * R(2, 1);
        tmp(m, 11) = kl(m, 9) * R(0, 2) + kl(m, 10) * R(1, 2) + kl(m, 11) * R(2, 2);


        if (nodeJOffset)
        {
            tmp(m, 9)   += kl(m, 6) * RWJ(0, 0)  + kl(m, 7) * RWJ(1, 0)  + kl(m, 8) * RWJ(2, 0);
            tmp(m, 10)  += kl(m, 6) * RWJ(0, 1)  + kl(m, 7) * RWJ(1, 1)  + kl(m, 8) * RWJ(2, 1);
            tmp(m, 11)  += kl(m, 6) * RWJ(0, 2)  + kl(m, 7) * RWJ(1, 2)  + kl(m, 8) * RWJ(2, 2);
        }


    }

    // Now compute T'_{lg}*(kl*T_{lg})
    for (m = 0; m < 12; m++)
    {
        kg(0, m) = R(0, 0) * tmp(0, m) + R(1, 0) * tmp(1, m)  + R(2, 0) * tmp(2, m);
        kg(1, m) = R(0, 1) * tmp(0, m) + R(1, 1) * tmp(1, m)  + R(2, 1) * tmp(2, m);
        kg(2, m) = R(0, 2) * tmp(0, m) + R(1, 2) * tmp(1, m)  + R(2, 2) * tmp(2, m);

        kg(3, m) = R(0, 0) * tmp(3, m) + R(1, 0) * tmp(4, m)  + R(2, 0) * tmp(5, m);
        kg(4, m) = R(0, 1) * tmp(3, m) + R(1, 1) * tmp(4, m)  + R(2, 1) * tmp(5, m);
        kg(5, m) = R(0, 2) * tmp(3, m) + R(1, 2) * tmp(4, m)  + R(2, 2) * tmp(5, m);


        if (nodeIOffset)
        {
            kg(3, m) += RWI(0, 0) * tmp(0, m)  + RWI(1, 0) * tmp(1, m) + RWI(2, 0) * tmp(2, m);
            kg(4, m) += RWI(0, 1) * tmp(0, m)  + RWI(1, 1) * tmp(1, m) + RWI(2, 1) * tmp(2, m);
            kg(5, m) += RWI(0, 2) * tmp(0, m)  + RWI(1, 2) * tmp(1, m) + RWI(2, 2) * tmp(2, m);
        }


        kg(6, m) = R(0, 0) * tmp(6, m) + R(1, 0) * tmp(7, m)  + R(2, 0) * tmp(8, m);
        kg(7, m) = R(0, 1) * tmp(6, m) + R(1, 1) * tmp(7, m)  + R(2, 1) * tmp(8, m);
        kg(8, m) = R(0, 2) * tmp(6, m) + R(1, 2) * tmp(7, m)  + R(2, 2) * tmp(8, m);

        kg(9, m)  = R(0, 0) * tmp(9, m) + R(1, 0) * tmp(10, m) + R(2, 0) * tmp(11, m);
        kg(10, m) = R(0, 1) * tmp(9, m) + R(1, 1) * tmp(10, m) + R(2, 1) * tmp(11, m);
        kg(11, m) = R(0, 2) * tmp(9, m) + R(1, 2) * tmp(10, m) + R(2, 2) * tmp(11, m);


        if (nodeJOffset)
        {
            kg(9, m)  += RWJ(0, 0) * tmp(6, m)  + RWJ(1, 0) * tmp(7, m) + RWJ(2, 0) * tmp(8, m);
            kg(10, m) += RWJ(0, 1) * tmp(6, m)  + RWJ(1, 1) * tmp(7, m) + RWJ(2, 1) * tmp(8, m);
            kg(11, m) += RWJ(0, 2) * tmp(6, m)  + RWJ(1, 2) * tmp(7, m) + RWJ(2, 2) * tmp(8, m);
        }

    }

    return kg;
}





const Matrix &
ElasticBeamLumpedMass::getInitialGlobalStiffMatrix (const Matrix &KB)
{
    static Matrix kg(12, 12);   // Global stiffness for return
    static Matrix kb(6, 6);     // Basic stiffness
    static Matrix kl(12, 12);   // Local stiffness
    static Matrix tmp(12, 12);  // Temporary storage

    double oneOverL = 1.0 / L;

    int i, j;

    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++)
        {
            kb(i, j) = KB(i, j);
        }

    // Transform basic stiffness to local system
    // First compute kb*T_{bl}
    for (i = 0; i < 6; i++)
    {
        tmp(i, 0)  = -kb(i, 0);
        tmp(i, 1)  =  oneOverL * (kb(i, 1) + kb(i, 2));
        tmp(i, 2)  = -oneOverL * (kb(i, 3) + kb(i, 4));
        tmp(i, 3)  = -kb(i, 5);
        tmp(i, 4)  =  kb(i, 3);
        tmp(i, 5)  =  kb(i, 1);
        tmp(i, 6)  =  kb(i, 0);
        tmp(i, 7)  = -tmp(i, 1);
        tmp(i, 8)  = -tmp(i, 2);
        tmp(i, 9)  =  kb(i, 5);
        tmp(i, 10) =  kb(i, 4);
        tmp(i, 11) =  kb(i, 2);
    }

    // Now compute T'_{bl}*(kb*T_{bl})
    for (i = 0; i < 12; i++)
    {
        kl(0, i)  = -tmp(0, i);
        kl(1, i)  =  oneOverL * (tmp(1, i) + tmp(2, i));
        kl(2, i)  = -oneOverL * (tmp(3, i) + tmp(4, i));
        kl(3, i)  = -tmp(5, i);
        kl(4, i)  =  tmp(3, i);
        kl(5, i)  =  tmp(1, i);
        kl(6, i)  =  tmp(0, i);
        kl(7, i)  = -kl(1, i);
        kl(8, i)  = -kl(2, i);
        kl(9, i)  =  tmp(5, i);
        kl(10, i) =  tmp(4, i);
        kl(11, i) =  tmp(2, i);
    }

    static Matrix RWI(3, 3);


    if (nodeIOffset)
    {
        // Compute RWI
        RWI(0, 0) = -R(0, 1) * (*nodeIOffset)(2) + R(0, 2) * (*nodeIOffset)(1);
        RWI(1, 0) = -R(1, 1) * (*nodeIOffset)(2) + R(1, 2) * (*nodeIOffset)(1);
        RWI(2, 0) = -R(2, 1) * (*nodeIOffset)(2) + R(2, 2) * (*nodeIOffset)(1);

        RWI(0, 1) =  R(0, 0) * (*nodeIOffset)(2) - R(0, 2) * (*nodeIOffset)(0);
        RWI(1, 1) =  R(1, 0) * (*nodeIOffset)(2) - R(1, 2) * (*nodeIOffset)(0);
        RWI(2, 1) =  R(2, 0) * (*nodeIOffset)(2) - R(2, 2) * (*nodeIOffset)(0);

        RWI(0, 2) = -R(0, 0) * (*nodeIOffset)(1) + R(0, 1) * (*nodeIOffset)(0);
        RWI(1, 2) = -R(1, 0) * (*nodeIOffset)(1) + R(1, 1) * (*nodeIOffset)(0);
        RWI(2, 2) = -R(2, 0) * (*nodeIOffset)(1) + R(2, 1) * (*nodeIOffset)(0);
    }


    static Matrix RWJ(3, 3);


    if (nodeJOffset)
    {
        // Compute RWJ
        RWJ(0, 0) = -R(0, 1) * (*nodeJOffset)(2) + R(0, 2) * (*nodeJOffset)(1);
        RWJ(1, 0) = -R(1, 1) * (*nodeJOffset)(2) + R(1, 2) * (*nodeJOffset)(1);
        RWJ(2, 0) = -R(2, 1) * (*nodeJOffset)(2) + R(2, 2) * (*nodeJOffset)(1);

        RWJ(0, 1) =  R(0, 0) * (*nodeJOffset)(2) - R(0, 2) * (*nodeJOffset)(0);
        RWJ(1, 1) =  R(1, 0) * (*nodeJOffset)(2) - R(1, 2) * (*nodeJOffset)(0);
        RWJ(2, 1) =  R(2, 0) * (*nodeJOffset)(2) - R(2, 2) * (*nodeJOffset)(0);

        RWJ(0, 2) = -R(0, 0) * (*nodeJOffset)(1) + R(0, 1) * (*nodeJOffset)(0);
        RWJ(1, 2) = -R(1, 0) * (*nodeJOffset)(1) + R(1, 1) * (*nodeJOffset)(0);
        RWJ(2, 2) = -R(2, 0) * (*nodeJOffset)(1) + R(2, 1) * (*nodeJOffset)(0);
    }



    // Transform local stiffness to global system
    // First compute kl*T_{lg}
    int m;

    for (m = 0; m < 12; m++)
    {
        tmp(m, 0) = kl(m, 0) * R(0, 0) + kl(m, 1) * R(1, 0)  + kl(m, 2) * R(2, 0);
        tmp(m, 1) = kl(m, 0) * R(0, 1) + kl(m, 1) * R(1, 1)  + kl(m, 2) * R(2, 1);
        tmp(m, 2) = kl(m, 0) * R(0, 2) + kl(m, 1) * R(1, 2)  + kl(m, 2) * R(2, 2);

        tmp(m, 3) = kl(m, 3) * R(0, 0) + kl(m, 4) * R(1, 0)  + kl(m, 5) * R(2, 0);
        tmp(m, 4) = kl(m, 3) * R(0, 1) + kl(m, 4) * R(1, 1)  + kl(m, 5) * R(2, 1);
        tmp(m, 5) = kl(m, 3) * R(0, 2) + kl(m, 4) * R(1, 2)  + kl(m, 5) * R(2, 2);


        if (nodeIOffset)
        {
            tmp(m, 3)  += kl(m, 0) * RWI(0, 0)  + kl(m, 1) * RWI(1, 0)  + kl(m, 2) * RWI(2, 0);
            tmp(m, 4)  += kl(m, 0) * RWI(0, 1)  + kl(m, 1) * RWI(1, 1)  + kl(m, 2) * RWI(2, 1);
            tmp(m, 5)  += kl(m, 0) * RWI(0, 2)  + kl(m, 1) * RWI(1, 2)  + kl(m, 2) * RWI(2, 2);
        }


        tmp(m, 6) = kl(m, 6) * R(0, 0) + kl(m, 7) * R(1, 0)  + kl(m, 8) * R(2, 0);
        tmp(m, 7) = kl(m, 6) * R(0, 1) + kl(m, 7) * R(1, 1)  + kl(m, 8) * R(2, 1);
        tmp(m, 8) = kl(m, 6) * R(0, 2) + kl(m, 7) * R(1, 2)  + kl(m, 8) * R(2, 2);

        tmp(m, 9)  = kl(m, 9) * R(0, 0) + kl(m, 10) * R(1, 0) + kl(m, 11) * R(2, 0);
        tmp(m, 10) = kl(m, 9) * R(0, 1) + kl(m, 10) * R(1, 1) + kl(m, 11) * R(2, 1);
        tmp(m, 11) = kl(m, 9) * R(0, 2) + kl(m, 10) * R(1, 2) + kl(m, 11) * R(2, 2);


        if (nodeJOffset)
        {
            tmp(m, 9)   += kl(m, 6) * RWJ(0, 0)  + kl(m, 7) * RWJ(1, 0)  + kl(m, 8) * RWJ(2, 0);
            tmp(m, 10)  += kl(m, 6) * RWJ(0, 1)  + kl(m, 7) * RWJ(1, 1)  + kl(m, 8) * RWJ(2, 1);
            tmp(m, 11)  += kl(m, 6) * RWJ(0, 2)  + kl(m, 7) * RWJ(1, 2)  + kl(m, 8) * RWJ(2, 2);
        }


    }

    // Now compute T'_{lg}*(kl*T_{lg})
    for (m = 0; m < 12; m++)
    {
        kg(0, m) = R(0, 0) * tmp(0, m) + R(1, 0) * tmp(1, m)  + R(2, 0) * tmp(2, m);
        kg(1, m) = R(0, 1) * tmp(0, m) + R(1, 1) * tmp(1, m)  + R(2, 1) * tmp(2, m);
        kg(2, m) = R(0, 2) * tmp(0, m) + R(1, 2) * tmp(1, m)  + R(2, 2) * tmp(2, m);

        kg(3, m) = R(0, 0) * tmp(3, m) + R(1, 0) * tmp(4, m)  + R(2, 0) * tmp(5, m);
        kg(4, m) = R(0, 1) * tmp(3, m) + R(1, 1) * tmp(4, m)  + R(2, 1) * tmp(5, m);
        kg(5, m) = R(0, 2) * tmp(3, m) + R(1, 2) * tmp(4, m)  + R(2, 2) * tmp(5, m);


        if (nodeIOffset)
        {
            kg(3, m) += RWI(0, 0) * tmp(0, m)  + RWI(1, 0) * tmp(1, m) + RWI(2, 0) * tmp(2, m);
            kg(4, m) += RWI(0, 1) * tmp(0, m)  + RWI(1, 1) * tmp(1, m) + RWI(2, 1) * tmp(2, m);
            kg(5, m) += RWI(0, 2) * tmp(0, m)  + RWI(1, 2) * tmp(1, m) + RWI(2, 2) * tmp(2, m);
        }


        kg(6, m) = R(0, 0) * tmp(6, m) + R(1, 0) * tmp(7, m)  + R(2, 0) * tmp(8, m);
        kg(7, m) = R(0, 1) * tmp(6, m) + R(1, 1) * tmp(7, m)  + R(2, 1) * tmp(8, m);
        kg(8, m) = R(0, 2) * tmp(6, m) + R(1, 2) * tmp(7, m)  + R(2, 2) * tmp(8, m);

        kg(9, m)  = R(0, 0) * tmp(9, m) + R(1, 0) * tmp(10, m) + R(2, 0) * tmp(11, m);
        kg(10, m) = R(0, 1) * tmp(9, m) + R(1, 1) * tmp(10, m) + R(2, 1) * tmp(11, m);
        kg(11, m) = R(0, 2) * tmp(9, m) + R(1, 2) * tmp(10, m) + R(2, 2) * tmp(11, m);


        if (nodeJOffset)
        {
            kg(9, m)  += RWJ(0, 0) * tmp(6, m)  + RWJ(1, 0) * tmp(7, m) + RWJ(2, 0) * tmp(8, m);
            kg(10, m) += RWJ(0, 1) * tmp(6, m)  + RWJ(1, 1) * tmp(7, m) + RWJ(2, 1) * tmp(8, m);
            kg(11, m) += RWJ(0, 2) * tmp(6, m)  + RWJ(1, 2) * tmp(7, m) + RWJ(2, 2) * tmp(8, m);
        }

    }

    return kg;
}






const Vector &
ElasticBeamLumpedMass::getGlobalResistingForce(const Vector &pb, const Vector &p0)
{
    // transform resisting forces from the basic system to local coordinates
    static Vector pl(12);

    double q0 = pb(0);
    double q1 = pb(1);
    double q2 = pb(2);
    double q3 = pb(3);
    double q4 = pb(4);
    double q5 = pb(5);

    double oneOverL = 1.0 / L;

    pl(0)  = -q0;
    pl(1)  =  oneOverL * (q1 + q2);
    pl(2)  = -oneOverL * (q3 + q4);
    pl(3)  = -q5;
    pl(4)  =  q3;
    pl(5)  =  q1;
    pl(6)  =  q0;
    pl(7)  = -pl(1);
    pl(8)  = -pl(2);
    pl(9)  =  q5;
    pl(10) =  q4;
    pl(11) =  q2;

    pl(0) += p0(0);
    pl(1) += p0(1);
    pl(7) += p0(2);
    pl(2) += p0(3);
    pl(8) += p0(4);

    // transform resisting forces  from local to global coordinates
    static Vector pg(12);

    pg(0)  = R(0, 0) * pl(0) + R(1, 0) * pl(1) + R(2, 0) * pl(2);
    pg(1)  = R(0, 1) * pl(0) + R(1, 1) * pl(1) + R(2, 1) * pl(2);
    pg(2)  = R(0, 2) * pl(0) + R(1, 2) * pl(1) + R(2, 2) * pl(2);

    pg(3)  = R(0, 0) * pl(3) + R(1, 0) * pl(4) + R(2, 0) * pl(5);
    pg(4)  = R(0, 1) * pl(3) + R(1, 1) * pl(4) + R(2, 1) * pl(5);
    pg(5)  = R(0, 2) * pl(3) + R(1, 2) * pl(4) + R(2, 2) * pl(5);

    pg(6)  = R(0, 0) * pl(6) + R(1, 0) * pl(7) + R(2, 0) * pl(8);
    pg(7)  = R(0, 1) * pl(6) + R(1, 1) * pl(7) + R(2, 1) * pl(8);
    pg(8)  = R(0, 2) * pl(6) + R(1, 2) * pl(7) + R(2, 2) * pl(8);

    pg(9)  = R(0, 0) * pl(9) + R(1, 0) * pl(10) + R(2, 0) * pl(11);
    pg(10) = R(0, 1) * pl(9) + R(1, 1) * pl(10) + R(2, 1) * pl(11);
    pg(11) = R(0, 2) * pl(9) + R(1, 2) * pl(10) + R(2, 2) * pl(11);

    if (nodeIOffset)
    {
        pg(3) += -(*nodeIOffset)(2) * pg(1) + (*nodeIOffset)(1) * pg(2);
        pg(4) +=  (*nodeIOffset)(2) * pg(0) - (*nodeIOffset)(0) * pg(2);
        pg(5) += -(*nodeIOffset)(1) * pg(0) + (*nodeIOffset)(0) * pg(1);
    }

    if (nodeJOffset)
    {
        pg(9)  += -(*nodeJOffset)(2) * pg(7) + (*nodeJOffset)(1) * pg(8);
        pg(10) +=  (*nodeJOffset)(2) * pg(6) - (*nodeJOffset)(0) * pg(8);
        pg(11) += -(*nodeJOffset)(1) * pg(6) + (*nodeJOffset)(0) * pg(7);
    }

    return pg;
}



Vector *
ElasticBeamLumpedMass::getForce(void)
{
    Vector *elementForces = new Vector(12);
    Vector tempForces(12);

    tempForces = this->getResistingForceIncInertia();


    (*elementForces)(0)  = tempForces(0);
    (*elementForces)(1)  = tempForces(1);
    (*elementForces)(2)  = tempForces(2);
    (*elementForces)(3)  = tempForces(3);
    (*elementForces)(4)  = tempForces(4);
    (*elementForces)(5)  = tempForces(5);
    (*elementForces)(6)  = tempForces(6);
    (*elementForces)(7)  = tempForces(7);
    (*elementForces)(8)  = tempForces(8);
    (*elementForces)(9)  = tempForces(9);
    (*elementForces)(10) = tempForces(10);
    (*elementForces)(11) = tempForces(11);


    return elementForces;
}
