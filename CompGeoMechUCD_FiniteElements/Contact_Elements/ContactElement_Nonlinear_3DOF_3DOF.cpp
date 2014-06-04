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
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Mario Martinelli, Boris Jeremic
//
// DATE:
// UPDATE HISTORY:    Some changes made by Babak Kamrani On October 2013
//                    Lock option added by Babak Kamrani on 4/12/14
//
///////////////////////////////////////////////////////////////////////////////


#include "ContactElement_Nonlinear_3DOF_3DOF.h"
#include <Information.h>
#include <Domain.h>
#include <Node.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
// #include <ElementResponse.h>
#include <fstream>
using std::ifstream;
using namespace std;
//#ifdef _BABAK_DEBUG
// #include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>

const int ContactElement_Nonlinear_3DOF_3DOF::numberNodes = 2 ;
Matrix  ContactElement_Nonlinear_3DOF_3DOF::stiff(3 * numberNodes, 3 * numberNodes);
// Vector  ContactElement_Nonlinear_3DOF_3DOF::resid(3*numberNodes);
Vector  ContactElement_Nonlinear_3DOF_3DOF::localresid(3 * numberNodes);
Matrix  ContactElement_Nonlinear_3DOF_3DOF::zeroMatrix(3 * numberNodes, 3 * numberNodes);
Vector ContactElement_Nonlinear_3DOF_3DOF::x_local(3); //Added by Babak by 11/22/13
Vector ContactElement_Nonlinear_3DOF_3DOF::y_local(3);
Vector ContactElement_Nonlinear_3DOF_3DOF::z_local(3);
Vector ContactElement_Nonlinear_3DOF_3DOF::x_local_normalized(3);
Vector ContactElement_Nonlinear_3DOF_3DOF::y_local_normalized(3);
Vector ContactElement_Nonlinear_3DOF_3DOF::z_local_normalized(3);
Matrix ContactElement_Nonlinear_3DOF_3DOF::transformation(3, 3);

//*********************************************************************
//  Constructor for defining the contact element

ContactElement_Nonlinear_3DOF_3DOF::ContactElement_Nonlinear_3DOF_3DOF(int tag,
        int Nd1, int Nd2,
        double Knormal_factor,
        double Ktangential,
        double frictionRatio,
        double maximum_gap,
        double x_local_1, double x_local_2, double x_local_3)
    : Element(tag, ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF),
      connectedExternalNodes(numberNodes),
      shearforce_n(2), shearforce_np1(2), shearforce_trial_np1(2), shearforce_increment_np1(2),
      n_trial_np1(2), global_gap_np1(3), local_gap_np1(3),
      total_shear_relative_displ_n(2), total_shear_relative_displ_np1(2), incr_shear_relative_displ_np1(2),
      plastic_shear_relative_displ_n(2), plastic_shear_relative_displ_np1(2), incr_plastic_shear_relative_displ_np1(2),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes), resid(3 * numberNodes),
      outputVector(ContactElement_Nonlinear_3DOF_3DOF_OUTPUT_SIZE)
{

    is_locked = 0;

    /// ensure the connectedExternalNode ID is of correct size & set values
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ZeroLength::setUp - failed to create an ID of correct size\n";
    }

    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;

    /// assign Kn_factor, Kt, fs, Gap_max
    Kn_factor = Knormal_factor ;          // [kN] [Force = Kn_factor*delta_un/(Gap_max - delta_un)]
    fs        = frictionRatio;                   // [-]
    Gap_max   = maximum_gap;

    Kn        = Kn_factor;    // Used to be    Kn_factor / Gap_max;
    Kt        = Ktangential;
    Kn_locked = Kn_factor;
    Kt_locked = Ktangential;

    // initialized contact flag and sliding flag to be zero
    ContactFlag = 0;
    SlidingFlag = 0;

    // vector components in global coordinates defining local x-axis of the element
    //   Vector x_local(3);   //Out by Babak and moved to the prototype 11/26/13
    x_local(0) = x_local_1;
    x_local(1) = x_local_2;
    x_local(2) = x_local_3;




    normalforce_n = 0.0;
    normalforce_np1 = 0.0;
    normalforce_increment_np1 = 0.0;
    total_normal_relative_displ_n = 0.0;
    total_normal_relative_displ_np1 = 0.0;
    incr_normal_relative_displ_np1 = 0.0;
    yield_criteria = 0.0;


    shearforce_n.Zero();
    shearforce_np1.Zero();
    shearforce_trial_np1.Zero();
    shearforce_increment_np1.Zero();
    total_shear_relative_displ_n.Zero();
    total_shear_relative_displ_np1.Zero();
    incr_shear_relative_displ_np1.Zero();
    plastic_shear_relative_displ_n.Zero();
    plastic_shear_relative_displ_np1.Zero();
    incr_plastic_shear_relative_displ_np1.Zero();
    n_trial_np1.Zero();
    global_gap_np1.Zero();
    local_gap_np1.Zero();

    // defining the contact plane based on the local x axis
    ContactPlane(x_local);


}


