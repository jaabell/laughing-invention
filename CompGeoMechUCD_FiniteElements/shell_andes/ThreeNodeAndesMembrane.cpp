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

#ifndef ThreeNodeAndesMembrane_CPP
#define ThreeNodeAndesMembrane_CPP

#include <iomanip>
#include <iostream>

using namespace std;

#include "ThreeNodeAndesMembrane.h"
#include <Vector.h>
#include <cmath>
#include <limits>

// static variables

// Using the optimal parameter set found in reference (iv) page 14
double ThreeNodeAndesMembrane::alpha_membrane = 1.5;          //Cfr. Reference (iii)
Vector ThreeNodeAndesMembrane::beta_membrane(10);


unsigned int ThreeNodeAndesMembrane::number_of_three_node_andes_membrane = 0;

//===================================================================================
// Element Constructors / Destructors
//===================================================================================

ThreeNodeAndesMembrane::ThreeNodeAndesMembrane()
    :
    Element(0, ELE_TAG_ThreeNodeAndesMembrane ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18), bf(3),
    thickness(0.0),
    xl1(3), xl2(3), xl3(3), x0(3), T_lg(3, 3),
    rho(0.0), E_planestress(3, 3),
    is_stiffness_calculated(false), is_mass_calculated(false)
{
    // zero node pointers
    for (int i = 0; i < 3; i++)
    {
        theNodes[i] = 0;
    }

    initializeBetaArrays();
}


ThreeNodeAndesMembrane::ThreeNodeAndesMembrane(int element_number,
        int node_numb_1, int node_numb_2, int node_numb_3,
        double t,
        NDMaterial *Globalmmodel)
    :
    Element(element_number, ELE_TAG_ThreeNodeAndesMembrane ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18), bf(3),
    thickness(t),
    xl1(3), xl2(3), xl3(3), x0(3), T_lg(3, 3),
    rho(0.0), E_planestress(3, 3),
    is_stiffness_calculated(false), is_mass_calculated(false)
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
        cerr << " ThreeNodeAndesMembrane::ThreeNodeAndesMembrane -- failed to allocate material model pointer\n";
        exit(-1);
    }

    *theMaterial = Globalmmodel->getCopy();

    if (*theMaterial == 0)
    {
        cerr << " ThreeNodeAndesMembrane::ThreeNodeAndesMembrane -- failed to allocate material model pointer\n";
        exit(-1);
    }

    //
    rho = Globalmmodel->getRho();

    if (rho < 0)
    {
        cerr << " ThreeNodeAndesMembrane::ThreeNodeAndesMembrane -- rho = " << rho << "\n";
        exit(-1);
    }

    //bf = bodyforce->getBodyForceVector();

    initializeBetaArrays();
}

ThreeNodeAndesMembrane::ThreeNodeAndesMembrane(int element_number,
        int node_numb_1, int node_numb_2, int node_numb_3,
        double t,
        NDMaterial **material)
    :
    Element(element_number, ELE_TAG_ThreeNodeAndesMembrane ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18), bf(3),
    thickness(t),
    xl1(3), xl2(3), xl3(3), x0(3), T_lg(3, 3),
    rho(0.0), E_planestress(3, 3),
    is_stiffness_calculated(false), is_mass_calculated(false)
{
    // Set connected external node IDs
    connectedExternalNodes(0) = node_numb_1;
    connectedExternalNodes(1) = node_numb_2;
    connectedExternalNodes(2) = node_numb_3;

    theMaterial = material;

    if ( (theMaterial == NULL) | (*theMaterial == NULL))
    {
        cerr << " ThreeNodeAndesMembrane::ThreeNodeAndesMembrane -- received a NULL material model pointer\n";
        exit(-1);
    }

    rho = (*theMaterial)->getRho();


    if (rho < 0)
    {
        cerr << " ThreeNodeAndesMembrane::ThreeNodeAndesMembrane -- rho = " << rho << "\n";
        exit(-1);
    }

    //bf = bodyforce->getBodyForceVector();

    initializeBetaArrays();
}

ThreeNodeAndesMembrane::~ThreeNodeAndesMembrane()
{

    if (*theMaterial != NULL)
    {
        delete *theMaterial;
        *theMaterial = NULL;
    }
}

//===================================================================================
// Element interface
//===================================================================================

int ThreeNodeAndesMembrane::getNumExternalNodes() const
{
    return 3;
}

const ID &ThreeNodeAndesMembrane::getExternalNodes()
{
    return connectedExternalNodes;
}

Node **ThreeNodeAndesMembrane::getNodePtrs(void)
{
    return theNodes;
}

int ThreeNodeAndesMembrane::getNumDOF()
{
    return 18;
}

void ThreeNodeAndesMembrane::setDomain(Domain *theDomain)
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
            cerr << "FATAL ERROR ThreeNodeAndesMembrane (tag: " << this->getTag() << "), node not found in domain\n";
            exit(-1);
        }

        int dofNd1 = theNodes[0]->getNumberDOF();
        int dofNd2 = theNodes[1]->getNumberDOF();
        int dofNd3 = theNodes[2]->getNumberDOF();

        if (dofNd1 != 6 || dofNd2 != 6 || dofNd3 != 6)
        {
            cerr << "FATAL ERROR ThreeNodeAndesMembrane (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit(-1);
        }

        this->DomainComponent::setDomain(theDomain);

        initializeGeometry();
    }
}

int ThreeNodeAndesMembrane::commitState ()
{
    return 0;
}

int ThreeNodeAndesMembrane::revertToLastCommit ()
{
    return 0;
}

int ThreeNodeAndesMembrane::revertToStart ()
{
    return 0;
}

