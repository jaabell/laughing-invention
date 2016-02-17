///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Zhaohui Yang, Xiaoyan Wu, Zhao Cheng
// DATE:               Aug. 2001
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


//////////////////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              EightNodeBrick_u_p_U.cpp
// CLASS:             EightNodeBrick_u_p_U
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class for coupled system
//  "Coupled system": Solid and fluid coexist.
//                    u-- Solid displacement
//                    p-- Pore pressure
//                    U-- Absolute fluid displacement
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic, Zhao Cheng
// PROGRAMMER:        Boris Jeremic, Zhaohui Yang, Xiaoyan Wu, Zhao Cheng
// DATE:              Aug. 2001
// UPDATE HISTORY:    Modified from EightNodeBrick.cpp  reorganized a lot by Xiaoyan
//                    01/24/2002    Xiaoyan
//                    Add the permeability tensor and ks, kf to the constructor  Xiaoyan
//
//                    Clean-up and re-write by Zhao Cheng, 10/20/2004
//
//                    Fixed a bug, and some small modification, ZC 05/2006
//
//                    Nima Tafazzoli, removing pressure from the constructor, July 2012
//                    Nima Tafazzoli, adding options for recorders (Nov. 2012)
//////////////////////////////////////////////////////////////////////////////////////////


#ifndef EIGHTNODEBRICK_U_P_U_CPP
#define EIGHTNODEBRICK_U_P_U_CPP

#include <EightNodeBrick_u_p_U.h>
#include <iostream>
using namespace std;


const int EightNodeBrick_u_p_U::Num_IntegrationPts = 2;
const int EightNodeBrick_u_p_U::Num_TotalGaussPts = 8;
const int EightNodeBrick_u_p_U::Num_Nodes = 8;
const int EightNodeBrick_u_p_U::Num_Dim = 3;
const int EightNodeBrick_u_p_U::Num_Dof = 7;
const int EightNodeBrick_u_p_U::Num_ElemDof = 56;
const double EightNodeBrick_u_p_U::pts[2] = { -0.577350269189626, +0.577350269189626};
const double EightNodeBrick_u_p_U::wts[2] = {1.0, 1.0};
Matrix EightNodeBrick_u_p_U::MCK(Num_ElemDof, Num_ElemDof);
Vector EightNodeBrick_u_p_U::P(Num_ElemDof);

//======================================================================
EightNodeBrick_u_p_U::EightNodeBrick_u_p_U(int element_number,
        int node_numb_1,
        int node_numb_2,
        int node_numb_3,
        int node_numb_4,
        int node_numb_5,
        int node_numb_6,
        int node_numb_7,
        int node_numb_8,
        NDMaterial *Globalmmodel,
        double nn,
        double alf,
        double rs,
        double rf,
        double permb_x,
        double permb_y,
        double permb_z,
        double kks,
        double kkf)
    : Element(element_number,
              ELE_TAG_EightNodeBrick_u_p_U ),
      connectedExternalNodes(Num_Nodes),
      perm(Num_Dim),
      poro(nn),
      alpha(alf),
      rho_s(rs),
      rho_f(rf),
      ks(kks),
      kf(kkf),
      Q(0),
      Ki(0), gauss_points(8, 3), outputVector(EightNodeBrick_U_P_U_OUTPUT_SIZE)
{
    // permeability
    perm(0) = permb_x;
    perm(1) = permb_y;
    perm(2) = permb_z;

    if (perm(0) == 0.0 || perm(1) == 0.0 || perm(2) == 0.0)
    {
        cerr << " Error, EightNodeBrick_u_p_U:: permeability (kx/ky/kz) is zero! \n";
        exit(-1);
    }

    //================================================================
    // added by Nima Tafazzoli (February 2011)
    // getting the body forces from domain instead of input argument
    // for element

    //     bf = bodyforce->getAccelerationFieldVector();

    //================================================================

    connectedExternalNodes( 0) = node_numb_1;
    connectedExternalNodes( 1) = node_numb_2;
    connectedExternalNodes( 2) = node_numb_3;
    connectedExternalNodes( 3) = node_numb_4;
    connectedExternalNodes( 4) = node_numb_5;
    connectedExternalNodes( 5) = node_numb_6;
    connectedExternalNodes( 6) = node_numb_7;
    connectedExternalNodes( 7) = node_numb_8;

    theMaterial = new NDMaterial *[Num_TotalGaussPts];

    if (theMaterial == 0)
    {
        cerr << " EightNodeBrick_u_p_U::EightNodeBrick_u_p_U -- failed allocate material model pointer\n";
        exit(-1);
    }

    for (int i = 0; i < Num_TotalGaussPts; i++)
    {
        theMaterial[i] = Globalmmodel->getCopy();

        if (theMaterial[i] == 0)
        {
            cerr << " EightNodeBrick_u_p_U::EightNodeBrick_u_p_U -- failed allocate material model pointer\n";
            exit(-1);
        }
    }

}

//======================================================================
EightNodeBrick_u_p_U::EightNodeBrick_u_p_U ()
    : Element(0, ELE_TAG_EightNodeBrick_u_p_U ),
      connectedExternalNodes(Num_Nodes), perm(Num_Dim),
      poro(0.0), alpha(1.0), rho_s(0.0), rho_f(0.0), ks(0.0), kf(0.0), Q(0), Ki(0), gauss_points(8, 3), outputVector(EightNodeBrick_U_P_U_OUTPUT_SIZE)
{
    theMaterial = 0;

    for (int j = 0; j < Num_Nodes; j++)
    {
        theNodes[j] = 0;
    }
}

//======================================================================
EightNodeBrick_u_p_U::~EightNodeBrick_u_p_U ()
{
    if (theMaterial)
    {
        delete [] theMaterial;
    }

    for (int j = 0; j < Num_Nodes; j++)
    {
        theNodes[j] = 0;
    }

    if (Q != 0)
    {
        delete Q;
    }

    if (Ki != 0)
    {
        delete Ki;
    }

}

