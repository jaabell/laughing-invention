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

// $Revision: 1.11 $
// $Date: 2003/10/30 22:34:23 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/Element.h,v $


// Written: fmk
// Created: 11/96
// Revision: A
//
// Description: This file contains the class definition for Element.
// Element is an abstract base class and thus no objects of it's type
// can be instantiated. It has pure virtual functions which must be
// implemented in it's derived classes.
//
// What: "@(#) Element.h, revA"

#ifndef Element_h
#define Element_h

#ifndef _bool_h
#include "bool.h"
#endif

#include <ID.h>

#include <DomainComponent.h>
#include <string>
using namespace std;


class Matrix;
class Vector;
class Renderer;
class Info;
class Information;
class Response;
class ElementalLoad;
class Node;

class Element : public DomainComponent
{
    public:
        Element(int tag, int classTag);
        virtual ~Element();

        // methods dealing with nodes and number of external dof
        virtual int getNumExternalNodes(void) const = 0;
        virtual const ID &getExternalNodes(void)  = 0;
        virtual Node **getNodePtrs(void)  = 0;
        virtual int getNumDOF(void) = 0;
        virtual Matrix &getGaussCoordinates(void);

        // methods dealing with committed state and update
        virtual int commitState(void);
        virtual int revertToLastCommit(void) = 0;
        virtual int revertToStart(void);
        virtual int update(void);
        virtual bool isSubdomain(void);

        // methods to return the current linearized stiffness,
        // damping and mass matrices
        virtual const Matrix &getTangentStiff(void) = 0;
        virtual const Matrix &getInitialStiff(void) = 0;
        virtual const Matrix &getDamp(void);
        virtual const Matrix &getMass(void);

        //     virtual Matrix returnMass(void);
        virtual double getDamping_a0(void);
        virtual double getDamping_a1(void);
        virtual double getDamping_a2(void);
        virtual double getDamping_a3(void);
        virtual int getDampingTag(void);
        virtual string getDamping_stiffnesstype(void);
        virtual string getDamping_type(void);
        virtual int getElementclassTag(void);


        // Nima Tafazzoli added for returning the penalty stiffness
        // for penalty element (2012)
        virtual double getPenaltyStiffness(void);
        // December 2013 (for coupled contact elemenet)
        virtual double returnPressure(void);

        // methods for applying loads
        virtual void zeroLoad(void) = 0;
        virtual int addLoad(ElementalLoad *theLoad, double loadFactor) = 0;
        virtual int addInertiaLoadToUnbalance(const Vector &accel) = 0;
        virtual int setDampingFactors(double a0, double a1, double a2, double a3, string Stiffness_type, string Damping_Type);

        // methods for obtaining resisting force (force includes elemental loads)
        virtual const Vector &getResistingForce(void) = 0;
        virtual const Vector &getResistingForceIncInertia(void);

        // method for obtaining information specific to an element
        // virtual Response *setResponse(const char **argv, int argc, Information &eleInformation);
        // virtual int getResponse(int responseID, Information &eleInformation);

        //method for retrieving data size for repartitioning, Guanzhou added
        virtual int getObjectSize()
        {
            return 1;
        }; //sub-classes to override!!!


        int CalculateDamping(Matrix *);
        void setSizedMatrix(void);


        virtual int addResistingForceToNodalReaction(int flag);

        // Nima Tafazzoli added for checking the mesh (September 2012)
        virtual int CheckMesh(ofstream &);
        virtual Vector *getStress(void);
        virtual Vector *getStrain(void);
        virtual Vector *getForce(void);
        void printSomeInformation(void); // Added by Babak to print


        virtual int getOutputSize() const;
        virtual const Vector &getOutput() const;

        virtual int describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel);
        virtual int sendSelf ( int commitTag, Channel &theChannel );
        virtual int recvSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker &);

        virtual std::string getElementName() const
        {
            return "Base Class Element";
        }
        // virtual void setDomain(Domain *myDomain);
        using DomainComponent::setDomain;



    protected:
        const Vector &getRayleighDampingForces(void);

        double a0, a1, a2, a3;
        string stiffness_type;
        string damping_type;
        Matrix *Kc; // pointer to hold last committed matrix if needed for rayleigh damping

    private:
        //     int index;
        //     int nodeIndex;

        //     static Matrix ** theMatrices;
        //     static Vector ** theVectors1;
        //     static Vector ** theVectors2;
        //     static int numMatrices;
        int classTag;
        Matrix *SizedMatrix;
        Vector *SizedVector1;
        Vector *SizedVector2;
        int index_sized_matrix_vector;

        static Matrix zero_gauss_coordinates;

};


#endif

