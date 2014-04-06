/////////////////////////////////////////////////////////////////////////////
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
// PURPOSE:           Penalty Element
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              May 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#include "PenaltyElementApplyDisplacement.h"
#include <Information.h>

#include <Domain.h>
#include <Node.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <UniaxialMaterial.h>
// #include <Renderer.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

// #include <ElementResponse.h>

//#include <fstream>

// initialise the class wide variables
Matrix PenaltyElementApplyDisplacement::PenaltyElementApplyDisplacementM3(3, 3);
Matrix PenaltyElementApplyDisplacement::PenaltyElementApplyDisplacementM6(6, 6);
Vector PenaltyElementApplyDisplacement::PenaltyElementApplyDisplacementV3(3);
Vector PenaltyElementApplyDisplacement::PenaltyElementApplyDisplacementV6(6);



PenaltyElementApplyDisplacement::PenaltyElementApplyDisplacement(int tag,
        int Existed_Node,
        int Element_Direction_Dof,
        double Penalty_Stiffness)
    : Element(tag, ELE_TAG_PenaltyElementApplyDisplacement),
      connectedExternalNodes(1), numDOF(0), theLoad(0),
      theMatrix(0), theVector(0), tempMatrix(0), tempVector(0),
      direction(Element_Direction_Dof), penaltystiffness(Penalty_Stiffness)
{




    // ensure the connectedExternalNode ID is of correct size & set values
    if (connectedExternalNodes.Size() != 1)
    {
        cerr << "FATAL PenaltyElementApplyDisplacement::PenaltyElementApplyDisplacement - " <<  tag << "failed to create an ID of size 2\n";
        exit(-1);
    }

    connectedExternalNodes(0) = Existed_Node;


    theNodes[0] = 0;

}


// constructor:
//   invoked by a FEM_ObjectBroker - blank object that recvSelf needs
//   to be invoked upon
PenaltyElementApplyDisplacement::PenaltyElementApplyDisplacement()
    : Element(0, ELE_TAG_PenaltyElementApplyDisplacement),
      connectedExternalNodes(1), numDOF(0), theLoad(0),
      theMatrix(0), theVector(0), tempMatrix(0), tempVector(0),
      direction(0), penaltystiffness(0)
{

    // ensure the connectedExternalNode ID is of correct size
    if (connectedExternalNodes.Size() != 1)
    {
        cerr << "FATAL PenaltyElementApplyDisplacement::PenaltyElementApplyDisplacement - failed to create an ID of size 2\n";
        exit(-1);
    }

    theNodes[0] = 0;

}


//  destructor
//     delete must be invoked on any objects created by the object
//     and on the matertial object.
PenaltyElementApplyDisplacement::~PenaltyElementApplyDisplacement()
{
    // invoke the destructor on any objects created by the object
    // that the object still holds a pointer to

    if (theLoad != 0)
    {
        delete theLoad;
    }
}


int
PenaltyElementApplyDisplacement::getNumExternalNodes(void) const
{
    return 1;
}


const ID &
PenaltyElementApplyDisplacement::getExternalNodes(void)
{
    return connectedExternalNodes;
}


Node **
PenaltyElementApplyDisplacement::getNodePtrs(void)
{
    return theNodes;
}


int
PenaltyElementApplyDisplacement::getNumDOF(void)
{
    return numDOF;
}



