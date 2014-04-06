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
// PURPOSE:           Zero Length Element for Contact
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:
// UPDATE HISTORY:    SendSelf and RecvSelf is implemented by Babak Kamrani 10/30/2013
//            Some changes are made in blank constructor and the constructor as well ... some changes in header file is performed
//
///////////////////////////////////////////////////////////////////////////////


#include "ContactElement_3DOF_3DOF.h"
#include <Information.h>
#include <Domain.h>
#include <Node.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
// #include <ElementResponse.h>


const int ContactElement_3DOF_3DOF::numberNodes = 2 ;
Matrix  ContactElement_3DOF_3DOF::stiff(3 * numberNodes, 3 * numberNodes);
Vector  ContactElement_3DOF_3DOF::resid(3 * numberNodes);
Vector  ContactElement_3DOF_3DOF::localresid(3 * numberNodes);
Matrix  ContactElement_3DOF_3DOF::zeroMatrix(3 * numberNodes, 3 * numberNodes);
Vector ContactElement_3DOF_3DOF::x_local(3); //Added by Babak by 11/20/13
Vector ContactElement_3DOF_3DOF::y_local(3);
Vector ContactElement_3DOF_3DOF::z_local(3);
Vector ContactElement_3DOF_3DOF::x_local_normalized(3);
Vector ContactElement_3DOF_3DOF::y_local_normalized(3);
Vector ContactElement_3DOF_3DOF::z_local_normalized(3);
// double ContactElement_3DOF_3DOF::normalforce;
// Vector ContactElement_3DOF_3DOF::shearforce(2);
Matrix ContactElement_3DOF_3DOF::transformation(3, 3);
// Vector ContactElement_3DOF_3DOF::global_gap(3);
// Vector ContactElement_3DOF_3DOF::local_gap(3);
// Vector ContactElement_3DOF_3DOF::shear_relative_displacement(2);

//*********************************************************************
//  Constructor for defining the contact element

ContactElement_3DOF_3DOF::ContactElement_3DOF_3DOF(int tag,
        int Nd1, int Nd2,
        double Knormal,
        double Ktangent,
        double frictionRatio,
        double x_local_1, double x_local_2, double x_local_3)
    : Element(tag, ELE_TAG_ContactElement_3DOF_3DOF),
      connectedExternalNodes(numberNodes),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes),
      normalforce(0), shearforce(2), global_gap(3), local_gap(3), stickPt(2), shear_relative_displacement(2),
      shearforce_committed(2), shearforce_trial(2), yield_criteria(2), signvalue(2)
{


    // ensure the connectedExternalNode ID is of correct size & set values
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ZeroLength::setUp - failed to create an ID of correct size\n";
    }

    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;

    // assign Kn, Kt, fs
    Kn = Knormal;
    Kt = Ktangent;
    fs = frictionRatio;


    // initialized contact flag and sliding flag to be zero
    ContactFlag = 0;
    SlidingFlag = 0;

    // vector components in global coordinates defining local x-axis of the element
    //   Vector x_local(3); //Moved by Babak to the h file 11/20/13
    x_local(0) = x_local_1;
    x_local(1) = x_local_2;
    x_local(2) = x_local_3;



    // set stick point cords in LOCAL basis
    stickPt(0) = 0;
    stickPt(1) = 0;



    for (int i = 0; i < 2; i++)
    {
        shearforce(i) = 0;
        shear_relative_displacement(i) = 0;
        yield_criteria(i) = 0;
        shearforce_trial(i) = 0;
        shearforce_committed(i) = 0;
        signvalue(i) = 0;
    }

    for (int i = 0; i < 3; i++)
    {
        global_gap(i) = 0;
        local_gap(i) = 0;
    }


    // defining the contact plane based on the local x axis
    definecontact = ContactPlane(x_local);

    if (definecontact != 0)
    {
        cerr << "\nProblem in Contact Plane ...";
    }

}


//null constructor
ContactElement_3DOF_3DOF::ContactElement_3DOF_3DOF(void)
    : Element(0, ELE_TAG_ContactElement_3DOF_3DOF),
      connectedExternalNodes(numberNodes),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes), stickPt(2), shear_relative_displacement(2),
      shearforce_committed(2), shearforce(2), global_gap(3), local_gap(3), shearforce_trial(2), yield_criteria(2), signvalue(2)
{

    // ensure the connectedExternalNode ID is of correct size
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ContactElement_3DOF_3DOF::ContactElement_3DOF_3DOF - failed to create an ID of correct size\n";
    }

    for (int j = 0; j < numberNodes; j++ )
    {
        nodePointers[j] = 0;
    }

    // initialized contact flag and sliding flag to be zero
    ContactFlag = 0;
    SlidingFlag = 0;

    // vector components in global coordinates defining local x-axis of the element
    x_local(0) = 0;
    x_local(1) = 0;
    x_local(2) = 0;

    // set stick point cords in LOCAL basis
    stickPt(0) = 0;
    stickPt(1) = 0;



    for (int i = 0; i < 2; i++)
    {
        shearforce(i) = 0;
        shear_relative_displacement(i) = 0;
        yield_criteria(i) = 0;
        shearforce_trial(i) = 0;
        shearforce_committed(i) = 0;
        signvalue(i) = 0;
    }

    for (int i = 0; i < 3; i++)
    {
        global_gap(i) = 0;
        local_gap(i) = 0;
    }
}