//======================================================================
int EightNodeBrick_u_p_U::getNumExternalNodes (void) const
{
    return Num_Nodes;
}

//======================================================================
const ID &EightNodeBrick_u_p_U::getExternalNodes (void)
{
    return connectedExternalNodes;
}

//======================================================================
Node **EightNodeBrick_u_p_U::getNodePtrs (void)
{
    return theNodes;
}

//======================================================================
int EightNodeBrick_u_p_U::getNumDOF (void)
{
    return Num_ElemDof;
}

//======================================================================
void EightNodeBrick_u_p_U::setDomain (Domain *theDomain)
{
    int i, Ndof;

    if (theDomain == 0)
    {
        for (i = 0; i < Num_Nodes; i++)
        {
            theNodes[i] = 0;
        }
    }

    for (i = 0; i < Num_Nodes; i++)
    {
        theNodes[i] = theDomain->getNode(connectedExternalNodes(i));

        if (theNodes[i] == 0)
        {
            cerr << "Error EightNodeBrick_u_p_U : node not found in the domain" << "\n";
            return ;
        }

        Ndof = theNodes[i]->getNumberDOF();

        if ( Ndof != Num_Dof)
        {
            cerr << "Error EightNodeBrick_u_p_U : has wrong number of DOFs at its nodes" << "\n";
            return ;
        }
    }

    this->DomainComponent::setDomain(theDomain);
    tensor gp = getGaussPts();
}

//======================================================================
int EightNodeBrick_u_p_U::commitState (void)
{
    int retVal = 0;
    int i;

    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "EightNodeBrick-u_p_U::commitState () - failed in base class";
        return (-1);
    }

    for (i = 0; i < Num_TotalGaussPts; i++ )
    {
        retVal += theMaterial[i]->commitState();
    }


    //Forming output
    // stresstensor stress;
    // straintensor strain;
    // straintensor plstrain;
    int ii = 0;
    int gp = 0;
    for ( short GP_c_r = 1 ; GP_c_r <= Num_IntegrationPts ; GP_c_r++ )
    {
        for ( short GP_c_s = 1 ; GP_c_s <= Num_IntegrationPts ; GP_c_s++ )
        {
            for ( short GP_c_t = 1 ; GP_c_t <= Num_IntegrationPts ; GP_c_t++ )
            {
                // i = ((GP_c_r - 1) * Num_IntegrationPts + GP_c_s - 1) * Num_IntegrationPts + GP_c_t - 1;

                const stresstensor &stress = theMaterial[gp]->getStressTensor();
                const straintensor &strain = theMaterial[gp]->getStrainTensor();
                const straintensor &plstrain = theMaterial[gp]->getPlasticStrainTensor();

                //Write strain
                outputVector(ii++) = strain.cval(1, 1);
                outputVector(ii++) = strain.cval(2, 2);
                outputVector(ii++) = strain.cval(3, 3);
                outputVector(ii++) = strain.cval(1, 2);
                outputVector(ii++) = strain.cval(1, 3);
                outputVector(ii++) = strain.cval(2, 3);

                //Write strain
                outputVector(ii++) = plstrain.cval(1, 1);
                outputVector(ii++) = plstrain.cval(2, 2);
                outputVector(ii++) = plstrain.cval(3, 3);
                outputVector(ii++) = plstrain.cval(1, 2);
                outputVector(ii++) = plstrain.cval(1, 3);
                outputVector(ii++) = plstrain.cval(2, 3);

                //Write stress
                outputVector(ii++) = stress.cval(1, 1);
                outputVector(ii++) = stress.cval(2, 2);
                outputVector(ii++) = stress.cval(3, 3);
                outputVector(ii++) = stress.cval(1, 2);
                outputVector(ii++) = stress.cval(1, 3);
                outputVector(ii++) = stress.cval(2, 3);

                //Pore pressure at gauss points
                double x1 = pts[GP_c_r];
                double x2 = pts[GP_c_s];
                double x3 = pts[GP_c_t];
                outputVector(ii++) = getPorePressure(x1, x2, x3);
                gp++;
            }
        }
    }
    return retVal;
}

//======================================================================
int EightNodeBrick_u_p_U::revertToLastCommit (void)
{
    int retVal = 0;
    int i;

    for (i = 0; i < Num_TotalGaussPts; i++ )
    {
        retVal += theMaterial[i]->revertToLastCommit() ;
    }

    return retVal;
}

//======================================================================
int EightNodeBrick_u_p_U::revertToStart (void)
{
    int retVal = 0;
    int i;

    for (i = 0; i < Num_TotalGaussPts; i++ )
    {
        retVal += theMaterial[i]->revertToStart() ;
    }

    return retVal;
}

//======================================================================
const Matrix &EightNodeBrick_u_p_U::getTangentStiff (void)
{
    return this->getStiff(1);
}

//======================================================================
const Matrix &EightNodeBrick_u_p_U::getInitialStiff (void)
{
    return this->getStiff(0);
}

