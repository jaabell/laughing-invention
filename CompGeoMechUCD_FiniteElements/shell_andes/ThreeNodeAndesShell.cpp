///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              ThreeNodeAndesShell.cpp
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
(vi)  Chapter 32 of Felipp's Lecture Notes
      Finite element templates for bending

Obtainable as of July 22 2012 at http://www.colorado.edu/engineering/CAS/Felippa.d/FelippaHome.d/Home.html

These documents all mirror published works in indexed journals.
*/

#ifndef THREENODEANDESSHELL_CPP
#define THREENODEANDESSHELL_CPP

#include <iomanip>
#include <iostream>

using namespace std;

#include "ThreeNodeAndesShell.h"
#include "ThreeNodeAndesBending.h"
#include "ThreeNodeAndesMembrane.h"
#include <Vector.h>
#include <cmath>
#include <limits>

// Initialize static variables

// Using the optimal parameter set found in reference (iv) page 14


unsigned int ThreeNodeAndesShell::number_of_three_node_andes_shells = 0;

//===================================================================================
// Element Constructors / Destructors
//===================================================================================

ThreeNodeAndesShell::ThreeNodeAndesShell()
    :
    Element(0, ELE_TAG_ThreeNodeAndesShell ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18), bf(3),
    is_stiffness_calculated(false),
    is_mass_calculated(false)
{
    // zero node pointers
    for (int i = 0; i < 3; i++)
    {
        theNodes[i] = 0;
    }

}


ThreeNodeAndesShell::ThreeNodeAndesShell(int element_number,
        int node_numb_1, int node_numb_2, int node_numb_3,
        double t,
        NDMaterial *Globalmmodel)
    :
    Element(element_number, ELE_TAG_ThreeNodeAndesShell ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18), bf(3),
    is_stiffness_calculated(false),
    is_mass_calculated(false)
{
    // Set connected external node IDs
    connectedExternalNodes(0) = node_numb_1;
    connectedExternalNodes(1) = node_numb_2;
    connectedExternalNodes(2) = node_numb_3;

    // zero node pointers
    for (int i = 0; i < 3; i++)
    {
        theNodes[i] = 0;
    }

    theMaterial = new NDMaterial*;

    if (theMaterial == 0)
    {
        cerr << " ThreeNodeAndesShell::ThreeNodeAndesShell -- failed to allocate material model pointer\n";
        exit(-1);
    }

    *theMaterial = Globalmmodel->getCopy();

    if (*theMaterial == 0)
    {
        cerr << " ThreeNodeAndesShell::ThreeNodeAndesShell -- failed to allocate material model pointer\n";
        exit(-1);
    }

    //


    membrane_element = new ThreeNodeAndesMembrane(element_number, node_numb_1, node_numb_2,  node_numb_3, t, theMaterial);
    bending_element = new ThreeNodeAndesBending(element_number, node_numb_1, node_numb_2,  node_numb_3, t, theMaterial);
}

ThreeNodeAndesShell::ThreeNodeAndesShell(int element_number,
        int node_numb_1, int node_numb_2, int node_numb_3,
        double t,
        NDMaterial **material)
    :
    Element(element_number, ELE_TAG_ThreeNodeAndesShell ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18), bf(3),
    is_stiffness_calculated(false),
    is_mass_calculated(false)
{
    // Set connected external node IDs
    connectedExternalNodes(0) = node_numb_1;
    connectedExternalNodes(1) = node_numb_2;
    connectedExternalNodes(2) = node_numb_3;

    theMaterial = material;

    if ( (theMaterial == NULL) | (*theMaterial == NULL))
    {
        cerr << " ThreeNodeAndesShell::ThreeNodeAndesShell -- received a NULL material model pointer\n";
        exit(-1);
    }

    membrane_element = new ThreeNodeAndesMembrane(element_number, node_numb_1, node_numb_2,  node_numb_3, t, theMaterial);
    bending_element = new ThreeNodeAndesBending(element_number, node_numb_1, node_numb_2,  node_numb_3, t, theMaterial);
}

