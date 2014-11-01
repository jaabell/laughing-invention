//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//# PROJECT:           Object Oriented Finite Element Program
//# PURPOSE:
//
//# VERSION:
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Nima Tafazzoli , Boris Jeremic
//# PROGRAMMER(S):     Nima Tafazzoli , Boris Jeremic
//
//# DATE:              May 2012
//# UPDATE HISTORY:
//
//
//===============================================================================


#include <Linear_Force_TimeHistory.h>
#include <math.h>
#include <iostream>
using namespace std;

#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;

Linear_Force_TimeHistory::Linear_Force_TimeHistory()
    : LoadPattern(0, PATTERN_TAG_Linear_Force_TimeHistory),
      nodeTag(0),
      dof(0),
      final_load(0),
      theFactor(0),
      isConstant(0)
{
    // does nothing
}

Linear_Force_TimeHistory::Linear_Force_TimeHistory(int tag,
        int NodeNumber,
        int dof_to_be_shaken,
        double final_load_value)
    : LoadPattern(tag, PATTERN_TAG_Linear_Force_TimeHistory),
      nodeTag(NodeNumber),
      dof(dof_to_be_shaken),
      final_load(final_load_value),
      theFactor(0),
      isConstant(0)
{
    // does nothing
}



Linear_Force_TimeHistory::~Linear_Force_TimeHistory()
{
    // does nothing
}


void
Linear_Force_TimeHistory::setDomain(Domain *theDomain)
{
    this->LoadPattern::setDomain(theDomain);
}


double
Linear_Force_TimeHistory::getFactor(double pseudoTime)
{
    if (isConstant == 0)
    {
        theFactor = final_load * pseudoTime;
    }

    return theFactor;

}

void
Linear_Force_TimeHistory::setLoadConstant(void)
{
    isConstant = 1;
}


void
Linear_Force_TimeHistory::applyLoad(double time)
{

    Domain *theDomain = this->getDomain();
    if (theDomain == 0)
    {
        return;
    }


    Node *theNode;

    theNode = theDomain->getNode(nodeTag);

    if (theNode == 0)
    {
        cerr << "Linear_Force_TimeHistory - no nodes associtated to the nodeTag " << nodeTag << "\n";
        // exit(1);
    }
    else
    {
        //Create the nodal load vector accoding to the DOFs the node has
        int numDOF = theNode->getNumberDOF();
        Vector *nodalLoad = new Vector(numDOF);



        for (int i = 0; i < numDOF; i++)
        {
            if ( i == (dof - 1) )
            {
                (*nodalLoad)(i) = getFactor(time);
            }
            else
            {
                (*nodalLoad)(i) = 0;
            }
        }

        theNode->addUnbalancedLoad(*nodalLoad);
        delete nodalLoad;

        cout << "Linear_Force_TimeHistory - Applied load!" << endl;
    }

}



int
Linear_Force_TimeHistory::sendSelf(int commitTag, Channel &theChannel)
{

    int dbTag = this->getDbTag();

    static Vector data(6);

    data(0) = this->getTag();
    data(1) = nodeTag;
    data(2) = dof;
    data(3) = final_load;
    data(4) = isConstant;
    data(5) = theFactor;

    int res = theChannel.sendVector(dbTag, commitTag, data);
    //   int res = theChannel.sendID(dbTag, commitTag, data);

    if (res < 0)
    {
        cerr << "Linear_Force_TimeHistory::sendSelf() - channel failed to send data\n";
        return res;
    }

    return 0;

}


int
Linear_Force_TimeHistory::receiveSelf(int commitTag, Channel &theChannel,
                                      FEM_ObjectBroker &theBroker)
{
    int dbTag = this->getDbTag();

    static Vector data(6);

    int res = theChannel.receiveVector(dbTag, commitTag, data);


    if (res < 0)
    {
        cerr << "Linear_Force_TimeHistory::receiveSelf() - channel failed to recv data\n";
        return res;
    }

    this->setTag(data(0));
    nodeTag = data(1);
    dof = data(2);
    final_load = data(3);
    isConstant = data(4);
    theFactor = data(5);


    return 0;
}


void
Linear_Force_TimeHistory::Print(ostream &s, int flag)
{
    s << "Linear_Force_TimeHistory " << this->getTag() << endln;
    s << "Node Number: "   << nodeTag << endln;
    s << "DOF to be Shaken: " << dof << endln;
    s << "Slope: "   << final_load << endln;

}



LoadPattern *
Linear_Force_TimeHistory::getCopy(void)
{
    LoadPattern *theCopy = new Linear_Force_TimeHistory(this->getTag(),
            nodeTag,
            dof,
            final_load);
    return theCopy;
}



