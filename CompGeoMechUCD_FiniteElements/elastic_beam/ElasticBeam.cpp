///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Beam element with consistent mass matrix
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              December 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#include <ElasticBeam.h>
#include <Domain.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <ElementalLoad.h>

#include <ID.h>
#include <math.h>
#include <stdlib.h>
#include <HDF5_Channel.h>


ElasticBeam::ElasticBeam()
    : Element(0, ELE_TAG_ElasticBeam),
      A(0), E(0), G(0), Jx(0), Iy(0), Iz(0), rho(0.0), L(0),
      nodeIOffset(0), nodeJOffset(0), Mass(12, 12), Stiffness(12, 12),
      builtK(0), builtM(0),  R(3, 3), P(12), Q(12),
      connectedExternalNodes(2),
      nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false)
{


    // set node pointers to NULL
    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }

    R.Zero();

}

ElasticBeam::ElasticBeam(int tag, double a, double e, double g,
                         double jx, double iy, double iz, int Nd1, int Nd2,
                         double r, int sectTag,
                         double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                         double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                         double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z)
    : Element(tag, ELE_TAG_ElasticBeam),
      A(a), E(e), G(g), Jx(jx), Iy(iy), Iz(iz), rho(r), L(0), nodeIOffset(0), nodeJOffset(0),
      Mass(12, 12), Stiffness(12, 12), builtK(0), builtM(0), R(3, 3), P(12),
      Q(12),  connectedExternalNodes(2), nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false)
{
    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;

    // set node pointers to NULL
    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }

    R(2, 0) = vecInLocXZPlane_x;
    R(2, 1) = vecInLocXZPlane_y;
    R(2, 2) = vecInLocXZPlane_z;

    nodeIOffset = new Vector(3);
    nodeJOffset = new Vector(3);

    (*nodeIOffset)(0) = rigJntOffset1_x;
    (*nodeIOffset)(1) = rigJntOffset1_y;
    (*nodeIOffset)(2) = rigJntOffset1_z;

    (*nodeJOffset)(0) = rigJntOffset2_x;
    (*nodeJOffset)(1) = rigJntOffset2_y;
    (*nodeJOffset)(2) = rigJntOffset2_z;
}


ElasticBeam::ElasticBeam(int tag, int Nd1, int Nd2, SectionForceDeformation *section,
                         double r,
                         double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                         double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                         double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z)
    : Element(tag, ELE_TAG_ElasticBeam),
      L(0), nodeIOffset(0), nodeJOffset(0), Mass(12, 12), Stiffness(12, 12), builtK(0), builtM(0), R(3, 3),  P(12),
      Q(12), connectedExternalNodes(2),  nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false)
{

    if (Jx == 0.0)
    {
        cerr.flush() << "ElasticBeam::ElasticBeam -- no torsion in section \n";
        exit(-1);
    }

    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;


    // set node pointers to NULL
    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }

    R(2, 0) = vecInLocXZPlane_x;
    R(2, 1) = vecInLocXZPlane_y;
    R(2, 2) = vecInLocXZPlane_z;

    nodeIOffset = new Vector(3);
    nodeJOffset = new Vector(3);
    (*nodeIOffset)(0) = rigJntOffset1_x;
    (*nodeIOffset)(1) = rigJntOffset1_y;
    (*nodeIOffset)(2) = rigJntOffset1_z;
    (*nodeJOffset)(0) = rigJntOffset2_x;
    (*nodeJOffset)(1) = rigJntOffset2_y;
    (*nodeJOffset)(2) = rigJntOffset2_z;
}

ElasticBeam::~ElasticBeam()
{

}

int
ElasticBeam::getNumExternalNodes(void) const
{
    return 2;
}

const ID &
ElasticBeam::getExternalNodes(void)
{
    return connectedExternalNodes;
}

Node **
ElasticBeam::getNodePtrs(void)
{
    return theNodes;
}

int
ElasticBeam::getNumDOF(void)
{
    return 12;
}

void
ElasticBeam::setDomain(Domain *theDomain)
{

    if (theDomain == 0)
    {
        cerr.flush() << "ElasticBeam::setDomain -- Domain is null\n";
        exit(-1);
    }

    theNodes[0] = theDomain->getNode(connectedExternalNodes(0));
    theNodes[1] = theDomain->getNode(connectedExternalNodes(1));


    if (theNodes[0] == 0)
    {
        cerr.flush() << "ElasticBeam::setDomain -- Node 1: " << connectedExternalNodes(0) << " does not exist\n";
        exit(-1);
    }

    if (theNodes[1] == 0)
    {
        cerr.flush() << "ElasticBeam::setDomain -- Node 2: " << connectedExternalNodes(1) << " does not exist\n";
        exit(-1);
    }

    int dofNd1 = theNodes[0]->getNumberDOF();
    int dofNd2 = theNodes[1]->getNumberDOF();

    if (dofNd1 != 6)
    {
        cerr.flush() << "ElasticBeam::setDomain -- Node 1: " << connectedExternalNodes(0)
                     << " has incorrect number of DOF\n";
        exit(-1);
    }

    if (dofNd2 != 6)
    {
        cerr.flush() << "ElasticBeam::setDomain -- Node 2: " << connectedExternalNodes(1)
                     << " has incorrect number of DOF\n";
        exit(-1);
    }

    this->DomainComponent::setDomain(theDomain);

    if (this->initialize() != 0)
    {
        cerr.flush() << "ElasticBeam::setDomain -- Error initializing coordinate transformation\n";
        exit(-1);
    }

}

int
ElasticBeam::commitState()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr.flush() << "ElasticBeam::commitState () - failed in base class";
    }

    return retVal;
}

int
ElasticBeam::revertToLastCommit()
{
    return 0;
}

int
ElasticBeam::revertToStart()
{
    return 0;
}

int
ElasticBeam::update(void)
{
    return 0;
}