ThreeNodeAndesShell::~ThreeNodeAndesShell()
{
    if (*theMaterial != NULL)
    {
        delete *theMaterial;
        *theMaterial = NULL;
    }

    delete bending_element;
    delete membrane_element;
}

//===================================================================================
// Element interface
//===================================================================================

int ThreeNodeAndesShell::getNumExternalNodes() const
{
    return 3;
}

const ID &ThreeNodeAndesShell::getExternalNodes()
{
    return connectedExternalNodes;
}

Node **ThreeNodeAndesShell::getNodePtrs(void)
{
    return theNodes;
}

int ThreeNodeAndesShell::getNumDOF()
{
    return 18;
}

void ThreeNodeAndesShell::setDomain(Domain *theDomain)
{
    if (theDomain == 0)
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
        theNodes[2] = 0;
    }
    else
    {
        int Nd1 = connectedExternalNodes(0);
        int Nd2 = connectedExternalNodes(1);
        int Nd3 = connectedExternalNodes(2);

        theNodes[0] = theDomain->getNode(Nd1);
        theNodes[1] = theDomain->getNode(Nd2);
        theNodes[2] = theDomain->getNode(Nd3);

        if (theNodes[0] == 0 || theNodes[1] == 0 || theNodes[2] == 0 )
        {
            cerr << "FATAL ERROR ThreeNodeAndesShell (tag: " << this->getTag() << "), node not found in domain\n";
            exit(-1);
        }

        int dofNd1 = theNodes[0]->getNumberDOF();
        int dofNd2 = theNodes[1]->getNumberDOF();
        int dofNd3 = theNodes[2]->getNumberDOF();

        if (dofNd1 != 6 || dofNd2 != 6 || dofNd3 != 6)
        {
            cerr << "FATAL ERROR ThreeNodeAndesShell (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit(-1);
        }

        membrane_element -> setDomain(theDomain);
        bending_element -> setDomain(theDomain);
        this->DomainComponent::setDomain(theDomain);

    }
}

int ThreeNodeAndesShell::commitState ()
{
    return 0;
}

int ThreeNodeAndesShell::revertToLastCommit ()
{
    return 0;
}

int ThreeNodeAndesShell::revertToStart ()
{
    return 0;
}

int ThreeNodeAndesShell::update(void)
{
    return 0;
}

const Matrix &ThreeNodeAndesShell::getTangentStiff ()
{
    // Since this is a linear element, the stiffness is calculated only once.
    if (!is_stiffness_calculated)
    {
        // Form completestiffness
        Matrix Kb(18, 18);       // Bending stiffness
        Matrix Km(18, 18);       // Membrane stiffness

        //Get matrices
        Kb = bending_element -> getTangentStiff();
        Km = membrane_element -> getTangentStiff();

        K = Kb + Km;

        is_stiffness_calculated = true;
    }

    return K;
}

const Matrix &ThreeNodeAndesShell::getInitialStiff()
{
    return getTangentStiff();
}

const Matrix &ThreeNodeAndesShell::getMass ()
{
    if (!is_mass_calculated)
    {
        // Form completestiffness
        Matrix Mb(18, 18);       // Bending mass
        Matrix Mm(18, 18);       // Membrane mass

        //Get matrices
        Mb = bending_element -> getMass();
        Mm = membrane_element -> getMass();

        M = Mb + Mm;

        is_mass_calculated = true;
    }

    return M;
}

void ThreeNodeAndesShell::zeroLoad ()
{
    Q.Zero();
}