//======================================================================
const Matrix &EightNodeBrick_u_p_U::getDamp (void)
{
    MCK.Zero();  // necessary

    tensor tC = getDampTensorC123();



# ifdef _DEBUG_PRINT_
    // PRINTING
    // PRINTING      tC.print("tc","tc in const Matrix& EightNodeBrick_u_p_U::getDamp (void) ");
#endif
    int i, j, m, n;

    double Ctemp = 0.0;
    tensor CRm;
    tensor CRk;

    if (a0 != 0.0)
# ifdef _DEBUG_PRINT_
        // PRINTING     fprintf(stdout,"      CRm = getMassTensorMsf(); in const Matrix& EightNodeBrick_u_p_U::getDamp (void) ");
#endif


        CRm = getMassTensorMsf();

    if (a1 != 0.0)
    {
        CRk = getStiffnessTensorKep();
    }

    if (a2 != 0.0 || a3 != 0.0)
    {
        cerr << "Warning: EightNodeBrick-u_p_U:: a2 or a3 are not used" << "\n";
    }

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            for ( m = 0; m < Num_Dim; m++)
            {
                for ( n = 0; n < Num_Dim; n++)
                {
                    Ctemp = tC.cval(i + 1, m + 1, n + 1, j + 1) * (poro * poro);

                    //C1
                    MCK(i * Num_Dof + m, j * Num_Dof + n) = Ctemp;

                    //                    if (a0 != 0.0)
                    //                    MCK(i*Num_Dof+m, j*Num_Dof+n) += CRm.cval(i+1, j+1) *((1.0-poro)*rho_s *a0);

                    //                    if (betaK != 0.0)
                    //                    MCK(i*Num_Dof+m, j*Num_Dof+n) += CRk.cval(i+1, m+1, n+1, j+1) * betaK;

                    //C3
                    MCK(i * Num_Dof + m + 4, j * Num_Dof + n + 4) = Ctemp;

                    //                    if (a0 != 0.0)
                    //                      MCK(i*Num_Dof+m+4, j*Num_Dof+n+4) += CRm.cval(i+1, j+1) *(poro*rho_f *a0);

                    //C2 and C2^T
                    MCK(i * Num_Dof + m, j * Num_Dof + n + 4) = - Ctemp;
                    MCK(j * Num_Dof + n + 4, i * Num_Dof + m) = - Ctemp;
                }
            }
        }
    }



# ifdef _DEBUG_PRINT_

    // PRINTING  // PRINTING  // PRINTING
    for ( i = 0 ; i < 56; i++ ) // 1-56
    {
        for ( j = 0; j < 56; j++ ) // 1-56
        {
            //            fprintf(stderr,"Mck(%3d, %3d) = %+12.6e   ", i+1, j+1, MCK(i, j));
            fprintf(stderr, "%+8.4e ", MCK(i, j));
        }

        fprintf(stderr, "\n");

    }

    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    //      getchar();
    // PRINTING
#endif


    return MCK;
}

//======================================================================
const Matrix &EightNodeBrick_u_p_U::getMass (void)
{
    MCK.Zero();  // necessary

    // PRINTING     fprintf(stdout,"        tensor tM = getMassTensorMsf();  in   const Matrix& EightNodeBrick_u_p_U::getMass (void)");
    tensor tM = getMassTensorMsf();

    int i, j;
    double Mtemp1 = 0.0;
    double Mtemp2 = 0.0;

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {

            //Ms, Note *(1.0-poro)*rho_s here!
            Mtemp1 = tM.cval(i + 1, j + 1) * (1.0 - poro) * rho_s;

            MCK(i * Num_Dof + 0, j * Num_Dof + 0) = Mtemp1;
            MCK(i * Num_Dof + 1, j * Num_Dof + 1) = Mtemp1;
            MCK(i * Num_Dof + 2, j * Num_Dof + 2) = Mtemp1;

            //Mf, Note *poro*rho_f here!
            Mtemp2 = tM.cval(i + 1, j + 1) * poro * rho_f;

            MCK(i * Num_Dof + 4, j * Num_Dof + 4) = Mtemp2;
            MCK(i * Num_Dof + 5, j * Num_Dof + 5) = Mtemp2;
            MCK(i * Num_Dof + 6, j * Num_Dof + 6) = Mtemp2;
        }
    }




# ifdef _DEBUG_PRINT_
    // PRINTING  // PRINTING
    fprintf(stderr, "MCK in const Matrix& EightNodeBrick_u_p_U::getMass (void) \n");

    for ( i = 0 ; i < 56; i++ ) // 1-56
    {
        for ( j = 0; j < 56; j++ ) // 1-56
        {
            //            fprintf(stderr,"Mck(%3d, %3d) = %+12.6e   ", i+1, j+1, MCK(i, j));
            fprintf(stderr, "%+8.4e ", MCK(i, j));
        }

        fprintf(stderr, "\n");

    }

    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    //      getchar();
    // PRINTING
#endif


    return MCK;
}

//======================================================================
void EightNodeBrick_u_p_U::zeroLoad()
{
    if ( Q != 0 )
    {
        Q->Zero();
    }
}

//======================================================================
int EightNodeBrick_u_p_U::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    int type;
    const Vector &data = theLoad->getData(type, loadFactor);

    if ( type == LOAD_TAG_ElementSelfWeight )
    {
        Vector Fbody = this->getBodyForce(data);

        if ( Q == 0 )
        {
            Q = new Vector(Num_ElemDof);
        }

        *Q = Fbody * loadFactor;
    }

    else
    {
        cerr << "EightNodeBrick_u_p_U::addLoad() " << this->getTag() << ", load type unknown\n";
        return -1;
    }

    return 0;
}

//======================================================================
int EightNodeBrick_u_p_U::addInertiaLoadToUnbalance(const Vector &accel)
{
    static Vector avu(Num_ElemDof);

    int i, ik;

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &RA = theNodes[i]->getRV(accel);

        if ( RA.Size() != Num_Dof )
        {
            cerr << "EightNodeBrick_u_p_U::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
            return (-1);
        }

        ik = i * Num_Dof;

        avu(ik + 0) = RA(0);
        avu(ik + 1) = RA(1);
        avu(ik + 2) = RA(2);
        avu(ik + 3) = 0.0;
        avu(ik + 4) = RA(4);
        avu(ik + 5) = RA(5);
        avu(ik + 6) = RA(6);
    }

    if (Q == 0)
    {
        Q = new Vector(Num_ElemDof);
    }

    this->getMass();
    Q->addMatrixVector(1.0, MCK, avu, -1.0);

    return 0;
}

