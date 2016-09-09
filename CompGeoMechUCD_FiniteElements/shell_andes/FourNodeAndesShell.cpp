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
(vi)  Chapter 32 of Felipp's Lecture Notes
      Finite element templates for bending

Obtainable as of July 22 2012 at http://www.colorado.edu/engineering/CAS/Felippa.d/FelippaHome.d/Home.html

These documents all mirror published works in indexed journals.
*/

#ifndef FourNodeAndesShell_CPP
#define FourNodeAndesShell_CPP

#include <iomanip>
#include <iostream>

using namespace std;

#include "FourNodeAndesShell.h"
#include "ThreeNodeAndesShell.h"

#include <Vector.h>
#include <cmath>
#include <limits>

// Initialize static variables
unsigned int FourNodeAndesShell::number_of_four_node_andes_shells = 0;
// vector<float> FourNodeAndesShell::Element_Output_Vector(6);

// This array relates the DOFs of each sub-triangle to the DOFs of the parent quad. pl is for "placement array"
const unsigned int FourNodeAndesShell::pl[4][18] =
{
    { 1, 2, 3,  4,  5,  6,    7,  8,  9, 10, 11, 12,   13, 14, 15, 16, 17, 18 },
    { 1, 2, 3,  4,  5,  6,   13, 14, 15, 16, 17, 18,   19, 20, 21, 22, 23, 24 },
    { 1, 2, 3,  4,  5,  6,    7,  8,  9, 10, 11, 12,   19, 20, 21, 22, 23, 24 },
    { 7, 8, 9, 10, 11, 12,   13, 14, 15, 16, 17, 18,   19, 20, 21, 22, 23, 24 }
};

//===================================================================================
// Element Constructors / Destructors
//===================================================================================

FourNodeAndesShell::FourNodeAndesShell()
    :
    Element(0, ELE_TAG_FourNodeAndesShell),
    connectedExternalNodes(4),
    K(24, 24), M(24, 24),
    P(24), Q(24), internal_forces(6), bf(3),  thickness(0.0), x0(3), T_lg(3, 3),
    is_stiffness_calculated(false),
    is_mass_calculated(false)
{
    // zero node pointers
    for (int i = 0; i < 4; i++)
    {
        theNodes[i] = 0;
    }

    // Added by Babak 12/27/12:
    //--------
    triangle1 = new ThreeNodeAndesShell();
    triangle2 = new ThreeNodeAndesShell();
    triangle3 = new ThreeNodeAndesShell();
    triangle4 = new ThreeNodeAndesShell();

    theMaterial = new NDMaterial*;

    if (theMaterial == 0)
    {
        cerr << " FourNodeAndesShell::FourNodeAndesShell -- failed to allocate material model pointer\n";
        exit(-1);
    }

    this->setNumberOfBoundaryNodes(2);

    //--------
}


FourNodeAndesShell::FourNodeAndesShell(int element_number,
                                       int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                                       double t,
                                       NDMaterial *Globalmmodel)
    :
    Element(element_number, ELE_TAG_FourNodeAndesShell ),
    connectedExternalNodes(4),
    K(24, 24), M(24, 24),
    P(24), Q(24), internal_forces(6), bf(3), thickness(t), x0(3), T_lg(3, 3),
    is_stiffness_calculated(false),
    is_mass_calculated(false)
{

    this->setMaterialTag(Globalmmodel->getTag());

    // Set connected external node IDs
    connectedExternalNodes(0) = node_numb_1;
    connectedExternalNodes(1) = node_numb_2;
    connectedExternalNodes(2) = node_numb_3;
    connectedExternalNodes(3) = node_numb_4;

    // zero node pointers
    for (int i = 0; i < 4; i++)
    {
        theNodes[i] = 0;
    }

    theMaterial = new NDMaterial*;

    if (theMaterial == 0)
    {
        cerr << " FourNodeAndesShell::FourNodeAndesShell -- failed to allocate material model pointer\n";
        exit(-1);
    }

    *theMaterial = Globalmmodel->getCopy();

    if (*theMaterial == 0)
    {
        cerr << " FourNodeAndesShell::FourNodeAndesShell -- failed to allocate material model pointer\n";
        exit(-1);
    }


    triangle1 = new ThreeNodeAndesShell(element_number, node_numb_1, node_numb_2,  node_numb_3, t, theMaterial);
    triangle2 = new ThreeNodeAndesShell(element_number, node_numb_1, node_numb_3,  node_numb_4, t, theMaterial);
    triangle3 = new ThreeNodeAndesShell(element_number, node_numb_1, node_numb_2,  node_numb_4, t, theMaterial);
    triangle4 = new ThreeNodeAndesShell(element_number, node_numb_2, node_numb_3,  node_numb_4, t, theMaterial);

    this->setNumberOfBoundaryNodes(2);


}