//  Destructor
ContactElement_3DOF_3DOF::~ContactElement_3DOF_3DOF()
{

}

// returning the number of the nodes for element
int
ContactElement_3DOF_3DOF::getNumExternalNodes(void) const
{
    return 2;
}

// returning the nodes of the elements
const ID &
ContactElement_3DOF_3DOF::getExternalNodes(void)
{
    return connectedExternalNodes;
}

// returning a pointer for nodes of the element
Node **
ContactElement_3DOF_3DOF::getNodePtrs(void)
{
    return nodePointers;
}

// returning the number of DOFs of the element
int
ContactElement_3DOF_3DOF::getNumDOF(void)
{
    return numDOF;
}


// method: setDomain()
//    to set a link to the enclosing Domain and to set the node pointers.
//    also determines the number of dof associated
//    with the ContactElement_3DOF_3DOF element
void
ContactElement_3DOF_3DOF::setDomain(Domain *theDomain)
{

    // check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        nodePointers[0] = 0;
        nodePointers[1] = 0;
        return;
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
            cerr << "WARNING ContactElement_3DOF_3DOF::setDomain() - Nd1: " << Nd1 << " does not exist in ";
        }
        else
        {
            cerr << "WARNING ContactElement_3DOF_3DOF::setDomain() - Nd2: " << Nd2 << " does not exist in ";
        }

        return;
    }

    // now determine the number of dof and the dimension
    int dofNd1 = nodePointers[0]->getNumberDOF();
    int dofNd2 = nodePointers[1]->getNumberDOF();

    // if differing dof at the ends - print a warning message
    if (dofNd1 != dofNd2)
    {
        cerr << "WARNING ContactElement_3DOF_3DOF::setDomain(): nodes " << Nd1 << " and " << Nd2 <<
             "have differing dof at ends for ContactElement_3DOF_3DOF " << this->getTag() << endln;
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
        cerr << "WARNING ContactElement_3DOF_3DOF::setDomain cannot handle " << dofNd1 <<
             "dofs at nodes in " << dofNd1 << " 3d problem\n";
        return;
    }
}


int
ContactElement_3DOF_3DOF::commitState()
{

    // need to update stick point here
    if (SlidingFlag == 1)   // update stick point only for slide case
    {
        stickPt = shear_relative_displacement;

        //   for (int i=0; i<2; i++) signvalue(i) *= -1;


        for (int i = 0; i < 2; i++)
        {
            if ( yield_criteria(i) > 0 )
            {
                shearforce_committed(i) = signvalue(i) * fs * fabs(normalforce);
            }
        }
    }

    //   else if (SlidingFlag == 0)
    //      {
    //        for (int i=0; i<2; i++)  stickPt(i) = 0;
    //      }


    //   RecordSlidingFlag = 1;

    //   stiff = this->getTangentStiff();
    //   resid = this->getResistingForce();

    return 0;
}


int
ContactElement_3DOF_3DOF::revertToLastCommit()
{
    shear_relative_displacement = stickPt;

    return 0;
}


int
ContactElement_3DOF_3DOF::revertToStart()
{
    stickPt.Zero();

    return 0;
}


const Matrix &
ContactElement_3DOF_3DOF::getDamp(void)
{
    // no mass
    zeroMatrix.Zero();
    return zeroMatrix;
}


const Matrix &
ContactElement_3DOF_3DOF::getMass(void)
{
    // no mass
    zeroMatrix.Zero();
    return zeroMatrix;
}


void
ContactElement_3DOF_3DOF::zeroLoad(void)
{
    // does nothing
}

int
ContactElement_3DOF_3DOF::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    // meaningless to addLoad to a contact
    return 0;
}

int
ContactElement_3DOF_3DOF::addInertiaLoadToUnbalance(const Vector &accel)
{
    // does nothing as element has no mass
    return 0;
}




