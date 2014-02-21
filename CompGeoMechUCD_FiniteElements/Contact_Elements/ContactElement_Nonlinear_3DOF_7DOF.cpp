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
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#include "ContactElement_Nonlinear_3DOF_7DOF.h"
#include <Information.h>
#include <Domain.h>
#include <Node.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ElementResponse.h>


const int ContactElement_Nonlinear_3DOF_7DOF::numberNodes = 2 ;
Matrix  ContactElement_Nonlinear_3DOF_7DOF::stiff(10, 10);
Vector  ContactElement_Nonlinear_3DOF_7DOF::resid(10);
Vector  ContactElement_Nonlinear_3DOF_7DOF::localresid(10);
Matrix  ContactElement_Nonlinear_3DOF_7DOF::zeroMatrix(10, 10);
Vector ContactElement_Nonlinear_3DOF_7DOF::y_local(3);
Vector ContactElement_Nonlinear_3DOF_7DOF::z_local(3);
Vector ContactElement_Nonlinear_3DOF_7DOF::x_local_normalized(3);
Vector ContactElement_Nonlinear_3DOF_7DOF::y_local_normalized(3);
Vector ContactElement_Nonlinear_3DOF_7DOF::z_local_normalized(3);
Matrix ContactElement_Nonlinear_3DOF_7DOF::transformation(3, 3);


//*********************************************************************
//  Constructor for defining the contact element

ContactElement_Nonlinear_3DOF_7DOF::ContactElement_Nonlinear_3DOF_7DOF(int tag,
        int Nd1, int Nd2,
        double Knormal_factor,
        double frictionRatio,
        double maximum_gap,
        double x_local_1, double x_local_2, double x_local_3)
    : Element(tag, ELE_TAG_ContactElement_Nonlinear_3DOF_7DOF),
      connectedExternalNodes(numberNodes),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes),
      global_gap_np1(3), local_gap_np1(3),
      normalforce_n(0),  normalforce_np1(0), normalforce_increment_np1(0),
      shearforce_n(2),  shearforce_np1(2), shearforce_trial_np1(2), shearforce_increment_np1(2),
      total_normal_relative_displ_n(0), total_normal_relative_displ_np1(0), incr_normal_relative_displ_np1(0),
      total_shear_relative_displ_n(2), total_shear_relative_displ_np1(2), incr_shear_relative_displ_np1(2),
      plastic_shear_relative_displ_n(2), plastic_shear_relative_displ_np1(2), incr_plastic_shear_relative_displ_np1(2),
      yield_criteria(1), n_trial_np1(2)
{



    // ensure the connectedExternalNode ID is of correct size & set values
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ZeroLength::setUp - failed to create an ID of correct size\n";
    }

    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;

    // assign Kn_factor, Kt, fs, Gap_max
    Kn_factor = Knormal_factor ;          // [kN] [Force =  Kn_factor*delta_un/(Gap_max - delta_un)]
    // Kt_factor = Ktangential_factor;     // [Kt = Kt_factor*Kn]
    fs = frictionRatio;                   // [-]
    Gap_max = maximum_gap;                        // [m]

    //   Kt = Ktangential;                      // [kN/m]
    Kn = Kn_factor / Gap_max;             // [kN/m]


    ///Initialized print flags
    jjj = 0;
    print_option = 0; //1 -> print ; 0 -> no print // print information for one element
    numb_contact_element = 4;


    // initialized contact flag and sliding flag to be zero
    ContactFlag = 0;
    SlidingFlag = 0;

    // vector components in global coordinates defining local x-axis of the element
    Vector x_local(3);
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
ContactElement_Nonlinear_3DOF_7DOF::ContactElement_Nonlinear_3DOF_7DOF(void)
    : Element(0, ELE_TAG_ContactElement_Nonlinear_3DOF_7DOF),
      connectedExternalNodes(numberNodes),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes),
      normalforce_n(0), normalforce_np1(0), normalforce_increment_np1(0),
      shearforce_n(2), shearforce_np1(2), shearforce_trial_np1(2), shearforce_increment_np1(2),
      global_gap_np1(3), local_gap_np1(3),
      total_normal_relative_displ_n(0), total_normal_relative_displ_np1(0), incr_normal_relative_displ_np1(0),
      total_shear_relative_displ_n(2), total_shear_relative_displ_np1(2), incr_shear_relative_displ_np1(2),
      plastic_shear_relative_displ_n(2), plastic_shear_relative_displ_np1(2), incr_plastic_shear_relative_displ_np1(2),
      yield_criteria(0), n_trial_np1(2)
{

    // ensure the connectedExternalNode ID is of correct size
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ContactElement_Nonlinear_3DOF_7DOF::ContactElement_Nonlinear_3DOF_7DOF - failed to create an ID of correct size\n";
    }

    for (int j = 0; j < numberNodes; j++ )
    {
        nodePointers[j] = 0;
    }

    // print option
    print_option = 0;
    jjj = jjj + 1;

    if(jjj == numb_contact_element)
    {
        jjj = 0;
    }

    shearforce_n(0) = 0.0;
    shearforce_n(1) = 0.0;

}


