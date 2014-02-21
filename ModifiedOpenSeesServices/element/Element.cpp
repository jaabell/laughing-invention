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

// $Revision: 1.15 $
// $Date: 2003/10/30 22:34:23 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/Element.cpp,v $


// Written: fmk 11/95
//
// Purpose: This file contains the class definition for Element.
// Element is an abstract base class and thus no objects of it's type
// can be instantiated. It has pure virtual functions which must be
// implemented in it's derived classes.
//
// The interface:
//

#include <stdlib.h>

#include "Element.h"
// #include <Renderer.h>
#include <Vector.h>
#include <Matrix.h>
#include <Node.h>
#include <Domain.h>
#include <iostream>
using namespace std;


// Matrix **Element::theMatrices;
// Vector **Element::theVectors1;
// Vector **Element::theVectors2;
// int  Element::numMatrices(0);

// Element(int tag, int noExtNodes);
//  constructor that takes the element's unique tag and the number
//  of external nodes for the element.

Element::Element(int tag, int cTag)
    : DomainComponent(tag, cTag), a0(0.0),
      a1(0.0), a2(0.0), a3(0.0), classTag(cTag),
      Kc(0), SizedMatrix(0), index_sized_matrix_vector(0), SizedVector1(0), SizedVector2(0)
      //   , nodeIndex(-1), index(0)
{
    // Nima added July 2011
    // Nima removed May 2012
    //     classTag = cTag;

}


Element::~Element()
{
    if (Kc != 0)
    {
        delete Kc;
    }


    if (SizedMatrix != 0)
    {
        delete SizedMatrix;
    }

    if (SizedVector1 != 0)
    {
        delete SizedVector1;
    }

    if (SizedVector2 != 0)
    {
        delete SizedVector2;
    }

}

int
Element::commitState(void)
{
    if (Kc != 0)
    {
        *Kc = this->getTangentStiff();
    }

    return 0;
}

int
Element::update(void)
{
    return 0;
}

int
Element::revertToStart(void)
{
    return 0;
}


void
Element::setSizedMatrix(void)
{

    int numDOF = this->getNumDOF();
    SizedMatrix = new Matrix(numDOF, numDOF);
    SizedVector1 = new Vector(numDOF);
    SizedVector2 = new Vector(numDOF);

    index_sized_matrix_vector = 1;

}


int
Element::setDampingFactors(double A0, double A1, double A2, double A3, string Stiffness_Type, string Damping_Type)
{
    a0 = A0;
    a1 = A1;
    a2 = A2;
    a3 = A3;
    stiffness_type = Stiffness_Type;
    damping_type = Damping_Type;


    if (index_sized_matrix_vector == 0)
    {
        this->setSizedMatrix();
    }



    // check that memory has been allocated to store compute/return
    // damping matrix & residual force calculations
    //   if (index == -1) {
    //     int numDOF = this->getNumDOF();
    //
    //     for (int i=0; i<numMatrices; i++) {
    //       Matrix *aMatrix = theMatrices[i];
    //       if (aMatrix->noRows() == numDOF) {
    //  index = i;
    //  i = numMatrices;
    //       }
    //     }
    //     if (index == -1) {
    //       Matrix **nextMatrices = new Matrix *[numMatrices+1];
    //       if (nextMatrices == 0) {
    //  cerr << "Element::getTheMatrix - out of memory\n";
    //       }
    //    int j;
    //       for (j=0; j<numMatrices; j++)
    //  nextMatrices[j] = theMatrices[j];
    //       Matrix *theMatrix = new Matrix(numDOF, numDOF);
    //       if (theMatrix == 0) {
    //  cerr << "Element::getTheMatrix - out of memory\n";
    //  exit(-1);
    //       }
    //       nextMatrices[numMatrices] = theMatrix;
    //
    //       Vector **nextVectors1 = new Vector *[numMatrices+1];
    //       Vector **nextVectors2 = new Vector *[numMatrices+1];
    //       if (nextVectors1 == 0 || nextVectors2 == 0) {
    //  cerr << "Element::getTheVector - out of memory\n";
    //  exit(-1);
    //       }
    //
    //       for (j=0; j<numMatrices; j++) {
    //  nextVectors1[j] = theVectors1[j];
    //  nextVectors2[j] = theVectors2[j];
    //       }
    //
    //       Vector *theVector1 = new Vector(numDOF);
    //       Vector *theVector2 = new Vector(numDOF);
    //       if (theVector1 == 0 || theVector2 == 0) {
    //  cerr << "Element::getTheVector - out of memory\n";
    //  exit(-1);
    //       }
    //
    //       nextVectors1[numMatrices] = theVector1;
    //       nextVectors2[numMatrices] = theVector2;
    //
    //       if (numMatrices != 0) {
    //  delete [] theMatrices;
    //  delete [] theVectors1;
    //  delete [] theVectors2;
    //       }
    //       index = numMatrices;
    //       numMatrices++;
    //       theMatrices = nextMatrices;
    //       theVectors1 = nextVectors1;
    //       theVectors2 = nextVectors2;
    //
    //
    //     }
    //   }

    // if need storage for Kc go get it
    if (stiffness_type.compare("Last_Commited_Stiffness") == 0)
    {
        if (Kc == 0)
        {
            Kc = new Matrix(this->getTangentStiff());
        }

        if (Kc == 0)
        {
            cerr << "WARNING - ELEMENT::setDampingFactors - out of memory\n";
            a3 = 0.0;
        }

        // if don't need storage for Kc & have allocated some for it, free the memory
    }
    else if (Kc != 0)
    {
        delete Kc;
        Kc = 0;
    }


    return 0;
}