const Matrix &
ElasticBeam::getTangentStiff(void)
{

    if (builtK == 0)
    {

        Stiffness.Zero();

        Matrix Stiffness_Local(12, 12);

        Stiffness_Local.Zero();


        Stiffness_Local(0, 0) = E * A / L;
        Stiffness_Local(0, 6) = -E * A / L;
        Stiffness_Local(1, 1) = 12 * E * Iz / pow(L, 3);
        Stiffness_Local(1, 5) = 6 * E * Iz / pow(L, 2);
        Stiffness_Local(1, 7) = -12 * E * Iz / pow(L, 3);
        Stiffness_Local(1, 11) = 6 * E * Iz / pow(L, 2);
        Stiffness_Local(2, 2) = 12 * E * Iy / pow(L, 3);
        Stiffness_Local(2, 4) = -6 * E * Iy / pow(L, 2);
        Stiffness_Local(2, 8) = -12 * E * Iy / pow(L, 3);
        Stiffness_Local(2, 10) = -6 * E * Iy / pow(L, 2);
        Stiffness_Local(3, 3) = G * Jx / L;
        Stiffness_Local(3, 9) = -G * Jx / L;
        Stiffness_Local(4, 2) = -6 * E * Iy / pow(L, 2);
        Stiffness_Local(4, 4) = 4 * E * Iy / L;
        Stiffness_Local(4, 8) = 6 * E * Iy / pow(L, 2);
        Stiffness_Local(4, 10) = 2 * E * Iy / L;
        Stiffness_Local(5, 1) = 6 * E * Iz / pow(L, 2);
        Stiffness_Local(5, 5) = 4 * E * Iz / L;
        Stiffness_Local(5, 7) = -6 * E * Iz / pow(L, 2);
        Stiffness_Local(5, 11) = 2 * E * Iz / L;
        Stiffness_Local(6, 0) = -E * A / L;
        Stiffness_Local(6, 6) = E * A / L;
        Stiffness_Local(7, 1) = -12 * E * Iz / pow(L, 3);
        Stiffness_Local(7, 5) = -6 * E * Iz / pow(L, 2);
        Stiffness_Local(7, 7) = 12 * E * Iz / pow(L, 3);
        Stiffness_Local(7, 11) = -6 * E * Iz / pow(L, 2);
        Stiffness_Local(8, 2) = -12 * E * Iy / pow(L, 3);
        Stiffness_Local(8, 4) = 6 * E * Iy / pow(L, 2);
        Stiffness_Local(8, 8) = 12 * E * Iy / pow(L, 3);
        Stiffness_Local(8, 10) = 6 * E * Iy / pow(L, 2);
        Stiffness_Local(9, 3) = -G * Jx / L;
        Stiffness_Local(9, 9) = G * Jx / L;
        Stiffness_Local(10, 2) = -6 * E * Iy / pow(L, 2);
        Stiffness_Local(10, 4) = 2 * E * Iy / L;
        Stiffness_Local(10, 8) = 6 * E * Iy / pow(L, 2);
        Stiffness_Local(10, 10) = 4 * E * Iy / L;
        Stiffness_Local(11, 1) = 6 * E * Iz / pow(L, 2);
        Stiffness_Local(11, 5) = 2 * E * Iz / L;
        Stiffness_Local(11, 7) = -6 * E * Iz / pow(L, 2);
        Stiffness_Local(11, 11) = 4 * E * Iz / L;

        Stiffness = this->getGlobalStiffnessMatrix(Stiffness_Local);

        builtK = 1;

    }

    return Stiffness;
}


const Matrix &
ElasticBeam::getInitialStiff(void)
{

    return this->getTangentStiff();

}

