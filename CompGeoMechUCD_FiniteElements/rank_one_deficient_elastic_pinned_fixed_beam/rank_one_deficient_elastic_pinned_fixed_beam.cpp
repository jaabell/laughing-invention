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
// PURPOSE:           Beam element to be connected to solid elements (9 dof)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Jose Abell, Boris Jeremic
//
// DATE:              December 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//============================================================================
// NOTE:
// This beam has zero stiffness components for torsion. It should be used
// either as a connection of solid and another beam element or in case
// of being used on its own, the corresponding dof for torsion (dof 7)
// should be fixed!
// Node 1 should have 3 dofs while node 2 should have 6 dofs.
//============================================================================

#include <rank_one_deficient_elastic_pinned_fixed_beam.h>


rank_one_deficient_elastic_pinned_fixed_beam::rank_one_deficient_elastic_pinned_fixed_beam()
    : Element(0, ELE_TAG_rank_one_deficient_elastic_pinned_fixed_beam),
      A(0), E(0), G(0), Jx(0), Iy(0), Iz(0), rho(0.0),
      nodeIOffset(0), nodeJOffset(0), Mass(9, 9), Stiffness(9, 9),
      builtK(0), builtM(0), R(3, 3), P(9), Q(9), q(6),
      connectedExternalNodes(2), L(0),
      nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false),
      outputVector(rank_one_deficient_elastic_pinned_fixed_beam_OUTPUT_SIZE)
{

    // set node pointers to NULL
    for (int i = 0; i < 2; i++)
    {
        theNodes[i] = 0;
    }


    R.Zero();


}


rank_one_deficient_elastic_pinned_fixed_beam::rank_one_deficient_elastic_pinned_fixed_beam(int tag, double a, double e, double g,
        double jx, double iy, double iz, int Nd1, int Nd2,
        double r, int sectTag,
        double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
        double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
        double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z)
    : Element(tag, ELE_TAG_rank_one_deficient_elastic_pinned_fixed_beam),
      A(a), E(e), G(g), Jx(jx), Iy(iy), Iz(iz), rho(r), sectionTag(sectTag), nodeIOffset(0), nodeJOffset(0),
      Mass(9, 9), Stiffness(9, 9), builtK(0), builtM(0), R(3, 3), P(9),
      Q(9), q(6), connectedExternalNodes(2), L(0), nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false),
      outputVector(rank_one_deficient_elastic_pinned_fixed_beam_OUTPUT_SIZE)
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


    double normI = sqrt(rigJntOffset1_x * rigJntOffset1_x + rigJntOffset1_y * rigJntOffset1_y + rigJntOffset1_z * rigJntOffset1_z);

    if (normI > 0)
    {
        nodeIOffset = new Vector(3);
        (*nodeIOffset)(0) = rigJntOffset1_x;
        (*nodeIOffset)(1) = rigJntOffset1_y;
        (*nodeIOffset)(2) = rigJntOffset1_z;
    }


    double normJ = sqrt(rigJntOffset2_x * rigJntOffset2_x + rigJntOffset2_y * rigJntOffset2_y + rigJntOffset2_z * rigJntOffset2_z);

    if (normJ > 0)
    {
        nodeJOffset = new Vector(3);
        (*nodeJOffset)(0) = rigJntOffset2_x;
        (*nodeJOffset)(1) = rigJntOffset2_y;
        (*nodeJOffset)(2) = rigJntOffset2_z;
    }


}