//*********************************************************************
//  Constructor for defining the contact element with lock option:

ContactElement_Nonlinear_3DOF_3DOF::ContactElement_Nonlinear_3DOF_3DOF(int tag,
        int Nd1, int Nd2,
        double Knormal_factor,
        double Ktangential,
        double frictionRatio,
        double maximum_gap,
        double x_local_1, double x_local_2, double x_local_3,
        int lock_flag)
    : Element(tag, ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF),
      connectedExternalNodes(numberNodes),
      shearforce_n(2), shearforce_np1(2), shearforce_trial_np1(2), shearforce_increment_np1(2),
      n_trial_np1(2), global_gap_np1(3), local_gap_np1(3),
      total_shear_relative_displ_n(2), total_shear_relative_displ_np1(2), incr_shear_relative_displ_np1(2),
      plastic_shear_relative_displ_n(2), plastic_shear_relative_displ_np1(2), incr_plastic_shear_relative_displ_np1(2),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes), resid(3 * numberNodes),
      outputVector(ContactElement_Nonlinear_3DOF_3DOF_OUTPUT_SIZE)
{

    is_locked = lock_flag;

    /// ensure the connectedExternalNode ID is of correct size & set values
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ZeroLength::setUp - failed to create an ID of correct size\n";
    }
    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;

    /// assign Kn_factor, Kt, fs, Gap_max
    Kn_factor = Knormal_factor ;          // [kN] [Force =  Kn_factor*delta_un/(Gap_max - delta_un)]
    fs = frictionRatio;                   // [-]
    Gap_max = maximum_gap;

    Kn        = Kn_factor;    // Used to be    Kn_factor / Gap_max;
    Kt        = Ktangential;
    Kn_locked = Kn_factor;
    Kt_locked = Ktangential;



    // initialized contact flag and sliding flag to be zero
    ContactFlag = 0;
    SlidingFlag = 0;

    // vector components in global coordinates defining local x-axis of the element
    //   Vector x_local(3);   //Out by Babak and moved to the prototype 11/26/13
    x_local(0) = x_local_1;
    x_local(1) = x_local_2;
    x_local(2) = x_local_3;

    normalforce_n = 0.0;
    normalforce_np1 = 0.0;
    normalforce_increment_np1 = 0.0;
    yield_criteria = 0.0;
    total_normal_relative_displ_n = 0.0;
    total_normal_relative_displ_np1 = 0.0;
    incr_normal_relative_displ_np1 = 0.0;


    shearforce_n.Zero();
    shearforce_np1.Zero();
    shearforce_trial_np1.Zero();
    shearforce_increment_np1.Zero();
    total_shear_relative_displ_n.Zero();
    total_shear_relative_displ_np1.Zero();
    incr_shear_relative_displ_np1.Zero();
    plastic_shear_relative_displ_n.Zero();
    plastic_shear_relative_displ_np1.Zero();
    incr_plastic_shear_relative_displ_np1.Zero();
    n_trial_np1.Zero();

    global_gap_np1.Zero();
    local_gap_np1.Zero();


    // defining the contact plane based on the local x axis
    ContactPlane(x_local);


}
//null constructor
ContactElement_Nonlinear_3DOF_3DOF::ContactElement_Nonlinear_3DOF_3DOF(void)
    : Element(0, ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF),
      connectedExternalNodes(numberNodes),
      shearforce_n(2), shearforce_np1(2), shearforce_trial_np1(2), shearforce_increment_np1(2),
      n_trial_np1(2), global_gap_np1(3), local_gap_np1(3),
      total_shear_relative_displ_n(2), total_shear_relative_displ_np1(2), incr_shear_relative_displ_np1(2),
      plastic_shear_relative_displ_n(2), plastic_shear_relative_displ_np1(2), incr_plastic_shear_relative_displ_np1(2),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes), resid(3 * numberNodes),
      outputVector(ContactElement_Nonlinear_3DOF_3DOF_OUTPUT_SIZE)
{

    // ensure the connectedExternalNode ID is of correct size
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ContactElement_Nonlinear_3DOF_3DOF::ContactElement_Nonlinear_3DOF_3DOF - failed to create an ID of correct size\n";
    }

    for (int j = 0; j < numberNodes; j++ )
    {
        nodePointers[j] = 0;
    }



    shearforce_n(0) = 0.0;
    shearforce_n(1) = 0.0;
    //Added by Babak by 11/22/13:
    //------
    normalforce_n = 0.0;
    normalforce_np1 = 0.0;
    normalforce_increment_np1 = 0.0;
    yield_criteria = 0.0;
    total_normal_relative_displ_n = 0.0;
    total_normal_relative_displ_np1 = 0.0;
    incr_normal_relative_displ_np1 = 0.0;

    shearforce_n.Zero();
    shearforce_np1.Zero();
    shearforce_trial_np1.Zero();
    shearforce_increment_np1.Zero();
    total_shear_relative_displ_n.Zero();
    total_shear_relative_displ_np1.Zero();
    incr_shear_relative_displ_np1.Zero();
    plastic_shear_relative_displ_n.Zero();
    plastic_shear_relative_displ_np1.Zero();
    incr_plastic_shear_relative_displ_np1.Zero();
    n_trial_np1.Zero();

    global_gap_np1.Zero();
    local_gap_np1.Zero();


}


