///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              FourNodeAndesShell.h
// CLASS:             FourNodeAndesShell
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

#ifndef FourNodeAndesShell_H
#define FourNodeAndesShell_H


#ifndef _bool_h
#include "bool.h"
#endif

// Basic Shell Triange
#include "ThreeNodeAndesShell.h"

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
#include <../../ModifiedOpenSeesServices/domain/damping/Damping.h>

#include "cross_product.h"

class Node;

class FourNodeAndesShell: public Element
{

    public:
        //===================================================================================
        // Element Constructors / Destructors
        //===================================================================================
        FourNodeAndesShell ();

        FourNodeAndesShell(int element_number,
                           int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4, double t,
                           NDMaterial* Globalmmodel);

        ~FourNodeAndesShell();

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

        void compute_internal_forces();

        Matrix returnMass(void);

        //Handling body and surface forces
        const Vector& getBodyForce(double loadFactor, const Vector& data);
        const Vector FormEquiBodyForce(void);

        void printMatrix(Matrix M, string name);
        const double getThickness(); //Added by Babak Kamrani to return the element thickness
        void fillM();           //Added by Babak to calculate mass matrices and fill in M on 2/7/13:

    private:


        //===================================================================================
        // Internal variables
        //===================================================================================

        // Pointers to other Domain components
        ID  connectedExternalNodes;     // Tags of  nodes
        Node* theNodes[4];              // pointers to 3 nodes
        NDMaterial** theMaterial;        // pointer to the ND material object at Gauss point (considered constant throughout the element)

        // Finite element matrices
        Matrix K;                       // Element stiffness Matrix
        Matrix M;                       // Element mass matrix
        Vector P;                       // Element resisting force vector
        Vector Q;                       // Applied nodal loads
        Vector internal_forces;         // Forces and moment fields "stresses" for plate bending
        Vector bf;                      // Body forces

        //Vector xl1,  xl2, xl3;          // This shell's local x and y coordinates (z = 0)
        double thickness;               // Element thickness -- Added by Babak to hold the thickness of teh element 12/27/12
        double Area;
        Vector x0;                      // Centroid in global coordinates
        Matrix T_lg;                    // Local-to-global transformation matrix T_lg = [x_l, y_l, z_l]

        ThreeNodeAndesShell* triangle1;
        ThreeNodeAndesShell* triangle2;
        ThreeNodeAndesShell* triangle3;
        ThreeNodeAndesShell* triangle4;

        static unsigned int number_of_four_node_andes_shells;
        static const unsigned int pl[4][18];

        bool is_stiffness_calculated;
        bool is_mass_calculated;

};


#endif // FourNodeAndesShell_H
