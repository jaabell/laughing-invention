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

// $Revision: 1.24 $
// $Date: 2006/03/21 22:19:12 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/truss/Truss.cpp,v $


// File: ~/element/truss/Truss.C
//
// Written: fmk
// Created: 07/98
// Revision: A
//
// Description: This file contains the implementation for the Truss class.
//
// What: "@(#) Truss.C, revA"

#include "Truss.h"
#include <Information.h>

#include <Domain.h>
#include <Node.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <UniaxialMaterial.h>
// #include <Renderer.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <ElementResponse.h>

//#include <fstream>

// initialise the class wide variables
Matrix Truss::trussM6(6, 6);
Matrix Truss::trussM12(12, 12);
Vector Truss::trussV6(6);
Vector Truss::trussV12(12);

// constructor:
//  responsible for allocating the necessary space needed by each object
//  and storing the tags of the truss end nodes.
Truss::Truss(int tag,
             int Nd1, int Nd2,
             UniaxialMaterial& theMat,
             double a, double r)
    : Element(tag, ELE_TAG_Truss),
      theMaterial(0), connectedExternalNodes(2),
      numDOF(0), theLoad(0),
      theMatrix(0), theVector(0),
      L(0.0), A(a), rho(r)
{
    // get a copy of the material and check we obtained a valid copy
    theMaterial = theMat.getCopy();

    if (theMaterial == 0)
    {
        std::cerr << "FATAL Truss::Truss - " << tag <<
                  "failed to get a copy of material with tag " << theMat.getTag() << endln;
        exit(-1);
    }

    // ensure the connectedExternalNode ID is of correct size & set values
    if (connectedExternalNodes.Size() != 2)
    {
        std::cerr << "FATAL Truss::Truss - " <<  tag << "failed to create an ID of size 2\n";
        exit(-1);
    }

    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;

    // set node pointers to NULL
    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }

    cosX[0] = 0.0;
    cosX[1] = 0.0;
    cosX[2] = 0.0;
}

// constructor:
//   invoked by a FEM_ObjectBroker - blank object that recvSelf needs
//   to be invoked upon
Truss::Truss()
    : Element(0, ELE_TAG_Truss),
      theMaterial(0), connectedExternalNodes(2),
      numDOF(0), theLoad(0),
      theMatrix(0), theVector(0),
      L(0.0), A(0.0), rho(0.0)
{
    // ensure the connectedExternalNode ID is of correct size
    if (connectedExternalNodes.Size() != 2)
    {
        std::cerr << "FATAL Truss::Truss - failed to create an ID of size 2\n";
        exit(-1);
    }

    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }

    cosX[0] = 0.0;
    cosX[1] = 0.0;
    cosX[2] = 0.0;

}

//  destructor
//     delete must be invoked on any objects created by the object
//     and on the matertial object.
Truss::~Truss()
{
    // invoke the destructor on any objects created by the object
    // that the object still holds a pointer to
    if (theMaterial != 0)
    {
        delete theMaterial;
    }

    if (theLoad != 0)
    {
        delete theLoad;
    }
}


int
Truss::getNumExternalNodes(void) const
{
    return 2;
}

const ID&
Truss::getExternalNodes(void)
{
    return connectedExternalNodes;
}

Node**
Truss::getNodePtrs(void)
{
    return theNodes;
}

int
Truss::getNumDOF(void)
{
    return numDOF;
}


