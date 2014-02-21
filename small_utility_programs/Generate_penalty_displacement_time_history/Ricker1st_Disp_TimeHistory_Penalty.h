//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  Frequency,  and anybody
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

#ifndef Ricker1st_Disp_TimeHistory_Penalty_h
#define Ricker1st_Disp_TimeHistory_Penalty_h

// Purpose:
#include <LoadPattern.h>
#include <Matrix.h>
#include <Domain.h>
#include <NodeIter.h>
#include <Node.h>
#include <stdlib.h>
#include <Channel.h>
#include <ErrorHandler.h>

#include <OPS_Globals.h>

class Vector;

class Ricker1st_Disp_TimeHistory_Penalty : public LoadPattern
{
    public:
        Ricker1st_Disp_TimeHistory_Penalty();

        Ricker1st_Disp_TimeHistory_Penalty(int tag,
                                           double PenaltyStiffness,
                                           int NodeNumber,
                                           int dof_to_be_shaken,
                                           double startTime,
                                           double finishTime,
                                           double Frequency,
                                           double TimeMaximum,
                                           double Amplitude);

        ~Ricker1st_Disp_TimeHistory_Penalty();

        void setDomain(Domain* theDomain);
        double getFactor(double pseudoTime);
        void applyLoad(double time);
        void Print(OPS_Stream& s, int flag = 0);

        // methods for o/p
        int sendSelf(int commitTag, Channel& theChannel);
        int recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

        //  method to obtain a blank copy of the LoadPattern
        LoadPattern* getCopy(void);


    protected:


    private:

        double tStart;    // start time of the pulse
        double tFinish;   // finish time of the pulse
        double frequency;
        double Timemax;
        double cFactor;
        int nodeTag;
        double penalty_stiffness;
        int dof;
        double previous_load;

};

#endif
