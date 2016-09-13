///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli , Boris Jeremic
// DATE:               May 2012
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////


#ifndef PathSeries_Disp_TimeHistory_Penalty_h
#define PathSeries_Disp_TimeHistory_Penalty_h

// Purpose:
#include <LoadPattern.h>
#include <Matrix.h>
#include <Domain.h>
#include <NodeIter.h>
#include <Node.h>
#include <stdlib.h>
#include <Channel.h>
#include <ErrorHandler.h>
#include <ID.h>

#include <OPS_Globals.h>

class Vector;

class PathSeries_Disp_TimeHistory_Penalty : public LoadPattern
{
public:
    PathSeries_Disp_TimeHistory_Penalty();

    PathSeries_Disp_TimeHistory_Penalty(int tag,
                                        double PenaltyStiffness,
                                        int NodeNumber,
                                        int dof_to_be_shaken,
                                        double theFactor,
                                        const char* fileName);

    PathSeries_Disp_TimeHistory_Penalty(int tag,
                                        double PenaltyStiffness,
                                        int NodeNumber,
                                        int dof_to_be_shaken,
                                        double theTimeIncr,
                                        double theFactor,
                                        const char* fileName);

    ~PathSeries_Disp_TimeHistory_Penalty();

    void setDomain(Domain* theDomain);
    double getFactor(double pseudoTime);
    void applyLoad(double time);
    void Print(ostream& s, int flag = 0);
    void setLoadConstant(void);

    // methods for o/p
    int sendSelf(int commitTag, Channel& theChannel);
    int receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

    //  method to obtain a blank copy of the LoadPattern
    LoadPattern* getCopy(void);


protected:


private:

    Vector* thePath;      // vector containg the data points
    Vector* time;         // vector containg the time values of data points
    int currentTimeLoc;   // current location in time
    double cFactor;       // additional factor on the returned load factor
    double pathTimeIncr;  // specifies the time increment used in load path vector
    double penalty_stiffness;
    int nodeTag;
    int dof;
    double loadvalue_previous;
    int isConstant;

};

#endif