//========================================================================
const Vector &EightNodeBrick_u_p_U::getResistingForce ()
{
    static Vector avu(Num_ElemDof);

    P.Zero();

    // Internal
    P = this->getInternalForce();

    // + K0*u
    int i, j;

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &disp = theNodes[i]->getTrialDisp();

        if ( disp.Size() != Num_Dof )
        {
            cerr << "EightNode_Brick_u_p_U::getResistingForce(): matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = disp(j);
        }
    }

    this->getStiff00();
    P.addMatrixVector(1.0, MCK, avu, 1.0);

    if (Q != 0)
    {
        P.addVector(1.0, *Q, -1.0);
    }

    return P;
}

//========================================================================
const Vector &EightNodeBrick_u_p_U::getResistingForceIncInertia ()
{
    static Vector avu(Num_ElemDof);

    this->getResistingForce();

    // + M*a
    int i, j;

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &acc = theNodes[i]->getTrialAccel();

        if ( acc.Size() != Num_Dof )
        {
            cerr << "EightNode_Brick_u_p_U::getResistingForceIncInertia matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = acc(j);
        }
    }

    this->getMass();
    P.addMatrixVector(1.0, MCK, avu, 1.0);

    // + C*v
    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &vel = theNodes[i]->getTrialVel();

        if ( vel.Size() != Num_Dof )
        {
            cerr << "EightNode_Brick_u_p_U::getResistingForceIncInertia matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = vel(j);
        }
    }

    this->getDamp();
    P.addMatrixVector(1.0, MCK, avu, 1.0);

    return P;
}

//=============================================================================
int EightNodeBrick_u_p_U::sendSelf (int commitTag, Channel &theChannel)
{
    // Not implemented yet
    return 0;
}

//=============================================================================
int EightNodeBrick_u_p_U::receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    // Not implemented yet
    return 0;
}

//=============================================================================
int EightNodeBrick_u_p_U::displaySelf (Renderer &theViewer, int displayMode, float fact)
{
    // Not implemented yet
    return 0;
}


//=============================================================================
void EightNodeBrick_u_p_U::Print(ostream &s, int flag)
{
    s << "EightNodeBrick_u_p_U, element id:  " << this->getTag() << "\n";
    s << "Connected external nodes:  " << connectedExternalNodes << "\n";

    s << "Node 1: " << connectedExternalNodes(0) << "\n";
    s << "Node 2: " << connectedExternalNodes(1) << "\n";
    s << "Node 3: " << connectedExternalNodes(2) << "\n";
    s << "Node 4: " << connectedExternalNodes(3) << "\n";
    s << "Node 5: " << connectedExternalNodes(4) << "\n";
    s << "Node 6: " << connectedExternalNodes(5) << "\n";
    s << "Node 7: " << connectedExternalNodes(6) << "\n";
    s << "Node 8: " << connectedExternalNodes(7) << "\n";

    s << "Material model:  " << "\n";

    int GP_c_r, GP_c_s, GP_c_t, where;

    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts ; GP_c_r++ )
    {
        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts ; GP_c_s++ )
        {
            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts ; GP_c_t++ )
            {
                where = (GP_c_r * Num_IntegrationPts + GP_c_s) * Num_IntegrationPts + GP_c_t;
                s << "\n where = " << where + 1 << "\n";
                s << " r= " << GP_c_r << " s= " << GP_c_s << " t= " << GP_c_t << "\n";
                theMaterial[where]->Print(s);
            }
        }
    }

}

//======================================================================
int EightNodeBrick_u_p_U::update()
{
    int ret = 0;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    int Tdisp_dim[] = {Num_Nodes, Num_Dof};
    tensor total_displacements(2, Tdisp_dim, 0.0);
    int tdisp_dim[] = {Num_Nodes, Num_Dim};
    tensor total_disp(2, tdisp_dim, 0.0);

    int dh_dim[] = {Num_Nodes, Num_Dim};
    tensor dh(2, dh_dim, 0.0);

    static straintensor eps;
    static straintensor symmetricEPS;

    tensor dhGlobal;

    total_displacements = getNodesDisp();
    int i;

    for (i = 1; i <= Num_Nodes; i++)
    {
        total_disp.val(i, 1) = total_displacements.cval(i, 1);
        total_disp.val(i, 2) = total_displacements.cval(i, 2);
        total_disp.val(i, 3) = total_displacements.cval(i, 3);
    }

    int GP_c_r, GP_c_s, GP_c_t, where;

    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts; GP_c_r++ )
    {
        r = pts[GP_c_r];

        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts; GP_c_s++ )
        {
            s = pts[GP_c_s];

            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts; GP_c_t++ )
            {
                t = pts[GP_c_t];
                where = (GP_c_r * Num_IntegrationPts + GP_c_s) * Num_IntegrationPts + GP_c_t;
                dhGlobal = dh_Global(r, s, t);
                eps = total_disp("ia") * dhGlobal("ib");
                eps.null_indices();
                //    eps.print("eps","\n\n\n eps  in update before symmetryze11 \n");
                // Boris added this new as it was not returnirning the symmetrized tensor (for some reason?!)
                symmetricEPS = eps.symmetrize11();

                //    symmetricEPS.print("symmetricEPS","\n\n\n symmetricEPS  in update after symmetryze11 \n");

                if ( (theMaterial[where]->setTrialStrain(symmetricEPS) ) )
                {
                    cerr << "EightNodeBrick_u_p_U::update (tag: " << this->getTag() << "), not converged\n";
                }
            }
        }
    }

    return ret;
}