int ThreeNodeAndesShell::addLoad(ElementalLoad *theLoad, double loadFactor)
{


    int type;
    const Vector &data = theLoad->getData(type, loadFactor);

    if (type == LOAD_TAG_ElementSelfWeight)
    {

        Vector Fbody = this->getBodyForce(loadFactor, data);

        Q.addVector(1.0, Fbody, 1.0);

    }
    //    else if (type == LOAD_TAG_BrickSurfaceLoad8Node)
    //    {
    //        Vector Fsurface = this->getSurfaceForce(loadFactor, data);
    //
    //        Q.addVector(1.0, Fsurface, 1.0);
    //
    //    }
    else
    {
        cerr << "ThreeNodeAndesShell::addLoad() - addLoad " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }



    return 0;
}

int ThreeNodeAndesShell::addInertiaLoadToUnbalance(const Vector &accel)
{
    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV(accel);
    const Vector &Raccel2 = theNodes[1]->getRV(accel);
    const Vector &Raccel3 = theNodes[2]->getRV(accel);

    if (6 != Raccel1.Size() || 6 != Raccel2.Size() || 6 != Raccel3.Size()  )
    {
        // Xiaoyan changed 2 to 3 and added Racce15-18  09/27/00
        cerr << "ThreeNodeAndesShell::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }

    static Vector ra(18);  // Changed form 8 to 24(3*8)  Xiaoyan 09/27/00

    ra.Zero();
    ra(0)  =  Raccel1(0);
    ra(1)  =  Raccel1(1);
    ra(2)  =  Raccel1(2);
    ra(3)  =  Raccel1(3);
    ra(4)  =  Raccel1(4);
    ra(5)  =  Raccel1(5);

    ra(6)  =  Raccel2(0);
    ra(7)  =  Raccel2(1);
    ra(8)  =  Raccel2(2);
    ra(9)  =  Raccel2(3);
    ra(10) =  Raccel2(4);
    ra(11) =  Raccel2(5);

    ra(12) =  Raccel3(0);
    ra(13) =  Raccel3(1);
    ra(14) =  Raccel3(2);
    ra(15) =  Raccel3(3);
    ra(16) =  Raccel3(4);
    ra(17) =  Raccel3(5);

    Q.addMatrixVector(1.0, M, ra, -1.0);
    //Q.addMatrixVector(1.0, M, ra, 1.0);

    return 0;
}

const Vector &ThreeNodeAndesShell::getResistingForce ()
{
    P.Zero();

    Vector NodalDisplacements(18);
    Vector disp_i(6);

    for (int node = 0; node < 3; node++)
    {
        Node *node_i = theNodes[node];
        disp_i = node_i->getDisp();
        //disp_i += node_i->getIncrDeltaDisp();
        disp_i += node_i->getIncrDisp();

        NodalDisplacements(6 * node + 0) = disp_i(0);
        NodalDisplacements(6 * node + 1) = disp_i(1);
        NodalDisplacements(6 * node + 2) = disp_i(2);
        NodalDisplacements(6 * node + 3) = disp_i(3);
        NodalDisplacements(6 * node + 4) = disp_i(4);
        NodalDisplacements(6 * node + 5) = disp_i(5);
    }

    P = K * NodalDisplacements;

    return P;
}

const Vector &ThreeNodeAndesShell::getResistingForceIncInertia ()
{
    Vector NodalDisplacements(18);
    Vector disp_i(6);
    Vector NodalAccelerations(18);
    Vector accel_i(6);

    for (int node = 0; node < 3; node++)
    {
        Node *node_i = theNodes[node];
        disp_i = node_i->getDisp();
        //disp_i += node_i->getIncrDeltaDisp();
        disp_i += node_i->getIncrDisp();

        accel_i = node_i->getTrialAccel();

        NodalDisplacements(6 * node + 0) = disp_i(0);
        NodalDisplacements(6 * node + 1) = disp_i(1);
        NodalDisplacements(6 * node + 2) = disp_i(2);
        NodalDisplacements(6 * node + 3) = disp_i(3);
        NodalDisplacements(6 * node + 4) = disp_i(4);
        NodalDisplacements(6 * node + 5) = disp_i(5);

        NodalAccelerations(6 * node + 0) = accel_i(0);
        NodalAccelerations(6 * node + 1) = accel_i(1);
        NodalAccelerations(6 * node + 2) = accel_i(2);
        NodalAccelerations(6 * node + 3) = accel_i(3);
        NodalAccelerations(6 * node + 4) = accel_i(4);
        NodalAccelerations(6 * node + 5) = accel_i(5);
    }

    P = K * NodalDisplacements + this->getMass() * NodalAccelerations;

    // Add nodal equivalent loads from surface or body loads
    P += Q;

    return P;
}

int ThreeNodeAndesShell::sendSelf (int commitTag, Channel &theChannel)
{
    return 0;
}

int ThreeNodeAndesShell::recvSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    return 0;
}

