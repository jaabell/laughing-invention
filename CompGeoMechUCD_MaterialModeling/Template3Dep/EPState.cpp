/*
//================================================================================
# COPYRIGHT (C):     :-))                                                        #
# PROJECT:           Object Oriented Finite Element Program                      #
# PURPOSE:           General platform for elaso-plastic constitutive model       #
#                    implementation                                              #
# CLASS:             DPEPState (the base class for all Elasto-plastic state)     #
#                                                                                #
# VERSION:                                                                       #
# LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 )    #
# TARGET OS:         DOS || UNIX || . . .                                        #
# DESIGNER(S):       Boris Jeremic, Zhaohui Yang                                 #
# PROGRAMMER(S):     Boris Jeremic, Zhaohui Yang                                 #
#                                                                                #
#                                                                                #
# DATE:              08-03-2000                                                  #
# UPDATE HISTORY:    May 2004 Guanzhou changed Commit to be consistent           #
#                             with theory                                        #
#            May 2004, Zhao Cheng splitted the elastic part          #
#                                                                                #
#                                                                                #
#                                                                                #
# SHORT EXPLANATION: This class is used to hold all state parameters and internal#
#                    variables in an elasto-plastic constitutive model!          #
#                                                                                #
//================================================================================
*/

#ifndef EPState_CPP
#define EPState_CPP

#include "EPState.h"
#include <iostream>
using namespace std;


//Guanzhou
#include <ID.h>
#include <Vector.h>
//stresstensor EPState::TensorVar[ 4 ];
//stresstensor EPState::TensorVar_commit[ 4 ];
//stresstensor EPState::TensorVar_init[ 4 ];

//tensor  EPState::Eep( 4, def_dim_4, 0.0 );
//tensor  EPState::Eep_commit( 4, def_dim_4, 0.0 );
//tensor  EPState::Eep_init( 4, def_dim_4, 0.0 );

//================================================================================
//Normal Constructor 1
//================================================================================

//ZC05/2004 EPState::EPState(double               Eod,
//ZC05/2004                  double               Ed,
//ZC05/2004                  double               nu,
//ZC05/2004                  double               rho,
EPState::EPState(
    const stresstensor&  stressp,
    const straintensor&  strainp,
    const straintensor&  Estrainp,
    const straintensor&  Pstrainp,
    const straintensor&  dEstrainp,
    const straintensor&  dPstrainp,
    int                  NScalarp,
    const double*        Scalarp,
    int                  NTensorp,
    const stresstensor* Tensorp,
    const tensor&        Eepp,
    const stresstensor& Stress_commitp,
    const straintensor& Strain_commitp,
    const straintensor& ElasticStrain_commitp,
    const double*        Scalar_commitp,
    const stresstensor* Tensor_commitp,
    const tensor&        Eep_commitp,
    const stresstensor& Stress_initp,
    const straintensor& Strain_initp,
    const double*        Scalar_initp,
    const stresstensor* Tensor_initp,
    const tensor&        Eep_initp,
    bool                 Convergedp,
    //ZC05/2004           int                  Elasticflagp,
    //ZC05/2004           double         Evp,
    //ZC05/2004           double               nuhvp,
    //ZC05/2004           double               Ghvp,
    //ZC05/2004           double               eop,
    //ZC05/2004           double               ecp,
    //ZC05/2004           double               Lamp,
    //ZC05/2004           double               pop,
    double               ep,
    double               psip,
    int flag,
    int tag)
//ZC05/2004           double          ap
//ZC05/2004     )
//ZC05/2004: Eo(Eod), E_Young(Ed), nu_Poisson(nu), rho_mass_density(rho), CurrentStress(stressp),
//ZC05/2004  CurrentStrain(strainp), ElasticStrain(Estrainp), PlasticStrain(Pstrainp),
//ZC05/2004  dElasticStrain(dEstrainp), dPlasticStrain(dPstrainp), Eep(Eepp),
//ZC05/2004  Stress_commit(Stress_commitp), Strain_commit(Strain_commitp),
//ZC05/2004  Eep_commit(Eep_commitp), Stress_init(Stress_initp), Strain_init(Strain_initp),
//ZC05/2004  Eep_init(Eep_initp), Converged (Convergedp),
//ZC05/2004  Elasticflag(Elasticflagp),Ev(Evp),nuhv(nuhvp),Ghv(Ghvp),
//ZC05/2004  eo(eop), ec(ecp), Lambda(Lamp), po(pop), e(ep), psi(psip), a(ap)
    : TaggedObject(tag), MovableObject(TAG_EPState), CurrentStress(stressp), //Guanzhou changed
      CurrentStrain(strainp), ElasticStrain(Estrainp), PlasticStrain(Pstrainp),
      dElasticStrain(dEstrainp), dPlasticStrain(dPstrainp), Eep(Eepp),
      Stress_commit(Stress_commitp), Strain_commit(Strain_commitp), ElasticStrain_commit(ElasticStrain_commitp),
      Eep_commit(Eep_commitp), Stress_init(Stress_initp), Strain_init(Strain_initp),
      Eep_init(Eep_initp), Converged (Convergedp), integratorFlag(flag), e(ep), psi(psip)
{

    //Eo               = Eod;
    //E_Young          = Ed;
    //nu_Poisson       = nu;
    //rho_mass_density = rho;
    Delta_lambda = 0.0;

    NScalarVar = NScalarp;

    //ScalarVar = new double[ NScalarVar ];
    //if ( !ScalarVar ) {
    if ( ( !Scalarp ) && (NScalarVar != 0) )
    {
        cerr << "EPState::EPState   No initial values for scalar hardening vars, set to aero\n";
        //::exit(1);
    }
    else
    {
        for (int i = 0; i < NScalarVar; i++)
        {
            //cerr << Scalarp[i] << endlnn;
            ScalarVar[i] = Scalarp[i];
            ScalarVar_commit[i] = Scalar_commitp[i];
            ScalarVar_init[i] = Scalar_initp[i];
        }
    }

    NTensorVar = NTensorp;
    //TensorVar = new stresstensor[ NTensorVar ];
    //if ( !TensorVar ) {
    //   g3ErrorHandler->fatal("EPState::EPState insufficient memory for Tensor hardening vars");
    //   ::exit(1);
    //}

    if ( (!Tensorp ) && ( NTensorVar))
    {
        //ScalarVar = new double[ NScalarVar ];
        //if ( !ScalarVar ) {
        cerr << "EPState::EPState   No initial values for tensorial hardening vars, set to zero\n";
        //::exit(1);
    }
    else
    {
        for (int i = 0; i < NTensorVar; i++)
        {
            //cerr << Tensorp[i];
            TensorVar[i] = Tensorp[i];
            TensorVar_commit[i] = Tensor_commitp[i];
            TensorVar_init[i] = Tensor_initp[i];
            //cerr << TensorVar[i];
            //TensorVar[i].null_indices();
        }
    }
}