int ThreeNodeAndesMembrane::update(void)
{
    return 0;
}

const Matrix &ThreeNodeAndesMembrane::getTangentStiff ()
{
    // Since this is a linear element, the stiffness is calculated only once.
    if (!is_stiffness_calculated)
    {
        int I, J;

        // Form completestiffness
        Matrix Kb_membrane(9, 9);       // Basic membrane stiffness
        Matrix Kh_membrane(9, 9);       // High-order membrane stiffness
        //Matrix Kb_bending(9,9);         // Basic bending stiffness
        //Matrix Kh_bending(9,9);         // High-order bending stiffness
        Matrix Klocal(18, 18);          // Total (membrane + bending) stiffness in local coordinates
        Matrix TLG(18, 18);             // Local-to-global transformation matrix

        //Get matrices
        Kb_membrane = getMembraneBasicStiffness();
        Kh_membrane = getMembraneHighOrderStiffness();
        //Kb_bending  = getBendingBasicStiffness();
        //Kh_bending  = getBendingHighOrderStiffness();

        // Expand matrices to local complete stiffness
        //   DOFS   --->        ux1, uy1, uz1, rx1, ry1, rz1  |  ux2, uy2, uz2, rx2, ry2, rz2  |  ux3, uy3, uz3, rx3, ry3, rz3
        int membrane_dofs[9] = {  0,   1,                  5,      6,   7,                 11,     12,  13,                 17 };
        //int  bending_dofs[9] = {            2,   3,   4,                     8,   9,  10,                    14,  15,  16      };

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // Expand membrane stiffness to local membrane (& drilling) DOFS.
                I = membrane_dofs[i];
                J = membrane_dofs[j];
                Klocal(I, J) = Kb_membrane(i, j) + Kh_membrane(i, j);

                // Expand bending stiffness to local bending DOFS.
                //I = bending_dofs[i];
                //J = bending_dofs[j];
                //Klocal(I,J) = Kb_bending(i,j)/Area + Area*Kh_bending(i,j);
            }
        }

        // Transform K from local to global, K_glob = [TLG]^T * K * [TLG]
        // Where [T_lg] is blockwise diagonal arrangement of 6 T_lg matrices.
        //
        //           / T_lg     0     0     0 \                               .
        // [TLG] =  |     0  T_lg     0     0  |                              .
        //          |     0     0  ...      0  |                              .
        //           \    0     0     0  T_lg / 18x18                         .
        //

        // Form TLG
        TLG.Zero();
        Matrix T_gl(3, 3);
        T_lg.Invert(T_gl);
        int offset = 0;

        for (int n = 0; n < 6; n++)
        {
            I = 0;

            for (int i = 0; i < 3; i++)
            {
                J = 0;

                for (int j = 0; j < 3; j++)
                {
                    TLG(I + offset, J + offset) = T_gl(i, j);
                    J++;
                }

                I++;
            }

            offset += 3;
        }

        // Compute total stiffness as K = TLG^T * Klocal *TLG
        K.Zero();

        K.addMatrixTripleProduct(1.0, TLG, Klocal, 1.0);

        is_stiffness_calculated = true;
    }


    return K;
}

const Matrix &ThreeNodeAndesMembrane::getInitialStiff()
{
    return getTangentStiff();
}