rank_one_deficient_elastic_pinned_fixed_beam::rank_one_deficient_elastic_pinned_fixed_beam(int tag, int Nd1, int Nd2, SectionForceDeformation *section,
        double r,
        double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
        double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
        double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z)
    : Element(tag, ELE_TAG_rank_one_deficient_elastic_pinned_fixed_beam),
      nodeIOffset(0), nodeJOffset(0), Mass(9, 9), Stiffness(9, 9), builtK(0), builtM(0), R(3, 3), P(9),
      Q(9), q(6), connectedExternalNodes(2), L(0),
      nodeIInitialDisp(0), nodeJInitialDisp(0), initialDispChecked(false),
      outputVector(rank_one_deficient_elastic_pinned_fixed_beam_OUTPUT_SIZE)
{
    if (section != 0)
    {
        sectionTag = section->getTag();
        E = 1.0;
        G = 1.0;
        Jx = 0.0;
        rho = r;

        const Matrix &sectTangent = section->getSectionTangent();
        const ID &sectCode = section->getType();

        for (int i = 0; i < sectCode.Size(); i++)
        {
            int code = sectCode(i);

            switch (code)
            {
                case SECTION_RESPONSE_P:
                    A = sectTangent(i, i);
                    break;

                case SECTION_RESPONSE_MZ:
                    Iz = sectTangent(i, i);
                    break;

                case SECTION_RESPONSE_MY:
                    Iy = sectTangent(i, i);
                    break;

                case SECTION_RESPONSE_T:
                    Jx = sectTangent(i, i);
                    break;

                default:
                    break;
            }
        }
    }

    if (Jx == 0.0)
    {
        cerr.flush() << "ERROR: rank_one_deficient_elastic_pinned_fixed_beam::rank_one_deficient_elastic_pinned_fixed_beam -- no torsion in section\n";
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


    double normI = sqrt(rigJntOffset1_x * rigJntOffset1_x + rigJntOffset1_y * rigJntOffset1_y + rigJntOffset1_z * rigJntOffset1_z);

    if (normI > 0)
    {
        nodeIOffset = new Vector(3);
        (*nodeIOffset)(0) = rigJntOffset1_x;
        (*nodeIOffset)(1) = rigJntOffset1_y;
        (*nodeIOffset)(2) = rigJntOffset1_z;
    }


    double normJ = sqrt(rigJntOffset2_x * rigJntOffset2_x + rigJntOffset2_y * rigJntOffset2_y + rigJntOffset2_z * rigJntOffset2_z);

    if (normJ > 0)
    {
        nodeJOffset = new Vector(3);
        (*nodeJOffset)(0) = rigJntOffset2_x;
        (*nodeJOffset)(1) = rigJntOffset2_y;
        (*nodeJOffset)(2) = rigJntOffset2_z;
    }


}

rank_one_deficient_elastic_pinned_fixed_beam::~rank_one_deficient_elastic_pinned_fixed_beam()
{

}

int
rank_one_deficient_elastic_pinned_fixed_beam::getNumExternalNodes(void) const
{
    return 2;
}

const ID &
rank_one_deficient_elastic_pinned_fixed_beam::getExternalNodes(void)
{
    return connectedExternalNodes;
}

Node **
rank_one_deficient_elastic_pinned_fixed_beam::getNodePtrs(void)
{
    return theNodes;
}

int
rank_one_deficient_elastic_pinned_fixed_beam::getNumDOF(void)
{
    return 9;
}

void
rank_one_deficient_elastic_pinned_fixed_beam::setDomain(Domain *theDomain)
{
    if (theDomain == 0)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::setDomain -- Domain is null\n";
        exit(-1);
    }

    theNodes[0] = theDomain->getNode(connectedExternalNodes(0));
    theNodes[1] = theDomain->getNode(connectedExternalNodes(1));


    if (theNodes[0] == 0)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::setDomain -- Node 1: " << connectedExternalNodes(0) << " does not exist\n";
        exit(-1);
    }

    if (theNodes[1] == 0)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::setDomain -- Node 2: " << connectedExternalNodes(1) << " does not exist\n";
        exit(-1);
    }

    int dofNd1 = theNodes[0]->getNumberDOF();
    int dofNd2 = theNodes[1]->getNumberDOF();

    if (dofNd1 != 3)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::setDomain -- Node 1: " << connectedExternalNodes(0)
                     << " has incorrect number of DOF\n";
        exit(-1);
    }

    if (dofNd2 != 6)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::setDomain -- Node 2: " << connectedExternalNodes(1)
                     << " has incorrect number of DOF\n";
        exit(-1);
    }

    this->DomainComponent::setDomain(theDomain);

    if (this->initialize() != 0)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::setDomain -- Error initializing coordinate transformation\n";
        exit(-1);
    }

}