//================================================================================
//Normal Constructor 11
//================================================================================

//ZC05/2004EPState::EPState(double              Eod,
//ZC05/2004                 double              Ed,
//ZC05/2004                 double              nu,
//ZC05/2004                 double              rho,
EPState::EPState(
    const stresstensor  stressp,
    const straintensor  strainp,
    const straintensor  Estrainp,
    const straintensor  Pstrainp,
    int                 NScalarp,
    const double*       Scalarp,
    int                 NTensorp,
    const stresstensor* Tensorp,
    double             ep,
    double             psip,
    int flag,
    int tag)
    : TaggedObject(tag), MovableObject(TAG_EPState)
    //ZC05/2004                 int                 Elasticflagp,
    //ZC05/2004                 double              Evp,
    //ZC05/2004                 double              nuhvp,
    //ZC05/2004                 double              Ghvp,
    //ZC05/2004                 double              eop,
    //ZC05/2004                 double              ecp,
    //ZC05/2004                 double              Lamp,
    //ZC05/2004                 double              pop,
    //ZC05/2004                 double              ap
    //ZC05/2004     )
    //: Eo(Eod), E_Young(Ed), nu_Poisson(nu), rho_mass_density(rho),
    //  CurrentStress(stressp), CurrentStrain(strainp), ElasticStrain(Estrainp),
    //  PlasticStrain(Pstrainp), Stress_commit(stressp), Strain_commit(strainp),
    //  Stress_init(stressp), Strain_init(strainp),
    //  Elasticflag(Elasticflagp),Ev(Evp),nuhv(nuhvp),Ghv(Ghvp),
    //  eo(eop), ec(ecp), Lambda(Lamp),po(pop), e(eop), a(ap)
{

    //ZC05/2004Eo  = Eod;
    //ZC05/2004E_Young  = Ed;
    //ZC05/2004nu_Poisson  = nu;
    //ZC05/2004rho_mass_density  = rho;

    CurrentStress  = stressp;
    CurrentStrain  = strainp;
    ElasticStrain  = Estrainp;

    PlasticStrain  = Pstrainp;
    Stress_commit  = stressp;
    Strain_commit  = strainp;
    ElasticStrain_commit = Estrainp;

    Stress_init  = stressp;
    Strain_init  = strainp;
    integratorFlag = flag;
    Delta_lambda = 0.0;

    //ZC05/2004Elasticflag  = Elasticflagp;
    //ZC05/2004Ev  = Evp;
    //ZC05/2004nuhv  = nuhvp;
    //ZC05/2004Ghv  = Ghvp;

    //ZC05/2004eo  = eop;
    //ZC05/2004ec  = ecp;
    //ZC05/2004Lambda  = Lamp;
    //ZC05/2004po  = pop;
    //ZC05/2004e  = eop;
    //ZC05/2004a  = ap;

    e = ep;    //ZC
    psi = psip;  //ZC

    //Eo               = Eod;
    //E_Young          = Ed;
    //nu_Poisson       = nu;
    //rho_mass_density = rho;

    //CurrentStress    = stressp;

    //cerr << "stressp " << stressp;
    //CurrentStress.null_indices();

    //CurrentStrain    =  strainp;
    //ElasticStrain    =  Estrainp;
    //PlasticStrain    =  Pstrainp;
    //Eep = Eepp;
    Eep = tensor( 4, def_dim_4, 0.0 ); // need to be initialized as 4th order tensor
    //Guanzhou added for PDD
    Eep_init = Eep;
    Eep_commit = Eep;

    //cerr << "strainp " << strainp;
    //CurrentStrain.null_indices();

    NScalarVar = NScalarp;

    //ScalarVar = new double[ NScalarVar ];
    //if ( !ScalarVar ) {
    if ( ( !Scalarp ) && (NScalarVar != 0) )
    {
        cerr << "EPState::EPState   No initial values for scalar hardening vars, set to zero\n";
        //::exit(1);
    }
    else
    {
        for (int i = 0; i < NScalarVar; i++)
        {
            //cerr << Scalarp[i] << endlnn;
            ScalarVar[i] = Scalarp[i];
            ScalarVar_commit[i] = Scalarp[i];
            ScalarVar_init[i] = Scalarp[i];
        }
    }


    NTensorVar = NTensorp;
    //TensorVar = new stresstensor[ NTensorVar ];
    //if ( !TensorVar ) {
    //   g3ErrorHandler->fatal("EPState::EPState insufficient memory for Tensor hardening vars");
    //   ::exit(1);
    //}

    if ( ( !Tensorp ) && ( NTensorVar != 0 ) )
    {
        //ScalarVar = new double[ NScalarVar ];
        //if ( !ScalarVar ) {
        cerr << "EPState::EPState   No initial values for tensorial hardening vars, set to zero\n";
        //::exit(1);
    }
    else
    {
        for (int i = 0; i < NTensorVar; i++)
        {
            //cerr << Tensorp[i] << endlnn;
            //cerr << TensorVar[i] << endlnn;
            TensorVar[i] = Tensorp[i];
            TensorVar_commit[i] = Tensorp[i];
            TensorVar_init[i] = Tensorp[i];
            //cerr << TensorVar[i] << endlnn;
            //TensorVar[i].null_indices();
        }
    }

    Converged = false;
    //ZC05/2004     psi = e - ec;
}

//================================================================================
//Normal Constructor 2
//================================================================================