const Matrix &
Element::getDamp(void)
{
    if (index_sized_matrix_vector  == 0)
    {
        this->setDampingFactors(a0, a1, a2, a3, stiffness_type, damping_type);
    }

    // now compute the damping matrix
    //   Matrix *theMatrix = theMatrices[index];
    //   theMatrix->Zero();
    SizedMatrix->Zero();


    CalculateDamping(SizedMatrix);


    // return the computed matrix
    return *SizedMatrix;
}



const Matrix &
Element::getMass(void)
{
    if (index_sized_matrix_vector  == 0)
    {
        this->setDampingFactors(a0, a1, a2, a3, stiffness_type, damping_type);
    }

    // zero the matrix & return it
    //   Matrix *theMatrix = theMatrices[index];
    //   theMatrix->Zero();
    SizedMatrix->Zero();

    return *SizedMatrix;
}



const Vector &
Element::getResistingForceIncInertia(void)
{
    if (index_sized_matrix_vector == 0)
    {
        this->setDampingFactors(a0, a1, a2, a3, stiffness_type, damping_type);
    }

    //   Matrix *theMatrix = theMatrices[index];
    //   Vector *theVector = theVectors2[index];
    //   Vector *theVector2 = theVectors1[index];

    //
    // perform: R = P(U) - Pext(t);
    //

    (*SizedVector1) = this->getResistingForce();

    //
    // perform: R = R - M * a
    //

    int loc = 0;
    Node **theNodes = this->getNodePtrs();
    int numNodes = this->getNumExternalNodes();

    int i;

    for (i = 0; i < numNodes; i++)
    {
        const Vector &acc = theNodes[i]->getAccel();

        for (int i = 0; i < acc.Size(); i++)
        {
            (*SizedVector2)(loc++) = acc(i);
        }
    }

    SizedVector1->addMatrixVector(1.0, this->getMass(), *SizedVector2, +1.0);

    //
    // perform: R = R + (alphaM * M + betaK0 * K0 + betaK * K) * v
    //            = R + D * v
    //

    // determine the vel vector from ele nodes
    loc = 0;

    for (i = 0; i < numNodes; i++)
    {
        const Vector &vel = theNodes[i]->getTrialVel();

        for (int i = 0; i < vel.Size(); i++)
        {
            (*SizedVector2)(loc++) = vel[i];
        }
    }

    // now compute the damping matrix
    SizedMatrix->Zero();

    CalculateDamping(SizedMatrix);


    // finally the D * v
    SizedVector1->addMatrixVector(1.0, *SizedMatrix, *SizedVector2, 1.0);

    return *SizedVector1;
}