int
rank_one_deficient_elastic_pinned_fixed_beam::commitState()
{
    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::commitState () - failed in base class";
    }

    outputVector = *getForce();

    return retVal;
}

int
rank_one_deficient_elastic_pinned_fixed_beam::revertToLastCommit()
{
    return 0;
}

int
rank_one_deficient_elastic_pinned_fixed_beam::revertToStart()
{
    return 0;
}

int
rank_one_deficient_elastic_pinned_fixed_beam::update(void)
{
    return 0;
}


const Matrix &
rank_one_deficient_elastic_pinned_fixed_beam::getTangentStiff(void)
{

    if (builtK == 0)
    {

        Stiffness.Zero();

        Matrix Stiffness_Local(9, 9);

        Stiffness_Local.Zero();

        Stiffness_Local(0, 0) = E * A / L;
        Stiffness_Local(0, 3) = -E * A / L;
        Stiffness_Local(1, 1) = 3 * E * Iz / pow(L, 3);
        Stiffness_Local(1, 4) = -Stiffness_Local(1, 1);
        Stiffness_Local(1, 8) = 3 * E * Iz / pow(L, 2);
        Stiffness_Local(2, 2) = 3 * E * Iy / pow(L, 3);
        Stiffness_Local(2, 5) = -3 * E * Iy / pow(L, 3);
        Stiffness_Local(2, 7) = -3 * E * Iy / pow(L, 2);
        Stiffness_Local(3, 0) = -E * A / L;
        Stiffness_Local(3, 3) = E * A / L;
        Stiffness_Local(4, 1) = Stiffness_Local(1, 4);
        Stiffness_Local(4, 4) = Stiffness_Local(1, 1);
        Stiffness_Local(4, 8) = -Stiffness_Local(1, 8);
        Stiffness_Local(5, 2) = Stiffness_Local(2, 5);
        Stiffness_Local(5, 5) = -Stiffness_Local(5, 2);
        Stiffness_Local(5, 7) = -Stiffness_Local(2, 7);
        Stiffness_Local(7, 2) = Stiffness_Local(2, 7);
        Stiffness_Local(7, 5) = Stiffness_Local(5, 7);
        Stiffness_Local(7, 7) = 3 * E * Iy / L;
        Stiffness_Local(8, 1) = Stiffness_Local(1, 8);
        Stiffness_Local(8, 4) = Stiffness_Local(4, 8);
        Stiffness_Local(8, 8) = 3 * E * Iz / L;


        Stiffness = this->getGlobalStiffnessMatrix(Stiffness_Local);

        builtK = 1;

    }

    return Stiffness;
}




const Matrix &
rank_one_deficient_elastic_pinned_fixed_beam::getInitialStiff(void)
{

    return this->getTangentStiff();

}