//  Destructor
ContactElement_Nonlinear_3DOF_3DOF::~ContactElement_Nonlinear_3DOF_3DOF()
{

}

// returning the number of the nodes for element
int
ContactElement_Nonlinear_3DOF_3DOF::getNumExternalNodes(void) const
{
    return 2;
}

// returning the nodes of the elements
const ID &
ContactElement_Nonlinear_3DOF_3DOF::getExternalNodes(void)
{
    return connectedExternalNodes;
}

// returning a pointer for nodes of the element
Node **
ContactElement_Nonlinear_3DOF_3DOF::getNodePtrs(void)
{
    return nodePointers;
}

// returning the number of DOFs of the element
int
ContactElement_Nonlinear_3DOF_3DOF::getNumDOF(void)
{
    return numDOF;
}


void
ContactElement_Nonlinear_3DOF_3DOF::setDomain(Domain *theDomain)
{

    // check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        nodePointers[0] = 0;
        nodePointers[1] = 0;
        exit(-1);
    }


    // first set the node pointers
    int Nd1 = connectedExternalNodes(0);
    int Nd2 = connectedExternalNodes(1);
    nodePointers[0] = theDomain->getNode(Nd1);
    nodePointers[1] = theDomain->getNode(Nd2);



    // if can't find both of the node pointers then send a warning message
    if ( nodePointers[0] == 0 || nodePointers[1] == 0 )
    {
        if (nodePointers[0] == 0)
        {
            cerr << "WARNING ContactElement_Nonlinear_3DOF_3DOF::setDomain() - Nd1: " << Nd1 << " does not exist in ";
        }
        else
        {
            cerr << "WARNING ContactElement_Nonlinear_3DOF_3DOF::setDomain() - Nd2: " << Nd2 << " does not exist in ";
        }

        return;
    }


    // now determine the number of dof and the dimension
    int dofNd1 = nodePointers[0]->getNumberDOF();
    int dofNd2 = nodePointers[1]->getNumberDOF();


    // if differing dof at the ends - print a warning message
    if (dofNd1 != dofNd2)
    {
        cerr << "WARNING ContactElement_Nonlinear_3DOF_3DOF::setDomain(): nodes " << Nd1 << " and " << Nd2 <<
             "have differing dof at ends for ContactElement_Nonlinear_3DOF_3DOF " << this->getTag() << endln;
        return;
    }



    // call the base class method
    this->DomainComponent::setDomain(theDomain);

    if (dofNd1 == 3 && dofNd2 == 3)
    {
        numDOF = 6;
    }
    else
    {
        cerr << "WARNING ContactElement_Nonlinear_3DOF_3DOF::setDomain cannot handle " << dofNd1 <<
             "dofs at nodes in " << dofNd1 << " 3d problem\n";
        return;
    }
}


