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


#include <Linear_Disp_TimeHistory_Penalty.h>
#include <math.h>
#include <iostream>
using namespace std;


#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;

Linear_Disp_TimeHistory_Penalty::Linear_Disp_TimeHistory_Penalty()
    : LoadPattern(0, PATTERN_TAG_Linear_Disp_TimeHistory_Penalty),
      nodeTag(0),
      penalty_stiffness(0),
      dof(0),
      previous_time(0),
      final_displacement(0),
      isConstant(0)
{
    // does nothing
}

Linear_Disp_TimeHistory_Penalty::Linear_Disp_TimeHistory_Penalty(int tag,
        double PenaltyStiffness,
        int NodeNumber,
        int dof_to_be_shaken,
        double final_displacement_value)
    : LoadPattern(tag, PATTERN_TAG_Linear_Disp_TimeHistory_Penalty),
      nodeTag(NodeNumber),
      penalty_stiffness(PenaltyStiffness),
      dof(dof_to_be_shaken),
      previous_time(0),
      final_displacement(final_displacement_value),
      isConstant(0)
{
    // does nothing
}



Linear_Disp_TimeHistory_Penalty::~Linear_Disp_TimeHistory_Penalty()
{
    // does nothing
}


void
Linear_Disp_TimeHistory_Penalty::setDomain(Domain* theDomain)
{

    this->LoadPattern::setDomain(theDomain);

}


double
Linear_Disp_TimeHistory_Penalty::getFactor(double pseudoTime)
{

    //     if (pseudoTime >= tStart && (pseudoTime - tFinish <= 1.0e-10))
    //        {
    if (isConstant == 0)
    {
        double theFactor = penalty_stiffness * final_displacement * (pseudoTime - previous_time);
        previous_time = pseudoTime;
        return theFactor;
    }
    else
    {
        return 0;
    }

    //        }
    //     else
    //      return 0.0;

}


void
Linear_Disp_TimeHistory_Penalty::applyLoad(double time)
{

    Domain* theDomain = this->getDomain();

    if (theDomain == 0)
    {
        return;
    }


    Node* theNode;

    theNode = theDomain->getNode(nodeTag);

    if (theNode == 0)
    {
        cerr << "Linear_Disp_TimeHistory_Penalty - no nodes associtated to the nodeTag " << nodeTag << "\n";
        exit(1);
    }

    //Create the nodal load vector accoding to the DOFs the node has
    int numDOF = theNode->getNumberDOF();
    Vector* nodalLoad = new Vector(numDOF);



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

    theNode->addUnbalancedLoad(*nodalLoad, true);
    delete nodalLoad;

}


void
Linear_Disp_TimeHistory_Penalty::setLoadConstant(void)
{
    isConstant = 1;
}


int
Linear_Disp_TimeHistory_Penalty::sendSelf(int commitTag, Channel& theChannel)
{
    // to be completed later
    return 0;
}


int
Linear_Disp_TimeHistory_Penalty::receiveSelf(int commitTag, Channel& theChannel,
        FEM_ObjectBroker& theBroker)
{
    // to be completed later
    return 0;
}


void
Linear_Disp_TimeHistory_Penalty::Print(ostream& s, int flag)
{
    s << "Linear_Disp_TimeHistory_Penalty " << this->getTag() << endln;
    s << "Node Number: "   << nodeTag << endln;
    s << "DOF to be Shaken: " << dof << endln;
    s << "Final Value: "   << final_displacement << endln;

}



// method to obtain a blank copy of the LoadPattern
LoadPattern*
Linear_Disp_TimeHistory_Penalty::getCopy(void)
{

    cerr << "Linear_Disp_TimeHistory_Penalty::getCopy() - not yet implemented\n";
    return 0;
}