const Matrix &
rank_one_deficient_elastic_pinned_fixed_beam::getMass(void)
{


    if (builtM == 0 )
    {

        Mass.Zero();

        if (rho == 0.0)
        {
            return Mass;
        }

        Matrix Mass_Local(9, 9);

        Mass_Local.Zero();


        // Based on Theory of Matrix Structural Analysis
        // J.S. Przemieniecki, 1985, McGraw-Hill



        Mass_Local(0, 0) =  1.0 / 3.0;
        Mass_Local(0, 3) =  1.0 / 6.0;

        Mass_Local(1, 1) = 33.0 / 140.0 + 6.0 * Iz / (5.0 * A * L * L);
        Mass_Local(1, 4) = 39.0 / 280.0 - 6.0 * Iz / (5.0 * A * L * L);
        Mass_Local(1, 8) = -11.0 * L / 280.0 + Iz / (5.0 * A * L);

        Mass_Local(2, 2) = 33.0 / 140.0 + 6.0 * Iy / (5.0 * A * L * L);
        Mass_Local(2, 5) = 39.0 / 280.0 - 6.0 * Iy / (5.0 * A * L * L);
        Mass_Local(2, 7) = 11.0 * L / 280.0 - Iy / (5.0 * A * L);

        Mass_Local(3, 0) = 1.0 / 6.0;
        Mass_Local(3, 3) = 1.0 / 3.0;

        Mass_Local(4, 1) = 39.0 / 280.0 - 6.0 * Iz / (5.0 * A * L * L);
        Mass_Local(4, 4) = 17.0 / 35.0 + 6.0 * Iz / (5.0 * A * L * L);
        Mass_Local(4, 8) = -3.0 * L / 35.0 - Iz / (5.0 * A * L);

        Mass_Local(5, 2) = 39.0 / 280.0 - 6.0 * Iy / (5.0 * A * L * L);
        Mass_Local(5, 5) = 17.0 / 35.0 + 6.0 * Iy / (5.0 * A * L * L);
        Mass_Local(5, 7) = 3.0 * L / 35.0 + Iy / (5.0 * A * L);

        Mass_Local(6, 6) = Jx / A;

        Mass_Local(7, 2) = 11.0 * L / 280.0 - Iy / (5.0 * A * L);
        Mass_Local(7, 5) = 3.0 * L / 35.0 + Iy / (5.0 * A * L);
        Mass_Local(7, 7) = Iy / (5.0 * A) + 2.0 * L * L / 105.0;

        Mass_Local(8, 1) = -11.0 * L / 280.0 + Iz / (5.0 * A * L);
        Mass_Local(8, 4) = -3.0 * L / 35.0 - Iz / (5.0 * A * L);
        Mass_Local(8, 8) = Iz / (5.0 * A) + 2.0 * L * L / 105.0;


        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
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
rank_one_deficient_elastic_pinned_fixed_beam::zeroLoad(void)
{
    Q.Zero();

    return;
}

int
rank_one_deficient_elastic_pinned_fixed_beam::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    // NOTE: To be completed later

    return 0;
}


int
rank_one_deficient_elastic_pinned_fixed_beam::addInertiaLoadToUnbalance(const Vector &accel)
{
    if (rho == 0.0)
    {
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV(accel);
    const Vector &Raccel2 = theNodes[1]->getRV(accel);


    if (Raccel1.Size() != 3 || Raccel2.Size() != 6)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }


    Vector Raccel(9);
    Raccel(0) = Raccel1(0);
    Raccel(1) = Raccel1(1);
    Raccel(2) = Raccel1(2);
    Raccel(3) = Raccel2(0);
    Raccel(4) = Raccel2(1);
    Raccel(5) = Raccel2(2);
    Raccel(6) = Raccel2(3);
    Raccel(7) = Raccel2(4);
    Raccel(8) = Raccel2(5);


    // Want to add ( - M R * accel ) to unbalance

    Q.addMatrixVector(1.0, this->getMass(), Raccel, -1.0);
    //   Q -= (this->getMass())*Raccel;


    return 0;
}



const Vector &
rank_one_deficient_elastic_pinned_fixed_beam::getResistingForceIncInertia()
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


        Vector accel(9);
        accel(0) = accel1(0);
        accel(1) = accel1(1);
        accel(2) = accel1(2);
        accel(3) = accel2(0);
        accel(4) = accel2(1);
        accel(5) = accel2(2);
        accel(6) = accel2(3);
        accel(7) = accel2(4);
        accel(8) = accel2(5);


        P.addMatrixVector(1.0, this->getMass(), accel, 1.0);
        //   P += (this->getMass())*accel;

        return P;
    }
}


const Vector &
rank_one_deficient_elastic_pinned_fixed_beam::getResistingForce()
{

    P.Zero();

    Vector displacement1 = theNodes[0]->getDisp();
    Vector displacement2 = theNodes[1]->getDisp();


    if (displacement1.Size() != 3 || displacement2.Size() != 6 )
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::getResistingForce matrix and vector sizes are incompatable\n";
        exit(-1);
    }


    Vector displacement(9);
    displacement(0) = displacement1(0);
    displacement(1) = displacement1(1);
    displacement(2) = displacement1(2);
    displacement(3) = displacement2(0);
    displacement(4) = displacement2(1);
    displacement(5) = displacement2(2);
    displacement(6) = displacement2(3);
    displacement(7) = displacement2(4);
    displacement(8) = displacement2(5);


    P.addMatrixVector(1.0, this->getTangentStiff(), displacement, 1.0);

    //   P = (this->getTangentStiff())*displacement;


    // P = P - Q;
    P.addVector(1.0, Q, -1.0);

    return P;
}