// method: setDomain()
//    to set a link to the enclosing Domain and to set the node pointers.
//    also determines the number of dof associated
//    with the truss element, we set matrix and vector pointers,
//    allocate space for t matrix, determine the length
//    and set the transformation matrix.
void
Truss::setDomain(Domain* theDomain)
{
    // check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
        L = 0;
        return;
    }

    // first set the node pointers
    int Nd1 = connectedExternalNodes(0);
    int Nd2 = connectedExternalNodes(1);
    theNodes[0] = theDomain->getNode(Nd1);
    theNodes[1] = theDomain->getNode(Nd2);

    // if can't find both - send a warning message
    if ((theNodes[0] == 0) || (theNodes[1] == 0))
    {
        if (theNodes[0] == 0)
            std::cerr << "Truss::setDomain() - truss" << this->getTag() << " node " << Nd1 <<
                      "does not exist in the model\n";
        else
            std::cerr << "Truss::setDomain() - truss" << this->getTag() << " node " << Nd2 <<
                      "does not exist in the model\n";

        exit(-1);

    }

    // now determine the number of dof and the dimesnion
    int dofNd1 = theNodes[0]->getNumberDOF();
    int dofNd2 = theNodes[1]->getNumberDOF();

    // if differing dof at the ends - print a warning message
    if (dofNd1 != dofNd2)
    {
        std::cerr << "WARNING Truss::setDomain(): nodes " << Nd1 << " and " << Nd2 <<
                  "have differing dof at ends for truss " << this->getTag() << endln;


        exit(-1);

    }

    // call the base class method
    this->DomainComponent::setDomain(theDomain);

    // now set the number of dof for element and set matrix and vector pointer
    if (dofNd1 == 3)
    {
        numDOF = 6;
        theMatrix = &trussM6;
        theVector = &trussV6;
    }
    else if (dofNd1 == 6)
    {
        numDOF = 12;
        theMatrix = &trussM12;
        theVector = &trussV12;
    }
    else
    {
        std::cerr << "WARNING Truss::setDomain cannot handle dofs at nodes in " <<
                  dofNd1  << " problem\n";
        exit(-1);
    }

    if (theLoad == 0)
    {
        theLoad = new Vector(numDOF);
    }
    else if (theLoad->Size() != numDOF)
    {
        delete theLoad;
        theLoad = new Vector(numDOF);
    }

    if (theLoad == 0)
    {
        std::cerr << "Truss::setDomain - truss " << this->getTag() <<
                  "out of memory creating vector of size" << numDOF << endln;
        exit(-1);
        return;
    }

    // now determine the length, cosines and fill in the transformation
    // NOTE t = -t(every one else uses for residual calc)
    const Vector& end1Crd = theNodes[0]->getCrds();
    const Vector& end2Crd = theNodes[1]->getCrds();



    double dx = end2Crd(0) - end1Crd(0);
    double dy = end2Crd(1) - end1Crd(1);
    double dz = end2Crd(2) - end1Crd(2);

    L = sqrt(dx * dx + dy * dy + dz * dz);

    if (L == 0.0)
    {
        std::cerr << "WARNING Truss::setDomain() - truss " << this->getTag() << " has zero length\n";
        return;
    }

    cosX[0] = dx / L;
    cosX[1] = dy / L;
    cosX[2] = dz / L;

}


int
Truss::commitState()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        std::cerr << "Truss::commitState () - failed in base class";
    }

    retVal = theMaterial->commitState();
    return retVal;
}

int
Truss::revertToLastCommit()
{
    return theMaterial->revertToLastCommit();
}

int
Truss::revertToStart()
{
    return theMaterial->revertToStart();
}

int
Truss::update(void)
{
    // determine the current strain given trial displacements at nodes
    double strain = this->computeCurrentStrain();
    double rate = this->computeCurrentStrainRate();
    return theMaterial->setTrialStrain(strain, rate);
}


const Matrix&
Truss::getTangentStiff(void)
{
    if (L == 0.0)   // - problem in setDomain() no further warnings
    {
        theMatrix->Zero();
        return *theMatrix;
    }

    double E = theMaterial->getTangent();

    // come back later and redo this if too slow
    Matrix& stiff = *theMatrix;

    int numDOF2 = numDOF / 2;
    double temp;
    double EAoverL = E * A / L;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp = cosX[i] * cosX[j] * EAoverL;
            stiff(i, j) = temp;
            stiff(i + numDOF2, j) = -temp;
            stiff(i, j + numDOF2) = -temp;
            stiff(i + numDOF2, j + numDOF2) = temp;
        }
    }



    return stiff;
}


