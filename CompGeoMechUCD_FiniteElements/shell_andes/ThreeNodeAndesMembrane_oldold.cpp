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


//===================================================================================
// Element Constructors / Destructors
//===================================================================================

ThreeNodeAndesMembrane::ThreeNodeAndesMembrane()
    :
    Element( 0, ELE_TAG_ThreeNodeAndesMembrane ),
    connectedExternalNodes( 3 ),
    K( 18, 18 ), M( 18, 18 ),
    P( 18 ), Q( 18 ), bf( 3 ),
    thickness( 0.0 ),
    x0( 3 ), T_lg( 3, 3 ),
    rho( 0.0 ), E_planestress( 3, 3 ),
    is_stiffness_calculated( false ), is_mass_calculated( false )
{
    // zero node pointers
    for ( int i = 0; i < 3; i++ )
    {
        theNodes[i] = 0;
    }

    //initializeBetaArrays();
    Ar = 0.0;
    bHqv_Set = false;
}


ThreeNodeAndesMembrane::ThreeNodeAndesMembrane( int element_number,
        int node_numb_1, int node_numb_2, int node_numb_3,
        double t,
        NDMaterial* material, BodyForce* bodyforce )
    :
    Element( element_number, ELE_TAG_ThreeNodeAndesMembrane ),
    connectedExternalNodes( 3 ),
    K( 18, 18 ), M( 18, 18 ),
    P( 18 ), Q( 18 ), bf( 3 ),
    thickness( t ),
    x0( 3 ), T_lg( 3, 3 ),
    rho( 0.0 ), E_planestress( 3, 3 ),
    is_stiffness_calculated( false ), is_mass_calculated( false )
{
    this->setMaterialTag(material->getTag());
    init(node_numb_1, node_numb_2, node_numb_3,
         material, bodyforce);
}


ThreeNodeAndesMembrane::ThreeNodeAndesMembrane( int element_number,
        int node_numb_1, int node_numb_2, int node_numb_3,
        double t,
        NDMaterial** material, BodyForce* bodyforce )
    :
    Element( element_number, ELE_TAG_ThreeNodeAndesMembrane ),
    connectedExternalNodes( 3 ),
    K( 18, 18 ), M( 18, 18 ),
    P( 18 ), Q( 18 ), bf( 3 ),
    thickness( t ),
    x0( 3 ), T_lg( 3, 3 ),
    rho( 0.0 ), E_planestress( 3, 3 ),
    is_stiffness_calculated( false ), is_mass_calculated( false )
{
    this->setMaterialTag((*material)->getTag());
    init(node_numb_1, node_numb_2, node_numb_3,
         *material, bodyforce);
}

// This contruction helper method is provided so that code is not duplicated.
void ThreeNodeAndesMembrane::init(
    int node_numb_1, int node_numb_2, int node_numb_3,
    NDMaterial* material, BodyForce* bodyforce )
{
    
    this->setMaterialTag(material->getTag());

    // Set connected external node IDs
    connectedExternalNodes( 0 ) = node_numb_1;
    connectedExternalNodes( 1 ) = node_numb_2;
    connectedExternalNodes( 2 ) = node_numb_3;

    // zero node pointers
    for ( int i = 0; i < 3; i++ )
    {
        theNodes[i] = 0;
    }

    theMaterial = new NDMaterial*;

    if ( theMaterial == 0 )
    {
        cerr << " ThreeNodeAndesMembrane::ThreeNodeAndesMembrane -- failed to allocate material model pointer\n";
        exit( -1 );
    }

    *theMaterial = material->getCopy();

    if ( *theMaterial == 0 )
    {
        cerr << " ThreeNodeAndesMembrane::ThreeNodeAndesMembrane -- failed to allocate material model pointer\n";
        exit( -1 );
    }

    //
    rho = material->getRho();

    if ( rho < 0 )
    {
        cerr << " ThreeNodeAndesMembrane::ThreeNodeAndesMembrane -- rho = " << rho << "\n";
        exit( -1 );
    }

    bf = bodyforce->getBodyForceVector();

    //initializeBetaArrays();
    Ar = 0.0;
    bHqv_Set = false;
}