const Matrix &
ElasticBeam::getMass(void)
{

    if (builtM == 0 )
    {

        Mass.Zero();

        if (rho == 0.0)
        {
            return Mass;
        }

        Matrix Mass_Local(12, 12);

        Mass_Local.Zero();

        // Based on Theory of Matrix Structural Analysis
        // J.S. Przemieniecki, 1985, McGraw-Hill


        Mass_Local(0, 0) = 1.0 / 3.0;
        Mass_Local(0, 6) = 1.0 / 6.0;
        Mass_Local(1, 1) = (13.0 / 35.0) + (6 * Iz) / (5 * A * L * L);
        Mass_Local(1, 5) = (11.0 * L / 210.0) + Iz / (10.0 * A * L);
        Mass_Local(1, 7) = (9.0 / 70.0) - (6 * Iz) / (5 * A * L * L);
        Mass_Local(1, 11) = -(13.0 * L) / 420.0 + Iz / (10 * A * L);
        Mass_Local(2, 2) = (13.0 / 35.0) + (6 * Iy) / (5 * A * L * L);
        Mass_Local(2, 4) = -(11.0 * L / 210.0) - Iy / (10.0 * A * L);
        Mass_Local(2, 8) = (9.0 / 70.0) - (6 * Iy) / (5 * A * L * L);
        Mass_Local(2, 10) = (13.0 * L / 420.0) - Iy / (10 * A * L);
        Mass_Local(3, 3) = Jx / (3 * A);
        Mass_Local(3, 9) = Jx / (6 * A);
        Mass_Local(4, 2) = -(11.0 * L / 210.0) - Iy / (10.0 * A * L);
        Mass_Local(4, 4) = (L * L / 105.0) + (2 * Iy) / (15.0 * A);
        Mass_Local(4, 8) = -(13.0 * L / 420.0) + Iy / (10 * A * L);
        Mass_Local(4, 10) = -(L * L / 140.0) - Iy / (30 * A);
        Mass_Local(5, 1) = (11.0 * L / 210.0) + Iz / (10.0 * A * L);
        Mass_Local(5, 5) = (L * L / 105.0) + (2 * Iz) / (15.0 * A);
        Mass_Local(5, 7) = (13.0 * L / 420.0) - Iz / (10 * A * L);
        Mass_Local(5, 11) = -(L * L / 140.0) - Iz / (30 * A);
        Mass_Local(6, 0) = 1.0 / 6.0;
        Mass_Local(6, 6) = 1.0 / 3.0;
        Mass_Local(7, 1) = (9.0 / 70.0) - (6 * Iz) / (5 * A * L * L);
        Mass_Local(7, 5) = (13.0 * L / 420.0) - Iz / (10 * A * L);
        Mass_Local(7, 7) = (13.0 / 35.0) + (6 * Iz) / (5 * A * L * L);;
        Mass_Local(7, 11) =  -(11.0 * L / 210.0) - Iz / (10.0 * A * L);
        Mass_Local(8, 2) = (9.0 / 70.0) - (6 * Iy) / (5 * A * L * L);
        Mass_Local(8, 4) = -(13.0 * L / 420.0) + Iy / (10 * A * L);
        Mass_Local(8, 8) = (13.0 / 35.0) + (6 * Iy) / (5 * A * L * L);
        Mass_Local(8, 10) = (11.0 * L / 210.0) + Iy / (10.0 * A * L);
        Mass_Local(9, 3) = Jx / (6 * A);
        Mass_Local(9, 9) = Jx / (3 * A);
        Mass_Local(10, 2) = (13.0 * L / 420.0) - Iy / (10 * A * L);
        Mass_Local(10, 4) = -(L * L / 140.0) - Iy / (30 * A);
        Mass_Local(10, 8) = (11.0 * L / 210.0) + Iy / (10.0 * A * L);
        Mass_Local(10, 10) = (L * L / 105.0) + (2 * Iy) / (15.0 * A);
        Mass_Local(11, 1) = -(13.0 * L) / 420.0 + Iz / (10 * A * L);
        Mass_Local(11, 5) = -(L * L / 140.0) - Iz / (30 * A);
        Mass_Local(11, 7) =  -(11.0 * L / 210.0) - Iz / (10.0 * A * L);
        Mass_Local(11, 11) = (L * L / 105.0) + (2 * Iz) / (15.0 * A);



        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                Mass_Local(i, j) = rho * A * L * Mass_Local(i, j);
            }
        }


        Mass = this->getGlobalConsistentMassMatrix(Mass_Local);

        builtM = 1;

    }


    return Mass;


}

void
ElasticBeam::zeroLoad(void)
{
    Q.Zero();
    P.Zero();

    return;
}

int
ElasticBeam::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    // NOTE: To be completed later

    return 0;
}


