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
// DispBeamColumn3d class
//============================================================


// $Revision: 1.25 $
// $Date: 2008/11/04 21:32:05 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/dispBeamColumn/DispBeamColumn3d.cpp,v $

// Written: MHS
// Created: Feb 2001
//
// Description: This file contains the class definition for DispBeamColumn3d.

#include <DispBeamColumn3d.h>
#include <Node.h>
#include <SectionForceDeformation.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>
#include <Domain.h>
#include <string.h>
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
// #include <ElementResponse.h>
#include <ElementalLoad.h>
#include <BeamIntegration.h>


Matrix DispBeamColumn3d::K(12, 12);
Vector DispBeamColumn3d::P(12);
double DispBeamColumn3d::workArea[200];

DispBeamColumn3d::DispBeamColumn3d(int tag, int nd1, int nd2,
                                   int numSec, SectionForceDeformation **s,
                                   BeamIntegration &bi, double r,
                                   double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                                   double rigJntOffset1_x,   double rigJntOffset1_y,   double rigJntOffset1_z,
                                   double rigJntOffset2_x,   double rigJntOffset2_y,   double rigJntOffset2_z)
    : Element (tag, ELE_TAG_DispBeamColumn3d),
      numSections(numSec), theSections(0), beamInt(0),
      connectedExternalNodes(2), nodeIOffset(0), nodeJOffset(0), L(0), A(0),
      Q(12), q(6), rho(r), parameterID(0),
      nodeIInitialDisp(0), nodeJInitialDisp(0)
{
    // Allocate arrays of pointers to SectionForceDeformations
    theSections = new SectionForceDeformation *[numSections];

    if (theSections == 0)
    {
        cerr << "DispBeamColumn3d::DispBeamColumn3d - failed to allocate section model pointer\n";
        exit(-1);
    }

    for (int i = 0; i < numSections; i++)
    {

        // Get copies of the material model for each integration point
        theSections[i] = s[i]->getCopy();

        // Check allocation
        if (theSections[i] == 0)
        {
            cerr << "DispBeamColumn3d::DispBeamColumn3d -- failed to get a copy of section model\n";
            exit(-1);
        }
    }

    beamInt = bi.getCopy();

    if (beamInt == 0)
    {
        cerr << "DispBeamColumn3d::DispBeamColumn3d - failed to copy beam integration\n";
        exit(-1);
    }



    //==========================================================================================
    // Nima Tafazzoli, Nov. 2012, storing the area of section to be used for lumped mass matrix

    A = theSections[0]->getArea();

    //==========================================================================================



    R[2][0] = vecInLocXZPlane_x;
    R[2][1] = vecInLocXZPlane_y;
    R[2][2] = vecInLocXZPlane_z;


    nodeIOffset = new double[3];
    nodeIOffset[0] = rigJntOffset1_x;
    nodeIOffset[1] = rigJntOffset1_y;
    nodeIOffset[2] = rigJntOffset1_z;


    nodeJOffset = new double[3];
    nodeJOffset[0] = rigJntOffset2_x;
    nodeJOffset[1] = rigJntOffset2_y;
    nodeJOffset[2] = rigJntOffset2_z;



    // Set connected external node IDs
    connectedExternalNodes(0) = nd1;
    connectedExternalNodes(1) = nd2;


    theNodes[0] = 0;
    theNodes[1] = 0;

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
}

DispBeamColumn3d::DispBeamColumn3d()
    : Element (0, ELE_TAG_DispBeamColumn3d),
      numSections(0), theSections(0), beamInt(0),
      connectedExternalNodes(2), nodeIOffset(0), nodeJOffset(0), L(0), A(0),
      Q(12), q(6), rho(0.0), parameterID(0),
      nodeIInitialDisp(0), nodeJInitialDisp(0)
{
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

    theNodes[0] = 0;
    theNodes[1] = 0;


    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            R[i][j] = 0.0;
        }
}


DispBeamColumn3d::~DispBeamColumn3d()
{
    for (int i = 0; i < numSections; i++)
    {
        if (theSections[i])
        {
            delete theSections[i];
        }
    }

    // Delete the array of pointers to SectionForceDeformation pointer arrays
    if (theSections)
    {
        delete [] theSections;
    }

    if (beamInt != 0)
    {
        delete beamInt;
    }

    if (nodeIOffset)
    {
        delete [] nodeIOffset;
    }

    if (nodeJOffset)
    {
        delete [] nodeJOffset;
    }

    if (nodeIInitialDisp != 0)
    {
        delete [] nodeIInitialDisp;
    }

    if (nodeJInitialDisp != 0)
    {
        delete [] nodeJInitialDisp;
    }

}

int
DispBeamColumn3d::getNumExternalNodes() const
{
    return 2;
}

const ID &
DispBeamColumn3d::getExternalNodes()
{
    return connectedExternalNodes;
}

Node **
DispBeamColumn3d::getNodePtrs()
{

    return theNodes;
}

int
DispBeamColumn3d::getNumDOF()
{
    return 12;
}

void
DispBeamColumn3d::setDomain(Domain *theDomain)
{
    // Check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
        return;
    }

    int Nd1 = connectedExternalNodes(0);
    int Nd2 = connectedExternalNodes(1);

    theNodes[0] = theDomain->getNode(Nd1);
    theNodes[1] = theDomain->getNode(Nd2);

    if (theNodes[0] == 0 || theNodes[1] == 0)
    {
        //cerr << "FATAL ERROR DispBeamColumn3d (tag: %d), node not found in domain",
        //  this->getTag());

        return;
    }

    int dofNd1 = theNodes[0]->getNumberDOF();
    int dofNd2 = theNodes[1]->getNumberDOF();

    if (dofNd1 != 6 || dofNd2 != 6)
    {
        //cerr << "FATAL ERROR DispBeamColumn3d (tag: %d), has differing number of DOFs at its nodes",
        //  this->getTag());

        return;
    }


    if (this->initialize() != 0)
    {
        cerr << "DispBeamColumn3d::setDomain -- Error initializing coordinate transformation\n";
        exit(-1);
    }


    this->DomainComponent::setDomain(theDomain);

    this->update();
}

int
DispBeamColumn3d::commitState()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "DispBeamColumn3d::commitState () - failed in base class";
    }

    // Loop over the integration points and commit the material states
    for (int i = 0; i < numSections; i++)
    {
        retVal += theSections[i]->commitState();
    }


    return retVal;
}

int
DispBeamColumn3d::revertToLastCommit()
{
    int retVal = 0;

    // Loop over the integration points and revert to last committed state
    for (int i = 0; i < numSections; i++)
    {
        retVal += theSections[i]->revertToLastCommit();
    }


    return retVal;
}

int
DispBeamColumn3d::revertToStart()
{
    int retVal = 0;

    // Loop over the integration points and revert states to start
    for (int i = 0; i < numSections; i++)
    {
        retVal += theSections[i]->revertToStart();
    }


    return retVal;
}

int
DispBeamColumn3d::update(void)
{
    int err = 0;


    // Get basic deformations
    const Vector &v = this->getBasicTrialDisp();

    double oneOverL = 1.0 / L;

    //const Matrix &pts = quadRule.getIntegrPointCoords(numSections);
    double xi[maxNumSections];
    beamInt->getSectionLocations(numSections, L, xi);

    // Loop over the integration points
    for (int i = 0; i < numSections; i++)
    {

        int order = theSections[i]->getOrder();
        const ID &code = theSections[i]->getType();

        Vector e(workArea, order);

        double xi6 = 6.0 * xi[i];

        int j;

        for (j = 0; j < order; j++)
        {
            switch (code(j))
            {
                case SECTION_RESPONSE_P:
                    e(j) = oneOverL * v(0);
                    break;

                case SECTION_RESPONSE_MZ:
                    e(j) = oneOverL * ((xi6 - 4.0) * v(1) + (xi6 - 2.0) * v(2));
                    break;

                case SECTION_RESPONSE_MY:
                    e(j) = oneOverL * ((xi6 - 4.0) * v(3) + (xi6 - 2.0) * v(4));
                    break;

                case SECTION_RESPONSE_T:
                    e(j) = oneOverL * v(5);
                    break;

                default:
                    e(j) = 0.0;
                    break;
            }
        }

        // Set the section deformations
        err += theSections[i]->setTrialSectionDeformation(e);
    }

    if (err != 0)
    {
        cerr << "DispBeamColumn3d::update() - failed setTrialSectionDeformations()\n";
        return err;
    }

    return 0;
}