ThreeNodeAndesMembrane::~ThreeNodeAndesMembrane()
{

    if ( *theMaterial != NULL )
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

const ID& ThreeNodeAndesMembrane::getExternalNodes()
{
    return connectedExternalNodes;
}

Node** ThreeNodeAndesMembrane::getNodePtrs( void )
{
    return theNodes;
}

int ThreeNodeAndesMembrane::getNumDOF()
{
    return 18;
}

void ThreeNodeAndesMembrane::setDomain( Domain* theDomain )
{
    if ( theDomain == 0 )
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
        theNodes[2] = 0;
    }
    else
    {
        int Nd1 = connectedExternalNodes( 0 );
        int Nd2 = connectedExternalNodes( 1 );
        int Nd3 = connectedExternalNodes( 2 );

        theNodes[0] = theDomain->getNode( Nd1 );
        theNodes[1] = theDomain->getNode( Nd2 );
        theNodes[2] = theDomain->getNode( Nd3 );

        if ( theNodes[0] == 0 || theNodes[1] == 0 || theNodes[2] == 0 )
        {
            cerr << "FATAL ERROR ThreeNodeAndesMembrane (tag: " << this->getTag() << "), node not found in domain\n";
            exit( -1 );
        }

        int dofNd1 = theNodes[0]->getNumberDOF();
        int dofNd2 = theNodes[1]->getNumberDOF();
        int dofNd3 = theNodes[2]->getNumberDOF();

        if ( dofNd1 != 6 || dofNd2 != 6 || dofNd3 != 6 )
        {
            cerr << "FATAL ERROR ThreeNodeAndesMembrane (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit( -1 );
        }

        this->DomainComponent::setDomain( theDomain );

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

int ThreeNodeAndesMembrane::update( void )
{
    return 0;
}

const Matrix& ThreeNodeAndesMembrane::getTangentStiff ()
{
    // Since this is a linear element, the stiffness is calculated only once.
    if ( ~is_stiffness_calculated )
    {
        int I, J;

        // Form completestiffness
        Matrix Kb( 9, 9 );     // Basic membrane stiffness
        Matrix Kh( 9, 9 );     // High-order membrane stiffness
        //Matrix Kb_bending(9,9);         // Basic bending stiffness
        //Matrix Kh_bending(9,9);         // High-order bending stiffness
        Matrix Kloc( 18, 18 );        // Total (membrane + bending) stiffness in local coordinates

        double beta = 0.5;

        //Get matrices
        Kb = getMembraneBasicStiffness();
        Kh = getMembraneHighOrderStiffness();
        //Kb_bending  = getBendingBasicStiffness();
        //Kh_bending  = getBendingHighOrderStiffness();

        // Expand matrices to local complete stiffness
        //   DOFS   --->        ux1, uy1, uz1, rx1, ry1, rz1  |  ux2, uy2, uz2, rx2, ry2, rz2  |  ux3, uy3, uz3, rx3, ry3, rz3
        int membrane_dofs[9] = {  0,   1,                  5,      6,   7,                 11,     12,  13,                 17 };
        //int  bending_dofs[9] = {            2,   3,   4,                     8,   9,  10,                    14,  15,  16      };

        for ( int i = 0; i < 9; i++ )
        {
            for ( int j = 0; j < 9; j++ )
            {
                // Expand membrane stiffness to local membrane (& drilling) DOFS.
                I = membrane_dofs[i];
                J = membrane_dofs[j];
                Kloc( I, J ) = (Kb( i, j ) + beta * Kh( i, j )) * thickness;

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
        Matrix TLG(18, 18);
        TLG.Zero();
        Matrix T_gl( 3, 3 );
        T_lg.Invert( T_gl );
        int offset = 0;

        for ( int n = 0; n < 6; n++ )
        {
            I = 0;

            for ( int i = 0; i < 3; i++ )
            {
                J = 0;

                for ( int j = 0; j < 3; j++ )
                {
                    TLG( I + offset, J + offset ) = T_gl( i, j );
                    J++;
                }

                I++;
            }

            offset += 3;
        }

        // Compute total stiffness as K = TLG^T * Klocal *TLG
        K.Zero();
        K.addMatrixTripleProduct( 1.0, TLG, Kloc, 1.0 );

        is_stiffness_calculated = true;
        //
        //        for (int i = 0; i < 3; i++)
        //        {
        //            cout << "(" << x[i] << ", " << y[i] << ", " << z[i] << ")\n";
        //        }
        //
        //        cerr << " Kb = " << Kb;
        //        cerr << " Kh = " << Kh;
        //        cerr << " K = " << K;
    }

    return K;
}

const Matrix& ThreeNodeAndesMembrane::getInitialStiff()
{
    return getTangentStiff();
}

const Matrix& ThreeNodeAndesMembrane::getMass ()
{
    if ( !is_mass_calculated )
    {
        double x13 = -x31;
        double y13 = -y31;
        double alpha = 1.5;

        Matrix TLG( 18, 18 );           // Local-to-global transformation matrix

        Matrix Ml( 9, 9 );
        Matrix Mlocal( 18, 18 );

        Ml( 0, 0 ) = 30;
        Ml( 0, 1 ) = 0;
        Ml( 0, 2 ) = 3 * alpha * y12 + 3 * alpha * y13;
        Ml( 0, 3 ) = 15;
        Ml( 0, 4 ) = 0;
        Ml( 0, 5 ) = -3 * alpha * y12 + 3 * alpha * y23 / 2;
        Ml( 0, 6 ) = 15;
        Ml( 0, 7 ) = 0;
        Ml( 0, 8 ) = -3 * alpha * y13 - 3 * alpha * y23 / 2;
        Ml( 1, 0 ) = 0;
        Ml( 1, 1 ) = 30;
        Ml( 1, 2 ) = -3 * alpha * x12 - 3 * alpha * x13;
        Ml( 1, 3 ) = 0;
        Ml( 1, 4 ) = 15;
        Ml( 1, 5 ) = 3 * alpha * x12 - 3 * alpha * x23 / 2;
        Ml( 1, 6 ) = 0;
        Ml( 1, 7 ) = 15;
        Ml( 1, 8 ) = 3 * alpha * x13 + 3 * alpha * x23 / 2;
        Ml( 2, 0 ) = 3 * alpha * y12 + 3 * alpha * y13;
        Ml( 2, 1 ) = -3 * alpha * x12 - 3 * alpha * x13;
        Ml( 2, 2 ) = -45 * alpha * x12 * ( -alpha * x12 / 45 - alpha * x13 / 90 ) / 2 - 45 * alpha * x13 * ( -alpha * x12 / 90 - alpha * x13 / 45 ) / 2 + 45 * alpha * y12 * ( alpha * y12 / 45 + alpha * y13 / 90 ) / 2 + 45 * alpha * y13 * ( alpha * y12 / 90 + alpha * y13 / 45 ) / 2;
        Ml( 2, 3 ) = 3 * alpha * y12 + 3 * alpha * y13 / 2;
        Ml( 2, 4 ) = -3 * alpha * x12 - 3 * alpha * x13 / 2;
        Ml( 2, 5 ) = 45 * alpha * x12 * ( -alpha * x12 / 45 - alpha * x13 / 90 ) / 2 - 45 * alpha * x23 * ( -alpha * x12 / 90 - alpha * x13 / 90 ) / 2 - 45 * alpha * y12 * ( alpha * y12 / 45 + alpha * y13 / 90 ) / 2 + 45 * alpha * y23 * ( alpha * y12 / 90 + alpha * y13 / 90 ) / 2;
        Ml( 2, 6 ) = 3 * alpha * y12 / 2 + 3 * alpha * y13;
        Ml( 2, 7 ) = -3 * alpha * x12 / 2 - 3 * alpha * x13;
        Ml( 2, 8 ) = 45 * alpha * x13 * ( -alpha * x12 / 90 - alpha * x13 / 45 ) / 2 + 45 * alpha * x23 * ( -alpha * x12 / 90 - alpha * x13 / 90 ) / 2 - 45 * alpha * y13 * ( alpha * y12 / 90 + alpha * y13 / 45 ) / 2 - 45 * alpha * y23 * ( alpha * y12 / 90 + alpha * y13 / 90 ) / 2;
        Ml( 3, 0 ) = 15;
        Ml( 3, 1 ) = 0;
        Ml( 3, 2 ) = 3 * alpha * y12 + 3 * alpha * y13 / 2;
        Ml( 3, 3 ) = 30;
        Ml( 3, 4 ) = 0;
        Ml( 3, 5 ) = -3 * alpha * y12 + 3 * alpha * y23;
        Ml( 3, 6 ) = 15;
        Ml( 3, 7 ) = 0;
        Ml( 3, 8 ) = -3 * alpha * y13 / 2 - 3 * alpha * y23;
        Ml( 4, 0 ) = 0;
        Ml( 4, 1 ) = 15;
        Ml( 4, 2 ) = -3 * alpha * x12 - 3 * alpha * x13 / 2;
        Ml( 4, 3 ) = 0;
        Ml( 4, 4 ) = 30;
        Ml( 4, 5 ) = 3 * alpha * x12 - 3 * alpha * x23;
        Ml( 4, 6 ) = 0;
        Ml( 4, 7 ) = 15;
        Ml( 4, 8 ) = 3 * alpha * x13 / 2 + 3 * alpha * x23;
        Ml( 5, 0 ) = -3 * alpha * y12 + 3 * alpha * y23 / 2;
        Ml( 5, 1 ) = 3 * alpha * x12 - 3 * alpha * x23 / 2;
        Ml( 5, 2 ) = -45 * alpha * x12 * ( alpha * x12 / 45 - alpha * x23 / 90 ) / 2 - 45 * alpha * x13 * ( alpha * x12 / 90 - alpha * x23 / 90 ) / 2 + 45 * alpha * y12 * ( -alpha * y12 / 45 + alpha * y23 / 90 ) / 2 + 45 * alpha * y13 * ( -alpha * y12 / 90 + alpha * y23 / 90 ) / 2;
        Ml( 5, 3 ) = -3 * alpha * y12 + 3 * alpha * y23;
        Ml( 5, 4 ) = 3 * alpha * x12 - 3 * alpha * x23;
        Ml( 5, 5 ) = 45 * alpha * x12 * ( alpha * x12 / 45 - alpha * x23 / 90 ) / 2 - 45 * alpha * x23 * ( alpha * x12 / 90 - alpha * x23 / 45 ) / 2 - 45 * alpha * y12 * ( -alpha * y12 / 45 + alpha * y23 / 90 ) / 2 + 45 * alpha * y23 * ( -alpha * y12 / 90 + alpha * y23 / 45 ) / 2;
        Ml( 5, 6 ) = -3 * alpha * y12 / 2 + 3 * alpha * y23;
        Ml( 5, 7 ) = 3 * alpha * x12 / 2 - 3 * alpha * x23;
        Ml( 5, 8 ) = 45 * alpha * x13 * ( alpha * x12 / 90 - alpha * x23 / 90 ) / 2 + 45 * alpha * x23 * ( alpha * x12 / 90 - alpha * x23 / 45 ) / 2 - 45 * alpha * y13 * ( -alpha * y12 / 90 + alpha * y23 / 90 ) / 2 - 45 * alpha * y23 * ( -alpha * y12 / 90 + alpha * y23 / 45 ) / 2;
        Ml( 6, 0 ) = 15;
        Ml( 6, 1 ) = 0;
        Ml( 6, 2 ) = 3 * alpha * y12 / 2 + 3 * alpha * y13;
        Ml( 6, 3 ) = 15;
        Ml( 6, 4 ) = 0;
        Ml( 6, 5 ) = -3 * alpha * y12 / 2 + 3 * alpha * y23;
        Ml( 6, 6 ) = 30;
        Ml( 6, 7 ) = 0;
        Ml( 6, 8 ) = -3 * alpha * y13 - 3 * alpha * y23;
        Ml( 7, 0 ) = 0;
        Ml( 7, 1 ) = 15;
        Ml( 7, 2 ) = -3 * alpha * x12 / 2 - 3 * alpha * x13;
        Ml( 7, 3 ) = 0;
        Ml( 7, 4 ) = 15;
        Ml( 7, 5 ) = 3 * alpha * x12 / 2 - 3 * alpha * x23;
        Ml( 7, 6 ) = 0;
        Ml( 7, 7 ) = 30;
        Ml( 7, 8 ) = 3 * alpha * x13 + 3 * alpha * x23;
        Ml( 8, 0 ) = -3 * alpha * y13 - 3 * alpha * y23 / 2;
        Ml( 8, 1 ) = 3 * alpha * x13 + 3 * alpha * x23 / 2;
        Ml( 8, 2 ) = -45 * alpha * x12 * ( alpha * x13 / 90 + alpha * x23 / 90 ) / 2 - 45 * alpha * x13 * ( alpha * x13 / 45 + alpha * x23 / 90 ) / 2 + 45 * alpha * y12 * ( -alpha * y13 / 90 - alpha * y23 / 90 ) / 2 + 45 * alpha * y13 * ( -alpha * y13 / 45 - alpha * y23 / 90 ) / 2;
        Ml( 8, 3 ) = -3 * alpha * y13 / 2 - 3 * alpha * y23;
        Ml( 8, 4 ) = 3 * alpha * x13 / 2 + 3 * alpha * x23;
        Ml( 8, 5 ) = 45 * alpha * x12 * ( alpha * x13 / 90 + alpha * x23 / 90 ) / 2 - 45 * alpha * x23 * ( alpha * x13 / 90 + alpha * x23 / 45 ) / 2 - 45 * alpha * y12 * ( -alpha * y13 / 90 - alpha * y23 / 90 ) / 2 + 45 * alpha * y23 * ( -alpha * y13 / 90 - alpha * y23 / 45 ) / 2;
        Ml( 8, 6 ) = -3 * alpha * y13 - 3 * alpha * y23;
        Ml( 8, 7 ) = 3 * alpha * x13 + 3 * alpha * x23;
        Ml( 8, 8 ) = 45 * alpha * x13 * ( alpha * x13 / 45 + alpha * x23 / 90 ) / 2 + 45 * alpha * x23 * ( alpha * x13 / 90 + alpha * x23 / 45 ) / 2 - 45 * alpha * y13 * ( -alpha * y13 / 45 - alpha * y23 / 90 ) / 2 - 45 * alpha * y23 * ( -alpha * y13 / 90 - alpha * y23 / 45 ) / 2;


        // Expand matrices to local complete stiffness
        //   DOFS   --->        ux1,  uy1, uz1, rx1, ry1, rz1  |  ux2, uy2, uz2, rx2, ry2, rz2  |  ux3, uy3, uz3, rx3, ry3, rz3
        int membrane_dofs[9] = {  0,   1,                  5,      6,   7,                 11,     12,  13,                 17 };
        //int  bending_dofs[9] = {            2,   3,   4,                     8,   9,  10,                    14,  15,  16      };

        double weight_over_180 = thickness * Ar * rho / 180.0;

        int I = 0;
        int J = 0;

        for ( int i = 0; i < 9; i++ )
        {
            for ( int j = 0; j < 9; j++ )
            {
                // Expand membrane mass to local membrane (& drilling) DOFS.
                I = membrane_dofs[i];
                J = membrane_dofs[j];
                Mlocal( I, J ) = weight_over_180 * Ml( i, j );
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
        Matrix T_gl( 3, 3 );
        T_lg.Invert( T_gl );
        int offset = 0;

        for ( int n = 0; n < 6; n++ )
        {
            I = 0;

            for ( int i = 0; i < 3; i++ )
            {
                J = 0;

                for ( int j = 0; j < 3; j++ )
                {
                    TLG( I + offset, J + offset ) = T_gl( i, j );
                    J++;
                }

                I++;
            }

            offset += 3;
        }

        // Compute total mass
        M.Zero();
        M.addMatrixTripleProduct( 1.0, TLG, Mlocal, 1.0 );

        is_mass_calculated = true;
    }

    return M;
}

void ThreeNodeAndesMembrane::zeroLoad ()
{
    Q.Zero();
}

int ThreeNodeAndesMembrane::addLoad( ElementalLoad* theLoad, double loadFactor )
{
    return 0;
}

int ThreeNodeAndesMembrane::addInertiaLoadToUnbalance( const Vector& accel )
{
    return 0;
}

const Vector& ThreeNodeAndesMembrane::getResistingForce ()
{
    Vector NodalDisplacements( 18 );
    Vector disp_i( 6 );

    for ( int node = 0; node < 3; node++ )
    {
        Node* node_i = theNodes[node];
        disp_i = node_i->getDisp();
        //disp_i += node_i->getIncrDeltaDisp();
        disp_i += node_i->getIncrDisp();

        NodalDisplacements( 6 * node + 0 ) = disp_i( 0 );
        NodalDisplacements( 6 * node + 1 ) = disp_i( 1 );
        NodalDisplacements( 6 * node + 2 ) = disp_i( 2 );
        NodalDisplacements( 6 * node + 3 ) = disp_i( 3 );
        NodalDisplacements( 6 * node + 4 ) = disp_i( 4 );
        NodalDisplacements( 6 * node + 5 ) = disp_i( 5 );
    }

    P = K * NodalDisplacements;

    return P;
}

const Vector& ThreeNodeAndesMembrane::getResistingForceIncInertia ()
{
    Vector NodalDisplacements( 18 );
    Vector disp_i( 6 );
    Vector NodalAccelerations( 18 );
    Vector accel_i( 6 );

    for ( int node = 0; node < 3; node++ )
    {
        Node* node_i = theNodes[node];
        disp_i = node_i->getDisp();
        //disp_i += node_i->getIncrDeltaDisp();
        disp_i += node_i->getIncrDisp();

        accel_i = node_i->getAccel();

        NodalDisplacements( 6 * node + 0 ) = disp_i( 0 );
        NodalDisplacements( 6 * node + 1 ) = disp_i( 1 );
        NodalDisplacements( 6 * node + 2 ) = disp_i( 2 );
        NodalDisplacements( 6 * node + 3 ) = disp_i( 3 );
        NodalDisplacements( 6 * node + 4 ) = disp_i( 4 );
        NodalDisplacements( 6 * node + 5 ) = disp_i( 5 );

        NodalAccelerations( 6 * node + 0 ) = accel_i( 0 );
        NodalAccelerations( 6 * node + 1 ) = accel_i( 1 );
        NodalAccelerations( 6 * node + 2 ) = accel_i( 2 );
        NodalAccelerations( 6 * node + 3 ) = accel_i( 3 );
        NodalAccelerations( 6 * node + 4 ) = accel_i( 4 );
        NodalAccelerations( 6 * node + 5 ) = accel_i( 5 );
    }

    P = K * NodalDisplacements + M * NodalAccelerations;

    return P;
}

int ThreeNodeAndesMembrane::sendSelf ( int commitTag, Channel& theChannel )
{
    return 0;
}

int ThreeNodeAndesMembrane::receiveSelf ( int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker )
{
    return 0;
}

void ThreeNodeAndesMembrane::Print( ostream& s, int flag = 0 )
{

}

Response* ThreeNodeAndesMembrane::setResponse ( const char** argv, int argc, Information& eleInformation )
{
    if ( strcmp( argv[0], "force" ) == 0 || strcmp( argv[0], "forces" ) == 0 )
    {
        return new ElementResponse( this, 1, P );
    }
    else if ( strcmp( argv[0], "stiff" ) == 0 || strcmp( argv[0], "stiffness" ) == 0 )
    {
        return new ElementResponse( this, 5, K );
    }
    else
    {
        return 0;  // Return a null pointer
    }
}

int ThreeNodeAndesMembrane::getResponse ( int responseID, Information& eleInformation )
{
    if ( responseID == 1 ) //forces
    {
        return eleInformation.setVector( P );
    }

    if ( responseID == 5 ) //stiffness
    {
        return eleInformation.setMatrix( K );
    }
    else
    {
        return -1;
    }
}

Matrix ThreeNodeAndesMembrane::returnMass( void )
{
    if ( ~is_mass_calculated )
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
    Vector e1( 3 ), e2( 3 ), e3( 3 );

    //node coordinates
    Vector x1( 3 ), x2( 3 ), x3( 3 );
    x1 = theNodes[0]->getCrds();
    x2 = theNodes[1]->getCrds();
    x3 = theNodes[2]->getCrds();

    //Centroid
    x0 = ( x1 + x2 + x3 ) / 3;

    Vector dpt1( 3 ), dpt2( 3 ), dpt3( 3 ), r1( 3 ), r2( 3 );
    dpt1 = x1 - x0;
    dpt2 = x2 - x0;
    dpt3 = x3 - x0;

    r1 = x2 - x1;
    r2 = x3 - x1;

    // Local x axis points paralell to side 1-2
    e1 = x2 - x1;
    e1.Normalize();

    // Local z axis is normal to the triangle
    e3 = calculate_cross_product( r1, r2 );
    Ar = 0.5 * e3.Norm();
    e3.Normalize();

    if ( Ar < 0 )
    {
        cout << "ThreeNodeAndesMembrane::initializeGeometry() -> Element # " << getTag() << " has A < 0!! " << endl;
    }

    //Local y axis is orthogonal to e1 and e3.
    e2 = calculate_cross_product( e3, e1 );


    useThisCoordinateSystem(e1, e2, e3);
    ab = 1.5;
    ah = 1.25;
}

void ThreeNodeAndesMembrane::useThisCoordinateSystem( Vector e1, Vector e2, Vector e3 )
{
    //Local-to-global transformation matrix
    for ( int i = 0; i < 3; i++ )
    {
        T_lg( i, 0 ) = e1( i );
        T_lg( i, 1 ) = e2( i );
        T_lg( i, 2 ) = e3( i );
    }

    //Matrix T_gl(3,3);
    //T_lg.Invert(T_gl);  //Compute global -> local transformation matrix

    Vector xl1(3), xl2(3), xl3(3);

    // Project global nodal coordinates into local coordinates
    T_lg.Solve( theNodes[0]->getCrds() - x0, xl1 ); // Solve T_lg * xl1 = x1
    T_lg.Solve( theNodes[1]->getCrds() - x0, xl2 ); // Solve T_lg * xl2 = x2
    T_lg.Solve( theNodes[2]->getCrds() - x0, xl3 ); // Solve T_lg * xl3 = x3

    x[0] = xl1[0];
    y[0] = xl1[1];
    z[0] = xl1[2];

    x[1] = xl2[0];
    y[1] = xl2[1];
    z[1] = xl2[2];

    x[2] = xl3[0];
    y[2] = xl3[1];
    z[2] = xl3[2];
}

void ThreeNodeAndesMembrane::calculate_E_planestress()
{
    // This condenses the E matrix which holds the elastic coefficients
    // into the plane-stress E matrix.
    Matrix E( 6, 6 );
    E = ( *theMaterial )->getTangent();

    //Static condensation
    Matrix A( 3, 3 );
    Matrix B( 3, 3 );
    Matrix C( 3, 3 );

    int keep_these_dofs[3] = {0, 1, 3};
    int condensed_these_dofs[3] = {2, 4, 5};

    for ( int i = 0; i < 3; i++ )
    {
        for ( int j = 0; j < 3; j++ )
        {
            A( i, j ) = E( keep_these_dofs[i], keep_these_dofs[j] );
            B( i, j ) = E( keep_these_dofs[i], condensed_these_dofs[j] );
            C( i, j ) = E( condensed_these_dofs[i], condensed_these_dofs[j] );
        }
    }

    Matrix C_inv( 3, 3 );
    C.Invert( C_inv ); // Invert C and place result in C_inv

    E_planestress.Zero();

    // Compute condensed matrix
    // E_planestress = A - B*(C^-1)*B^T
    E_planestress = A;

    for ( int i = 0; i < 3; i++ )
    {
        for ( int j = 0; j < 3; j++ )
        {
            for ( int m = 0; m < 3; m ++ )
            {
                for ( int n = 0; n < 3; n ++ )
                {
                    E_planestress( i, j ) -= B( i, m ) * C_inv( m, n ) * B( j, n );
                }
            }
        }
    }

}


//===================================================================================
// Membrane
//===================================================================================


Matrix ThreeNodeAndesMembrane::getMembraneBasicStiffness()
{
    //Initialize basic stiffness
    Matrix Kb( 9, 9 );
    // Force lumping matrix
    Matrix L( 9, 3 );

    ab = 1.25; // Da bi bio skaliran kao i u Kh


    //Condense E into plane stress matrix
    calculate_E_planestress();


    double x12 = x[0] - x[1];
    double x21 = -1.0 * x12;
    double x23 = x[1] - x[2];
    double x32 = -1.0 * x23;
    double x31 = x[2] - x[0];
    double x13 = -1.0 * x31;
    double y12 = y[0] - y[1];
    double y21 = -1.0 * y12;
    double y23 = y[1] - y[2];
    double y32 = -1.0 * y23;
    double y31 = y[2] - y[0];
    double y13 = -1.0 * y31;


    L( 0, 0 ) = y23;
    L( 0, 2 ) = x32;
    L( 1, 1 ) = x32;
    L( 1, 2 ) = y23;
    L( 2, 0 ) = ab * y23 * ( y13 - y21 ) / 6.0;
    L( 2, 1 ) = ab * x32 * ( x31 - x12 ) / 6.0;
    L( 2, 2 ) = ab * ( x31 * y13 - x12 * y21 ) / 3.0;

    L( 3, 0 ) = y31;
    L( 3, 2 ) = x13;
    L( 4, 1 ) = x13;
    L( 4, 2 ) = y31;
    L( 5, 0 ) = ab * y31 * ( y21 - y32 ) / 6.0;
    L( 5, 1 ) = ab * x13 * ( x12 - x23 ) / 6.0;
    L( 5, 2 ) = ab * ( x12 * y21 - x23 * y32 ) / 3.0;

    L( 6, 0 ) = y12;
    L( 6, 2 ) = x21;
    L( 7, 1 ) = x21;
    L( 7, 2 ) = y12;
    L( 8, 0 ) = ab * y12 * ( y32 - y13 ) / 6.0;
    L( 8, 1 ) = ab * x21 * ( x23 - x31 ) / 6.0;
    L( 8, 2 ) = ab * ( x23 * y32 - x31 * y13 ) / 3.0;

    Matrix Lt(3, 9);

    for (int i = 0 ; i < 9; i++ )
    {
        for (int j = 0 ; j < 3; j++ )
        {
            Lt(j, i) = L(i, j);
        }
    }

    Kb.Zero();
    Kb.addMatrixTripleProduct( 1.0, Lt, E_planestress, 1.0 );

    return Kb / ( 4.0 * Ar );
}

Matrix ThreeNodeAndesMembrane::getMembraneHighOrderStiffness()
{
    // vraca lokalnu matricu samo sa pomeranjima viseg reda>
    // Ova matrica je ujendni i Matrica krutosti ( nije potrebno dodavati baznu )
    // ----------------- Zatim matricu viseg reda :
    double x12 = x[0] - x[1];
    double x21 = -1.0 * x12;
    double x23 = x[1] - x[2];
    double x32 = -1.0 * x23;
    double x31 = x[2] - x[0];
    double x13 = -1.0 * x31;
    double y12 = y[0] - y[1];
    double y21 = -1.0 * y12;
    double y23 = y[1] - y[2];
    double y32 = -1.0 * y23;
    double y31 = y[2] - y[0];
    double y13 = -1.0 * y31;
    // C(1,3)=C(1,3)/2 ; C(2,3)=C(2,3)/2 ; C(3,3) = C(3,3)/2;
    // C(3,1)=2*C(3,1) ; C(3,2)=2*C(3,2) ; C(3,3) = 2*C(3,3);
    double x10 = x[0];
    double x20 = x[1] ;
    double x30 = x[2] ;
    double y10 = y[0];
    double y20 = y[1] ;
    double y30 = y[2] ;
    double k = 1.0 / ( 12.0 * Ar ); //Po meni bi trebalo:
    // U clanku pise:
    // k=3./(2.*Ar*Ar); // Ali je kruto ko djavo pa sam ja video da je trebalo: k=1/(12*Ar);
    k = 3. / ( 4.*Ar ); // Ali je kruto ko djavo pa sam ja video da je trebalo: k=1/(12*Ar);

    Matrix Kqh( 6, 6 );
    Kqh.Zero();

    Matrix C = E_planestress;

    Kqh( 0, 0 ) = 2 * k * ( C( 0, 0 ) * y30 * y30 - 2 * C( 0, 2 ) * x30 * y30 + C( 2, 2 ) * x30 * x30 );
    //double aTmp = Kqh( 0, 0 );
    Kqh( 0, 1 ) = k * ( ( C( 0, 2 ) * x10 - C( 0, 0 ) * y10 ) * y30 + ( C( 0, 2 ) * y10 - C( 2, 2 ) * x10 ) * x30 );
    Kqh( 0, 2 ) = k * ( ( C( 0, 2 ) * x20 - C( 0, 0 ) * y20 ) * y30 + ( C( 0, 2 ) * y20 - C( 2, 2 ) * x20 ) * x30 );
    Kqh( 0, 3 ) = 2 * k * ( C( 0, 2 ) * y30 * y30 - ( C( 2, 2 ) + C( 0, 1 ) ) * x30 * y30 + C( 1, 2 ) * x30 * x30 );
    Kqh( 0, 4 ) = k * ( ( C( 0, 1 ) * x10 - C( 0, 2 ) * y10 ) * y30 + ( C( 2, 2 ) * y10 - C( 1, 2 ) * x10 ) * x30 );
    Kqh( 0, 5 ) = k * ( ( C( 0, 1 ) * x20 - C( 0, 2 ) * y20 ) * y30 + ( C( 2, 2 ) * y20 - C( 1, 2 ) * x20 ) * x30 );

    Kqh( 1, 1 ) = 2 * k * ( C( 0, 0 ) * y10 * y10 - 2 * C( 0, 2 ) * x10 * y10 + C( 2, 2 ) * x10 * x10 );
    Kqh( 1, 2 ) = k * ( ( C( 0, 2 ) * x10 - C( 0, 0 ) * y10 ) * y20 + ( C( 0, 2 ) * y10 - C( 2, 2 ) * x10 ) * x20 );
    Kqh( 1, 3 ) = k * ( ( C( 2, 2 ) * x10 - C( 0, 2 ) * y10 ) * y30 + ( C( 0, 1 ) * y10 - C( 1, 2 ) * x10 ) * x30 );
    Kqh( 1, 4 ) = 2 * k * ( C( 0, 2 ) * y10 * y10 - ( C( 2, 2 ) + C( 0, 1 ) ) * x10 * y10 + C( 1, 2 ) * x10 * x10 );
    Kqh( 1, 5 ) = k * ( ( C( 2, 2 ) * x10 - C( 0, 2 ) * y10 ) * y20 + ( C( 0, 1 ) * y10 - C( 1, 2 ) * x10 ) * x20 );

    Kqh( 2, 2 ) = 2 * k * ( C( 0, 0 ) * y20 * y20 - 2 * C( 0, 2 ) * x20 * y20 + C( 2, 2 ) * x20 * x20 );
    Kqh( 2, 3 ) = k * ( ( C( 2, 2 ) * x20 - C( 0, 2 ) * y20 ) * y30 + ( C( 0, 1 ) * y20 - C( 1, 2 ) * x20 ) * x30 );
    Kqh( 2, 4 ) = k * ( ( C( 0, 1 ) * x10 - C( 0, 2 ) * y10 ) * y20 + ( C( 2, 2 ) * y10 - C( 1, 2 ) * x10 ) * x20 );
    Kqh( 2, 5 ) = 2 * k * ( C( 0, 2 ) * y20 * y20 - ( C( 2, 2 ) + C( 0, 1 ) ) * x20 * y20 + C( 1, 2 ) * x20 * x20 );

    Kqh( 3, 3 ) = 2 * k * ( C( 2, 2 ) * y30 * y30 - 2 * C( 1, 2 ) * x30 * y30 + C( 1, 1 ) * x30 * x30 );
    Kqh( 3, 4 ) = k * ( ( C( 1, 2 ) * x10 - C( 2, 2 ) * y10 ) * y30 + ( C( 1, 2 ) * y10 - C( 1, 1 ) * x10 ) * x30 );
    Kqh( 3, 5 ) = k * ( ( C( 1, 2 ) * x20 - C( 2, 2 ) * y20 ) * y30 + ( C( 1, 2 ) * y20 - C( 1, 1 ) * x20 ) * x30 );

    Kqh( 4, 4 ) = 2 * k * ( C( 2, 2 ) * y10 * y10 - 2 * C( 1, 2 ) * x10 * y10 + C( 1, 1 ) * x10 * x10 );
    Kqh( 4, 5 ) = k * ( ( C( 1, 2 ) * x10 - C( 2, 2 ) * y10 ) * y20 + ( C( 1, 2 ) * y10 - C( 1, 1 ) * x10 ) * x20 );

    Kqh( 5, 5 ) = 2 * k * ( C( 2, 2 ) * y20 * y20 - 2 * C( 1, 2 ) * x20 * y20 + C( 1, 1 ) * x20 * x20 );


    //-------------------------------- Simetricni deo;
    Kqh( 1, 0 ) = Kqh( 0, 1 );
    Kqh( 2, 0 ) = Kqh( 0, 2 );
    Kqh( 3, 0 ) = Kqh( 0, 3 );
    Kqh( 4, 0 ) = Kqh( 0, 4 );
    Kqh( 5, 0 ) = Kqh( 0, 5 );
    Kqh( 2, 1 ) = Kqh( 1, 2 );
    Kqh( 3, 1 ) = Kqh( 1, 3 );
    Kqh( 4, 1 ) = Kqh( 1, 4 );
    Kqh( 5, 1 ) = Kqh( 1, 5 );
    Kqh( 3, 2 ) = Kqh( 2, 3 );
    Kqh( 4, 2 ) = Kqh( 2, 4 );
    Kqh( 5, 2 ) = Kqh( 2, 5 );
    Kqh( 4, 3 ) = Kqh( 3, 4 );
    Kqh( 5, 3 ) = Kqh( 3, 5 );
    Kqh( 5, 4 ) = Kqh( 4, 5 );
    //--------------------------------------------------------------------------------------------
    //   Za matrice H moram naci i koeficijent Si i a_{ij}

    // cout<< "Kqh=:" << Kqh.OutString();
    double l12 = sqrt( x12 * x12 + y12 * y12 );
    double l23 = sqrt( x23 * x23 + y23 * y23 );
    double l31 = sqrt( x31 * x31 + y31 * y31 );

    double S1 = ( l12 * l12 - l31 * l31 ) / 4.0;
    double S2 = ( l23 * l23 - l12 * l12 ) / 4.0;
    double S3 = ( l31 * l31 - l23 * l23 ) / 4.0;

    double a12 = 1.5 * sqrt( x30 * x30 + y30 * y30 );
    double a23 = 1.5 * sqrt( x10 * x10 + y10 * y10 );
    double a31 = 1.5 * sqrt( x20 * x20 + y20 * y20 );

    Matrix Hmt( 6, 3 ); //
    Hmt.Zero();

    Hmt( 0, 0 ) = ( ( 0.6 * a12 * a12 - S3 ) * y30 + Ar * x30 ) / ( 0.4 * a12 * a12 );
    Hmt( 0, 1 ) = ( ( 0.6 * a12 * a12 + S3 ) * y30 - Ar * x30 ) / ( 0.4 * a12 * a12 );
    Hmt( 0, 2 ) = y30;

    Hmt( 1, 0 ) = y10;
    Hmt( 1, 1 ) = ( ( 0.6 * a23 * a23 - S1 ) * y10 + Ar * x10 ) / ( 0.4 * a23 * a23 );
    Hmt( 1, 2 ) = ( ( 0.6 * a23 * a23 + S1 ) * y10 - Ar * x10 ) / ( 0.4 * a23 * a23 );

    Hmt( 2, 0 ) = ( ( 0.6 * a31 * a31 + S2 ) * y20 - Ar * x20 ) / ( 0.4 * a31 * a31 );
    Hmt( 2, 1 ) = y20;
    Hmt( 2, 2 ) = ( ( 0.6 * a31 * a31 - S2 ) * y20 + Ar * x20 ) / ( 0.4 * a31 * a31 );

    Hmt( 3, 0 ) = ( ( -0.6 * a12 * a12 + S3 ) * x30 + Ar * y30 ) / ( 0.4 * a12 * a12 );
    Hmt( 3, 1 ) = ( ( -0.6 * a12 * a12 - S3 ) * x30 - Ar * y30 ) / ( 0.4 * a12 * a12 );
    Hmt( 3, 2 ) = -1.0 * x30;

    Hmt( 4, 0 ) = -1.0 * x10;
    Hmt( 4, 1 ) = ( ( -0.6 * a23 * a23 + S1 ) * x10 + Ar * y10 ) / ( 0.4 * a23 * a23 );
    Hmt( 4, 2 ) = ( ( -0.6 * a23 * a23 - S1 ) * x10 - Ar * y10 ) / ( 0.4 * a23 * a23 );

    Hmt( 5, 0 ) = ( ( -0.6 * a31 * a31 - S2 ) * x20 - Ar * y20 ) / ( 0.4 * a31 * a31 );
    Hmt( 5, 1 ) = -1.0 * x20;
    Hmt( 5, 2 ) = ( ( -0.6 * a31 * a31 + S2 ) * x20 + Ar * y20 ) / ( 0.4 * a31 * a31 );
    //cout << "Hmt* 3/16=" << (Hmt*(3.0/16.0)).OutString();

    Matrix Hqm( 6, 6 );
    Hqm.Zero();

    Hqm( 0, 0 ) = -10. ;
    Hqm( 0, 1 ) = 2;
    Hqm( 0, 2 ) = 2;
    Hqm( 1, 0 ) = 2.  ;
    Hqm( 1, 1 ) = -10;
    Hqm( 1, 2 ) = 2;
    Hqm( 2, 0 ) = 2.  ;
    Hqm( 2, 1 ) = 2;
    Hqm( 2, 2 ) = -10;

    Hqm( 3, 3 ) = -10. ;
    Hqm( 3, 4 ) = 2;
    Hqm( 3, 5 ) = 2;
    Hqm( 4, 3 ) = 2.  ;
    Hqm( 4, 4 ) = -10;
    Hqm( 4, 5 ) = 2;
    Hqm( 5, 3 ) = 2.  ;
    Hqm( 5, 4 ) = 2;
    Hqm( 5, 5 ) = -10;

    Matrix Hqt( 6, 3 );
    Hqt = Hqm * ( Hmt * ( 1.0 / 48.0 ) ) ;
    //cout << "Hqt=" << Hqt.OutString();
    //-----------------------------------------------------------------------------------------
    //
    //   Konacno nalazi matricu
    //
    //    Kht= Hqt^T  Kqh Hqt
    //

    Matrix Kht( 3, 3 );
    Kht.Zero();
    Kht.addMatrixTripleProduct( 1.0, Hqt, Kqh, 1.0 );

    //-----------------------------------------------------------------------------------------
    //
    //   Konacno nalazi matricu
    //
    //    Kh= Htv^T  Kht Htv
    //
    Matrix Htv(3, 9 );
    Htv.Zero();

    Htv(0, 0) = x32 / (4 * Ar);
    Htv(0, 1) = y32 / (4 * Ar);
    Htv(0, 2) = 1.0;
    Htv(0, 3) = x13 / (4 * Ar);
    Htv(0, 4) = y13 / (4 * Ar); /* Htv(0,5)=1.0; */
    Htv(0, 6) = x21 / (4 * Ar);
    Htv(0, 7) = y21 / (4 * Ar); /* Htv(0,8)=1.0; */

    Htv(1, 0) = x32 / (4 * Ar);
    Htv(1, 1) = y32 / (4 * Ar); /* Htv(1,2)=1.0; */
    Htv(1, 3) = x13 / (4 * Ar);
    Htv(1, 4) = y13 / (4 * Ar);
    Htv(1, 5) = 1.0;
    Htv(1, 6) = x21 / (4 * Ar);
    Htv(1, 7) = y21 / (4 * Ar); /* Htv(0,8)=1.0; */

    Htv(2, 0) = x32 / (4 * Ar);
    Htv(2, 1) = y32 / (4 * Ar); /* Htv(1,2)=1.0; */
    Htv(2, 3) = x13 / (4 * Ar);
    Htv(2, 4) = y13 / (4 * Ar); /* Htv(2,5)=1.0; */
    Htv(2, 6) = x21 / (4 * Ar);
    Htv(2, 7) = y21 / (4 * Ar);
    Htv(2, 8) = 1.0;

    Matrix Kh(9, 9); // (2,2,0.0);

    Kh.Zero();
    Kh.addMatrixTripleProduct( 1.0, Htv, Kht, 1.0 );

    if ( ! bHqv_Set )
    {
        // Nije postavljena matrica->
        //Hqv = Hqt * Htv; // Sad je postavljena matrica
        bHqv_Set = true;
    }

    //cout<< "Kh=" << Kh.OutString();
    return Kh;
}


double ThreeNodeAndesMembrane::getArea() const
{
    return Ar;
}

bool ThreeNodeAndesMembrane::gotMass() const
{
    return rho > 0.0 ? true : false;
}
#endif