const Matrix&
Truss::getInitialStiff(void)
{
    if (L == 0.0)   // - problem in setDomain() no further warnings
    {
        theMatrix->Zero();
        return *theMatrix;
    }

    double E = theMaterial->getInitialTangent();

    // come back later and redo this if too slow
    Matrix& stiff = *theMatrix;

    int numDOF2 = numDOF / 2;
    double temp;
    double EAoverL = E * A / L;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp = cosX[i] * cosX[j] * EAoverL;
            stiff(i, j) = temp;
            stiff(i + numDOF2, j) = -temp;
            stiff(i, j + numDOF2) = -temp;
            stiff(i + numDOF2, j + numDOF2) = temp;
        }
    }

    return *theMatrix;
}

const Matrix&
Truss::getDamp(void)
{
    if (L == 0.0)   // - problem in setDomain() no further warnings
    {
        theMatrix->Zero();
        return *theMatrix;
    }

    double eta = theMaterial->getDampTangent();

    // come back later and redo this if too slow
    Matrix& damp = *theMatrix;

    int numDOF2 = numDOF / 2;
    double temp;
    double etaAoverL = eta * A / L;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp = cosX[i] * cosX[j] * etaAoverL;
            damp(i, j) = temp;
            damp(i + numDOF2, j) = -temp;
            damp(i, j + numDOF2) = -temp;
            damp(i + numDOF2, j + numDOF2) = temp;
        }
    }

    return damp;
}


const Matrix&
Truss::getMass(void)
{
    // zero the matrix
    Matrix& mass = *theMatrix;
    mass.Zero();

    // check for quick return
    if (L == 0.0 || rho == 0.0)   // - problem in setDomain() no further warnings
    {
        return mass;
    }

    double M = 0.5 * rho * L;
    int numDOF2 = numDOF / 2;

    for (int i = 0; i < 3; i++)
    {
        mass(i, i) = M;
        mass(i + numDOF2, i + numDOF2) = M;
    }

    return mass;
}

void
Truss::zeroLoad(void)
{
    theLoad->Zero();
}

int
Truss::addLoad(ElementalLoad* theLoad, double loadFactor)

{
    std::cerr << "Truss::addLoad - load type unknown for truss with tag: " << this->getTag() << endln;

    return -1;
}

int
Truss::addInertiaLoadToUnbalance(const Vector& accel)
{
    // check for a quick return
    if (L == 0.0 || rho == 0.0)
    {
        return 0;
    }

    // get R * accel from the nodes
    const Vector& Raccel1 = theNodes[0]->getRV(accel);
    const Vector& Raccel2 = theNodes[1]->getRV(accel);

    int nodalDOF = numDOF / 2;

    #ifdef _G3DEBUG

    if (nodalDOF != Raccel1.Size() || nodalDOF != Raccel2.Size())
    {
        std::cerr << "Truss::addInertiaLoadToUnbalance " <<
                  "matrix and vector sizes are incompatable\n";
        return -1;
    }

    #endif

    double M = 0.5 * rho * L;

    // want to add ( - fact * M R * accel ) to unbalance
    for (int i = 0; i < 3; i++)
    {
        double val1 = Raccel1(i);
        double val2 = Raccel2(i);

        // perform - fact * M*(R * accel) // remember M a diagonal matrix
        val1 *= -M;
        val2 *= -M;

        (*theLoad)(i) += val1;
        (*theLoad)(i + nodalDOF) += val2;
    }

    return 0;
}