const Matrix &ThreeNodeAndesMembrane::getMass ()
{
    if (!is_mass_calculated)
    {
        double x13 = -x31;
        double y13 = -y31;
        double alpha = 1.5;

        Matrix TLG(18, 18);             // Local-to-global transformation matrix

        Matrix Ml(9, 9);
        Matrix Mlocal(18, 18);

        Ml(0, 0) = 30;
        Ml(0, 1) = 0;
        Ml(0, 2) = 3 * alpha * y12 + 3 * alpha * y13;
        Ml(0, 3) = 15;
        Ml(0, 4) = 0;
        Ml(0, 5) = -3 * alpha * y12 + 3 * alpha * y23 / 2;
        Ml(0, 6) = 15;
        Ml(0, 7) = 0;
        Ml(0, 8) = -3 * alpha * y13 - 3 * alpha * y23 / 2;
        Ml(1, 0) = 0;
        Ml(1, 1) = 30;
        Ml(1, 2) = -3 * alpha * x12 - 3 * alpha * x13;
        Ml(1, 3) = 0;
        Ml(1, 4) = 15;
        Ml(1, 5) = 3 * alpha * x12 - 3 * alpha * x23 / 2;
        Ml(1, 6) = 0;
        Ml(1, 7) = 15;
        Ml(1, 8) = 3 * alpha * x13 + 3 * alpha * x23 / 2;
        Ml(2, 0) = 3 * alpha * y12 + 3 * alpha * y13;
        Ml(2, 1) = -3 * alpha * x12 - 3 * alpha * x13;
        Ml(2, 2) = -45 * alpha * x12 * (-alpha * x12 / 45 - alpha * x13 / 90) / 2 - 45 * alpha * x13 * (-alpha * x12 / 90 - alpha * x13 / 45) / 2 + 45 * alpha * y12 * (alpha * y12 / 45 + alpha * y13 / 90) / 2 + 45 * alpha * y13 * (alpha * y12 / 90 + alpha * y13 / 45) / 2;
        Ml(2, 3) = 3 * alpha * y12 + 3 * alpha * y13 / 2;
        Ml(2, 4) = -3 * alpha * x12 - 3 * alpha * x13 / 2;
        Ml(2, 5) = 45 * alpha * x12 * (-alpha * x12 / 45 - alpha * x13 / 90) / 2 - 45 * alpha * x23 * (-alpha * x12 / 90 - alpha * x13 / 90) / 2 - 45 * alpha * y12 * (alpha * y12 / 45 + alpha * y13 / 90) / 2 + 45 * alpha * y23 * (alpha * y12 / 90 + alpha * y13 / 90) / 2;
        Ml(2, 6) = 3 * alpha * y12 / 2 + 3 * alpha * y13;
        Ml(2, 7) = -3 * alpha * x12 / 2 - 3 * alpha * x13;
        Ml(2, 8) = 45 * alpha * x13 * (-alpha * x12 / 90 - alpha * x13 / 45) / 2 + 45 * alpha * x23 * (-alpha * x12 / 90 - alpha * x13 / 90) / 2 - 45 * alpha * y13 * (alpha * y12 / 90 + alpha * y13 / 45) / 2 - 45 * alpha * y23 * (alpha * y12 / 90 + alpha * y13 / 90) / 2;
        Ml(3, 0) = 15;
        Ml(3, 1) = 0;
        Ml(3, 2) = 3 * alpha * y12 + 3 * alpha * y13 / 2;
        Ml(3, 3) = 30;
        Ml(3, 4) = 0;
        Ml(3, 5) = -3 * alpha * y12 + 3 * alpha * y23;
        Ml(3, 6) = 15;
        Ml(3, 7) = 0;
        Ml(3, 8) = -3 * alpha * y13 / 2 - 3 * alpha * y23;
        Ml(4, 0) = 0;
        Ml(4, 1) = 15;
        Ml(4, 2) = -3 * alpha * x12 - 3 * alpha * x13 / 2;
        Ml(4, 3) = 0;
        Ml(4, 4) = 30;
        Ml(4, 5) = 3 * alpha * x12 - 3 * alpha * x23;
        Ml(4, 6) = 0;
        Ml(4, 7) = 15;
        Ml(4, 8) = 3 * alpha * x13 / 2 + 3 * alpha * x23;
        Ml(5, 0) = -3 * alpha * y12 + 3 * alpha * y23 / 2;
        Ml(5, 1) = 3 * alpha * x12 - 3 * alpha * x23 / 2;
        Ml(5, 2) = -45 * alpha * x12 * (alpha * x12 / 45 - alpha * x23 / 90) / 2 - 45 * alpha * x13 * (alpha * x12 / 90 - alpha * x23 / 90) / 2 + 45 * alpha * y12 * (-alpha * y12 / 45 + alpha * y23 / 90) / 2 + 45 * alpha * y13 * (-alpha * y12 / 90 + alpha * y23 / 90) / 2;
        Ml(5, 3) = -3 * alpha * y12 + 3 * alpha * y23;
        Ml(5, 4) = 3 * alpha * x12 - 3 * alpha * x23;
        Ml(5, 5) = 45 * alpha * x12 * (alpha * x12 / 45 - alpha * x23 / 90) / 2 - 45 * alpha * x23 * (alpha * x12 / 90 - alpha * x23 / 45) / 2 - 45 * alpha * y12 * (-alpha * y12 / 45 + alpha * y23 / 90) / 2 + 45 * alpha * y23 * (-alpha * y12 / 90 + alpha * y23 / 45) / 2;
        Ml(5, 6) = -3 * alpha * y12 / 2 + 3 * alpha * y23;
        Ml(5, 7) = 3 * alpha * x12 / 2 - 3 * alpha * x23;
        Ml(5, 8) = 45 * alpha * x13 * (alpha * x12 / 90 - alpha * x23 / 90) / 2 + 45 * alpha * x23 * (alpha * x12 / 90 - alpha * x23 / 45) / 2 - 45 * alpha * y13 * (-alpha * y12 / 90 + alpha * y23 / 90) / 2 - 45 * alpha * y23 * (-alpha * y12 / 90 + alpha * y23 / 45) / 2;
        Ml(6, 0) = 15;
        Ml(6, 1) = 0;
        Ml(6, 2) = 3 * alpha * y12 / 2 + 3 * alpha * y13;
        Ml(6, 3) = 15;
        Ml(6, 4) = 0;
        Ml(6, 5) = -3 * alpha * y12 / 2 + 3 * alpha * y23;
        Ml(6, 6) = 30;
        Ml(6, 7) = 0;
        Ml(6, 8) = -3 * alpha * y13 - 3 * alpha * y23;
        Ml(7, 0) = 0;
        Ml(7, 1) = 15;
        Ml(7, 2) = -3 * alpha * x12 / 2 - 3 * alpha * x13;
        Ml(7, 3) = 0;
        Ml(7, 4) = 15;
        Ml(7, 5) = 3 * alpha * x12 / 2 - 3 * alpha * x23;
        Ml(7, 6) = 0;
        Ml(7, 7) = 30;
        Ml(7, 8) = 3 * alpha * x13 + 3 * alpha * x23;
        Ml(8, 0) = -3 * alpha * y13 - 3 * alpha * y23 / 2;
        Ml(8, 1) = 3 * alpha * x13 + 3 * alpha * x23 / 2;
        Ml(8, 2) = -45 * alpha * x12 * (alpha * x13 / 90 + alpha * x23 / 90) / 2 - 45 * alpha * x13 * (alpha * x13 / 45 + alpha * x23 / 90) / 2 + 45 * alpha * y12 * (-alpha * y13 / 90 - alpha * y23 / 90) / 2 + 45 * alpha * y13 * (-alpha * y13 / 45 - alpha * y23 / 90) / 2;
        Ml(8, 3) = -3 * alpha * y13 / 2 - 3 * alpha * y23;
        Ml(8, 4) = 3 * alpha * x13 / 2 + 3 * alpha * x23;
        Ml(8, 5) = 45 * alpha * x12 * (alpha * x13 / 90 + alpha * x23 / 90) / 2 - 45 * alpha * x23 * (alpha * x13 / 90 + alpha * x23 / 45) / 2 - 45 * alpha * y12 * (-alpha * y13 / 90 - alpha * y23 / 90) / 2 + 45 * alpha * y23 * (-alpha * y13 / 90 - alpha * y23 / 45) / 2;
        Ml(8, 6) = -3 * alpha * y13 - 3 * alpha * y23;
        Ml(8, 7) = 3 * alpha * x13 + 3 * alpha * x23;
        Ml(8, 8) = 45 * alpha * x13 * (alpha * x13 / 45 + alpha * x23 / 90) / 2 + 45 * alpha * x23 * (alpha * x13 / 90 + alpha * x23 / 45) / 2 - 45 * alpha * y13 * (-alpha * y13 / 45 - alpha * y23 / 90) / 2 - 45 * alpha * y23 * (-alpha * y13 / 90 - alpha * y23 / 45) / 2;


        // Expand matrices to local complete stiffness
        //   DOFS   --->        ux1,  uy1, uz1, rx1, ry1, rz1  |  ux2, uy2, uz2, rx2, ry2, rz2  |  ux3, uy3, uz3, rx3, ry3, rz3
        int membrane_dofs[9] = {  0,   1,                  5,      6,   7,                 11,     12,  13,                 17 };
        //int  bending_dofs[9] = {            2,   3,   4,                     8,   9,  10,                    14,  15,  16      };

        double weight_over_180 = thickness * Area * rho / 180.0;

        int I = 0;
        int J = 0;

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // Expand membrane mass to local membrane (& drilling) DOFS.
                I = membrane_dofs[i];
                J = membrane_dofs[j];
                Mlocal(I, J) = weight_over_180 * Ml(i, j);
            }
        }

        // Transform K from local to global, K_glob = [TLG]^T * K * [TLG]
        // Where [T_lg] is blockwise diagonal arrangement of 6 T_lg matrices.
        //
        //           / T_lg     0     0     0 \                                 .
        // [TLG] =  |     0  T_lg     0     0  |                                .
        //          |     0     0  ...      0  |                                .
        //           \    0     0     0  T_lg / 18 x 18                         .
        //

        // Form TLG
        TLG.Zero();
        Matrix T_gl(3, 3);
        T_lg.Invert(T_gl);
        int offset = 0;

        for (int n = 0; n < 6; n++)
        {
            I = 0;

            for (int i = 0; i < 3; i++)
            {
                J = 0;

                for (int j = 0; j < 3; j++)
                {
                    TLG(I + offset, J + offset) = T_gl(i, j);
                    J++;
                }

                I++;
            }

            offset += 3;
        }

        // Compute total mass
        M.Zero();

        M.addMatrixTripleProduct(1.0, TLG, Mlocal, 1.0);

        is_mass_calculated = true;
    }

    return M;
}

