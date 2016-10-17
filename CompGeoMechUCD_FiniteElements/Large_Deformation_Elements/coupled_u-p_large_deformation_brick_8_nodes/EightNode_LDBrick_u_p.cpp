///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Zhao Cheng, Boris Jeremic
// DATE:               Aug. 2006
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


#ifndef EIGHTNODE_LDBRICK_U_P_CPP
#define EIGHTNODE_LDBRICK_U_P_CPP

#include <EightNode_LDBrick_u_p.h>

const int EightNode_LDBrick_u_p::Num_IntegrationPts = 2;
const int EightNode_LDBrick_u_p::Num_TotalGaussPts = 8;
const int EightNode_LDBrick_u_p::Num_Nodes = 8;
const int EightNode_LDBrick_u_p::Num_Dim = 3;
const int EightNode_LDBrick_u_p::Num_Dof = 4;
const int EightNode_LDBrick_u_p::Num_ElemDof = Num_Nodes * Num_Dof;
const double EightNode_LDBrick_u_p::pts[2] = { -0.577350269189626, +0.577350269189626};
const double EightNode_LDBrick_u_p::wts[2] = {1.0, 1.0};
Matrix EightNode_LDBrick_u_p::MCK(Num_ElemDof, Num_ElemDof);
Vector EightNode_LDBrick_u_p::P(Num_ElemDof);
tensor EightNode_LDBrick_u_p::perm(2, def_dim_2, 0.0);

//======================================================================
EightNode_LDBrick_u_p::EightNode_LDBrick_u_p(int element_ID,
        int node_numb_1,
        int node_numb_2,
        int node_numb_3,
        int node_numb_4,
        int node_numb_5,
        int node_numb_6,
        int node_numb_7,
        int node_numb_8,
        NDMaterial *Globalmmodel,
        double b1,
        double b2,
        double b3,
        double fn,
        double rs,
        double rf,
        double permb_x,
        double permb_y,
        double permb_z,
        double kkf)
    : Element(element_ID, ELE_TAG_EightNode_LDBrick_u_p ),
      connectedExternalNodes(Num_Nodes), bf(Num_Dim), nf(fn),
      rho_s(rs), rho_f(rf), kf(kkf), Q(0), Ki(0)
{
    
    this->setMaterialTag(Globalmmodel->getTag());

    // body forces
    bf(0) = b1;
    bf(1) = b2;
    bf(2) = b3;

    // permeability
    if (permb_x == 0.0 || permb_y == 0.0 || permb_z == 0.0)
    {
        cerr << " Error EightNode_LDBrick_u_p::getDampTensorC123 -- permeability (x/y/z) is zero\n";
        exit(-1);
    }

    perm.val(1, 1) = permb_x;
    perm.val(2, 2) = permb_y;
    perm.val(3, 3) = permb_z;

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
        cerr << " Error! EightNode_LDBrick_u_p::EightNode_LDBrick_u_p -- failed allocate material model pointer. \n";
        exit(-1);
    }

    int i;

    for (i = 0; i < Num_TotalGaussPts; i++)
    {
        theMaterial[i] = Globalmmodel->getCopy();

        if (theMaterial[i] == 0)
        {
            cerr << " Error! EightNode_LDBrick_u_p::EightNode_LDBrick_u_p -- failed allocate material model pointer. \n";
            exit(-1);
        }
    }

    if (kf < 1.0e-6)
    {
        cerr << " Error! EightNode_LDBrick_u_p::EightNode_LDBrick_u_p -- zero or almost compression modulus! \n";
        exit(-1);
    }
}

//======================================================================
EightNode_LDBrick_u_p::EightNode_LDBrick_u_p ()
    : Element(0, ELE_TAG_EightNode_LDBrick_u_p ),
      connectedExternalNodes(Num_Nodes), bf(Num_Dim),
      nf(0.0), rho_s(0.0), rho_f(0.0), kf(0.0), Q(0), Ki(0)
{
    theMaterial = 0;

    int i;

    for (i = 0; i < Num_Nodes; i++)
    {
        theNodes[i] = 0;
    }
}