void ThreeNodeAndesShell::Print(ostream &s, int flag = 0)
{

}

// Response* ThreeNodeAndesShell::setResponse (const char** argv, int argc, Information& eleInformation)
// {
//     if (strcmp(argv[0], "force") == 0 || strcmp(argv[0], "forces") == 0)
//     {
//         return new ElementResponse(this, 1, P);
//     }
//     else if (strcmp(argv[0], "stiff") == 0 || strcmp(argv[0], "stiffness") == 0)
//     {
//         return new ElementResponse(this, 5, K);
//     }
//     else
//     {
//         return 0;  // Return a null pointer
//     }
// }

// int ThreeNodeAndesShell::getResponse (int responseID, Information& eleInformation)
// {
//     if (responseID == 1) //forces
//     {
//         return eleInformation.setVector(P);
//     }

//     if (responseID == 5) //stiffness
//     {
//         return eleInformation.setMatrix(K);
//     }
//     else
//     {
//         return -1;
//     }
// }

Matrix ThreeNodeAndesShell::returnMass(void)
{
    if (!is_mass_calculated)
    {
        return getMass();
    }
    else
    {
        return M;
    }
}

void ThreeNodeAndesShell::useThisCoordinateSystem(Vector e1, Vector e2, Vector e3)
{
    bending_element -> useThisCoordinateSystem(e1, e2, e3);
    membrane_element -> useThisCoordinateSystem(e1, e2, e3);
}

double ThreeNodeAndesShell::getArea() const
{
    return bending_element -> getArea();
}

bool ThreeNodeAndesShell::gotMass() const
{
    bool got = bending_element->gotMass() & membrane_element->gotMass();
    //cout << "Got mass?" << (got? "Yes!" : "No :(") << endl;
    return got;
}

const Vector &ThreeNodeAndesShell::getBodyForce(double loadFactor, const Vector &data)
{
    static Vector bforce(18);
    Vector ba(18), bfx(3);

    bforce.Zero();


    //    Vector bf(3);
    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);


    bfx = bf * loadFactor;

    ba(0) =  bfx(0);
    ba(1) =  bfx(1);
    ba(2) =  bfx(2);
    ba(3) =  0.0;
    ba(4) =  0.0;
    ba(5) =  0.0;
    ba(6) =  bfx(0);
    ba(7) =  bfx(1);
    ba(8) =  bfx(2);
    ba(9) =  0.0;
    ba(10) = 0.0;
    ba(11) = 0.0;
    ba(12) = bfx(0);
    ba(13) = bfx(1);
    ba(14) = bfx(2);
    ba(15) = 0.0;
    ba(16) = 0.0;
    ba(17) = 0.0;

    //Form equivalent body force
    this->getMass();

    bforce.addMatrixVector(0.0, M, ba, 1.0);

    return bforce;
}


Vector ThreeNodeAndesShell::get_moment_field()
{
    Vector moment_field(3);
    moment_field = bending_element->get_moment_field();
    return moment_field;
}

#endif