int
ElasticBeam::addInertiaLoadToUnbalance(const Vector &accel)
{
    if (rho == 0.0)
    {
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV(accel);
    const Vector &Raccel2 = theNodes[1]->getRV(accel);

    if (6 != Raccel1.Size() || 6 != Raccel2.Size())
    {
        cerr.flush() << "ElasticBeam::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }


    static Vector Raccel(12);
    Raccel(0)  = Raccel1(0);
    Raccel(1)  = Raccel1(1);
    Raccel(2)  = Raccel1(2);
    Raccel(3)  = Raccel1(3);
    Raccel(4)  = Raccel1(4);
    Raccel(5)  = Raccel1(5);
    Raccel(6)  = Raccel2(0);
    Raccel(7)  = Raccel2(1);
    Raccel(8)  = Raccel2(2);
    Raccel(9)  = Raccel2(3);
    Raccel(10) = Raccel2(4);
    Raccel(11) = Raccel2(5);


    // Want to add ( - M R * accel ) to unbalance

    Q.addMatrixVector(1.0, this->getMass(), Raccel, -1.0);


    return 0;
}



const Vector &
ElasticBeam::getResistingForceIncInertia()
{

    P.Zero();

    P = this->getResistingForce();

    // add the damping forces if rayleigh damping
    if (a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
    {
        P += this->getRayleighDampingForces();
    }

    if (rho == 0.0)
    {
        return P;
    }
    else
    {
        const Vector &accel1 = theNodes[0]->getTrialAccel();
        const Vector &accel2 = theNodes[1]->getTrialAccel();


        static Vector accel(12);
        accel(0)  = accel1(0);
        accel(1)  = accel1(1);
        accel(2)  = accel1(2);
        accel(3)  = accel1(3);
        accel(4)  = accel1(4);
        accel(5)  = accel1(5);
        accel(6)  = accel2(0);
        accel(7)  = accel2(1);
        accel(8)  = accel2(2);
        accel(9)  = accel2(3);
        accel(10) = accel2(4);
        accel(11) = accel2(5);

        P.addMatrixVector(1.0, this->getMass(), accel, 1.0);

        return P;
    }
}


const Vector &
ElasticBeam::getResistingForce()
{

    P.Zero();

    Vector displacement1 = theNodes[0]->getTrialDisp();
    Vector displacement2 = theNodes[1]->getTrialDisp();


    if (displacement1.Size() != 6 || displacement2.Size() != 6 )
    {
        cerr.flush() << "ElasticBeam::getResistingForce matrix and vector sizes are incompatable\n";
        exit(-1);
    }


    Vector displacement(12);
    displacement(0)  = displacement1(0);
    displacement(1)  = displacement1(1);
    displacement(2)  = displacement1(2);
    displacement(3)  = displacement1(3);
    displacement(4)  = displacement1(4);
    displacement(5)  = displacement1(5);
    displacement(6)  = displacement2(0);
    displacement(7)  = displacement2(1);
    displacement(8)  = displacement2(2);
    displacement(9)  = displacement2(3);
    displacement(10) = displacement2(4);
    displacement(11) = displacement2(5);

    P.addMatrixVector(1.0, Stiffness, displacement, 1.0);
    P.addVector(1.0, Q, -1.0);

    return P;


}

int
ElasticBeam::describeSelf(int cTag, HDF5_Channel &theHDF5Channel)
{
    theHDF5Channel.beginElementDescription("ElasticBeam", this->getTag());
    theHDF5Channel.addField("parameters"                  , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_BASIC);
    theHDF5Channel.addField("connected_nodes"             , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_BASIC);
    theHDF5Channel.addField("node_I_offset"               , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED);
    theHDF5Channel.addField("node_J_offset"               , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED);
    theHDF5Channel.addField("node_I_initial_displacement" , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED);
    theHDF5Channel.addField("node_J_initial_displacement" , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED);
    theHDF5Channel.addField("transformation_matrix"       , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED);
    theHDF5Channel.addField("internal_forces"             , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);
    theHDF5Channel.addField("internal_forces2"            , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DETAILED);
    theHDF5Channel.endElementDescription();

    return 0;
}

int
ElasticBeam::sendSelf(int cTag, Channel &theChannel)
{


    static Vector parameters(8);

    parameters(0) = A;
    parameters(1) = E;
    parameters(2) = G;
    parameters(3) = Jx;
    parameters(4) = Iy;
    parameters(5) = Iz;
    parameters(6) = rho;
    parameters(7) = this->getTag();


    // Send the parameters vector
    if (theChannel.sendVector(this->getDbTag(), cTag, parameters) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send parameters Vector\n";
        return -1;
    }


    // Send the connectivity vector
    if (theChannel.sendID(this->getDbTag(), cTag, connectedExternalNodes) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send connectivity Vector\n";
        return -1;
    }

    //send node_I_offset
    if (theChannel.sendVector(this->getDbTag(), cTag, *nodeIOffset) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send nodeIOffset Vector\n";
        return -1;
    }

    //send node_J_offset
    if (theChannel.sendVector(this->getDbTag(), cTag, *nodeJOffset) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send nodeJOffset Vector\n";
        return -1;
    }

    //send node_I_initial_displacement
    if (theChannel.sendVector(this->getDbTag(), cTag, *nodeIInitialDisp) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send nodeIInitialDisp Vector\n";
        return -1;
    }

    //send node_J_initial_displacement
    if (theChannel.sendVector(this->getDbTag(), cTag, *nodeJInitialDisp) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send nodeJInitialDisp Vector\n";
        return -1;
    }


    // Send the R matrixx
    if (theChannel.sendMatrix(this->getDbTag(), cTag, R) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send R\n";
        return -1;
    }


    // Send the P Vector
    if (theChannel.sendVector(this->getDbTag(), cTag, P) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send P\n";
        return -1;
    }

    // Send the Q Vector
    if (theChannel.sendVector(this->getDbTag(), cTag, Q) < 0)
    {
        cerr.flush() << "ElasticBeam::sendSelf -- could not send Q\n";
        return -1;
    }


    return 0;
}

int
ElasticBeam::recvSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    int res = 0;

    static Vector data(12);

    res += theChannel.recvVector(this->getDbTag(), cTag, data);

    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data Vector\n";
        return res;
    }

    A = data(0);
    E = data(1);
    G = data(2);
    Jx = data(3);
    Iy = data(4);
    Iz = data(5);
    rho = data(6);
    this->setTag((int)data(7));

    res += theChannel.recvID(this->getDbTag(), cTag, connectedExternalNodes);
    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data Vector\n";
        return res;
    }
    res += theChannel.recvVector(this->getDbTag(), cTag, *nodeIOffset);
    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data nodeIOffset\n";
        return res;
    }
    res += theChannel.recvVector(this->getDbTag(), cTag, *nodeJOffset);
    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data nodeJOffset\n";
        return res;
    }
    res += theChannel.recvVector(this->getDbTag(), cTag, *nodeIInitialDisp);
    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data nodeIInitialDisp\n";
        return res;
    }
    res += theChannel.recvVector(this->getDbTag(), cTag, *nodeJInitialDisp);
    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data nodeJInitialDisp\n";
        return res;
    }
    res += theChannel.recvMatrix(this->getDbTag(), cTag, R);
    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data R\n";
        return res;
    }
    res += theChannel.recvVector(this->getDbTag(), cTag, P);
    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data P\n";
        return res;
    }
    res += theChannel.recvVector(this->getDbTag(), cTag, Q);
    if (res < 0)
    {
        cerr.flush() << "ElasticBeam::recvSelf -- could not receive data Q\n";
        return res;
    }


    return res;
}

void
ElasticBeam::Print(ostream &s, int flag)
{
    //Do nothng now!
}



//====================================================================
// Nima Tafazzoli (Nov. 2012) Moving from transformation ro element

int
ElasticBeam::initialize()
{
    int error;

    // see if there is some initial displacements at nodes
    if (initialDispChecked == false)
    {
        const Vector &nodeIDisp = theNodes[0]->getDisp();
        const Vector &nodeJDisp = theNodes[1]->getDisp();

        cout << "done!"  << endl;

        nodeIInitialDisp = new Vector(6);

        for (int j = 0; j < 6; j++)
        {
            (*nodeIInitialDisp)(j) = nodeIDisp(j);
        }

        nodeJInitialDisp = new Vector(6);

        for (int i = 0; i < 6; i++)
        {
            (*nodeJInitialDisp)(i) = nodeJDisp(i);
        }


        initialDispChecked = true;
    }

    // get element length and orientation
    if ((error = this->computeElemtLengthAndOrient()))
    {
        return error;
    }

    static Vector XAxis(3);
    static Vector YAxis(3);
    static Vector ZAxis(3);

    // get 3by3 rotation matrix
    if ((error = this->getLocalAxes(XAxis, YAxis, ZAxis)))
    {
        return error;
    }

    return 0;
}




