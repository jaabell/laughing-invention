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
// DATE:              November 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

// This contact element is the modified and generalized form of the
// Gang Wang's ZeroLengthContact3D element in OpenSees.

// Firs node should belong to solid element and 2nd node should belong to beam element.

#include "ContactElement_3DOF_6DOF.h"
#include <Information.h>
#include <Domain.h>
#include <Node.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
// #include <ElementResponse.h>


const int ContactElement_3DOF_6DOF::numberNodes = 2 ;
Matrix  ContactElement_3DOF_6DOF::stiff(9, 9);
Vector  ContactElement_3DOF_6DOF::resid(9);
Matrix  ContactElement_3DOF_6DOF::zeroMatrix(9, 9);
Vector ContactElement_3DOF_6DOF::y_local(3);
Vector ContactElement_3DOF_6DOF::z_local(3);
Vector ContactElement_3DOF_6DOF::x_local_normalized(3);
Vector ContactElement_3DOF_6DOF::y_local_normalized(3);
Vector ContactElement_3DOF_6DOF::z_local_normalized(3);
double ContactElement_3DOF_6DOF::normalforce;
Vector ContactElement_3DOF_6DOF::shearforce(2);
Matrix ContactElement_3DOF_6DOF::transformation(3, 3);
Vector ContactElement_3DOF_6DOF::global_gap(3);
Vector ContactElement_3DOF_6DOF::local_gap(3);
Vector ContactElement_3DOF_6DOF::shear_relative_displacement(2);

//*********************************************************************
//  Constructor for defining the contact element

ContactElement_3DOF_6DOF::ContactElement_3DOF_6DOF(int tag,
        int Nd1, int Nd2,
        double Knormal,
        double Ktangent,
        double frictionRatio,
        double c,
        double x_local_1, double x_local_2, double x_local_3)
    : Element(tag, ELE_TAG_ContactElement_3DOF_6DOF),
      connectedExternalNodes(numberNodes),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes)
{


    // ensure the connectedExternalNode ID is of correct size & set values
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ZeroLength::setUp - failed to create an ID of correct size\n";
    }

    connectedExternalNodes(0) = Nd1;
    connectedExternalNodes(1) = Nd2;

    // assign Kn, Kt, fs, cohesion
    Kn = Knormal;
    Kt = Ktangent;
    fs = frictionRatio;
    cohesion = c;

    // initialized contact flag and sliding flag to be zero
    ContactFlag = 0;
    SlidingFlag = 0;

    // vector components in global coordinates defining local x-axis of the element
    Vector x_local(3);
    x_local(0) = x_local_1;
    x_local(1) = x_local_2;
    x_local(2) = x_local_3;

    // defining the contact plane based on the local x axis
    definecontact = ContactPlane(x_local);

    if (definecontact != 0)
    {
        cerr << "\nProblem in Contact Plane ...";
    }

}


//null constructor
ContactElement_3DOF_6DOF::ContactElement_3DOF_6DOF(void)
    : Element(0, ELE_TAG_ContactElement_3DOF_6DOF),
      connectedExternalNodes(numberNodes),
      N(3 * numberNodes), T1(3 * numberNodes), T2(3 * numberNodes)
{

    // ensure the connectedExternalNode ID is of correct size
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL ContactElement_3DOF_6DOF::ContactElement_3DOF_6DOF - failed to create an ID of correct size\n";
    }

    for (int j = 0; j < numberNodes; j++ )
    {
        nodePointers[j] = 0;
    }
}


//  Destructor
ContactElement_3DOF_6DOF::~ContactElement_3DOF_6DOF()
{

}

// returning the number of the nodes for element
int
ContactElement_3DOF_6DOF::getNumExternalNodes(void) const
{
    return 2;
}

// returning the nodes of the elements
const ID &
ContactElement_3DOF_6DOF::getExternalNodes(void)
{
    return connectedExternalNodes;
}

// returning a pointer for nodes of the element
Node **
ContactElement_3DOF_6DOF::getNodePtrs(void)
{
    return nodePointers;
}

// returning the number of DOFs of the element
int
ContactElement_3DOF_6DOF::getNumDOF(void)
{
    return numDOF;
}


