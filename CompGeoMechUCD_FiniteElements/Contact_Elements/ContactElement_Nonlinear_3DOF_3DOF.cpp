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
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes),
      normalforce_n(0), normalforce_np1(0), normalforce_increment_np1(0),
      shearforce_n(2), shearforce_np1(2), shearforce_trial_np1(2), shearforce_increment_np1(2),
      global_gap_np1(3), local_gap_np1(3),
      total_normal_relative_displ_n(0), total_normal_relative_displ_np1(0), incr_normal_relative_displ_np1(0),
      total_shear_relative_displ_n(2), total_shear_relative_displ_np1(2), incr_shear_relative_displ_np1(2),
      plastic_shear_relative_displ_n(2), plastic_shear_relative_displ_np1(2), incr_plastic_shear_relative_displ_np1(2),
      yield_criteria(0), n_trial_np1(2), resid(3 * numberNodes), is_locked(0), outputVector(ContactElement_Nonlinear_3DOF_3DOF_OUTPUT_SIZE)
{

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

    Kn = Kn_factor / Gap_max;
    //   Kt = Ktangential;

    ///Initialized print flags
    print_option = 0;
    // numb_contact_element = 4;


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
    //   yield_criteria = 0.0;
    total_normal_relative_displ_n = 0.0;
    total_normal_relative_displ_np1 = 0.0;
    incr_normal_relative_displ_np1 = 0.0;


    for (int i = 0; i < 2; i++)
    {
        shearforce_n(i) = 0.0;
        shearforce_np1(i) = 0.0;
        shearforce_trial_np1(i) = 0.0;
        shearforce_increment_np1(i) = 0.0;
        total_shear_relative_displ_n(i) = 0.0;
        total_shear_relative_displ_np1(i) = 0.0;
        incr_shear_relative_displ_np1(i) = 0.0;
        plastic_shear_relative_displ_n(i) = 0.0;
        plastic_shear_relative_displ_np1(i) = 0.0;
        incr_plastic_shear_relative_displ_np1(i) = 0.0;
        n_trial_np1(i) = 0.0;
    }

    for (int i = 0; i < 3; i++)
    {
        global_gap_np1(i) = 0.0;
        local_gap_np1(i) = 0.0;
    }


    // defining the contact plane based on the local x axis
    definecontact = ContactPlane(x_local);

    if (definecontact != 0)
    {
        cerr << "\nProblem in Contact Plane ...";
    }

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
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes),
      normalforce_n(0), normalforce_np1(0), normalforce_increment_np1(0),
      shearforce_n(2), shearforce_np1(2), shearforce_trial_np1(2), shearforce_increment_np1(2),
      global_gap_np1(3), local_gap_np1(3),
      total_normal_relative_displ_n(0), total_normal_relative_displ_np1(0), incr_normal_relative_displ_np1(0),
      total_shear_relative_displ_n(2), total_shear_relative_displ_np1(2), incr_shear_relative_displ_np1(2),
      plastic_shear_relative_displ_n(2), plastic_shear_relative_displ_np1(2), incr_plastic_shear_relative_displ_np1(2),
      yield_criteria(0), n_trial_np1(2), resid(3 * numberNodes),
      is_locked(lock_flag), outputVector(ContactElement_Nonlinear_3DOF_3DOF_OUTPUT_SIZE)
{

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

    Kn = Kn_factor / Gap_max;
    //   Kt = Ktangential;

    ///Initialized print flags
    print_option = 1;
    numb_contact_element = 4;


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
    //   yield_criteria = 0.0;
    total_normal_relative_displ_n = 0.0;
    total_normal_relative_displ_np1 = 0.0;
    incr_normal_relative_displ_np1 = 0.0;


    for (int i = 0; i < 2; i++)
    {
        shearforce_n(i) = 0.0;
        shearforce_np1(i) = 0.0;
        shearforce_trial_np1(i) = 0.0;
        shearforce_increment_np1(i) = 0.0;
        total_shear_relative_displ_n(i) = 0.0;
        total_shear_relative_displ_np1(i) = 0.0;
        incr_shear_relative_displ_np1(i) = 0.0;
        plastic_shear_relative_displ_n(i) = 0.0;
        plastic_shear_relative_displ_np1(i) = 0.0;
        incr_plastic_shear_relative_displ_np1(i) = 0.0;
        n_trial_np1(i) = 0.0;
    }

    for (int i = 0; i < 3; i++)
    {
        global_gap_np1(i) = 0.0;
        local_gap_np1(i) = 0.0;
    }


    // defining the contact plane based on the local x axis
    definecontact = ContactPlane(x_local);
    if (definecontact != 0)
    {
        cerr << "\nProblem in Contact Plane ...";
    }

}
//null constructor
ContactElement_Nonlinear_3DOF_3DOF::ContactElement_Nonlinear_3DOF_3DOF(void)
    : Element(0, ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF),
      connectedExternalNodes(numberNodes),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes),
      normalforce_n(0), normalforce_np1(0), normalforce_increment_np1(0),
      shearforce_n(2), shearforce_np1(2), shearforce_trial_np1(2), shearforce_increment_np1(2),
      global_gap_np1(3), local_gap_np1(3),
      total_normal_relative_displ_n(0), total_normal_relative_displ_np1(0), incr_normal_relative_displ_np1(0),
      total_shear_relative_displ_n(2), total_shear_relative_displ_np1(2), incr_shear_relative_displ_np1(2),
      plastic_shear_relative_displ_n(2), plastic_shear_relative_displ_np1(2), incr_plastic_shear_relative_displ_np1(2),
      yield_criteria(0), n_trial_np1(2), resid(3 * numberNodes), is_locked(0), outputVector(ContactElement_Nonlinear_3DOF_3DOF_OUTPUT_SIZE)
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

    // print option
    print_option = 1;


    shearforce_n(0) = 0.0;
    shearforce_n(1) = 0.0;
    //Added by Babak by 11/22/13:
    //------
    normalforce_n = 0.0;
    normalforce_np1 = 0.0;
    normalforce_increment_np1 = 0.0;
    //   yield_criteria = 0.0;
    total_normal_relative_displ_n = 0.0;
    total_normal_relative_displ_np1 = 0.0;
    incr_normal_relative_displ_np1 = 0.0;


    for (int i = 0; i < 2; i++)
    {
        shearforce_n(i) = 0.0;
        shearforce_np1(i) = 0.0;
        shearforce_trial_np1(i) = 0.0;
        shearforce_increment_np1(i) = 0.0;
        total_shear_relative_displ_n(i) = 0.0;
        total_shear_relative_displ_np1(i) = 0.0;
        incr_shear_relative_displ_np1(i) = 0.0;
        plastic_shear_relative_displ_n(i) = 0.0;
        plastic_shear_relative_displ_np1(i) = 0.0;
        incr_plastic_shear_relative_displ_np1(i) = 0.0;
        n_trial_np1(i) = 0.0;
    }

    for (int i = 0; i < 3; i++)
    {
        global_gap_np1(i) = 0.0;
        local_gap_np1(i) = 0.0;
    }


    // defining the contact plane based on the local x axis
    //   definecontact = ContactPlane(x_local);
    //   if (definecontact != 0)
    //       cerr << "\nProblem in Contact Plane ...";
    //------

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
    // update total and plastic diplacements
    plastic_shear_relative_displ_n = plastic_shear_relative_displ_np1;
    plastic_shear_relative_displ_np1.Zero();
    total_shear_relative_displ_n = total_shear_relative_displ_np1;
    total_shear_relative_displ_np1.Zero();
    total_normal_relative_displ_n = total_normal_relative_displ_np1;
    total_normal_relative_displ_np1 = 0.0;

    // update forces
    normalforce_n = normalforce_np1;
    normalforce_np1 = 0.0;
    shearforce_n = shearforce_np1;
    shearforce_np1.Zero();


    // Output

    outputVector(0) = plastic_shear_relative_displ_n(0);
    outputVector(1) = total_shear_relative_displ_n(0);
    outputVector(2) = total_normal_relative_displ_n;
    outputVector(3) = normalforce_n;
    outputVector(4) = shearforce_n(0);




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




