///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Jose Abell
// DATE:               July 2012
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////

/*

Element is based on the following references by Carlos Felippa

Membrane and drilling parts:
(i)   Membrane Triangles with Corner Drilling Freedoms Part I: The EFF Element.
        Ken Alvin, Horacio M. de la Fuente, Bjorn Haugen, & Carlos A. Felippa.
        August 1991,
        University of Colorado, Boulder,
        Report No. CU-CDDC-91-24
(ii)  Membrane Triangles with Corner Drilling Freedoms Part II: The ANDES Element.
        Carlos A. Felippa & Carmello Militello,
        August 1991,
        University of Colorado, Boulder,
        Report No. CU-CDDC-91-24b
(iii) Membrane Triangles with Corner Drilling Freedoms Part III: Implementation and performance evaluation.
        Carlos A. Felippa & Scott Alexander,
        August 1991,
        University of Colorado, Boulder,
        Report No. CU-CDDC-91-24c
(iv)  A Study of Optimal Triangles with Drilling Freedoms.
        Carlos A. Felippa
        February 2003
        Report No. CU-CAS-03-02

For the bending component
(v)   The First ANDES Elements: 9-DOF Plate Bending Trianges
        Carmello Militello & Carlos A. Felippa
        December 1989
        Report No. CU-CSSC-89-22

Obtainable as of July 22 2012 at http://www.colorado.edu/engineering/CAS/Felippa.d/FelippaHome.d/Home.html

These documents all mirror published works in indexed journals.
*/

#ifndef ThreeNodeAndesMembrane_H
#define ThreeNodeAndesMembrane_H


#ifndef _bool_h
#include "bool.h"
#endif

// MOSS
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <Element.h>
#include <Node.h>
#include <ID.h>
#include <Domain.h>
#include <OPS_Globals.h>
#include <NDMaterial.h>

//#include <string.h>  // Apparently not needed

//NDArray
#include <basics.h>
#include <Matrix.h>
#include <Vector.h>
#include <BJtensor.h>
#include <nDarray.h>
#include <stresst.h>
#include <straint.h>

//Materials
#include <MatPoint3D.h>
#include <Template3Dep.h>

//Others
// #include <ElementResponse.h>
#include <ElementalLoad.h>
#include <Damping.h>
// #include <BodyForce.h>

#include "cross_product.h"

class Node;

class ThreeNodeAndesMembrane: public Element
{

public:
    //===================================================================================
    // Element Constructors / Destructors
    //===================================================================================
    ThreeNodeAndesMembrane();

    ThreeNodeAndesMembrane(int element_number,
                           int node_numb_1, int node_numb_2, int node_numb_3, double t,
                           NDMaterial *Globalmmodel);
    ThreeNodeAndesMembrane(int element_number,
                           int node_numb_1, int node_numb_2, int node_numb_3, double t,
                           NDMaterial **material);  // Need this to be called when this element is part of complete shell element :P
    ~ThreeNodeAndesMembrane();

    //===================================================================================
    // Element interface
    //===================================================================================
    // These member functions are needed to satisfy the Element interface
    // They all overload virtual functions. Most are even pure virtual functions.

    int getNumExternalNodes () const;
    const ID &getExternalNodes ();
    Node **getNodePtrs(void);
    int getNumDOF ();
    void setDomain(Domain *theDomain);

    int commitState ();
    int revertToLastCommit ();
    int revertToStart ();
    int update(void);


    const Matrix &getTangentStiff ();
    const Matrix &getInitialStiff();
    const Matrix &getMass ();

    void zeroLoad ();
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);

    const Vector &getResistingForce ();
    const Vector &getResistingForceIncInertia ();

    int sendSelf (int commitTag, Channel &theChannel);
    int receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    void Print(ostream &s, int flag);
    // Response* setResponse (const char** argv, int argc, Information& eleInformation);
    // int getResponse (int responseID, Information& eleInformation);

    Matrix returnMass(void);

    void useThisCoordinateSystem(Vector e1, Vector e2, Vector e3);
    double getArea() const;

    //Utility
    bool gotMass() const;           // Got Mass?  (check for rho value)

    std::string getElementName() const
    {
        return "ThreeNodeAndesMembrane";
    }

private:
    //===================================================================================
    // Internal Functions
    //===================================================================================
    void initializeGeometry();                              // Calculates all geometry related internal variables
    void calculate_E_planestress_and_beta0();
    void initializeBetaArrays();

    // Member functions for membrane component
    Matrix getMembraneForceLumpingMatrix();
    Matrix getMembraneHierarchicalProjectionMatrix();
    Matrix getMembraneNaturalStrainProjectionMatrix();
    Matrix getMembraneBasicStiffness();
    Matrix getMembraneHighOrderStiffness();


    //===================================================================================
    // Internal variables
    //===================================================================================

    // Pointers to other Domain components
    ID  connectedExternalNodes;     // Tags of  nodes
    Node *theNodes[3];              // pointers to 3 nodes
    NDMaterial **theMaterial;        // pointer to the ND material object at Gauss point (considered constant throughout the element)

    // Finite element matrices
    Matrix K;                       // Element stiffness Matrix
    Matrix M;                       // Element mass matrix
    Vector P;                       // Element resisting force vector
    Vector Q;                       // Applied nodal loads
    Vector bf;                      // Body forces

    // Geometry related
    double thickness;               // Element thickness
    Vector xl1,  xl2, xl3;          // Node i's local x and y coordinates (z = 0)
    Vector x0;                      // Centroid in global coordinates
    Matrix T_lg;                    // Local-to-global transformation matrix T_lg = [x_l, y_l, z_l]
    double Area;                    // yes... its the area of the triangle :)
    double x12, x23, x31;           // Differences in local x coordinates on nodes ij, xij = xli[0] - xlj[0]
    double y12, y23, y31;           // Differences in local y coordinates on nodes ij, yij = yli[0] - ylj[0]


    // Material related
    double rho;                     // Mass per unit volume
    Matrix E_planestress;           // Plane stress constitutive matrix

    // Element formulation related
    static double alpha_membrane;   // Scale factor relating boundary normal displacements to corner rotations. See paper (1) by Felippa.
    static Vector beta_membrane;    // beta_membrane parameter values
    double beta0;                   // Optimal beta0 depends on material Poisson's ratio as seen in ref (iv) page 17 --> beta0 = 1/2*(1-4*nu^2)

    static unsigned int number_of_three_node_andes_membrane;

    bool is_stiffness_calculated;
    bool is_mass_calculated;

};


#endif // ThreeNodeAndesMembrane_H