const Matrix &
DispBeamColumn3d::getTangentStiff()
{
    static Matrix kb(6, 6);

    // Zero for integral
    kb.Zero();
    q.Zero();

    double oneOverL = 1.0 / L;

    //const Matrix &pts = quadRule.getIntegrPointCoords(numSections);
    //const Vector &wts = quadRule.getIntegrPointWeights(numSections);
    double xi[maxNumSections];
    beamInt->getSectionLocations(numSections, L, xi);
    double wt[maxNumSections];
    beamInt->getSectionWeights(numSections, L, wt);

    // Loop over the integration points
    for (int i = 0; i < numSections; i++)
    {

        int order = theSections[i]->getOrder();
        const ID &code = theSections[i]->getType();

        Matrix ka(workArea, order, 6);
        ka.Zero();

        double xi6 = 6.0 * xi[i];

        // Get the section tangent stiffness and stress resultant
        const Matrix &ks = theSections[i]->getSectionTangent();
        const Vector &s = theSections[i]->getStressResultant();

        // Perform numerical integration
        //kb.addMatrixTripleProduct(1.0, *B, ks, wts(i)/L);
        double wti = wt[i] * oneOverL;
        double tmp;
        int j, k;

        for (j = 0; j < order; j++)
        {
            switch (code(j))
            {
                case SECTION_RESPONSE_P:
                    for (k = 0; k < order; k++)
                    {
                        ka(k, 0) += ks(k, j) * wti;
                    }

                    break;

                case SECTION_RESPONSE_MZ:
                    for (k = 0; k < order; k++)
                    {
                        tmp = ks(k, j) * wti;
                        ka(k, 1) += (xi6 - 4.0) * tmp;
                        ka(k, 2) += (xi6 - 2.0) * tmp;
                    }

                    break;

                case SECTION_RESPONSE_MY:
                    for (k = 0; k < order; k++)
                    {
                        tmp = ks(k, j) * wti;
                        ka(k, 3) += (xi6 - 4.0) * tmp;
                        ka(k, 4) += (xi6 - 2.0) * tmp;
                    }

                    break;

                case SECTION_RESPONSE_T:
                    for (k = 0; k < order; k++)
                    {
                        ka(k, 5) += ks(k, j) * wti;
                    }

                    break;

                default:
                    break;
            }
        }

        for (j = 0; j < order; j++)
        {
            switch (code(j))
            {
                case SECTION_RESPONSE_P:
                    for (k = 0; k < 6; k++)
                    {
                        kb(0, k) += ka(j, k);
                    }

                    break;

                case SECTION_RESPONSE_MZ:
                    for (k = 0; k < 6; k++)
                    {
                        tmp = ka(j, k);
                        kb(1, k) += (xi6 - 4.0) * tmp;
                        kb(2, k) += (xi6 - 2.0) * tmp;
                    }

                    break;

                case SECTION_RESPONSE_MY:
                    for (k = 0; k < 6; k++)
                    {
                        tmp = ka(j, k);
                        kb(3, k) += (xi6 - 4.0) * tmp;
                        kb(4, k) += (xi6 - 2.0) * tmp;
                    }

                    break;

                case SECTION_RESPONSE_T:
                    for (k = 0; k < 6; k++)
                    {
                        kb(5, k) += ka(j, k);
                    }

                    break;

                default:
                    break;
            }
        }

        //q.addMatrixTransposeVector(1.0, *B, s, wts(i));
        double si;

        for (j = 0; j < order; j++)
        {
            si = s(j) * wt[i];

            switch (code(j))
            {
                case SECTION_RESPONSE_P:
                    q(0) += si;
                    break;

                case SECTION_RESPONSE_MZ:
                    q(1) += (xi6 - 4.0) * si;
                    q(2) += (xi6 - 2.0) * si;
                    break;

                case SECTION_RESPONSE_MY:
                    q(3) += (xi6 - 4.0) * si;
                    q(4) += (xi6 - 2.0) * si;
                    break;

                case SECTION_RESPONSE_T:
                    q(5) += si;
                    break;

                default:
                    break;
            }
        }

    }

    q(0) += q0[0];
    q(1) += q0[1];
    q(2) += q0[2];
    q(3) += q0[3];
    q(4) += q0[4];

    // Transform to global stiffness
    K = this->getGlobalStiffMatrix(kb, q);

    return K;
}

const Matrix &
DispBeamColumn3d::getInitialBasicStiff()
{
    static Matrix kb(6, 6);

    // Zero for integral
    kb.Zero();

    double oneOverL = 1.0 / L;

    //const Matrix &pts = quadRule.getIntegrPointCoords(numSections);
    //const Vector &wts = quadRule.getIntegrPointWeights(numSections);
    double xi[maxNumSections];
    beamInt->getSectionLocations(numSections, L, xi);
    double wt[maxNumSections];
    beamInt->getSectionWeights(numSections, L, wt);

    // Loop over the integration points
    for (int i = 0; i < numSections; i++)
    {

        int order = theSections[i]->getOrder();
        const ID &code = theSections[i]->getType();

        Matrix ka(workArea, order, 6);
        ka.Zero();

        double xi6 = 6.0 * xi[i];

        // Get the section tangent stiffness and stress resultant
        const Matrix &ks = theSections[i]->getInitialTangent();

        // Perform numerical integration
        //kb.addMatrixTripleProduct(1.0, *B, ks, wts(i)/L);
        double wti = wt[i] * oneOverL;
        double tmp;
        int j, k;

        for (j = 0; j < order; j++)
        {
            switch (code(j))
            {
                case SECTION_RESPONSE_P:
                    for (k = 0; k < order; k++)
                    {
                        ka(k, 0) += ks(k, j) * wti;
                    }

                    break;

                case SECTION_RESPONSE_MZ:
                    for (k = 0; k < order; k++)
                    {
                        tmp = ks(k, j) * wti;
                        ka(k, 1) += (xi6 - 4.0) * tmp;
                        ka(k, 2) += (xi6 - 2.0) * tmp;
                    }

                    break;

                case SECTION_RESPONSE_MY:
                    for (k = 0; k < order; k++)
                    {
                        tmp = ks(k, j) * wti;
                        ka(k, 3) += (xi6 - 4.0) * tmp;
                        ka(k, 4) += (xi6 - 2.0) * tmp;
                    }

                    break;

                case SECTION_RESPONSE_T:
                    for (k = 0; k < order; k++)
                    {
                        ka(k, 5) += ks(k, j) * wti;
                    }

                    break;

                default:
                    break;
            }
        }

        for (j = 0; j < order; j++)
        {
            switch (code(j))
            {
                case SECTION_RESPONSE_P:
                    for (k = 0; k < 6; k++)
                    {
                        kb(0, k) += ka(j, k);
                    }

                    break;

                case SECTION_RESPONSE_MZ:
                    for (k = 0; k < 6; k++)
                    {
                        tmp = ka(j, k);
                        kb(1, k) += (xi6 - 4.0) * tmp;
                        kb(2, k) += (xi6 - 2.0) * tmp;
                    }

                    break;

                case SECTION_RESPONSE_MY:
                    for (k = 0; k < 6; k++)
                    {
                        tmp = ka(j, k);
                        kb(3, k) += (xi6 - 4.0) * tmp;
                        kb(4, k) += (xi6 - 2.0) * tmp;
                    }

                    break;

                case SECTION_RESPONSE_T:
                    for (k = 0; k < 6; k++)
                    {
                        kb(5, k) += ka(j, k);
                    }

                    break;

                default:
                    break;
            }
        }

    }

    return kb;
}

const Matrix &
DispBeamColumn3d::getInitialStiff()
{
    const Matrix &kb = this->getInitialBasicStiff();

    // Transform to global stiffness
    K = this->getInitialGlobalStiffMatrix(kb);

    return K;
}