void ThreeNodeAndesMembrane::zeroLoad ()
{
    Q.Zero();
}

int ThreeNodeAndesMembrane::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    return 0;
}

int ThreeNodeAndesMembrane::addInertiaLoadToUnbalance(const Vector &accel)
{
    return 0;
}

const Vector &ThreeNodeAndesMembrane::getResistingForce ()
{
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

const Vector &ThreeNodeAndesMembrane::getResistingForceIncInertia ()
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

        accel_i = node_i->getAccel();

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

    P = K * NodalDisplacements + M * NodalAccelerations;

    return P;
}

int ThreeNodeAndesMembrane::sendSelf (int commitTag, Channel &theChannel)
{
    return 0;
}

int ThreeNodeAndesMembrane::receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    return 0;
}

void ThreeNodeAndesMembrane::Print(ostream &s, int flag = 0)
{

}

// Response* ThreeNodeAndesMembrane::setResponse (const char** argv, int argc, Information& eleInformation)
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

// int ThreeNodeAndesMembrane::getResponse (int responseID, Information& eleInformation)
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

Matrix ThreeNodeAndesMembrane::returnMass(void)
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





// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
//===================================================================================
// Internal Functions
//===================================================================================
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
void ThreeNodeAndesMembrane::initializeGeometry()
{
    //Local unit vectors
    Vector e1(3), e2(3), e3(3);

    //node coordinates
    Vector x1(3);
    x1 = theNodes[0]->getCrds();
    Vector x2(3);
    x2 = theNodes[1]->getCrds();
    Vector x3(3);
    x3 = theNodes[2]->getCrds();

    //Centroid
    x0 = (x1 + x2 + x3) / 3;

    // Local x axis points paralell to side 1-2
    e1 = x2 - x1;
    e1.Normalize();

    // Local z axis is normal to the triangle
    e3 = calculate_cross_product(x2 - x1, x3 - x1);
    Area = 0.5 * e3.Norm();
    e3.Normalize();

    if (Area < 0)
    {
        cout << "ThreeNodeAndesMembrane::initializeGeometry() -> Element # " << getTag() << " has A < 0!! " << endl;
    }

    //Local y axis is orthogonal to e1 and e3.
    e2 = calculate_cross_product(e3, e1);


    useThisCoordinateSystem(e1, e2, e3);
}

