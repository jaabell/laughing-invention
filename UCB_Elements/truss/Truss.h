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

// $Revision: 1.13 $
// $Date: 2003/03/17 19:19:49 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/truss/Truss.h,v $


#ifndef Truss_h
#define Truss_h

// File: ~/element/truss/Truss.h
//
// Written: fmk
// Created: 07/98
// Revision: A
//
// Description: This file contains the class definition for Truss. A Truss object
// provides the abstraction of the small deformation bar element. Each truss
// object is assocaited with a material object. This Truss element will work
// in 1d, 2d or 3d problems.
//
// What: "@(#) Truss.h, revA"

#include <Element.h>
#include <Matrix.h>

class Node;
class Channel;
class UniaxialMaterial;

class Truss : public Element
{
public:
    Truss(int tag,
          int Nd1, int Nd2,
          UniaxialMaterial &theMaterial,
          double A, double rho = 0.0);

    Truss();
    ~Truss();

    // public methods to obtain inforrmation about dof & connectivity
    int getNumExternalNodes(void) const;
    const ID &getExternalNodes(void);
    Node **getNodePtrs(void);

    int getNumDOF(void);
    void setDomain(Domain *theDomain);

    // public methods to set the state of the element
    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);
    int update(void);

    // public methods to obtain stiffness, mass, damping and residual information
    const Matrix &getKi(void);
    const Matrix &getTangentStiff(void);
    const Matrix &getInitialStiff(void);
    const Matrix &getDamp(void);
    const Matrix &getMass(void);

    void zeroLoad(void);
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);

    const Vector &getResistingForce(void);
    const Vector &getResistingForceIncInertia(void);

    // public methods for element output
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
    //     int displaySelf(Renderer &theViewer, int displayMode, float fact);
    void Print(ostream &s, int flag = 0);

    // Response* setResponseResponse(const char** argv, int argc, Information& eleInfo);
    // int getResponse(int responseID, Information& eleInformation);
    Vector *getForce(void);

    std::string getElementName() const
    {
        return "Truss";
    }

    /********************************************************************************************************************
    * Sumeet August, 2016
    * This element has no gauss points and thus no outputs at gauss points 
    * so no needto have the "getGaussOutput()" function
    *********************************************************************************************************************/
    const vector<float> &getElementOutput() ;

protected:

private:
    double computeCurrentStrain(void) const;
    double computeCurrentStrainRate(void) const;

    // private attributes - a copy for each object of the class
    UniaxialMaterial *theMaterial;  // pointer to a material
    ID  connectedExternalNodes;     // contains the tags of the end nodes
    int numDOF;                     // number of dof for truss

    Vector *theLoad;     // pointer to the load vector P
    Matrix *theMatrix; // pointer to objects matrix (a class wide Matrix)
    Vector *theVector; // pointer to objects vector (a clas wide Vector)

    double L;       // length of truss based on undeformed configuration
    double A;       // area of truss
    double rho;     // rho: mass density per unit length

    double cosX[3]; // direction cosines

    Node *theNodes[2];

    // static data - single copy for all objects of the class
    static Matrix trussM6;   // class wide matrix for 6*6
    static Matrix trussM12;  // class wide matrix for 12*12
    static Vector trussV6;   // class wide Vector for size 6
    static Vector trussV12;  // class wide Vector for size 12

    // Sumeet August, 2016
    // Initialize only if there is any output for element except at gauss points
    static vector<float> Element_Output_Vector; 
};

#endif