//======================================================================
EightNode_LDBrick_u_p::~EightNode_LDBrick_u_p ()
{
    int i;

    for (i = 0; i < Num_TotalGaussPts; i++)
    {
        if (theMaterial[i])
        {
            delete theMaterial[i];
        }
    }

    if (theMaterial)
    {
        delete [] theMaterial;
    }

    for (i = 0; i < Num_Nodes; i++)
    {
        theNodes[i] = 0;
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
int EightNode_LDBrick_u_p::getNumExternalNodes (void) const
{
    return Num_Nodes;
}

//======================================================================
const ID &EightNode_LDBrick_u_p::getExternalNodes (void)
{
    return connectedExternalNodes;
}

//======================================================================
Node **EightNode_LDBrick_u_p::getNodePtrs (void)
{
    return theNodes;
}

//======================================================================
int EightNode_LDBrick_u_p::getNumDOF (void)
{
    return Num_ElemDof;
}

//======================================================================
void EightNode_LDBrick_u_p::setDomain (Domain *theDomain)
{
    int i;
    int Ndof;

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
            cerr << "Error: EightNode_LDBrick_u_p: node not found in the domain" << "\n";
            return ;
        }

        Ndof = theNodes[i]->getNumberDOF();

        if ( Ndof != Num_Dof )
        {
            cerr << "Error: EightNode_LDBrick_u_p: has wrong number of DOFs at its nodes " << i << " \n";
            return ;
        }
    }
    
    this->DomainComponent::setDomain(theDomain);

}

//======================================================================
int EightNode_LDBrick_u_p::commitState (void)
{
    int retVal = 0;
    int i;

    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "EightNode_LDBrick_u_p::commitState () - failed in base class";
        return (-1);
    }

    for (i = 0; i < Num_TotalGaussPts; i++ )
    {
        retVal += theMaterial[i]->commitState();
    }

    return retVal;
}

//======================================================================
int EightNode_LDBrick_u_p::revertToLastCommit (void)
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
int EightNode_LDBrick_u_p::revertToStart (void)
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
const Matrix &EightNode_LDBrick_u_p::getTangentStiff (void)
{
    return getStiff(1);
}

//======================================================================
const Matrix &EightNode_LDBrick_u_p::getInitialStiff (void)
{
    return getStiff(0);
}

//======================================================================
const Matrix &EightNode_LDBrick_u_p::getDamp (void)
{
    MCK.Zero();

    int i, j, m;

    tensor C1 = this->getDampingTensorC1();

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            for ( m = 0; m < Num_Dim; m++)
            {
                MCK(i * Num_Dof + 3, j * Num_Dof + m) = C1.cval(i + 1, j + 1, m + 1);
            }
        }
    }

    tensor C2 = this->getDampingTensorC2();

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            MCK(i * Num_Dof + 3, j * Num_Dof + 3) = C2.cval(i + 1, j + 1);
        }
    }

    return MCK;
}

//======================================================================
const Matrix &EightNode_LDBrick_u_p::getMass (void)
{
    MCK.Zero();

    int i, j, m;

    tensor M1 = this->getMassTensorM1();

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            MCK(i * Num_Dof + 0, j * Num_Dof + 0) = M1.cval(i + 1, j + 1);
            MCK(i * Num_Dof + 1, j * Num_Dof + 1) = M1.cval(i + 1, j + 1);
            MCK(i * Num_Dof + 2, j * Num_Dof + 2) = M1.cval(i + 1, j + 1);
        }
    }

    tensor M2 = this->getMassTensorM2();

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            for ( m = 0; m < Num_Dim; m++)
            {
                MCK(i * Num_Dof + 3, j * Num_Dof + m) = M2.cval(i + 1, j + 1, m + 1);
            }
        }
    }

    return MCK;
}

