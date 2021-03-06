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

    // Sumeet [September, 2016] For Elastic Materials
    // Set status of is_elastic_material_status ==1;
    // Called only while adding element by the user
    /***[Sumeet September, 2016] // Keeps imformation whether the material applied to this element is elastic or not ***/
    /* -1 means not an elastic material */
    /*  0 means elastic material but stiffness has not been calculated yet*/
    /*  1 means elastic material and stiffness has been calculated */
    virtual void setElasticMaterialStatus(int i=0);
    virtual int  getElasticMaterialStatus();

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
 
    //General reporting of element status
    virtual void Print( ostream &s, int flag = 0 );

    void printSomeInformation(void); // Added by Babak to print

    /*************************************************************************
    * Sumeet August, 2016
    * No. of ElementOutputs should be as per the Element_Class_tag_Desc 
    * See the classtags.h for more decription of encoding of 
    * Class_tag Descriptions.
    * For Optimization all the information about elements are encoded
    * in the Element_Class_tag Description 
    * NOTE:- Must Obey the Element_Class_Description [see classTags.h]
    **************************************************************************/
    /**/virtual const vector<float> &getElementOutput();
    /**/virtual const vector<float> &getGaussOutput();
    /**************************************************************************/

    //virtual int describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel);
    virtual int sendSelf ( int commitTag, Channel &theChannel );
    virtual int receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker &);

    virtual std::string getElementName() const
    {
        return "Element";
    }
    // virtual void setDomain(Domain *myDomain);
    using DomainComponent::setDomain;

    //These are for building graph (J.Abell, 2014)
    int getNumberOfBoundaryNodes() const;
    void setNumberOfBoundaryNodes(int n) ;

    // Informs element about starting a new stage.
    virtual int startNewStage();

    virtual void setProducesOutputFlag(bool truthvalue);

    virtual void zeroStrain();

    // added by sumeet 30th July , 201
    virtual void setMaterialTag(int materialTag);
    virtual int  getMaterialTag();

protected:
    const Vector &getRayleighDampingForces(void);


    double a0, a1, a2, a3;
    bool produces_output;
    static string stiffness_type;
    static string damping_type;
    Matrix *Kc; // pointer to hold last committed matrix if needed for rayleigh damping

    Matrix *SizedMatrix;
    Vector *SizedVector1;
    Vector *SizedVector2;

private:

    /***[Sumeet September, 2016] // Keeps imformation whether the material applied to this element is elastic or not ***/
    /* -1 means not an elastic material */
    /*  0 means elastic material but stiffness has not been calculated yet*/
    /*  1 means elastic material and stiffness has been calculated */
    int is_elastic_material_status=-1;  

    int classTag;
    int index_sized_matrix_vector;

    int materialTag = -1;                           // added by sumeet 30th July , 2016

    int numberOfBoundaryNodes;

    static Matrix zero_gauss_coordinates;
    static vector<float> zero_Element_output;      // added by sumeet 30th July , 2016
    static vector<float> zero_Gauss_output;        // added by sumeet 30th July , 2016

};


#endif