int
rank_one_deficient_elastic_pinned_fixed_beam::sendSelf(int cTag, Channel &theChannel)
{
    int res = 0;

    static Vector data(12);

    data(0) = A;
    data(1) = E;
    data(2) = G;
    data(3) = Jx;
    data(4) = Iy;
    data(5) = Iz;
    data(6) = rho;
    data(7) = this->getTag();
    data(8) = connectedExternalNodes(0);
    data(9) = connectedExternalNodes(1);
    //     data(10) = theCoordTransf->getClassTag();

    //  int dbTag = theCoordTransf->getDbTag();
    //
    //  if (dbTag == 0) {
    //      dbTag = theChannel.getDbTag();
    //      if (dbTag != 0)
    //          theCoordTransf->setDbTag(dbTag);
    //  }
    //
    //  data(11) = dbTag;

    // Send the data vector
    res += theChannel.sendVector(this->getDbTag(), cTag, data);

    if (res < 0)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::sendSelf -- could not send data Vector\n";
        return res;
    }

    // Ask the CoordTransf to send itself
    //     res += theCoordTransf->sendSelf(cTag, theChannel);
    //     if (res < 0) {
    //       cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::sendSelf -- could not send CoordTransf\n";
    //       return res;
    //     }

    return res;
}

int
rank_one_deficient_elastic_pinned_fixed_beam::recvSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    int res = 0;

    static Vector data(12);

    res += theChannel.recvVector(this->getDbTag(), cTag, data);

    if (res < 0)
    {
        cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::recvSelf -- could not receive data Vector\n";
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
    connectedExternalNodes(0) = (int)data(8);
    connectedExternalNodes(1) = (int)data(9);

    // Check if the CoordTransf is null; if so, get a new one
    //     int crdTag = (int)data(10);
    //     if (theCoordTransf == 0) {
    //       theCoordTransf = theBroker.getNewCrdTransf3d(crdTag);
    //       if (theCoordTransf == 0) {
    //  cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::recvSelf -- could not get a CrdTransf\n";
    //  exit(-1);
    //       }
    //     }

    // Check that the CoordTransf is of the right type; if not, delete
    // the current one and get a new one of the right type
    //     if (theCoordTransf->getClassTag() != crdTag) {
    //       delete theCoordTransf;
    //       theCoordTransf = theBroker.getNewCrdTransf3d(crdTag);
    //       if (theCoordTransf == 0) {
    //  cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::recvSelf -- could not get a CrdTransf\n";
    //  exit(-1);
    //       }
    //     }

    // Now, receive the CoordTransf
    //     theCoordTransf->setDbTag((int)data(11));
    //     res += theCoordTransf->recvSelf(cTag, theChannel, theBroker);
    //     if (res < 0) {
    //       cerr.flush() << "rank_one_deficient_elastic_pinned_fixed_beam::recvSelf -- could not receive CoordTransf\n";
    //       return res;
    //     }

    // Revert the crdtrasf to its last committed state
    //     theCoordTransf->revertToLastCommit();

    return res;
}

void
rank_one_deficient_elastic_pinned_fixed_beam::Print(ostream &s, int flag)
{
    //Do nothng now!
}




// Response*
// rank_one_deficient_elastic_pinned_fixed_beam::setResponse(const char** argv, int argc, Information& info)
// {
//     // stiffness
//     if (strcmp(argv[0], "stiffness") == 0)
//     {
//         return new ElementResponse(this, 1, Stiffness);
//     }

//     // global forces
//     else if (strcmp(argv[0], "force") == 0 || strcmp(argv[0], "forces") == 0 ||
//              strcmp(argv[0], "globalForce") == 0 || strcmp(argv[0], "globalForces") == 0)
//     {
//         return new ElementResponse(this, 2, P);
//     }


//     else
//     {
//         return 0;
//     }
// }