//======================================================================
void EightNode_LDBrick_u_p::zeroLoad()
{
    if ( Q != 0 )
    {
        Q->Zero();
    }
}

//======================================================================
int EightNode_LDBrick_u_p::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    int type;
    const Vector &data = theLoad->getData(type, loadFactor);

    if ( type == LOAD_TAG_BrickSelfWeight )
    {
        if ( Q == 0 )
        {
            Q = new Vector(Num_ElemDof);
        }

        *Q = ( this->getForceU() + this->getForceP() ) * loadFactor;
    }
    else
    {
        cerr << "EightNode_LDBrick_u_p::addLoad() " << this->getTag() << ", load type unknown\n";
        return -1;
    }

    return 0;
}

//======================================================================
int EightNode_LDBrick_u_p::addInertiaLoadToUnbalance(const Vector &accel)
{
    static Vector ra(Num_ElemDof);

    int i;

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &RA = theNodes[i]->getRV(accel);

        if ( RA.Size() != Num_Dof )
        {
            cerr << "EightNode_LDBrick_u_p::addInertiaLoadToUnbalance(): matrix and vector sizes are incompatable \n";
            return (-1);
        }

        ra(i * Num_Dof + 0) = RA(0);
        ra(i * Num_Dof + 1) = RA(1);
        ra(i * Num_Dof + 2) = RA(2);
        ra(i * Num_Dof + 3) = 0.0;
    }

    if (Q == 0)
    {
        Q = new Vector(Num_ElemDof);
    }

    this->getMass();
    Q->addMatrixVector(1.0, MCK, ra, -1.0);

    return 0;
}

//========================================================================
const Vector &EightNode_LDBrick_u_p::getResistingForce ()
{
    int i, j;
    static Vector avu(Num_ElemDof);

    P.Zero();

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &disp = theNodes[i]->getTrialDisp();

        if ( disp.Size() != Num_Dof )
        {
            cerr << "EightNode_LDBrick_u_p::getResistingForce(): matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = disp(j);
        }
    }

    this->getStiffnessK0();
    P.addMatrixVector(0.0, MCK, avu, 1.0);

    Vector Fi =  this->getInternalForce();
    P.addVector(1.0, Fi, 1.0);

    if (Q != 0)
    {
        P.addVector(1.0, *Q, -1.0);
    }

    return P;
}