const Matrix &
DispBeamColumn3d::getMass()
{
    K.Zero();

    if (rho == 0.0)
    {
        return K;
    }


    // Nima Tafazzoli (Nov. 2012)
    // changing density from per length to per volume
    double m = 0.5 * rho * L * A;

    K(0, 0) = K(1, 1) = K(2, 2) = K(6, 6) = K(7, 7) = K(8, 8) = m;

    return K;
}

void
DispBeamColumn3d::zeroLoad(void)
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
DispBeamColumn3d::addLoad(ElementalLoad *theLoad, double loadFactor)
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
        cerr << "DispBeamColumn3d::addLoad() -- load type unknown for element with tag: " <<
             this->getTag() << endln;
        return -1;
    }

    return 0;
}

int
DispBeamColumn3d::addInertiaLoadToUnbalance(const Vector &accel)
{
    // Check for a quick return
    if (rho == 0.0)
    {
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV(accel);
    const Vector &Raccel2 = theNodes[1]->getRV(accel);

    if (6 != Raccel1.Size() || 6 != Raccel2.Size())
    {
        cerr << "DispBeamColumn3d::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }


    // Nima Tafazzoli (Nov. 2012)
    // changing density from per length to per volume
    double m = 0.5 * rho * L * A;

    // Want to add ( - fact * M R * accel ) to unbalance
    // Take advantage of lumped mass matrix
    Q(0) -= m * Raccel1(0);
    Q(1) -= m * Raccel1(1);
    Q(2) -= m * Raccel1(2);
    Q(6) -= m * Raccel2(0);
    Q(7) -= m * Raccel2(1);
    Q(8) -= m * Raccel2(2);

    return 0;
}

const Vector &
DispBeamColumn3d::getResistingForce()
{

    //const Matrix &pts = quadRule.getIntegrPointCoords(numSections);
    //const Vector &wts = quadRule.getIntegrPointWeights(numSections);
    double xi[maxNumSections];
    beamInt->getSectionLocations(numSections, L, xi);
    double wt[maxNumSections];
    beamInt->getSectionWeights(numSections, L, wt);

    // Zero for integration
    q.Zero();

    // Loop over the integration points
    for (int i = 0; i < numSections; i++)
    {

        int order = theSections[i]->getOrder();
        const ID &code = theSections[i]->getType();

        double xi6 = 6.0 * xi[i];

        // Get section stress resultant
        const Vector &s = theSections[i]->getStressResultant();

        // Perform numerical integration on internal force
        //q.addMatrixTransposeVector(1.0, *B, s, wts(i));

        double si;

        for (int j = 0; j < order; j++)
        {
            si = s(j) * wt[i];

            switch (code(j))
            {
                case SECTION_RESPONSE_P:
                    q(0) += si;
                    break;

                case SECTION_RESPONSE_MZ:
                    q(1) += (xi6 - 4.0) * si;
                    q(2) += (xi6 - 2.0) * si;
                    break;

                case SECTION_RESPONSE_MY:
                    q(3) += (xi6 - 4.0) * si;
                    q(4) += (xi6 - 2.0) * si;
                    break;

                case SECTION_RESPONSE_T:
                    q(5) += si;
                    break;

                default:
                    break;
            }
        }

    }

    q(0) += q0[0];
    q(1) += q0[1];
    q(2) += q0[2];
    q(3) += q0[3];
    q(4) += q0[4];

    // Transform forces
    Vector p0Vec(p0, 5);
    P = this->getGlobalResistingForce(q, p0Vec);

    // Subtract other external nodal loads ... P_res = P_int - P_ext
    P.addVector(1.0, Q, -1.0);

    return P;
}

const Vector &
DispBeamColumn3d::getResistingForceIncInertia()
{
    this->getResistingForce();

    if (rho != 0.0)
    {
        const Vector &accel1 = theNodes[0]->getTrialAccel();
        const Vector &accel2 = theNodes[1]->getTrialAccel();

        // Compute the current resisting force
        this->getResistingForce();


        // Nima Tafazzoli (Nov. 2012)
        // changing density from per length to per volume
        double m = 0.5 * rho * L * A;

        P(0) += m * accel1(0);
        P(1) += m * accel1(1);
        P(2) += m * accel1(2);
        P(6) += m * accel2(0);
        P(7) += m * accel2(1);
        P(8) += m * accel2(2);

        // add the damping forces if rayleigh damping
        if (a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            P += this->getRayleighDampingForces();
        }

    }
    else
    {

        // add the damping forces if rayleigh damping
        if (a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            P += this->getRayleighDampingForces();
        }
    }

    return P;
}

int
DispBeamColumn3d::sendSelf(int commitTag, Channel &theChannel)
{
    // place the integer data into an ID

    int dbTag = this->getDbTag();
    int i, j;
    int loc = 0;

    static ID idData(7);  // one bigger than needed so no clash later
    idData(0) = this->getTag();
    idData(1) = connectedExternalNodes(0);
    idData(2) = connectedExternalNodes(1);
    idData(3) = numSections;
    //   idData(4) = crdTransf->getClassTag();
    //   int crdTransfDbTag  = crdTransf->getDbTag();
    //   if (crdTransfDbTag  == 0) {
    //     crdTransfDbTag = theChannel.getDbTag();
    //     if (crdTransfDbTag  != 0)
    //       crdTransf->setDbTag(crdTransfDbTag);
    //   }
    //   idData(5) = crdTransfDbTag;

    if (a0 != 0 || a1 != 0 || a2 != 0 || a3 != 0)
    {
        idData(6) = 1;
    }
    else
    {
        idData(6) = 0;
    }


    if (theChannel.sendID(dbTag, commitTag, idData) < 0)
    {
        cerr << "DispBeamColumn3d::sendSelf() - failed to send ID data\n";
        return -1;
    }

    if (idData(6) == 1)
    {
        // send damping coefficients
        static Vector dData(4);
        dData(0) = a0;
        dData(1) = a1;
        dData(2) = a2;
        dData(3) = a3;

        if (theChannel.sendVector(dbTag, commitTag, dData) < 0)
        {
            cerr << "DispBeamColumn3d::sendSelf() - failed to send double data\n";
            return -1;
        }
    }

    // send the coordinate transformation
    //   if (crdTransf->sendSelf(commitTag, theChannel) < 0) {
    //      cerr << "DispBeamColumn3d::sendSelf() - failed to send crdTranf\n";
    //      return -1;
    //   }


    //
    // send an ID for the sections containing each sections dbTag and classTag
    // if section ha no dbTag get one and assign it
    //

    ID idSections(2 * numSections);
    loc = 0;

    for (i = 0; i < numSections; i++)
    {
        int sectClassTag = theSections[i]->getClassTag();
        int sectDbTag = theSections[i]->getDbTag();

        if (sectDbTag == 0)
        {
            sectDbTag = theChannel.getDbTag();
            theSections[i]->setDbTag(sectDbTag);
        }

        idSections(loc) = sectClassTag;
        idSections(loc + 1) = sectDbTag;
        loc += 2;
    }

    if (theChannel.sendID(dbTag, commitTag, idSections) < 0)
    {
        cerr << "DispBeamColumn3d::sendSelf() - failed to send ID data\n";
        return -1;
    }

    //
    // send the sections
    //

    for (j = 0; j < numSections; j++)
    {
        if (theSections[j]->sendSelf(commitTag, theChannel) < 0)
        {
            cerr << "DispBeamColumn3d::sendSelf() - section " << j << "failed to send itself\n";
            return -1;
        }
    }

    return 0;
}

int
DispBeamColumn3d::receiveSelf(int commitTag, Channel &theChannel,
                           FEM_ObjectBroker &theBroker)
{
    //
    // receive the integer data containing tag, numSections and coord transformation info
    //
    int dbTag = this->getDbTag();
    int i;

    static ID idData(7); // one bigger than needed so no clash with section ID

    if (theChannel.receiveID(dbTag, commitTag, idData) < 0)
    {
        cerr << "DispBeamColumn3d::receiveSelf() - failed to recv ID data\n";
        return -1;
    }

    this->setTag(idData(0));
    connectedExternalNodes(0) = idData(1);
    connectedExternalNodes(1) = idData(2);

    //   int crdTransfClassTag = idData(4);
    //   int crdTransfDbTag = idData(5);

    if (idData(6) == 1)
    {
        // recv damping coefficients
        static Vector dData(4);

        if (theChannel.receiveVector(dbTag, commitTag, dData) < 0)
        {
            cerr << "DispBeamColumn3d::sendSelf() - failed to recv double data\n";
            return -1;
        }

        a0 = dData(0);
        a1 = dData(1);
        a2 = dData(2);
        a3 = dData(3);
    }

    // create a new crdTransf object if one needed
    //   if (crdTransf == 0 || crdTransf->getClassTag() != crdTransfClassTag) {
    //       if (crdTransf != 0)
    //    delete crdTransf;

    //       crdTransf = theBroker.getNewCrdTransf3d(crdTransfClassTag);

    //       if (crdTransf == 0) {
    //  cerr << "DispBeamColumn3d::receiveSelf() - " <<
    //    "failed to obtain a CrdTrans object with classTag" <<
    //    crdTransfClassTag << endln;
    //  return -2;
    //       }
    //   }

    //   crdTransf->setDbTag(crdTransfDbTag);

    // invoke receiveSelf on the crdTransf object
    //   if (crdTransf->receiveSelf(commitTag, theChannel, theBroker) < 0) {
    //     cerr << "DispBeamColumn3d::sendSelf() - failed to recv crdTranf\n";
    //     return -3;
    //   }

    //
    // recv an ID for the sections containing each sections dbTag and classTag
    //

    ID idSections(2 * idData(3));
    int loc = 0;

    if (theChannel.receiveID(dbTag, commitTag, idSections) < 0)
    {
        cerr << "DispBeamColumn3d::receiveSelf() - failed to recv ID data\n";
        return -1;
    }

    //
    // now receive the sections
    //

    if (numSections != idData(3))
    {

        //
        // we do not have correct number of sections, must delete the old and create
        // new ones before can receiveSelf on the sections
        //

        // delete the old
        if (numSections != 0)
        {
            for (int i = 0; i < numSections; i++)
            {
                delete theSections[i];
            }

            delete [] theSections;
        }

        // create a new array to hold pointers
        theSections = new SectionForceDeformation *[idData(3)];

        if (theSections == 0)
        {
            cerr << "DispBeamColumn3d::receiveSelf() - out of memory creating sections array of size" <<
                 idData(3) << endln;
            exit(-1);
        }

        // create a section and receiveSelf on it
        numSections = idData(3);
        loc = 0;

        for (i = 0; i < numSections; i++)
        {
            int sectClassTag = idSections(loc);
            int sectDbTag = idSections(loc + 1);
            loc += 2;
            theSections[i] = theBroker.getNewSection(sectClassTag);

            if (theSections[i] == 0)
            {
                cerr << "DispBeamColumn3d::receiveSelf() - Broker could not create Section of class type" <<
                     sectClassTag << endln;
                exit(-1);
            }

            theSections[i]->setDbTag(sectDbTag);

            if (theSections[i]->receiveSelf(commitTag, theChannel, theBroker) < 0)
            {
                cerr << "DispBeamColumn3d::receiveSelf() - section " <<
                     i << "failed to recv itself\n";
                return -1;
            }
        }

    }
    else
    {

        //
        // for each existing section, check it is of correct type
        // (if not delete old & create a new one) then receiveSelf on it
        //

        loc = 0;

        for (i = 0; i < numSections; i++)
        {
            int sectClassTag = idSections(loc);
            int sectDbTag = idSections(loc + 1);
            loc += 2;

            // check of correct type
            if (theSections[i]->getClassTag() !=  sectClassTag)
            {
                // delete the old section[i] and create a new one
                delete theSections[i];
                theSections[i] = theBroker.getNewSection(sectClassTag);

                if (theSections[i] == 0)
                {
                    cerr << "DispBeamColumn3d::receiveSelf() - Broker could not create Section of class type" <<
                         sectClassTag << endln;
                    exit(-1);
                }
            }

            // receiveSelf on it
            theSections[i]->setDbTag(sectDbTag);

            if (theSections[i]->receiveSelf(commitTag, theChannel, theBroker) < 0)
            {
                cerr << "DispBeamColumn3d::receiveSelf() - section " <<
                     i << "failed to recv itself\n";
                return -1;
            }
        }
    }

    return 0;
}

void
DispBeamColumn3d::Print(ostream &s, int flag)
{
    s << "\nDispBeamColumn3d, element id:  " << this->getTag() << endln;
    s << "\tConnected external nodes:  " << connectedExternalNodes;
    s << "\tmass density:  " << rho << endln;

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

    //for (int i = 0; i < numSections; i++)
    //theSections[i]->Print(s,flag);
}


// int
// DispBeamColumn3d::displaySelf(Renderer &theViewer, int displayMode, float fact)
// {
//   // first determine the end points of the quad based on
//   // the display factor (a measure of the distorted image)
//   const Vector &end1Crd = theNodes[0]->getCrds();
//   const Vector &end2Crd = theNodes[1]->getCrds();
//
//   static Vector v1(3);
//   static Vector v2(3);
//
//   if (displayMode >= 0) {
//     const Vector &end1Disp = theNodes[0]->getDisp();
//     const Vector &end2Disp = theNodes[1]->getDisp();
//
//     for (int i = 0; i < 3; i++) {
//       v1(i) = end1Crd(i) + end1Disp(i)*fact;
//       v2(i) = end2Crd(i) + end2Disp(i)*fact;
//     }
//   } else {
//     int mode = displayMode * -1;
//     const Matrix &eigen1 = theNodes[0]->getEigenvectors();
//     const Matrix &eigen2 = theNodes[1]->getEigenvectors();
//     if (eigen1.noCols() >= mode) {
//       for (int i = 0; i < 3; i++) {
//  v1(i) = end1Crd(i) + eigen1(i,mode-1)*fact;
//  v2(i) = end2Crd(i) + eigen2(i,mode-1)*fact;
//       }
//
//     } else {
//       for (int i = 0; i < 3; i++) {
//  v1(i) = end1Crd(i);
//  v2(i) = end2Crd(i);
//       }
//     }
//   }
//   return theViewer.drawLine (v1, v2, 1.0, 1.0);
// }

// Response*
// DispBeamColumn3d::setResponse(const char** argv, int argc, Information& eleInfo)
// {
//     // global force -
//     if (strcmp(argv[0], "forces") == 0 || strcmp(argv[0], "force") == 0
//             || strcmp(argv[0], "globalForce") == 0 || strcmp(argv[0], "globalForces") == 0)
//     {
//         return new ElementResponse(this, 1, P);
//     }

//     // local force -
//     else if (strcmp(argv[0], "localForce") == 0 || strcmp(argv[0], "localForces") == 0)
//     {
//         return new ElementResponse(this, 2, P);
//     }

//     // chord rotation -
//     else if (strcmp(argv[0], "chordRotation") == 0 || strcmp(argv[0], "chordDeformation") == 0
//              || strcmp(argv[0], "basicDeformation") == 0)
//     {
//         return new ElementResponse(this, 3, Vector(6));
//     }

//     // plastic rotation -
//     else if (strcmp(argv[0], "plasticRotation") == 0 || strcmp(argv[0], "plasticDeformation") == 0)
//     {
//         return new ElementResponse(this, 4, Vector(6));
//     }

//     // section response -
//     else if (strcmp(argv[0], "section") == 0 || strcmp(argv[0], "-section") == 0)
//     {
//         if (argc <= 2)
//         {
//             return 0;
//         }

//         int sectionNum = atoi(argv[1]);

//         if (sectionNum > 0 && sectionNum <= numSections)
//         {
//             return theSections[sectionNum - 1]->setResponse(&argv[2], argc - 2, eleInfo);
//         }
//         else
//         {
//             return 0;
//         }
//     }

//     else
//     {
//         return 0;
//     }
// }

// int
// DispBeamColumn3d::getResponse(int responseID, Information& eleInfo)
// {
//     double N, V, M1, M2, T;
//     double oneOverL = 1.0 / L;

//     if (responseID == 1)
//     {
//         return eleInfo.setVector(this->getResistingForce());
//     }

//     else if (responseID == 2)
//     {
//         // Axial
//         N = q(0);
//         P(6) =  N;
//         P(0) = -N + p0[0];

//         // Torsion
//         T = q(5);
//         P(9) =  T;
//         P(3) = -T;

//         // Moments about z and shears along y
//         M1 = q(1);
//         M2 = q(2);
//         P(5)  = M1;
//         P(11) = M2;
//         V = (M1 + M2) * oneOverL;
//         P(1) =  V + p0[1];
//         P(7) = -V + p0[2];

//         // Moments about y and shears along z
//         M1 = q(3);
//         M2 = q(4);
//         P(4)  = M1;
//         P(10) = M2;
//         V = -(M1 + M2) * oneOverL;
//         P(2) = -V + p0[3];
//         P(8) =  V + p0[4];

//         return eleInfo.setVector(P);
//     }

//     // Chord rotation
//     else if (responseID == 3)
//     {
//         return eleInfo.setVector(this->getBasicTrialDisp());
//     }

//     // Plastic rotation
//     else if (responseID == 4)
//     {
//         static Vector vp(6);
//         static Vector ve(6);
//         const Matrix& kb = this->getInitialBasicStiff();
//         kb.Solve(q, ve);
//         vp = this->getBasicTrialDisp();
//         vp -= ve;
//         return eleInfo.setVector(vp);
//     }

//     else
//     {
//         return -1;
//     }
// }




//====================================================================
// Nima Tafazzoli (Nov. 2012) Moving from transformation ro element

int
DispBeamColumn3d::initialize()
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
                nodeIInitialDisp = new double [6];

                for (int j = 0; j < 6; j++)
                {
                    nodeIInitialDisp[j] = nodeIDisp(j);
                }

                i = 6;
            }

        for (int j = 0; j < 6; j++)
            if (nodeJDisp(j) != 0.0)
            {
                nodeJInitialDisp = new double [6];

                for (int i = 0; i < 6; i++)
                {
                    nodeJInitialDisp[i] = nodeJDisp(i);
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
DispBeamColumn3d::computeElemtLengthAndOrient()
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
        dx(0) += nodeJOffset[0];
        dx(1) += nodeJOffset[1];
        dx(2) += nodeJOffset[2];
    }

    if (nodeIOffset != 0)
    {
        dx(0) -= nodeIOffset[0];
        dx(1) -= nodeIOffset[1];
        dx(2) -= nodeIOffset[2];
    }

    if (nodeIInitialDisp != 0)
    {
        dx(0) -= nodeIInitialDisp[0];
        dx(1) -= nodeIInitialDisp[1];
        dx(2) -= nodeIInitialDisp[2];
    }

    if (nodeJInitialDisp != 0)
    {
        dx(0) += nodeJInitialDisp[0];
        dx(1) += nodeJInitialDisp[1];
        dx(2) += nodeJInitialDisp[2];
    }

    // calculate the element length
    L = dx.Norm();

    if (L == 0.0)
    {
        cerr << "DispBeamColumn3d::computeElemtLengthAndOrien: 0 length\n";
        return -2;
    }

    // calculate the element local x axis components (direction cossines)
    // wrt to the global coordinates
    R[0][0] = dx(0) / L;
    R[0][1] = dx(1) / L;
    R[0][2] = dx(2) / L;

    return 0;
}




int
DispBeamColumn3d::getLocalAxes(Vector &XAxis, Vector &YAxis, Vector &ZAxis)
{
    // Compute y = v cross x
    // Note: v(i) is stored in R[2][i]
    static Vector vAxis(3);
    vAxis(0) = R[2][0];
    vAxis(1) = R[2][1];
    vAxis(2) = R[2][2];

    static Vector xAxis(3);
    xAxis(0) = R[0][0];
    xAxis(1) = R[0][1];
    xAxis(2) = R[0][2];
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
        cerr << "DispBeamColumn3d::getLocalAxes";
        cerr << "\nvector v that defines plane xz is parallel to x axis\n";
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
    R[1][0] = yAxis(0);
    R[1][1] = yAxis(1);
    R[1][2] = yAxis(2);

    R[2][0] = zAxis(0);
    R[2][1] = zAxis(1);
    R[2][2] = zAxis(2);

    return 0;
}




const Vector &
DispBeamColumn3d::getBasicTrialDisp (void)
{
    // determine global displacements
    const Vector &disp1 = theNodes[0]->getTrialDisp();
    const Vector &disp2 = theNodes[1]->getTrialDisp();

    static double ug[12];

    for (int i = 0; i < 6; i++)
    {
        ug[i]   = disp1(i);
        ug[i + 6] = disp2(i);
    }

    if (nodeIInitialDisp != 0)
    {
        for (int j = 0; j < 6; j++)
        {
            ug[j] -= nodeIInitialDisp[j];
        }
    }

    if (nodeJInitialDisp != 0)
    {
        for (int j = 0; j < 6; j++)
        {
            ug[j + 6] -= nodeJInitialDisp[j];
        }
    }

    double oneOverL = 1.0 / L;

    static Vector ub(6);

    static double ul[12];

    ul[0]  = R[0][0] * ug[0] + R[0][1] * ug[1] + R[0][2] * ug[2];
    ul[1]  = R[1][0] * ug[0] + R[1][1] * ug[1] + R[1][2] * ug[2];
    ul[2]  = R[2][0] * ug[0] + R[2][1] * ug[1] + R[2][2] * ug[2];

    ul[3]  = R[0][0] * ug[3] + R[0][1] * ug[4] + R[0][2] * ug[5];
    ul[4]  = R[1][0] * ug[3] + R[1][1] * ug[4] + R[1][2] * ug[5];
    ul[5]  = R[2][0] * ug[3] + R[2][1] * ug[4] + R[2][2] * ug[5];

    ul[6]  = R[0][0] * ug[6] + R[0][1] * ug[7] + R[0][2] * ug[8];
    ul[7]  = R[1][0] * ug[6] + R[1][1] * ug[7] + R[1][2] * ug[8];
    ul[8]  = R[2][0] * ug[6] + R[2][1] * ug[7] + R[2][2] * ug[8];

    ul[9]  = R[0][0] * ug[9] + R[0][1] * ug[10] + R[0][2] * ug[11];
    ul[10] = R[1][0] * ug[9] + R[1][1] * ug[10] + R[1][2] * ug[11];
    ul[11] = R[2][0] * ug[9] + R[2][1] * ug[10] + R[2][2] * ug[11];

    static double Wu[3];

    if (nodeIOffset)
    {
        Wu[0] =  nodeIOffset[2] * ug[4] - nodeIOffset[1] * ug[5];
        Wu[1] = -nodeIOffset[2] * ug[3] + nodeIOffset[0] * ug[5];
        Wu[2] =  nodeIOffset[1] * ug[3] - nodeIOffset[0] * ug[4];

        ul[0] += R[0][0] * Wu[0] + R[0][1] * Wu[1] + R[0][2] * Wu[2];
        ul[1] += R[1][0] * Wu[0] + R[1][1] * Wu[1] + R[1][2] * Wu[2];
        ul[2] += R[2][0] * Wu[0] + R[2][1] * Wu[1] + R[2][2] * Wu[2];
    }

    if (nodeJOffset)
    {
        Wu[0] =  nodeJOffset[2] * ug[10] - nodeJOffset[1] * ug[11];
        Wu[1] = -nodeJOffset[2] * ug[9]  + nodeJOffset[0] * ug[11];
        Wu[2] =  nodeJOffset[1] * ug[9]  - nodeJOffset[0] * ug[10];

        ul[6] += R[0][0] * Wu[0] + R[0][1] * Wu[1] + R[0][2] * Wu[2];
        ul[7] += R[1][0] * Wu[0] + R[1][1] * Wu[1] + R[1][2] * Wu[2];
        ul[8] += R[2][0] * Wu[0] + R[2][1] * Wu[1] + R[2][2] * Wu[2];
    }

    ub(0) = ul[6] - ul[0];
    double tmp;
    tmp = oneOverL * (ul[1] - ul[7]);
    ub(1) = ul[5] + tmp;
    ub(2) = ul[11] + tmp;
    tmp = oneOverL * (ul[8] - ul[2]);
    ub(3) = ul[4] + tmp;
    ub(4) = ul[10] + tmp;
    ub(5) = ul[9] - ul[3];

    return ub;
}




const Matrix &
DispBeamColumn3d::getGlobalStiffMatrix (const Matrix &KB, const Vector &pb)
{
    static Matrix kg(12, 12);   // Global stiffness for return
    static double kb[6][6];     // Basic stiffness
    static double kl[12][12];   // Local stiffness
    static double tmp[12][12];  // Temporary storage

    double oneOverL = 1.0 / L;

    int i, j;

    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++)
        {
            kb[i][j] = KB(i, j);
        }

    // Transform basic stiffness to local system
    // First compute kb*T_{bl}
    for (i = 0; i < 6; i++)
    {
        tmp[i][0]  = -kb[i][0];
        tmp[i][1]  =  oneOverL * (kb[i][1] + kb[i][2]);
        tmp[i][2]  = -oneOverL * (kb[i][3] + kb[i][4]);
        tmp[i][3]  = -kb[i][5];
        tmp[i][4]  =  kb[i][3];
        tmp[i][5]  =  kb[i][1];
        tmp[i][6]  =  kb[i][0];
        tmp[i][7]  = -tmp[i][1];
        tmp[i][8]  = -tmp[i][2];
        tmp[i][9]  =  kb[i][5];
        tmp[i][10] =  kb[i][4];
        tmp[i][11] =  kb[i][2];
    }

    // Now compute T'_{bl}*(kb*T_{bl})
    for (i = 0; i < 12; i++)
    {
        kl[0][i]  = -tmp[0][i];
        kl[1][i]  =  oneOverL * (tmp[1][i] + tmp[2][i]);
        kl[2][i]  = -oneOverL * (tmp[3][i] + tmp[4][i]);
        kl[3][i]  = -tmp[5][i];
        kl[4][i]  =  tmp[3][i];
        kl[5][i]  =  tmp[1][i];
        kl[6][i]  =  tmp[0][i];
        kl[7][i]  = -kl[1][i];
        kl[8][i]  = -kl[2][i];
        kl[9][i]  =  tmp[5][i];
        kl[10][i] =  tmp[4][i];
        kl[11][i] =  tmp[2][i];
    }

    static double RWI[3][3];

    if (nodeIOffset)
    {
        // Compute RWI
        RWI[0][0] = -R[0][1] * nodeIOffset[2] + R[0][2] * nodeIOffset[1];
        RWI[1][0] = -R[1][1] * nodeIOffset[2] + R[1][2] * nodeIOffset[1];
        RWI[2][0] = -R[2][1] * nodeIOffset[2] + R[2][2] * nodeIOffset[1];

        RWI[0][1] =  R[0][0] * nodeIOffset[2] - R[0][2] * nodeIOffset[0];
        RWI[1][1] =  R[1][0] * nodeIOffset[2] - R[1][2] * nodeIOffset[0];
        RWI[2][1] =  R[2][0] * nodeIOffset[2] - R[2][2] * nodeIOffset[0];

        RWI[0][2] = -R[0][0] * nodeIOffset[1] + R[0][1] * nodeIOffset[0];
        RWI[1][2] = -R[1][0] * nodeIOffset[1] + R[1][1] * nodeIOffset[0];
        RWI[2][2] = -R[2][0] * nodeIOffset[1] + R[2][1] * nodeIOffset[0];
    }

    static double RWJ[3][3];

    if (nodeJOffset)
    {
        // Compute RWJ
        RWJ[0][0] = -R[0][1] * nodeJOffset[2] + R[0][2] * nodeJOffset[1];
        RWJ[1][0] = -R[1][1] * nodeJOffset[2] + R[1][2] * nodeJOffset[1];
        RWJ[2][0] = -R[2][1] * nodeJOffset[2] + R[2][2] * nodeJOffset[1];

        RWJ[0][1] =  R[0][0] * nodeJOffset[2] - R[0][2] * nodeJOffset[0];
        RWJ[1][1] =  R[1][0] * nodeJOffset[2] - R[1][2] * nodeJOffset[0];
        RWJ[2][1] =  R[2][0] * nodeJOffset[2] - R[2][2] * nodeJOffset[0];

        RWJ[0][2] = -R[0][0] * nodeJOffset[1] + R[0][1] * nodeJOffset[0];
        RWJ[1][2] = -R[1][0] * nodeJOffset[1] + R[1][1] * nodeJOffset[0];
        RWJ[2][2] = -R[2][0] * nodeJOffset[1] + R[2][1] * nodeJOffset[0];
    }

    // Transform local stiffness to global system
    // First compute kl*T_{lg}
    int m;

    for (m = 0; m < 12; m++)
    {
        tmp[m][0] = kl[m][0] * R[0][0] + kl[m][1] * R[1][0]  + kl[m][2] * R[2][0];
        tmp[m][1] = kl[m][0] * R[0][1] + kl[m][1] * R[1][1]  + kl[m][2] * R[2][1];
        tmp[m][2] = kl[m][0] * R[0][2] + kl[m][1] * R[1][2]  + kl[m][2] * R[2][2];

        tmp[m][3] = kl[m][3] * R[0][0] + kl[m][4] * R[1][0]  + kl[m][5] * R[2][0];
        tmp[m][4] = kl[m][3] * R[0][1] + kl[m][4] * R[1][1]  + kl[m][5] * R[2][1];
        tmp[m][5] = kl[m][3] * R[0][2] + kl[m][4] * R[1][2]  + kl[m][5] * R[2][2];

        if (nodeIOffset)
        {
            tmp[m][3]  += kl[m][0] * RWI[0][0]  + kl[m][1] * RWI[1][0]  + kl[m][2] * RWI[2][0];
            tmp[m][4]  += kl[m][0] * RWI[0][1]  + kl[m][1] * RWI[1][1]  + kl[m][2] * RWI[2][1];
            tmp[m][5]  += kl[m][0] * RWI[0][2]  + kl[m][1] * RWI[1][2]  + kl[m][2] * RWI[2][2];
        }

        tmp[m][6] = kl[m][6] * R[0][0] + kl[m][7] * R[1][0]  + kl[m][8] * R[2][0];
        tmp[m][7] = kl[m][6] * R[0][1] + kl[m][7] * R[1][1]  + kl[m][8] * R[2][1];
        tmp[m][8] = kl[m][6] * R[0][2] + kl[m][7] * R[1][2]  + kl[m][8] * R[2][2];

        tmp[m][9]  = kl[m][9] * R[0][0] + kl[m][10] * R[1][0] + kl[m][11] * R[2][0];
        tmp[m][10] = kl[m][9] * R[0][1] + kl[m][10] * R[1][1] + kl[m][11] * R[2][1];
        tmp[m][11] = kl[m][9] * R[0][2] + kl[m][10] * R[1][2] + kl[m][11] * R[2][2];

        if (nodeJOffset)
        {
            tmp[m][9]   += kl[m][6] * RWJ[0][0]  + kl[m][7] * RWJ[1][0]  + kl[m][8] * RWJ[2][0];
            tmp[m][10]  += kl[m][6] * RWJ[0][1]  + kl[m][7] * RWJ[1][1]  + kl[m][8] * RWJ[2][1];
            tmp[m][11]  += kl[m][6] * RWJ[0][2]  + kl[m][7] * RWJ[1][2]  + kl[m][8] * RWJ[2][2];
        }

    }

    // Now compute T'_{lg}*(kl*T_{lg})
    for (m = 0; m < 12; m++)
    {
        kg(0, m) = R[0][0] * tmp[0][m] + R[1][0] * tmp[1][m]  + R[2][0] * tmp[2][m];
        kg(1, m) = R[0][1] * tmp[0][m] + R[1][1] * tmp[1][m]  + R[2][1] * tmp[2][m];
        kg(2, m) = R[0][2] * tmp[0][m] + R[1][2] * tmp[1][m]  + R[2][2] * tmp[2][m];

        kg(3, m) = R[0][0] * tmp[3][m] + R[1][0] * tmp[4][m]  + R[2][0] * tmp[5][m];
        kg(4, m) = R[0][1] * tmp[3][m] + R[1][1] * tmp[4][m]  + R[2][1] * tmp[5][m];
        kg(5, m) = R[0][2] * tmp[3][m] + R[1][2] * tmp[4][m]  + R[2][2] * tmp[5][m];

        if (nodeIOffset)
        {
            kg(3, m) += RWI[0][0] * tmp[0][m]  + RWI[1][0] * tmp[1][m] + RWI[2][0] * tmp[2][m];
            kg(4, m) += RWI[0][1] * tmp[0][m]  + RWI[1][1] * tmp[1][m] + RWI[2][1] * tmp[2][m];
            kg(5, m) += RWI[0][2] * tmp[0][m]  + RWI[1][2] * tmp[1][m] + RWI[2][2] * tmp[2][m];
        }

        kg(6, m) = R[0][0] * tmp[6][m] + R[1][0] * tmp[7][m]  + R[2][0] * tmp[8][m];
        kg(7, m) = R[0][1] * tmp[6][m] + R[1][1] * tmp[7][m]  + R[2][1] * tmp[8][m];
        kg(8, m) = R[0][2] * tmp[6][m] + R[1][2] * tmp[7][m]  + R[2][2] * tmp[8][m];

        kg(9, m)  = R[0][0] * tmp[9][m] + R[1][0] * tmp[10][m] + R[2][0] * tmp[11][m];
        kg(10, m) = R[0][1] * tmp[9][m] + R[1][1] * tmp[10][m] + R[2][1] * tmp[11][m];
        kg(11, m) = R[0][2] * tmp[9][m] + R[1][2] * tmp[10][m] + R[2][2] * tmp[11][m];

        if (nodeJOffset)
        {
            kg(9, m)  += RWJ[0][0] * tmp[6][m]  + RWJ[1][0] * tmp[7][m] + RWJ[2][0] * tmp[8][m];
            kg(10, m) += RWJ[0][1] * tmp[6][m]  + RWJ[1][1] * tmp[7][m] + RWJ[2][1] * tmp[8][m];
            kg(11, m) += RWJ[0][2] * tmp[6][m]  + RWJ[1][2] * tmp[7][m] + RWJ[2][2] * tmp[8][m];
        }
    }

    return kg;
}