int
ContactElement_Nonlinear_3DOF_3DOF::commitState()
{
    // cout << "commitState()\n\n";
    // update total and plastic diplacements
    plastic_shear_relative_displ_n = plastic_shear_relative_displ_np1;
    total_shear_relative_displ_n = total_shear_relative_displ_np1;
    total_normal_relative_displ_n = total_normal_relative_displ_np1;

    // update forces
    normalforce_n = normalforce_np1;
    shearforce_n = shearforce_np1;

    //Set next diplacements = 0
    shearforce_np1.Zero();
    plastic_shear_relative_displ_np1.Zero();
    total_shear_relative_displ_np1.Zero();
    normalforce_np1 = 0.0;
    total_normal_relative_displ_np1 = 0.0;


    // Output

    outputVector(0) = total_normal_relative_displ_n;
    outputVector(1) = total_shear_relative_displ_n(0);
    outputVector(2) = total_shear_relative_displ_n(1);
    outputVector(3) = normalforce_n;
    outputVector(4) = shearforce_n(0);
    outputVector(5) = shearforce_n(1);
    outputVector(6) = SlidingFlag;




    return 0;
}


int
ContactElement_Nonlinear_3DOF_3DOF::revertToLastCommit()
{
    return 0;
}


int
ContactElement_Nonlinear_3DOF_3DOF::revertToStart()
{
    return 0;
}


const Matrix &
ContactElement_Nonlinear_3DOF_3DOF::getDamp(void)
{
    zeroMatrix.Zero();
    return zeroMatrix;
}


const Matrix &
ContactElement_Nonlinear_3DOF_3DOF::getMass(void)
{
    zeroMatrix.Zero();
    return zeroMatrix;
}


void
ContactElement_Nonlinear_3DOF_3DOF::zeroLoad(void)
{
}

int
ContactElement_Nonlinear_3DOF_3DOF::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    return 0;
}

int
ContactElement_Nonlinear_3DOF_3DOF::addInertiaLoadToUnbalance(const Vector &accel)
{
    return 0;
}




int ContactElement_Nonlinear_3DOF_3DOF::nodes_are_in_contact(void)
{
    // cout << "if_nodes_are_in_contact_or_not\n\n";
    double gap;


    Vector slaveNode_location(3);
    Vector masterNode_location(3);


    slaveNode_location  = nodePointers[0]->getTrialDisp();
    masterNode_location = nodePointers[1]->getTrialDisp();


    global_gap_np1 = masterNode_location - slaveNode_location;



    local_gap_np1 = transformation * global_gap_np1;

    gap = local_gap_np1(0);


    if (gap <= 0)
    {
        return 1;
    }
    else
    {

        return 0;
    }


    cerr << "ContactElement_Nonlinear_3DOF_3DOF::if_nodes_are_in_contact_or_not: If it reaches here something is wrong! " << endl;
    exit(-1);
}



const Matrix &
ContactElement_Nonlinear_3DOF_3DOF::getTangentStiff(void)
{
    int i , j;
    stiff.Zero( ) ;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            stiff(i, j) = Kn * (N(i) * N(j)) + Kt * (T1(i) * T1(j) + T2(i) * T2(j));
        }
    }
    // cout << stiff << endl;
    return stiff ;
}




const Matrix &
ContactElement_Nonlinear_3DOF_3DOF::getInitialStiff(void)
{
    update();
    getTangentStiff();

    return stiff ;
}



const Vector &
ContactElement_Nonlinear_3DOF_3DOF::getResistingForce()
{
    int i;
    resid.Zero( ) ;

    for (i = 0; i < 6; i++)
    {
        resid(i) = (-normalforce_np1) * N(i) + shearforce_np1(0) * T1(i) + shearforce_np1(1) * T2(i);
    }
    // cout << "normalforce_np1 = " << normalforce_np1 << endl;
    // cout << resid << endl;

    return resid ;
}