//======================================================================
const Vector &EightNodeBrick_u_p_U::getInternalForce ()
{
    static Vector Pforce(Num_ElemDof);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;
    int where = 0;

    int dh_dim[] = {Num_Nodes, Num_Dim};
    tensor dh(2, dh_dim, 0.0);
    tensor Pins(2, dh_dim, 0.0);

    tensor Jacobian;
    stresstensor Mstress;

    int GP_c_r, GP_c_s, GP_c_t;

    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts; GP_c_r++ )
    {
        r = pts[GP_c_r];
        rw = wts[GP_c_r];

        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts; GP_c_s++ )
        {
            s = pts[GP_c_s];
            sw = wts[GP_c_s];

            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts; GP_c_t++ )
            {
                t = pts[GP_c_t];
                tw = wts[GP_c_t];
                where = (GP_c_r * Num_IntegrationPts + GP_c_s) * Num_IntegrationPts + GP_c_t;
                Jacobian = Jacobian_3D(r, s, t);
                det_of_Jacobian = Jacobian.determinant();
                dh = dh_Global(r, s, t);
                weight = rw * sw * tw * det_of_Jacobian;
                Mstress = this->theMaterial[where]->getStressTensor();
                Pins += (dh("Kj") * Mstress("ij")) * weight;
            }
        }
    }

    Pforce.Zero(); // necessary

    int i, j;

    for (i = 0; i < Num_Nodes; i++)
    {
        for (j = 0; j < Num_Dim; j++)
        {
            Pforce(i * Num_Dof + j) = Pins.cval(i + 1, j + 1);
        }
    }

    return Pforce;
}


//======================================================================
const Vector &EightNodeBrick_u_p_U::getBodyForce(const Vector &data)
{
    static Vector Pforce(Num_ElemDof);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;


    Vector bf(3);
    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);



    int hp_dim[] = {Num_Nodes};
    tensor hp(1, hp_dim, 0.0);
    tensor Pexf(1, hp_dim, 0.0);

    tensor Jacobian;

    int GP_c_r, GP_c_s, GP_c_t;

    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts; GP_c_r++ )
    {
        r = pts[GP_c_r];
        rw = wts[GP_c_r];

        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts; GP_c_s++ )
        {
            s = pts[GP_c_s];
            sw = wts[GP_c_s];

            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts; GP_c_t++ )
            {
                t = pts[GP_c_t];
                tw = wts[GP_c_t];
                hp = shapeFunction(r, s, t);
                Jacobian = Jacobian_3D(r, s, t);
                det_of_Jacobian = Jacobian.determinant();
                weight = rw * sw * tw * det_of_Jacobian;
                Pexf += hp * weight;
            }
        }
    }


# ifdef _DEBUG_PRINT_
    // PRINTING       // PRINTING
    fprintf(stderr, "Pexf  --->  ");

    for (int i = 0; i < Num_Nodes; i++)
    {
        fprintf(stderr, " %12.6e  ", Pexf.cval(i + 1));

    }

    fprintf(stderr, "\n");
    // PRINTING       // PRINTING
#endif

    Pforce.Zero(); // necessary

    for (int i = 0; i < Num_Nodes; i++)
    {
        for (int j = 0; j < Num_Dim; j++)
        {
            Pforce(i * Num_Dof + j) = Pexf.cval(i + 1) * bf(j) * (1.0 - poro) * rho_s;
            Pforce(i * Num_Dof + j + 4) = Pexf.cval(i + 1) * bf(j) * poro * rho_f;
        }
    }


# ifdef _DEBUG_PRINT_
    // PRINTING       // PRINTING
    fprintf(stderr, "Pforce in const Vector& EightNodeBrick_u_p_U::getAccelerationField ()\n");

    for (int i = 0; i < Num_Nodes; i++)
    {
        fprintf(stderr, "PRINTING LOADS: node (i+1)  %i  ", i + 1);
        fprintf(stderr, "Fu(x)= %12.6e   Fu(y)= %12.6e   Fu(z)= %12.6e   ", Pforce(i * Num_Dof + 0), Pforce(i * Num_Dof + 1), Pforce(i * Num_Dof + 2));
        fprintf(stderr, "       Fp= %12.6e      ", Pforce(i * Num_Dof + 3));
        fprintf(stderr, "FU(x)= %12.6e   FU(y)= %12.6e   FU(z)= %12.6e  ", Pforce(i * Num_Dof + 4), Pforce(i * Num_Dof + 5), Pforce(i * Num_Dof + 6));
        //          getchar();

    }

    fprintf(stderr, "\n New Element----------------------------------------\n");
    // PRINTING
#endif



    return Pforce;
}


//======================================================================
const Matrix &EightNodeBrick_u_p_U::getStiff00 (void)
{
    MCK.Zero();  // necessary

    int i, j, m;

    tensor tG   = getStiffnessTensorG12();

    //G1 and G1^T, Note *(alpha-poro) here!
    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            for ( m = 0; m < Num_Dim; m++)
            {
                MCK(i * Num_Dof + m, j * Num_Dof + 3) = -tG.cval(i + 1, m + 1, j + 1) * (alpha - poro);
                MCK(j * Num_Dof + 3, i * Num_Dof + m) = -tG.cval(i + 1, m + 1, j + 1) * (alpha - poro);
            }
        }
    }

    //G2 and G2^T, Note *poro here!
    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            for ( m = 0; m < Num_Dim; m++)
            {
                MCK(i * Num_Dof + m + 4, j * Num_Dof + 3) = -tG.cval(i + 1, m + 1, j + 1) * poro;
                MCK(j * Num_Dof + 3, i * Num_Dof + m + 4) = -tG.cval(i + 1, m + 1, j + 1) * poro;
            }
        }
    }


    //P
    if (ks == 0.0 || kf == 0.0)
    {
        cerr << " Error, EightNodeBrick_u_p_U::getStiffnessTensorP -- solid and/or fluid bulk modulus is zero\n";
        exit(-1);
    }

    double  oneOverQ = poro / kf + (alpha - poro) / ks;

    // PRINTING    fprintf(stdout,"    tensor tP = getMassTensorMsf();     in  const Matrix& EightNodeBrick_u_p_U::getStiff00 (void)  ");

    tensor tP = getMassTensorMsf();

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            MCK(i * Num_Dof + 3, j * Num_Dof + 3) = tP.cval(i + 1, j + 1) * (-oneOverQ);
        }
    }