//  Destructor
ContactElement_Nonlinear_3DOF_7DOF::~ContactElement_Nonlinear_3DOF_7DOF()
{

}

// returning the number of the nodes for element
int
ContactElement_Nonlinear_3DOF_7DOF::getNumExternalNodes(void) const
{
    return 2;
}

// returning the nodes of the elements
const ID&
ContactElement_Nonlinear_3DOF_7DOF::getExternalNodes(void)
{
    return connectedExternalNodes;
}

// returning a pointer for nodes of the element
Node**
ContactElement_Nonlinear_3DOF_7DOF::getNodePtrs(void)
{
    return nodePointers;
}

// returning the number of DOFs of the element
int
ContactElement_Nonlinear_3DOF_7DOF::getNumDOF(void)
{
    return numDOF;
}



void
ContactElement_Nonlinear_3DOF_7DOF::setDomain(Domain* theDomain)
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
            cerr << "WARNING ContactElement_Nonlinear_3DOF_7DOF::setDomain() - Nd1: " << Nd1 << " does not exist in ";
        }
        else
        {
            cerr << "WARNING ContactElement_Nonlinear_3DOF_7DOF::setDomain() - Nd2: " << Nd2 << " does not exist in ";
        }

        return;
    }

    // now determine the number of dof and the dimension
    int dofNd1 = nodePointers[0]->getNumberDOF();
    int dofNd2 = nodePointers[1]->getNumberDOF();


    // check dofs at the ends - print a warning message
    if (dofNd1 != 7)
    {
        cerr << "WARNING ContactElement_Nonlinear_3DOF_7DOF::setDomain(): nodes " << Nd1 <<
             "have different dof-numbs with respect to 7 " << this->getTag() << endln;
        return;
    }

    if (dofNd2 != 3)
    {
        cerr << "WARNING ContactElement_Nonlinear_3DOF_7DOF::setDomain(): nodes " << Nd2 <<
             "have different dof-numbs with respect to 3 " << this->getTag() << endln;
        return;
    }



    // call the base class method
    this->DomainComponent::setDomain(theDomain);

    if (dofNd1 == 7 && dofNd2 == 3)
    {
        numDOF = 10;
    }
    else
    {
        cerr << "WARNING ContactElement_Nonlinear_3DOF_7DOF::setDomain cannot handle dofs at nodes \n";
        return;
    }
}


int
ContactElement_Nonlinear_3DOF_7DOF::commitState()
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


    return 0;
}


int
ContactElement_Nonlinear_3DOF_7DOF::revertToLastCommit()
{
    return 0;
}


int
ContactElement_Nonlinear_3DOF_7DOF::revertToStart()
{
    return 0;
}


const Matrix&
ContactElement_Nonlinear_3DOF_7DOF::getDamp(void)
{
    zeroMatrix.Zero();
    return zeroMatrix;
}


const Matrix&
ContactElement_Nonlinear_3DOF_7DOF::getMass(void)
{
    zeroMatrix.Zero();
    return zeroMatrix;
}


void
ContactElement_Nonlinear_3DOF_7DOF::zeroLoad(void)
{
}

int
ContactElement_Nonlinear_3DOF_7DOF::addLoad(ElementalLoad* theLoad, double loadFactor)
{
    return 0;
}