// int
// rank_one_deficient_elastic_pinned_fixed_beam::getResponse (int responseID, Information& eleInfo)
// {

//     switch (responseID)
//     {
//         case 1: // stiffness
//             return eleInfo.setMatrix(this->getTangentStiff());

//         case 2: // global forces
//             return eleInfo.setVector(this->getResistingForce());

//         default:
//             return -1;
//     }
// }



//====================================================================
// Nima Tafazzoli (Nov. 2012) Moving from transformation ro element

int
rank_one_deficient_elastic_pinned_fixed_beam::initialize()
{
    int error;

    // see if there is some initial displacements at nodes
    if (initialDispChecked == false)
    {
        const Vector &nodeIDisp = theNodes[0]->getDisp();
        const Vector &nodeJDisp = theNodes[1]->getDisp();


        for (int i = 0; i < 3; i++)
        {
            if (nodeIDisp(i) != 0.0)
            {
                nodeIInitialDisp = new Vector(3);

                for (int j = 0; j < 3; j++)
                {
                    (*nodeIInitialDisp)(j) = nodeIDisp(j);
                }

                i = 3;
            }
        }


        for (int j = 0; j < 6; j++)
        {
            if (nodeJDisp(j) != 0.0)
            {
                nodeJInitialDisp = new Vector(6);

                for (int i = 0; i < 6; i++)
                {
                    (*nodeJInitialDisp)(i) = nodeJDisp(i);
                }

                j = 6;
            }
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
rank_one_deficient_elastic_pinned_fixed_beam::computeElemtLengthAndOrient()
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
        cerr.flush() << "\rank_one_deficient_elastic_pinned_fixed_beam::computeElemtLengthAndOrien: 0 length\n";
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
rank_one_deficient_elastic_pinned_fixed_beam::getLocalAxes(Vector &XAxis, Vector &YAxis, Vector &ZAxis)
{
    // Compute y = v cross x
    // Note: v(i) is stored in R[2][i]
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
        cerr.flush() << "\nrank_one_deficient_elastic_pinned_fixed_beam::getLocalAxes";
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
rank_one_deficient_elastic_pinned_fixed_beam::getGlobalStiffnessMatrix(const Matrix &KB)
{
    static Matrix kg(9, 9); // Global stiffness for return
    static Matrix kl(9, 9); // Local stiffness
    static Matrix tmp(9, 9); // Temporary storage


    int i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            kl(i, j) = KB(i, j);
        }
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

    for (m = 0; m < 9; m++)
    {
        tmp(m, 0) = kl(m, 0) * R(0, 0) + kl(m, 1) * R(1, 0)  + kl(m, 2) * R(2, 0);
        tmp(m, 1) = kl(m, 0) * R(0, 1) + kl(m, 1) * R(1, 1)  + kl(m, 2) * R(2, 1);
        tmp(m, 2) = kl(m, 0) * R(0, 2) + kl(m, 1) * R(1, 2)  + kl(m, 2) * R(2, 2);

        tmp(m, 3) = kl(m, 3) * R(0, 0) + kl(m, 4) * R(1, 0)  + kl(m, 5) * R(2, 0);
        tmp(m, 4) = kl(m, 3) * R(0, 1) + kl(m, 4) * R(1, 1)  + kl(m, 5) * R(2, 1);
        tmp(m, 5) = kl(m, 3) * R(0, 2) + kl(m, 4) * R(1, 2)  + kl(m, 5) * R(2, 2);


        tmp(m, 6) = kl(m, 6) * R(0, 0) + kl(m, 7) * R(1, 0)  + kl(m, 8) * R(2, 0);
        tmp(m, 7) = kl(m, 6) * R(0, 1) + kl(m, 7) * R(1, 1)  + kl(m, 8) * R(2, 1);
        tmp(m, 8) = kl(m, 6) * R(0, 2) + kl(m, 7) * R(1, 2)  + kl(m, 8) * R(2, 2);


        if (nodeJOffset)
        {
            tmp(m, 6)  += kl(m, 3) * RWJ(0, 0)  + kl(m, 4) * RWJ(1, 0)  + kl(m, 5) * RWJ(2, 0);
            tmp(m, 7)  += kl(m, 3) * RWJ(0, 1)  + kl(m, 4) * RWJ(1, 1)  + kl(m, 5) * RWJ(2, 1);
            tmp(m, 8)  += kl(m, 3) * RWJ(0, 2)  + kl(m, 4) * RWJ(1, 2)  + kl(m, 5) * RWJ(2, 2);
        }

    }

    // Now compute T'_{lg}*(kl*T_{lg})
    for (m = 0; m < 9; m++)
    {
        kg(0, m) = R(0, 0) * tmp(0, m) + R(1, 0) * tmp(1, m)  + R(2, 0) * tmp(2, m);
        kg(1, m) = R(0, 1) * tmp(0, m) + R(1, 1) * tmp(1, m)  + R(2, 1) * tmp(2, m);
        kg(2, m) = R(0, 2) * tmp(0, m) + R(1, 2) * tmp(1, m)  + R(2, 2) * tmp(2, m);

        kg(3, m) = R(0, 0) * tmp(3, m) + R(1, 0) * tmp(4, m)  + R(2, 0) * tmp(5, m);
        kg(4, m) = R(0, 1) * tmp(3, m) + R(1, 1) * tmp(4, m)  + R(2, 1) * tmp(5, m);
        kg(5, m) = R(0, 2) * tmp(3, m) + R(1, 2) * tmp(4, m)  + R(2, 2) * tmp(5, m);

        kg(6, m) = R(0, 0) * tmp(6, m) + R(1, 0) * tmp(7, m)  + R(2, 0) * tmp(8, m);
        kg(7, m) = R(0, 1) * tmp(6, m) + R(1, 1) * tmp(7, m)  + R(2, 1) * tmp(8, m);
        kg(8, m) = R(0, 2) * tmp(6, m) + R(1, 2) * tmp(7, m)  + R(2, 2) * tmp(8, m);


        if (nodeJOffset)
        {
            kg(6, m)  += RWJ(0, 0) * tmp(3, m)  + RWJ(1, 0) * tmp(4, m) + RWJ(2, 0) * tmp(5, m);
            kg(7, m)  += RWJ(0, 1) * tmp(3, m)  + RWJ(1, 1) * tmp(4, m) + RWJ(2, 1) * tmp(5, m);
            kg(8, m)  += RWJ(0, 2) * tmp(3, m)  + RWJ(1, 2) * tmp(4, m) + RWJ(2, 2) * tmp(5, m);
        }
    }

    return kg;
}



const Matrix &
rank_one_deficient_elastic_pinned_fixed_beam::getGlobalConsistentMassMatrix(const Matrix &KB)
{
    static Matrix mg(9, 9); // Global mass for return
    static Matrix ml(9, 9); // Local mass
    static Matrix tmp(9, 9); // Temporary storage


    int i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            ml(i, j) = KB(i, j);
        }
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

    for (m = 0; m < 9; m++)
    {
        tmp(m, 0) = ml(m, 0) * R(0, 0) + ml(m, 1) * R(1, 0)  + ml(m, 2) * R(2, 0);
        tmp(m, 1) = ml(m, 0) * R(0, 1) + ml(m, 1) * R(1, 1)  + ml(m, 2) * R(2, 1);
        tmp(m, 2) = ml(m, 0) * R(0, 2) + ml(m, 1) * R(1, 2)  + ml(m, 2) * R(2, 2);

        tmp(m, 3) = ml(m, 3) * R(0, 0) + ml(m, 4) * R(1, 0)  + ml(m, 5) * R(2, 0);
        tmp(m, 4) = ml(m, 3) * R(0, 1) + ml(m, 4) * R(1, 1)  + ml(m, 5) * R(2, 1);
        tmp(m, 5) = ml(m, 3) * R(0, 2) + ml(m, 4) * R(1, 2)  + ml(m, 5) * R(2, 2);


        tmp(m, 6) = ml(m, 6) * R(0, 0) + ml(m, 7) * R(1, 0)  + ml(m, 8) * R(2, 0);
        tmp(m, 7) = ml(m, 6) * R(0, 1) + ml(m, 7) * R(1, 1)  + ml(m, 8) * R(2, 1);
        tmp(m, 8) = ml(m, 6) * R(0, 2) + ml(m, 7) * R(1, 2)  + ml(m, 8) * R(2, 2);

        if (nodeJOffset)
        {
            tmp(m, 6)  += ml(m, 3) * RWJ(0, 0)  + ml(m, 4) * RWJ(1, 0)  + ml(m, 5) * RWJ(2, 0);
            tmp(m, 7)  += ml(m, 3) * RWJ(0, 1)  + ml(m, 4) * RWJ(1, 1)  + ml(m, 5) * RWJ(2, 1);
            tmp(m, 8)  += ml(m, 3) * RWJ(0, 2)  + ml(m, 4) * RWJ(1, 2)  + ml(m, 5) * RWJ(2, 2);
        }

    }


    // Now compute T'_{lg}*(ml*T_{lg})
    for (m = 0; m < 9; m++)
    {
        mg(0, m) = R(0, 0) * tmp(0, m) + R(1, 0) * tmp(1, m)  + R(2, 0) * tmp(2, m);
        mg(1, m) = R(0, 1) * tmp(0, m) + R(1, 1) * tmp(1, m)  + R(2, 1) * tmp(2, m);
        mg(2, m) = R(0, 2) * tmp(0, m) + R(1, 2) * tmp(1, m)  + R(2, 2) * tmp(2, m);

        mg(3, m) = R(0, 0) * tmp(3, m) + R(1, 0) * tmp(4, m)  + R(2, 0) * tmp(5, m);
        mg(4, m) = R(0, 1) * tmp(3, m) + R(1, 1) * tmp(4, m)  + R(2, 1) * tmp(5, m);
        mg(5, m) = R(0, 2) * tmp(3, m) + R(1, 2) * tmp(4, m)  + R(2, 2) * tmp(5, m);

        mg(6, m) = R(0, 0) * tmp(6, m) + R(1, 0) * tmp(7, m)  + R(2, 0) * tmp(8, m);
        mg(7, m) = R(0, 1) * tmp(6, m) + R(1, 1) * tmp(7, m)  + R(2, 1) * tmp(8, m);
        mg(8, m) = R(0, 2) * tmp(6, m) + R(1, 2) * tmp(7, m)  + R(2, 2) * tmp(8, m);


        if (nodeJOffset)
        {
            mg(6, m) += RWJ(0, 0) * tmp(3, m)  + RWJ(1, 0) * tmp(4, m) + RWJ(2, 0) * tmp(5, m);
            mg(7, m) += RWJ(0, 1) * tmp(3, m)  + RWJ(1, 1) * tmp(4, m) + RWJ(2, 1) * tmp(5, m);
            mg(8, m) += RWJ(0, 2) * tmp(3, m)  + RWJ(1, 2) * tmp(4, m) + RWJ(2, 2) * tmp(5, m);
        }
    }

    return mg;
}


//==================================================================================
Vector *
rank_one_deficient_elastic_pinned_fixed_beam::getForce(void)
{
    Vector *elementForces = new Vector(12);

    (*elementForces)(0)  = P(0);
    (*elementForces)(1)  = P(1);
    (*elementForces)(2)  = P(2);
    (*elementForces)(3)  = 0.0;
    (*elementForces)(4)  = 0.0;
    (*elementForces)(5)  = 0.0;
    (*elementForces)(6)  = P(3);
    (*elementForces)(7)  = P(4);
    (*elementForces)(8)  = P(5);
    (*elementForces)(9)  = P(6);
    (*elementForces)(10) = P(7);
    (*elementForces)(11) = P(8);


    return elementForces;
}


// Matrix &rank_one_deficient_elastic_pinned_fixed_beam::getGaussCoordinates(void)
// {
//     return gauss_points;
// }

int rank_one_deficient_elastic_pinned_fixed_beam::getOutputSize() const
{
    return rank_one_deficient_elastic_pinned_fixed_beam_OUTPUT_SIZE;
}



const Vector &rank_one_deficient_elastic_pinned_fixed_beam::getOutput() const
{
    return outputVector;
}


