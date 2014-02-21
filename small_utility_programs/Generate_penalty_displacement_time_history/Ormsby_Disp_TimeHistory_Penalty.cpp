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


#include <Ormsby_Disp_TimeHistory_Penalty.h>
#include <math.h>

#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;


#define tolerance 1e-10


Ormsby_Disp_TimeHistory_Penalty::Ormsby_Disp_TimeHistory_Penalty()
    : LoadPattern(0, PATTERN_TAG_Ormsby_Disp_TimeHistory_Penalty),
      nodeTag(0),
      penalty_stiffness(0),
      dof(0),
      tStart(0),
      tFinish(0),
      f1(0.0), f2(0.0),
      f3(0.0), f4(0.0),
      Timemax(0),
      previous_load(0),
      amplitude_max(1.0)
{
    // does nothing
}

Ormsby_Disp_TimeHistory_Penalty::Ormsby_Disp_TimeHistory_Penalty(int tag,
        double PenaltyStiffness,
        int NodeNumber,
        int dof_to_be_shaken,
        double startTime,
        double finishTime,
        double Frequency1,
        double Frequency2,
        double Frequency3,
        double Frequency4,
        double TimeMaximum,
        double Amplitude)
    : LoadPattern(tag, PATTERN_TAG_Ormsby_Disp_TimeHistory_Penalty),
      nodeTag(NodeNumber),
      penalty_stiffness(PenaltyStiffness),
      dof(dof_to_be_shaken),
      tStart(startTime),
      tFinish(finishTime),
      f1(Frequency1), f2(Frequency2),
      f3(Frequency3), f4(Frequency4),
      Timemax(TimeMaximum),
      previous_load(0),
      amplitude_max(Amplitude)
{
    // does nothing
}



Ormsby_Disp_TimeHistory_Penalty::~Ormsby_Disp_TimeHistory_Penalty()
{
    // does nothing
}


void
Ormsby_Disp_TimeHistory_Penalty::setDomain(Domain* theDomain)
{

    this->LoadPattern::setDomain(theDomain);

}


double
Ormsby_Disp_TimeHistory_Penalty::getFactor(double pseudoTime)
{

    static double Pi = 2 * asin(1.0);
    double a1 = f4 - f3;
    double a2 = f2 - f1;
    double b4 = Pi * f4;
    double b3 = Pi * f3;
    double b2 = Pi * f2;
    double b1 = Pi * f1;

    double cFactor = Pi * (f4 + f3 - f2 - f1);

    if (pseudoTime >= tStart && (pseudoTime - tFinish <= tolerance))
    {
        if (fabs(pseudoTime - tStart - Timemax) > tolerance)
        {
            double current_load = (amplitude_max / cFactor) * ( b4 * f4 * sin(b4 * (pseudoTime - tStart - Timemax)) * sin(b4 * (pseudoTime - tStart - Timemax)) / (a1 * b4 * (pseudoTime - tStart - Timemax) * b4 * (pseudoTime - tStart - Timemax)) -
                                  b3 * f3 * sin(b3 * (pseudoTime - tStart - Timemax)) * sin(b3 * (pseudoTime - tStart - Timemax)) / (a1 * b3 * (pseudoTime - tStart - Timemax) * b3 * (pseudoTime - tStart - Timemax)) -
                                  b2 * f2 * sin(b2 * (pseudoTime - tStart - Timemax)) * sin(b2 * (pseudoTime - tStart - Timemax)) / (a2 * b2 * (pseudoTime - tStart - Timemax) * b2 * (pseudoTime - tStart - Timemax)) +
                                  b1 * f1 * sin(b1 * (pseudoTime - tStart - Timemax)) * sin(b1 * (pseudoTime - tStart - Timemax)) / (a2 * b1 * (pseudoTime - tStart - Timemax) * b1 * (pseudoTime - tStart - Timemax)) );


            double difference_load = current_load - previous_load;
            previous_load = current_load;
            return penalty_stiffness * difference_load;
        }
        else
        {
            double current_load = (amplitude_max / cFactor) * Pi * (f4 + f3 - f2 - f1);
            double difference_load = current_load - previous_load;
            previous_load = current_load;
            return penalty_stiffness * difference_load;
        }
    }
    else
    {
        return 0.0;
    }


}


void
Ormsby_Disp_TimeHistory_Penalty::applyLoad(double time)
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
        std::cerr << "Ormsby_Disp_TimeHistory_Penalty - no nodes associtated to the nodeTag " << nodeTag << "\n";
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
Ormsby_Disp_TimeHistory_Penalty::sendSelf(int commitTag, Channel& theChannel)
{
    // to be completed later
    return 0;
}


int
Ormsby_Disp_TimeHistory_Penalty::recvSelf(int commitTag, Channel& theChannel,
        FEM_ObjectBroker& theBroker)
{
    // to be completed later
    return 0;
}


void
Ormsby_Disp_TimeHistory_Penalty::Print(OPS_Stream& s, int flag)
{
    s << "Ormsby_Disp_TimeHistory_Penalty " << this->getTag() << endln;
    s << "Node Number: "   << nodeTag << endln;
    s << "DOF to be Shaken: " << dof << endln;
    s << "Amplitude: " << amplitude_max << endln;
    s << "tStart: " << tStart << endln;
    s << "tFinish: " << tFinish << endln;
    s << "Timemax: " << Timemax << endln;
    s << "Frequency1: " << f3 << endln;
    s << "Frequency2: " << f3 << endln;
    s << "Frequency3: " << f3 << endln;
    s << "Frequency4: " << f4 << endln;

}



// method to obtain a blank copy of the LoadPattern
LoadPattern*
Ormsby_Disp_TimeHistory_Penalty::getCopy(void)
{

    std::cerr << "Ormsby_Disp_TimeHistory_Penalty::getCopy() - not yet implemented\n";
    return 0;
}