int ContactElement_3DOF_3DOF::if_nodes_are_in_contact_or_not(void)
{

    Vector slaveNode_location(3);
    Vector masterNode_location(3);

    //   slaveNode_location  = nodePointers[0]->getCrds() + nodePointers[0]->getTrialDisp();
    //   masterNode_location = nodePointers[1]->getCrds() + nodePointers[1]->getTrialDisp();

    slaveNode_location  = nodePointers[0]->getTrialDisp();
    masterNode_location = nodePointers[1]->getTrialDisp();


    //    slaveNode_location=nodePointers[0]->getIncrDisp();
    //    masterNode_location=nodePointers[1]->getIncrDisp();

    //    slaveNode_location=nodePointers[0]->getIncrDeltaDisp();
    //    masterNode_location=nodePointers[1]->getIncrDeltaDisp();




    global_gap = masterNode_location - slaveNode_location;



    local_gap = transformation * global_gap;

    cerr << "\nGap between nodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " is " << local_gap(0) << endln;

    /*  cerr<< "N 1 = " << N(0) << endln;
        cerr<< "N 2 = " << N(1) << endln;
        cerr<< "N 3 = " << N(2) << endln;
        cerr<< "N 4 = " << N(3) << endln;
        cerr<< "N 5 = " << N(4) << endln;
        cerr<< "N 6 = " << N(5) << endln;


        cerr<< "T1 1 = " << T1(0) << endln;
        cerr<< "T1 2 = " << T1(1) << endln;
        cerr<< "T1 3 = " << T1(2) << endln;
        cerr<< "T1 4 = " << T1(3) << endln;
        cerr<< "T1 5 = " << T1(4) << endln;
        cerr<< "T1 6 = " << T1(5) << endln;

        cerr<< "T2 1 = " << T2(0) << endln;
        cerr<< "T2 2 = " << T2(1) << endln;
        cerr<< "T2 3 = " << T2(2) << endln;
        cerr<< "T2 4 = " << T2(3) << endln;
        cerr<< "T2 5 = " << T2(4) << endln;
        cerr<< "T2 6 = " << T2(5) << endln;*/


    if (local_gap(0) > 0.0)
    {
        // Not in contact
        cerr << "Nodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " are NOT in Contact ..." << endln;
        return 0;
    }
    else     // contact occur
    {
        cerr << "Nodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " are in Contact ..." << endln;
        return 1;
    }
}



int ContactElement_3DOF_3DOF::stick_or_slide(void)
{

    Vector DispTrialSlave(3); // trial disp for slave node
    Vector DispTrialMaster(3); // trial disp for master node

    //    Vector DispIncrSlave(3); // incremental disp for slave node
    //    Vector DispIncrMaster(3); // incremental disp for master node


    int i;


    normalforce = Kn * local_gap(0); // Kn : normal penalty
    //    cerr<< "Normal Force = " << fabs(normalforce) << endln;
    cerr << "Normal Force = " << (normalforce) << endln;

    DispTrialSlave = nodePointers[0]->getTrialDisp();
    DispTrialMaster = nodePointers[1]->getTrialDisp();

    //    DispIncrSlave=nodePointers[0]->getIncrDisp();
    //    DispIncrMaster=nodePointers[1]->getIncrDisp();

    //    DispTrialSlave=nodePointers[0]->getIncrDeltaDisp();
    //    DispTrialMaster=nodePointers[1]->getIncrDeltaDisp();

    cerr << "Trial Disp Node 1 = " << DispTrialSlave(1) << endln;
    cerr << "Trial Disp Node 2 = " << DispTrialMaster(1) << endln;

    //  cerr<< "Incr Disp Node 1 = " << DispTrialSlave(1) << endln;
    //  cerr<< "Incr Disp Node 2 = " << DispTrialMaster(1) << endln;



    double DispTrial[6];
    //    DispTrial[0]=DispTrialSlave(0);
    //    DispTrial[1]=DispTrialSlave(1);
    //    DispTrial[2]=DispTrialSlave(2);
    //    DispTrial[3]=DispTrialMaster(0);
    //    DispTrial[4]=DispTrialMaster(1);
    //    DispTrial[5]=DispTrialMaster(2);
    DispTrial[0] = DispTrialMaster(0);
    DispTrial[1] = DispTrialMaster(1);
    DispTrial[2] = DispTrialMaster(2);
    DispTrial[3] = DispTrialSlave(0);
    DispTrial[4] = DispTrialSlave(1);
    DispTrial[5] = DispTrialSlave(2);



    shearforce.Zero();
    shear_relative_displacement.Zero();

    for (i = 0; i < 6; i++)
    {
        //    shear_relative_displacement(0) -= T1(i)*DispTrial[i];
        //    shear_relative_displacement(1) -= T2(i)*DispTrial[i];
        shear_relative_displacement(0) += T1(i) * DispTrial[i];
        shear_relative_displacement(1) += T2(i) * DispTrial[i];
    }


    ///////////////////////////////////////////////////////////////////////
    //    double temp_shear_relative_displacement[2];
    //    temp_shear_relative_displacement[0]=shear_relative_displacement(0);
    //    temp_shear_relative_displacement[1]=shear_relative_displacement(1);
    ///////////////////////////////////////////////////////////////////////


    // Compute trial shear force
    for (i = 0; i < 2; i++)
    {
        shearforce(i) = Kt * ( shear_relative_displacement(i) - stickPt(i) );    //Kt: tangential penalty
    }


    //     if (SlidingFlag == 0)
    //     {
    for (int j = 0; j < 2; j++)
    {
        if ( shearforce_trial(j) >= 0)
        {
            signvalue(j) = 1;
        }

        if ( shearforce_trial(j) < 0)
        {
            signvalue(j) = -1;
        }
    }

    //     }


    for (i = 0; i < 2; i++)
    {
        //      if ( shearforce(i) < 0 )
        //      {
        //        shearforce_trial(i) = shearforce_committed(i) - signvalue(i) * shearforce(i);
        //      } else if ( shearforce(i) >=0)
        //      {
        shearforce_trial(i) = shearforce_committed(i) + shearforce(i);
    }


    shear_force_norm = shearforce.Norm();
    shear_force_trial_norm = shearforce_trial.Norm();


    //    shearforce_trial = fabs(shearforce_committed) + shear_force_norm;  //Kt: tangential penalty


    //    yield_criteria = shear_force_norm - (fs * fabs(normalforce));
    //    yield_criteria = shearforce_trial - (fs * fabs(normalforce));

    for (int j = 0; j < 2; j++)
    {
        yield_criteria(j) = fabs(shearforce_trial(j)) - (fs * fabs(normalforce));
    }

    cerr << "normalforce:" << normalforce << "\n";
    //       yield_criteria(0) = fabs(shearforce_trial(0)) - (fs * fabs(normalforce));
    //       yield_criteria(1) = fabs(shearforce_trial(1)) - (fs * fabs(normalforce));



    cerr << "Relative Shear Disp T1 = " << shear_relative_displacement(0) << endln;
    cerr << "Relative Shear Disp T2 = " << shear_relative_displacement(1) << endln;
    cerr << "Stick 0 = " << stickPt(0) << endln;
    cerr << "Stick 1 = " << stickPt(1) << endln;
    cerr << "Shear Force 1 = " << (shearforce(0)) << endln;
    cerr << "Shear Force 2 = " << (shearforce(1)) << endln;
    cerr << "Shearforce Committed 1 = " << shearforce_committed(0) << endln;
    cerr << "Shearforce Committed 2 = " << shearforce_committed(1) << endln;
    cerr << "Sign value 1 = " << signvalue(0) << endln;
    cerr << "Sign value 2 = " << signvalue(1) << endln;
    cerr << "Shearforce Trial 1 = " << shearforce_trial(0) << endln;
    cerr << "Shearforce Trial 2 = " << shearforce_trial(1) << endln;
    cerr << "yield_criteria (1) = " << yield_criteria(0) << endln;
    cerr << "yield_criteria (2) = " << yield_criteria(1) << endln;


    //    if (yield_criteria <= 0)
    if (yield_criteria(0) <= 0 && yield_criteria(1) <= 0)
    {
        // stick
        cerr << "Sticking ..." << endln;
        //        RecordSlidingFlag = 0;
        return 0;
    }
    else
    {
        // slide
        cerr << "Sliding ..." << endln;

        //        if ( yield_criteria(0) > 0 )  = ;
        //        if ( yield_criteria(1) > 0 )  = ;

        //        if ( yield_criteria(0) > 0 ) signvalue(0) *= -1;
        //        if ( yield_criteria(1) > 0 ) signvalue(1) *= -1;
        //        RecordSlidingFlag = 1;
        return 1;
    }



}