//========================================================================
const Vector &EightNode_LDBrick_u_p::getResistingForceIncInertia ()
{
    int i, j;
    static Vector avu(Num_ElemDof);

    this->getResistingForce();

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &acc = theNodes[i]->getTrialAccel();

        if ( acc.Size() != Num_Dof )
        {
            cerr << "EightNode_Brick_u_p::getResistingForceIncInertia matrix and vector sizes are incompatable \n";
            exit(-1);
        }

        for (j = 0; j < Num_Dof; j++)
        {
            avu(i * Num_Dof + j) = acc(j);
        }
    }

    this->getMass();
    P.addMatrixVector(1.0, MCK, avu, 1.0);

    for (i = 0; i < Num_Nodes; i++)
    {
        const Vector &vel = theNodes[i]->getTrialVel();

        if ( vel.Size() != Num_Dof )
        {
            cerr << "EightNode_Brick_u_p::getResistingForceIncInertia matrix and vector sizes are incompatable \n";
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
int EightNode_LDBrick_u_p::sendSelf (int commitTag, Channel &theChannel)
{
    // Not implemtented yet
    return 0;
}

//=============================================================================
int EightNode_LDBrick_u_p::receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    // Not implemtented yet
    return 0;
}


// //=============================================================================
// Response* EightNode_LDBrick_u_p::setResponse(const char** argv, int argc, Information& eleInfo)
// {
//     if (strcmp(argv[0], "stresses") == 0)
//     {
//         return new ElementResponse(this, 1, Vector(Num_TotalGaussPts * 6) );
//     }

//     else if (strcmp(argv[0], "gausspoint") == 0 || strcmp(argv[0], "GaussPoint") == 0)
//     {
//         return new ElementResponse(this, 2, Vector(Num_TotalGaussPts * Num_Dim) );
//     }

//     else
//     {
//         return 0;
//     }
// }

// //=============================================================================
// int EightNode_LDBrick_u_p::getResponse(int responseID, Information& eleInfo)
// {
//     if (responseID == 1)
//     {
//         static Vector CauchyStr(Num_TotalGaussPts * 6);
//         stresstensor sigma;
//         int i;

//         for (i = 0; i < Num_TotalGaussPts; i++)
//         {
//             sigma = theMaterial[i]->getCauchyStressTensor();
//             //sigma.print("C", " ");
//             CauchyStr(i * 6  ) = sigma.cval(1, 1); //xx
//             CauchyStr(i * 6 + 1) = sigma.cval(2, 2); //yy
//             CauchyStr(i * 6 + 2) = sigma.cval(3, 3); //zz
//             CauchyStr(i * 6 + 3) = sigma.cval(2, 3); //yz
//             CauchyStr(i * 6 + 4) = sigma.cval(3, 1); //zx
//             CauchyStr(i * 6 + 5) = sigma.cval(2, 3); //xy
//         }

//         return eleInfo.setVector(CauchyStr);
//     }

//     else if (responseID == 2)
//     {
//         static Vector Gpts(Num_TotalGaussPts * Num_Dim);
//         tensor GCoord;
//         int cnt = 0;
//         int i, j;
//         GCoord = getGaussPts();

//         for (i = 0; i < Num_TotalGaussPts; i++)
//         {
//             for (j = 0; j < Num_Dim; j++)
//             {
//                 Gpts(cnt++) = GCoord.cval(i + 1, j + 1);
//             }
//         }

//         return eleInfo.setVector(Gpts);
//     }

//     else
//     {
//         return (-1);
//     }
// }

//=============================================================================
void EightNode_LDBrick_u_p::Print(ostream &s, int flag)
{
    s << "EightNode_LDBrick_u_p, element id:  " << this->getTag() << "\n";
    s << "Connected external nodes:  " << connectedExternalNodes << "\n";

    s << "Node  1: " << connectedExternalNodes(0) << "\n";
    s << "Node  2: " << connectedExternalNodes(1) << "\n";
    s << "Node  3: " << connectedExternalNodes(2) << "\n";
    s << "Node  4: " << connectedExternalNodes(3) << "\n";
    s << "Node  5: " << connectedExternalNodes(4) << "\n";
    s << "Node  6: " << connectedExternalNodes(5) << "\n";
    s << "Node  7: " << connectedExternalNodes(6) << "\n";
    s << "Node  8: " << connectedExternalNodes(7) << "\n";

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
int EightNode_LDBrick_u_p::update()
{
    int ret = 0;
    int where;
    int i, j;

    tensor I2("I", 2, def_dim_2);

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    int tdisp_dim[] = {Num_Nodes, Num_Dim};
    tensor total_disp(2, tdisp_dim, 0.0);

    int dh_dim[] = {Num_Nodes, Num_Dim};
    tensor dh(2, dh_dim, 0.0);

    straintensor ff;

    tensor t_disp = this->getNodesDisp();
    //t_disp.print("u"," ");

    for (i = 1; i <= Num_Nodes; i++)
    {
        for (j = 1; j <= Num_Dim; j++)
        {
            total_disp.val(i, j) = t_disp.cval(i, j);
        }
    }

    int GP_c_r, GP_c_s, GP_c_t;

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
                dh = dh_Global(r, s, t);
                ff = total_disp("Ia") * dh("Ib");
                ff = ff + I2;

                if ( (theMaterial[where]->setTrialF(ff) ) )
                {
                    cerr << "EightNode_LDBrick_u_p::update (tag: " << this->getTag() << "), not converged\n";
                }
            }
        }
    }

    return ret;
}