//ZC05/2004EPState::EPState(double              Eod,
//ZC05/2004                 double              Ed,
//ZC05/2004                 double              nu,
//ZC05/2004                 double              rho,
EPState::EPState(
    int                 NScalarp,
    const double*       Scalarp,
    int                 NTensorp,
    const stresstensor* Tensorp,
    //ZC05/2004           int                 Elasticflagp,
    //ZC05/2004           double        Evp,
    //ZC05/2004           double              nuhvp,
    //ZC05/2004           double              Ghvp,
    //ZC05/2004           double              eop,
    //ZC05/2004           double              ecp,
    //ZC05/2004           double              Lamp,
    //ZC05/2004           double              pop,
    //ZC05/2004           double         ap
    int tag)
    : TaggedObject(tag), MovableObject(TAG_EPState) //Guanzhou added
    //: Eo(Eod), E_Young(Ed), nu_Poisson(nu), rho_mass_density(rho),
    //  Elasticflag(Elasticflagp),Ev(Evp),nuhv(nuhvp),Ghv(Ghvp),
    //  eo(eop), ec(ecp), Lambda(Lamp), po(pop), e(eop), a(ap)
{

    //ZC05/2004Eo = Eod;
    //ZC05/2004E_Young = Ed;
    //ZC05/2004nu_Poisson = nu;
    //ZC05/2004rho_mass_density = rho;
    //ZC05/2004Elasticflag = Elasticflagp;
    //ZC05/2004Ev = Evp;
    //ZC05/2004nuhv = nuhvp;
    //ZC05/2004Ghv = Ghvp;
    //ZC05/2004eo = eop;
    //ZC05/2004ec = ecp;
    //ZC05/2004Lambda = Lamp;
    //ZC05/2004po = pop;
    //ZC05/2004e = eop;
    //ZC05/2004a = a;

    e = 0.85;    //ZC
    psi  = 0.05; //ZC
    integratorFlag = 0;
    Delta_lambda = 0.0;

    //Eo               = Eod;
    //E_Young          = Ed;
    //nu_Poisson       = nu;
    //rho_mass_density = rho;

    //CurrentStress    = stressp;
    //cerr << "CurrentStress " << CurrentStress;
    //CurrentStress.null_indices();
    //CurrentStrain    =  strainp;
    //ElasticStrain    =  Estrainp;
    //PlasticStrain    =  Pstrainp;
    //dElasticStrain   =  dEstrainp;
    //dPlasticStrain   =  dPstrainp;

    Eep = tensor( 4, def_dim_4, 0.0 ); // need to be initialized as 4th order tensor
    //cerr << "strainp " << strainp;
    //CurrentStrain.null_indices();

    //Guanzhou added for PDD
    Eep_init = Eep;
    Eep_commit = Eep;

    NScalarVar  =  NScalarp;

    //ScalarVar = new double[ NScalarVar ];
    //if ( !ScalarVar ) {
    //   g3ErrorHandler->fatal("EPState::EPState insufficient memory for Scalar hardening vars");
    //   ::exit(1);
    //}
    if ( (!Scalarp) && (NScalarVar != 0) )
    {
        cerr << "EPState::EPState   No initial values for scalar hardening vars, set to zero\n";
        //::exit(1);
    }
    else
    {
        for (int i = 0; i < NScalarVar; i++)
        {
            //cerr << Scalarp[i] << endlnn;
            ScalarVar[i] = Scalarp[i];
            ScalarVar_commit[i] = Scalarp[i];
            ScalarVar_init[i] = Scalarp[i];
        }
    }

    NTensorVar = NTensorp;

    //TensorVar = new stresstensor[ NTensorVar ];
    //if ( !TensorVar ) {
    //   g3ErrorHandler->fatal("EPState::EPState insufficient memory for Tensor hardening vars");
    //   ::exit(1);
    //}
    if ( (!Scalarp) && ( NTensorVar != 0 ))
    {
        cerr << "EPState::EPState   No initial values for tensorial hardening vars, set to zero\n";
        //::exit(1);
    }
    else
    {
        for (int i = 0; i < NTensorVar; i++)
        {
            //cerr << Tensorp[i];
            TensorVar[i] = Tensorp[i];
            TensorVar_commit[i] = Tensorp[i];
            TensorVar_init[i] = Tensorp[i];
            //cerr << TensorVar[i];
            //TensorVar[i].null_indices();
        }
    }

    Converged = false;
    //ZC05/2004      psi = e - ec;
}


//================================================================================
//Normal Constructor --no parameters
//================================================================================

EPState::EPState()
//ZC05/2004: Eo(30000.0), E_Young(30000.0), nu_Poisson(0.3), rho_mass_density(0.0),
//ZC05/2004  Converged(false),
//ZC05/2004  Elasticflag(0),Ev(0.0),nuhv(0.0),Ghv(0.0),
//ZC05/2004  eo(0.85), ec(0.80), Lambda(0.025), po(100.0), e(0.85), psi(0.05), a(0.5)
    : TaggedObject(0), MovableObject(TAG_EPState), Converged(false), e(0.85), psi(0.05) //ZC05/2004

{

    //Eo               = 30000.0;
    //E_Young          = 30000.0;
    //nu_Poisson       = 0.3;
    //rho_mass_density = 0.0;
    Eep = tensor( 4, def_dim_4, 0.0 );
    integratorFlag = 0;//ForwardEuler assumed

    //Guanzhou added for PDD
    Eep_init = Eep;
    Eep_commit = Eep;

    NScalarVar = MaxNScalarVar;

    for (int i = 0; i < NScalarVar; i++)
    {
        ScalarVar_init[i] = 0.0;
        ScalarVar[i] = 0.0;
        ScalarVar_commit[i] = 0.0;
    }

    NTensorVar = MaxNTensorVar;

    for (int i = 0; i < NTensorVar; i++)
    {
        TensorVar[i] = stresstensor(0.0);
    }

    Delta_lambda = 0.0;

    //Converged = false;

}

//================================================================================
//create a clone of itself
//================================================================================

EPState* EPState::newObj()
{

    //ZC05/2004      EPState * eps = new  EPState(this->getEo(),
    //ZC05/2004                 this->getE(),
    //ZC05/2004                 this->getnu(),
    //ZC05/2004                 this->getrho(),
    EPState* eps = new  EPState(
        this->getStress(),
        this->getStrain(),
        this->getElasticStrain(),
        this->getPlasticStrain(),
        this->getdElasticStrain(),
        this->getdPlasticStrain(),
        this->getNScalarVar(),
        this->getScalarVar(),
        this->getNTensorVar(),
        this->getTensorVar(),
        this->getEep(),
        this->getStress_commit(),
        this->getStrain_commit(),
        this->getElasticStrain_commit(),
        this->getScalarVar_commit(),
        this->getTensorVar_commit(),
        this->getEep_commit(),
        this->getStress_init(),
        this->getStrain_init(),
        this->getScalarVar_init(),
        this->getTensorVar_init(),
        this->getEep_init(),
        this->getConverged(),
        //ZC05/2004                 this->getElasticflag(),
        //ZC05/2004                 this->getEv(),
        //ZC05/2004                 this->getnuhv(),
        //ZC05/2004                 this->getGhv(),
        //ZC05/2004                 this->geteo(),
        //ZC05/2004                 this->getec(),
        //ZC05/2004                 this->getLam(),
        //ZC05/2004                 this->getpo(),
        this->gete(),
        this->getpsi(),
        this->getIntegratorFlag()
        //ZC05/2004                 this->geta()
    );
    eps->Delta_lambda = this->Delta_lambda;

    return eps;
}