const Matrix &
ContactElement_3DOF_3DOF::getTangentStiff(void)
{

    ContactFlag = if_nodes_are_in_contact_or_not();
    int i , j;
    stiff.Zero( ) ;


    if (ContactFlag == 1) // in contact
    {
        SlidingFlag = stick_or_slide();

        if (SlidingFlag == 0) // sticking
        {
            for (i = 0; i < 6; i++)
            {
                for (j = 0; j < 6; j++)
                {
                    stiff(i, j) = Kn * (N(i) * N(j)) + Kt * (T1(i) * T1(j) + T2(i) * T2(j));
                }
            }
        }


        else if (SlidingFlag == 1) // sliding
        {


            //  double shear_ratio1=shearforce(0)/shear_force_trial_norm;
            //  double shear_ratio2=shearforce(1)/shear_force_trial_norm;
            //  double shear_ratio1=shearforce(0)/shear_force_norm;
            //  double shear_ratio2=shearforce(1)/shear_force_norm;
            double shear_ratio1 = shearforce_trial(0) / shear_force_trial_norm;
            double shear_ratio2 = shearforce_trial(1) / shear_force_trial_norm;
            //  double coefficient1=fs*Kn;
            //  double coefficient2=Kt*(-fs*normalforce)/shear_force_norm;
            double coefficient1 = fs * Kn * Kt;
            double coefficient2 = Kt;
            cerr << "shear force norm = " << shear_force_norm << endln;
            cerr << "shear_ratio1 = " << shear_ratio1 << endln;
            cerr << "shear_ratio2 = " << shear_ratio2 << endln;


            for (i = 0; i < 6; i++)
            {
                for (j = 0; j < 6; j++)
                {
                    stiff(i, j) = Kn * (N(i) * N(j)) + coefficient1 * (shear_ratio1 * T1(i) * N(j) + shear_ratio2 * T2(i) * N(j))
                                  + coefficient2 * ( (1 - shear_ratio1 * shear_ratio1) * T1(i) * T1(j) - shear_ratio1 * shear_ratio2 * T1(i) * T2(j)
                                                     - shear_ratio1 * shear_ratio2 * T2(i) * T1(j) + (1 - shear_ratio2 * shear_ratio2) * T2(i) * T2(j));
                }
            }



            //   for (i=0; i<6; i++) {
            //  for (j=0; j<6; j++) {
            //       stiff(i,j) = Kn*(N(i)*N(j)) + coefficient1*(shear_ratio1*T1(i)*N(j)+shear_ratio2*T2(i)*N(j))
            //          + coefficient2*( (1-shear_ratio1*shear_ratio1)*T1(i)*T1(j) + (1-shear_ratio2*shear_ratio2)*T2(i)*T2(j));
            //                      }
            //                       }
            //
            // //   stiff(1,4) = 0;
            // //   stiff(4,1) = 0;
            //  stiff(1,1) = 0;
            //  stiff(4,4) = 0;



            //  double shear_ratio1=shearforce(0)/shear_force_norm;
            //  double shear_ratio2=shearforce(1)/shear_force_norm;
            //  cerr<< "shear_ratio1 = " << shear_ratio1 << endln;
            //  cerr<< "shear_ratio2 = " << shear_ratio2 << endln;
            //
            //   for (i=0; i<6; i++) {
            //  for (j=0; j<6; j++) {
            //       stiff(i,j) = Kn*(N(i)*N(j)) + fs*Kn*(shear_ratio1*T1(i)*N(j)+shear_ratio2*T2(i)*N(j))
            //          + Kt*( (1-shear_ratio1*shear_ratio1)*T1(i)*T1(j) + (1-shear_ratio2*shear_ratio2)*T2(i)*T2(j));
            //                      }
            //                       }





            //  double shear_ratio1=shearforce_trial(0)/shear_force_norm;
            //  double shear_ratio2=shearforce_trial(1)/shear_force_norm;
            // //   double coefficient1=fs*Kn*Kt;
            // //   double coefficient2=Kt*(-fs*normalforce)/shear_force_norm;
            //  cerr<< "shear force norm = " << shear_force_norm << endln;
            //  cerr<< "shear_ratio1 = " << shear_ratio1 << endln;
            //  cerr<< "shear_ratio2 = " << shear_ratio2 << endln;
            //
            //
            //   for (i=0; i<6; i++) {
            //  for (j=0; j<6; j++) {
            //       stiff(i,j) = Kn*(N(i)*N(j)) + fs*Kn*Kt*(shear_ratio1*T1(i)*N(j)+shear_ratio2*T2(i)*N(j))
            //          + Kt*( (1-shear_ratio1*shear_ratio1)*T1(i)*T1(j) - shear_ratio1*shear_ratio2 *T1(i)*T2(j)
            //              - shear_ratio1*shear_ratio2 *T2(i)*T1(j) + (1-shear_ratio2*shear_ratio2)*T2(i)*T2(j));
            //                      }
            //                       }



            //  double shear_ratio1, shear_ratio2;
            //  shear_ratio1=shearforce_trial(0)/shear_force_norm;
            //  shear_ratio2=shearforce_trial(1)/shear_force_norm;
            //  double coefficient1=fs*Kn;
            //  double coefficient2=Kt*(-fs*normalforce)/shear_force_norm;
            //
            //   for (i=0; i<6; i++) {
            //  for (j=0; j<6; j++) {
            //       stiff(i,j) = Kn*(N(i)*N(j)) - coefficient1*(shear_ratio1*T1(i)*N(j)+shear_ratio2*T2(i)*N(j))
            //          + coefficient2*( (1-shear_ratio1*shear_ratio1)*T1(i)*T1(j) - shear_ratio1*shear_ratio2 *T1(i)*T2(j)
            //              - shear_ratio1*shear_ratio2 *T2(i)*T1(j) + (1-shear_ratio1*shear_ratio2)*T2(i)*T2(j));
            //                      }
            //                       }



            //  double shear_ratio1, shear_ratio2;
            //  shear_ratio1=shearforce_trial(0)/shear_force_norm;
            //  shear_ratio2=shearforce_trial(1)/shear_force_norm;
            //
            //  cerr<< "shear_ratio1 = " << shear_ratio1 << endln;
            //  cerr<< "shear_ratio2 = " << shear_ratio2 << endln;
            //
            //
            //  shear_ratio1=shearforce_trial(0)/shear_force_norm;
            //  shear_ratio2=shearforce_trial(1)/shear_force_norm;
            //  double coefficient1=fs*Kn;
            //  double coefficient2=Kt*(-fs*normalforce)/shear_force_norm;
            //
            //   for (i=0; i<6; i++) {
            //  for (j=0; j<6; j++) {
            //       stiff(i,j) = Kn*(N(i)*N(j)) - coefficient1 * (shear_ratio1*T1(i)*N(j) + shear_ratio2*T2(i)*N(j))
            //          + coefficient2 * ( (1-shear_ratio1*shear_ratio1)*T1(i)*T1(j) + (1-shear_ratio2*shear_ratio2)*T2(i)*T2(j)
            //                            - shear_ratio1*shear_ratio2 *T1(i)*T2(j) - shear_ratio1*shear_ratio2 *T2(i)*T1(j) );
            //                      }
            //                       }




            //  double shear_ratio1, shear_ratio2;
            //  shear_ratio1=shearforce_trial(0)/shear_force_norm;
            //  shear_ratio2=shearforce_trial(1)/shear_force_norm;
            //
            //  cerr<< "shear_ratio1 = " << shear_ratio1 << endln;
            //  cerr<< "shear_ratio2 = " << shear_ratio2 << endln;
            //
            //  double coefficient1=fs*Kn;
            //  double coefficient2=Kt*(-fs*normalforce)/shear_force_norm;
            //
            //
            //         for (i=0; i<6; i++)
            //       {
            //          for (j=0; j<6; j++)
            //           {
            //                   stiff(i,j) = 0;
            //               }
            //               }
            //
            //
            //  stiff(0,0) = Kn;
            //  stiff(0,1) = 0;
            //  stiff(0,2) = 0;
            //  stiff(0,3) = -Kn;
            //  stiff(0,4) = 0;
            //  stiff(0,5) = 0;
            //
            //  stiff(1,0) = fs * Kn;
            //  stiff(1,1) = Kt;
            //  stiff(1,2) = 0;
            //  stiff(1,3) = -fs * Kn;
            //  stiff(1,4) = 0;
            //  stiff(1,5) = 0;
            //
            //  stiff(2,0) = 0;
            //  stiff(2,1) = 0;
            //  stiff(2,2) = Kt;
            //  stiff(2,3) = 0;
            //  stiff(2,4) = 0;
            //  stiff(2,5) = -Kt;
            //
            //  stiff(3,0) = -Kn;
            //  stiff(3,1) = 0;
            //  stiff(3,2) = 0;
            //  stiff(3,3) = Kn;
            //  stiff(3,4) = 0;
            //  stiff(3,5) = 0;
            //
            //  stiff(4,0) = -fs * Kn;
            //  stiff(4,1) = 0;
            //  stiff(4,2) = 0;
            //  stiff(4,3) = fs * Kn;
            //  stiff(4,4) = Kt;
            //  stiff(4,5) = 0;
            //
            //  stiff(5,0) = 0;
            //  stiff(5,1) = 0;
            //  stiff(5,2) = -Kt;
            //  stiff(5,3) = 0;
            //  stiff(5,4) = 0;
            //  stiff(5,5) = Kt;



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


    ///////////////////////////////////////////////////////////////////////

    //    double contactvector_N[6];
    //    contactvector_N[0]=N(0);
    //    contactvector_N[1]=N(1);
    //    contactvector_N[2]=N(2);
    //    contactvector_N[3]=N(3);
    //    contactvector_N[4]=N(4);
    //    contactvector_N[5]=N(5);
    //

    //    double contactvector_T[6];
    //    contactvector_T[0]=T1(0);
    //    contactvector_T[1]=T1(1);
    //    contactvector_T[2]=T1(2);
    //    contactvector_T[3]=T1(3);
    //    contactvector_T[4]=T1(4);
    //    contactvector_T[5]=T1(5);
    //
    //      for (int m=0; m<6; m++)
    //      {
    //       cout << "T1(" << m << "):" << contactvector_T[m] << endl;
    //      }

    //    double temp_stiff[36];
    //    temp_stiff[0]=stiff(0,0);
    //    temp_stiff[1]=stiff(0,1);


    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            cout << "K(" << i + 1 << "," << j + 1 << "):" << stiff(i, j) << "   ";
        }

        cout << endl;
    }

    ///////////////////////////////////////////////////////////////////////


    return stiff ;
}