//======================================================================
const Vector &EightNode_LDBrick_u_p::getInternalForce ()
{
    static Vector PpI(Num_ElemDof);

    int U_dim[] = {Num_Nodes, Num_Dim};
    tensor Ut(2, U_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    stresstensor ST;

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
                ST = theMaterial[where]->getPK1StressTensor();
                Ut += dhGlobal("aJ") * ST("iJ") * weight;
            }
        }
    }

    PpI.Zero();

    int i, j;

    for (i = 0; i < Num_Nodes; i++)
    {
        for (j = 0; j < Num_Dim; j++)
        {
            PpI(i * Num_Dof + j) = Ut.cval(i + 1, j + 1);
        }
    }

    return PpI;
}

//======================================================================
const Vector &EightNode_LDBrick_u_p::getForceU ()
{
    static Vector PpU(Num_ElemDof);

    int U_dim[] = {Num_Nodes, Num_Dim};
    tensor Ut(2, U_dim, 0.0);

    int bf_dim[] = {Num_Dim};
    tensor bftensor(1, bf_dim, 0.0);
    bftensor.val(1) = bf(0);
    bftensor.val(2) = bf(1);
    bftensor.val(3) = bf(2);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    stresstensor ST;
    tensor Jacobian;
    tensor dh;
    tensor F;
    double J = 1.0;
    double rho_0 = 0.0;

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
                F = this->theMaterial[where]->getF();
                J = F.determinant();
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian  = Jacobian.determinant();
                rho_0 = (1.0 - nf) * rho_s + (J - 1.0 + nf) * rho_f;
                dh = shapeFunction(r, s, t);
                weight = rw * sw * tw * det_of_Jacobian * rho_0;
                Ut += dh("a") * bftensor("i") * weight;
            }
        }
    }

    PpU.Zero();

    int i, j;

    for (i = 0; i < Num_Nodes; i++)
    {
        for (j = 0; j < Num_Dim; j++)
        {
            PpU(i * Num_Dof + j) = Ut.cval(i + 1, j + 1);
        }
    }

    return PpU;
}

//======================================================================
const Vector &EightNode_LDBrick_u_p::getForceP ()
{
    static Vector PpP(Num_ElemDof);

    int P_dim[] = {Num_Nodes};
    tensor Pt(1, P_dim, 0.0);
    tensor Ppt(1, P_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    int bf_dim[] = {Num_Dim};
    tensor bftensor(1, bf_dim, 0.0);
    bftensor.val(1) = bf(0);
    bftensor.val(2) = bf(1);
    bftensor.val(3) = bf(2);

    tensor Jacobian;
    tensor dhGlobal;
    tensor F;
    tensor Finv;
    tensor KIJ;
    double J = 1.0;

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
                F = this->theMaterial[where]->getF();
                Finv = F.inverse();
                J = F.determinant();
                KIJ = this->LagrangianPerm(Finv, perm, J);
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian  = Jacobian.determinant();
                dhGlobal = this->dh_Global(r, s, t);
                weight = rw * sw * tw * det_of_Jacobian * rho_f;
                Ppt = dhGlobal("gI") * KIJ("IJ") * F("nJ") * bftensor("n");
                Pt += Ppt * (rho_f * weight);
            }
        }
    }

    PpP.Zero();

    int i;

    for (i = 0; i < Num_Nodes; i++)
    {
        PpP(i * Num_Dof + 3) = Pt.cval(i + 1);
    }

    return PpP;
}