// method: setDomain()
//    to set a link to the enclosing Domain and to set the node pointers.
//    also determines the number of dof associated
//    with the ContactElement_3DOF_6DOF element
void
ContactElement_3DOF_6DOF::setDomain(Domain *theDomain)
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
            cerr << "WARNING ContactElement_3DOF_6DOF::setDomain() - Nd1: " << Nd1 << " does not exist in ";
        }
        else
        {
            cerr << "WARNING ContactElement_3DOF_6DOF::setDomain() - Nd2: " << Nd2 << " does not exist in ";
        }

        return;
    }

    // now determine the number of dof and the dimension
    int dofNd1 = nodePointers[0]->getNumberDOF();
    int dofNd2 = nodePointers[1]->getNumberDOF();


    // call the base class method
    this->DomainComponent::setDomain(theDomain);

    if (dofNd1 == 3 && dofNd2 == 6)
    {
        numDOF = 9;
    }
    else
    {
        cerr << "WARNING ContactElement_3DOF_6DOF::setDomain cannot handle " << dofNd1 <<
             "dofs at nodes in " << dofNd1 << " d problem\n";
        return;
    }
}


int
ContactElement_3DOF_6DOF::commitState()
{
    return 0;
}


int
ContactElement_3DOF_6DOF::revertToLastCommit()
{
    return 0;
}


int
ContactElement_3DOF_6DOF::revertToStart()
{
    return 0;
}


const Matrix &
ContactElement_3DOF_6DOF::getDamp(void)
{
    // no mass
    zeroMatrix.Zero();
    return zeroMatrix;
}


const Matrix &
ContactElement_3DOF_6DOF::getMass(void)
{
    // no mass
    zeroMatrix.Zero();
    return zeroMatrix;
}


void
ContactElement_3DOF_6DOF::zeroLoad(void)
{
    // does nothing
}

int
ContactElement_3DOF_6DOF::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    // meaningless to addLoad to a contact
    return 0;
}

int
ContactElement_3DOF_6DOF::addInertiaLoadToUnbalance(const Vector &accel)
{
    // does nothing as element has no mass
    return 0;
}



const Matrix &
ContactElement_3DOF_6DOF::getTangentStiff(void)
{

    ContactFlag = if_nodes_are_in_contact_or_not();
    int i , j;
    stiff.Zero( ) ;


    if (ContactFlag == 1) // contacted
    {
        SlidingFlag = stick_or_slide();

        if (SlidingFlag == 0)
        {
            for (i = 0; i < 6; i++)
            {
                for (j = 0; j < 6; j++)
                {
                    stiff(i, j) = Kn * (N(i) * N(j)) + Kt * (T1(i) * T1(j) + T2(i) * T2(j));
                }
            }


        }

        else if (SlidingFlag == 1)
        {

            double shear_ratio1, shear_ratio2;
            shear_ratio1 = shearforce(0) / shear_force_norm;
            shear_ratio2 = shearforce(1) / shear_force_norm;
            double coefficient1 = fs * Kn;
            double coefficient2 = Kt * (fs * normalforce + cohesion) / shear_force_norm;


            for (i = 0; i < 6; i++)
            {
                for (j = 0; j < 6; j++)
                {
                    stiff(i, j) = Kn * (N(i) * N(j)) - coefficient1 * (shear_ratio1 * T1(i) * N(j) + shear_ratio2 * T2(i) * N(j))
                                  + coefficient2 * ( (1 - shear_ratio1 * shear_ratio1) * T1(i) * T1(j) - shear_ratio1 * shear_ratio2 * T1(i) * T2(j)
                                                     - shear_ratio1 * shear_ratio2 * T2(i) * T1(j) + (1 - shear_ratio1 * shear_ratio2) * T2(i) * T2(j));
                }
            }


        }

    }
    else if (ContactFlag == 0)  // not contacted
    {
        for (i = 0; i < 9; i++)
        {
            for (j = 0; j < 9; j++)
            {
                stiff(i, j) = 0.0;
            }
        }
    }


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

       double temp_stiff[36];
       temp_stiff[0]=stiff(0,0);
       temp_stiff[1]=stiff(0,1);
       temp_stiff[2]=stiff(0,2);
       temp_stiff[3]=stiff(0,3);
       temp_stiff[4]=stiff(0,4);
       temp_stiff[5]=stiff(0,5);
       temp_stiff[6]=stiff(1,0);
       temp_stiff[7]=stiff(1,1);
       temp_stiff[8]=stiff(1,2);
       temp_stiff[9]=stiff(1,3);
       temp_stiff[10]=stiff(1,4);
       temp_stiff[11]=stiff(1,5);
       temp_stiff[12]=stiff(2,0);
       temp_stiff[13]=stiff(2,1);
       temp_stiff[14]=stiff(2,2);
       temp_stiff[15]=stiff(2,3);
       temp_stiff[16]=stiff(2,4);
       temp_stiff[17]=stiff(2,5);
       temp_stiff[18]=stiff(3,0);
       temp_stiff[19]=stiff(3,1);
       temp_stiff[20]=stiff(3,2);
       temp_stiff[21]=stiff(3,3);
       temp_stiff[22]=stiff(3,4);
       temp_stiff[23]=stiff(3,5);
       temp_stiff[24]=stiff(4,0);
       temp_stiff[25]=stiff(4,1);
       temp_stiff[26]=stiff(4,2);
       temp_stiff[27]=stiff(4,3);
       temp_stiff[28]=stiff(4,4);
       temp_stiff[29]=stiff(4,5);
       temp_stiff[30]=stiff(5,0);
       temp_stiff[31]=stiff(5,1);
       temp_stiff[32]=stiff(5,2);
       temp_stiff[33]=stiff(5,3);
       temp_stiff[34]=stiff(5,4);
       temp_stiff[35]=stiff(5,5);
    */
    ///////////////////////////////////////////////////////////////////////



    return stiff ;
}