const Vector&
Truss::getResistingForce()
{
    if (L == 0.0)   // - problem in setDomain() no further warnings
    {
        theVector->Zero();
        return *theVector;
    }

    // R = Ku - Pext
    // Ku = F * transformation
    double force = A * theMaterial->getStress();
    int numDOF2 = numDOF / 2;
    double temp;

    for (int i = 0; i < 3; i++)
    {
        temp = cosX[i] * force;
        (*theVector)(i) = -temp;
        (*theVector)(i + numDOF2) = temp;
    }

    // subtract external load:  Ku - P
    (*theVector) -= *theLoad;



    return *theVector;
}


const Vector&
Truss::getResistingForceIncInertia()
{
    this->getResistingForce();

    // now include the mass portion
    if (L != 0.0 && rho != 0.0)
    {

        const Vector& accel1 = theNodes[0]->getTrialAccel();
        const Vector& accel2 = theNodes[1]->getTrialAccel();

        int numDOF2 = numDOF / 2;
        double M = 0.5 * rho * L;

        for (int i = 0; i < 3; i++)
        {
            (*theVector)(i) += M * accel1(i);
            (*theVector)(i + numDOF2) += M * accel2(i);
        }

        // add the damping forces if rayleigh damping
        if (a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            (*theVector) += this->getRayleighDampingForces();
        }
    }
    else
    {

        // add the damping forces if rayleigh damping
        if (a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            (*theVector) += this->getRayleighDampingForces();
        }
    }

    return *theVector;
}

int
Truss::sendSelf(int commitTag, Channel& theChannel)
{
    int res;

    // note: we don't check for dataTag == 0 for Element
    // objects as that is taken care of in a commit by the Domain
    // object - don't want to have to do the check if sending data
    int dataTag = this->getDbTag();

    // truss packs it's data into a Vector and sends this to theChannel
    // along with it's dbTag and the commitTag passed in the arguments

    static Vector data(6);
    data(0) = this->getTag();
    data(1) = numDOF;
    data(2) = A;
    data(5) = rho;

    data(3) = theMaterial->getClassTag();
    int matDbTag = theMaterial->getDbTag();

    // NOTE: we do have to ensure that the material has a database
    // tag if we are sending to a database channel.
    if (matDbTag == 0)
    {
        matDbTag = theChannel.getDbTag();

        if (matDbTag != 0)
        {
            theMaterial->setDbTag(matDbTag);
        }
    }

    data(4) = matDbTag;

    res = theChannel.sendVector(dataTag, commitTag, data);

    if (res < 0)
    {
        std::cerr << "WARNING Truss::sendSelf() - " << this->getTag() << " failed to send Vector\n";
        return -1;
    }

    // truss then sends the tags of it's two end nodes

    res = theChannel.sendID(dataTag, commitTag, connectedExternalNodes);

    if (res < 0)
    {
        std::cerr << "WARNING Truss::sendSelf() - " << this->getTag() << " failed to send Vector\n";
        return -2;
    }

    // finally truss asks it's material object to send itself
    res = theMaterial->sendSelf(commitTag, theChannel);

    if (res < 0)
    {
        std::cerr << "WARNING Truss::sendSelf() - " << this->getTag() << " failed to send its Material\n";
        return -3;
    }

    return 0;
}