//======================================================================
tensor EightNode_LDBrick_u_p::getMatStiffness(void)
{
    tensor tI2("I", 2, def_dim_2);
    int K_dim[] = {Num_Nodes, Num_Dim, Num_Dim, Num_Nodes};
    tensor Kk(4, K_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {Num_Nodes, Num_Dim};
    tensor dh(2, dh_dim, 0.0);
    stresstensor PK2Stress;
    tensor L2;

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor dhGlobal;
    tensor nodesDisp;
    tensor F;
    tensor temp02;
    tensor temp03;
    tensor temp04;
    tensor temp05;
    tensor temp06;

    nodesDisp = this->getNodesDisp( );

    for ( short GP_c_r = 0 ; GP_c_r < Num_IntegrationPts ; GP_c_r++ )
    {
        r = pts[GP_c_r ];
        rw = wts[GP_c_r ];

        for ( short GP_c_s = 0 ; GP_c_s < Num_IntegrationPts ; GP_c_s++ )
        {
            s = pts[GP_c_s ];
            sw = wts[GP_c_s ];

            for ( short GP_c_t = 0 ; GP_c_t < Num_IntegrationPts ; GP_c_t++ )
            {
                t = pts[GP_c_t ];
                tw = wts[GP_c_t ];
                where = (GP_c_r * Num_IntegrationPts + GP_c_s) * Num_IntegrationPts + GP_c_t;
                dh = shapeFunctionDerivative(r, s, t);
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian  = Jacobian.determinant();
                dhGlobal = this->dh_Global(r, s, t);
                weight = rw * sw * tw * det_of_Jacobian;
                PK2Stress = theMaterial[where]->getStressTensor();
                L2 = theMaterial[where]->getTangentTensor();
                F = theMaterial[where]->getF();

                //K01
                temp04 = dhGlobal("Pb") * tI2("mn");
                temp04.null_indices();
                temp02 = PK2Stress("bd") * dhGlobal("Qd");
                temp02.null_indices();
                temp06 = temp04("Pbmn") * temp02("bQ") * weight;
                temp06.null_indices();
                Kk += temp06;

                //K02
                temp03 =  dhGlobal("Pb") * F("ma");
                temp03.null_indices();
                temp04 = F("nc") * L2("abcd");
                temp04.null_indices();
                temp05 = temp04("nabd") * dhGlobal("Qd");
                temp05.null_indices();
                temp06 = temp03("Pbma") * temp05("nabQ") * weight;
                temp06.null_indices();
                Kk += temp06;
            }
        }
    }

    return Kk;
}


//======================================================================
const Matrix &EightNode_LDBrick_u_p::getStiffnessK0 ()
{
    int i, j, m;
    MCK.Zero();

    //K1
    tensor K1 = getStiffnessTensorK1();

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( m = 0; m < Num_Dim; m++ )
        {
            for ( j = 0; j < Num_Nodes; j++)
            {
                MCK(i * Num_Dof + m, j * Num_Dof + 3) = - K1.cval(i + 1, m + 1, j + 1);
            }
        }
    }

    //K2
    tensor K2 = getStiffnessTensorK2();

    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            MCK(i * Num_Dof + 3, j * Num_Dof + 3) = K2.cval(i + 1, j + 1);
        }
    }

    return MCK;
}


