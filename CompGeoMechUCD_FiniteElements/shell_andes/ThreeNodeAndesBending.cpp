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
(vii)  C. A. Felippa and P. G. Bergan, A triangular plate bending element based on an energy-orthogonal free
formulation, Comp. Meth. Appl. Mech. Engrg., 61, 129–160, 1987.

Obtainable as of July 22 2012 at http://www.colorado.edu/engineering/CAS/Felippa.d/FelippaHome.d/Home.html

These documents all mirror published works in indexed journals.
*/

#ifndef ThreeNodeAndesBending_CPP
#define ThreeNodeAndesBending_CPP

#include <iomanip>
#include <iostream>

using namespace std;

#include "ThreeNodeAndesBending.h"
#include <Vector.h>
#include <cmath>
#include <limits>

// Initialize static variables

unsigned int ThreeNodeAndesBending::number_of_three_node_andes_bending = 0;
Matrix ThreeNodeAndesBending::Mq(9, 9);

//===================================================================================
// Element Constructors / Destructors
//===================================================================================

ThreeNodeAndesBending::ThreeNodeAndesBending()
    :
    Element(0, ELE_TAG_ThreeNodeAndesBending ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18),
    thickness(0.0),
    xl1(3), xl2(3), xl3(3), x0(3), T_lg(3, 3),
    rho(0.0), E_planestress(3, 3), is_stiffness_calculated(false)
{
    // zero node pointers
    for (int i = 0; i < 3; i++)
    {
        theNodes[i] = 0;
    }


    if (number_of_three_node_andes_bending == 0)
    {
        initializeMq();
    }

    number_of_three_node_andes_bending++;
}


ThreeNodeAndesBending::ThreeNodeAndesBending(int element_number,
        int node_numb_1, int node_numb_2, int node_numb_3,
        double t,
        NDMaterial *Globalmmodel)
    :
    Element(element_number, ELE_TAG_ThreeNodeAndesBending ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18),
    thickness(t),
    xl1(3), xl2(3), xl3(3), x0(3), T_lg(3, 3),
    rho(0.0), E_planestress(3, 3), is_stiffness_calculated(false)
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
        cerr << " ThreeNodeAndesBending::ThreeNodeAndesBending -- failed to allocate material model pointer\n";
        exit(-1);
    }

    *theMaterial = Globalmmodel->getCopy();

    if (*theMaterial == 0)
    {
        cerr << " ThreeNodeAndesBending::ThreeNodeAndesBending -- failed to allocate material model pointer\n";
        exit(-1);
    }

    //
    rho = Globalmmodel->getRho();


    if (rho < 0)
    {
        cerr << " ThreeNodeAndesBending::ThreeNodeAndesBending -- rho = " << rho << "\n";
        exit(-1);
    }


    if (number_of_three_node_andes_bending == 0)
    {
        initializeMq();
    }

    number_of_three_node_andes_bending++;
}


ThreeNodeAndesBending::ThreeNodeAndesBending(int element_number,
        int node_numb_1, int node_numb_2, int node_numb_3,
        double t,
        NDMaterial **material)
    :
    Element(element_number, ELE_TAG_ThreeNodeAndesBending ),
    connectedExternalNodes(3),
    K(18, 18), M(18, 18),
    P(18), Q(18),
    thickness(t),
    xl1(3), xl2(3), xl3(3), x0(3), T_lg(3, 3),
    rho(0.0), E_planestress(3, 3), is_stiffness_calculated(false)
{
    // Set connected external node IDs
    connectedExternalNodes(0) = node_numb_1;
    connectedExternalNodes(1) = node_numb_2;
    connectedExternalNodes(2) = node_numb_3;

    theMaterial = material;

    if ((theMaterial == NULL) | (*theMaterial == NULL))
    {
        cerr << " ThreeNodeAndesBending::ThreeNodeAndesBending -- received a NULL material model pointer\n";
        exit(-1);
    }

    rho = (*theMaterial)->getRho();


    if (rho < 0)
    {
        cerr << " ThreeNodeAndesBending::ThreeNodeAndesBending -- rho = " << rho << "\n";
        exit(-1);
    }



    if (number_of_three_node_andes_bending == 0)
    {
        initializeMq();
    }

    number_of_three_node_andes_bending++;

}


ThreeNodeAndesBending::~ThreeNodeAndesBending()
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

int ThreeNodeAndesBending::getNumExternalNodes() const
{
    return 3;
}

const ID &ThreeNodeAndesBending::getExternalNodes()
{
    return connectedExternalNodes;
}

Node **ThreeNodeAndesBending::getNodePtrs(void)
{
    return theNodes;
}

int ThreeNodeAndesBending::getNumDOF()
{
    return 18;
}

void ThreeNodeAndesBending::setDomain(Domain *theDomain)
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
            cerr << "FATAL ERROR ThreeNodeAndesBending (tag: " << this->getTag() << "), node not found in domain\n";
            exit(-1);
        }

        int dofNd1 = theNodes[0]->getNumberDOF();
        int dofNd2 = theNodes[1]->getNumberDOF();
        int dofNd3 = theNodes[2]->getNumberDOF();

        if (dofNd1 != 6 || dofNd2 != 6 || dofNd3 != 6)
        {
            cerr << "FATAL ERROR ThreeNodeAndesBending (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit(-1);
        }

        this->DomainComponent::setDomain(theDomain);

        initializeGeometry();
    }
}

