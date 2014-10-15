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
// PURPOSE:           Penalty Element
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              May 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#include "PenaltyElement.h"


PenaltyElement::PenaltyElement(int tag, int node1, int node2, double Penalty_Stiffness, int dof_in)
    : Element(tag, ELE_TAG_PenaltyElement),
      connectedExternalNodes(2), numDOF(0), dof(dof_in), penaltystiffness(Penalty_Stiffness),
      tempZeroVector(3), tempZeroMatrix(3, 3), outputVector(0)
{
    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL PenaltyElement::PenaltyElement - " <<  tag << "failed to create an ID of size 2\n";
        exit(-1);
    }

    connectedExternalNodes(0) = node1;
    connectedExternalNodes(1) = node2;

    theNodes[0] = 0;
    theNodes[1] = 0;
    tempZeroMatrix.Zero();
    tempZeroVector.Zero();

    this->setNumberOfBoundaryNodes(1);
}



PenaltyElement::PenaltyElement()
    : Element(0, ELE_TAG_PenaltyElement),
      connectedExternalNodes(2),
      numDOF(0), dof(0), penaltystiffness(0),
      tempZeroVector(3), tempZeroMatrix(3, 3), outputVector(0)
{


    if (connectedExternalNodes.Size() != 2)
    {
        cerr << "FATAL PenaltyElement::PenaltyElement - failed to create an ID of size 2\n";
        exit(-1);
    }

    theNodes[0] = 0;
    theNodes[1] = 0;
    tempZeroMatrix.Zero();
    tempZeroVector.Zero();

    this->setNumberOfBoundaryNodes(1);
}



PenaltyElement::~PenaltyElement()
{
    if (Stiffness != 0)
    {
        delete Stiffness;
    }

    //   if (tempZeroVector != 0)
    //     delete tempZeroVector;

    /*  if (tempZeroMatrix != 0)
        delete tempZeroMatrix; */
}


int
PenaltyElement::getNumExternalNodes(void) const
{
    return 2;
}


const ID &
PenaltyElement::getExternalNodes(void)
{
    return connectedExternalNodes;
}


Node **
PenaltyElement::getNodePtrs(void)
{
    return theNodes;
}


int
PenaltyElement::getNumDOF(void)
{
    return numDOF;
}



void
PenaltyElement::setDomain(Domain *theDomain)
{

    if (theDomain == 0)
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
        return;
    }

    theNodes[0] = theDomain->getNode( connectedExternalNodes(0) );
# ifdef _PARALLEL_PROCESSING

    if ( theNodes[0] == 0 )
    {
        theNodes[0] = theDomain->getOutsideNode( connectedExternalNodes(0) );
    }

# endif
    theNodes[1] = theDomain->getNode( connectedExternalNodes(1) );
# ifdef _PARALLEL_PROCESSING

    if ( theNodes[1] == 0 )
    {
        theNodes[1] = theDomain->getOutsideNode( connectedExternalNodes(1) );
    }

# endif


    if (theNodes[0] == 0 || theNodes[1] == 0)
    {
        cerr << "PenaltyElement::setDomain() - PenaltyElement " << this->getTag() << " node does not exist in the model\n";
        exit(-1);
    }


    if ( (dof > theNodes[0]->getNumberDOF()) || (dof > theNodes[1]->getNumberDOF()) )
    {
        cerr << "PenaltyElement::setDomain() - PenaltyElement " << this->getTag() << " defined dof is appropriates for either of the nodes!\n";
        exit(-1);
    }



    numDOF = (theNodes[0]->getNumberDOF()) + (theNodes[1]->getNumberDOF());


    // call the base class method
    this->DomainComponent::setDomain(theDomain);


    Stiffness = new Matrix(numDOF, numDOF);

}


int
PenaltyElement::commitState()
{

    int retVal = 0;

    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "PenaltyElement::commitState () - failed in base class";
        exit(-1);
    }

    return retVal;

}



const Matrix &
PenaltyElement::getTangentStiff(void)
{
    int node1_numDOF = theNodes[0]->getNumberDOF();

    (*Stiffness)(dof - 1, dof - 1)                               = penaltystiffness;
    (*Stiffness)(dof - 1 + node1_numDOF, dof - 1 + node1_numDOF) = penaltystiffness;
    (*Stiffness)(dof - 1, dof - 1 + node1_numDOF)                = -penaltystiffness;
    (*Stiffness)(dof - 1 + node1_numDOF, dof - 1)                = -penaltystiffness;

    return *Stiffness;
}


const Matrix &
PenaltyElement::getInitialStiff(void)
{

    return this->getTangentStiff();
}