const Matrix &
DispBeamColumn3d::getInitialGlobalStiffMatrix (const Matrix &KB)
{
    static Matrix kg(12, 12); // Global stiffness for return
    static double kb[6][6];   // Basic stiffness
    static double kl[12][12]; // Local stiffness
    static double tmp[12][12];    // Temporary storage

    double oneOverL = 1.0 / L;

    int i, j;

    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++)
        {
            kb[i][j] = KB(i, j);
        }

    // Transform basic stiffness to local system
    // First compute kb*T_{bl}
    for (i = 0; i < 6; i++)
    {
        tmp[i][0]  = -kb[i][0];
        tmp[i][1]  =  oneOverL * (kb[i][1] + kb[i][2]);
        tmp[i][2]  = -oneOverL * (kb[i][3] + kb[i][4]);
        tmp[i][3]  = -kb[i][5];
        tmp[i][4]  =  kb[i][3];
        tmp[i][5]  =  kb[i][1];
        tmp[i][6]  =  kb[i][0];
        tmp[i][7]  = -tmp[i][1];
        tmp[i][8]  = -tmp[i][2];
        tmp[i][9]  =  kb[i][5];
        tmp[i][10] =  kb[i][4];
        tmp[i][11] =  kb[i][2];
    }

    // Now compute T'_{bl}*(kb*T_{bl})
    for (i = 0; i < 12; i++)
    {
        kl[0][i]  = -tmp[0][i];
        kl[1][i]  =  oneOverL * (tmp[1][i] + tmp[2][i]);
        kl[2][i]  = -oneOverL * (tmp[3][i] + tmp[4][i]);
        kl[3][i]  = -tmp[5][i];
        kl[4][i]  =  tmp[3][i];
        kl[5][i]  =  tmp[1][i];
        kl[6][i]  =  tmp[0][i];
        kl[7][i]  = -kl[1][i];
        kl[8][i]  = -kl[2][i];
        kl[9][i]  =  tmp[5][i];
        kl[10][i] =  tmp[4][i];
        kl[11][i] =  tmp[2][i];
    }

    static double RWI[3][3];

    if (nodeIOffset)
    {
        // Compute RWI
        RWI[0][0] = -R[0][1] * nodeIOffset[2] + R[0][2] * nodeIOffset[1];
        RWI[1][0] = -R[1][1] * nodeIOffset[2] + R[1][2] * nodeIOffset[1];
        RWI[2][0] = -R[2][1] * nodeIOffset[2] + R[2][2] * nodeIOffset[1];

        RWI[0][1] =  R[0][0] * nodeIOffset[2] - R[0][2] * nodeIOffset[0];
        RWI[1][1] =  R[1][0] * nodeIOffset[2] - R[1][2] * nodeIOffset[0];
        RWI[2][1] =  R[2][0] * nodeIOffset[2] - R[2][2] * nodeIOffset[0];

        RWI[0][2] = -R[0][0] * nodeIOffset[1] + R[0][1] * nodeIOffset[0];
        RWI[1][2] = -R[1][0] * nodeIOffset[1] + R[1][1] * nodeIOffset[0];
        RWI[2][2] = -R[2][0] * nodeIOffset[1] + R[2][1] * nodeIOffset[0];
    }

    static double RWJ[3][3];

    if (nodeJOffset)
    {
        // Compute RWJ
        RWJ[0][0] = -R[0][1] * nodeJOffset[2] + R[0][2] * nodeJOffset[1];
        RWJ[1][0] = -R[1][1] * nodeJOffset[2] + R[1][2] * nodeJOffset[1];
        RWJ[2][0] = -R[2][1] * nodeJOffset[2] + R[2][2] * nodeJOffset[1];

        RWJ[0][1] =  R[0][0] * nodeJOffset[2] - R[0][2] * nodeJOffset[0];
        RWJ[1][1] =  R[1][0] * nodeJOffset[2] - R[1][2] * nodeJOffset[0];
        RWJ[2][1] =  R[2][0] * nodeJOffset[2] - R[2][2] * nodeJOffset[0];

        RWJ[0][2] = -R[0][0] * nodeJOffset[1] + R[0][1] * nodeJOffset[0];
        RWJ[1][2] = -R[1][0] * nodeJOffset[1] + R[1][1] * nodeJOffset[0];
        RWJ[2][2] = -R[2][0] * nodeJOffset[1] + R[2][1] * nodeJOffset[0];
    }

    // Transform local stiffness to global system
    // First compute kl*T_{lg}
    int m;

    for (m = 0; m < 12; m++)
    {
        tmp[m][0] = kl[m][0] * R[0][0] + kl[m][1] * R[1][0]  + kl[m][2] * R[2][0];
        tmp[m][1] = kl[m][0] * R[0][1] + kl[m][1] * R[1][1]  + kl[m][2] * R[2][1];
        tmp[m][2] = kl[m][0] * R[0][2] + kl[m][1] * R[1][2]  + kl[m][2] * R[2][2];

        tmp[m][3] = kl[m][3] * R[0][0] + kl[m][4] * R[1][0]  + kl[m][5] * R[2][0];
        tmp[m][4] = kl[m][3] * R[0][1] + kl[m][4] * R[1][1]  + kl[m][5] * R[2][1];
        tmp[m][5] = kl[m][3] * R[0][2] + kl[m][4] * R[1][2]  + kl[m][5] * R[2][2];

        if (nodeIOffset)
        {
            tmp[m][3]  += kl[m][0] * RWI[0][0]  + kl[m][1] * RWI[1][0]  + kl[m][2] * RWI[2][0];
            tmp[m][4]  += kl[m][0] * RWI[0][1]  + kl[m][1] * RWI[1][1]  + kl[m][2] * RWI[2][1];
            tmp[m][5]  += kl[m][0] * RWI[0][2]  + kl[m][1] * RWI[1][2]  + kl[m][2] * RWI[2][2];
        }

        tmp[m][6] = kl[m][6] * R[0][0] + kl[m][7] * R[1][0]  + kl[m][8] * R[2][0];
        tmp[m][7] = kl[m][6] * R[0][1] + kl[m][7] * R[1][1]  + kl[m][8] * R[2][1];
        tmp[m][8] = kl[m][6] * R[0][2] + kl[m][7] * R[1][2]  + kl[m][8] * R[2][2];

        tmp[m][9]  = kl[m][9] * R[0][0] + kl[m][10] * R[1][0] + kl[m][11] * R[2][0];
        tmp[m][10] = kl[m][9] * R[0][1] + kl[m][10] * R[1][1] + kl[m][11] * R[2][1];
        tmp[m][11] = kl[m][9] * R[0][2] + kl[m][10] * R[1][2] + kl[m][11] * R[2][2];

        if (nodeJOffset)
        {
            tmp[m][9]   += kl[m][6] * RWJ[0][0]  + kl[m][7] * RWJ[1][0]  + kl[m][8] * RWJ[2][0];
            tmp[m][10]  += kl[m][6] * RWJ[0][1]  + kl[m][7] * RWJ[1][1]  + kl[m][8] * RWJ[2][1];
            tmp[m][11]  += kl[m][6] * RWJ[0][2]  + kl[m][7] * RWJ[1][2]  + kl[m][8] * RWJ[2][2];
        }

    }

    // Now compute T'_{lg}*(kl*T_{lg})
    for (m = 0; m < 12; m++)
    {
        kg(0, m) = R[0][0] * tmp[0][m] + R[1][0] * tmp[1][m]  + R[2][0] * tmp[2][m];
        kg(1, m) = R[0][1] * tmp[0][m] + R[1][1] * tmp[1][m]  + R[2][1] * tmp[2][m];
        kg(2, m) = R[0][2] * tmp[0][m] + R[1][2] * tmp[1][m]  + R[2][2] * tmp[2][m];

        kg(3, m) = R[0][0] * tmp[3][m] + R[1][0] * tmp[4][m]  + R[2][0] * tmp[5][m];
        kg(4, m) = R[0][1] * tmp[3][m] + R[1][1] * tmp[4][m]  + R[2][1] * tmp[5][m];
        kg(5, m) = R[0][2] * tmp[3][m] + R[1][2] * tmp[4][m]  + R[2][2] * tmp[5][m];

        if (nodeIOffset)
        {
            kg(3, m) += RWI[0][0] * tmp[0][m]  + RWI[1][0] * tmp[1][m] + RWI[2][0] * tmp[2][m];
            kg(4, m) += RWI[0][1] * tmp[0][m]  + RWI[1][1] * tmp[1][m] + RWI[2][1] * tmp[2][m];
            kg(5, m) += RWI[0][2] * tmp[0][m]  + RWI[1][2] * tmp[1][m] + RWI[2][2] * tmp[2][m];
        }

        kg(6, m) = R[0][0] * tmp[6][m] + R[1][0] * tmp[7][m]  + R[2][0] * tmp[8][m];
        kg(7, m) = R[0][1] * tmp[6][m] + R[1][1] * tmp[7][m]  + R[2][1] * tmp[8][m];
        kg(8, m) = R[0][2] * tmp[6][m] + R[1][2] * tmp[7][m]  + R[2][2] * tmp[8][m];

        kg(9, m)  = R[0][0] * tmp[9][m] + R[1][0] * tmp[10][m] + R[2][0] * tmp[11][m];
        kg(10, m) = R[0][1] * tmp[9][m] + R[1][1] * tmp[10][m] + R[2][1] * tmp[11][m];
        kg(11, m) = R[0][2] * tmp[9][m] + R[1][2] * tmp[10][m] + R[2][2] * tmp[11][m];

        if (nodeJOffset)
        {
            kg(9, m)  += RWJ[0][0] * tmp[6][m]  + RWJ[1][0] * tmp[7][m] + RWJ[2][0] * tmp[8][m];
            kg(10, m) += RWJ[0][1] * tmp[6][m]  + RWJ[1][1] * tmp[7][m] + RWJ[2][1] * tmp[8][m];
            kg(11, m) += RWJ[0][2] * tmp[6][m]  + RWJ[1][2] * tmp[7][m] + RWJ[2][2] * tmp[8][m];
        }
    }

    return kg;
}






