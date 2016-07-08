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

#ifndef SAniSandZ_CPP
#define SAniSandZ_CPP

#include "SAniSandZ.h"
#include "LTensorDisplay.h"
#include <Channel.h>

DTensor2 SAniSandZ::kronecker_delta(3, 3, 0.0);                 // kronecker_delta is the identity matrix

DTensor4 SAniSandZ::Stiffness( 3, 3, 3, 3, 0.0 );

//================================================================================
// Constructors & Destructors
//================================================================================
SAniSandZ::SAniSandZ(int tag,
                     double rho_in,
                     double initial_mean_stress,
                     ... // more params in
                    )
    : NDMaterialLT(tag, ND_TAG_SAniSandZ),
      rho(rho_in),
      ... //initialize params here
      TrialStrain( 3, 3, 0.0 ),
      TrialStress( 3, 3, 0.0 ),
      TrialPlastic_Strain( 3, 3, 0.0 ),
      CommitStress( 3, 3, 0.0 ),
      CommitStrain( 3, 3, 0.0 ),
      CommitPlastic_Strain( 3, 3, 0.0 )
{

    //Initialize identity matrix
    kronecker_delta(0, 0) = 1;
    kronecker_delta(1, 1) = 1;
    kronecker_delta(2, 2) = 1;

    //Initialize trial stress to some small initial isotropic stress
    TrialStress(0, 0) = -initial_mean_stress ;
    TrialStress(1, 1) = -initial_mean_stress ;
    TrialStress(2, 2) = -initial_mean_stress ;

    CommitStress(0, 0) = TrialStress(0, 0);
    CommitStress(1, 1) = TrialStress(1, 1);
    CommitStress(2, 2) = TrialStress(2, 2);

}


SAniSandZ::SAniSandZ() :
    NDMaterialLT(0, ND_TAG_SAniSandZ),
    rho(0),
    ... //initialize params here
    TrialStrain( 3, 3, 0.0 ),
    TrialStress( 3, 3, 0.0 ),
    TrialPlastic_Strain( 3, 3, 0.0 ),
    CommitStress( 3, 3, 0.0 ),
    CommitStrain( 3, 3, 0.0 ),
    CommitPlastic_Strain( 3, 3, 0.0 )
{

    //Initialize identity matrix
    kronecker_delta(0, 0) = 1;
    kronecker_delta(1, 1) = 1;
    kronecker_delta(2, 2) = 1;
}

SAniSandZ::~SAniSandZ()
{


}

//Gets trial strain from element and computes the strain increment and sets it
int SAniSandZ::setTrialStrain(const DTensor2 &new_trial_strain)
{
    static DTensor2 strain_increment( 3, 3, 0.0 );
    strain_increment *= 0;
    TrialStrain(i, j) = new_trial_strain(i, j);
    strain_increment( i, j ) = new_trial_strain( i, j ) - CommitStrain( i, j );

    return setTrialStrainIncr( strain_increment );
}

//Gets the strain increment and calls the integration function (check for quick exit if there is no increment)
int SAniSandZ::setTrialStrainIncr(const DTensor2 &strain_increment)
{
    int exitflag = 0;

    //Check for quick return:
    double eps_norm = strain_increment(i, j) * strain_increment(i, j);
    if (eps_norm == 0)
    {
        return exitflag;
    }

    exitflag = integrate(strain_increment);

    return exitflag;
}

//Return the tangent stiffness to the element
const DTensor4 & SAniSandZ::getTangentTensor(void)
{
    //Stiffness should be set at the end of integrate function
    return this->Stiffness;
}

//Return current trial stress after integration for computation of residual
const DTensor2  & SAniSandZ::getStressTensor(void)
{
    return this->TrialStress;
}


//...
const DTensor2 & SAniSandZ::getStrainTensor(void)
{
    return this->TrialStrain;
}

//Called by output subsystem to store the strain
const DTensor2  & SAniSandZ::getCommittedStressTensor(void)
{

    return this->CommitStress;
}

//Called by output subsystem to store the stress
const DTensor2 & SAniSandZ::getCommittedStrainTensor(void)
{

    return this->CommitStrain;
}