//================================================================================
// Copy constructor
//================================================================================
EPState::EPState( const EPState& rhs ) : TaggedObject(rhs.getTag()), MovableObject(TAG_EPState)
{
    //ZC05/2004       Eo               = rhs.getEo();
    //ZC05/2004       E_Young          = rhs.getE();
    //ZC05/2004       nu_Poisson       = rhs.getnu();
    //ZC05/2004       rho_mass_density = rhs.getrho();
    CurrentStress    = rhs.getStress();
    CurrentStrain    = rhs.getStrain();
    ElasticStrain    = rhs.getElasticStrain();
    PlasticStrain    = rhs.getPlasticStrain();
    dElasticStrain   = rhs.getdElasticStrain();
    dPlasticStrain   = rhs.getdPlasticStrain();

    Stress_commit = rhs.getStress_commit();
    Strain_commit = rhs.getStrain_commit();
    ElasticStrain_commit = rhs.getElasticStrain_commit();
    Stress_init   = rhs.getStress_init();
    Strain_init   = rhs.getStrain_init();
    //cerr << Eep.rank() << " ";
    //Eep.printshort("before copy con ");
    Eep = rhs.getEep();
    Eep_commit = rhs.getEep_commit();
    Eep_init = rhs.getEep_init();
    //cerr << Eep.rank() << " ";
    //Eep.printshort("after copy con ");

    NScalarVar  =  rhs.getNScalarVar();
    //ScalarVar = new double[ NScalarVar ];
    //if ( !ScalarVar ) {
    //   g3ErrorHandler->fatal("EPState::EPState insufficient memory for Scalar hardening vars");
    //   ::exit(1);
    //}
    int i;

    for (i = 0; i < NScalarVar; i++)
    {
        ScalarVar[i] = rhs.ScalarVar[ i ];
        ScalarVar_commit[i] = rhs.ScalarVar_commit[ i ];
        ScalarVar_init[i] = rhs.ScalarVar_init[ i ];
    }

    NTensorVar = rhs.getNTensorVar();

    //TensorVar = new stresstensor[ NTensorVar ];
    //if ( !TensorVar ) {
    //   g3ErrorHandler->fatal("EPState::EPState insufficient memory for Tensor hardening vars");
    //   ::exit(1);
    //}
    for (i = 0; i < NTensorVar; i++)
    {
        TensorVar[i] = rhs.TensorVar[ i ];
        TensorVar_commit[i] = rhs.TensorVar_commit[ i ];
        TensorVar_init[i] = rhs.TensorVar_init[ i ];
        //cerr << TensorVar[i];
        //TensorVar[i].null_indices();
    }

    Converged = rhs.getConverged();

    //ZC05/2004       Elasticflag = rhs.getElasticflag();
    //ZC05/2004       Ev        = rhs.getEv();
    //ZC05/2004       nuhv      = rhs.getnuhv();
    //ZC05/2004       Ghv       = rhs.getGhv();

    //ZC05/2004       eo        = rhs.geteo();
    //ZC05/2004       ec        = rhs.getec();
    //ZC05/2004       Lambda    = rhs.getLam();
    //ZC05/2004       po        = rhs.getpo();
    e         = rhs.gete();
    psi       = rhs.getpsi();
    Delta_lambda = rhs.Delta_lambda; //Guanzhou
    integratorFlag = rhs.getIntegratorFlag();
    //ZC05/2004       a         = rhs.geta();

}

//================================================================================
// Destructor
//================================================================================
EPState::~EPState()
{

    //if ( ScalarVar )
    //  delete [] ScalarVar;
    //if ( TensorVar )
    //  delete TensorVar;

    //if ( ScalarVar_commit )
    //  delete [] ScalarVar_commit;
    //if ( TensorVar_commit )
    //  delete TensorVar_commit;

    //if ( ScalarVar_init )
    //  delete [] ScalarVar_init;
    //if ( TensorVar_init )
    //  delete TensorVar_init;

}


//================================================================================
//Overloading the assignment sign
//================================================================================
const EPState& EPState::operator=(const EPState& rhs )
{

    if ( this != &rhs )
    {
        //ZC05/2004          Eo               = rhs.getEo();
        //ZC05/2004          E_Young          = rhs.getE();
        //ZC05/2004          nu_Poisson       = rhs.getnu();
        //ZC05/2004          rho_mass_density = rhs.getrho();

        CurrentStress    = rhs.getStress();
        //cerr << "Current stress " << CurrentStress;
        CurrentStrain    = rhs.getStrain();
        //cerr << "strainp " << strainp;
        ElasticStrain    = rhs.getElasticStrain();
        PlasticStrain    = rhs.getPlasticStrain();
        dElasticStrain   = rhs.getdElasticStrain();
        dPlasticStrain   = rhs.getdPlasticStrain();

        Stress_commit = rhs.getStress_commit();
        Strain_commit = rhs.getStrain_commit();
        ElasticStrain_commit = rhs.getElasticStrain_commit();
        Stress_init   = rhs.getStress_init();
        Strain_init   = rhs.getStrain_init();

        Eep              = rhs.getEep();
        Eep_commit = rhs.getEep_commit();
        Eep_init = rhs.getEep_init();

        NScalarVar  =  rhs.getNScalarVar();
        //ScalarVar = new double[ NScalarVar ];
        //if ( !ScalarVar ) {
        //   g3ErrorHandler->fatal("EPState::operator= insufficient memory for Scalar hardening vars");
        //   ::exit(1);
        //}
        int i;

        for (i = 0; i < NScalarVar; i++)
        {
            ScalarVar[i] = rhs.ScalarVar[i];
            ScalarVar_commit[i] = rhs.ScalarVar_commit[i];
            ScalarVar_init[i] = rhs.ScalarVar_init[i];
        }

        NTensorVar = rhs.getNTensorVar();

        //TensorVar = new stresstensor[ NTensorVar ];
        //if ( !TensorVar ) {
        //   g3ErrorHandler->fatal("EPState::operator= insufficient memory for Tensor hardening vars");
        //   ::exit(1);
        //}
        for (i = 0; i < NTensorVar; i++)
        {
            TensorVar[i] = rhs.TensorVar[i];
            TensorVar_commit[i] = rhs.TensorVar_commit[i];
            TensorVar_init[i] = rhs.TensorVar_init[i];
            //TensorVar[i].null_indices();
        }

        Converged = rhs.getConverged();
        Delta_lambda = rhs.Delta_lambda; //Guanzhou

        //ZC05/2004          Elasticflag = rhs.getElasticflag();
        //ZC05/2004          Ev        = rhs.getEv();
        //ZC05/2004          nuhv      = rhs.getnuhv();
        //ZC05/2004          Ghv       = rhs.getGhv();

        //ZC05/2004          eo        = rhs.geteo();
        //ZC05/2004          ec        = rhs.getec();
        //ZC05/2004          Lambda    = rhs.getLam();
        //ZC05/2004          po        = rhs.getpo();
        e         = rhs.gete();
        integratorFlag = rhs.getIntegratorFlag();
        psi       = rhs.getpsi();
        //ZC05/2004          a         = rhs.geta();

    }

    return *this;

}