// method: setDomain()
//    to set a link to the enclosing Domain and to set the node pointers.
//    also determines the number of dof associated
//    with the PenaltyElementApplyDisplacement element, we set matrix and vector pointers,
//    allocate space for t matrix, determine the length
//    and set the transformation matrix.
void
PenaltyElementApplyDisplacement::setDomain(Domain *theDomain)
{
    // check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        theNodes[0] = 0;
        return;
    }


    // first set the node pointers
    int Existed_Node = connectedExternalNodes(0);
    theNodes[0] = theDomain->getNode(Existed_Node);


    if (theNodes[0] == 0)
    {
        cerr << "PenaltyElementApplyDisplacement::setDomain() - PenaltyElementApplyDisplacement" << this->getTag() << " node " << Existed_Node <<
             "does not exist in the model\n";

        return;
    }


    int dofExisted_Node = theNodes[0]->getNumberDOF();



    // call the base class method
    this->DomainComponent::setDomain(theDomain);

    // now set the number of dof for element and set matrix and vector pointer
    if (dofExisted_Node == 3)
    {
        numDOF = 3;
        theMatrix = &PenaltyElementApplyDisplacementM3;
        theVector = &PenaltyElementApplyDisplacementV3;
        tempMatrix = &PenaltyElementApplyDisplacementM3;
        tempVector = &PenaltyElementApplyDisplacementV3;
    }
    else if (dofExisted_Node == 6)
    {
        numDOF = 6;
        theMatrix = &PenaltyElementApplyDisplacementM6;
        theVector = &PenaltyElementApplyDisplacementV6;
        tempMatrix = &PenaltyElementApplyDisplacementM6;
        tempVector = &PenaltyElementApplyDisplacementV6;
    }
    else
    {
        cerr << "WARNING PenaltyElementApplyDisplacement::setDomain cannot handle " << dofExisted_Node << " dofs at nodes in " <<
             dofExisted_Node  << " problem\n";
        return;
    }



    if (theLoad == 0)
    {
        theLoad = new Vector(numDOF);
    }
    else if (theLoad->Size() != numDOF)
    {
        delete theLoad;
        theLoad = new Vector(numDOF);
    }

    if (theLoad == 0)
    {
        cerr << "PenaltyElementApplyDisplacement::setDomain - PenaltyElementApplyDisplacement " << this->getTag() <<
             "out of memory creating vector of size" << numDOF << endln;
        exit(-1);
        return;
    }



}


int
PenaltyElementApplyDisplacement::commitState()
{

    int retVal = 0;

    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "PenaltyElementApplyDisplacement::commitState () - failed in base class";
    }

    return retVal;
}



int
PenaltyElementApplyDisplacement::revertToLastCommit()
{
    return 0;
}



int
PenaltyElementApplyDisplacement::revertToStart()
{
    return 0;
}



int
PenaltyElementApplyDisplacement::update(void)
{
    return 0;
}



const Matrix &
PenaltyElementApplyDisplacement::getTangentStiff(void)
{


    Matrix &stiff = *theMatrix;


    if (numDOF == 3)
    {
        if (direction == 1)
        {
            stiff(0, 0) = penaltystiffness;
        }
        else if (direction == 2)
        {
            stiff(1, 1) = penaltystiffness;
        }
        else if (direction == 3)
        {
            stiff(2, 2) = penaltystiffness;
        }
        else
        {
            cerr << "WARNING PenaltyElementApplyDisplacement::setDomain() - PenaltyElementApplyDisplacement " << this->getTag() << " has direction problem\n";
            abort();
        }
    }
    else if (numDOF == 6)
    {
        if (direction == 1)
        {
            stiff(0, 0) = penaltystiffness;
        }
        else if (direction == 2)
        {
            stiff(1, 1) = penaltystiffness;
        }
        else if (direction == 3)
        {
            stiff(2, 2) = penaltystiffness;
        }
        else if (direction == 4)
        {
            stiff(3, 3) = penaltystiffness;
        }
        else if (direction == 5)
        {
            stiff(4, 4) = penaltystiffness;
        }
        else if (direction == 6)
        {
            stiff(5, 5) = penaltystiffness;
        }
        else
        {
            cerr << "WARNING PenaltyElementApplyDisplacement::setDomain() - PenaltyElementApplyDisplacement " << this->getTag() << " has direction problem\n";
            abort();
        }
    }


    return stiff;
}


const Matrix &
PenaltyElementApplyDisplacement::getInitialStiff(void)
{

    this->getTangentStiff();

    return *theMatrix;
}



double
PenaltyElementApplyDisplacement::getPenaltyStiffness()
{
    return penaltystiffness;
}



const Matrix &
PenaltyElementApplyDisplacement::getDamp(void)
{
    Matrix &damp = *theMatrix;
    damp.Zero();
    return damp;
}



const Matrix &
PenaltyElementApplyDisplacement::getMass(void)
{
    Matrix &mass = *theMatrix;
    mass.Zero();
    return mass;
}