//Called by output subsystem to store the plastic strain
const DTensor2 & SAniSandZ::getCommittedPlasticStrainTensor(void)
{

    return this->CommitPlastic_Strain;
}

//...
const DTensor2  & SAniSandZ::getTrialStressTensor(void)
{
    return this->TrialStress;
}

//...
const DTensor2 & SAniSandZ::getTrialStrainTensor(void)
{

    return this->TrialStrain;
}

//...
const DTensor2 & SAniSandZ::getTrialPlasticStrainTensor(void)
{
    return this->TrialPlastic_Strain;
}


// Once step is accepted by global integration, this is called to commit the current state as a
// converged state, and get ready for next global step.
int SAniSandZ::commitState(void)
{
    CommitStress(i, j)            = TrialStress(i, j);
    CommitStrain(i, j)            = TrialStrain(i, j);
    CommitPlastic_Strain(i, j)    = TrialPlastic_Strain(i, j);

    //More internal variables

    return 0;
}

// Revert to committed state
int SAniSandZ::revertToLastCommit(void)
{

    TrialStress = CommitStress;
    TrialStrain = CommitStrain;
    TrialPlastic_Strain = CommitPlastic_Strain;

    //More internal variables

    return 0;
}

//Return to initial state
int SAniSandZ::revertToStart(void)
{
    CommitPlastic_Strain *= 0;
    TrialStress = CommitStress;
    TrialStrain = CommitStrain;

    return 0;
}

// Copy the material...
NDMaterialLT * SAniSandZ::getCopy(void)
{

    NDMaterialLT *tmp = new SAniSandZ(this->getTag(),
                                      this->getE0(),
                                      this->getv(),
                                      this->getM(),
                                      this->getkd(),
                                      this->getxi(),
                                      this->geth(),
                                      this->getm(),
                                      this->getRho(),
                                      this->getInitialConfiningStress(),
                                      this->getn(),
                                      this->geta(),
                                      this->geteplcum_cr());
    //Should also set other internal variables
    return tmp;
}


//Unused
NDMaterialLT * SAniSandZ::getCopy(const char *code)
{
    return getCopy();
}

//================================================================================
//
//================================================================================
const char *SAniSandZ::getType(void) const
{
    return "ThreeDimensional";
}

//================================================================================
//  For the parallel / storage
//================================================================================

int SAniSandZ::sendSelf(int commitTag, Channel &theChannel)
{
    constexpr int size = 18;   //Set size properly
    static Vector material_parameters(size);

    //Send material parameters
    material_parameters(0) = rho;
    material_parameters(1) = ....; //More parameters

    // DTensor2 material_parameters;
    if ( theChannel.sendVector( 0, commitTag, material_parameters ) < 0 )
    {
        cerr << "WARNING SAniSandZ::sendSelf() - " << this->getTag() << " failed to receive material_parameters\n";
        return -1;
    }

    // DTensor2 TrialStrain
    if ( theChannel.sendDTensor2( 0, commitTag, TrialStrain ) < 0 )
    {
        cerr << "WARNING SAniSandZ::sendSelf() - " << this->getTag() << " failed to send TrialStrain\n";
        return -1;
    }
    // DTensor2 TrialStress
    if ( theChannel.sendDTensor2( 0, commitTag, TrialStress ) < 0 )
    {
        cerr << "WARNING SAniSandZ::sendSelf() - " << this->getTag() << " failed to send TrialStress\n";
        return -1;
    }
    // DTensor2 TrialPlastic_Strain
    if ( theChannel.sendDTensor2( 0, commitTag, TrialPlastic_Strain ) < 0 )
    {
        cerr << "WARNING SAniSandZ::sendSelf() - " << this->getTag() << " failed to send TrialPlastic_Strain\n";
        return -1;
    }

    // DTensor2 CommitStress
    if ( theChannel.sendDTensor2( 0, commitTag, CommitStress ) < 0 )
    {
        cerr << "WARNING SAniSandZ::sendSelf() - " << this->getTag() << " failed to send CommitStress\n";
        return -1;
    }
    // DTensor2 CommitStrain
    if ( theChannel.sendDTensor2( 0, commitTag, CommitStrain ) < 0 )
    {
        cerr << "WARNING SAniSandZ::sendSelf() - " << this->getTag() << " failed to send CommitStrain\n";
        return -1;
    }
    // DTensor2 CommitPlastic_Strain
    if ( theChannel.sendDTensor2( 0, commitTag, CommitPlastic_Strain ) < 0 )
    {
        cerr << "WARNING SAniSandZ::sendSelf() - " << this->getTag() << " failed to send CommitPlastic_Strain\n";
        return -1;
    }

    // Send other internal variables!

    return 0;
}


