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

#ifndef Ormsby_Force_TimeHistory_h
#define Ormsby_Force_TimeHistory_h

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

class Ormsby_Force_TimeHistory : public LoadPattern
{
    public:
        Ormsby_Force_TimeHistory();

        Ormsby_Force_TimeHistory(int tag,
                                 int NodeNumber,
                                 int dof_to_be_shaken,
                                 double startTime,
                                 double finishTime,
                                 double Frequency1,
                                 double Frequency2,
                                 double Frequency3,
                                 double Frequency4,
                                 double TimeMaximum,
                                 double Amplitude);

        ~Ormsby_Force_TimeHistory();

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

        double tStart;
        double tFinish;
        double Timemax;
        double f1;
        double f2;
        double f3;
        double f4;
        double amplitude_max;
        int nodeTag;
        int dof;

};

#endif