void
PenaltyElementApplyDisplacement::zeroLoad(void)
{
    theLoad->Zero();
}



int
PenaltyElementApplyDisplacement::addLoad(ElementalLoad *theLoad, double loadFactor)

{
    cerr << "PenaltyElementApplyDisplacement::addLoad - load type unknown for PenaltyElementApplyDisplacement with tag: " << this->getTag() << endln;

    return -1;
}



int
PenaltyElementApplyDisplacement::addInertiaLoadToUnbalance(const Vector &accel)
{
    return 0;
}



const Vector &
PenaltyElementApplyDisplacement::getResistingForce()
{


    //   Vector displacement(numDOF);
    //   displacement = theNodes[0]->getTrialDisp();


    //   Vector unbalancedforce(numDOF);
    //   unbalancedforce = theNodes[0]->getUnbalancedLoad();
    //
    //
    //
    //   for (int i=0; i<numDOF; i++)
    //   {
    //     (*theVector)(i) = unbalancedforce(i);
    //   }
    //
    //
    //
    //   for (int i=0; i<numDOF; i++)
    //   {
    //     cout.flush() << "penalty force (" << i << "):" << (*theVector)(i) << endl;
    //   }


    return *theVector;
}



const Vector &
PenaltyElementApplyDisplacement::getResistingForceIncInertia()
{

    this->getResistingForce();
    return *theVector;

}




int
PenaltyElementApplyDisplacement::sendSelf(int commitTag, Channel &theChannel)
{

    //Iimplemented by Babak Kamrani on 6/12/13
    //-----------
    int res = 0;
    static Vector data(5);

    data(0) = (double)numDOF;
    data(1) = penaltystiffness;
    data(2) = (double)connectedExternalNodes(0);
    data(3) = direction;
    data(4) = (double)(this->getTag());


    res += theChannel.sendVector(this->getDbTag(), commitTag, data);

    if (res < 0)
    {
        cerr << "PenaltyElement::sendSelf -- could not send data Vector\n";
        return res;
    }

    return res;
    //-----------
}



int
PenaltyElementApplyDisplacement::recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    //Iimplemented by Babak Kamrani on 6/12/13
    //-----------
    int res = 0;
    static Vector data(5);


    res += theChannel.recvVector(this->getDbTag(), commitTag, data);

    if (res < 0)
    {
        cerr << "PenaltyElementApplyDisplacement::recvSelf -- could not receive data Vector\n";
        return res;
    }

    numDOF = (int)data(0);
    penaltystiffness = data(1);
    connectedExternalNodes(0) = (int)data(2);
    direction = data(3);
    this->setTag((int)(data(4)));

    return 0;
    //-----------
}



void
PenaltyElementApplyDisplacement::Print(ostream &s, int flag)
{
    s << "Element: " << this->getTag();
    s << " type: PenaltyElementApplyDisplacement Node: " << connectedExternalNodes(0);
}


// Response*
// PenaltyElementApplyDisplacement::setResponse(const char** argv, int argc, Information& eleInfo)
// {


//     //   if (strcmp(argv[0],"defo") == 0 || strcmp(argv[0],"deformations") == 0 ||
//     //     strcmp(argv[0],"deformation") == 0)
//     //     return new ElementResponse(this, 2, 0.0);

//     // tangent stiffness matrix
//     if (strcmp(argv[0], "stiff") == 0)
//     {
//         return new ElementResponse(this, 3, *theMatrix);
//     }

//     // a material quantity
//     //   else if (strcmp(argv[0],"material") == 0 || strcmp(argv[0],"-material") == 0)
//     //     return theMaterial->setResponse(&argv[1], argc-1, eleInfo);

//     else
//     {
//         return 0;
//     }
// }




// int
// PenaltyElementApplyDisplacement::getResponse(int responseID, Information& eleInfo)
// {
//     switch (responseID)
//     {
//         //     case 1:
//         //       return eleInfo.setDouble(theMaterial->getStress());

//         //     case 2:
//         //       return eleInfo.setDouble(theMaterial->getStrain());

//         case 3:
//             return eleInfo.setMatrix(this->getTangentStiff());

//         default:
//             return 0;
//     }
// }