# ifdef _DEBUG_PRINT_
    // PRINTING  // PRINTING
    fprintf(stderr, "MCK in const Matrix& EightNodeBrick_u_p_U::getStiff00 (void) \n");

    for ( i = 0 ; i < 56; i++ ) // 1-56
    {
        for ( j = 0; j < 56; j++ ) // 1-56
        {
            fprintf(stderr, "%+8.4e ", MCK(i, j));
        }

        fprintf(stderr, "\n");

    }

    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    //      getchar();
    // PRINTING
#endif





    return MCK;
}

//======================================================================
const Matrix &EightNodeBrick_u_p_U::getStiff (int Ki_flag)
{
    if (Ki_flag != 0 && Ki_flag != 1)
    {
        cerr << "Error EightNodeBrick_u_p_U::getStiff() - illegal use\n";
        exit(-1);
    }

    if (Ki_flag == 0 && Ki != 0)
    {
        return *Ki;
    }

    tensor tKep = getStiffnessTensorKep();

    int i, j, m, n;

    this->getStiff00();

    // + Kep
    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            for ( m = 0; m < Num_Dim; m++)
            {
                for ( n = 0; n < Num_Dim; n++)
                {
                    MCK(i * Num_Dof + m, j * Num_Dof + n) = tKep.cval(i + 1, m + 1, n + 1, j + 1);
                }
            }
        }
    }

    if ( Ki_flag == 1)
    {
        return MCK;
    }

    Ki = new Matrix(MCK);

    if (Ki == 0)
    {
        cerr << "Error EightNodeBrick_u_p_U::getStiff() -";
        cerr << "ran out of memory\n";
        exit(-1);
    }




# ifdef _DEBUG_PRINT_
    // PRINTING  // PRINTING
    fprintf(stderr, "MCK in const Matrix& EightNodeBrick_u_p_U::getStiff00 (void) \n");

    for ( i = 0 ; i < 56; i++ ) // 1-56
    {
        for ( j = 0; j < 56; j++ ) // 1-56
        {
            fprintf(stderr, "%+8.4e ", MCK(i, j));
        }

        fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    //    getchar();
    // PRINTING  // PRINTING
#endif


    return *Ki;
}

//======================================================================
tensor EightNodeBrick_u_p_U::getStiffnessTensorKep( )
{
    int K_dim[] = {Num_Nodes, Num_Dim, Num_Dim, Num_Nodes};
    tensor Kep(4, K_dim, 0.0);
    tensor Kkt(4, K_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    int dh_dim[] = {Num_Nodes, Num_Dim};
    tensor dh(2, dh_dim, 0.0);

    tensor Constitutive;

    tensor Jacobian;
    tensor dhGlobal;

    int GP_c_r, GP_c_s, GP_c_t;

    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts; GP_c_r++ )
    {
        r = pts[GP_c_r];
        rw = wts[GP_c_r];

        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts; GP_c_s++ )
        {
            s = pts[GP_c_s];
            sw = wts[GP_c_s];

            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts; GP_c_t++ )
            {
                t = pts[GP_c_t];
                tw = wts[GP_c_t];
                where = (GP_c_r * Num_IntegrationPts + GP_c_s) * Num_IntegrationPts + GP_c_t;
                Jacobian = Jacobian_3D(r, s, t);
                det_of_Jacobian = Jacobian.determinant();
                dhGlobal = dh_Global(r, s, t);
                weight = rw * sw * tw * det_of_Jacobian;
                Constitutive = theMaterial[where]->getTangentTensor();
                Kkt = dhGlobal("kj") * Constitutive("ijml");
                Kkt = Kkt("kiml") * dhGlobal("pl") * weight;
                Kep = Kep + Kkt;
            }
        }
    }

    return Kep;
}

//======================================================================
tensor EightNodeBrick_u_p_U::getStiffnessTensorG12()
{
    // This is for G1 and G2
    // G1 = (alpha-poro) *G;
    // G2 = poro *G;

    int G_dim[] = {Num_Nodes, Num_Dim, Num_Nodes};
    tensor G(3, G_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    double weight = 0.0;

    int dh_dim[] = {Num_Nodes, Num_Dim};
    tensor dh(2, dh_dim, 0.0);
    int hp_dim[] = {Num_Nodes};
    tensor hp(1, hp_dim, 0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor dhGlobal;

    int GP_c_r, GP_c_s, GP_c_t;

    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts; GP_c_r++ )
    {
        r = pts[GP_c_r];
        rw = wts[GP_c_r];

        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts; GP_c_s++ )
        {
            s = pts[GP_c_s];
            sw = wts[GP_c_s];

            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts; GP_c_t++ )
            {
                t = pts[GP_c_t];
                tw = wts[GP_c_t];
                hp = shapeFunction(r, s, t);
                Jacobian = Jacobian_3D(r, s, t);
                dhGlobal = dh_Global(r, s, t);
                det_of_Jacobian = Jacobian.determinant();
                weight = rw * sw * tw * det_of_Jacobian;
                G += dhGlobal("ki") * hp("m") * weight;
            }
        }
    }

    return G;
}

//======================================================================
tensor EightNodeBrick_u_p_U::getDampTensorC123()
{
    // This is for C1, C2 and C3, C1 = C2 = c3
    // Since solid and fluid shape function the same

    int perm_dim[] = {Num_Dim, Num_Dim};
    tensor perm_inv(2, perm_dim, 0.0);

    perm_inv.val(1, 1) = 1.0 / perm(0);
    perm_inv.val(2, 2) = 1.0 / perm(1);
    perm_inv.val(3, 3) = 1.0 / perm(2);

    int C_dim[] = {Num_Nodes, Num_Dim, Num_Dim, Num_Nodes};
    tensor C123(4, C_dim, 0.0);
    int c_dim[] = {Num_Nodes, Num_Dim, Num_Dim};
    tensor c123(3, c_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    int hp_dim[] = {Num_Nodes};
    tensor hp(1, hp_dim, 0.0);

    tensor Jacobian;

    int GP_c_r, GP_c_s, GP_c_t;

    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts; GP_c_r++ )
    {
        r = pts[GP_c_r];
        rw = wts[GP_c_r];

        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts; GP_c_s++ )
        {
            s = pts[GP_c_s];
            sw = wts[GP_c_s];

            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts; GP_c_t++ )
            {
                t = pts[GP_c_t];
                tw = wts[GP_c_t];
                hp = shapeFunction(r, s, t);
                Jacobian = Jacobian_3D(r, s, t);
                det_of_Jacobian = Jacobian.determinant();
                weight = rw * sw * tw * det_of_Jacobian;
                c123 = hp("k") * perm_inv("ij");
                C123 += c123("kij") * hp("m") * weight;
            }
        }
    }


    return C123;
}