const Vector &
ContactElement_Nonlinear_3DOF_3DOF::getResistingForceIncInertia()
{
    return this->getResistingForce();
}



void
ContactElement_Nonlinear_3DOF_3DOF::Print(ostream &s, int flag)
{
    if (flag == 0)   // print everything
    {
        s << "Element: " << this->getTag();
        s << " type: ContactElement_Nonlinear_3DOF_3DOF  iNode: " << connectedExternalNodes(0);
        s << " jNode: " << connectedExternalNodes(1) << endln;
    }
    else if (flag == 1)
    {
        s << this->getTag() << endln;
    }
}








int
ContactElement_Nonlinear_3DOF_3DOF::ContactPlane(const Vector &x_local)
{

    if ((x_local(0) == 0.0) && (x_local(2) == 0.0))
    {
        y_local(0) = -1.0;
        y_local(1) =  0.0;
        y_local(2) =  0.0;

        z_local(0) = x_local(1) * y_local(2) - x_local(2) * y_local(1);
        z_local(1) = x_local(2) * y_local(0) - x_local(0) * y_local(2);
        z_local(2) = x_local(0) * y_local(1) - x_local(1) * y_local(0);
    }
    else
    {
        // creating a temporary vector (global y axis)
        Vector temp_vector(3);
        temp_vector(0) =  0.0;
        temp_vector(1) =  1.0;
        temp_vector(2) =  0.0;

        // creating the z_local axis from cross product of x_local and temp_vector
        // which will be perpendicular to x_local
        z_local(0) = x_local(1) * temp_vector(2) - x_local(2) * temp_vector(1);
        z_local(1) = x_local(2) * temp_vector(0) - x_local(0) * temp_vector(2);
        z_local(2) = x_local(0) * temp_vector(1) - x_local(1) * temp_vector(0);

        // creating the y_local axis from cross product of x_local and z_local
        // which will be perpendicular to x_local and z_local
        y_local(0) = z_local(1) * x_local(2) - z_local(2) * x_local(1);
        y_local(1) = z_local(2) * x_local(0) - z_local(0) * x_local(2);
        y_local(2) = z_local(0) * x_local(1) - z_local(1) * x_local(0);
    }

    // compute length (norm) of vectors
    double x_local_norm = sqrt(x_local(0) * x_local(0) + x_local(1) * x_local(1) + x_local(2) * x_local(2));
    double y_local_norm = sqrt(y_local(0) * y_local(0) + y_local(1) * y_local(1) + y_local(2) * y_local(2));
    double z_local_norm = sqrt(z_local(0) * z_local(0) + z_local(1) * z_local(1) + z_local(2) * z_local(2));

    // find the normalized local vectors
    for (int i = 0; i < 3; i++)
    {
        x_local_normalized(i) = x_local(i) / x_local_norm;
        y_local_normalized(i) = y_local(i) / y_local_norm;
        z_local_normalized(i) = z_local(i) / z_local_norm;
    }

    N(0)   =   x_local_normalized(0) ;
    N(1)   =   x_local_normalized(1) ;
    N(2)   =   x_local_normalized(2) ;
    N(3)   =  -x_local_normalized(0) ;
    N(4)   =  -x_local_normalized(1) ;
    N(5)   =  -x_local_normalized(2) ;



    T1(0)  =   y_local_normalized(0) ;
    T1(1)  =   y_local_normalized(1) ;
    T1(2)  =   y_local_normalized(2) ;
    T1(3)  =  -y_local_normalized(0) ;
    T1(4)  =  -y_local_normalized(1) ;
    T1(5)  =  -y_local_normalized(2) ;



    T2(0)  =   z_local_normalized(0) ;
    T2(1)  =   z_local_normalized(1) ;
    T2(2)  =   z_local_normalized(2) ;
    T2(3)  =  -z_local_normalized(0) ;
    T2(4)  =  -z_local_normalized(1) ;
    T2(5)  =  -z_local_normalized(2) ;


    // create transformation matrix of the element
    for (int i = 0; i < 3; i++)
    {
        transformation(0, i) = x_local_normalized(i);
        transformation(1, i) = y_local_normalized(i);
        transformation(2, i) = z_local_normalized(i);
    }

    return 0;
}