const Matrix &
ContactElement_3DOF_3DOF::getInitialStiff(void)
{

    getTangentStiff();

    return stiff ;
}


const Vector &
ContactElement_3DOF_3DOF::getResistingForce()
{

    ContactFlag = if_nodes_are_in_contact_or_not();

    int i;
    resid.Zero( ) ;

    if (ContactFlag == 1) // in contact
    {
        SlidingFlag = stick_or_slide();

        if (SlidingFlag == 0) // stick
        {
            for (i = 0; i < 6; i++)
                //           resid(i) = (-normalforce)*N(i) + shearforce(0)*T1(i) + shearforce(1)*T2(i);
            {
                resid(i) = (-normalforce) * N(i) + shearforce_trial(0) * T1(i) + shearforce_trial(1) * T2(i);
            }
        }


        else if (SlidingFlag == 1) // slide
        {
            double shear_slide_1, shear_slide_2;
            //         shear_slide_1 = (-fs*normalforce) * shearforce(0)/shear_force_norm;
            //         shear_slide_2 = (-fs*normalforce) * shearforce(1)/shear_force_norm;
            shear_slide_1 = (-fs * normalforce) * shearforce_trial(0) / shear_force_trial_norm;
            shear_slide_2 = (-fs * normalforce) * shearforce_trial(1) / shear_force_trial_norm;

            for (i = 0; i < 6; i++)
            {
                resid(i) = (-normalforce) * N(i) + shear_slide_1 * T1(i) + shear_slide_2 * T2(i) ;
            }
        }
    }


    else if (ContactFlag == 0)  // not in contact
    {

        for (i = 0; i < 6; i++)
        {
            resid(i) = 0.0;
        }
    }



    return resid ;
}