int ContactElement_Nonlinear_3DOF_3DOF::if_nodes_are_in_contact_or_not(void)
{

    double gap;


    Vector slaveNode_location(3);
    Vector masterNode_location(3);


    slaveNode_location  = nodePointers[0]->getTrialDisp();
    masterNode_location = nodePointers[1]->getTrialDisp();


    global_gap_np1 = masterNode_location - slaveNode_location;



    local_gap_np1 = transformation * global_gap_np1;

    gap = Gap_max + local_gap_np1(0);

    if (print_option == 1)
    {
        cerr << "Gap between nodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " is " << gap << " over " << Gap_max << endln;
    }


    if (gap <= Gap_max)
    {
        // contact occur
        if (print_option == 1)
        {
            cerr << "\nNodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " are in Contact ...";
            cerr << "gap " << gap << endln;
        }

        return 1;
    }
    else
    {
        if (print_option == 1)
        {
            cerr << "\nNodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " are NOT in Contact ...";
            cerr << "gap " << gap << endln;
        }


        return 0;
    }


    cerr << "ContactElement_Nonlinear_3DOF_3DOF::if_nodes_are_in_contact_or_not: If it reaches here something is wrong! " << endl;
    exit(-1);
}



int ContactElement_Nonlinear_3DOF_3DOF::stick_or_slide(void)
{

    Vector DispTrialSlave(3); // trial disp for slave node
    Vector DispTrialMaster(3); // trial disp for master node


    int i;
    double lamda;
    double tol;


    tol = 0.0;
    //    tol = 1.0e-10;



    DispTrialSlave = nodePointers[0]->getTrialDisp();
    DispTrialMaster = nodePointers[1]->getTrialDisp();


    double DispTrial[6];

    DispTrial[0] = DispTrialMaster(0);
    DispTrial[1] = DispTrialMaster(1);
    DispTrial[2] = DispTrialMaster(2);
    DispTrial[3] = DispTrialSlave(0);
    DispTrial[4] = DispTrialSlave(1);
    DispTrial[5] = DispTrialSlave(2);

    if (print_option == 1)
    {
        cerr << "\n "  << endln;
        cerr << "DispTrial[0]= " << DispTrial[0] << endln;
        cerr << "DispTrial[1]= " << DispTrial[1] << endln;
        cerr << "DispTrial[2]= " << DispTrial[2] << endln;
        cerr << "DispTrial[3]= " << DispTrial[3] << endln;
        cerr << "DispTrial[4]= " << DispTrial[4] << endln;
        cerr << "DispTrial[5]= " << DispTrial[5] << endln;
        cerr << "\n "  << endln;
    }

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

    for (i = 0; i < 2; i++)
    {
        incr_shear_relative_displ_np1(i) = total_shear_relative_displ_np1(i) - total_shear_relative_displ_n(i);
    }


    //Compute Kn at (n+1)/2

    if (print_option == 1)
    {
        cout << "total_normal_relative_displ_np1 = " << fabs(total_normal_relative_displ_np1) << endl;
    }


    //    if(fabs(total_normal_relative_displ_np1) <= 0.9*Gap_max)
    //    {


    double delta_u_np1;
    double delta_u_n;
    double Kn_np1;
    double Kn_n;
    delta_u_np1 = - total_normal_relative_displ_np1;
    delta_u_n = - total_normal_relative_displ_n;

    Kn_np1 = Kn_factor * Gap_max / ((Gap_max - delta_u_np1) * (Gap_max - delta_u_np1));

    Kn_n = Kn_factor * Gap_max / ((Gap_max - delta_u_n) * (Gap_max - delta_u_np1));


    if (print_option == 1)
    {
        cout << "delta_u_np1 = " << delta_u_np1 << endl;
        cout << "Gap_max = " << Gap_max << endl;
        cout << "Kn_np1 = " << Kn_np1 << endl;
        cout << "Kn_n = " << Kn_n << endl;
    }


    Kn = 0.5 * (Kn_np1 + Kn_n);
    //    }


    Kt = Kn;

    if (print_option == 1)
    {
        cout << "Kn = " << Kn << endl;
    }


    if (print_option == 1)
    {
        cerr << "stick or slide " << endln;
        cerr << "Kn [kN/m] = " << Kn << endln;
        cerr << "Kt [kN/m] = " << Kt << endln;
    }




    // Compute normal force
    normalforce_increment_np1 = Kn * incr_normal_relative_displ_np1;
    normalforce_np1 = normalforce_n + normalforce_increment_np1;


    //    if(print_option == 1)
    //    {
    //      cerr<< "incr_normal_relative_displ_np1 = " << incr_normal_relative_displ_np1 << endln;
    // cerr << "incr_shear_relative_displ_np1(0) = " << incr_shear_relative_displ_np1(0) << endln;
    // cerr << "incr_shear_relative_displ_np1(1) = " << incr_shear_relative_displ_np1(1) << endln;
    //      cerr<< "normalforce_n = " << normalforce_n << endln;
    //      cerr<< "normalforce_np1 = " << normalforce_np1 << endln;
    //    }



    // Compute trial shear force
    for (i = 0; i < 2; i++)
    {
        shearforce_increment_np1(i) = Kt * incr_shear_relative_displ_np1(i);    //Kt: tangential penalty
    }

    for (i = 0; i < 2; i++)
    {
        shearforce_trial_np1(i) = shearforce_n(i) + shearforce_increment_np1(i);
    }


    // Compute shearforce_trial_np1 norm and n_trial_np1
    shear_force_trial_np1_norm = shearforce_trial_np1.Norm();

    for (i = 0; i < 2; i++)
    {
        n_trial_np1(i) = shearforce_trial_np1(i) / shear_force_trial_np1_norm;

        if (shear_force_trial_np1_norm == 0.0)
        {
            n_trial_np1(i) = 0;
        }
    }


    // Check if trial state is inside or outside the yield surface
    yield_criteria = 0.0; // initialization
    lamda = 0.0;          // initialization
    yield_criteria = shear_force_trial_np1_norm - fs * fabs(normalforce_np1);



    // cerr << "normalforce_np1 = " << fabs(normalforce_np1) << endln;
    // cerr << "shearforce_trial_np1(0) = " << shearforce_trial_np1(0) << endln;
    // cerr << "shearforce_trial_np1(1) = " << shearforce_trial_np1(1) << endln;
    // cerr << "shear_force_trial_np1_norm = " << shear_force_trial_np1_norm << endln;
    // cerr << "yield_criteria = " << yield_criteria << endln;



    // Trial state is outside the yield surface
    // Compute correct force state and plastic displacement
    if (yield_criteria > tol)
    {
        lamda = 1 / Kt * (shear_force_trial_np1_norm - fs * fabs(normalforce_np1) - tol);

        for (i = 0; i < 2; i++)
        {
            //shearforce_np1(i) = shearforce_trial_np1(i) - lamda*Kt*n_trial_np1(i);
            shearforce_np1(i) = fs * fabs(normalforce_np1) * n_trial_np1(i);
            //fs*abs(normalforce_np1)*force_angle(i) + tol;
            incr_plastic_shear_relative_displ_np1(i) = lamda * n_trial_np1(i);
            plastic_shear_relative_displ_np1(i) =  plastic_shear_relative_displ_n(i) + incr_plastic_shear_relative_displ_np1(i);
        }

        shearforce_np1_norm = shearforce_np1.Norm();
        yield_criteria = shearforce_np1_norm - fs * fabs(normalforce_np1);



        cerr << "Sliding ..." << endln;



        if (print_option == 1)
        {
            // slide
            cerr << "Sliding ..." << endln;

            cerr << "incr_normal_relative_displ_np1 = " << incr_normal_relative_displ_np1 << endln;
            cerr << "incr_shear_relative_displ_np1(0) = " << incr_shear_relative_displ_np1(0) << endln;
            cerr << "incr_shear_relative_displ_np1(1) = " << incr_shear_relative_displ_np1(1) << endln;
            cerr << "normalforce_n = " << normalforce_n << endln;
            cerr << "normalforce_np1 = " << normalforce_np1 << endln;
            cerr << "shearforce_n(0) = " << shearforce_n(0) << endln;
            cerr << "shearforce_n(1) = " << shearforce_n(1) << endln;
            cerr << "shearforce_trial_np1(0) = " << shearforce_trial_np1(0) << endln;
            cerr << "shearforce_trial_np1(1) = " << shearforce_trial_np1(1) << endln;
            cerr << "shearforce_np1(0) = " << shearforce_np1(0) << endln;
            cerr << "shearforce_np1(1) = " << shearforce_np1(1) << endln;
            cerr << "n_trial_np1(0) = " << n_trial_np1(0) << endln;
            cerr << "n_trial_np1(1) = " << n_trial_np1(1) << endln;
            cerr << "yield_criteria = " << yield_criteria << endln;
            cerr << "lamda = " << lamda << endln;
            cerr << "incr_plastic_shear_relative_displ_np1(0) = " << incr_plastic_shear_relative_displ_np1(0) << endln;
            cerr << "incr_plastic_shear_relative_displ_np1(1) = " << incr_plastic_shear_relative_displ_np1(1) << endln;
            cerr << "plastic_shear_relative_displ_np1(0) = " << plastic_shear_relative_displ_np1(0) << endln;
            cerr << "plastic_shear_relative_displ_np1(1) = " << plastic_shear_relative_displ_np1(1) << endln;
            cerr << "\n " << endln;
        }

        SlidingFlag = 1;

        //     return 1;

    }
    else
    {
        // stick

        for (i = 0; i < 2; i++)
        {
            lamda = 0.0; // no plastic deformation
            shearforce_np1(i) = shearforce_trial_np1(i) - lamda * Kt * n_trial_np1(i);
            incr_plastic_shear_relative_displ_np1(i) = lamda * n_trial_np1(i);
            plastic_shear_relative_displ_np1(i) =  plastic_shear_relative_displ_n(i) + incr_plastic_shear_relative_displ_np1(i);
            shearforce_np1_norm = shearforce_np1.Norm();
            yield_criteria = shearforce_np1_norm - fs * fabs(normalforce_np1);
        }


        // cerr << "Sticking ..." << endln;

        if (print_option == 1)
        {
            cerr << "Sticking ..." << endln;
            cerr << "incr_normal_relative_displ_np1 = " << incr_normal_relative_displ_np1 << endln;
            cerr << "incr_shear_relative_displ_np1(0) = " << incr_shear_relative_displ_np1(0) << endln;
            cerr << "incr_shear_relative_displ_np1(1) = " << incr_shear_relative_displ_np1(1) << endln;
            cerr << "normalforce_n = " << normalforce_n << endln;
            cerr << "normalforce_np1 = " << normalforce_np1 << endln;
            cerr << "shearforce_n(0) = " << shearforce_n(0) << endln;
            cerr << "shearforce_n(1) = " << shearforce_n(1) << endln;
            cerr << "shearforce_trial_np1(0) = " << shearforce_trial_np1(0) << endln;
            cerr << "shearforce_trial_np1(1) = " << shearforce_trial_np1(1) << endln;
            cerr << "shearforce_np1(0) = " << shearforce_np1(0) << endln;
            cerr << "shearforce_np1(1) = " << shearforce_np1(1) << endln;
            cerr << "n_trial_np1(0) = " << n_trial_np1(0) << endln;
            cerr << "n_trial_np1(1) = " << n_trial_np1(1) << endln;
            cerr << "yield_criteria = " << yield_criteria << endln;
            cerr << "lamda = " << lamda << endln;
            cerr << "incr_plastic_shear_relative_displ_np1(0) = " << incr_plastic_shear_relative_displ_np1(0) << endln;
            cerr << "incr_plastic_shear_relative_displ_np1(1) = " << incr_plastic_shear_relative_displ_np1(1) << endln;
            cerr << "plastic_shear_relative_displ_np1(0) = " << plastic_shear_relative_displ_np1(0) << endln;
            cerr << "plastic_shear_relative_displ_np1(1) = " << plastic_shear_relative_displ_np1(1) << endln;
            cerr << "\n " << endln;
        }


        SlidingFlag = 0;

        //     return 0;

    }


    //    cerr << "ContactElement_Nonlinear_3DOF_3DOF::stick_or_slide: If it reaches here something is wrong!" << endln;

    return SlidingFlag;

}