//======================================================================
const Matrix &EightNode_LDBrick_u_p::getStiff (int Ki_flag)
{
    int i, j, m, n;

    if (Ki_flag != 0 && Ki_flag != 1)
    {
        cerr << "Error EightNode_LDBrick_u_p::getStiff() - illegal use\n";
        exit(-1);
    }

    if (Ki_flag == 0 && Ki != 0)
    {
        return *Ki;
    }

    this->getStiffnessK0();

    tensor Km = this->getMatStiffness();

    //Material stiffness
    for ( i = 0 ; i < Num_Nodes; i++ )
    {
        for ( j = 0; j < Num_Nodes; j++ )
        {
            for ( m = 0; m < Num_Dim; m++)
            {
                for ( n = 0; n < Num_Dim; n++)
                {
                    MCK(i * Num_Dof + m, j * Num_Dof + n) = Km.cval(i + 1, m + 1, n + 1, j + 1); //(8,3,3,8)
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
        cerr << "Error EightNode_LDBrick_u_p::getStiff() -";
        cerr << "ran out of memory\n";
        exit(-1);
    }

    return *Ki;
}


//======================================================================
tensor EightNode_LDBrick_u_p::getStiffnessTensorK1( )
{
    int K1_dim[] = {Num_Nodes, Num_Dim, Num_Nodes};
    tensor K1(3, K1_dim, 0.0);
    tensor Kk1(3, K1_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    int h_dim[] = {Num_Nodes, Num_Dim};
    tensor h(2, h_dim, 0.0);

    tensor Jacobian;
    tensor dhGlobal;
    tensor F;
    tensor Finv;
    double J = 1.0;

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
                F = theMaterial[where]->getF();
                Finv = F.inverse();
                J = F.determinant();
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian  = Jacobian.determinant();
                weight = rw * sw * tw * det_of_Jacobian * J;
                h = shapeFunction(r, s, t);
                dhGlobal = this->dh_Global(r, s, t);
                Kk1 = dhGlobal("aJ") * Finv("Ji") * h("k");
                K1 += (Kk1 * weight);
            }
        }
    }

    return K1;
}

//======================================================================
tensor EightNode_LDBrick_u_p::getStiffnessTensorK2( )
{
    int K2_dim[] = {Num_Nodes, Num_Nodes};
    tensor K2(2, K2_dim, 0.0);
    tensor Kk2(2, K2_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor KIJ;
    tensor dhGlobal;
    tensor F;
    tensor Finv;
    double J = 1.0;

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
                F = theMaterial[where]->getF();
                Finv = F.inverse();
                J = F.determinant();
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian  = Jacobian.determinant();
                weight = rw * sw * tw * det_of_Jacobian;
                dhGlobal = this->dh_Global(r, s, t);
                KIJ = this->LagrangianPerm(Finv, perm, J);
                Kk2 = dhGlobal("aI") * KIJ("IJ");
                Kk2 = Kk2("aJ") * dhGlobal("kJ");
                K2 += (Kk2 * weight);
            }
        }
    }

    return K2;
}

//======================================================================
tensor EightNode_LDBrick_u_p::getDampingTensorC1( )
{
    int C1_dim[] = {Num_Nodes, Num_Nodes, Num_Dim};
    tensor C1(3, C1_dim, 0.0);
    tensor Cc1(3, C1_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor h;
    tensor dhGlobal;
    tensor F;
    tensor Finv;
    double J = 1.0;

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
                F = this->theMaterial[where]->getF();
                Finv = F.inverse();
                J = F.determinant();
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian  = Jacobian.determinant();
                weight = rw * sw * tw * det_of_Jacobian * J;
                dhGlobal = this->dh_Global(r, s, t);
                h = shapeFunction(r, s, t);
                Cc1 = h("g") * dhGlobal("bJ") * Finv("Jj");
                C1 += (Cc1 * weight);
            }
        }
    }

    return C1;
}

//======================================================================
tensor EightNode_LDBrick_u_p::getDampingTensorC2( )
{
    int C2_dim[] = {Num_Nodes, Num_Nodes};
    tensor C2(2, C2_dim, 0.0);
    tensor Cc2(2, C2_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor h;
    tensor F;
    double J = 1.0;
    double nnff = 0.0;

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
                F = this->theMaterial[where]->getF();
                J = F.determinant();
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian = Jacobian.determinant();
                nnff = (J - 1.0 + nf);
                weight = rw * sw * tw * det_of_Jacobian * (nnff / kf);
                h = shapeFunction(r, s, t);
                Cc2 = h("a") * h("k");
                C2 += (Cc2 * weight);
            }
        }
    }

    return C2;
}

