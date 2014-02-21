//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  frequency,  and anybody
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


#include <Ricker1st_Disp_TimeHistory_Penalty.h>
#include <math.h>

#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;

Ricker1st_Disp_TimeHistory_Penalty::Ricker1st_Disp_TimeHistory_Penalty()
    : LoadPattern(0, PATTERN_TAG_Ricker1st_Disp_TimeHistory_Penalty),
      nodeTag(0),
      penalty_stiffness(0),
      dof(0),
      tStart(0),
      tFinish(0),
      frequency(0),
      Timemax(0),
      previous_load(0),
      cFactor(1.0)
{
    // does nothing
}

Ricker1st_Disp_TimeHistory_Penalty::Ricker1st_Disp_TimeHistory_Penalty(int tag,
        double PenaltyStiffness,
        int NodeNumber,
        int dof_to_be_shaken,
        double startTime,
        double finishTime,
        double Frequency,
        double TimeMaximum,
        double Amplitude)
    : LoadPattern(tag, PATTERN_TAG_Ricker1st_Disp_TimeHistory_Penalty),
      nodeTag(NodeNumber),
      penalty_stiffness(PenaltyStiffness),
      dof(dof_to_be_shaken),
      tStart(startTime),
      tFinish(finishTime),
      frequency(Frequency),
      previous_load(0),
      Timemax(TimeMaximum),
      cFactor(Amplitude)
{
    // does nothing
}



Ricker1st_Disp_TimeHistory_Penalty::~Ricker1st_Disp_TimeHistory_Penalty()
{
    // does nothing
}


void
Ricker1st_Disp_TimeHistory_Penalty::setDomain(Domain* theDomain)
{

    this->LoadPattern::setDomain(theDomain);

}


double
Ricker1st_Disp_TimeHistory_Penalty::getFactor(double pseudoTime)
{

    static double Pi = 2 * asin(1.0);

    if (pseudoTime >= tStart && (pseudoTime - tFinish <= 1.0e-10))
    {
        double current_load = cFactor * (Pi * frequency * (pseudoTime - tStart - Timemax)
                                         * exp(-Pi * Pi * frequency * frequency * (pseudoTime - tStart - Timemax)
                                               * (pseudoTime - tStart - Timemax)));

        double difference_load = current_load - previous_load;
        previous_load = current_load;
        return penalty_stiffness * difference_load;
    }
    else
    {
        return 0.0;
    }
}


void
Ricker1st_Disp_TimeHistory_Penalty::applyLoad(double time)
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
        std::cerr << "Ricker1st_Disp_TimeHistory_Penalty - no nodes associtated to the nodeTag " << nodeTag << "\n";
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



int
Ricker1st_Disp_TimeHistory_Penalty::sendSelf(int commitTag, Channel& theChannel)
{
    // to be completed later
    return 0;
}


int
Ricker1st_Disp_TimeHistory_Penalty::recvSelf(int commitTag, Channel& theChannel,
        FEM_ObjectBroker& theBroker)
{
    // to be completed later
    return 0;
}


void
Ricker1st_Disp_TimeHistory_Penalty::Print(OPS_Stream& s, int flag)
{
    s << "Ricker1st_Disp_TimeHistory_Penalty " << this->getTag() << endln;
    s << "Node Number: "   << nodeTag << endln;
    s << "DOF to be Shaken: " << dof << endln;
    s << "Amplitude: "   << cFactor << endln;
    s << "Start Time: "   << tStart << endln;
    s << "Finish Time: "  << tFinish << endln;
    s << "frequency: "   << frequency << endln;
    s << "Timemax angle: "   <<  Timemax << endln;

}



// method to obtain a blank copy of the LoadPattern
LoadPattern*
Ricker1st_Disp_TimeHistory_Penalty::getCopy(void)
{

    std::cerr << "Ricker1st_Disp_TimeHistory_Penalty::getCopy() - not yet implemented\n";
    return 0;
}