int
Truss::recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    int res;
    int dataTag = this->getDbTag();

    // truss creates a Vector, receives the Vector and then sets the
    // internal data with the data in the Vector

    static Vector data(6);
    res = theChannel.recvVector(dataTag, commitTag, data);

    if (res < 0)
    {
        std::cerr << "WARNING Truss::recvSelf() - failed to receive Vector\n";
        return -1;
    }

    this->setTag((int)data(0));
    numDOF = (int)data(1);
    A = data(2);
    rho = data(5);

    // truss now receives the tags of it's two external nodes
    res = theChannel.recvID(dataTag, commitTag, connectedExternalNodes);

    if (res < 0)
    {
        std::cerr << "WARNING Truss::recvSelf() - " << this->getTag() << " failed to receive ID\n";
        return -2;
    }

    // finally truss creates a material object of the correct type,
    // sets its database tag and asks this new object to recveive itself.

    int matClass = (int)data(3);
    int matDb = (int)data(4);

    // check if we have a material object already & if we do if of right type
    if ((theMaterial == 0) || (theMaterial->getClassTag() != matClass))
    {

        // if old one .. delete it
        if (theMaterial != 0)
        {
            delete theMaterial;
        }

        // create a new material object
        theMaterial = theBroker.getNewUniaxialMaterial(matClass);

        if (theMaterial == 0)
        {
            std::cerr << "WARNING Truss::recvSelf() - " << this->getTag()
                      << " failed to get a blank Material of type " << matClass << endln;
            return -3;
        }
    }

    theMaterial->setDbTag(matDb); // note: we set the dbTag before we receive the material
    res = theMaterial->recvSelf(commitTag, theChannel, theBroker);

    if (res < 0)
    {
        std::cerr << "WARNING Truss::recvSelf() - " << this->getTag() << "failed to receive its Material\n";
        return -3;
    }

    return 0;
}


// int
// Truss::displaySelf(Renderer &theViewer, int displayMode, float fact)
// {
//     // ensure setDomain() worked
//     if (L == 0.0)
//        return 0;
//
//     // first determine the two end points of the truss based on
//     // the display factor (a measure of the distorted image)
//     // store this information in 2 3d vectors v1 and v2
//     const Vector &end1Crd = theNodes[0]->getCrds();
//     const Vector &end2Crd = theNodes[1]->getCrds();
//
//     static Vector v1(3);
//     static Vector v2(3);
//
//     if (displayMode == 1 || displayMode == 2) {
//       const Vector &end1Disp = theNodes[0]->getDisp();
//       const Vector &end2Disp = theNodes[1]->getDisp();
//
//  for (int i=0; i<dimension; i++) {
//      v1(i) = end1Crd(i)+end1Disp(i)*fact;
//      v2(i) = end2Crd(i)+end2Disp(i)*fact;
//  }
//
//  // compute the strain and axial force in the member
//  double strain, force;
//  if (L == 0.0) {
//      strain = 0.0;
//      force = 0.0;
//  } else {
//      strain = this->computeCurrentStrain();
//      theMaterial->setTrialStrain(strain);
//      force = A*theMaterial->getStress();
//  }
//
//  if (displayMode == 2) // use the strain as the drawing measure
//    return theViewer.drawLine(v1, v2, strain, strain);
//  else { // otherwise use the axial force as measure
//    return theViewer.drawLine(v1,v2, force, force);
//  }
//     } else if (displayMode < 0) {
//       int mode = displayMode  *  -1;
//       const Matrix &eigen1 = theNodes[0]->getEigenvectors();
//       const Matrix &eigen2 = theNodes[1]->getEigenvectors();
//       if (eigen1.noCols() >= mode) {
//  for (int i = 0; i < dimension; i++) {
//    v1(i) = end1Crd(i) + eigen1(i,mode-1)*fact;
//    v2(i) = end2Crd(i) + eigen2(i,mode-1)*fact;
//  }
//       } else {
//  for (int i = 0; i < dimension; i++) {
//    v1(i) = end1Crd(i);
//    v2(i) = end2Crd(i);
//  }
//       }
//     }
//     return 0;
// }


void
Truss::Print(ostream& s, int flag)
{
    // compute the strain and axial force in the member
    double strain, force;
    strain = theMaterial->getStrain();
    force = A * theMaterial->getStress();

    if (flag == 0)   // print everything
    {
        s << "Element: " << this->getTag();
        s << " type: Truss  iNode: " << connectedExternalNodes(0);
        s << " jNode: " << connectedExternalNodes(1);
        s << " Area: " << A << " Mass/Length: " << rho;

        s << " \n\t strain: " << strain;
        s << " axial load: " <<  force;

        if (L != 0.0)
        {
            int numDOF2 = numDOF / 2;
            double temp;

            for (int i = 0; i < 3; i++)
            {
                temp = cosX[i] * force;
                (*theVector)(i) = -temp;
                (*theVector)(i + numDOF2) = temp;
            }

            s << " \n\t unbalanced load: " << *theVector;
        }

        s << " \t Material: " << *theMaterial;
        s << endln;
    }
    else if (flag == 1)
    {
        s << this->getTag() << "  " << strain << "  ";
        s << force << endln;
    }
}