const Matrix &
ContactElement_3DOF_6DOF::getInitialStiff(void)
{

    getTangentStiff();

    return stiff ;
}


const Vector &
ContactElement_3DOF_6DOF::getResistingForce()
{

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
                resid(i) = (normalforce) * N(i) + shearforce(0) * T1(i) + shearforce(1) * T2(i);
            }
        }
        else if (SlidingFlag == 1) // slide
        {
            double shear_slide_1, shear_slide_2;
            shear_slide_1 = - (fs * normalforce + cohesion) * shearforce(0) / shear_force_norm;
            shear_slide_2 = - (fs * normalforce + cohesion) * shearforce(1) / shear_force_norm;

            for (i = 0; i < 6; i++)
            {
                resid(i) = -(normalforce) * N(i) + shear_slide_1 * T1(i) + shear_slide_2 * T2(i) ;
            }
        }
    }
    else if (ContactFlag == 0)  // not contacted
    {

        for (i = 0; i < 9; i++)
        {
            resid(i) = 0.0;
        }
    }

    //////////////////////////////////////////////
    /*
       double temp_shearforce_resid[2];
       temp_shearforce_resid[0]=shearforce(0);
       temp_shearforce_resid[1]=shearforce(1);


       double temp_resid[6];
       temp_resid[0]=resid(0);
       temp_resid[1]=resid(1);
       temp_resid[2]=resid(2);
       temp_resid[3]=resid(3);
       temp_resid[4]=resid(4);
       temp_resid[5]=resid(5);
    */
    /////////////////////////////////////////////

    return resid ;
}



const Vector &
ContactElement_3DOF_6DOF::getResistingForceIncInertia()
{
    return this->getResistingForce();
}


void
ContactElement_3DOF_6DOF::Print(ostream &s, int flag)
{
    if (flag == 0)   // print everything
    {
        s << "Element: " << this->getTag();
        s << " type: ContactElement_3DOF_6DOF  iNode: " << connectedExternalNodes(0);
        s << " jNode: " << connectedExternalNodes(1) << endln;
    }
    else if (flag == 1)
    {
        s << this->getTag() << endln;
    }
}


// Response*
// ContactElement_3DOF_6DOF::setResponse(const char** argv, int argc, Information& eleInformation)
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

//     else
//     {
//         return 0;
//     }
// }


// int
// ContactElement_3DOF_6DOF::getResponse(int responseID, Information& eleInfo)
// {
//     if (responseID == 1)
//     {
//         return eleInfo.setVector(this->getResistingForce());
//     }
//     else if (responseID == 2)
//     {
//         return eleInfo.setMatrix(this->getTangentStiff());
//     }
//     else
//     {
//         return -1;
//     }
// }