FourNodeAndesShell::~FourNodeAndesShell()
{
    if (*theMaterial != NULL)
    {
        delete *theMaterial;
        *theMaterial = NULL;
    }

    delete triangle1;
    delete triangle2;
    delete triangle3;
    delete triangle4;
}

//===================================================================================
// Element interface
//===================================================================================

int FourNodeAndesShell::getNumExternalNodes() const
{
    return 4;
}

const ID &FourNodeAndesShell::getExternalNodes()
{
    return connectedExternalNodes;
}

Node **FourNodeAndesShell::getNodePtrs(void)
{
    return theNodes;
}

int FourNodeAndesShell::getNumDOF()
{
    return 24;
}

void FourNodeAndesShell::setDomain(Domain *theDomain)
{
    if (theDomain == 0)
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
        theNodes[2] = 0;
        theNodes[3] = 0;
    }
    else
    {
        int Nd1 = connectedExternalNodes(0);
        int Nd2 = connectedExternalNodes(1);
        int Nd3 = connectedExternalNodes(2);
        int Nd4 = connectedExternalNodes(3);

        theNodes[0] = theDomain->getNode(Nd1);
        theNodes[1] = theDomain->getNode(Nd2);
        theNodes[2] = theDomain->getNode(Nd3);
        theNodes[3] = theDomain->getNode(Nd4);

        if (theNodes[0] == 0 || theNodes[1] == 0 || theNodes[2] == 0 || theNodes[3] == 0 )
        {
            cerr << "FATAL ERROR FourNodeAndesShell (tag: " << this->getTag() << "), node not found in domain\n";
            exit(-1);
        }

        int dofNd1 = theNodes[0]->getNumberDOF();
        int dofNd2 = theNodes[1]->getNumberDOF();
        int dofNd3 = theNodes[2]->getNumberDOF();
        int dofNd4 = theNodes[3]->getNumberDOF();

        if (dofNd1 != 6 || dofNd2 != 6 || dofNd3 != 6 || dofNd4 != 6)
        {
            cerr << "FATAL ERROR FourNodeAndesShell (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit(-1);
        }

        triangle1 -> setDomain(theDomain);
        triangle2 -> setDomain(theDomain);
        triangle3 -> setDomain(theDomain);
        triangle4 -> setDomain(theDomain);

        this->DomainComponent::setDomain(theDomain);

        // ========================================================
        // Compute the local coordinate system
        // ========================================================

        //Local unit vectors
        Vector e1(3), e2(3), e3(3), e1a(3), e2a(3), e1b(3), e2b(3);

        //node coordinates
        Vector p1(3), p2(3), p3(3), p4(3);

        p1 = theNodes[0]->getCrds();
        p2 = theNodes[1]->getCrds();
        p3 = theNodes[2]->getCrds();
        p4 = theNodes[3]->getCrds();

        x0 = (p1 + p2 + p3 + p4) / 4;

        // Local x axis points parallel to 1-2 side
        e1a = p2 - p1;
        e2a = p4 - p1;
        e1b = p3 - p4;
        e2b = p3 - p2;

        //These two vectors are in the plane of the element
        // or at least in the average plane of the element.
        e1 = 0.5 * (e1a + e1b);
        e2 = 0.5 * (e2a + e2b);


        Area = (calculate_cross_product(e1a, e2a).Norm() +
                calculate_cross_product(e1b, e2b).Norm() +
                calculate_cross_product(e1b, e2a).Norm() +
                calculate_cross_product(e1a, e2b).Norm()) / 4;

        if (Area <= 0)
        {
            cout << "ThreeNodeAndesBending::initializeGeometry() -> Element # " << getTag() << " has A = " << Area << "!!" << endl;
        }

        //Normal to the (average) plane of the element
        e3 = calculate_cross_product(e1, e2);
        e3.Normalize();

        if (  e3(0)*e3(0) + e3(1)*e3(1) <= 1e-6   )
        {
            // If the element normal is neary vertical, make local axis vertical and axis aligned with global axis
            e1(0) = 1;
            e1(1) = 0;
            e1(2) = 0;

            e2(0) = 0;
            e2(1) = 1;
            e2(2) = 0;

            e3(0) = 0;
            e3(1) = 0;
            e3(2) = 1;
        }
        else
        {
            // Define e1 to be in plane of the element (orthogonal to e3) and horizontal (no z component)
            double nx = e3(0);
            double ny = e3(1);

            //             double nz = e3(2);
            if ( abs(nx) > abs(ny) )
            {
                e1(0) = -ny / nx;
                e1(1) = 1;
                e1(2) = 0;
            }
            else
            {
                e1(0) = 1;
                e1(1) = -nx / ny;
                e1(2) = 0;
            }
        }

        e1.Normalize();
        e2 = calculate_cross_product(e3, e1) ;
        e2.Normalize();

        //Local-to-global transformation matrix
        for (int i = 0; i < 3; i++)
        {
            T_lg(i, 0) = e1(i);
            T_lg(i, 1) = e2(i);
            T_lg(i, 2) = e3(i);
        }

        // Make all triangles have the same coordinate system computed by the quad
        triangle1 -> useThisCoordinateSystem(e1, e2, e3);
        triangle2 -> useThisCoordinateSystem(e1, e2, e3);
        triangle3 -> useThisCoordinateSystem(e1, e2, e3);
        triangle4 -> useThisCoordinateSystem(e1, e2, e3);
    }
}