//=============================================================================
//implemented by Babak Kamrani on 11/22/2013
int
ContactElement_Nonlinear_3DOF_3DOF::sendSelf(int commitTag, Channel &theChannel)
{
    int res = 0;
    int dataTag = this->getDbTag();

    static ID idData(6);
    idData(0) = this->getTag();

    idData(1) = connectedExternalNodes(0);
    idData(2) = connectedExternalNodes(1);
    idData(3) = ContactFlag;
    idData(4) = SlidingFlag;
    idData(5) = 0;

    res = theChannel.sendID(dataTag, commitTag, idData);

    if (res < 0)
    {
        std::cerr << "WARNING ContactElement_Nonlinear_3DOF_3DOF::sendSelf() - " << this->getTag() << " failed to send idData\n";
        return res;
    }

    static Vector VectorData(44);

    VectorData(0)  = Kn_factor;
    VectorData(1)  = Gap_max;
    VectorData(2)  = fs;
    VectorData(3)  = Kn;
    VectorData(4)  = Kt;

    VectorData(5)  = x_local(0);
    VectorData(6)  = x_local(1);
    VectorData(7)  = x_local(2);

    VectorData(8)  = y_local(0);
    VectorData(9)  = y_local(1);
    VectorData(10) = y_local(2);

    VectorData(11) = z_local(0);
    VectorData(12) = z_local(1);
    VectorData(13) = z_local(2);

    VectorData(14) = normalforce_n;
    VectorData(15) = normalforce_np1;
    VectorData(16) = normalforce_increment_np1;

    VectorData(17) = shearforce_n(0);
    VectorData(18) = shearforce_n(1);
    VectorData(19) = shearforce_np1(0);
    VectorData(20) = shearforce_np1(1);

    VectorData(21) = n_trial_np1(0);
    VectorData(22) = n_trial_np1(1);

    VectorData(23) = global_gap_np1(0);
    VectorData(24) = global_gap_np1(1);
    VectorData(25) = global_gap_np1(2);

    VectorData(26) = local_gap_np1(0);
    VectorData(27) = local_gap_np1(1);
    VectorData(28) = local_gap_np1(2);

    VectorData(29) = total_normal_relative_displ_n;
    VectorData(30) = total_normal_relative_displ_np1;
    VectorData(31) = incr_normal_relative_displ_np1;

    VectorData(32) = total_shear_relative_displ_n(0);
    VectorData(33) = total_shear_relative_displ_n(1);

    VectorData(34) = total_shear_relative_displ_np1(0);
    VectorData(35) = total_shear_relative_displ_np1(1);

    VectorData(36) = incr_shear_relative_displ_np1(0);
    VectorData(37) = incr_shear_relative_displ_np1(1);

    VectorData(38) = plastic_shear_relative_displ_n(0);
    VectorData(39) = plastic_shear_relative_displ_n(1);

    VectorData(40) = plastic_shear_relative_displ_np1(0);
    VectorData(41) = plastic_shear_relative_displ_np1(1);

    VectorData(42) = incr_plastic_shear_relative_displ_np1(0);
    VectorData(43) = incr_plastic_shear_relative_displ_np1(1);

    res = theChannel.sendVector(dataTag, commitTag, VectorData);

    if (res < 0)
    {
        cerr << "WARNING ContactElement_Nonlinear_3DOF_3DOF::sendSelf() - " << this->getTag() << " failed to send VectorData\n";
        return res;
    }

    return res;

}

