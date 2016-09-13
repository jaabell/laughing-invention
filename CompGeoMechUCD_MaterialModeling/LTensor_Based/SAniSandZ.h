///////////////////////////////////////////////////////////////////////////////
// LEGACY/DEFUNCT COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):  Version of Creative Commons (for details contact Boris Jeremic jeremic@ucdavis.edu)
// PROJECT:        Real ESSI Simulator
// PROGRAMMER:     Alexandros Petalas, Jose Abell, Boris Jeremic
// DATE:           July 2016
// UPDATE HISTORY: Full update history in git repository.
//
/////////////////////////////////////////////////////////////////////////////



#ifndef SAniSandZ_H
#define SAniSandZ_H


#include <G3Globals.h>
#include "NDMaterialLT.h"
#include "../../ltensor/LTensor.h"
#include <iostream>
#include <Channel.h>

using namespace std;

#ifdef DEACTIVATETHIS

class SAniSandZ : public NDMaterialLT
{

public:

    SAniSandZ( int tag,
               double rho_in,
               double initial_mean_stress,
               .... //more parameters
             );

    SAniSandZ(void);

    ~SAniSandZ(void);

    const char *getClassType(void) const
    {
        return "SAniSandZ";
    };



    // methods to set and retrieve state using the Tensor class
    int setTrialStrain( const DTensor2 &v );         // Used
    int setTrialStrainIncr( const DTensor2 &v );     // Not used (used by setTrialStrain)


    virtual const DTensor4 &getTangentTensor( void );
    virtual const DTensor2 &getStressTensor( void );
    virtual const DTensor2 &getStrainTensor( void );
    virtual const DTensor2 &getPlasticStrainTensor( void );

    const DTensor2  &getCommittedStressTensor(void);

    const DTensor2 &getCommittedStrainTensor(void);

    const DTensor2 &getCommittedPlasticStrainTensor(void);

    const DTensor2  &getTrialStressTensor(void);

    const DTensor2 &getTrialStrainTensor(void);

    const DTensor2 &getTrialPlasticStrainTensor(void);

    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);

    NDMaterialLT *getCopy(void);
    NDMaterialLT *getCopy(const char *code);

    const char *getType(void) const;

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
    void Print(ostream &s, int flag = 0);

    int getObjectSize();


private:
    //Place here the internal functions

    int integrate(const DTensor2& strain_increment);



private:
    //Model parameters
    double rho;                 //
    ... // more parameters



    DTensor2 TrialStrain;
    DTensor2 TrialStress;
    DTensor2 TrialPlastic_Strain;

    DTensor2 CommitStress;
    DTensor2 CommitStrain;
    DTensor2 CommitPlastic_Strain;

    //Fourth order stiffness
    static DTensor4 Stiffness;

    static DTensor2 kronecker_delta;

    //Global indexes for LTensor
    Index < 'i' > i;
    Index < 'j' > j;
    Index < 'k' > k;
    Index < 'l' > l;
};

#endif
#endif