int FourNodeAndesShell::commitState ()
{
    // compute_internal_forces();  //writes output vector too!

    return 0;
}

int FourNodeAndesShell::revertToLastCommit ()
{
    return 0;
}

int FourNodeAndesShell::revertToStart ()
{
    return 0;
}

int FourNodeAndesShell::update(void)
{
    return 0;
}

const Matrix &FourNodeAndesShell::getTangentStiff ()
{
    // Since this is a linear element, the stiffness is calculated only once.
    if (!is_stiffness_calculated)
    {

        // Form completestiffness
        Matrix K1(18, 18), K2(18, 18), K3(18, 18), K4(18, 18); //, Ka(24,24), Kb(24,24);

        K1 = triangle1 -> getTangentStiff();
        K2 = triangle2 -> getTangentStiff();
        K3 = triangle3 -> getTangentStiff();
        K4 = triangle4 -> getTangentStiff();

        K.Zero();

        for (int i = 0; i < 18; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                K(pl[0][i] - 1, pl[0][j] - 1) += K1(i, j) * 0.5;
                K(pl[1][i] - 1, pl[1][j] - 1) += K2(i, j) * 0.5;
                K(pl[2][i] - 1, pl[2][j] - 1) += K3(i, j) * 0.5;
                K(pl[3][i] - 1, pl[3][j] - 1) += K4(i, j) * 0.5;
            }
        }

        is_stiffness_calculated = true;
    }

    return K;
}

const Matrix &FourNodeAndesShell::getInitialStiff()
{
    return getTangentStiff();
}

const Matrix &FourNodeAndesShell::getMass ()
{
    if (!is_mass_calculated)
    {
        // Form complete mass

        Matrix M1(18, 18), M2(18, 18), M3(18, 18), M4(18, 18);

        M1 = triangle1 -> getMass();
        M2 = triangle2 -> getMass();
        M3 = triangle3 -> getMass();
        M4 = triangle4 -> getMass();

        M.Zero();

        for (int i = 0; i < 18; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                M(pl[0][i] - 1, pl[0][j] - 1) += M1(i, j) * 0.5;
                M(pl[1][i] - 1, pl[1][j] - 1) += M2(i, j) * 0.5;
                M(pl[2][i] - 1, pl[2][j] - 1) += M3(i, j) * 0.5;
                M(pl[3][i] - 1, pl[3][j] - 1) += M4(i, j) * 0.5;
            }
        }

        is_mass_calculated = true;
    }

    //cerr << "\n M = \n" << M << "\n";

    return M;
}