int
ElasticBeam::computeElemtLengthAndOrient()
{
    // element projection
    static Vector dx(3);

    const Vector &ndICoords = theNodes[0]->getCrds();
    const Vector &ndJCoords = theNodes[1]->getCrds();

    dx(0) = ndJCoords(0) - ndICoords(0);
    dx(1) = ndJCoords(1) - ndICoords(1);
    dx(2) = ndJCoords(2) - ndICoords(2);


    if (nodeJOffset != 0)
    {
        dx(0) += (*nodeJOffset)(0);
        dx(1) += (*nodeJOffset)(1);
        dx(2) += (*nodeJOffset)(2);
    }


    if (nodeIOffset != 0)
    {
        dx(0) -= (*nodeIOffset)(0);
        dx(1) -= (*nodeIOffset)(1);
        dx(2) -= (*nodeIOffset)(2);
    }


    if (nodeIInitialDisp != 0)
    {
        dx(0) -= (*nodeIInitialDisp)(0);
        dx(1) -= (*nodeIInitialDisp)(1);
        dx(2) -= (*nodeIInitialDisp)(2);
    }


    if (nodeJInitialDisp != 0)
    {
        dx(0) += (*nodeJInitialDisp)(0);
        dx(1) += (*nodeJInitialDisp)(1);
        dx(2) += (*nodeJInitialDisp)(2);
    }


    // calculate the element length
    L = dx.Norm();

    if (L == 0.0)
    {
        cerr.flush() << "\ElasticBeam::computeElemtLengthAndOrien: 0 length\n";
        return -2;
    }

    // calculate the element local x axis components (direction cossines)
    // wrt to the global coordinates
    R(0, 0) = dx(0) / L;
    R(0, 1) = dx(1) / L;
    R(0, 2) = dx(2) / L;

    return 0;
}




int
ElasticBeam::getLocalAxes(Vector &XAxis, Vector &YAxis, Vector &ZAxis)
{
    // Compute y = v cross x
    // Note: v(i) is stored in R(2,i]
    static Vector vAxis(3);
    vAxis(0) = R(2, 0);
    vAxis(1) = R(2, 1);
    vAxis(2) = R(2, 2);

    static Vector xAxis(3);
    xAxis(0) = R(0, 0);
    xAxis(1) = R(0, 1);
    xAxis(2) = R(0, 2);
    XAxis(0) = xAxis(0);
    XAxis(1) = xAxis(1);
    XAxis(2) = xAxis(2);

    static Vector yAxis(3);
    yAxis(0) = vAxis(1) * xAxis(2) - vAxis(2) * xAxis(1);
    yAxis(1) = vAxis(2) * xAxis(0) - vAxis(0) * xAxis(2);
    yAxis(2) = vAxis(0) * xAxis(1) - vAxis(1) * xAxis(0);

    double ynorm = yAxis.Norm();

    if (ynorm == 0)
    {
        cerr.flush() << "\ElasticBeam::getLocalAxes";
        cerr.flush() << "\nvector v that defines plane xz is parallel to x axis\n";
        return -3;
    }

    yAxis /= ynorm;

    YAxis(0) = yAxis(0);
    YAxis(1) = yAxis(1);
    YAxis(2) = yAxis(2);

    // Compute z = x cross y
    static Vector zAxis(3);

    zAxis(0) = xAxis(1) * yAxis(2) - xAxis(2) * yAxis(1);
    zAxis(1) = xAxis(2) * yAxis(0) - xAxis(0) * yAxis(2);
    zAxis(2) = xAxis(0) * yAxis(1) - xAxis(1) * yAxis(0);
    ZAxis(0) = zAxis(0);
    ZAxis(1) = zAxis(1);
    ZAxis(2) = zAxis(2);

    // Fill in transformation matrix
    R(1, 0) = yAxis(0);
    R(1, 1) = yAxis(1);
    R(1, 2) = yAxis(2);

    R(2, 0) = zAxis(0);
    R(2, 1) = zAxis(1);
    R(2, 2) = zAxis(2);

    return 0;
}