const Vector &
Element::getRayleighDampingForces(void)
{

    if (index_sized_matrix_vector == 0)
    {
        this->setDampingFactors(a0, a1, a2, a3, stiffness_type, damping_type);
    }

    //   Matrix *theMatrix = theMatrices[index];
    //   Vector *theVector = theVectors2[index];
    //   Vector *theVector2 = theVectors1[index];

    //
    // perform: R = (alphaM * M + betaK0 * K0 + betaK * K) * v
    //            = D * v
    //

    // determine the vel vector from ele nodes
    Node **theNodes = this->getNodePtrs();
    int numNodes = this->getNumExternalNodes();
    int loc = 0;

    for (int i = 0; i < numNodes; i++)
    {
        const Vector &vel = theNodes[i]->getTrialVel();

        for (int i = 0; i < vel.Size(); i++)
        {
            (*SizedVector2)(loc++) = vel[i];
        }
    }

    // now compute the damping matrix
    SizedMatrix->Zero();

    CalculateDamping(SizedMatrix);



    // finally the D * v
    SizedVector1->addMatrixVector(0.0, *SizedMatrix, *SizedVector2, 1.0);

    return *SizedVector1;
}



int
Element::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    return 0;
}

/*
int
Element::addInertiaLoadToUnbalance(const Vector &accel)
{
  // some vectors to hold the load increment and RV
  int ndof = this->getNumDOF();
  Vector load(ndof);
  Vector RV(ndof);

  //
  // for each node we will add it's R*accel contribution to RV
  //

  const ID &theNodes = this->getExternalNodes();
  int numNodes = theNodes.Size();
  int loc = 0;
  Domain *theDomain = this->getDomain();
  for (int i=0; i<numNodes; i++) {
    Node *theNode = theDomain->getNode(theNodes(i));
    if (theNode == 0)
      return -1;
    else {
      int numNodeDOF = theNode->getNumberDOF();
      const Vector &nodeRV = theNode->getRV(accel);
      for (int j=0; j<numNodeDOF; j++)
#ifdef _G3DEBUG
    if (loc<ndof)
#endif
      RV(loc++) = nodeRV(j);
    }
  }

  //
  // now we determine - M * R * accel
  //
  const Matrix &mass = this->getMass();
  load = mass * RV;
  load *= -1.0;

  return this->addLoad(load);
}
*/

bool
Element::isSubdomain(void)
{
    return false;
}

Response *
Element::setResponse(const char **argv, int argc, Information &eleInfo)
{
    return 0;
}

int
Element::getResponse(int responseID, Information &eleInformation)
{
    return -1;
}