const Vector &
ContactElement_3DOF_3DOF::getLocalResistingForce()
{
    //need to be fixed
    ContactFlag = if_nodes_are_in_contact_or_not();

    int i;
    resid.Zero( ) ;

    if (ContactFlag == 1) // contacted
    {
        SlidingFlag = stick_or_slide();

        if (SlidingFlag == 0) // stick
        {
            for (i = 0; i < 6; i++)
            {
                localresid(0) = -normalforce;
            }

            localresid(1) = -shearforce(0);
            localresid(2) =  shearforce(1);
            localresid(3) =  normalforce;
            localresid(4) =  shearforce(0);
            localresid(5) = -shearforce(1);
        }
        else if (SlidingFlag == 1) // slide
        {
            double shear_slide_1, shear_slide_2;
            shear_slide_1 = -(fs * normalforce) * shearforce(0) / shear_force_norm;
            shear_slide_2 = -(fs * normalforce) * shearforce(1) / shear_force_norm;

            for (i = 0; i < 6; i++)
            {
                localresid(0) = -normalforce;
            }

            localresid(1) =  -shear_slide_1;
            localresid(2) =  shear_slide_2;
            localresid(3) =  normalforce;
            localresid(4) =  shear_slide_1;
            localresid(5) =  -shear_slide_2;
        }
    }
    else if (ContactFlag == 0)  // not contacted
    {

        for (i = 0; i < 6; i++)
        {
            localresid(i) = 0.0;
        }
    }


    return localresid ;
}






