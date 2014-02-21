///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              ThreeNodeAndesShell.h
// CLASS:             ThreeNodeAndesShell
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic, Jose Abell
// PROGRAMMER:        Boris Jeremic, Jose Abell
// DATE:              July 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

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

#ifndef THREENODEANDESSHELL_H
#define THREENODEANDESSHELL_H


#ifndef _bool_h
#include "bool.h"
#endif

// COMPONENTS
#include "ThreeNodeAndesBending.h"
#include "ThreeNodeAndesMembrane.h"

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
#include <ElementResponse.h>
#include <ElementalLoad.h>
#include <Damping.h>
#include <BodyForce.h>

class Node;

class ThreeNodeAndesShell: public Element
{

    public:
        //===================================================================================
        // Element Constructors / Destructors
        //===================================================================================
        ThreeNodeAndesShell ();

        ThreeNodeAndesShell(int element_number,
                            int node_numb_1, int node_numb_2, int node_numb_3, double t,
                            NDMaterial* Globalmmodel, BodyForce* bodyforce);
        ThreeNodeAndesShell(int element_number,
                            int node_numb_1, int node_numb_2, int node_numb_3,
                            double t,
                            NDMaterial** material, BodyForce* bodyforce);

        ~ThreeNodeAndesShell();

        //===================================================================================
        // Element interface
        //===================================================================================
        // These member functions are needed to satisfy the Element interface
        // They all overload virtual functions. Most are even pure virtual functions.

        int getNumExternalNodes () const;
        const ID& getExternalNodes ();
        Node** getNodePtrs(void);
        int getNumDOF ();
        void setDomain(Domain* theDomain);

        int commitState ();
        int revertToLastCommit ();
        int revertToStart ();
        int update(void);


        const Matrix& getTangentStiff ();
        const Matrix& getInitialStiff();
        const Matrix& getMass ();

        void zeroLoad ();
        int addLoad(ElementalLoad* theLoad, double loadFactor);
        int addInertiaLoadToUnbalance(const Vector& accel);

        const Vector& getResistingForce ();
        const Vector& getResistingForceIncInertia ();

        int sendSelf (int commitTag, Channel& theChannel);
        int recvSelf (int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag);
        Response* setResponse (const char** argv, int argc, Information& eleInformation);
        int getResponse (int responseID, Information& eleInformation);

        Matrix returnMass(void);

        void useThisCoordinateSystem(Vector e1, Vector e2, Vector e3);

        double getArea() const;

        //Handling body and surface forces
        const Vector& getBodyForce(double loadFactor);
        const Vector FormEquiBodyForce(void);

        //Utility
        bool gotMass() const;           // Got Mass?  (check for rho value)

    private:


        //===================================================================================
        // Internal variables
        //===================================================================================

        // Pointers to other Domain components
        ID  connectedExternalNodes;     // Tags of  nodes
        Node* theNodes[3];              // pointers to 3 nodes
        NDMaterial** theMaterial;        // pointer to the ND material object at Gauss point (considered constant throughout the element)

        // Finite element matrices
        Matrix K;                       // Element stiffness Matrix
        Matrix M;                       // Element mass matrix
        Vector P;                       // Element resisting force vector
        Vector Q;                       // Applied nodal loads
        Vector bf;                      // Body forces

        ThreeNodeAndesBending* bending_element;
        ThreeNodeAndesMembrane* membrane_element;

        static unsigned int number_of_three_node_andes_shells;

        bool is_stiffness_calculated;
        bool is_mass_calculated;

};


#endif // THREENODEANDESSHELL_H