// Nima Tafazzoli took out (Oct. 2012) (changes are made in the following functions)
// int
// Element::addResistingForceToNodalReaction(int flag)
// {
//
//   int result = 0;
//
//
// if (classTag != ELE_TAG_PenaltyElement) // if the element is not the penalty type
// {
//
//   int numNodes = this->getNumExternalNodes();
//   Node **theNodes = this->getNodePtrs();
//
//   //
//   // first we create the nodes in static arrays as presume
//   // we are going to do this many times & save new/delete
//   //
//
//   if (nodeIndex == -1) {
//     int numLastDOF = -1;
//     for (int i=0; i<numNodes; i++) {
//       int numNodalDOF = theNodes[i]->getNumberDOF();
//
//       if (numNodalDOF != 0 && numNodalDOF != numLastDOF) {
//  // see if an existing vector will do
//  int j;
//  for (j=0; j<numMatrices; j++) {
//    if (theVectors1[j]->Size() == numNodalDOF)
//      nodeIndex = j;
//      j = numMatrices+2;
//  }
//
//  // if not we need to enlarge the bool of temp vectors, matrices
//  if (j != (numMatrices+2)) {
//      Matrix **nextMatrices = new Matrix *[numMatrices+1];
//      if (nextMatrices == 0) {
//        cerr << "Element::addResistingForceToNodalReaction - out of memory\n";
//      }
//      int j;
//      for (j=0; j<numMatrices; j++)
//        nextMatrices[j] = theMatrices[j];
//      Matrix *theMatrix = new Matrix(numNodalDOF, numNodalDOF);
//      if (theMatrix == 0) {
//        cerr << "Element::addResistingForceToNodalReaction - out of memory\n";
//        exit(-1);
//      }
//      nextMatrices[numMatrices] = theMatrix;
//
//      Vector **nextVectors1 = new Vector *[numMatrices+1];
//      Vector **nextVectors2 = new Vector *[numMatrices+1];
//      if (nextVectors1 == 0 || nextVectors2 == 0) {
//        cerr << "Element::addResistingForceToNodalReaction - out of memory\n";
//        exit(-1);
//      }
//
//      for (j=0; j<numMatrices; j++) {
//        nextVectors1[j] = theVectors1[j];
//        nextVectors2[j] = theVectors2[j];
//      }
//
//      Vector *theVector1 = new Vector(numNodalDOF);
//      Vector *theVector2 = new Vector(numNodalDOF);
//      if (theVector1 == 0 || theVector2 == 0) {
//        cerr << "Element::addResistingForceToNodalReaction - out of memory\n";
//        exit(-1);
//      }
//
//      nextVectors1[numMatrices] = theVector1;
//      nextVectors2[numMatrices] = theVector2;
//
//      if (numMatrices != 0) {
//        delete [] theMatrices;
//        delete [] theVectors1;
//        delete [] theVectors2;
//      }
//      nodeIndex = numMatrices;
//      numMatrices++;
//      theMatrices = nextMatrices;
//      theVectors1 = nextVectors1;
//      theVectors2 = nextVectors2;
//  }
//       }
//       numLastDOF = numNodalDOF;
//     }
//   }
//
//   //
//   // now determine the resisting force
//   //
//
//   const Vector *theResistingForce;
//   if (flag == 0)
//     theResistingForce = &(this->getResistingForce());
//   else if (flag == 1)
//     theResistingForce = &(this->getResistingForceIncInertia());
//   else if (flag == 2)
//     theResistingForce = &(this->getRayleighDampingForces());
//
//   if (nodeIndex == -1) {
//     cerr << "LOGIC ERROR Element::addResistingForceToNodalReaction() -HUH!\n";
//     return -1;
//   }
//
//   //
//   // iterate over the elements nodes; determine nodes contribution & add it
//   //
//
//   int nodalDOFCount = 0;
//
//   for (int i=0; i<numNodes; i++) {
//     Node *theNode = theNodes[i];
//
//     int numNodalDOF = theNode->getNumberDOF();
//     Vector *theVector = theVectors1[nodeIndex];
//     if (theVector->Size() != numNodalDOF) {
//       for (int j=0; j<numMatrices; j++)
//  if (theVectors1[j]->Size() == numNodalDOF) {
//    j = numMatrices;
//    theVector = theVectors1[j];
//  }
//     }
//     for (int j=0; j<numNodalDOF; j++) {
//       (*theVector)(j) = (*theResistingForce)(nodalDOFCount);
//       nodalDOFCount++;
//     }
//     result +=theNode->addReactionForce(*theVector, 1.0);
//   }
//
// } // end of if (classTag != ELE_TAG_PenaltyElement) // if the element is not the penalty type
//
//   return result;
//
// }