const Vector &
ContactElement_3DOF_3DOF::getResistingForceIncInertia()
{
    return this->getResistingForce();
}





void
ContactElement_3DOF_3DOF::Print(ostream &s, int flag)
{
    if (flag == 0)   // print everything
    {
        s << "Element: " << this->getTag();
        s << " type: ContactElement_3DOF_3DOF  iNode: " << connectedExternalNodes(0);
        s << " jNode: " << connectedExternalNodes(1) << endln;
    }
    else if (flag == 1)
    {
        s << this->getTag() << endln;
    }
}




// Response*
// ContactElement_3DOF_3DOF::setResponse(const char** argv, int argc, Information& eleInformation)
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
// ContactElement_3DOF_3DOF::getResponse(int responseID, Information& eleInfo)
// {
//     if (responseID == 1)
//     {
//         //       return eleInfo.setVector(this->getResistingForce());
//         return eleInfo.setVector(resid);
//     }

//     else if (responseID == 2)
//         //       return eleInfo.setMatrix(this->getTangentStiff());
//     {
//         return eleInfo.setMatrix(stiff);
//     }

//     else if (responseID == 3)
//         //       return eleInfo.setVector(this->getLocalResistingForce());
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

//         //     slaveNode_displacement=nodePointers[0]->getIncrDisp();
//         //     masterNode_displacement=nodePointers[1]->getIncrDisp();

//         //    slaveNode_displacement=nodePointers[0]->getIncrDeltaDisp();
//         //    masterNode_displacement=nodePointers[1]->getIncrDeltaDisp();


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
//         //        contactsituation(1) = RecordSlidingFlag;

//         return eleInfo.setVector(contactsituation);
//     }

//     else if (responseID == 6)
//     {
//         static Vector RelativeDisplacement(3);
//         RelativeDisplacement(0) = normalforce   / Kn;
//         RelativeDisplacement(1) = shearforce(0) / Kt;
//         RelativeDisplacement(2) = shearforce(1) / Kt;

//         //        contactsituation(1) = RecordSlidingFlag;

//         return eleInfo.setVector(RelativeDisplacement);
//     }

//     else
//     {
//         return -1;
//     }
// }





