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


#include <SineWave_Force_TimeHistory.h>
#include <math.h>

#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;

SineWave_Force_TimeHistory::SineWave_Force_TimeHistory()
    : LoadPattern(0, PATTERN_TAG_SineWave_Force_TimeHistory),
      nodeTag(0),
      dof(0),
      tStart(0),
      tFinish(0),
      period(0),
      phase(0),
      cFactor(1.0)
{
    // does nothing
}

SineWave_Force_TimeHistory::SineWave_Force_TimeHistory(int tag,
        int NodeNumber,
        int dof_to_be_shaken,
        double startTime,
        double finishTime,
        double Period,
        double phaseangle,
        double Amplitude)
    : LoadPattern(tag, PATTERN_TAG_SineWave_Force_TimeHistory),
      nodeTag(NodeNumber),
      dof(dof_to_be_shaken),
      tStart(startTime),
      tFinish(finishTime),
      period(Period),
      phase(phaseangle),
      cFactor(Amplitude)
{
    // does nothing
}



SineWave_Force_TimeHistory::~SineWave_Force_TimeHistory()
{
    // does nothing
}


void
SineWave_Force_TimeHistory::setDomain(Domain* theDomain)
{

    this->LoadPattern::setDomain(theDomain);

}


double
SineWave_Force_TimeHistory::getFactor(double pseudoTime)
{
    static double twopi = 4 * asin(1.0);

    if (pseudoTime >= tStart && pseudoTime <= tFinish)
    {
        return cFactor * sin(twopi * (pseudoTime - tStart) / period + phase);
    }
    else
    {
        return 0.0;
    }
}


void
SineWave_Force_TimeHistory::applyLoad(double time)
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
        std::cerr << "SineWave_Force_TimeHistory - no nodes associtated to the nodeTag " << nodeTag << "\n";
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

    theNode->addUnbalancedLoad(*nodalLoad);
    delete nodalLoad;

}



int
SineWave_Force_TimeHistory::sendSelf(int commitTag, Channel& theChannel)
{
    // to be completed later
    return 0;
}


int
SineWave_Force_TimeHistory::recvSelf(int commitTag, Channel& theChannel,
                                     FEM_ObjectBroker& theBroker)
{
    // to be completed later
    return 0;
}


void
SineWave_Force_TimeHistory::Print(OPS_Stream& s, int flag)
{
    s << "SineWave_Force_TimeHistory " << this->getTag() << endln;
    s << "Node Number: "   << nodeTag << endln;
    s << "DOF to be Shaken: " << dof << endln;
    s << "Amplitude: "   << cFactor << endln;
    s << "Start Time: "   << tStart << endln;
    s << "Finish Time: "  << tFinish << endln;
    s << "Period: "   << period << endln;
    s << "Phase angle: "   <<  phase << endln;

}



// method to obtain a blank copy of the LoadPattern
LoadPattern*
SineWave_Force_TimeHistory::getCopy(void)
{

    std::cerr << "SineWave_Force_TimeHistory::getCopy() - not yet implemented\n";
    return 0;
}