int SAniSandZ::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    constexpr int size = 18;   //Must match size in sendSelf
    static Vector material_parameters(size);

    //Receive material parameters


    // DTensor2 material_parameters;
    if ( theChannel.receiveVector( 0, commitTag, material_parameters ) < 0 )
    {
        cerr << "WARNING SAniSandZ::receiveSelf() - " << this->getTag() << " failed to receive material_parameters\n";
        return -1;
    }

    rho                   = material_parameters(0);
    ... // More parameters


    // DTensor2 TrialStrain
    if ( theChannel.receiveDTensor2( 0, commitTag, TrialStrain ) < 0 )
    {
        cerr << "WARNING SAniSandZ::receiveSelf() - " << this->getTag() << " failed to receive TrialStrain\n";
        return -1;
    }
    // DTensor2 TrialStress
    if ( theChannel.receiveDTensor2( 0, commitTag, TrialStress ) < 0 )
    {
        cerr << "WARNING SAniSandZ::receiveSelf() - " << this->getTag() << " failed to receive TrialStress\n";
        return -1;
    }
    // DTensor2 TrialPlastic_Strain
    if ( theChannel.receiveDTensor2( 0, commitTag, TrialPlastic_Strain ) < 0 )
    {
        cerr << "WARNING SAniSandZ::receiveSelf() - " << this->getTag() << " failed to receive TrialPlastic_Strain\n";
        return -1;
    }

    // DTensor2 CommitStress
    if ( theChannel.receiveDTensor2( 0, commitTag, CommitStress ) < 0 )
    {
        cerr << "WARNING SAniSandZ::receiveSelf() - " << this->getTag() << " failed to receive CommitStress\n";
        return -1;
    }
    // DTensor2 CommitStrain
    if ( theChannel.receiveDTensor2( 0, commitTag, CommitStrain ) < 0 )
    {
        cerr << "WARNING SAniSandZ::receiveSelf() - " << this->getTag() << " failed to receive CommitStrain\n";
        return -1;
    }
    // DTensor2 CommitPlastic_Strain
    if ( theChannel.receiveDTensor2( 0, commitTag, CommitPlastic_Strain ) < 0 )
    {
        cerr << "WARNING SAniSandZ::receiveSelf() - " << this->getTag() << " failed to receive CommitPlastic_Strain\n";
        return -1;
    }

    // Receive more internal variables!


    return 0;
}

//Print useful debugging info
void SAniSandZ::Print(ostream &s, int flag)
{
    s << (*this);

}

//================================================================================
//================================================================================
//================================================================================
//  Plasticity!
//================================================================================
//================================================================================
//================================================================================

int SAniSandZ::integrate(const DTensor2& strain_increment)

{

    // Here is where integration should happen!!
    // Return negative value if this fails.


    return 0;
}

//================================================================================
//================================================================================
//================================================================================
//================================================================================
//================================================================================
//================================================================================



//Compute memory usage of the object. Used for parallel.
int SAniSandZ::getObjectSize()
{

    int size = 0;

    size += sizeof(double) * strainplcum.get_size();
    size += sizeof(double) * TrialStrain.get_size();
    size += sizeof(double) * TrialStress.get_size();
    size += sizeof(double) * TrialPlastic_Strain.get_size();
    size += sizeof(double) * CommitStress.get_size();
    size += sizeof(double) * CommitStrain.get_size();
    size += sizeof(double) * CommitPlastic_Strain.get_size();
    size += sizeof(i);
    size += sizeof(j);
    size += sizeof(k);
    size += sizeof(l);

    //Add other internal variables

    return size;
}


int SAniSandZ::startNewStage()
{
    return 0;
}



#endif