int
ContactElement_3DOF_3DOF::ContactPlane(const Vector &x_local)
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

    ///////////////////////////////////////////////////////////////////////
    /*
      double contactvector_N[6];
      contactvector_N[0]=N(0);
      contactvector_N[1]=N(1);
      contactvector_N[2]=N(2);
      contactvector_N[3]=N(3);
      contactvector_N[4]=N(4);
      contactvector_N[5]=N(5);

      double contactvector_T1[6];
      contactvector_T1[0]=T1(0);
      contactvector_T1[1]=T1(1);
      contactvector_T1[2]=T1(2);
      contactvector_T1[3]=T1(3);
      contactvector_T1[4]=T1(4);
      contactvector_T1[5]=T1(5);

      double contactvector_T2[6];
      contactvector_T2[0]=T2(0);
      contactvector_T2[1]=T2(1);
      contactvector_T2[2]=T2(2);
      contactvector_T2[3]=T2(3);
      contactvector_T2[4]=T2(4);
      contactvector_T2[5]=T2(5);
    */
    ///////////////////////////////////////////////////////////////////////


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
//implemented by Babak Kamrani on 10/30/2013
int
ContactElement_3DOF_3DOF::sendSelf(int commitTag, Channel &theChannel)
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
        std::cerr << "WARNING ContactElement_3DOF_3DOF::sendSelf() - " << this->getTag() << " failed to send idData\n";
        return res;
    }

    static Vector VectorData(32);

    VectorData(0) = Kn;
    VectorData(1) = Kt;
    VectorData(2) = fs;

    VectorData(3) = y_local(0);
    VectorData(4) = y_local(1);
    VectorData(5) = y_local(2);

    VectorData(6) = z_local(0);
    VectorData(7) = z_local(1);
    VectorData(8) = z_local(2);

    VectorData(9)  = stickPt(0);
    VectorData(10) = stickPt(1);

    VectorData(11)  = shearforce(0);
    VectorData(12)  = shearforce(1);

    VectorData(13)  = shear_relative_displacement(0);
    VectorData(14)  = shear_relative_displacement(1);

    VectorData(15)  = yield_criteria(0);
    VectorData(16)  = yield_criteria(1);

    VectorData(17)  = shearforce_trial(0);
    VectorData(18)  = shearforce_trial(1);

    VectorData(19)  = shearforce_committed(0);
    VectorData(20)  = shearforce_committed(1);

    VectorData(21)  = signvalue(0);
    VectorData(22)  = signvalue(1);



    VectorData(23)  = global_gap(0);
    VectorData(24)  = global_gap(1);
    VectorData(25)  = global_gap(2);

    VectorData(26)  = local_gap(0);
    VectorData(27)  = local_gap(1);
    VectorData(28)  = local_gap(2);

    VectorData(29)  = x_local(0);
    VectorData(30)  = x_local(1);
    VectorData(31)  = x_local(2);

    res = theChannel.sendVector(dataTag, commitTag, VectorData);

    if (res < 0)
    {
        cerr << "WARNING ContactElement_3DOF_3DOF::sendSelf() - " << this->getTag() << " failed to send VectorData\n";
        return res;
    }

    return res;
}


//=============================================================================
//implemented by Babak Kamrani on 10/30/2013
int
ContactElement_3DOF_3DOF::recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    int res = 0;

    int dataTag = this->getDbTag();

    static ID idData(6);
    res = theChannel.recvID(dataTag, commitTag, idData);

    if (res < 0)
    {
        std::cerr << "WARNING ContactElement_3DOF_3DOF::recvSelf() - " << dataTag << " failed to receive idData\n";
        return res;
    }

    this->setTag(idData(0));
    connectedExternalNodes(0) = idData(1);
    connectedExternalNodes(1) = idData(2);
    ContactFlag = idData(3);
    SlidingFlag = idData(4);
    definecontact = idData(5);

    static Vector VectorData(32);
    res = theChannel.recvVector(this->getDbTag(), commitTag, VectorData);

    if (res < 0)
    {
        std::cerr << "WARNING ContactElement_3DOF_3DOF::recvSelf() - " << dataTag << " failed to receive VectorData\n";
        return res;
    }


    Kn = VectorData(0);
    Kt = VectorData(1);
    fs = VectorData(2);

    y_local(0) = VectorData(3);
    y_local(1) = VectorData(4);
    y_local(2) = VectorData(5);

    z_local(0) = VectorData(6);
    z_local(1) = VectorData(7);
    z_local(2) = VectorData(8);

    stickPt(0) = VectorData(9);
    stickPt(1) = VectorData(10);

    shearforce(0) = VectorData(11);
    shearforce(1) = VectorData(12);

    shear_relative_displacement(0) = VectorData(13);
    shear_relative_displacement(1) = VectorData(14);

    yield_criteria(0) = VectorData(15);
    yield_criteria(1) = VectorData(16);

    shearforce_trial(0) = VectorData(17);
    shearforce_trial(1) = VectorData(18);

    shearforce_committed(0) = VectorData(19);
    shearforce_committed(1) = VectorData(20);

    signvalue(0) = VectorData(21);
    signvalue(1) = VectorData(22);

    global_gap(0) = VectorData(23);
    global_gap(1) = VectorData(24);
    global_gap(2) = VectorData(25);

    local_gap(0) = VectorData(26);
    local_gap(1) = VectorData(27);
    local_gap(2) = VectorData(28);

    x_local(0)   = VectorData(29);
    x_local(1)   = VectorData(30);
    x_local(2)   = VectorData(31);

    // defining the contact plane based on the local x axis
    definecontact = ContactPlane(x_local);

    if (definecontact != 0)
    {
        cerr << "\nProblem in Contact Plane ...";
    }

    return res;
}
//=============================================================================