//ZC05/2004 //================================================================================
//ZC05/2004 int EPState::getElasticflag(void) const {
//ZC05/2004       return Elasticflag;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::getE() const {
//ZC05/2004       return E_Young;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 // Ev: Young's modulus in a vertical direction -- [out-of-plane]
//ZC05/2004 double EPState::getEv() const {
//ZC05/2004       return Ev;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::getEo() const {
//ZC05/2004       return Eo;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::getnu() const {
//ZC05/2004       return nu_Poisson;
//ZC05/2004 }
//ZC05/2004
//ZC05/2004 //================================================================================
//ZC05/2004 // nuhv: Poisson's ratio for strain in the vertical direction due to a horizontal direct stress -- [out-of-plane]
//ZC05/2004 double EPState::getnuhv() const {
//ZC05/2004       return nuhv;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 // Ghv: Modulus for shear deformation in a vertical direction plane-- [out-of-plane]
//ZC05/2004 double EPState::getGhv() const {
//ZC05/2004       return Ghv;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::getrho() const {
//ZC05/2004       return rho_mass_density;
//ZC05/2004 };

//================================================================================
int EPState::getNScalarVar() const
{
    return NScalarVar;
}

//================================================================================
int EPState::getNTensorVar() const
{
    return NTensorVar;
}