int
Element::addResistingForceToNodalReaction(int flag)
{

    int result = 0;


    if (classTag != ELE_TAG_PenaltyElement) // if the element is not the penalty type
    {

        int numNodes = this->getNumExternalNodes();
        Node **theNodes = this->getNodePtrs();

        static Vector theVector(48);


        //
        // now determine the resisting force
        //

        const Vector *theResistingForce;

        if (flag == 0)
        {
            theResistingForce = &(this->getResistingForce());
        }
        else if (flag == 1)
        {
            theResistingForce = &(this->getResistingForceIncInertia());
        }
        else if (flag == 2)
        {
            theResistingForce = &(this->getRayleighDampingForces());
        }


        //
        // iterate over the elements nodes; determine nodes contribution & add it
        //

        int nodalDOFCount = 0;

        for (int i = 0; i < numNodes; i++)
        {
            Node *theNode = theNodes[i];

            int numNodalDOF = theNode->getNumberDOF();
            theVector.resize(numNodalDOF);

            for (int j = 0; j < numNodalDOF; j++)
            {
                (theVector)(j) = (*theResistingForce)(nodalDOFCount);
                nodalDOFCount++;
            }

            result += theNode->addReactionForce(theVector, 1.0);
        }

    } // end of if (classTag != ELE_TAG_PenaltyElement) // if the element is not the penalty type

    return result;

}





// Matrix Element::returnMass(void)
// {
//     return this->returnMass();
// }



int Element::getDampingTag(void)
{
    return this->getDampingTag();
}


double Element::getDamping_a0(void)
{
    return this->getDamping_a0();
}


double Element::getDamping_a1(void)
{
    return this->getDamping_a1();
}


double Element::getDamping_a2(void)
{
    return this->getDamping_a2();
}


double Element::getDamping_a3(void)
{
    return this->getDamping_a3();
}


string Element::getDamping_stiffnesstype(void)
{
    return this->getDamping_stiffnesstype();
}


string Element::getDamping_type(void)
{
    return this->getDamping_type();
}


// Nima added July 2011
int Element::getElementclassTag(void)
{
    return classTag;
}