const Matrix &
ElasticBeam::getGlobalStiffnessMatrix(const Matrix &KB)
{

    static Matrix kg(12, 12);   // Global stiffness for return
    static Matrix kl(12, 12);   // Local stiffness
    static Matrix tmp(12, 12);  // Temporary storage


    int i, j;

    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 12; j++)
        {
            kl(i, j) = KB(i, j);
        }
    }



    static Matrix RWI(3, 3);


    if (nodeIOffset)
    {
        // Compute RWI
        RWI(0, 0) = -R(0, 1) * (*nodeIOffset)(2) + R(0, 2) * (*nodeIOffset)(1);
        RWI(1, 0) = -R(1, 1) * (*nodeIOffset)(2) + R(1, 2) * (*nodeIOffset)(1);
        RWI(2, 0) = -R(2, 1) * (*nodeIOffset)(2) + R(2, 2) * (*nodeIOffset)(1);

        RWI(0, 1) =  R(0, 0) * (*nodeIOffset)(2) - R(0, 2) * (*nodeIOffset)(0);
        RWI(1, 1) =  R(1, 0) * (*nodeIOffset)(2) - R(1, 2) * (*nodeIOffset)(0);
        RWI(2, 1) =  R(2, 0) * (*nodeIOffset)(2) - R(2, 2) * (*nodeIOffset)(0);

        RWI(0, 2) = -R(0, 0) * (*nodeIOffset)(1) + R(0, 1) * (*nodeIOffset)(0);
        RWI(1, 2) = -R(1, 0) * (*nodeIOffset)(1) + R(1, 1) * (*nodeIOffset)(0);
        RWI(2, 2) = -R(2, 0) * (*nodeIOffset)(1) + R(2, 1) * (*nodeIOffset)(0);
    }


    static Matrix RWJ(3, 3);


    if (nodeJOffset)
    {
        // Compute RWJ
        RWJ(0, 0) = -R(0, 1) * (*nodeJOffset)(2) + R(0, 2) * (*nodeJOffset)(1);
        RWJ(1, 0) = -R(1, 1) * (*nodeJOffset)(2) + R(1, 2) * (*nodeJOffset)(1);
        RWJ(2, 0) = -R(2, 1) * (*nodeJOffset)(2) + R(2, 2) * (*nodeJOffset)(1);

        RWJ(0, 1) =  R(0, 0) * (*nodeJOffset)(2) - R(0, 2) * (*nodeJOffset)(0);
        RWJ(1, 1) =  R(1, 0) * (*nodeJOffset)(2) - R(1, 2) * (*nodeJOffset)(0);
        RWJ(2, 1) =  R(2, 0) * (*nodeJOffset)(2) - R(2, 2) * (*nodeJOffset)(0);

        RWJ(0, 2) = -R(0, 0) * (*nodeJOffset)(1) + R(0, 1) * (*nodeJOffset)(0);
        RWJ(1, 2) = -R(1, 0) * (*nodeJOffset)(1) + R(1, 1) * (*nodeJOffset)(0);
        RWJ(2, 2) = -R(2, 0) * (*nodeJOffset)(1) + R(2, 1) * (*nodeJOffset)(0);
    }


    // Transform local stiffness to global system
    // First compute kl*T_{lg}
    int m;

    for (m = 0; m < 12; m++)
    {
        tmp(m, 0) = kl(m, 0) * R(0, 0) + kl(m, 1) * R(1, 0)  + kl(m, 2) * R(2, 0);
        tmp(m, 1) = kl(m, 0) * R(0, 1) + kl(m, 1) * R(1, 1)  + kl(m, 2) * R(2, 1);
        tmp(m, 2) = kl(m, 0) * R(0, 2) + kl(m, 1) * R(1, 2)  + kl(m, 2) * R(2, 2);

        tmp(m, 3) = kl(m, 3) * R(0, 0) + kl(m, 4) * R(1, 0)  + kl(m, 5) * R(2, 0);
        tmp(m, 4) = kl(m, 3) * R(0, 1) + kl(m, 4) * R(1, 1)  + kl(m, 5) * R(2, 1);
        tmp(m, 5) = kl(m, 3) * R(0, 2) + kl(m, 4) * R(1, 2)  + kl(m, 5) * R(2, 2);


        if (nodeIOffset)
        {
            tmp(m, 3)  += kl(m, 0) * RWI(0, 0)  + kl(m, 1) * RWI(1, 0)  + kl(m, 2) * RWI(2, 0);
            tmp(m, 4)  += kl(m, 0) * RWI(0, 1)  + kl(m, 1) * RWI(1, 1)  + kl(m, 2) * RWI(2, 1);
            tmp(m, 5)  += kl(m, 0) * RWI(0, 2)  + kl(m, 1) * RWI(1, 2)  + kl(m, 2) * RWI(2, 2);
        }


        tmp(m, 6) = kl(m, 6) * R(0, 0) + kl(m, 7) * R(1, 0)  + kl(m, 8) * R(2, 0);
        tmp(m, 7) = kl(m, 6) * R(0, 1) + kl(m, 7) * R(1, 1)  + kl(m, 8) * R(2, 1);
        tmp(m, 8) = kl(m, 6) * R(0, 2) + kl(m, 7) * R(1, 2)  + kl(m, 8) * R(2, 2);

        tmp(m, 9)  = kl(m, 9) * R(0, 0) + kl(m, 10) * R(1, 0) + kl(m, 11) * R(2, 0);
        tmp(m, 10) = kl(m, 9) * R(0, 1) + kl(m, 10) * R(1, 1) + kl(m, 11) * R(2, 1);
        tmp(m, 11) = kl(m, 9) * R(0, 2) + kl(m, 10) * R(1, 2) + kl(m, 11) * R(2, 2);


        if (nodeJOffset)
        {
            tmp(m, 9)   += kl(m, 6) * RWJ(0, 0)  + kl(m, 7) * RWJ(1, 0)  + kl(m, 8) * RWJ(2, 0);
            tmp(m, 10)  += kl(m, 6) * RWJ(0, 1)  + kl(m, 7) * RWJ(1, 1)  + kl(m, 8) * RWJ(2, 1);
            tmp(m, 11)  += kl(m, 6) * RWJ(0, 2)  + kl(m, 7) * RWJ(1, 2)  + kl(m, 8) * RWJ(2, 2);
        }


    }

    // Now compute T'_{lg}*(kl*T_{lg})
    for (m = 0; m < 12; m++)
    {
        kg(0, m) = R(0, 0) * tmp(0, m) + R(1, 0) * tmp(1, m)  + R(2, 0) * tmp(2, m);
        kg(1, m) = R(0, 1) * tmp(0, m) + R(1, 1) * tmp(1, m)  + R(2, 1) * tmp(2, m);
        kg(2, m) = R(0, 2) * tmp(0, m) + R(1, 2) * tmp(1, m)  + R(2, 2) * tmp(2, m);

        kg(3, m) = R(0, 0) * tmp(3, m) + R(1, 0) * tmp(4, m)  + R(2, 0) * tmp(5, m);
        kg(4, m) = R(0, 1) * tmp(3, m) + R(1, 1) * tmp(4, m)  + R(2, 1) * tmp(5, m);
        kg(5, m) = R(0, 2) * tmp(3, m) + R(1, 2) * tmp(4, m)  + R(2, 2) * tmp(5, m);


        if (nodeIOffset)
        {
            kg(3, m) += RWI(0, 0) * tmp(0, m)  + RWI(1, 0) * tmp(1, m) + RWI(2, 0) * tmp(2, m);
            kg(4, m) += RWI(0, 1) * tmp(0, m)  + RWI(1, 1) * tmp(1, m) + RWI(2, 1) * tmp(2, m);
            kg(5, m) += RWI(0, 2) * tmp(0, m)  + RWI(1, 2) * tmp(1, m) + RWI(2, 2) * tmp(2, m);
        }


        kg(6, m) = R(0, 0) * tmp(6, m) + R(1, 0) * tmp(7, m)  + R(2, 0) * tmp(8, m);
        kg(7, m) = R(0, 1) * tmp(6, m) + R(1, 1) * tmp(7, m)  + R(2, 1) * tmp(8, m);
        kg(8, m) = R(0, 2) * tmp(6, m) + R(1, 2) * tmp(7, m)  + R(2, 2) * tmp(8, m);

        kg(9, m)  = R(0, 0) * tmp(9, m) + R(1, 0) * tmp(10, m) + R(2, 0) * tmp(11, m);
        kg(10, m) = R(0, 1) * tmp(9, m) + R(1, 1) * tmp(10, m) + R(2, 1) * tmp(11, m);
        kg(11, m) = R(0, 2) * tmp(9, m) + R(1, 2) * tmp(10, m) + R(2, 2) * tmp(11, m);


        if (nodeJOffset)
        {
            kg(9, m)  += RWJ(0, 0) * tmp(6, m)  + RWJ(1, 0) * tmp(7, m) + RWJ(2, 0) * tmp(8, m);
            kg(10, m) += RWJ(0, 1) * tmp(6, m)  + RWJ(1, 1) * tmp(7, m) + RWJ(2, 1) * tmp(8, m);
            kg(11, m) += RWJ(0, 2) * tmp(6, m)  + RWJ(1, 2) * tmp(7, m) + RWJ(2, 2) * tmp(8, m);
        }

    }

    return kg;


}