void FourNodeAndesShell::printMatrix(Matrix M, string name)
{
    cout << endl;
    cout << endl;
    cout << name << " = " << endl;
    int field_width = 10;

    for (int i = 0; i < M.noCols(); i++)
    {
        for (int j = 0; j < M.noRows(); j++)
        {
            if (i >= j)
            {
                if (M(i, j) == 0)
                {
                    for (int k = 0;  k <= field_width; k++)
                    {
                        cout << " ";
                    }
                }
                else
                {
                    cout << setw(field_width) << setprecision(6) << fixed << right  << M(i, j) << " ";
                }
            }
        }

        cout << endl;
    }
}

void FourNodeAndesShell::zeroLoad ()
{
    Q.Zero();
}

int FourNodeAndesShell::addLoad(ElementalLoad *theLoad, double loadFactor)
{


    int type;
    const Vector &data = theLoad->getData(type, loadFactor);

    if (type == LOAD_TAG_ElementSelfWeight)
    {

        Vector Fbody = this->getBodyForce(loadFactor, data);

        //-Factor added to compatibilize with rest of code :/
        Q.addVector(1.0, Fbody, -1.0);

    }
    else
    {
        cerr << "FourNodeAndesShell::addLoad() - addLoad " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }



    return 0;
}


int FourNodeAndesShell::addInertiaLoadToUnbalance(const Vector &accel)
{
    // Check for a quick return
    if ( !triangle1->gotMass() & !triangle2->gotMass() & !triangle3->gotMass() & !triangle4->gotMass())
    {
        cout << "FourNodeAndesShell::addInertiaLoadToUnbalance: Quick return!" << endl;
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV(accel);
    const Vector &Raccel2 = theNodes[1]->getRV(accel);
    const Vector &Raccel3 = theNodes[2]->getRV(accel);
    const Vector &Raccel4 = theNodes[3]->getRV(accel);

    if (6 != Raccel1.Size() || 6 != Raccel2.Size() || 6 != Raccel3.Size() || 6 != Raccel4.Size()  )
    {
        cerr << "FourNodeAndesShell::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }

    static Vector ra(24);

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

    ra(18) =  Raccel4(0);
    ra(19) =  Raccel4(1);
    ra(20) =  Raccel4(2);
    ra(21) =  Raccel4(3);
    ra(22) =  Raccel4(4);
    ra(23) =  Raccel4(5);


    //Added by Babak Kamrani to fill the M if it is not filled before 2/7/13:
    //---
    if (!is_mass_calculated)
    {
        this->fillM();
    }

    //---

    Q.addMatrixVector(1.0, M, ra, -1.0);

    return 0;
}


const Vector &FourNodeAndesShell::getResistingForce ()
{
    P.Zero();

    Vector NodalDisplacements(24);
    Vector disp_i(6);

    for (int node = 0; node < 4; node++)
    {
        Node *node_i = theNodes[node];
        disp_i = node_i->getTrialDisp();

        NodalDisplacements(6 * node + 0) = disp_i(0);
        NodalDisplacements(6 * node + 1) = disp_i(1);
        NodalDisplacements(6 * node + 2) = disp_i(2);
        NodalDisplacements(6 * node + 3) = disp_i(3);
        NodalDisplacements(6 * node + 4) = disp_i(4);
        NodalDisplacements(6 * node + 5) = disp_i(5);
    }

    P = K * NodalDisplacements;

    // Add nodal equivalent loads from surface or body loads
    P += Q;

    return P;
}

const Vector &FourNodeAndesShell::getResistingForceIncInertia ()
{
    Vector NodalDisplacements(24);
    Vector disp_i(6);
    Vector NodalAccelerations(24);
    Vector accel_i(6);

    for (int node = 0; node < 4; node++)
    {
        Node *node_i = theNodes[node];

        disp_i = node_i->getTrialDisp();
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

    if (!is_mass_calculated)
    {
        this->getMass();
    }

    P = K * NodalDisplacements + M * NodalAccelerations;

    // Add nodal equivalent loads from surface or body loads
    P += Q;

    // Add damping forces if they are defined
    if (a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
    {
        P += this->getRayleighDampingForces();
    }

    return P;
}

int FourNodeAndesShell::sendSelf (int commitTag, Channel &theChannel)
//Implemented by Babak Kamrani on 12/27/12:
{
    int dataTag = this->getDbTag();
    int matDbTag = 0;
    static ID idData(8);


    idData(0) = (*theMaterial)->getClassTag();
    idData(1) = this->getTag();
    idData(2) = connectedExternalNodes(0);
    idData(3) = connectedExternalNodes(1);
    idData(4) = connectedExternalNodes(2);
    idData(5) = connectedExternalNodes(3);


    matDbTag = (*theMaterial)->getDbTag();
    idData(6) = matDbTag;

    idData(7) = this->getTag();




    if (theChannel.sendID(dataTag, commitTag, idData) < 0)
    {
        std::cerr << "WARNING  FourNodeAndesShell::sendSelf - " << this->getTag() << " failed to send ID\n";
        return -1;
    }




    if ((*theMaterial)->sendSelf(commitTag, theChannel) < 0)
    {
        std::cerr << "WARNING  FourNodeAndesShell::sendSelf - " << this->getTag() << " failed to send NDMaterial\n";
        return -1;
    }

    static Vector matProp(5);
    matProp(0) = 0; //body Force Number
    matProp(1) = bf(0);
    matProp(2) = bf(1);
    matProp(3) = bf(2);
    matProp(4) = thickness;

    if (matProp(4) == 0.0)

    {
        std::cerr << " FourNodeAndesShell::sendSelf -- Element Number #" << this->getTag() << "  Element thickness is zero ... \n";
        abort();
    }


    if (theChannel.sendVector(dataTag, commitTag, matProp) < 0)
    {
        std::cerr << "WARNING  FourNodeAndesShell::sendSelf - " << this->getTag() << " failed to send its Material Property\n";
        return -1;
    }

    return 0;
}

int FourNodeAndesShell::receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
//Implemented by Babak Kamrani on 12/27/12:
{
    int dataTag = this->getDbTag();
    int matDbTag = 0;


    static ID idData(8);

    if (theChannel.receiveID(dataTag, commitTag, idData) < 0)
    {
        std::cerr << "WARNIN FourNodeAndesShell::receiveSelf() - " << this->getTag() << " failed to receive IdData\n";
        return -1;
    }



    int matClassTag = idData(0);
    this->setTag(idData(1));
    connectedExternalNodes(0) = idData(2);
    connectedExternalNodes(1) = idData(3);
    connectedExternalNodes(2) = idData(4);
    connectedExternalNodes(3) = idData(5);
    matDbTag = idData(6);
    int element_number = idData(7);





    *theMaterial = theBroker.getNewNDMaterial(matClassTag);

    if (theMaterial == 0)
    {
        std::cerr << "WARNING FourNodeAndesShell::receiveSelf() - Broker could not create NDMaterial of class type " << matClassTag << endln;
        return -1;
    }



    (*theMaterial)->setDbTag(matDbTag);

    if ((*theMaterial)->receiveSelf(commitTag, theChannel, theBroker) < 0)
    {
        cerr << "FourNodeAndesShell::receiveSelf() - material failed to recv itself\n";
        return -1;
    }

    static Vector matProp(5);

    if ( theChannel.receiveVector(dataTag, commitTag, matProp) < 0 )
    {
        std::cerr << "WARNING FourNodeAndesShell::receiveSelf() - failed to recv matProb!\n";
        return -1;
    }

    bf(0) = matProp(1);
    bf(1) = matProp(2);
    bf(2) = matProp(3);
    double t = (double)matProp(4);
    thickness = t;

    if (t == 0.0)

    {
        std::cerr << "FourNodeAndesShell::receiveSelf() -- Element Number #" << element_number << "   Element thickness is zero ... \n";
        abort();
    }

    triangle1 = new ThreeNodeAndesShell(element_number, connectedExternalNodes(0), connectedExternalNodes(1), connectedExternalNodes(2), t, theMaterial);
    triangle2 = new ThreeNodeAndesShell(element_number, connectedExternalNodes(0), connectedExternalNodes(2), connectedExternalNodes(3), t, theMaterial);
    triangle3 = new ThreeNodeAndesShell(element_number, connectedExternalNodes(0), connectedExternalNodes(1), connectedExternalNodes(3), t, theMaterial);
    triangle4 = new ThreeNodeAndesShell(element_number, connectedExternalNodes(1), connectedExternalNodes(2), connectedExternalNodes(3), t, theMaterial);

    return 0;
}

void FourNodeAndesShell::Print(ostream &s, int flag = 0)
{
    cout << "Element # " << this->getTag() << " Four Node ANDES Shell " << endl << endl;
    cout << "Nodes: " << endl;

    for (int n = 0; n < 4; n++)
    {
        Vector crd = this->theNodes[n]->getCrds();
        cout << "  " << n + 1 << " (domain node # " << connectedExternalNodes(n) << ") : (" << crd(0) << ", " << crd(1) << ", " << crd(2) << ") " << endl;
    }

    cout << "Area: " << this->Area << endl;
    cout << "Thickness: " << this->thickness << endl;
    cout << "Local axes (e1, e2, e3):" << endl;

    for (int i = 0; i < 3; i++)
    {
        cout << "    |";

        for (int j = 0; j < 3; j++)
        {
            cout << setw(6) << setprecision(4) << this->T_lg(i, j);

            if (j < 2)
            {
                cout << ", ";
            }
        }

        cout << "|" << endl;
    }

    cout << endl << endl;
    cout << "Material info:" << endl;
    (*theMaterial)->Print(s);
    cout << endl << endl;

    //printMatrix(M, "Mdiag");



}

//Added by Babak Kamrani to return the element thickness
const double FourNodeAndesShell::getThickness()
{
    return thickness;
}

// Response* FourNodeAndesShell::setResponse (const char** argv, int argc, Information& eleInformation)
// {
//     if (strcmp(argv[0], "force") == 0 || strcmp(argv[0], "forces") == 0)
//     {
//         return new ElementResponse(this, 1, P);
//     }
//     else if (strcmp(argv[0], "stiff") == 0 || strcmp(argv[0], "stiffness") == 0)
//     {
//         return new ElementResponse(this, 5, K);
//     }
//     else if (strcmp(argv[0], "stress") == 0 || strcmp(argv[0], "stresses") == 0)
//     {
//         return new ElementResponse(this, 4, internal_forces);
//     }
//     else
//     {
//         return 0;  // Return a null pointer
//     }
// }

// int FourNodeAndesShell::getResponse (int responseID, Information& eleInformation)
// {
//     if (responseID == 1) //forces
//     {
//         return eleInformation.setVector(P);
//     }
//     else if (responseID == 5) //stiffness
//     {
//         return eleInformation.setMatrix(K);
//     }
//     else if (responseID == 4)
//     {
//         compute_internal_forces();
//         return eleInformation.setVector(internal_forces);
//     }
//     else
//     {
//         return -1;
//     }
// }

Matrix FourNodeAndesShell::returnMass(void)
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

const Vector &FourNodeAndesShell::getBodyForce(double loadFactor, const Vector &data)
{
    static Vector bforce(24);
    Vector ba(24), bfx(3);

    bforce.Zero();

    // Check for a quick return
    if ( (!triangle1->gotMass()) & (!triangle2->gotMass()) & (!triangle3->gotMass()) & (!triangle4->gotMass()))
    {
        return bforce;
    }

    //Vector bf(3); Part of element's data members now
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
    ba(18) = bfx(0);
    ba(19) = bfx(1);
    ba(20) = bfx(2);
    ba(21) = 0.0;
    ba(22) = 0.0;
    ba(23) = 0.0;

    //Form equivalent body force
    if (!is_mass_calculated)
    {
        this->getMass();
    }

    bforce.addMatrixVector(0.0, M, ba, 1.0);

    return bforce;
}

//Added by Babak to calculate mass matrices and fill in M on 2/7/13:
void FourNodeAndesShell::fillM()
{
    if (~is_mass_calculated)
    {
        // Form complete mass

        Matrix M1(18, 18), M2(18, 18), M3(18, 18), M4(18, 18);

        M1 = triangle1 -> getMass();
        M2 = triangle2 -> getMass();
        M3 = triangle3 -> getMass();
        M4 = triangle4 -> getMass();

        M.Zero();

        for (int i = 0; i < 18; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                M(pl[0][i] - 1, pl[0][j] - 1) += M1(i, j) * 0.5;
                M(pl[1][i] - 1, pl[1][j] - 1) += M2(i, j) * 0.5;
                M(pl[2][i] - 1, pl[2][j] - 1) += M3(i, j) * 0.5;
                M(pl[3][i] - 1, pl[3][j] - 1) += M4(i, j) * 0.5;
            }
        }

        is_mass_calculated = true;
    }
}

void FourNodeAndesShell::compute_internal_forces()
{
    // Hold moment fields from each subtriangle and master quad
    Vector m1(3), m2(3), m3(3), m4(3), m(3);

    m1 = triangle1->get_moment_field();
    m2 = triangle2->get_moment_field();
    m3 = triangle3->get_moment_field();
    m4 = triangle4->get_moment_field();

    // Average fields over master quad (NOTE: Who knows if this is correct?) Ask Felippa.... good luck with that
    m = 0.25 * (m1 + m2 + m3 + m4);

    internal_forces(0) = 0.0;    // p11 TODO:implement this
    internal_forces(1) = 0.0;    // p22 TODO:implement this
    internal_forces(2) = 0.0;    // p12 TODO:implement this
    internal_forces(3) = m(0);   // m11
    internal_forces(4) = m(1);   // m22
    internal_forces(5) = m(2);   // m12

}

// Sumeet Yuan and Boris Removed on 8th September as we do not know output
// const vector<float> &FourNodeAndesShell::getElementOutput()
// {
//     // Hold moment fields from each subtriangle and master quad
//     Vector m1(3), m2(3), m3(3), m4(3), m(3);

//     m1 = triangle1->get_moment_field();
//     m2 = triangle2->get_moment_field();
//     m3 = triangle3->get_moment_field();
//     m4 = triangle4->get_moment_field();

//     // Average fields over master quad (NOTE: Who knows if this is correct?) Ask Felippa.... good luck with that
//     m = 0.25 * (m1 + m2 + m3 + m4);

//     Element_Output_Vector[0] = 0.0;    // p11 TODO:implement this
//     Element_Output_Vector[1] = 0.0;    // p22 TODO:implement this
//     Element_Output_Vector[2] = 0.0;    // p12 TODO:implement this
//     Element_Output_Vector[3] = m(0);   // m11
//     Element_Output_Vector[4] = m(1);   // m22
//     Element_Output_Vector[5] = m(2);   // m12

//     return Element_Output_Vector;
// }


int FourNodeAndesShell::getObjectSize()
{
    //Copied from EightNodeBrickLT (with modifications)

    int size = sizeof(*this);
    // ID  connectedExternalNodes;     // Tags of  nodes
    size += 4 * sizeof(int); ///
    // Node *theNodes[4];              // pointers to 3 nodes
    size += 4 * sizeof(Node*); ///

    // Pointers to other Domain components
    // NDMaterial **theMaterial;        // pointer to the ND material object at Gauss point (considered constant throughout the element)
    size +=  (*theMaterial)->getObjectSize();

    // Finite element matrices
    // Matrix K;                       // Element stiffness Matrix
    size += 24 * 24 * sizeof(double);
    // Matrix M;                       // Element mass matrix
    size += 24 * 24 * sizeof(double);
    // Vector P;                       // Element resisting force vector
    size += 24 * sizeof(double);
    // Vector Q;                       // Applied nodal loads
    size += 24 * sizeof(double);
    // Vector internal_forces;         // Forces and moment fields "stresses" for plate bending
    size += 6 * sizeof(double);
    // Vector bf;                      // Body forces
    size += 3 * sizeof(double);

    //Vector xl1,  xl2, xl3;          // This shell's local x and y coordinates (z = 0)
    // double thickness;               // Element thickness -- Added by Babak to hold the thickness of teh element 12/27/12
    // double Area;
    size += 2 * sizeof(double);
    // Vector x0;                      // Centroid in global coordinates
    size += 3 * sizeof(double);
    // Matrix T_lg;                    // Local-to-global transformation matrix T_lg = [x_l, y_l, z_l]
    size += 3 * 3 * sizeof(double);

    // ThreeNodeAndesShell *triangle1;
    // ThreeNodeAndesShell *triangle2;
    // ThreeNodeAndesShell *triangle3;
    // ThreeNodeAndesShell *triangle4;
    // size += 3 * 3 * sizeof(double);

    // static unsigned int number_of_four_node_andes_shells;
    // static const unsigned int pl[4][18];

    // bool is_stiffness_calculated;
    // bool is_mass_calculated;
    size += 2 * sizeof(bool);

    return size;
}


#endif