double
PenaltyElement::getPenaltyStiffness()
{
    return penaltystiffness;
}



const Matrix &
PenaltyElement::getMass(void)
{
    //    tempZeroMatrix.Zero();
    return tempZeroMatrix;
}



const Vector &
PenaltyElement::getResistingForce()
{
    static Vector P(numDOF);

    Vector displacement1 = theNodes[0]->getTrialDisp();
    Vector displacement2 = theNodes[1]->getTrialDisp();

    Vector displacement(numDOF);
    // cout << "numDOF = " << numDOF << endl;
    int d;
    int node1_numDOF = displacement1.Size();
    for (d = 0; d < node1_numDOF; d++)
    {
        displacement(d)  = displacement1(d);
    }
    for (; d < numDOF; d++)
    {
        displacement(d)  = displacement2(d - node1_numDOF);
    }

    P.Zero();
    P.addMatrixVector(1.0, *Stiffness, displacement, 1.0);
    // cout << "P = " << P << endl;
    // P.addVector(1.0, Q, -1.0);

    return P;
}


void
PenaltyElement::zeroLoad(void)
{
}


const Vector &
PenaltyElement::getResistingForceIncInertia()
{
    static Vector P(numDOF);

    Vector displacement1 = theNodes[0]->getTrialDisp();
    Vector displacement2 = theNodes[1]->getTrialDisp();

    Vector displacement(numDOF);
    // cout << "numDOF = " << numDOF << endl;
    int d;
    int node1_numDOF = displacement1.Size();
    for (d = 0; d < node1_numDOF; d++)
    {
        displacement(d)  = displacement1(d);
    }
    for (; d < numDOF; d++)
    {
        displacement(d)  = displacement2(d - node1_numDOF);
    }

    P.Zero();
    P.addMatrixVector(1.0, *Stiffness, displacement, 1.0);
    // cout << "P = " << P << endl;
    // P.addVector(1.0, Q, -1.0);

    return P;
}



int
PenaltyElement::sendSelf(int commitTag, Channel &theChannel)
{
    ID intData(5);
    Vector doubleData(1);

    intData(0) = this->getTag();
    intData(1) = numDOF;
    intData(2) = dof;
    intData(3) = connectedExternalNodes(0);
    intData(4) = connectedExternalNodes(1);


    if (theChannel.sendID(this->getDbTag(), commitTag, intData) < 0)
    {
        cerr << "PenaltyElement::sendSelf -- could not send intData Vector\n";
        return -1;
    }


    doubleData(0) = penaltystiffness;
    if (theChannel.sendVector(this->getDbTag(), commitTag, doubleData) < 0)
    {
        cerr << "PenaltyElement::sendSelf -- could not send doubleData Vector\n";
        return -1;
    }

    return 0;
}



int
PenaltyElement::recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    ID intData(5);
    Vector doubleData(1);

    if (theChannel.recvID(this->getDbTag(), commitTag, intData) < 0)
    {
        cerr << "PenaltyElement::recvSelf -- could not receive doubleData Vector\n";
        return -1;
    }

    this->setTag(intData(0));
    numDOF                    = intData(1);
    dof                       = intData(2);
    connectedExternalNodes(0) = intData(3);
    connectedExternalNodes(1) = intData(4);

    if (theChannel.recvVector(this->getDbTag(), commitTag, doubleData) < 0)
    {
        cerr << "PenaltyElement::recvSelf -- could not receive doubleData Vector\n";
        return -1;
    }
    penaltystiffness = doubleData(0);

    return 0;

}



void
PenaltyElement::Print(ostream &s, int flag)
{
    s << "Element: " << this->getTag();
    s << " type: PenaltyElement Nodes: " << connectedExternalNodes(0) << "   " << connectedExternalNodes(1);
}


// Matrix &getGaussCoordinates(void);
int PenaltyElement::getOutputSize() const
{
    return 0;
}

const Vector &PenaltyElement::getOutput() const
{
    return outputVector;
}

// Response*
// PenaltyElement::setResponse(const char** argv, int argc, Information& eleInfo)
// {

//     if (strcmp(argv[0], "stiff") == 0)
//     {
//         return new ElementResponse(this, 1, Matrix(numDOF, numDOF) );
//     }

//     else
//     {
//         return 0;
//     }
// }




// int
// PenaltyElement::getResponse(int responseID, Information& eleInfo)
// {
//     switch (responseID)
//     {

//         case 1:
//             return eleInfo.setMatrix(this->getTangentStiff());

//         default:
//             return 0;
//     }
// }