//======================================================================
tensor EightNodeBrick_u_p_U::getMassTensorMsf()
{
    // This is for Ms and Mf -> M_kl
    // Ms = Msf * (1.0-poro)*rho_s
    // Mf = Msf * poro*rho_f

    // Also this is for Compression term Pc
    // Pc = Msf * oneOverQ
    //    = Msf * (poro/kf + (alpha-poro)/ks)

    int M_dim[] = {Num_Nodes, Num_Nodes};
    tensor Msf(2, M_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    int dh_dim[] = {Num_Nodes, Num_Dim};
    tensor dh(2, dh_dim, 0.0);
    int hp_dim[] = {Num_Nodes};
    tensor hp(1, hp_dim, 0.0);

    tensor Jacobian;

    int GP_c_r, GP_c_s, GP_c_t;

    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts; GP_c_r++ )
    {
        r = pts[GP_c_r];
        rw = wts[GP_c_r];

        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts; GP_c_s++ )
        {
            s = pts[GP_c_s];
            sw = wts[GP_c_s];

            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts; GP_c_t++ )
            {
                t = pts[GP_c_t];
                tw = wts[GP_c_t];
                hp = shapeFunction(r, s, t);
                Jacobian = Jacobian_3D(r, s, t);
                det_of_Jacobian = Jacobian.determinant();
                weight = rw * sw * tw * det_of_Jacobian;
                Msf += hp("m") * hp("n") * weight;
            }
        }
    }

# ifdef _DEBUG_PRINT_
    // PRINTING // PRINTING     // PRINTING
    Msf.print("Msf", "\n Msf tensor from tensor EightNodeBrick_u_p_U::getMassTensorMsf()");
    // PRINTING     getchar();
    // PRINTING
#endif

    return Msf;
}

//======================================================================
tensor EightNodeBrick_u_p_U::Jacobian_3D(double x, double y, double z)
{
    tensor N_C = this->getNodesCrds();
    tensor dh = this->shapeFunctionDerivative(x, y, z);

    tensor J3D = N_C("ki") * dh("kj");
    J3D.null_indices();
    return J3D;
}

//======================================================================
tensor EightNodeBrick_u_p_U::Jacobian_3Dinv(double x, double y, double z)
{
    tensor J = this->Jacobian_3D(x, y, z);
    return J.inverse();
}

//======================================================================
tensor EightNodeBrick_u_p_U::dh_Global(double x, double y, double z)
{
    tensor JacobianINV0 = this->Jacobian_3Dinv(x, y, z);
    tensor dh = this->shapeFunctionDerivative(x, y, z);
    tensor  dhGlobal = dh("ik") * JacobianINV0("kj");
    dhGlobal.null_indices();
    return dhGlobal;
}

//======================================================================
tensor EightNodeBrick_u_p_U::getNodesCrds(void)
{
    int i, j;
    int dimX[] = {Num_Nodes, Num_Dim};
    tensor N_coord(2, dimX, 0.0);

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &TNodesCrds = theNodes[i]->getCrds();

        for (j = 0; j < Num_Dim; j++)
        {
            N_coord.val(i + 1, j + 1) = TNodesCrds(j);
        }
    }

    return N_coord;
}

//======================================================================
tensor EightNodeBrick_u_p_U::getNodesDisp(void)
{
    int i, j;
    int dimU[] = {Num_Nodes, Num_Dof};
    tensor total_disp(2, dimU, 0.0);

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &TNodesDisp = theNodes[i]->getTrialDisp();

        for (j = 0; j < Num_Dof; j++)
        {
            total_disp.val(i + 1, j + 1) = TNodesDisp(j);
        }
    }

    return total_disp;
}

//======================================================================
double EightNodeBrick_u_p_U::getPorePressure(double x1, double x2, double x3)
{
    double pp = 0.0;
    tensor h = shapeFunction(x1, x2, x3);

    for (int node = 0; node < Num_Nodes; node++)
    {
        const Vector &T_disp = theNodes[node]->getTrialDisp();
        pp += h.cval(node + 1) * T_disp(3);
    }

    return pp;
}

//======================================================================
tensor EightNodeBrick_u_p_U::shapeFunction(double r1, double r2, double r3)
{
    int Hfun[] = {Num_Nodes};
    tensor h(1, Hfun, 0.0);

    h.val(8) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) * 0.125;
    h.val(7) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) * 0.125;
    h.val(6) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) * 0.125;
    h.val(5) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) * 0.125;
    h.val(4) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) * 0.125;
    h.val(3) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) * 0.125;
    h.val(2) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) * 0.125;
    h.val(1) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) * 0.125;

    return h;
}