int
ContactElement_Nonlinear_3DOF_7DOF::addInertiaLoadToUnbalance(const Vector& accel)
{
    return 0;
}




int ContactElement_Nonlinear_3DOF_7DOF::if_nodes_are_in_contact_or_not(void)
{

    double gap;


    Vector slaveNode_location(3);
    Vector masterNode_location(3);
    Vector slaveNode_TrialDisp_original(7);
    Vector slaveNode_TrialDisp_reduced(3);

    masterNode_location = nodePointers[1]->getTrialDisp();

    slaveNode_TrialDisp_original = nodePointers[0]->getTrialDisp();
    slaveNode_TrialDisp_reduced(0) = slaveNode_TrialDisp_original(0);
    slaveNode_TrialDisp_reduced(1) = slaveNode_TrialDisp_original(1);
    slaveNode_TrialDisp_reduced(2) = slaveNode_TrialDisp_original(2);

    slaveNode_location =  slaveNode_TrialDisp_reduced;


    global_gap_np1 = masterNode_location - slaveNode_location;



    local_gap_np1 = transformation * global_gap_np1;

    gap = Gap_max + local_gap_np1(0);


    if(print_option == 1)
    {
        cerr << "slaveNode_TrialDisp_reduced(2)= " << slaveNode_TrialDisp_reduced(2) << endln;
        cerr << "masterNode_location= " << masterNode_location(2) << endln;
        cerr << "Gap between nodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " is " << gap << " over " << Gap_max << endln;
    }



    if (gap < 0.0)
    {
        cerr << "gap < 0 !!!! " << endln;
        std::cin.get();

        return -1;
    }


    if (gap > Gap_max)
    {
        // Not in contact
        if(print_option == 1)
        {
            cerr << "Nodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " are NOT in Contact ..." << endln;
        }

        cerr << "gap " << gap << endln;
        return 0;
    }
    else
    {
        // contact occur
        if(print_option == 1)
        {
            cerr << "Nodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " are in Contact ..." << endln;
        }

        return 1;
    }


}