int
ContactElement_Nonlinear_3DOF_3DOF::recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    int res = 0;

    int dataTag = this->getDbTag();

    static ID idData(6);
    res = theChannel.recvID(dataTag, commitTag, idData);

    if (res < 0)
    {
        std::cerr << "WARNING ContactElement_Nonlinear_3DOF_3DOF::recvSelf() - " << dataTag << " failed to receive idData\n";
        return res;
    }

    this->setTag(idData(0));
    connectedExternalNodes(0) = idData(1);
    connectedExternalNodes(1) = idData(2);
    ContactFlag = idData(3);
    SlidingFlag = idData(4);
    // definecontact = idData(5);

    static Vector VectorData(44);
    res = theChannel.recvVector(this->getDbTag(), commitTag, VectorData);

    if (res < 0)
    {
        std::cerr << "WARNING ContactElement_Nonlinear_3DOF_3DOF::recvSelf() - " << dataTag << " failed to receive VectorData\n";
        return res;
    }

    Kn_factor = VectorData(0);
    Gap_max   = VectorData(1);
    fs        = VectorData(2);
    Kn        = VectorData(3);
    Kt        = VectorData(4);

    x_local(0) = VectorData(5);
    x_local(1) = VectorData(6);
    x_local(2) = VectorData(7);

    y_local(0) = VectorData(8);
    y_local(1) = VectorData(9);
    y_local(2) = VectorData(10);

    z_local(0) = VectorData(11);
    z_local(1) = VectorData(12);
    z_local(2) = VectorData(13);

    normalforce_n             = VectorData(14);
    normalforce_np1           = VectorData(15);
    normalforce_increment_np1 = VectorData(16);

    shearforce_n(0) = VectorData(17);
    shearforce_n(1) = VectorData(18);
    shearforce_np1(0) = VectorData(19);
    shearforce_np1(1) = VectorData(20);

    n_trial_np1(0) = VectorData(21);
    n_trial_np1(1) = VectorData(22);

    global_gap_np1(0) = VectorData(23);
    global_gap_np1(1) = VectorData(24);
    global_gap_np1(2) = VectorData(25);

    local_gap_np1(0) = VectorData(26);
    local_gap_np1(1) = VectorData(27);
    local_gap_np1(2) = VectorData(28);

    total_normal_relative_displ_n   = VectorData(29);
    total_normal_relative_displ_np1 = VectorData(30);
    incr_normal_relative_displ_np1  = VectorData(31);

    total_shear_relative_displ_n(0) = VectorData(32);
    total_shear_relative_displ_n(1) = VectorData(33);

    total_shear_relative_displ_np1(0) = VectorData(34);
    total_shear_relative_displ_np1(1) = VectorData(35);

    incr_shear_relative_displ_np1(0) = VectorData(36);
    incr_shear_relative_displ_np1(1) = VectorData(37);

    plastic_shear_relative_displ_n(0) = VectorData(38);
    plastic_shear_relative_displ_n(1) = VectorData(39);

    plastic_shear_relative_displ_np1(0) = VectorData(40);
    plastic_shear_relative_displ_np1(1) = VectorData(41);

    incr_plastic_shear_relative_displ_np1(0) = VectorData(42);
    incr_plastic_shear_relative_displ_np1(1) = VectorData(43 );

    // defining the contact plane based on the local x axis
    ContactPlane(x_local);

    return res;
}

int
ContactElement_Nonlinear_3DOF_3DOF::CheckMesh(ofstream &checkmesh_file)
{
    return 0;
}


int ContactElement_Nonlinear_3DOF_3DOF::getOutputSize() const
{
    return ContactElement_Nonlinear_3DOF_3DOF_OUTPUT_SIZE;
}

const Vector &ContactElement_Nonlinear_3DOF_3DOF::getOutput() const
{
    return outputVector;
}