int ThreeNodeAndesBending::commitState ()
{
    return 0;
}

int ThreeNodeAndesBending::revertToLastCommit ()
{
    return 0;
}

int ThreeNodeAndesBending::revertToStart ()
{
    return 0;
}

int ThreeNodeAndesBending::update(void)
{
    return 0;
}

const Matrix &ThreeNodeAndesBending::getTangentStiff ()
{
    // Since this is a linear element, the stiffness is calculated only once.
    if (!is_stiffness_calculated)
    {
        int I, J;

        // Form completestiffness
        Matrix Kb_bending(9, 9);        // Basic bending stiffness
        Matrix Kh_bending(9, 9);        // High-order bending stiffness
        Matrix Klocal(18, 18);          // Total (membrane + bending) stiffness in local coordinates
        Matrix TLG(18, 18);             // Local-to-global transformation matrix

        //Get matrices
        Kb_bending  = getBendingBasicStiffness();
        Kh_bending  = getBendingHighOrderStiffness();

        // Expand matrices to local complete stiffness
        //   DOFS   --->        ux1, uy1, uz1, rx1, ry1, rz1  |  ux2, uy2, uz2, rx2, ry2, rz2  |  ux3, uy3, uz3, rx3, ry3, rz3
        //int membrane_dofs[9] = {  0,   1,                  5,      6,   7,                 11,     12,  13,                 17 };
        int  bending_dofs[9] = {            2,   3,   4,                     8,   9,  10,                    14,  15,  16      };

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // Expand bending stiffness to local bending DOFS.
                I = bending_dofs[i];
                J = bending_dofs[j];
                Klocal(I, J) = Kb_bending(i, j) + Kh_bending(i, j);
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

const Matrix &ThreeNodeAndesBending::getInitialStiff()
{
    return getTangentStiff();
}

const Matrix &ThreeNodeAndesBending::getMass ()
{
    if (!is_mass_calculated)
    {
        double x_1 = this->xl1[0];
        double y_1 = this->xl1[1];

        double x_2 = this->xl2[0];
        double y_2 = this->xl2[1];

        double x_3 = this->xl3[0];
        double y_3 = this->xl3[1];
        //
        //        cout << endl << "Sub-element of Element #" << this->getTag() << endl;
        //        cout << "   Node 1 ("  << xl1[0] << ", " << xl1[1] << ", " << xl1[2] << ")" << endl;
        //        cout << "   Node 2 ("  << xl2[0] << ", " << xl2[1] << ", " << xl2[2] << ")" << endl;
        //        cout << "   Node 3 ("  << xl3[0] << ", " << xl3[1] << ", " << xl3[2] << ")" << endl;

        Matrix Gp(9, 9);
        Matrix Hp(9, 9);

        Gp(0, 0) = 2;
        Gp(0, 1) = 1;
        Gp(0, 2) = 0;
        Gp(0, 3) = 1;
        Gp(0, 4) = 0;
        Gp(0, 5) = 1;
        Gp(0, 6) = 1;
        Gp(0, 7) = 0;
        Gp(0, 8) = -1;

        Gp(1, 0) = (-x_1 - x_2 + 2 * x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(1, 1) = (2 * x_1 - x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(1, 2) = (-x_1 + 2 * x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(1, 3) = 2 * (-x_1 - x_2 + 2 * x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(1, 4) = 0;
        Gp(1, 5) = 2 * (x_1 - 2 * x_2 + x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(1, 6) = 3 * (-x_1 - x_2 + 2 * x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(1, 7) = 0;
        Gp(1, 8) = 3 * (-x_1 + 2 * x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);

        Gp(2, 0) = (-y_1 - y_2 + 2 * y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(2, 1) = (2 * y_1 - y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(2, 2) = (-y_1 + 2 * y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(2, 3) = 2 * (-y_1 - y_2 + 2 * y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(2, 4) = 0;
        Gp(2, 5) = 2 * (y_1 - 2 * y_2 + y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(2, 6) = 3 * (-y_1 - y_2 + 2 * y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(2, 7) = 0;
        Gp(2, 8) = 3 * (-y_1 + 2 * y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);

        Gp(3, 0) = 0;
        Gp(3, 1) = 2;
        Gp(3, 2) = 1;
        Gp(3, 3) = 1;
        Gp(3, 4) = 1;
        Gp(3, 5) = 0;
        Gp(3, 6) = -1;
        Gp(3, 7) = 1;
        Gp(3, 8) = 0;

        Gp(4, 0) = (-x_1 - x_2 + 2 * x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(4, 1) = (2 * x_1 - x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(4, 2) = (-x_1 + 2 * x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(4, 3) = 2 * (x_1 + x_2 - 2 * x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(4, 4) = 2 * (2 * x_1 - x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(4, 5) = 0;
        Gp(4, 6) = 3 * (-x_1 - x_2 + 2 * x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(4, 7) = 3 * (2 * x_1 - x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(4, 8) = 0;

        Gp(5, 0) = (-y_1 - y_2 + 2 * y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(5, 1) = (2 * y_1 - y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(5, 2) = (-y_1 + 2 * y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(5, 3) = 2 * (y_1 + y_2 - 2 * y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(5, 4) = 2 * (2 * y_1 - y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(5, 5) = 0;
        Gp(5, 6) = 3 * (-y_1 - y_2 + 2 * y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(5, 7) = 3 * (2 * y_1 - y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(5, 8) = 0;

        Gp(6, 0) = 1;
        Gp(6, 1) = 0;
        Gp(6, 2) = 2;
        Gp(6, 3) = 0;
        Gp(6, 4) = 1;
        Gp(6, 5) = 1;
        Gp(6, 6) = 0;
        Gp(6, 7) = -1;
        Gp(6, 8) = 1;

        Gp(7, 0) = (-x_1 - x_2 + 2 * x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(7, 1) = (2 * x_1 - x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(7, 2) = (-x_1 + 2 * x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(7, 3) = 0;
        Gp(7, 4) = 2 * (-2 * x_1 + x_2 + x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(7, 5) = 2 * (-x_1 + 2 * x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(7, 6) = 0;
        Gp(7, 7) = 3 * (2 * x_1 - x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(7, 8) = 3 * (-x_1 + 2 * x_2 - x_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);

        Gp(8, 0) = (-y_1 - y_2 + 2 * y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(8, 1) = (2 * y_1 - y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(8, 2) = (-y_1 + 2 * y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(8, 3) = 0;
        Gp(8, 4) = 2 * (-2 * y_1 + y_2 + y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(8, 5) = 2 * (-y_1 + 2 * y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(8, 6) = 0;
        Gp(8, 7) = 3 * (2 * y_1 - y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);
        Gp(8, 8) = 3 * (-y_1 + 2 * y_2 - y_3) / (x_1 * y_2 - x_1 * y_3 - x_2 * y_1 + x_2 * y_3 + x_3 * y_1 - x_3 * y_2);

        Gp.Invert(Hp);

        // Get local matrix
        Matrix Ml(9, 9);
        Ml.Zero();
        Ml.addMatrixTripleProduct(1.0, Hp, Mq, rho * Area * thickness);


        //Expand local matrix to local complete dofs.
        Matrix Mlocal(18, 18);
        Mlocal.Zero();

        // Expand matrices to local complete mass
        //   DOFS   --->          ux1, uy1, uz1, rx1, ry1, rz1  |  ux2, uy2, uz2, rx2, ry2, rz2  |  ux3, uy3, uz3, rx3, ry3, rz3
        //int membrane_dofs[9] = {  0,   1,                  5,      6,   7,                 11,     12,  13,                 17 };
        int  bending_dofs[9] = {            2,   3,   4,                     8,   9,  10,                    14,  15,  16      };
        int I, J;

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // Expand bending mass to local bending DOFS.
                I = bending_dofs[i];
                J = bending_dofs[j];
                Mlocal(I, J) = Ml(i, j);
            }
        }

        // Form TLG Transform local -> global
        Matrix TLG(18, 18);
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

        // Compute total stiffness as M = TLG^T * Mlocal *TLG
        M.Zero();

        M.addMatrixTripleProduct(1.0, TLG, Mlocal, 1.0);

        is_mass_calculated = true;
    }

    return M;
}

void ThreeNodeAndesBending::zeroLoad ()
{
    Q.Zero();
}

int ThreeNodeAndesBending::addLoad(ElementalLoad *theLoad, double loadFactor)
{

    return 0;
}

int ThreeNodeAndesBending::addInertiaLoadToUnbalance(const Vector &accel)

{
    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV(accel);
    const Vector &Raccel2 = theNodes[1]->getRV(accel);
    const Vector &Raccel3 = theNodes[2]->getRV(accel);

    if (6 != Raccel1.Size() || 6 != Raccel2.Size() || 6 != Raccel3.Size() )
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

    return 0;
}

const Vector &ThreeNodeAndesBending::getResistingForce ()
{
    //cout << "Called getResistingForce() for ThreeNodeAndesBending # " << getTag() << endl;

    Vector NodalDisplacements(18);
    Vector disp_i(6);

    for (int node = 0; node < 3; node++)
    {
        Node *node_i = theNodes[node];

        disp_i = node_i->getDisp();
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

const Vector &ThreeNodeAndesBending::getResistingForceIncInertia ()
{
    Vector NodalDisplacements(18);
    Vector NodalAccelerations(18);

    Vector disp_i(6);
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

    // Add nodal equivalent loads from surface or body loads
    P += Q;

    return P;
}

int ThreeNodeAndesBending::sendSelf (int commitTag, Channel &theChannel)
{
    cout << "ThreeNodeAndesBending::sendSelf ()" << endl;
    return 0;
}

int ThreeNodeAndesBending::receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    cout << "ThreeNodeAndesBending::receiveSelf ()" << endl;
    return 0;
}

void ThreeNodeAndesBending::Print(ostream &s, int flag = 0)
{

}

// Response* ThreeNodeAndesBending::setResponse (const char** argv, int argc, Information& eleInformation)
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

// int ThreeNodeAndesBending::getResponse (int responseID, Information& eleInformation)
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

Matrix ThreeNodeAndesBending::returnMass(void)
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
void ThreeNodeAndesBending::initializeGeometry()
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
        cout << "ThreeNodeAndesBending::initializeGeometry() -> Element # " << getTag() << " has A < 0!! " << endl;
    }

    //Local y axis is orthogonal to e1 and e3.
    e2 = calculate_cross_product(e3, e1);

    useThisCoordinateSystem(e1, e2, e3);

}

void ThreeNodeAndesBending::useThisCoordinateSystem(Vector e1, Vector e2, Vector e3)
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

    //    cerr << "\n Node1 = \n" << theNodes[0]->getCrds() << "\n";
    //    cerr << "\n Node2 = \n" << theNodes[1]->getCrds() << "\n";
    //    cerr << "\n Node3 = \n" << theNodes[2]->getCrds() << "\n";
    //    cerr << "\n T_lg = \n" << T_lg << "\n";
    //    cerr << "\n xl1 = \n" << xl1 << "\n";
    //    cerr << "\n xl2 = \n" << xl2 << "\n";
    //    cerr << "\n xl3 = \n" << xl3 << "\n";
}

void ThreeNodeAndesBending::calculate_E_planestress()
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

    E_planestress(2, 2) = E_planestress(2, 2) / 4.0;
    E_planestress(0, 2) = E_planestress(0, 2) / 2.0;
    E_planestress(1, 2) = E_planestress(1, 2) / 2.0;
    E_planestress(2, 0) = E_planestress(2, 0) / 2.0;
    E_planestress(2, 1) = E_planestress(2, 1) / 2.0;
}


//===================================================================================
// Bending
//===================================================================================

Matrix ThreeNodeAndesBending::getBendingBasicStiffness()
{
    //    // Original implementation
    //    Matrix L(3,9), D(3,3), Kb(9,9);
    //
    //    L = getBendingForceLumpingMatrix();
    //    D = E_planestress*(thickness*thickness*thickness/12);
    //
    //    Kb.Zero();
    //    Kb.addMatrixTripleProduct(1.0, L, D, 1.0);
    //
    //    return Kb / Area;
    //


    // =====================================================================================
    // Other implementation (ported from code by Sasa Stosic)
    // =====================================================================================

    // Jose: Original comments in Serbian were translated with Google Translate for pseudoclarity.
    //
    //
    //
    // Racuna i nalazi lokalnu matricu 9x9 u kojoj su pomeranja svih 9
    //
    // matrica C je matrica krutosti preseka sa osu X kao osu koordinatnog sistema
    //
    // C je takva da je treca vrsta pomnozena sa 2 ( ovo je znacajno jer je kod mene ova vrsta
    // deljena sa 2 jer vezujem sigma-epsilon a ovde ocekuju sigma-gamma
    //    -------------------------------------------------------------------
    //    Izrazi se mogu naci u matrci (56) na strani 233 rada na osnovu koga su izvedeni
    //    ---------------------------------------------------------------------
    //
    // [[ Google translate:  Account and finds the local 9x9 matrix in which the displacement of 9
    //
    // Matrix stiffness matrix C is the intersection with the X axis as the axis of the coordinate system
    //
    // C is such that a third species multiplied by 2 (this is important to me because this kind of
    // Shared by 2 because bind sigma-epsilon and here expect sigma-gamma
    //     ------------------------------------------------ -------------------
    //     The terms can be found in matrci (56) on page 233 of whom are based on the derived
    //     -------------------------------------------------- -------------------  ]]
    //
    //
    // Ovo bi trebalo da bude bolje ali se kod primera PLATE_10x10_B javlja
    // glupost ( nisam uspeo da nadjem zasto ) pa se vracam na linearno koje
    // do sad nije davalo gluposti,
    //
    //
    // [[ Google translate:   This is supposed to be better but in the example occurs PLATE_10x10_B
    // Stupid (I could not find out why) and returns to the linear
    // So far gave no nonsense, ]]



    // Geometric data
    double x21 = -x12;
    double y21 = -y12;
    double x32 = -x23;
    double y32 = -y23;
    double x13 = -x31;
    double y13 = -y31;

    double l12 = sqrt(x12 * x12 + y12 * y12);
    double l23 = sqrt(x23 * x23 + y23 * y23);
    double l31 = sqrt(x31 * x31 + y31 * y31);

    double c12 = x21 / l12;
    double c23 = x32 / l23;
    double c31 = x13 / l31;
    double s12 = y21 / l12;
    double s23 = y32 / l23;
    double s31 = y13 / l31;


    Matrix L(3, 9);
    Matrix Kb(9, 9);

    L(0, 0) = -c12 * s12 + c31 * s31;
    L(1, 0) = -c31 * s31 + c12 * s12;
    L(2, 0) = (s31 * s31 - c31 * c31) - (s12 * s12 - c12 * c12);

    L(0, 1) = (s12 * s12 * x12 + s31 * s31 * x31) / 2.0;
    L(1, 1) = (c12 * c12 * x12 + c31 * c31 * x31) / 2.0;
    L(2, 1) = c12 * c12 * y21 + c31 * c31 * y13;

    L(0, 2) = (s12 * s12 * y12 + s31 * s31 * y31) / 2.0;
    L(1, 2) = (c12 * c12 * y12 + c31 * c31 * y31) / 2.0;
    L(2, 2) = s12 * s12 * x21 + s31 * s31 * x13;
    //--------------------------------------------------
    L(0, 3) = -c23 * s23 + c12 * s12;
    L(1, 3) = -c12 * s12 + c23 * s23;
    L(2, 3) = (s12 * s12 - c12 * c12) - (s23 * s23 - c23 * c23);

    L(0, 4) = (s12 * s12 * x12 + s23 * s23 * x23) / 2.0;
    L(1, 4) = (c12 * c12 * x12 + c23 * c23 * x23) / 2.0;
    L(2, 4) = c12 * c12 * y21 + c23 * c23 * y32;

    L(0, 5) = (s12 * s12 * y12 + s23 * s23 * y23) / 2.0;
    //  L(1,5) = (c12*c12*y12 + c23*c23*y32)/2.0;
    L(1, 5) = (c12 * c12 * y12 + c23 * c23 * y23) / 2.0; //- Izmena 11-05-2002 [[ Google translate: changed 11-05-2002]]
    L(2, 5) = s12 * s12 * x21 + s23 * s23 * x32;
    //--------------------------------------------------
    L(0, 6) = -c31 * s31 + c23 * s23;
    L(1, 6) = -c23 * s23 + c31 * s31;
    L(2, 6) = (s23 * s23 - c23 * c23) - (s31 * s31 - c31 * c31);

    L(0, 7) = (s23 * s23 * x23 + s31 * s31 * x31) / 2.0;
    L(1, 7) = (c23 * c23 * x23 + c31 * c31 * x31) / 2.0;
    // Izmena 16-jan-2003 ovo je bilo kod mene :
    // L(7,2,7) = c23*c23*y32 + c31*c31*y31; // L(8,3) = c23*c23*y23 + c31*c31*y13;
    //         // Bilo je pa nije valjalo i treba y32
    // A u Radimpex-u je bilo:
    L(2, 7) = c23 * c23 * y32 + c31 * c31 * y13; //# odnosno permutovan je index y31 u y13
    // Treba proveriti navedeni primer PLATE_10x10_B jer je kod mene radilo
    // A sad je postavljeno kao u RadImpexu.

    L(0, 8) = (s23 * s23 * y23 + s31 * s31 * y31) / 2.0;
    L(1, 8) = (c23 * c23 * y23 + c31 * c31 * y31) / 2.0;
    L(2, 8) = s23 * s23 * x32 + s31 * s31 * x13;

    // U odnosu na izraze izvedene u Mathematici u file-u LMatrica.nb, svi clanovi
    // u vrstama 2,3,5,6,8 i 9 imaju negativne vrednosti odnosno treba proveriti
    // konvenciju o znaku momenata savijanja.
    //
    //  Lt = L.transpose();
    //
    // Modifikuje matricu C:
    // C(1,3) = C(1,3)/2 ; C(2,3) = C(2,3)/2 ; C(3,3)  =  C(3,3)/2;
    // To sad radi rutina koja poziva
    //
    //
    //    C1 = C *(1./Ar) ;
    //    Kb = C1*Lt;
    //    Kb = L*Kb;
    // cout<<"Kb = " <<Kb.OutString();
    // cout<<"det Kb = ";
    // Kb.eigenvalues().print();


    Kb.Zero();
    calculate_E_planestress();

    Kb.addMatrixTripleProduct(1.0, L, E_planestress * thickness * thickness * thickness / 12 / Area, 1.0);

    return Kb;
}

Matrix ThreeNodeAndesBending::getBendingHighOrderStiffness()
{
    Matrix Kh(9, 9);
    Kh.Zero();

    // Geometric data
    double x21 = -x12;
    double y21 = -y12;
    double x32 = -x23;
    double y32 = -y23;
    double x13 = -x31;
    double y13 = -y31;

    double l12 = sqrt(x12 * x12 + y12 * y12);
    double l23 = sqrt(x23 * x23 + y23 * y23);
    double l31 = sqrt(x31 * x31 + y31 * y31);
    double k_A = 4.0 * Area * Area;

    Matrix T(3, 3);
    T.Zero();

    T(0, 0) = y23 * y13 / k_A;
    T(0, 1) = y31 * y21 / k_A;
    T(0, 2) = y12 * y32 / k_A;
    T(1, 0) = x23 * x13 / k_A;
    T(1, 1) = x31 * x21 / k_A;
    T(1, 2) = x12 * x32 / k_A;
    T(2, 0) = (y23 * x31 + x32 * y13) / k_A;
    T(2, 1) = (y31 * x12 + x13 * y21) / k_A;
    T(2, 2) = (y12 * x23 + x21 * y32) / k_A;

    Matrix D(3, 3);
    D.Zero();

    D.addMatrixTripleProduct(1.0, T, E_planestress * thickness * thickness * thickness / 12, 1.0);

    // D = T.transpose()*(C*T);
    // D = T.inverse()*(C*T);
    // Ovo posle kad proradi prebaci preko promenjivih da brze radi
    // [[ Google translate: This starts to work after you get over the variables that work fast ]]

    double La12 = (x12 * x13 + y12 * y13) / pow(l12, 2);
    double La23 = (x23 * x21 + y23 * y21) / pow(l23, 2);
    double La31 = (x31 * x32 + y31 * y32) / pow(l31, 2);

    double B11 = 2.0 * (La12 * La12 - La12 + 1);
    double B22 = 2.0 * (La23 * La23 - La23 + 1);
    double B33 = 2.0 * (La31 * La31 - La31 + 1);

    double B12 = (2.0 - La12) * La23 - La12 - 1.0;
    double B23 = (2.0 - La23) * La31 - La23 - 1.0;
    double B13 = (2.0 - La31) * La12 - La31 - 1.0;

    double r11 = B11 * D(0, 0);
    double r12 = B12 * D(0, 1);
    double r13 = B13 * D(0, 2);
    double r22 = B22 * D(1, 1);
    double r23 = B23 * D(1, 2);
    double r33 = B33 * D(2, 2);

    //matrix_m Kh(9,9,0.0);

    Kh(0, 0) = 4.*(r33 - 2 * r13 + r11);
    Kh(0, 1) = 2.*((r11 - r13) * y21 + (r13 - r33) * y13);
    Kh(0, 2) = 2.*((r11 - r13) * x12 + (r13 - r33) * x31);
    Kh(0, 3) = 4.*( - r23 + r13 + r12 - r11);
    Kh(0, 4) = 2.*((r12 - r23) * y32 + (r11 - r13) * y21);
    Kh(0, 5) = 2.*((r12 - r23) * x23 + (r11 - r13) * x12);
    Kh(0, 6) = 4.*( - r33 + r23 + r13 - r12);
    Kh(0, 7) = 2.*((r12 - r23) * y32 + (r13 - r33) * y13);
    Kh(0, 8) = 2.*((r12 - r23) * x23 + (r13 - r33) * x31);

    Kh(1, 0) = Kh(0, 1);
    Kh(1, 1) = r11 * pow(y21, 2) + 2 * r13 * y13 * y21 + r33 * pow(y13, 2);
    Kh(1, 2) = (r11 * x21 + r13 * x13) * y12 + (r13 * x21 + r33 * x13) * y31;
    Kh(1, 3) = 2.*((r12 - r11) * y21 + (r23 - r13) * y13);
    Kh(1, 4) = (r12 * y21 + r23 * y13) * y32 + r11 * pow(y21, 2) + r13 * y13 * y21;
    Kh(1, 5) = (r12 * x32 + r11 * x21) * y12 + (r23 * x32 + r13 * x21) * y31;
    Kh(1, 6) = 2.*((r13 - r12) * y21 + (r33 - r23) * y13);
    Kh(1, 7) = (r12 * y21 + r23 * y13) * y32 + r13 * y13 * y21 + r33 * y13 * y13;
    Kh(1, 8) = (r12 * x32 + r13 * x13) * y12 + (r23 * x32 + r33 * x13) * y31; // Ovde je bila greska !!! 6 - 04 - 2002 [[ Google translate: There was a mistake! 6 - 04-2002 ]]

    Kh(2, 0) = Kh(0, 2);
    Kh(2, 1) = Kh(1, 2);
    Kh(2, 2) = r11 * pow(x21, 2) + 2.*r13 * x13 * x21 + r33 * pow(x13, 2);
    Kh(2, 3) = 2.*((r11 - r12) * x21 + (r13 - r23) * x13);
    Kh(2, 4) = (r12 * x21 + r23 * x13) * y23 + (r11 * x21 + r13 * x13) * y12;
    Kh(2, 5) = (r12 * x21 + r23 * x13) * x32 + r11 * pow(x21, 2) + r13 * x13 * x21;
    Kh(2, 6) = 2.*((r12 - r13) * x21 + (r23 - r33) * x13);
    Kh(2, 7) = (r12 * x21 + r23 * x13) * y23 + (r13 * x21 + r33 * x13) * y31;
    Kh(2, 8) = (r12 * x21 + r23 * x13) * x32 + (r13 * x21 + r33 * x13) * x13;

    Kh(3, 0) = Kh(0, 3);
    Kh(3, 1) = Kh(1, 3) ;
    Kh(3, 2) = Kh(2, 3) ;
    Kh(3, 3) = 4.0 * (r22 - 2 * r12 + r11);
    Kh(3, 4) = 2.*((r22 - r12) * y32 + (r12 - r11) * y21);
    Kh(3, 5) = 2.*((r22 - r12) * x23 + (r12 - r11) * x12);
    Kh(3, 6) = 4.0 * (r23 - r22 - r13 + r12);
    Kh(3, 7) = 2.*((r22 - r12) * y32 + (r23 - r13) * y13);
    Kh(3, 8) = 2.*((r22 - r12) * x23 + (r23 - r13) * x31);

    Kh(4, 0) = Kh(0, 4);
    Kh(4, 1) = Kh(1, 4);
    Kh(4, 2) = Kh(2, 4);
    Kh(4, 3) = Kh(3, 4);
    Kh(4, 4) = r22 * pow(y32, 2) + 2.*r12 * y21 * y32 + r11 * pow(y21, 2);
    Kh(4, 5) = (r22 * x32 + r12 * x21) * y23 + (r12 * x32 + r11 * x21) * y12;
    Kh(4, 6) = 2.*((r23 - r22) * y32 + (r13 - r12) * y21);
    Kh(4, 7) = r22 * pow(y32, 2) + (r12 * y21 + r23 * y13) * y32 + r13 * y13 * y21;
    Kh(4, 8) = (r22 * x32 + r23 * x13) * y23 + (r12 * x32 + r13 * x13) * y12;

    Kh(5, 0) = Kh(0, 5);
    Kh(5, 1) = Kh(1, 5);
    Kh(5, 2) = Kh(2, 5);
    Kh(5, 3) = Kh(3, 5);
    Kh(5, 4) = Kh(4, 5);
    Kh(5, 5) = r22 * pow(x32, 2) + 2.*r12 * x21 * x32 + r11 * pow(x21, 2);
    Kh(5, 6) = 2.*((r22 - r23) * x32 + (r12 - r13) * x21);
    Kh(5, 7) = (r22 * x32 + r12 * x21) * y23 + (r23 * x32 + r13 * x21) * y31;
    Kh(5, 8) = r22 * pow(x32, 2) + (r12 * x21 + r23 * x13) * x32 + r13 * x13 * x21;

    Kh(6, 0) = Kh(0, 6);
    Kh(6, 1) = Kh(1, 6);
    Kh(6, 2) = Kh(2, 6);
    Kh(6, 3) = Kh(3, 6);
    Kh(6, 4) = Kh(4, 6);
    Kh(6, 5) = Kh(5, 6);
    Kh(6, 6) = 4.0 * (r33 - 2 * r23 + r22);
    Kh(6, 7) = 2.*((r23 - r22) * y32 + (r33 - r23) * y13);
    Kh(6, 8) = 2.*((r23 - r22) * x23 + (r33 - r23) * x31);

    Kh(7, 0) = Kh(0, 7);
    Kh(7, 1) = Kh(1, 7);
    Kh(7, 2) = Kh(2, 7);
    Kh(7, 3) = Kh(3, 7);
    Kh(7, 4) = Kh(4, 7);
    Kh(7, 5) = Kh(5, 7);
    Kh(7, 6) = Kh(6, 7);
    Kh(7, 7) = r22 * pow(y32, 2) + 2.*r23 * y13 * y32 + r33 * pow(y13, 2);
    Kh(7, 8) = (r22 * x32 + r23 * x13) * y23 + (r23 * x32 + r33 * x13) * y31;

    Kh(8, 0) = Kh(0, 8);
    Kh(8, 1) = Kh(1, 8);
    Kh(8, 2) = Kh(2, 8);
    Kh(8, 3) = Kh(3, 8);
    Kh(8, 4) = Kh(4, 8);
    Kh(8, 5) = Kh(5, 8);
    Kh(8, 6) = Kh(6, 8);
    Kh(8, 7) = Kh(7, 8);
    Kh(8, 8) = r22 * pow(x32, 2) + 2.*r23 * x13 * x32 + r33 * pow(x13, 2);

    return Kh * Area;
}

double ThreeNodeAndesBending::getArea() const
{
    return Area;
}

void ThreeNodeAndesBending::initializeMq()
{
    // This matrix is the same for all instances of the element.
    // Appendix A of Ref. (vii) extended to the mass matrix.
    // Generated from mass_matrix.py
    Mq(0, 0) =  1960;
    Mq(0, 1) =  1540;
    Mq(0, 2) =  1540;
    Mq(0, 3) =  280;
    Mq(0, 4) =  224;
    Mq(0, 5) =  336;
    Mq(0, 6) =  112;
    Mq(0, 7) =  -56;
    Mq(0, 8) =  -56;
    Mq(1, 0) =  1540;
    Mq(1, 1) =  1960;
    Mq(1, 2) =  1540;
    Mq(1, 3) =  336;
    Mq(1, 4) =  280;
    Mq(1, 5) =  224;
    Mq(1, 6) =  -56;
    Mq(1, 7) =  112;
    Mq(1, 8) =  -56;
    Mq(2, 0) =  1540;
    Mq(2, 1) =  1540;
    Mq(2, 2) =  1960;
    Mq(2, 3) =  224;
    Mq(2, 4) =  336;
    Mq(2, 5) =  280;
    Mq(2, 6) =  -56;
    Mq(2, 7) =  -56;
    Mq(2, 8) =  112;
    Mq(3, 0) =  280;
    Mq(3, 1) =  336;
    Mq(3, 2) =  224;
    Mq(3, 3) =  112;
    Mq(3, 4) =  56;
    Mq(3, 5) =  56;
    Mq(3, 6) =  0;
    Mq(3, 7) =  32;
    Mq(3, 8) =  -32;
    Mq(4, 0) =  224;
    Mq(4, 1) =  280;
    Mq(4, 2) =  336;
    Mq(4, 3) =  56;
    Mq(4, 4) =  112;
    Mq(4, 5) =  56;
    Mq(4, 6) =  -32;
    Mq(4, 7) =  0;
    Mq(4, 8) =  32;
    Mq(5, 0) =  336;
    Mq(5, 1) =  224;
    Mq(5, 2) =  280;
    Mq(5, 3) =  56;
    Mq(5, 4) =  56;
    Mq(5, 5) =  112;
    Mq(5, 6) =  32;
    Mq(5, 7) =  -32;
    Mq(5, 8) =  0;
    Mq(6, 0) =  112;
    Mq(6, 1) =  -56;
    Mq(6, 2) =  -56;
    Mq(6, 3) =  0;
    Mq(6, 4) =  -32;
    Mq(6, 5) =  32;
    Mq(6, 6) =  60;
    Mq(6, 7) =  -27;
    Mq(6, 8) =  -27;
    Mq(7, 0) =  -56;
    Mq(7, 1) =  112;
    Mq(7, 2) =  -56;
    Mq(7, 3) =  32;
    Mq(7, 4) =  0;
    Mq(7, 5) =  -32;
    Mq(7, 6) =  -27;
    Mq(7, 7) =  60;
    Mq(7, 8) =  -27;
    Mq(8, 0) =  -56;
    Mq(8, 1) =  -56;
    Mq(8, 2) =  112;
    Mq(8, 3) =  -32;
    Mq(8, 4) =  32;
    Mq(8, 5) =  0;
    Mq(8, 6) =  -27;
    Mq(8, 7) =  -27;
    Mq(8, 8) =  60;

    Mq = Mq / 1680.;
}

bool ThreeNodeAndesBending::gotMass() const
{
    return rho > 0.0 ? true : false;
}

Vector ThreeNodeAndesBending::get_moment_field()
{
    Vector m(3);
    Vector disps(9);

    Vector d1 = theNodes[0]->getTrialDisp();
    Vector d2 = theNodes[1]->getTrialDisp();
    Vector d3 = theNodes[2]->getTrialDisp();

    disps(0) = d1(0);
    disps(1) = d1(1);
    disps(2) = d1(2);
    disps(3) = d2(0);
    disps(4) = d2(1);
    disps(5) = d2(2);
    disps(6) = d3(0);
    disps(7) = d3(1);
    disps(8) = d3(2);

    // Geometric data
    double x21 = -x12;
    double y21 = -y12;
    double x32 = -x23;
    double y32 = -y23;
    double x13 = -x31;
    double y13 = -y31;

    double l12 = sqrt(x12 * x12 + y12 * y12);
    double l23 = sqrt(x23 * x23 + y23 * y23);
    double l31 = sqrt(x31 * x31 + y31 * y31);

    double c12 = x21 / l12;
    double c23 = x32 / l23;
    double c31 = x13 / l31;
    double s12 = y21 / l12;
    double s23 = y32 / l23;
    double s31 = y13 / l31;


    Matrix L(3, 9);
    Matrix EL(3, 9);

    L(0, 0) = -c12 * s12 + c31 * s31;
    L(1, 0) = -c31 * s31 + c12 * s12;
    L(2, 0) = (s31 * s31 - c31 * c31) - (s12 * s12 - c12 * c12);
    L(0, 1) = (s12 * s12 * x12 + s31 * s31 * x31) / 2.0;
    L(1, 1) = (c12 * c12 * x12 + c31 * c31 * x31) / 2.0;
    L(2, 1) = c12 * c12 * y21 + c31 * c31 * y13;
    L(0, 2) = (s12 * s12 * y12 + s31 * s31 * y31) / 2.0;
    L(1, 2) = (c12 * c12 * y12 + c31 * c31 * y31) / 2.0;
    L(2, 2) = s12 * s12 * x21 + s31 * s31 * x13;
    L(0, 3) = -c23 * s23 + c12 * s12;
    L(1, 3) = -c12 * s12 + c23 * s23;
    L(2, 3) = (s12 * s12 - c12 * c12) - (s23 * s23 - c23 * c23);
    L(0, 4) = (s12 * s12 * x12 + s23 * s23 * x23) / 2.0;
    L(1, 4) = (c12 * c12 * x12 + c23 * c23 * x23) / 2.0;
    L(2, 4) = c12 * c12 * y21 + c23 * c23 * y32;
    L(0, 5) = (s12 * s12 * y12 + s23 * s23 * y23) / 2.0;
    L(1, 5) = (c12 * c12 * y12 + c23 * c23 * y23) / 2.0;
    L(2, 5) = s12 * s12 * x21 + s23 * s23 * x32;
    L(0, 6) = -c31 * s31 + c23 * s23;
    L(1, 6) = -c23 * s23 + c31 * s31;
    L(2, 6) = (s23 * s23 - c23 * c23) - (s31 * s31 - c31 * c31);
    L(0, 7) = (s23 * s23 * x23 + s31 * s31 * x31) / 2.0;
    L(1, 7) = (c23 * c23 * x23 + c31 * c31 * x31) / 2.0;
    L(2, 7) = c23 * c23 * y32 + c31 * c31 * y13;
    L(0, 8) = (s23 * s23 * y23 + s31 * s31 * y31) / 2.0;
    L(1, 8) = (c23 * c23 * y23 + c31 * c31 * y31) / 2.0;
    L(2, 8) = s23 * s23 * x32 + s31 * s31 * x13;

    EL.Zero();
    EL.addMatrixProduct(1.0, E_planestress * (thickness * thickness * thickness / 12 / Area), L, 1.0);

    m.Zero();
    m.addMatrixVector(1.0, EL , disps, 1.0);


    return m;
}


#endif