const Vector &
DispBeamColumn3d::getGlobalResistingForce(const Vector &pb, const Vector &p0)
{
    // transform resisting forces from the basic system to local coordinates
    static double pl[12];

    double q0 = pb(0);
    double q1 = pb(1);
    double q2 = pb(2);
    double q3 = pb(3);
    double q4 = pb(4);
    double q5 = pb(5);

    double oneOverL = 1.0 / L;

    pl[0]  = -q0;
    pl[1]  =  oneOverL * (q1 + q2);
    pl[2]  = -oneOverL * (q3 + q4);
    pl[3]  = -q5;
    pl[4]  =  q3;
    pl[5]  =  q1;
    pl[6]  =  q0;
    pl[7]  = -pl[1];
    pl[8]  = -pl[2];
    pl[9]  =  q5;
    pl[10] =  q4;
    pl[11] =  q2;

    pl[0] += p0(0);
    pl[1] += p0(1);
    pl[7] += p0(2);
    pl[2] += p0(3);
    pl[8] += p0(4);

    // transform resisting forces  from local to global coordinates
    static Vector pg(12);

    pg(0)  = R[0][0] * pl[0] + R[1][0] * pl[1] + R[2][0] * pl[2];
    pg(1)  = R[0][1] * pl[0] + R[1][1] * pl[1] + R[2][1] * pl[2];
    pg(2)  = R[0][2] * pl[0] + R[1][2] * pl[1] + R[2][2] * pl[2];

    pg(3)  = R[0][0] * pl[3] + R[1][0] * pl[4] + R[2][0] * pl[5];
    pg(4)  = R[0][1] * pl[3] + R[1][1] * pl[4] + R[2][1] * pl[5];
    pg(5)  = R[0][2] * pl[3] + R[1][2] * pl[4] + R[2][2] * pl[5];

    pg(6)  = R[0][0] * pl[6] + R[1][0] * pl[7] + R[2][0] * pl[8];
    pg(7)  = R[0][1] * pl[6] + R[1][1] * pl[7] + R[2][1] * pl[8];
    pg(8)  = R[0][2] * pl[6] + R[1][2] * pl[7] + R[2][2] * pl[8];

    pg(9)  = R[0][0] * pl[9] + R[1][0] * pl[10] + R[2][0] * pl[11];
    pg(10) = R[0][1] * pl[9] + R[1][1] * pl[10] + R[2][1] * pl[11];
    pg(11) = R[0][2] * pl[9] + R[1][2] * pl[10] + R[2][2] * pl[11];

    if (nodeIOffset)
    {
        pg(3) += -nodeIOffset[2] * pg(1) + nodeIOffset[1] * pg(2);
        pg(4) +=  nodeIOffset[2] * pg(0) - nodeIOffset[0] * pg(2);
        pg(5) += -nodeIOffset[1] * pg(0) + nodeIOffset[0] * pg(1);
    }

    if (nodeJOffset)
    {
        pg(9)  += -nodeJOffset[2] * pg(7) + nodeJOffset[1] * pg(8);
        pg(10) +=  nodeJOffset[2] * pg(6) - nodeJOffset[0] * pg(8);
        pg(11) += -nodeJOffset[1] * pg(6) + nodeJOffset[0] * pg(7);
    }

    return pg;
}



Vector *
DispBeamColumn3d::getForce(void)
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