double
Truss::computeCurrentStrain(void) const
{
    // NOTE method will not be called if L == 0

    // determine the strain
    const Vector& disp1 = theNodes[0]->getTrialDisp();
    const Vector& disp2 = theNodes[1]->getTrialDisp();

    double dLength = 0.0;

    for (int i = 0; i < 3; i++)
    {
        dLength += (disp2(i) - disp1(i)) * cosX[i];
    }

    // this method should never be called with L == 0
    return dLength / L;
}

double
Truss::computeCurrentStrainRate(void) const
{
    // NOTE method will not be called if L == 0

    // determine the strain
    const Vector& vel1 = theNodes[0]->getTrialVel();
    const Vector& vel2 = theNodes[1]->getTrialVel();

    double dLength = 0.0;

    for (int i = 0; i < 3; i++)
    {
        dLength += (vel2(i) - vel1(i)) * cosX[i];
    }

    // this method should never be called with L == 0
    return dLength / L;
}

Response*
Truss::setResponse(const char** argv, int argc, Information& eleInfo)
{
    //
    // we compare argv[0] for known response types for the Truss
    //

    if (strcmp(argv[0], "force") == 0 || strcmp(argv[0], "forces") == 0 || strcmp(argv[0], "axialForce") == 0)
    {
        return new ElementResponse(this, 1, 0.0);
    }

    else if (strcmp(argv[0], "defo") == 0 || strcmp(argv[0], "deformations") == 0 ||
             strcmp(argv[0], "deformation") == 0)
    {
        return new ElementResponse(this, 2, 0.0);
    }

    // tangent stiffness matrix
    else if (strcmp(argv[0], "stiff") == 0)
    {
        return new ElementResponse(this, 3, *theMatrix);
    }

    // a material quantity
    else if (strcmp(argv[0], "material") == 0 || strcmp(argv[0], "-material") == 0)
    {
        return theMaterial->setResponse(&argv[1], argc - 1, eleInfo);
    }

    else
    {
        return 0;
    }
}

int
Truss::getResponse(int responseID, Information& eleInfo)
{
    switch (responseID)
    {
        case 1:
            return eleInfo.setDouble(A * theMaterial->getStress());

        case 2:
            return eleInfo.setDouble(L * theMaterial->getStrain());

        case 3:
            return eleInfo.setMatrix(this->getTangentStiff());

        default:
            return 0;
    }
}



Vector*
Truss::getForce(void)
{
    Vector* elementForces;

    if (numDOF == 6)
    {
        elementForces = new Vector(12);
        Vector tempForces(6);

        tempForces = this->getResistingForceIncInertia();

        (*elementForces)(0)  = tempForces(0);
        (*elementForces)(1)  = tempForces(1);
        (*elementForces)(2)  = tempForces(2);
        (*elementForces)(3)  = 0.0;
        (*elementForces)(4)  = 0.0;
        (*elementForces)(5)  = 0.0;
        (*elementForces)(6)  = tempForces(3);
        (*elementForces)(7)  = tempForces(4);
        (*elementForces)(8)  = tempForces(5);
        (*elementForces)(9)  = 0.0;
        (*elementForces)(10) = 0.0;
        (*elementForces)(11) = 0.0;

        return elementForces;

    }
    else if (numDOF == 12)
    {
        elementForces = new Vector(12);
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
    else
    {
        cerr.flush() << "WARNING Truss::getForce() - wrong size of numDOF\n";
        exit(-1);
    }

}