// Nima Tafazzoli (For different types of damping)
int Element::CalculateDamping(Matrix *theMatrix)
{

    if (damping_type.compare("Rayleigh") == 0)
    {
        if (a0 != 0.0)
        {
            theMatrix->addMatrix(0.0, this->getMass(), a0);
        }

        if (stiffness_type.compare("Current_Stiffness") == 0)
        {
            theMatrix->addMatrix(1.0, this->getTangentStiff(), a1);
        }

        else if (stiffness_type.compare("Initial_Stiffness") == 0)
        {
            theMatrix->addMatrix(1.0, this->getInitialStiff(), a1);
        }

        else if (stiffness_type.compare("Last_Commited_Stiffness") == 0)
        {
            theMatrix->addMatrix(1.0, *Kc, a1);
        }
    }

    else if (damping_type.compare("Caughey3rd") == 0)
    {
        if (a0 != 0.0)
        {
            theMatrix->addMatrix(0.0, this->getMass(), a0);
        }


        int numDOF = this->getNumDOF();
        Matrix InvertedMassMatrix(numDOF, numDOF);
        (this->getMass()).Invert(InvertedMassMatrix);

        if (stiffness_type.compare("Current_Stiffness") == 0)
        {
            theMatrix->addMatrix(1.0, this->getTangentStiff(), a1);
            theMatrix->addMatrixTripleProduct(1.0, this->getTangentStiff(), InvertedMassMatrix, a2);
        }

        else if (stiffness_type.compare("Initial_Stiffness") == 0)
        {
            theMatrix->addMatrix(1.0, this->getInitialStiff(), a1);

            //          for (int i=0; i<numDOF; i++)
            //          {
            //            for (int j=0; j<numDOF; j++)
            //            {
            //              cout << (*theMatrix)(i,j) << " ";
            //            }
            //              cout << endl;
            //          }


            theMatrix->addMatrixTripleProduct(1.0, this->getInitialStiff(), InvertedMassMatrix, a2);


            //          for (int i=0; i<numDOF; i++)
            //          {
            //            for (int j=0; j<numDOF; j++)
            //            {
            //              cout << (*theMatrix)(i,j) << " ";
            //            }
            //              cout << endl;
            //          }



            //         theMatrix->addMatrixTripleProduct(1.0, this->getInitialStiff(), this->getInitialStiff(), a2);
        }

        else if (stiffness_type.compare("Last_Commited_Stiffness") == 0)
        {
            theMatrix->addMatrix(1.0, *Kc, a1);
            theMatrix->addMatrixTripleProduct(1.0, *Kc, InvertedMassMatrix, a2);
        }



    }

    else if (damping_type.compare("Caughey4th") == 0)
    {
        if (a0 != 0.0)
        {
            theMatrix->addMatrix(0.0, this->getMass(), a0);
        }


        int numDOF = this->getNumDOF();
        Matrix InvertedMassMatrix(numDOF, numDOF);
        (this->getMass()).Invert(InvertedMassMatrix);

        if (stiffness_type.compare("Current_Stiffness") == 0)
        {
            Matrix ktemp(numDOF, numDOF);
            ktemp = this->getTangentStiff();
            Matrix temp(numDOF, numDOF);
            Matrix temp2(numDOF, numDOF);
            Matrix temp3(numDOF, numDOF);
            temp.addMatrixTripleProduct(0.0, ktemp, InvertedMassMatrix, 1.0);
            temp2.addMatrixProduct(0.0, temp, InvertedMassMatrix, 1.0);
            temp3.addMatrixProduct(0.0, temp2, ktemp, 1.0);

            theMatrix->addMatrix(1.0, ktemp, a1);
            theMatrix->addMatrixTripleProduct(1.0, ktemp, InvertedMassMatrix, a2);
            theMatrix->addMatrix(1.0, temp3, a3);
        }

        else if (stiffness_type.compare("Initial_Stiffness") == 0)
        {
            Matrix ktemp(numDOF, numDOF);
            ktemp = this->getInitialStiff();
            Matrix temp(numDOF, numDOF);
            Matrix temp2(numDOF, numDOF);
            Matrix temp3(numDOF, numDOF);
            temp.addMatrixTripleProduct(0.0, ktemp, InvertedMassMatrix, 1.0);
            temp2.addMatrixProduct(0.0, temp, InvertedMassMatrix, 1.0);
            temp3.addMatrixProduct(0.0, temp2, ktemp, 1.0);

            theMatrix->addMatrix(1.0, ktemp, a1);
            theMatrix->addMatrixTripleProduct(1.0, ktemp, InvertedMassMatrix, a2);
            theMatrix->addMatrix(1.0, temp3, a3);
        }

        else if (stiffness_type.compare("Last_Commited_Stiffness") == 0)
        {
            Matrix ktemp(numDOF, numDOF);
            Matrix temp(numDOF, numDOF);
            Matrix temp2(numDOF, numDOF);
            Matrix temp3(numDOF, numDOF);
            temp.addMatrixTripleProduct(0.0, *Kc, InvertedMassMatrix, 1.0);
            temp2.addMatrixProduct(0.0, temp, InvertedMassMatrix, 1.0);
            temp3.addMatrixProduct(0.0, temp2, *Kc, 1.0);

            theMatrix->addMatrix(1.0, *Kc, a1);
            theMatrix->addMatrixTripleProduct(1.0, *Kc, InvertedMassMatrix, a2);
            theMatrix->addMatrix(1.0, *Kc, a3);
        }
    }


    return 0;
}



double Element::getPenaltyStiffness(void)
{
    return this->getPenaltyStiffness();
}



// Nima Tafazzoli added for checking the mesh (September 2012)
int
Element::CheckMesh(ofstream &checkmesh_file)
{
    return this->CheckMesh(checkmesh_file);
}


Vector *
Element::getStress(void)
{
    return this->getStress();
}


Vector *
Element::getStrain(void)
{
    return this->getStrain();
}


Vector *
Element::getForce(void)
{
    return this->getForce();
}


double Element::returnPressure(void)
{
    return this->returnPressure();
}




; // Added by Babak to print
//--------
void
Element::printSomeInformation(void)
{
    std::cerr << "Element::printSomeInformation(void): -- tag: " << " " << " a1: " << a1 << " a2: " << a2 << " a3:" << a3 << " classTag: " << classTag
              << " Kc: " << Kc << " SizedMatrix: " << SizedMatrix << " index_sized_matrix_vector: " << index_sized_matrix_vector
              << " SizedVector1:" << SizedVector1 << " SizedVector2: " << SizedVector2 << "\n";
}