const Matrix &
ContactElement_Nonlinear_3DOF_3DOF::getTangentStiff(void)
{


    if (print_option == 1)
    {
        cerr << "\n  "  << endln;
        cerr << "\nSTIFFNESS MATRIX  "  << endln;
    }


    ContactFlag = if_nodes_are_in_contact_or_not();
    int i , j;
    stiff.Zero( ) ;


    if ((ContactFlag == 1) || (is_locked == 1)) // in contact or locked
    {

        stick_or_slide();


        for (i = 0; i < 6; i++)
        {
            for (j = 0; j < 6; j++)
            {
                stiff(i, j) = Kn * (N(i) * N(j)) + Kt * (T1(i) * T1(j) + T2(i) * T2(j));
            }
        }


    }

    else if (ContactFlag == 0)  // not in contact
    {
        for (i = 0; i < 6; i++)
        {
            for (j = 0; j < 6; j++)
            {
                stiff(i, j) = 0.0;
            }
        }
    }



    if (print_option == 1)
    {

        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                cout << "K(" << i + 1 << "," << j + 1 << "):" << stiff(i, j) << "   ";
            }

            cout << endl;
        }

    }

    ///////////////////////////////////////////////////////////////////////


    return stiff ;
}




const Matrix &
ContactElement_Nonlinear_3DOF_3DOF::getInitialStiff(void)
{

    getTangentStiff();

    return stiff ;
}



