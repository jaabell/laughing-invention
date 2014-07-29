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

#ifndef PathSeries_Force_TimeHistory_h
#define PathSeries_Force_TimeHistory_h

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

class PathSeries_Force_TimeHistory : public LoadPattern
{
    public:
        PathSeries_Force_TimeHistory();

        PathSeries_Force_TimeHistory(int tag,
                                     int NodeNumber,
                                     int dof_to_be_shaken,
                                     double theFactor,
                                     const char *fileName);

        PathSeries_Force_TimeHistory(int tag,
                                     int NodeNumber,
                                     int dof_to_be_shaken,
                                     double theTimeIncr,
                                     double theFactor,
                                     const char *fileName);

        PathSeries_Force_TimeHistory(int tag,
                                     int NodeNumber,
                                     int dof_to_be_shaken,
                                     double theTimeIncr,
                                     double theFactor,
                                     int currentTimeLoc_,
                                     int isConstant_,
                                     double loadValue_,
                                     Vector *thePath_,
                                     Vector *time_);




        ~PathSeries_Force_TimeHistory();

        void setDomain(Domain *theDomain);
        double getFactor(double pseudoTime);
        void applyLoad(double time);
        void Print(ostream &s, int flag = 0);
        void setLoadConstant(void);

        // methods for o/p
        int sendSelf(int commitTag, Channel &theChannel);
        int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

        //  method to obtain a blank copy of the LoadPattern
        LoadPattern *getCopy(void);


    protected:


    private:


        Vector *thePath;      // vector containg the data points
        Vector *time;         // vector containg the time values of data points
        int currentTimeLoc;   // current location in time
        double cFactor;       // additional factor on the returned load factor
        double pathTimeIncr;  // specifies the time increment used in load path vector

        int nodeTag;
        int dof;
        double loadValue;
        int isConstant;

};

#endif