void ThreeNodeAndesMembrane::useThisCoordinateSystem(Vector e1, Vector e2, Vector e3)
{
    //Local-to-global transformation matrix
    for (int i = 0; i < 3; i++)
    {
        T_lg(i, 0) = e1(i);
        T_lg(i, 1) = e2(i);
        T_lg(i, 2) = e3(i);
    }

    //Matrix T_gl(3,3);
    //T_lg.Invert(T_gl);  //Compute global -> local transformation matrix

    // Project global nodal coordinates into local coordinates
    T_lg.Solve(theNodes[0]->getCrds() - x0, xl1);  // Solve T_lg * xl1 = x1
    T_lg.Solve(theNodes[1]->getCrds() - x0, xl2);  // Solve T_lg * xl2 = x2
    T_lg.Solve(theNodes[2]->getCrds() - x0, xl3);  // Solve T_lg * xl3 = x3

    // Node differences
    x12 = xl1(0) - xl2(0);
    x23 = xl2(0) - xl3(0);
    x31 = xl3(0) - xl1(0);
    y12 = xl1(1) - xl2(1);
    y23 = xl2(1) - xl3(1);
    y31 = xl3(1) - xl1(1);
}

void ThreeNodeAndesMembrane::calculate_E_planestress_and_beta0()
{
    // This condenses the E matrix which holds the elastic coefficients
    // into the plane-stress E matrix.
    Matrix E(6, 6);
    E = (*theMaterial)->getTangent();

    //Static condensation
    Matrix A(3, 3);
    Matrix B(3, 3);
    Matrix C(3, 3);

    int keep_these_dofs[3] = {0, 1, 3};
    int condensed_these_dofs[3] = {2, 4, 5};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            A(i, j) = E(keep_these_dofs[i], keep_these_dofs[j]);
            B(i, j) = E(keep_these_dofs[i], condensed_these_dofs[j]);
            C(i, j) = E(condensed_these_dofs[i], condensed_these_dofs[j]);
        }
    }

    Matrix C_inv(3, 3);
    C.Invert(C_inv); // Invert C and place result in C_inv

    E_planestress.Zero();

    // Compute condensed matrix
    // E_planestress = A - B*(C^-1)*B^T
    E_planestress = A;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int m = 0; m < 3; m ++)
            {
                for (int n = 0; n < 3; n ++)
                {
                    E_planestress(i, j) -= B(i, m) * C_inv(m, n) * B(j, n);
                }
            }
        }
    }

    // Calculate beta0
    // From Carlos Felippa's fortran code:

    double E11 =  E_planestress(0, 0);
    double E22 =  E_planestress(1, 1);
    double E33 =  E_planestress(2, 2);
    double E12 =  E_planestress(0, 1);
    double E13 =  E_planestress(0, 2);
    double E23 =  E_planestress(1, 2);

    double Edet = E11 * E22 * E33 + 2 * E12 * E13 * E23 - E11 * pow(E23, 2) - E22 * pow(E13, 2) - E33 * pow(E12, 2);

    double E11C11 = (-5 * E11 * pow(E12, 2) - 6 * pow(E12, 3) - 3 * E11 * pow(E13, 2) + 14 * E12 * pow(E13, 2) +
                     5 * pow(E11, 2) * E22 + 6 * E11 * E12 * E22 - 5 * pow(E12, 2) * E22 - 75 * pow(E13, 2) * E22 +
                     5 * E11 * pow(E22, 2) - 14 * E11 * E13 * E23 + 92 * E12 * E13 * E23 - 14 * E13 * E22 * E23 -
                     75 * E11 * pow(E23, 2) + 14 * E12 * pow(E23 , 2) - 3 * E22 * pow(E23, 2) + (3 * pow(E11, 2) + 82 * E11 * E22 +
                             3 * pow(E22, 2) - 4 * (6 * pow(E12, 2) + 5 * pow(E13, 2) - 6 * E13 * E23 + 5 * pow(E23, 2))) * E33 +
                     4 * (5 * E11 - 6 * E12 + 5 * E22) * pow(E33, 2)) / (128 * Edet);

    beta0 = max(2.0 / E11C11 - 1.5, 0.01);
}

void ThreeNodeAndesMembrane::initializeBetaArrays()
{
    // We only want to initialize these numbers once and for all elements of this class
    if (number_of_three_node_andes_membrane == 0)
    {

        // Membrane
        beta_membrane.Zero();
        beta_membrane(0) = 0;
        beta_membrane(1) = 1;
        beta_membrane(2) = 2;
        beta_membrane(3) = 1;
        beta_membrane(4) = 0;
        beta_membrane(5) = 1;
        beta_membrane(6) = -1;
        beta_membrane(7) = -1;
        beta_membrane(8) = -1;
        beta_membrane(9) = -2;
    }

    // Increment the number of shells
    number_of_three_node_andes_membrane++;
}

//===================================================================================
// Membrane
//===================================================================================

Matrix ThreeNodeAndesMembrane::getMembraneForceLumpingMatrix()
{
    Matrix L(3, 9); // This is the transpose of Felippa's article
    L.Zero();

    // Filling each row
    L(0, 0) = y23;
    L(2, 0) = -x23;
    L(1, 1) = -x23;
    L(2, 1) = y23;
    L(0, 2) = alpha_membrane * y23 * (-y31 + y12) / 6;
    L(1, 2) = alpha_membrane * (-x23) * (x31 - x12) / 6;
    L(2, 2) = alpha_membrane * (-x31 * y31 + x12 * y12) / 3;

    L(0, 3) = y31;
    L(2, 3) = -x31;
    L(1, 4) = -x31;
    L(2, 4) = y31;
    L(0, 5) = alpha_membrane * y31 * (-y12 + y23) / 6;
    L(1, 5) = alpha_membrane * (-x31) * (x12 - x23) / 6;
    L(2, 5) = alpha_membrane * (-x12 * y12 + x23 * y23) / 3;

    L(0, 6) = y12;
    L(2, 6) = -x12;
    L(1, 7) = -x12;
    L(2, 7) = y12;
    L(0, 8) = alpha_membrane * y12 * (-y23 + y31) / 6;
    L(1, 8) = alpha_membrane * (-x12) * (x23 - x31) / 6;
    L(2, 8) = alpha_membrane * (-x23 * y23 + x31 * y31) / 3;

    return 0.5 * thickness * L;
}