int ContactElement_3DOF_6DOF::if_nodes_are_in_contact_or_not(void)
{

    Vector slaveNode_location(3);
    Vector masterNode_location(3);
    Vector masterNode_TrialDisp_original(6);
    Vector masterNode_TrialDisp_reduced(3);

    slaveNode_location  = nodePointers[0]->getCrds() + nodePointers[0]->getTrialDisp();

    masterNode_TrialDisp_original = nodePointers[1]->getTrialDisp();
    masterNode_TrialDisp_reduced(0) = masterNode_TrialDisp_original(0);
    masterNode_TrialDisp_reduced(1) = masterNode_TrialDisp_original(1);
    masterNode_TrialDisp_reduced(2) = masterNode_TrialDisp_original(2);

    masterNode_location = nodePointers[1]->getCrds() + masterNode_TrialDisp_reduced;

    ///////////////////////////////////////////////////////////////////////
    /*
      double temp_slaveNd_location[3];
      temp_slaveNd_location[0]=slaveNode_location(0);
      temp_slaveNd_location[1]=slaveNode_location(1);
      temp_slaveNd_location[2]=slaveNode_location(2);

      double temp_masterNd_location[3];
      temp_masterNd_location[0]=masterNode_location(0);
      temp_masterNd_location[1]=masterNode_location(1);
      temp_masterNd_location[2]=masterNode_location(2);
    */
    ///////////////////////////////////////////////////////////////////////


    global_gap = masterNode_location - slaveNode_location;

    ///////////////////////////////////////////////////////////////////////
    /*
      double temp_global_gap[3];
      temp_global_gap[0]=global_gap(0);
      temp_global_gap[1]=global_gap(1);
      temp_global_gap[2]=global_gap(2);
    */
    ///////////////////////////////////////////////////////////////////////

    local_gap = transformation * global_gap;

    cerr << "\nGap between nodes " << connectedExternalNodes(0) << " and " << connectedExternalNodes(1) << " is " << local_gap(0) << endln;


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

int ContactElement_3DOF_6DOF::stick_or_slide(void)
{

    Vector DispTrialSlave(3); // trial disp for slave node
    Vector DispTrialMaster(3); // trial disp for master node
    int i;


    normalforce = Kn * local_gap(0); // Kn : normal penalty
    cerr << "Normal Force = " << fabs(normalforce) << endln;

    Vector masterNode_TrialDisp_original(6);
    Vector masterNode_TrialDisp_reduced(3);

    masterNode_TrialDisp_original = nodePointers[1]->getTrialDisp();
    masterNode_TrialDisp_reduced(0) = masterNode_TrialDisp_original(0);
    masterNode_TrialDisp_reduced(1) = masterNode_TrialDisp_original(1);
    masterNode_TrialDisp_reduced(2) = masterNode_TrialDisp_original(2);

    DispTrialSlave = nodePointers[0]->getTrialDisp();
    DispTrialMaster = masterNode_TrialDisp_reduced;

    double DispTrial[6];
    DispTrial[0] = DispTrialSlave(0);
    DispTrial[1] = DispTrialSlave(1);
    DispTrial[2] = DispTrialSlave(2);
    DispTrial[3] = DispTrialMaster(0);
    DispTrial[4] = DispTrialMaster(1);
    DispTrial[5] = DispTrialMaster(2);

    shearforce.Zero();
    shear_relative_displacement.Zero();

    for (i = 0; i < 6; i++)
    {
        shear_relative_displacement(0) += T1(i) * DispTrial[i];
        shear_relative_displacement(1) += T2(i) * DispTrial[i];
    }

    ///////////////////////////////////////////////////////////////////////
    /*
       double temp_shear_relative_displacement[2];
       temp_shear_relative_displacement[0]=shear_relative_displacement(0);
       temp_shear_relative_displacement[1]=shear_relative_displacement(1);
    */
    ///////////////////////////////////////////////////////////////////////


    // Compute trial shear force
    for (i = 0; i < 2; i++)
    {
        shearforce(i) = Kt * shear_relative_displacement(i);    //Kt: tangential penalty
    }

    cerr << "Shear Force 1 = " << fabs(shearforce(0)) << endln;
    cerr << "Shear Force 2 = " << fabs(shearforce(1)) << endln;

    shear_force_norm = shearforce.Norm();

    ///////////////////////////////////////////////////////////////////////
    /*
       double temp_shearforce[2];
       temp_shearforce[0]=shearforce(0);
       temp_shearforce[1]=shearforce(1);
    */
    ///////////////////////////////////////////////////////////////////////


    yield_criteria = shear_force_norm - (fs * fabs(normalforce) + cohesion);


    if (yield_criteria <= 0)
    {
        // stick
        cerr << "Sticking ..." << endln;
        return 0;
    }
    else
    {
        // slide
        cerr << "Sliding ..." << endln;
        return 1;
    }
}


int
ContactElement_3DOF_6DOF::ContactPlane(const Vector &x_local)
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


int
ContactElement_3DOF_6DOF::sendSelf(int commitTag, Channel &theChannel)
{
    return 0;
}

int
ContactElement_3DOF_6DOF::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    return 0;
}