const Matrix &
ElasticBeam::getGlobalConsistentMassMatrix(const Matrix &KB)
{
    static Matrix mg(12, 12);   // Global mass for return
    static Matrix ml(12, 12);   // Local mass
    static Matrix tmp(12, 12);  // Temporary storage


    int i, j;

    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 12; j++)
        {
            ml(i, j) = KB(i, j);
        }
    }



    static Matrix RWI(3, 3);


    if (nodeIOffset)
    {
        // Compute RWI
        RWI(0, 0) = -R(0, 1) * (*nodeIOffset)(2) + R(0, 2) * (*nodeIOffset)(1);
        RWI(1, 0) = -R(1, 1) * (*nodeIOffset)(2) + R(1, 2) * (*nodeIOffset)(1);
        RWI(2, 0) = -R(2, 1) * (*nodeIOffset)(2) + R(2, 2) * (*nodeIOffset)(1);

        RWI(0, 1) =  R(0, 0) * (*nodeIOffset)(2) - R(0, 2) * (*nodeIOffset)(0);
        RWI(1, 1) =  R(1, 0) * (*nodeIOffset)(2) - R(1, 2) * (*nodeIOffset)(0);
        RWI(2, 1) =  R(2, 0) * (*nodeIOffset)(2) - R(2, 2) * (*nodeIOffset)(0);

        RWI(0, 2) = -R(0, 0) * (*nodeIOffset)(1) + R(0, 1) * (*nodeIOffset)(0);
        RWI(1, 2) = -R(1, 0) * (*nodeIOffset)(1) + R(1, 1) * (*nodeIOffset)(0);
        RWI(2, 2) = -R(2, 0) * (*nodeIOffset)(1) + R(2, 1) * (*nodeIOffset)(0);
    }


    static Matrix RWJ(3, 3);


    if (nodeJOffset)
    {
        // Compute RWJ
        RWJ(0, 0) = -R(0, 1) * (*nodeJOffset)(2) + R(0, 2) * (*nodeJOffset)(1);
        RWJ(1, 0) = -R(1, 1) * (*nodeJOffset)(2) + R(1, 2) * (*nodeJOffset)(1);
        RWJ(2, 0) = -R(2, 1) * (*nodeJOffset)(2) + R(2, 2) * (*nodeJOffset)(1);

        RWJ(0, 1) =  R(0, 0) * (*nodeJOffset)(2) - R(0, 2) * (*nodeJOffset)(0);
        RWJ(1, 1) =  R(1, 0) * (*nodeJOffset)(2) - R(1, 2) * (*nodeJOffset)(0);
        RWJ(2, 1) =  R(2, 0) * (*nodeJOffset)(2) - R(2, 2) * (*nodeJOffset)(0);

        RWJ(0, 2) = -R(0, 0) * (*nodeJOffset)(1) + R(0, 1) * (*nodeJOffset)(0);
        RWJ(1, 2) = -R(1, 0) * (*nodeJOffset)(1) + R(1, 1) * (*nodeJOffset)(0);
        RWJ(2, 2) = -R(2, 0) * (*nodeJOffset)(1) + R(2, 1) * (*nodeJOffset)(0);
    }


    // Transform local stiffness to global system
    // First compute ml*T_{lg}
    int m;

    for (m = 0; m < 12; m++)
    {
        tmp(m, 0) = ml(m, 0) * R(0, 0) + ml(m, 1) * R(1, 0)  + ml(m, 2) * R(2, 0);
        tmp(m, 1) = ml(m, 0) * R(0, 1) + ml(m, 1) * R(1, 1)  + ml(m, 2) * R(2, 1);
        tmp(m, 2) = ml(m, 0) * R(0, 2) + ml(m, 1) * R(1, 2)  + ml(m, 2) * R(2, 2);

        tmp(m, 3) = ml(m, 3) * R(0, 0) + ml(m, 4) * R(1, 0)  + ml(m, 5) * R(2, 0);
        tmp(m, 4) = ml(m, 3) * R(0, 1) + ml(m, 4) * R(1, 1)  + ml(m, 5) * R(2, 1);
        tmp(m, 5) = ml(m, 3) * R(0, 2) + ml(m, 4) * R(1, 2)  + ml(m, 5) * R(2, 2);


        if (nodeIOffset)
        {
            tmp(m, 3)  += ml(m, 0) * RWI(0, 0)  + ml(m, 1) * RWI(1, 0)  + ml(m, 2) * RWI(2, 0);
            tmp(m, 4)  += ml(m, 0) * RWI(0, 1)  + ml(m, 1) * RWI(1, 1)  + ml(m, 2) * RWI(2, 1);
            tmp(m, 5)  += ml(m, 0) * RWI(0, 2)  + ml(m, 1) * RWI(1, 2)  + ml(m, 2) * RWI(2, 2);
        }


        tmp(m, 6) = ml(m, 6) * R(0, 0) + ml(m, 7) * R(1, 0)  + ml(m, 8) * R(2, 0);
        tmp(m, 7) = ml(m, 6) * R(0, 1) + ml(m, 7) * R(1, 1)  + ml(m, 8) * R(2, 1);
        tmp(m, 8) = ml(m, 6) * R(0, 2) + ml(m, 7) * R(1, 2)  + ml(m, 8) * R(2, 2);

        tmp(m, 9)  = ml(m, 9) * R(0, 0) + ml(m, 10) * R(1, 0) + ml(m, 11) * R(2, 0);
        tmp(m, 10) = ml(m, 9) * R(0, 1) + ml(m, 10) * R(1, 1) + ml(m, 11) * R(2, 1);
        tmp(m, 11) = ml(m, 9) * R(0, 2) + ml(m, 10) * R(1, 2) + ml(m, 11) * R(2, 2);


        if (nodeJOffset)
        {
            tmp(m, 9)   += ml(m, 6) * RWJ(0, 0)  + ml(m, 7) * RWJ(1, 0)  + ml(m, 8) * RWJ(2, 0);
            tmp(m, 10)  += ml(m, 6) * RWJ(0, 1)  + ml(m, 7) * RWJ(1, 1)  + ml(m, 8) * RWJ(2, 1);
            tmp(m, 11)  += ml(m, 6) * RWJ(0, 2)  + ml(m, 7) * RWJ(1, 2)  + ml(m, 8) * RWJ(2, 2);
        }


    }

    // Now compute T'_{lg}*(ml*T_{lg})
    for (m = 0; m < 12; m++)
    {
        mg(0, m) = R(0, 0) * tmp(0, m) + R(1, 0) * tmp(1, m)  + R(2, 0) * tmp(2, m);
        mg(1, m) = R(0, 1) * tmp(0, m) + R(1, 1) * tmp(1, m)  + R(2, 1) * tmp(2, m);
        mg(2, m) = R(0, 2) * tmp(0, m) + R(1, 2) * tmp(1, m)  + R(2, 2) * tmp(2, m);

        mg(3, m) = R(0, 0) * tmp(3, m) + R(1, 0) * tmp(4, m)  + R(2, 0) * tmp(5, m);
        mg(4, m) = R(0, 1) * tmp(3, m) + R(1, 1) * tmp(4, m)  + R(2, 1) * tmp(5, m);
        mg(5, m) = R(0, 2) * tmp(3, m) + R(1, 2) * tmp(4, m)  + R(2, 2) * tmp(5, m);


        if (nodeIOffset)
        {
            mg(3, m) += RWI(0, 0) * tmp(0, m)  + RWI(1, 0) * tmp(1, m) + RWI(2, 0) * tmp(2, m);
            mg(4, m) += RWI(0, 1) * tmp(0, m)  + RWI(1, 1) * tmp(1, m) + RWI(2, 1) * tmp(2, m);
            mg(5, m) += RWI(0, 2) * tmp(0, m)  + RWI(1, 2) * tmp(1, m) + RWI(2, 2) * tmp(2, m);
        }


        mg(6, m) = R(0, 0) * tmp(6, m) + R(1, 0) * tmp(7, m)  + R(2, 0) * tmp(8, m);
        mg(7, m) = R(0, 1) * tmp(6, m) + R(1, 1) * tmp(7, m)  + R(2, 1) * tmp(8, m);
        mg(8, m) = R(0, 2) * tmp(6, m) + R(1, 2) * tmp(7, m)  + R(2, 2) * tmp(8, m);

        mg(9, m)  = R(0, 0) * tmp(9, m) + R(1, 0) * tmp(10, m) + R(2, 0) * tmp(11, m);
        mg(10, m) = R(0, 1) * tmp(9, m) + R(1, 1) * tmp(10, m) + R(2, 1) * tmp(11, m);
        mg(11, m) = R(0, 2) * tmp(9, m) + R(1, 2) * tmp(10, m) + R(2, 2) * tmp(11, m);


        if (nodeJOffset)
        {
            mg(9, m)  += RWJ(0, 0) * tmp(6, m)  + RWJ(1, 0) * tmp(7, m) + RWJ(2, 0) * tmp(8, m);
            mg(10, m) += RWJ(0, 1) * tmp(6, m)  + RWJ(1, 1) * tmp(7, m) + RWJ(2, 1) * tmp(8, m);
            mg(11, m) += RWJ(0, 2) * tmp(6, m)  + RWJ(1, 2) * tmp(7, m) + RWJ(2, 2) * tmp(8, m);
        }

    }

    return mg;
}


//==================================================================================
Vector *
ElasticBeam::getForce(void)
{
    Vector *elementForces = new Vector(12);

    (*elementForces)(0)  = P(0);
    (*elementForces)(1)  = P(1);
    (*elementForces)(2)  = P(2);
    (*elementForces)(3)  = P(3);
    (*elementForces)(4)  = P(4);
    (*elementForces)(5)  = P(5);
    (*elementForces)(6)  = P(6);
    (*elementForces)(7)  = P(7);
    (*elementForces)(8)  = P(8);
    (*elementForces)(9)  = P(9);
    (*elementForces)(10) = P(10);
    (*elementForces)(11) = P(11);


    return elementForces;
}