int ContactElement_Nonlinear_3DOF_7DOF::stick_or_slide(void)
{

    Vector DispTrialSlave(3); // trial disp for slave node
    Vector DispTrialMaster(3); // trial disp for master node


    int i;
    double lamda;
    double tol;


    tol = 0.0;//1.0e-10;

    DispTrialMaster = nodePointers[1]->getTrialDisp();


    Vector slaveNode_TrialDisp_original(7);
    Vector slaveNode_TrialDisp_reduced(3);

    slaveNode_TrialDisp_original = nodePointers[0]->getTrialDisp();
    slaveNode_TrialDisp_reduced(0) = slaveNode_TrialDisp_original(0);
    slaveNode_TrialDisp_reduced(1) = slaveNode_TrialDisp_original(1);
    slaveNode_TrialDisp_reduced(2) = slaveNode_TrialDisp_original(2);

    DispTrialSlave = slaveNode_TrialDisp_reduced;


    double DispTrial[6];

    DispTrial[0] = DispTrialMaster(0);
    DispTrial[1] = DispTrialMaster(1);
    DispTrial[2] = DispTrialMaster(2);
    DispTrial[3] = DispTrialSlave(0);
    DispTrial[4] = DispTrialSlave(1);
    DispTrial[5] = DispTrialSlave(2);

    if(print_option == 1)
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
    incr_normal_relative_displ_np1 = total_normal_relative_displ_np1 -  total_normal_relative_displ_n;

    for (i = 0; i < 2; i++)
    {
        incr_shear_relative_displ_np1(i) = total_shear_relative_displ_np1(i) - total_shear_relative_displ_n(i);
    }

    //Compute Kn at (n+1)/2
    double delta_u_np1;
    double delta_u_n;
    double Kn_np1;
    double Kn_n;
    delta_u_np1 = - total_normal_relative_displ_np1;
    delta_u_n = - total_normal_relative_displ_n;
    Kn_np1 = Kn_factor * Gap_max / ((Gap_max - delta_u_np1) * (Gap_max - delta_u_np1));
    Kn_n = Kn_factor * Gap_max / ((Gap_max - delta_u_n) * (Gap_max - delta_u_np1));

    Kn = 0.5 * (Kn_np1 + Kn_n);


    Kt = Kn;

    //    if(Kn>Kn_max) Kn=Kn_max;


    if(print_option == 1)
    {
        //Kn = Kn_np1;
        //cerr<< "Kn_n [kN/m] = " << Kn_n << endln;
        //cerr<< "Kn_np1 [kN/m] = " << Kn_np1 << endln;
        //cerr<< "total_normal_relative_displ_n = " << total_normal_relative_displ_n << endln;
        //cerr<< "total_normal_relative_displ_np1 = " << total_normal_relative_displ_np1 << endln;
        cerr << "stick or slide " << endln;
        cerr << "Kn [kN/m] = " << Kn << endln;
    }



    // Compute normal force
    normalforce_increment_np1 = Kn * incr_normal_relative_displ_np1;
    normalforce_np1 = normalforce_n + normalforce_increment_np1;


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

        if(shear_force_trial_np1_norm == 0.0)
        {
            n_trial_np1(i) = 0;
        }
    }


    // Check if trial state is inside or outside the yield surface
    yield_criteria = 0.0; // initialization
    lamda = 0.0;          // initialization
    yield_criteria = shear_force_trial_np1_norm - fs * fabs(normalforce_np1);

    // Trial state is outside the yield surface
    // Compute correct force state and plastic displacement
    if(yield_criteria > tol)
    {
        lamda = 1 / Kt * (shear_force_trial_np1_norm - fs * fabs(normalforce_np1) - tol);

        for(i = 0; i < 2; i++)
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



        if(print_option == 1)
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

        for(i = 0; i < 2; i++)
        {
            lamda = 0.0; // no plastic deformation
            shearforce_np1(i) = shearforce_trial_np1(i) - lamda * Kt * n_trial_np1(i);
            incr_plastic_shear_relative_displ_np1(i) = lamda * n_trial_np1(i);
            plastic_shear_relative_displ_np1(i) =  plastic_shear_relative_displ_n(i) + incr_plastic_shear_relative_displ_np1(i);
            shearforce_np1_norm = shearforce_np1.Norm();
            yield_criteria = shearforce_np1_norm - fs * fabs(normalforce_np1);
        }



        if(print_option == 1)
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




const Matrix&
ContactElement_Nonlinear_3DOF_7DOF::getTangentStiff(void)
{

    // print option
    jjj = jjj + 1;

    if(jjj == numb_contact_element)
    {
        jjj = 0;
    }


    if(print_option == 1)
    {
        cerr << "\n  "  << endln;
        cerr << "\nSTIFFNESS MATRIX  "  << endln;
    }


    ContactFlag = if_nodes_are_in_contact_or_not();
    int i , j;
    stiff.Zero( ) ;


    if (ContactFlag == 1) // in contact
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
        for (i = 0; i < 10; i++)
        {
            for (j = 0; j < 10; j++)
            {
                stiff(i, j) = 0.0;
            }
        }
    }



    if(print_option == 1)
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




const Matrix&
ContactElement_Nonlinear_3DOF_7DOF::getInitialStiff(void)
{

    getTangentStiff();

    return stiff ;
}



const Vector&
ContactElement_Nonlinear_3DOF_7DOF::getResistingForce()
{

    ContactFlag = if_nodes_are_in_contact_or_not();

    int i;
    resid.Zero( ) ;

    if (ContactFlag == 1) // in contact
    {
        stick_or_slide();


        for (i = 0; i < 6; i++)
        {
            resid(i) = (-normalforce_np1) * N(i) - shearforce_np1(0) * T1(i) - shearforce_np1(1) * T2(i);
        }


        if(print_option == 1)
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
            resid(i) = 0.0;
        }
    }


    return resid ;
}




const Vector&
ContactElement_Nonlinear_3DOF_7DOF::getLocalResistingForce()
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






const Vector&
ContactElement_Nonlinear_3DOF_7DOF::getResistingForceIncInertia()
{
    return this->getResistingForce();
}