//======================================================================
tensor EightNode_LDBrick_u_p::getMassTensorM1()
{
    int M1_dim[] = {Num_Nodes, Num_Nodes};
    tensor M1(2, M1_dim, 0.0);
    tensor Mm1(2, M1_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    tensor hu;
    tensor Jacobian;
    tensor F;
    double J = 1.0;
    double rho_0 = 0.0;

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
                F = this->theMaterial[where]->getF();
                J = F.determinant();
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian = Jacobian.determinant();
                rho_0 = (1.0 - nf) * rho_s + (J - 1.0 + nf) * rho_f;
                hu = shapeFunction(r, s, t);
                weight = rw * sw * tw * det_of_Jacobian * rho_0;
                Mm1 = hu("m") * hu("n");
                M1 += (Mm1 * weight);
            }
        }
    }

    return M1;
}

//======================================================================
tensor EightNode_LDBrick_u_p::getMassTensorM2()
{
    int M2_dim[] = {Num_Nodes, Num_Nodes, Num_Dim};
    tensor M2(3, M2_dim, 0.0);
    tensor Mm2(3, M2_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;
    int where = 0;
    double weight = 0.0;
    double det_of_Jacobian = 0.0;

    tensor h;
    tensor dhGlobal;
    tensor Jacobian;
    tensor F;
    tensor Finv;
    tensor KIJ;
    double J = 1.0;

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
                F = this->theMaterial[where]->getF();
                Finv = F.inverse();
                J = F.determinant();
                Jacobian = this->Jacobian_3D(r, s, t);
                det_of_Jacobian  = Jacobian.determinant();
                dhGlobal = this->dh_Global(r, s, t);
                h = shapeFunction(r, s, t);
                weight = rw * sw * tw * det_of_Jacobian * rho_f;
                KIJ = this->LagrangianPerm(Finv, perm, J);
                Mm2 = dhGlobal("gI") * KIJ("IJ") * h("b") * F("jJ");
                M2 += Mm2 * weight;
            }
        }
    }

    return M2;
}

//======================================================================
tensor EightNode_LDBrick_u_p::Jacobian_3D(double x, double y, double z)
{
    tensor N_C = this->getNodesCrds();
    tensor dh = this->shapeFunctionDerivative(x, y, z);

    tensor J3D = N_C("ki") * dh("kj");
    J3D.null_indices();
    return J3D;
}

//======================================================================
tensor EightNode_LDBrick_u_p::Jacobian_3Dinv(double x, double y, double z)
{
    tensor Ju = this->Jacobian_3D(x, y, z);
    return Ju.inverse();
}

//======================================================================
tensor EightNode_LDBrick_u_p::dh_Global(double x, double y, double z)
{
    tensor JacobianINV0 = this->Jacobian_3Dinv(x, y, z);
    tensor dh = this->shapeFunctionDerivative(x, y, z);
    tensor  dhGlobal = dh("ik") * JacobianINV0("kj");
    dhGlobal.null_indices();
    return dhGlobal;
}

//======================================================================
tensor EightNode_LDBrick_u_p::getNodesCrds(void)
{
    int i, j;
    int dimXU[] = {Num_Nodes, Num_Dim};
    tensor N_coord(2, dimXU, 0.0);

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
tensor EightNode_LDBrick_u_p::getNodesDisp(void)
{
    // return only the solid displacements at fluid nodes
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
tensor EightNode_LDBrick_u_p::shapeFunction(double r1, double r2, double r3)
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
tensor EightNode_LDBrick_u_p::shapeFunctionDerivative(double r1, double r2, double r3)
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
tensor EightNode_LDBrick_u_p::getGaussPts(void)
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
            }
        }
    }

    return Gs;
}

//==============================================================
tensor EightNode_LDBrick_u_p::LagrangianPerm(const tensor &Finv, const tensor &permea, double Jin)
{
    tensor KIJ;

    tensor temp1 = Finv;
    tensor temp2 = permea;

    KIJ = temp1("Ii") * temp2("ij");
    KIJ = KIJ("Ij") * temp1("Jj");

    return KIJ * Jin;
}


#endif