//==============================================================
tensor EightNodeBrick_u_p_U::shapeFunctionDerivative(double r1, double r2, double r3)
{
    int DHfun[] = {Num_Nodes, Num_Dim};
    tensor dh(2, DHfun, 0.0);

    //  node number 8
    dh.val(8, 1) = (1.0 - r2) * (1.0 - r3) * 0.125;
    dh.val(8, 2) = -(1.0 + r1) * (1.0 - r3) * 0.125;
    dh.val(8, 3) = -(1.0 + r1) * (1.0 - r2) * 0.125;
    //  node number 7
    dh.val(7, 1) = -(1.0 - r2) * (1.0 - r3) * 0.125;
    dh.val(7, 2) = -(1.0 - r1) * (1.0 - r3) * 0.125;
    dh.val(7, 3) = -(1.0 - r1) * (1.0 - r2) * 0.125;
    //  node number 6
    dh.val(6, 1) = -(1.0 + r2) * (1.0 - r3) * 0.125;
    dh.val(6, 2) = (1.0 - r1) * (1.0 - r3) * 0.125;
    dh.val(6, 3) = -(1.0 - r1) * (1.0 + r2) * 0.125;
    //  node number 5
    dh.val(5, 1) = (1.0 + r2) * (1.0 - r3) * 0.125;
    dh.val(5, 2) = (1.0 + r1) * (1.0 - r3) * 0.125;
    dh.val(5, 3) = -(1.0 + r1) * (1.0 + r2) * 0.125;
    //  node number 4
    dh.val(4, 1) = (1.0 - r2) * (1.0 + r3) * 0.125;
    dh.val(4, 2) = -(1.0 + r1) * (1.0 + r3) * 0.125;
    dh.val(4, 3) = (1.0 + r1) * (1.0 - r2) * 0.125;
    //  node number 3
    dh.val(3, 1) = -(1.0 - r2) * (1.0 + r3) * 0.125;
    dh.val(3, 2) = -(1.0 - r1) * (1.0 + r3) * 0.125;
    dh.val(3, 3) = (1.0 - r1) * (1.0 - r2) * 0.125;
    //  node number 2
    dh.val(2, 1) = -(1.0 + r2) * (1.0 + r3) * 0.125;
    dh.val(2, 2) = (1.0 - r1) * (1.0 + r3) * 0.125;
    dh.val(2, 3) = (1.0 - r1) * (1.0 + r2) * 0.125;
    //  node number 1
    dh.val(1, 1) = (1.0 + r2) * (1.0 + r3) * 0.125;
    dh.val(1, 2) = (1.0 + r1) * (1.0 + r3) * 0.125;
    dh.val(1, 3) = (1.0 + r1) * (1.0 + r2) * 0.125;

    return dh;
}

//==============================================================
tensor EightNodeBrick_u_p_U::getGaussPts(void)
{
    int dimensions1[] = {Num_TotalGaussPts, Num_Dim};
    tensor Gs(2, dimensions1, 0.0);
    int dimensions2[] = {Num_Nodes};
    tensor shp(1, dimensions2, 0.0);

    double r = 0.0;
    double s = 0.0;
    double t = 0.0;
    int i, j, where;

    int GP_c_r, GP_c_s, GP_c_t;
    int gp = 0;
    for ( GP_c_r = 0 ; GP_c_r < Num_IntegrationPts; GP_c_r++ )
    {
        r = pts[GP_c_r];

        for ( GP_c_s = 0 ; GP_c_s < Num_IntegrationPts; GP_c_s++ )
        {
            s = pts[GP_c_s];

            for ( GP_c_t = 0 ; GP_c_t < Num_IntegrationPts; GP_c_t++ )
            {
                t = pts[GP_c_t];
                where = (GP_c_r * Num_IntegrationPts + GP_c_s) * Num_IntegrationPts + GP_c_t;
                shp = shapeFunction(r, s, t);

                for (i = 0; i < Num_Nodes; i++)
                {
                    const Vector &T_Crds = theNodes[i]->getCrds();

                    for (j = 0; j < Num_Dim; j++)
                    {
                        Gs.val(where + 1, j + 1) += shp.cval(i + 1) * T_Crds(j);
                    }
                }
                gauss_points(gp, 0) = Gs.val(where + 1,  1);
                gauss_points(gp, 1) = Gs.val(where + 1,  2);
                gauss_points(gp, 2) = Gs.val(where + 1,  3);
                gp++;
            }
        }
    }

    return Gs;
}


const Matrix &EightNodeBrick_u_p_U::getTangent( int gaussNum )
{
    if ( gaussNum < 8 && gaussNum > -1 )
    {
        return   theMaterial[gaussNum]->getTangent();
    }

    cerr << " Brick::no tangent for gauss " << gaussNum;
    static Matrix t(1, 1);
    return t;
}



//==================================================================================
Vector *
EightNodeBrick_u_p_U::getStress(void)
{

    int i = 0;
    int j = 0;
    stresstensor sts;
    Vector *stresses = new Vector(48);


    for ( short GP_c_r = 1 ; GP_c_r <= Num_IntegrationPts ; GP_c_r++ )
    {
        for ( short GP_c_s = 1 ; GP_c_s <= Num_IntegrationPts ; GP_c_s++ )
        {
            for ( short GP_c_t = 1 ; GP_c_t <= Num_IntegrationPts ; GP_c_t++ )
            {
                i = ((GP_c_r - 1) * Num_IntegrationPts + GP_c_s - 1) * Num_IntegrationPts + GP_c_t - 1;

                sts = theMaterial[i]->getStressTensor();
                (*stresses)(j * 6 + 0) = sts.cval(1, 1); //sigma_xx
                (*stresses)(j * 6 + 1) = sts.cval(2, 2); //sigma_yy
                (*stresses)(j * 6 + 2) = sts.cval(3, 3); //sigma_zz
                (*stresses)(j * 6 + 3) = sts.cval(1, 2); //Assign sigma_xy
                (*stresses)(j * 6 + 4) = sts.cval(1, 3); //Assign sigma_xz
                (*stresses)(j * 6 + 5) = sts.cval(2, 3); //Assign sigma_yz

                j++;
            }
        }
    }

    return stresses;

}




Matrix &EightNodeBrick_u_p_U::getGaussCoordinates(void)
{
    return gauss_points;
}



int EightNodeBrick_u_p_U::getOutputSize() const
{
    return EightNodeBrick_U_P_U_OUTPUT_SIZE;
}



const Vector &EightNodeBrick_u_p_U::getOutput()
{


    return outputVector;
}




#endif