void
ContactElement_Nonlinear_3DOF_7DOF::Print(ostream& s, int flag)
{
    if (flag == 0)
    {
        // print everything
        s << "Element: " << this->getTag();
        s << " type: ContactElement_Nonlinear_3DOF_7DOF  iNode: " << connectedExternalNodes(0);
        s << " jNode: " << connectedExternalNodes(1) << endln;
    }
    else if (flag == 1)
    {
        s << this->getTag() << endln;
    }
}




Response*
ContactElement_Nonlinear_3DOF_7DOF::setResponse(const char** argv, int argc, Information& eleInformation)
{
    if (strcmp(argv[0], "force") == 0 || strcmp(argv[0], "forces") == 0)
    {
        return new ElementResponse(this, 1, resid);
    }

    // tangent stiffness matrix
    else if (strcmp(argv[0], "stiff") == 0 || strcmp(argv[0], "stiffness") == 0)
    {
        return new ElementResponse(this, 2, stiff);
    }

    if (strcmp(argv[0], "localforce") == 0 || strcmp(argv[0], "localforces") == 0)
    {
        return new ElementResponse(this, 3, localresid);
    }

    if (strcmp(argv[0], "nodaldisplacement") == 0 || strcmp(argv[0], "nodesdisp") == 0)
    {
        return new ElementResponse(this, 4, Vector(10) );
    }

    if (strcmp(argv[0], "contactsituation") == 0 || strcmp(argv[0], "contactsituation") == 0)
    {
        return new ElementResponse(this, 5, Vector(2) );
    }

    if (strcmp(argv[0], "relativedisplacement") == 0 || strcmp(argv[0], "reldisp") == 0)
    {
        return new ElementResponse(this, 6, Vector(3) );
    }

    else
    {
        return 0;
    }
}



int
ContactElement_Nonlinear_3DOF_7DOF::getResponse(int responseID, Information& eleInfo)
{
    if (responseID == 1)
    {
        return eleInfo.setVector(resid);
    }

    else if (responseID == 2)
    {
        return eleInfo.setMatrix(stiff);
    }

    else if (responseID == 3)
    {
        return eleInfo.setVector(localresid);
    }

    else if (responseID == 4)
    {
        static Vector nodaldisplacements(3 * 2);
        Vector slaveNode_displacement(7);
        Vector masterNode_displacement(3);

        slaveNode_displacement  = nodePointers[0]->getTrialDisp();
        masterNode_displacement = nodePointers[1]->getTrialDisp();


        for (int i = 0; i < 6; i++)
        {
            nodaldisplacements(0) = slaveNode_displacement(0);
            nodaldisplacements(1) = slaveNode_displacement(1);
            nodaldisplacements(2) = slaveNode_displacement(2);
            nodaldisplacements(3) = masterNode_displacement(0);
            nodaldisplacements(4) = masterNode_displacement(1);
            nodaldisplacements(5) = masterNode_displacement(2);
        }

        return eleInfo.setVector(nodaldisplacements);
    }
    else if (responseID == 5)
    {
        static Vector contactsituation(2);
        contactsituation(0) = ContactFlag;
        contactsituation(1) = SlidingFlag;

        return eleInfo.setVector(contactsituation);
    }

    else if (responseID == 6)
    {
        static Vector RelativeDisplacement(3);

        RelativeDisplacement(0) = total_normal_relative_displ_np1;
        RelativeDisplacement(1) = total_shear_relative_displ_np1(0);
        RelativeDisplacement(2) = total_shear_relative_displ_np1(1);

        return eleInfo.setVector(RelativeDisplacement);
    }

    else
    {
        return -1;
    }
}





int
ContactElement_Nonlinear_3DOF_7DOF::ContactPlane(const Vector& x_local)
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


int
ContactElement_Nonlinear_3DOF_7DOF::sendSelf(int commitTag, Channel& theChannel)
{
    return 0;
}

int
ContactElement_Nonlinear_3DOF_7DOF::recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    return 0;
}


double
ContactElement_Nonlinear_3DOF_7DOF::returnPressure(void)
{

    Vector node_displacements(7);
    node_displacements = nodePointers[0]->getTrialDisp();
    double pore_pressure = node_displacements(3);

    return pore_pressure;
}