const Vector &
ContactElement_Nonlinear_3DOF_3DOF::getResistingForce()
{

    ContactFlag = if_nodes_are_in_contact_or_not();

    int i;
    resid.Zero( ) ;

    if (ContactFlag == 1) // in contact
    {
        stick_or_slide();


        for (i = 0; i < 6; i++)
        {
            resid(i) = (-normalforce_np1) * N(i) + shearforce_np1(0) * T1(i) + shearforce_np1(1) * T2(i);
        }


        if (print_option == 1)
        {
            for (i = 0; i < 6; i++)
            {
                cerr << "resid(i) = " << resid(i);
            }

            cerr << endl;
        }

    }
    else if (ContactFlag == 0)    // not in contact
    {

        for (i = 0; i < 6; i++)
        {
            resid(i) = 0.0;   // Not exactly zero....
        }
    }


    return resid ;
}




const Vector &
ContactElement_Nonlinear_3DOF_3DOF::getLocalResistingForce()
{
    ContactFlag = if_nodes_are_in_contact_or_not();

    int i;
    resid.Zero( ) ;

    if (ContactFlag == 1) // contacted
    {
        stick_or_slide();


        for (i = 0; i < 6; i++)
        {
            localresid(0) = -normalforce_np1;
            localresid(1) = -shearforce_np1(0);
            localresid(2) =  shearforce_np1(1);
            localresid(3) =  normalforce_np1;
            localresid(4) =  shearforce_np1(0);
            localresid(5) = -shearforce_np1(1);
        }


    }
    else if (ContactFlag == 0)  // not contacted
    {

        for (i = 0; i < 6; i++)
        {
            localresid(i) = 0.0;
        }
    }


    return localresid;
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




// Response*
// ContactElement_Nonlinear_3DOF_3DOF::setResponse(const char** argv, int argc, Information& eleInformation)
// {
//     if (strcmp(argv[0], "force") == 0 || strcmp(argv[0], "forces") == 0)
//     {
//         return new ElementResponse(this, 1, resid);
//     }

//     // tangent stiffness matrix
//     else if (strcmp(argv[0], "stiff") == 0 || strcmp(argv[0], "stiffness") == 0)
//     {
//         return new ElementResponse(this, 2, stiff);
//     }

//     if (strcmp(argv[0], "localforce") == 0 || strcmp(argv[0], "localforces") == 0)
//     {
//         return new ElementResponse(this, 3, localresid);
//     }

//     if (strcmp(argv[0], "nodaldisplacement") == 0 || strcmp(argv[0], "nodesdisp") == 0)
//     {
//         return new ElementResponse(this, 4, Vector(3 * 2) );
//     }

//     if (strcmp(argv[0], "contactsituation") == 0 || strcmp(argv[0], "contactsituation") == 0)
//     {
//         return new ElementResponse(this, 5, Vector(2) );
//     }

//     if (strcmp(argv[0], "relativedisplacement") == 0 || strcmp(argv[0], "reldisp") == 0)
//     {
//         return new ElementResponse(this, 6, Vector(3) );
//     }

//     else
//     {
//         return 0;
//     }
// }


// int
// ContactElement_Nonlinear_3DOF_3DOF::getResponse(int responseID, Information& eleInfo)
// {
//     if (responseID == 1)
//     {
//         return eleInfo.setVector(resid);
//     }

//     else if (responseID == 2)
//     {
//         return eleInfo.setMatrix(stiff);
//     }

//     else if (responseID == 3)
//     {
//         return eleInfo.setVector(localresid);
//     }

//     else if (responseID == 4)
//     {
//         static Vector nodaldisplacements(3 * 2);
//         Vector slaveNode_displacement(3);
//         Vector masterNode_displacement(3);

//         slaveNode_displacement  = nodePointers[0]->getTrialDisp();
//         masterNode_displacement = nodePointers[1]->getTrialDisp();


//         for (int i = 0; i < 6; i++)
//         {
//             nodaldisplacements(0) = slaveNode_displacement(0);
//             nodaldisplacements(1) = slaveNode_displacement(1);
//             nodaldisplacements(2) = slaveNode_displacement(2);
//             nodaldisplacements(3) = masterNode_displacement(0);
//             nodaldisplacements(4) = masterNode_displacement(1);
//             nodaldisplacements(5) = masterNode_displacement(2);
//         }

//         return eleInfo.setVector(nodaldisplacements);
//     }
//     else if (responseID == 5)
//     {
//         static Vector contactsituation(2);
//         contactsituation(0) = ContactFlag;
//         contactsituation(1) = SlidingFlag;

//         return eleInfo.setVector(contactsituation);
//     }

//     else if (responseID == 6)
//     {
//         static Vector RelativeDisplacement(3);
//         RelativeDisplacement(0) = normalforce_np1   / Kn;
//         RelativeDisplacement(1) = shearforce_np1(0) / Kt;
//         RelativeDisplacement(2) = shearforce_np1(1) / Kt;

//         return eleInfo.setVector(RelativeDisplacement);
//     }

//     else
//     {
//         return -1;
//     }
// }





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


    //  cerr<< "N(0) = " << N(0) << endln;
    //  cerr<< "N(1) = " << N(1) << endln;
    //  cerr<< "N(2) = " << N(2) << endln;
    //  cerr<< "N(3) = " << N(3) << endln;
    //  cerr<< "N(4) = " << N(4) << endln;
    //  cerr<< "N(5) = " << N(5) << endln;



    T1(0)  =   y_local_normalized(0) ;
    T1(1)  =   y_local_normalized(1) ;
    T1(2)  =   y_local_normalized(2) ;
    T1(3)  =  -y_local_normalized(0) ;
    T1(4)  =  -y_local_normalized(1) ;
    T1(5)  =  -y_local_normalized(2) ;

    //  cerr<< "T1(0) = " << T1(0) << endln;
    //  cerr<< "T1(1) = " << T1(1) << endln;
    //  cerr<< "T1(2) = " << T1(2) << endln;
    //  cerr<< "T1(3) = " << T1(3) << endln;
    //  cerr<< "T1(4) = " << T1(4) << endln;
    //  cerr<< "T1(5) = " << T1(5) << endln;



    T2(0)  =   z_local_normalized(0) ;
    T2(1)  =   z_local_normalized(1) ;
    T2(2)  =   z_local_normalized(2) ;
    T2(3)  =  -z_local_normalized(0) ;
    T2(4)  =  -z_local_normalized(1) ;
    T2(5)  =  -z_local_normalized(2) ;

    //  cerr<< "T2(0) = " << T2(0) << endln;
    //  cerr<< "T2(1) = " << T2(1) << endln;
    //  cerr<< "T2(2) = " << T2(2) << endln;
    //  cerr<< "T2(3) = " << T2(3) << endln;
    //  cerr<< "T2(4) = " << T2(4) << endln;
    //  cerr<< "T2(5) = " << T2(5) << endln;



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
    idData(5) = definecontact;

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
    definecontact = idData(5);

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
    definecontact = ContactPlane(x_local);

    if (definecontact != 0)
    {
        cerr << "\nProblem in Contact Plane ...";
    }

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