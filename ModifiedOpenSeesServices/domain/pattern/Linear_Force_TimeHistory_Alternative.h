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

#ifndef Linear_Force_TimeHistory_h
#define Linear_Force_TimeHistory_h

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

#include <ID.h>
#include <Vector.h>


// class Vector;

class Linear_Force_TimeHistory : public LoadPattern
{
public:
    Linear_Force_TimeHistory();

    Linear_Force_TimeHistory(int tag,
                             int NodeNumber,
                             int dof_to_be_shaken,
                             double final_load_value);

    ~Linear_Force_TimeHistory();

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

    double final_load;
    int nodeTag;
    int dof;
    double theFactor;
    int isConstant;

};

#endif