Matrix ThreeNodeAndesMembrane::getMembraneHierarchicalProjectionMatrix()
{
    Matrix T_thu(3, 9);
    T_thu.Zero();

    double x21 = -x12;
    double y21 = -y12;
    double x32 = -x23;
    double y32 = -y23;
    double x13 = -x31;
    double y13 = -y31;


    T_thu(0, 0) = x32 / (4 * Area);
    T_thu(1, 0) = x32 / (4 * Area);
    T_thu(2, 0) = x32 / (4 * Area);

    T_thu(0, 1) = y32 / (4 * Area);
    T_thu(1, 1) = y32 / (4 * Area);
    T_thu(2, 1) = y32 / (4 * Area);

    T_thu(0, 2) = 1;


    T_thu(0, 3) = x13 / (4 * Area);
    T_thu(1, 3) = x13 / (4 * Area);
    T_thu(2, 3) = x13 / (4 * Area);

    T_thu(0, 4) = y13 / (4 * Area);
    T_thu(1, 4) = y13 / (4 * Area);
    T_thu(2, 4) = y13 / (4 * Area);

    T_thu(1, 5) = 1;


    T_thu(0, 6) = x21 / (4 * Area);
    T_thu(1, 6) = x21 / (4 * Area);
    T_thu(2, 6) = x21 / (4 * Area);

    T_thu(0, 7) = y21 / (4 * Area);
    T_thu(1, 7) = y21 / (4 * Area);
    T_thu(2, 7) = y21 / (4 * Area);

    T_thu(2, 8) = 1;

    return T_thu;
}

Matrix ThreeNodeAndesMembrane::getMembraneNaturalStrainProjectionMatrix()
{
    Matrix T_e(3, 3);

    double x21 = -x12;
    double y21 = -y12;
    double x32 = -x23;
    double y32 = -y23;
    double x13 = -x31;
    double y13 = -y31;


    // From Felippa's code
    double ll21 =       x21 * x21 + y21 * y21;
    double ll32 =       x32 * x32 + y32 * y32;
    double ll13 =       x13 * x13 + y13 * y13;
    double tfac =       1.0 / (4.0 * Area * Area);

    T_e(0, 0) =    tfac * y23 * y13 * ll21;
    T_e(0, 1) =    tfac * y31 * y21 * ll32;
    T_e(0, 2) =    tfac * y12 * y32 * ll13;
    T_e(1, 0) =    tfac * x23 * x13 * ll21;
    T_e(1, 1) =    tfac * x31 * x21 * ll32;
    T_e(1, 2) =    tfac * x12 * x32 * ll13;
    T_e(2, 0) =    tfac * (y23 * x31 + x32 * y13) * ll21;
    T_e(2, 1) =    tfac * (y31 * x12 + x13 * y21) * ll32;
    T_e(2, 2) =    tfac * (y12 * x23 + x21 * y32) * ll13;

    return T_e;
}

Matrix ThreeNodeAndesMembrane::getMembraneBasicStiffness()
{
    //Initialize basic stiffness
    Matrix Kb(9, 9);

    //Condense E into plane stress matrix
    calculate_E_planestress_and_beta0();

    // Force lumping matrix
    Matrix L(3, 9);
    L = getMembraneForceLumpingMatrix();

    double Vol = thickness * Area;


    Kb.Zero();
    Kb.addMatrixTripleProduct(1.0, L, E_planestress, 1.0);


    return Kb / Vol;
}