int ContactElement_Nonlinear_3DOF_3DOF::update(void)
{
    int i;

    // cout << "update\n\n";
    Vector DispTrialI(3); // trial disp for slave node
    Vector DispTrialJ(3); // trial disp for master node

    DispTrialI = nodePointers[0]->getTrialDisp();
    DispTrialJ = nodePointers[1]->getTrialDisp();


    double DispTrial[6];

    DispTrial[0] = DispTrialI(0);
    DispTrial[1] = DispTrialI(1);
    DispTrial[2] = DispTrialI(2);
    DispTrial[3] = DispTrialJ(0);
    DispTrial[4] = DispTrialJ(1);
    DispTrial[5] = DispTrialJ(2);


    //set zero normal and shear displacements at time (n+1)
    incr_normal_relative_displ_np1 = 0.0;
    total_normal_relative_displ_np1 = 0.0;
    incr_shear_relative_displ_np1.Zero();
    total_shear_relative_displ_np1.Zero();

    //set zero normal and shear plastic displacements at time (n+1)
    incr_plastic_shear_relative_displ_np1.Zero();
    plastic_shear_relative_displ_np1.Zero();


    //set zero normal, shear and trial forces at time (n+1)
    normalforce_increment_np1 = 0.0;
    normalforce_np1 = 0.0;
    shearforce_increment_np1.Zero();
    shearforce_trial_np1.Zero();
    shearforce_np1.Zero();




    //compute total relative displacement at time (n+1)
    for (i = 0; i < 6; i++)
    {
        total_normal_relative_displ_np1 += N(i) * DispTrial[i];
        total_shear_relative_displ_np1(0) += T1(i) * DispTrial[i];
        total_shear_relative_displ_np1(1) += T2(i) * DispTrial[i];
    }


    // Compute increment of relative displacement
    incr_normal_relative_displ_np1 = total_normal_relative_displ_np1 - total_normal_relative_displ_n;
    incr_shear_relative_displ_np1  = total_shear_relative_displ_np1  - total_shear_relative_displ_n;


    // is_locked = 1;
    if (is_locked == 1) //Element is locked
    {
        Kn = Kn_locked;
        Kt = Kt_locked;
        normalforce_np1 = Kn * -total_normal_relative_displ_np1;
        shearforce_np1(0) = Kt * total_shear_relative_displ_np1(0);
        shearforce_np1(1) = Kt * total_shear_relative_displ_np1(1);
    }
    else   // Element is unlocked
    {

        if (nodes_are_in_contact())
        {
            double lamda;
            double tol = 0;
            double Kn_np1;
            double Kn_n;

            //Compute Kn at (n+1)/2
            Kn_np1 = 2 * Kn_factor * total_normal_relative_displ_np1;;
            Kn_n   = 2 * Kn_factor  * total_normal_relative_displ_n;
            // Kn = 0.5 * (Kn_np1 + Kn_n);
            Kn = Kn_np1;

            if (total_normal_relative_displ_n < ContactElement_Nonlinear_3DOF_3DOF_TOL_ZEROGAP) // Are in contact but stiffness is too low
            {
                Kn = 2 * Kn_factor  * ContactElement_Nonlinear_3DOF_3DOF_TOL_ZEROGAP;
            }


            // Compute normal force
            normalforce_np1 = -Kn_factor * total_normal_relative_displ_np1 * total_normal_relative_displ_np1;


            // Compute trial shear force

            shearforce_increment_np1  = Kt * (incr_shear_relative_displ_np1 );  //Kt: tangential penalty
            shearforce_trial_np1      = shearforce_n + shearforce_increment_np1;

            shear_force_trial_np1_norm = shearforce_trial_np1.Norm();

            // Check yielding in shear

            if (shear_force_trial_np1_norm == 0.0)
            {
                n_trial_np1.Zero();
            }
            else
            {
                n_trial_np1 = shearforce_trial_np1 / shear_force_trial_np1_norm;
            }

            yield_criteria = shear_force_trial_np1_norm - fs * fabs(normalforce_np1);  //Nice and simple
            // cout << "shearforce_trial_np1 = " << shearforce_trial_np1 <<
            //      "   normalforce_np1 = " << normalforce_np1 << endl;

            if (yield_criteria > tol)  //Sliding case
            {
                lamda                                 = 1 / Kt * (shear_force_trial_np1_norm - fs * fabs(normalforce_np1) - tol);

                shearforce_np1                        = fs * fabs(normalforce_np1) * n_trial_np1;
                // cout << "  > shearforce_trial_np1 = " << shearforce_trial_np1 << endl;

                incr_plastic_shear_relative_displ_np1 = lamda * n_trial_np1;
                plastic_shear_relative_displ_np1      = plastic_shear_relative_displ_n + incr_plastic_shear_relative_displ_np1;

                Kt = 0;

                SlidingFlag = 1;
            }
            else  // Sticking case
            {
                lamda = 0.0; // no plastic deformation

                shearforce_np1                        = shearforce_trial_np1;
                incr_plastic_shear_relative_displ_np1.Zero();
                plastic_shear_relative_displ_np1      = plastic_shear_relative_displ_n;

                Kt = Kt_locked;

                SlidingFlag = 0;
            }
            // cout << " --- > shearforce_np1 = " << shearforce_np1 << endl;

        }
        else //Nodes are not in contact
        {
            Kn = 0;
            Kt = 0;
            shearforce_np1.Zero();
            normalforce_np1 = 0;
        }
    }
    return 0;
}