//================================================================================
bool EPState::getConverged() const
{
    return Converged;
}

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::geteo() const {
//ZC05/2004       return eo;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::getec() const {
//ZC05/2004       return ec;
//ZC05/2004 }

//================================================================================
double EPState::gete() const
{
    return e;
}

//================================================================================
double EPState::getpsi() const
{
    return psi;
}

int EPState::getIntegratorFlag() const
{
    return integratorFlag;
}

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::getLam() const {
//ZC05/2004       return Lambda;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::getpo() const {
//ZC05/2004       return po;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 double EPState::geta() const {
//ZC05/2004       return a;
//ZC05/2004 }

//================================================================================
stresstensor EPState::getStress() const
{

    return CurrentStress;

}

//================================================================================
stresstensor EPState::getStress_commit() const
{

    return Stress_commit;

}

//================================================================================
stresstensor EPState::getStress_init() const
{

    return Stress_init;

}

//================================================================================
stresstensor EPState::getIterativeStress() const
{

    return IterativeStress;

}

//================================================================================
straintensor EPState::getStrain() const
{

    return CurrentStrain;

}


//================================================================================
straintensor EPState::getStrain_commit() const
{

    return Strain_commit;

}

//GZ Mar2005
straintensor EPState::getElasticStrain_commit() const
{

    return ElasticStrain_commit;

}

//================================================================================
straintensor EPState::getStrain_init() const
{

    return Strain_init;

}

//================================================================================
straintensor EPState::getElasticStrain() const
{

    return ElasticStrain;

}

//================================================================================
straintensor EPState::getPlasticStrain() const
{

    return PlasticStrain;

}
//================================================================================
straintensor EPState::getdElasticStrain() const
{

    return dElasticStrain;

}


//================================================================================
straintensor EPState::getdPlasticStrain() const
{

    return dPlasticStrain;

}


//================================================================================
tensor EPState::getEep() const
{

    return Eep;

}

//================================================================================
tensor EPState::getEep_commit() const
{

    return Eep_commit;

}

//================================================================================
tensor EPState::getEep_init() const
{

    return Eep_init;

}

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setElasticflag( int efd ) {
//ZC05/2004       Elasticflag = efd;
//ZC05/2004 }


//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setEo( double Eod ) {
//ZC05/2004       Eo = Eod;
//ZC05/2004 }


//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setE( double Ey ) {
//ZC05/2004       E_Young = Ey;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setEv( double Evd ) {
//ZC05/2004       Ev = Evd;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setGhv( double Ghvd ) {
//ZC05/2004       Ghv = Ghvd;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setnu( double nud ) {
//ZC05/2004       nu_Poisson = nud;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setnuhv( double nuhvd ) {
//ZC05/2004       nuhv = nuhvd;
//ZC05/2004 }

//================================================================================
void EPState::setStress(const stresstensor& newstress )
{
    CurrentStress = newstress;
}

//================================================================================
void EPState::setIterativeStress(const stresstensor& newstress )
{
    IterativeStress = newstress;
}


//================================================================================
void EPState::setStrain(const straintensor& newstrain )
{

    CurrentStrain = newstrain;

}

//================================================================================
void EPState::setStress_commit(const stresstensor& newstress )
{
    Stress_commit = newstress;
}

//================================================================================
void EPState::setStrain_commit(const straintensor& newstrain )
{

    Strain_commit = newstrain;

}

//================================================================================
void EPState::setStress_init(const stresstensor& newstress )
{
    Stress_init = newstress;
}

//================================================================================
void EPState::setStrain_init(const straintensor& newstrain )
{

    Strain_init = newstrain;

}

//================================================================================
void EPState::setElasticStrain(const straintensor& newEstrain )
{

    ElasticStrain = newEstrain;

}

//================================================================================
void EPState::setPlasticStrain(const straintensor& newPstrain )
{

    PlasticStrain = newPstrain;

}


//================================================================================
void EPState::setdElasticStrain(const straintensor& newdEstrain )
{

    dElasticStrain = newdEstrain;

}

//================================================================================
void EPState::setdPlasticStrain(const straintensor& newdPstrain )
{

    dPlasticStrain = newdPstrain;

}

//================================================================================
void EPState::setEep(const tensor& newEep )
{

    Eep = newEep;

}

//================================================================================
void EPState::setConverged( bool b)
{
    Converged = b;
}

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::seteo( double eod ) {
//ZC05/2004       eo = eod;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setec( double ecd ) {
//ZC05/2004       ec = ecd;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setLam( double Lamd ) {
//ZC05/2004       Lambda = Lamd;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::setpo( double pod ) {
//ZC05/2004       po = pod;
//ZC05/2004 }

//ZC05/2004 //================================================================================
//ZC05/2004 void EPState::seta( double ad ) {
//ZC05/2004       a = ad;
//ZC05/2004 }

//================================================================================
void EPState::sete( double ed )
{
    e = ed;
}

//================================================================================
void EPState::setpsi( double psid )
{
    psi = psid;
}

//================================================================================
// Retrun the nth Scalar Var.... Starting from 1!!
//================================================================================
double EPState::getScalarVar( int WhichOne) const
{

    if ( WhichOne <= getNScalarVar() )
    {
        return ScalarVar[ WhichOne - 1 ];
    }
    else
    {
        cerr << "EPState::getScalarVar Out of ScalarVar's range: " <<  getNScalarVar()  << endln;
        return 0.0;
        exit(1);
    }

    return 0.0;

}


//================================================================================
// Retrun the nth Tensor Var.... Starting from 1!!
//================================================================================
stresstensor EPState::getTensorVar(int WhichOne) const
{

    stresstensor st;

    if ( WhichOne <= getNTensorVar() )
    {
        return TensorVar[ WhichOne - 1 ];
    }
    else
    {
        cerr << "EPState::getTensorVar No. %d: Out of Tensortial Var's range " << WhichOne << " out of " <<  getNTensorVar() << endln;
        exit(1);
    }

    return st;

}

//================================================================================
// Return Scalar pointer
//================================================================================
double* EPState::getScalarVar()
{

    return ScalarVar;

}

//================================================================================
void EPState::setNScalarVar(int rval)
{

    NScalarVar = rval;

}

//================================================================================
void EPState::setNTensorVar(int rval)
{

    NTensorVar = rval;

}


//================================================================================
double* EPState::getScalarVar_commit( )
{

    return ScalarVar_commit;

}

//================================================================================
double EPState::getScalarVar_commit( int i)
{

    return ScalarVar_commit[i - 1];

}

//================================================================================
double* EPState::getScalarVar_init()
{

    return ScalarVar_init;

}

//================================================================================
double EPState::getScalarVar_init(int i)
{

    return ScalarVar_init[i];

}

//================================================================================
// Return Tensor pointer
//================================================================================
stresstensor* EPState::getTensorVar()
{

    return TensorVar;

}

//================================================================================
stresstensor* EPState::getTensorVar_commit()
{

    return TensorVar_commit;

}

//================================================================================
stresstensor EPState::getTensorVar_commit(int i)
{

    return TensorVar_commit[i - 1];

}

//================================================================================
stresstensor EPState::getTensorVar_init(int i)
{

    return TensorVar_init[i - 1];

}

//================================================================================
stresstensor* EPState::getTensorVar_init()
{

    return TensorVar_init;

}

//================================================================================
// set nth Scalar Var.... Starting from 1!!
//================================================================================
void EPState::setScalarVar(int WhichOne, double rval)
{

    if ( WhichOne <= getNScalarVar() )
    {
        ScalarVar[ WhichOne - 1 ] = rval;
    }
    else
    {
        cerr << "EPState::setScalarVar Out of ScalarVar's range " << getNScalarVar() << endln;
        //cerr << " Out of ScalarVar's range!";
        exit(1);
    }
}
void EPState::setScalarVar_commit(int WhichOne, double rval)
{

    if ( WhichOne <= getNScalarVar() )
    {
        ScalarVar_commit[ WhichOne - 1 ] = rval;
    }
    else
    {
        cerr << "EPState::setScalarVar Out of ScalarVar's range " <<  getNScalarVar() << endln;
        //cerr << " Out of ScalarVar's range!";
        exit(1);
    }
}

void EPState::setScalarVar_init(int WhichOne, double rval)
{

    if ( WhichOne <= getNScalarVar() )
    {
        ScalarVar_init[ WhichOne - 1 ] = rval;
    }
    else
    {
        cerr << "EPState::setScalarVar Out of ScalarVar's range " <<  getNScalarVar() << endln;
        //cerr << " Out of ScalarVar's range!";
        exit(1);
    }
}


//================================================================================
// set nth Tensor Var.... Starting from 1!!
//================================================================================
void EPState::setTensorVar(int WhichOne, const stresstensor& rval)
{

    if ( WhichOne <= getNTensorVar() )
    {
        TensorVar[ WhichOne - 1 ] = rval;
    }
    else
    {
        cerr << "EPState::setTensorVar Out of Tensor Var's range " <<  getNTensorVar() << endln;;
        exit(1);
    }

}
//================================================================================
void EPState::setTensorVar_commit(int WhichOne, const stresstensor& rval)
{

    if ( WhichOne <= getNTensorVar() )
    {
        TensorVar_commit[ WhichOne - 1 ] = rval;
    }
    else
    {
        cerr << "EPState::setTensorVar Out of Tensor Var's range " <<  getNTensorVar() << endln;
        exit(1);
    }

}
//================================================================================
void EPState::setTensorVar_init(int WhichOne, const stresstensor& rval)
{

    if ( WhichOne <= getNTensorVar() )
    {
        TensorVar_init[ WhichOne - 1 ] = rval;
    }
    else
    {
        cerr << "EPState::setTensorVar Out of Tensor Var's range " <<  getNTensorVar() << endln;
        exit(1);
    }

}


//================================================================================
// set all Scalar Vars ..No boundary checking!
//================================================================================
void EPState::setScalarVar(double* rval)
{

    if ( !rval )
    {
        cerr << "EPState::setScalarVar No scalar vars supplied\n";
        ::exit(1);
    }

    for (int i = 0; i < getNScalarVar(); i++)
    {
        //cerr << Scalarp[i] << endlnn;
        ScalarVar[i] = rval[i];
    }

}

//================================================================================
// set all Scalar Vars
//================================================================================
void EPState::setTensorVar(const stresstensor* rval)
{

    if ( !rval )
    {
        cerr << "EPState::setTensorVar No tensorial vars supplied\n";
        ::exit(1);
    }

    for (int i = 0; i < getNTensorVar(); i++)
    {
        TensorVar[i] = rval[i];
        TensorVar[i].null_indices();
    }

}

//================================================================================
void EPState::print()
{
    //       cerr << *this;

}

//Guanzhou added to avoid compiler errors
void EPState::Print(ostream& s, int flag)
{
    s << *this;
}


//================================================================================
// Set all state variables to initials

void EPState::setInit()
{

    stresstensor Stress0;
    straintensor Strain0;

    CurrentStress   = Stress_init;
    CurrentStrain   = Strain_init;
    ElasticStrain   = Strain0;
    PlasticStrain   = Strain0;
    dElasticStrain  = Strain0;
    dPlasticStrain  = Strain0;
    Eep = Eep_init;

    Stress_commit   = Stress_init;
    Strain_commit   = Strain_init;
    ElasticStrain_commit = Strain0;
    Eep_commit = Eep_init;

    int i;

    for (i = 0; i < NScalarVar; i++)
    {
        ScalarVar[i] = ScalarVar_init[i];
        ScalarVar_commit[i] = ScalarVar_init[i];
    }

    for (i = 0; i < NTensorVar; i++)
    {
        TensorVar[i] = TensorVar_init[i];
        TensorVar_commit[i] = TensorVar_init[i];
    }

    Converged = false;
}

//================================================================================
int EPState::commitState ()
{


    //      cerr << "\n\n\n\n int EPState::commitState ()\n\n\n\n";


    int err = 0;
    // commit the variables state
    //Guanzhou out 5-6-2004      CurrentStress   = Stress_init;
    //Guanzhou out 5-6-2004      CurrentStrain   = Strain_init;
    //Guanzhou out 5-6-2004      Eep = Eep_init;

    Stress_commit   = CurrentStress;
    Strain_commit   = CurrentStrain;
    Eep_commit = Eep;
    ElasticStrain_commit = ElasticStrain;

    int i;

    for (i = 0; i < NScalarVar; i++)
    {
        //ScalarVar[i] = ScalarVar_init[i];
        ScalarVar_commit[i] = ScalarVar[i];
    }

    for (i = 0; i < NTensorVar; i++)
    {
        //TensorVar[i] = TensorVar_init[i];
        TensorVar_commit[i] = TensorVar[i];
    }

    return err;

}

//================================================================================
int EPState::revertToLastCommit ()
{
    int err = 0;
    // revert the variables state  to last commited
    CurrentStress   = Stress_commit;
    CurrentStrain   = Strain_commit;
    Eep = Eep_commit;
    ElasticStrain = ElasticStrain_commit;

    int i;

    for (i = 0; i < NScalarVar; i++)
    {
        //ScalarVar[i] = ScalarVar_init[i];
        ScalarVar[i] = ScalarVar_commit[i];
    }

    for (i = 0; i < NTensorVar; i++)
    {
        //TensorVar[i] = TensorVar_init[i];
        TensorVar[i] = TensorVar_commit[i];
    }

    return err;

}

//================================================================================
int EPState::revertToStart ()
{

    int err = 0;
    // revert the variables state to the initials
    CurrentStress   = Stress_init;
    CurrentStrain   = Strain_init;
    Eep = Eep_init;

    Stress_commit   = Stress_init;
    Strain_commit   = Strain_init;
    Eep_commit = Eep_init;

    int i;

    for (i = 0; i < NScalarVar; i++)
    {
        ScalarVar[i] = ScalarVar_init[i];
        ScalarVar_commit[i] = ScalarVar_init[i];
    }

    for (i = 0; i < NTensorVar; i++)
    {
        TensorVar[i] = TensorVar_init[i];
        TensorVar_commit[i] = TensorVar_init[i];
    }

    return err;
}





// //================================================================================
// // Overloaded Insertion Operator
// // prints an EPState's contents
// //================================================================================
// ostream & operator<< (ostream& os, const EPState & EPS)
// //ostream & operator<< (ostream& os, const EPState & EPS)
//     {
//       //        os.setf( ios::showpos | ios::scientific);
//       os.precision(4);
//         os.width(10);
// //ZC05/2004         os << endln << "Elastic plastic state parameters: "  << endln;
// //ZC05/2004
// //ZC05/2004   int ef = EPS.getElasticflag();
// //ZC05/2004   os << "\tElastic Flag = " << ef << ";";
// //ZC05/2004   if (ef == 1)
// //ZC05/2004      os << " pressure dependent isotropic material (default case, for soil)." << endln;
// //ZC05/2004   else if (ef == 2)
// //ZC05/2004      os << " pressure independent isotropic material." << endln;
// //ZC05/2004   else if (ef == 3)
// //ZC05/2004      os << " pressure independent cross-anisotropic material." << endln;
// //ZC05/2004   else if (ef == 4)
// //ZC05/2004      os << " pressure dependent cross-anisotropic material." << endln;
// //ZC05/2004   else
// //ZC05/2004      os << " elastic portion code not correct. Flag must be 1, 2, 3 or 4." << endln;
//
//
// //ZC05/2004         os << "\tEo = " << EPS.getEo() << ";";
// //ZC05/2004         os << " E_Young = " << EPS.getE() << ";";
// //ZC05/2004         //os.width(10);
// //ZC05/2004   os << " nu_Poisson = " << EPS.getnu() << ";";
// //ZC05/2004         os << " \tE_v = " << EPS.getEv() << ";";
// //ZC05/2004         os << " nu_hv = " << EPS.getnuhv() << ";";
// //ZC05/2004   os << " G_hv = " << EPS.getGhv() << ";";
// //ZC05/2004   os << " rho = " << EPS.getrho() << endln;
// //ZC05/2004
// //ZC05/2004         os << "\teo = " << EPS.geteo() << ";";
// //ZC05/2004         os << " ec = " << EPS.getec() << ";";
// //ZC05/2004         os << " Lambda = " << EPS.getLam() << ";";
// //ZC05/2004         os << " po = " << EPS.getpo() << ";";
//   os << " e = " << EPS.gete() << endln;
//   os << " psi = " << EPS.getpsi() << endln;
// //ZC05/2004         os << " a = " << EPS.geta() << ";";
//
//   if ( EPS.getConverged() )
//      os << "\tConverged = ok! ";
//   else
//      os << "\tConverged = no! " << endln;
//
//         //os.width(10);
//         os << endln << "\tCurrent Stress:" << EPS.getStress() << endln;
//         os << "\tIterati Stress:" << EPS.getIterativeStress() << endln;
//
//   os << "\tCurrent Strain:" << EPS.getStrain() << endln;
//   os << "\tElasticStrain :" << EPS.getElasticStrain() << endln;
//   os << "\tPlasticStrain :" << EPS.getPlasticStrain() << endln;
//   os << "\tdElasticStrain:" << EPS.getdElasticStrain() << endln;
//   os << "\tdPlasticStrain:" << EPS.getdPlasticStrain() << endln;
//   os << "\tEep.rank():" << EPS.getEep().rank() << endln;
//
//   //        os.unsetf( ios::showpos );
//   int NS = EPS.getNScalarVar();
//   int NT = EPS.getNTensorVar();
//
//   os << endln << "\tNScalarVar = " << NS << endln;
//
//     int i;
//         for (i = 0; i < NS; i++) {
//             os << "\tNo." << i+1 << " " << EPS.ScalarVar[i] << "; ";
//   }
//         os << endln << endln;
//
//         os << "\tNTensorVar = " << NT;
//         for (i = 0; i < NT; i++) {
//     //           os.unsetf( ios::showpos);
//            os << endln << "\tNo." << i+1 << " tensorial var:";
//      //           os.setf( ios::showpos);
//            os << EPS.TensorVar[i];
//         }
//
//         os << endln;
//         return os;
//     }


//Guanzhou added for PDD
int EPState::sendSelf(int commitTag, Channel& theChannel)
{
    //int res = 0;

    int dataTag = this->getDbTag();
    //int dbTag = this->getDbTag();

    static ID idData(5);
    static Vector EPSData(15);
    //double epStData[26];

    idData(0) = this->getTag();
    idData(1) = NScalarVar;
    idData(2) = NTensorVar;

    //idData(3) = Elasticflag;
    if(Converged == true)
    {
        idData(3) = 1;
    }
    else
    {
        idData(3) = 0;
    }

    idData(4) = integratorFlag;

    //res = ;
    if (theChannel.sendID(dataTag, commitTag, idData) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send ID\n";
        return -1;
    }

    EPSData(0) = ScalarVar[0];
    EPSData(1) = ScalarVar[1];
    EPSData(2) = ScalarVar[2];
    EPSData(3) = ScalarVar[3];
    EPSData(4) = ScalarVar_commit[0];
    EPSData(5) = ScalarVar_commit[1];
    EPSData(6) = ScalarVar_commit[2];
    EPSData(7) = ScalarVar_commit[3];
    EPSData(8) = ScalarVar_init[0];
    EPSData(9) = ScalarVar_init[1];
    EPSData(10) = ScalarVar_init[2];
    EPSData(11) = ScalarVar_init[3];
    EPSData(12) = e;
    EPSData(13) = psi;
    EPSData(14) = Delta_lambda;

    //static Vector epStDataVec;
    //epStDataVec.setData(epStData,26);
    if (theChannel.sendVector(dataTag, commitTag, EPSData) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send Vector EPSData\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, CurrentStress) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray CurrentStress\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, CurrentStrain) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray CurrentStrain\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, ElasticStrain) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray ElasticStrain\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, PlasticStrain) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray PlasticStrain\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, dElasticStrain) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray dElasticStrain\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, dPlasticStrain) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray dPlasticStrain\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Stress_commit) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray Stress_commit\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Strain_commit) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray Strain_commit\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, ElasticStrain_commit) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray ElasticStrain_commit\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Stress_init) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray Stress_init\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Strain_init) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray Strain_init\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Eep) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray Eep\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Eep_commit) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray Eep_commit\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Eep_init) < 0)
    {
        cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray Eep_init\n";
        return -1;
    }

    for(int i = 0; i < 4; i++)
    {
        if (theChannel.sendnDarray(dataTag, commitTag, TensorVar[i]) < 0)
        {
            cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray TensorVar\n";
            return -1;
        }

        if (theChannel.sendnDarray(dataTag, commitTag, TensorVar_commit[i]) < 0)
        {
            cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray TensorVar_commit\n";
            return -1;
        }

        if (theChannel.sendnDarray(dataTag, commitTag, TensorVar_init[i]) < 0)
        {
            cerr << "WARNING EPState::sendSelf() - " << this->getTag() << " failed to send nDarray TensorVar_init\n";
            return -1;
        }
    }

    return 0;
}