Matrix ThreeNodeAndesMembrane::getMembraneHighOrderStiffness()
{
    Matrix Kh(9, 9);
    Matrix Ktheta(3, 3);

    Matrix Q1(3, 3), Q2(3, 3), Q3(3, 3), Q4(3, 3), Q5(3, 3), Q6(3, 3);

    double c12 = (2 * Area) / (3 * ( x12 * x12 + y12 * y12 ));
    double c23 = (2 * Area) / (3 * ( x23 * x23 + y23 * y23 ));
    double c31 = (2 * Area) / (3 * ( x31 * x31 + y31 * y31 ));

    // Q1
    Q1(0, 0) = beta_membrane(1) * c12;
    Q1(0, 1) = beta_membrane(2) * c12;
    Q1(0, 2) = beta_membrane(3) * c12;

    Q1(1, 0) = beta_membrane(4) * c23;
    Q1(1, 1) = beta_membrane(5) * c23;
    Q1(1, 2) = beta_membrane(6) * c23;

    Q1(2, 0) = beta_membrane(7) * c31;
    Q1(2, 1) = beta_membrane(8) * c31;
    Q1(2, 2) = beta_membrane(9) * c31;

    // Q2
    Q2(0, 0) = beta_membrane(9) * c12;
    Q2(0, 1) = beta_membrane(7) * c12;
    Q2(0, 2) = beta_membrane(8) * c12;

    Q2(1, 0) = beta_membrane(3) * c23;
    Q2(1, 1) = beta_membrane(1) * c23;
    Q2(1, 2) = beta_membrane(2) * c23;

    Q2(2, 0) = beta_membrane(6) * c31;
    Q2(2, 1) = beta_membrane(4) * c31;
    Q2(2, 2) = beta_membrane(5) * c31;

    // Q3
    Q3(0, 0) = beta_membrane(5) * c12;
    Q3(0, 1) = beta_membrane(6) * c12;
    Q3(0, 2) = beta_membrane(4) * c12;

    Q3(1, 0) = beta_membrane(8) * c23;
    Q3(1, 1) = beta_membrane(9) * c23;
    Q3(1, 2) = beta_membrane(7) * c23;

    Q3(2, 0) = beta_membrane(2) * c31;
    Q3(2, 1) = beta_membrane(3) * c31;
    Q3(2, 2) = beta_membrane(1) * c31;

    // Q4 Q5 and Q6
    Q4 = 0.5 * (Q1 + Q2);
    Q5 = 0.5 * (Q2 + Q3);
    Q6 = 0.5 * (Q3 + Q1);

    //Transpose Qs


    Matrix T_e = getMembraneNaturalStrainProjectionMatrix();
    Matrix Enat(3, 3);
    Enat.Zero();
    Enat.addMatrixTripleProduct(1.0, T_e, E_planestress, 1.0);  // (T_e)^T*E_planestress*T_e


    // Form high order matrix in Natural coordinates

    //Felippa's unrolled loop implementation
    // this is very slow in C++ because the array accesses are done through function
    // calling instead of pointers. It can be changed..... though....
    double x21 = -x12;
    double y21 = -y12;
    double x32 = -x23;
    double y32 = -y23;
    double x13 = -x31;
    double y13 = -y31;


    // This code calculates the Kh
    double  sfac =    0.75 * beta0 * Area;

    double  a4 =     Enat(0, 0) * Q4(0, 0) + Enat(0, 1) * Q4(1, 0) + Enat(0, 2) * Q4(2, 0);
    double  b4 =     Enat(1, 0) * Q4(0, 0) + Enat(1, 1) * Q4(1, 0) + Enat(1, 2) * Q4(2, 0);
    double  c4 =     Enat(2, 0) * Q4(0, 0) + Enat(2, 1) * Q4(1, 0) + Enat(2, 2) * Q4(2, 0);
    double  a5 =     Enat(0, 0) * Q5(0, 0) + Enat(0, 1) * Q5(1, 0) + Enat(0, 2) * Q5(2, 0);
    double  b5 =     Enat(1, 0) * Q5(0, 0) + Enat(1, 1) * Q5(1, 0) + Enat(1, 2) * Q5(2, 0);
    double  c5 =     Enat(2, 0) * Q5(0, 0) + Enat(2, 1) * Q5(1, 0) + Enat(2, 2) * Q5(2, 0);
    double  a6 =     Enat(0, 0) * Q6(0, 0) + Enat(0, 1) * Q6(1, 0) + Enat(0, 2) * Q6(2, 0);
    double  b6 =     Enat(1, 0) * Q6(0, 0) + Enat(1, 1) * Q6(1, 0) + Enat(1, 2) * Q6(2, 0);
    double  c6 =     Enat(2, 0) * Q6(0, 0) + Enat(2, 1) * Q6(1, 0) + Enat(2, 2) * Q6(2, 0);

    double  S11 =   (Q4(0, 0) * a4 + Q4(1, 0) * b4 + Q4(2, 0) * c4 + Q5(0, 0) * a5 + Q5(1, 0) * b5
                     + Q5(2, 0) * c5 + Q6(0, 0) * a6 + Q6(1, 0) * b6 + Q6(2, 0) * c6) * sfac;
    double  S12 =   (Q4(0, 1) * a4 + Q4(1, 1) * b4 + Q4(2, 1) * c4 + Q5(0, 1) * a5 + Q5(1, 1) * b5
                     + Q5(2, 1) * c5 + Q6(0, 1) * a6 + Q6(1, 1) * b6 + Q6(2, 1) * c6) * sfac;
    double  S13 =   (Q4(0, 2) * a4 + Q4(1, 2) * b4 + Q4(2, 2) * c4 + Q5(0, 2) * a5 + Q5(1, 2) * b5
                     + Q5(2, 2) * c5 + Q6(0, 2) * a6 + Q6(1, 2) * b6 + Q6(2, 2) * c6) * sfac;

    a4 =     Enat(0, 0) * Q4(0, 1) + Enat(0, 1) * Q4(1, 1) + Enat(0, 2) * Q4(2, 1);
    b4 =     Enat(1, 0) * Q4(0, 1) + Enat(1, 1) * Q4(1, 1) + Enat(1, 2) * Q4(2, 1);
    c4 =     Enat(2, 0) * Q4(0, 1) + Enat(2, 1) * Q4(1, 1) + Enat(2, 2) * Q4(2, 1);
    a5 =     Enat(0, 0) * Q5(0, 1) + Enat(0, 1) * Q5(1, 1) + Enat(0, 2) * Q5(2, 1);
    b5 =     Enat(1, 0) * Q5(0, 1) + Enat(1, 1) * Q5(1, 1) + Enat(1, 2) * Q5(2, 1);
    c5 =     Enat(2, 0) * Q5(0, 1) + Enat(2, 1) * Q5(1, 1) + Enat(2, 2) * Q5(2, 1);
    a6 =     Enat(0, 0) * Q6(0, 1) + Enat(0, 1) * Q6(1, 1) + Enat(0, 2) * Q6(2, 1);
    b6 =     Enat(1, 0) * Q6(0, 1) + Enat(1, 1) * Q6(1, 1) + Enat(1, 2) * Q6(2, 1);
    c6 =     Enat(2, 0) * Q6(0, 1) + Enat(2, 1) * Q6(1, 1) + Enat(2, 2) * Q6(2, 1);

    double  S22 =   (Q4(0, 1) * a4 + Q4(1, 1) * b4 + Q4(2, 1) * c4 + Q5(0, 1) * a5 + Q5(1, 1) * b5
                     + Q5(2, 1) * c5 + Q6(0, 1) * a6 + Q6(1, 1) * b6 + Q6(2, 1) * c6) * sfac;
    double   S23 =   (Q4(0, 2) * a4 + Q4(1, 2) * b4 + Q4(2, 2) * c4 + Q5(0, 2) * a5 + Q5(1, 2) * b5
                      + Q5(2, 2) * c5 + Q6(0, 2) * a6 + Q6(1, 2) * b6 + Q6(2, 2) * c6) * sfac;

    a4 =     Enat(0, 0) * Q4(0, 2) + Enat(0, 1) * Q4(1, 2) + Enat(0, 2) * Q4(2, 2);
    b4 =     Enat(1, 0) * Q4(0, 2) + Enat(1, 1) * Q4(1, 2) + Enat(1, 2) * Q4(2, 2);
    c4 =     Enat(2, 0) * Q4(0, 2) + Enat(2, 1) * Q4(1, 2) + Enat(2, 2) * Q4(2, 2);
    a5 =     Enat(0, 0) * Q5(0, 2) + Enat(0, 1) * Q5(1, 2) + Enat(0, 2) * Q5(2, 2);
    b5 =     Enat(1, 0) * Q5(0, 2) + Enat(1, 1) * Q5(1, 2) + Enat(1, 2) * Q5(2, 2);
    c5 =     Enat(2, 0) * Q5(0, 2) + Enat(2, 1) * Q5(1, 2) + Enat(2, 2) * Q5(2, 2);
    a6 =     Enat(0, 0) * Q6(0, 2) + Enat(0, 1) * Q6(1, 2) + Enat(0, 2) * Q6(2, 2);
    b6 =     Enat(1, 0) * Q6(0, 2) + Enat(1, 1) * Q6(1, 2) + Enat(1, 2) * Q6(2, 2);
    c6 =     Enat(2, 0) * Q6(0, 2) + Enat(2, 1) * Q6(1, 2) + Enat(2, 2) * Q6(2, 2);

    double  S33 =   (Q4(0, 2) * a4 + Q4(1, 2) * b4 + Q4(2, 2) * c4 + Q5(0, 2) * a5 + Q5(1, 2) * b5
                     + Q5(2, 2) * c5 + Q6(0, 2) * a6 + Q6(1, 2) * b6 + Q6(2, 2) * c6) * sfac;

    double  Ssum1 =    (S11 + S12 + S13) / (4 * Area);
    double  Ssum2 =    (S12 + S22 + S23) / (4 * Area);
    double  Ssum3 =    (S13 + S23 + S33) / (4 * Area);
    double  Ssum123 =  (Ssum1 + Ssum2 + Ssum3) / (4 * Area);

    Kh(0, 0) =  Ssum123 * x32 * x32;
    Kh(0, 1) =  Ssum123 * x32 * y32;
    Kh(0, 2) =  Ssum1 * x32;
    Kh(0, 3) =  Ssum123 * x13 * x32;
    Kh(0, 4) =  Ssum123 * x32 * y13;
    Kh(0, 5) =  Ssum2 * x32;
    Kh(0, 6) =  Ssum123 * x21 * x32;
    Kh(0, 7) =  Ssum123 * x32 * y21;
    Kh(0, 8) =  Ssum3 * x32;
    Kh(1, 1) =  Ssum123 * y32 * y32;
    Kh(1, 2) =  Ssum1 * y32;
    Kh(1, 3) =  Ssum123 * x13 * y32;
    Kh(1, 4) =  Ssum123 * y13 * y32;
    Kh(1, 5) =  Ssum2 * y32;
    Kh(1, 6) =  Ssum123 * x21 * y32;
    Kh(1, 7) =  Ssum123 * y21 * y32;
    Kh(1, 8) =  Ssum3 * y32;
    Kh(2, 2) =  S11;
    Kh(2, 3) =  Ssum1 * x13;
    Kh(2, 4) =  Ssum1 * y13;
    Kh(2, 5) =  S12;
    Kh(2, 6) =  Ssum1 * x21;
    Kh(2, 7) =  Ssum1 * y21;
    Kh(2, 8) =  S13;
    Kh(3, 3) =  Ssum123 * x13 * x13;
    Kh(3, 4) =  Ssum123 * x13 * y13;
    Kh(3, 5) =  Ssum2 * x13;
    Kh(3, 6) =  Ssum123 * x13 * x21;
    Kh(3, 7) =  Ssum123 * x13 * y21;
    Kh(3, 8) =  Ssum3 * x13;
    Kh(4, 4) =  Ssum123 * y13 * y13;
    Kh(4, 5) =  Ssum2 * y13;
    Kh(4, 6) =  Ssum123 * x21 * y13;
    Kh(4, 7) =  Ssum123 * y13 * y21;
    Kh(4, 8) =  Ssum3 * y13;
    Kh(5, 5) =  S22;
    Kh(5, 6) =  Ssum2 * x21;
    Kh(5, 7) =  Ssum2 * y21;
    Kh(5, 8) =  S23;
    Kh(6, 6) =  Ssum123 * x21 * x21;
    Kh(6, 7) =  Ssum123 * x21 * y21;
    Kh(6, 8) =  Ssum3 * x21;
    Kh(7, 7) =  Ssum123 * y21 * y21         ;
    Kh(7, 8) =  Ssum3 * y21;
    Kh(8, 8) =  S33;

    // Fill in the rest of the matrix by symmetry
    for (int i = 1; i < 9; i++ )
    {
        for (int j = 0; j < i; j++ )
        {
            Kh(i, j) = Kh(j, i);
        }
    }

    return Kh * thickness;
}


double ThreeNodeAndesMembrane::getArea() const
{
    return Area;
}

bool ThreeNodeAndesMembrane::gotMass() const
{
    return rho > 0.0 ? true : false;
}
#endif