//Guanzhou added
int EPState::receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    int dataTag = this->getDbTag();
    static ID idData(5);

    if (theChannel.receiveID(dataTag, commitTag, idData) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv ID\n";
        return -1;
    }

    this->setTag(idData(0));
    NScalarVar = idData(1);
    NTensorVar = idData(2);

    //Elasticflag = idData(3);
    if(idData(3) == 1)
    {
        Converged = true;
    }
    else
    {
        Converged = false;
    }

    integratorFlag = idData(4);

    static Vector EPSData(15);

    if (theChannel.receiveVector(dataTag, commitTag, EPSData) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv Vector EPSData\n";
        return -1;
    }
    else
    {
        ScalarVar[0]        = EPSData(0);
        ScalarVar[1]        = EPSData(1);
        ScalarVar[2]        = EPSData(2);
        ScalarVar[3]        = EPSData(3);
        ScalarVar_commit[0] = EPSData(4);
        ScalarVar_commit[1] = EPSData(5);
        ScalarVar_commit[2] = EPSData(6);
        ScalarVar_commit[3] = EPSData(7);
        ScalarVar_init[0]   = EPSData(8);
        ScalarVar_init[1]   = EPSData(9);
        ScalarVar_init[2]   = EPSData(10);
        ScalarVar_init[3]   = EPSData(11);
        e                 = EPSData(12);
        psi               = EPSData(13);
        Delta_lambda = EPSData(14);
    }

    if (theChannel.receivenDarray(dataTag, commitTag, CurrentStress) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray CurrentStress\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, CurrentStrain) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray CurrentStrain\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, ElasticStrain) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray ElasticStrain\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, PlasticStrain) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray PlasticStrain\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, dElasticStrain) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray dElasticStrain\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, dPlasticStrain) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray dPlasticStrain\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Stress_commit) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray Stress_commit\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Strain_commit) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray Strain_commit\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, ElasticStrain_commit) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray ElasticStrain_commit\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Stress_init) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray Stress_init\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Strain_init) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray Strain_init\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Eep) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray Eep\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Eep_commit) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray Eep_commit\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Eep_init) < 0)
    {
        cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray Eep_init\n";
        return -1;
    }

    for(int i = 0; i < 4; i++)
    {
        if (theChannel.receivenDarray(dataTag, commitTag, TensorVar[i]) < 0)
        {
            cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray TensorVar\n";
            return -1;
        }

        if (theChannel.receivenDarray(dataTag, commitTag, TensorVar_commit[i]) < 0)
        {
            cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray TensorVar_commit\n";
            return -1;
        }

        if (theChannel.receivenDarray(dataTag, commitTag, TensorVar_init[i]) < 0)
        {
            cerr << "WARNING EPState::receiveSelf() - " << this->getTag() << " failed to recv nDarray TensorVar_init\n";
            return -1;
        }
    }

    return 0;
}

//Guanzhou added for repartitioning
int
EPState::getObjectSize()
{
    int size = 0;
    size += 5 * sizeof(int);
    size += 15 * sizeof(double);
    size += (CurrentStress.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (CurrentStrain.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (ElasticStrain.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (PlasticStrain.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (dElasticStrain.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (dPlasticStrain.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Stress_commit.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Strain_commit.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (ElasticStrain_commit.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Stress_init.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Strain_init.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Eep.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Eep_init.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Eep_commit.pc_nDarray_rep)->total_numb * sizeof(double);

    for (int i = 0; i < 4 ; i++)
    {
        size += (TensorVar[i].pc_nDarray_rep)->total_numb * sizeof(double);
        size += (TensorVar_commit[i].pc_nDarray_rep)->total_numb * sizeof(double);
        size += (TensorVar_init[i].pc_nDarray_rep)->total_numb * sizeof(double);
    }

    return size;

}

#endif

