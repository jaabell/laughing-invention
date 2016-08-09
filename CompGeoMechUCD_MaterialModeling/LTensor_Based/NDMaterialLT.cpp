///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell & Boris Jeremic
// DATE:               August 2013
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


#include <nDarray.h>
#include "NDMaterialLT.h"
#include <Information.h>
#include <OPS_Globals.h>
//#include <DTensor2.h>
//#include <Vector.h>
//#include <stresst.h>
//#include <straint.h>
// #include <MaterialResponse.h>
#include <iostream>
#include "../../ltensor/LTensor.h"
using namespace std;


DTensor2 NDMaterialLT::errMatrix      (3, 3, 0.0);
DTensor1 NDMaterialLT::errVector      (3,    0.0);
DTensor2 NDMaterialLT::errTensor      (3, 3, 0.0);
DTensor4 NDMaterialLT::errTensor4     (3, 3, 3, 3, 0.0);
DTensor2 NDMaterialLT::errTensor2     (3, 3, 0.0);
DTensor2 NDMaterialLT::errstresstensor(3, 3, 0.0);
DTensor2 NDMaterialLT::errstraintensor(3, 3, 0.0);
Vector  NDMaterialLT::errVectorVector(0.0);
NDMaterialLT_Constitutive_Integration_Method NDMaterialLT::constitutive_integration_method(NDMaterialLT_Constitutive_Integration_Method::Not_Set);     //
double NDMaterialLT::f_relative_tol(-1);
double NDMaterialLT::stress_relative_tol(-1);
int NDMaterialLT::n_max_iterations(0);

NDMaterialLT::NDMaterialLT(int tag, int classTag)
    : Material(tag, classTag)
{

}

NDMaterialLT::NDMaterialLT()
    : Material(0, 0)
{

}

NDMaterialLT::~NDMaterialLT()
{

}


double
NDMaterialLT::getRho(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterialLT::getE(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterialLT::getnu(void)
{
    return 0.0;
}

// BJ added 19June2002
double NDMaterialLT::getpsi(void)
{
    return 0.0;
}

const DTensor1 &
NDMaterialLT::getCommittedStress(void)
{
    return this->getStress();
}

const DTensor1 &
NDMaterialLT::getCommittedStrain(void)
{
    return this->getStrain();
}






// methods to set and retrieve state.
int
NDMaterialLT::setTrialStrain(const DTensor1 &v)
{
    cerr << "NDMaterialLT::setTrialStrain(const DTensor1 &v) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrain(const DTensor1 &v, const DTensor1 &r)
{
    cerr << "NDMaterialLT::setTrialStrain(const DTensor1 &v, const DTensor1 &r) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrain(const DTensor2  &v)
{
    cerr << "NDMaterialLT::setTrialStrain(const DTensor2  &v) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrain(const DTensor2  &v, const DTensor2  &r)
{
    cerr << "NDMaterialLT::setTrialStrain(const DTensor2  &v, const DTensor2  &r) -- subclass responsibility\n";
    return -1;
}





int
NDMaterialLT::setTrialStrainIncr(const DTensor1 &v)
{
    cerr << "NDMaterialLT::setTrialStrainIncr(const DTensor1 &v) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrainIncr(const DTensor1 &v, const DTensor1 &r)
{
    cerr << "NDMaterialLT::setTrialStrainIncr(const DTensor1 &v, const DTensor1 &r) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrainIncr(const DTensor2  &v)
{
    cerr << "NDMaterialLT::setTrialStrainIncr(const DTensor2  &v) -- subclass responsibility\n";
    return -1;
}

int
NDMaterialLT::setTrialStrainIncr(const DTensor2  &v, const DTensor2  &r)
{
    cerr << "NDMaterialLT::setTrialStrainIncr(const DTensor2  &v, const DTensor2  &r) -- subclass responsibility\n";
    return -1;
}













const DTensor2 &
NDMaterialLT::getTangent(void)
{
    cerr << "NDMaterialLT::getTangent -- subclass responsibility\n";
    return errTensor2;
}

const DTensor1 &
NDMaterialLT::getStress(void)
{
    cerr << "NDMaterialLT::getStress -- subclass responsibility\n";
    return errVector;
}

const DTensor1 &
NDMaterialLT::getStrain(void)
{
    cerr << "NDMaterialLT::getStrain -- subclass responsibility\n";
    return errVector;
}


// const DTensor4 &
// NDMaterialLT::getTangentTensor(void)
// {
//     cerr << "NDMaterialLT::getTangentTensor  -- subclass responsibility\n";
//     return errTensor4;
// }

const DTensor2    &NDMaterialLT::getStressTensor(void)
{
    cerr << "NDMaterialLT::getStressTensor    -- subclass responsibility\n";
    return errTensor;
}

const DTensor2    &NDMaterialLT::getStrainTensor(void)
{
    cerr << "NDMaterialLT::getStrainTensor    -- subclass responsibility\n";
    return errstraintensor;
}

const DTensor2    &NDMaterialLT::getPlasticStrainTensor(void)
{
    cerr << "NDMaterialLT::getPlasticStrainTensor    -- subclass responsibility\n";
    return errstraintensor;
}


// Response *
// NDMaterialLT::setResponse (const char **argv, int argc, Information &matInfo)
// {
//     if (strcmp(argv[0], "stress") == 0 || strcmp(argv[0], "stresses") == 0)
//     {
//         return 0;
//     }
//     //  return new MaterialResponse(this, 1, DTensor1_to_nDarray(this->getStress()));

//     else if (strcmp(argv[0], "strain") == 0 || strcmp(argv[0], "strains") == 0)
//     {
//         return 0;
//     }
//     //return new MaterialResponse(this, 2, this->getStrain());

//     else if (strcmp(argv[0], "tangent") == 0)
//     {
//         return 0;
//     }
//     //return new MaterialResponse(this, 3, this->getTangent());

//     else
//     {
//         return 0;
//     }
// }

// // TODO: Need to interface LTensor with old tensors so that information can be passed.
// //   Optionally we can rewrite or overload functions everywhere to also work with LTensor types.
// int
// NDMaterialLT::getResponse (int responseID, Information &matInfo)
// {
//     switch (responseID)
//     {
//         case 1:
//             break;//return matInfo.setVector(this->getStress());

//         case 2:
//             break;//return matInfo.setVector(this->getStrain());

//         case 3:
//             break;//return matInfo.setDTensor2(this->getTangent());

//         default:
//             return -1;
//     }

//     return -1;
// }


// Nima Tafazzoli added for checking the mesh (September 2012)
int
NDMaterialLT::CheckMesh(ofstream &checkmesh_file)
{
    return 0;
}

// J.Abell added June 20, 2013 -> needed for Concrete3D model -> to partially fix mesh sensitivity during softening of concrete
void NDMaterialLT::setVolume(double vol)
{
    volume = vol;
}


int NDMaterialLT::sendSelf(int commitTag, Channel &theChannel)
{
    cerr << "NDMaterialLT::sendSelf -> Subclass responsability" << endl;
    return -1;
}
int NDMaterialLT::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    cerr << "NDMaterialLT::receiveSelf -> Subclass responsability" << endl;
    return -1;
}

void NDMaterialLT::Print(ostream &s, int flag)
{
    cerr << "NDMaterialLT::Print -> Subclass responsability" << endl;
}


int NDMaterialLT::commitState(void)
{
    cerr << "NDMaterialLT::commitState -> Subclass responsability" << endl;
    return -1;
}

int NDMaterialLT::revertToLastCommit(void)
{
    cerr << "NDMaterialLT::revertToLastCommit -> Subclass responsability" << endl;
    return -1;
}

int NDMaterialLT::revertToStart(void)
{
    cerr << "NDMaterialLT::revertToStart -> Subclass responsability" << endl;
    return -1;
}

NDMaterialLT *NDMaterialLT::getCopy(void)
{
    cerr << "NDMaterialLT::getCopy -> Subclass responsability" << endl;
    return 0;
}


const char *NDMaterialLT::getType(void) const
{
    cerr << "NDMaterialLT::getType -> Subclass responsability" << endl;
    return 0;
}

void NDMaterialLT::zeroStrain(void)
{
    cerr << "NDMaterialLT::zeroStrain -> Subclass responsability" << endl;
    return ;
}

bool NDMaterialLT::set_constitutive_integration_method(int method, double f_relative_tol, double stress_relative_tol, int n_max_iterations)
{
    if ( method == (int) NDMaterialLT_Constitutive_Integration_Method::Not_Set
            || method == (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler
            || method == (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler_Crisfield
            || method == (int) NDMaterialLT_Constitutive_Integration_Method::Multistep_Forward_Euler
            || method == (int) NDMaterialLT_Constitutive_Integration_Method::Multistep_Forward_Euler_Crisfield
            || method == (int) NDMaterialLT_Constitutive_Integration_Method::Modified_Euler_Error_Control
            || method == (int) NDMaterialLT_Constitutive_Integration_Method::Runge_Kutta_45_Error_Control
            || method == (int) NDMaterialLT_Constitutive_Integration_Method::Backward_Euler
            || method == (int) NDMaterialLT_Constitutive_Integration_Method::Full_Backward_Euler)
    {
        NDMaterialLT::constitutive_integration_method = (NDMaterialLT_Constitutive_Integration_Method) method ;
        NDMaterialLT::f_relative_tol = f_relative_tol ;
        NDMaterialLT::stress_relative_tol = stress_relative_tol ;
        NDMaterialLT::n_max_iterations = n_max_iterations ;

        cout << "Setting set_constitutive_integration_method = " << method << endl;

        return true;
    }
    else
    {
        cerr << "NDMaterialLT::set_constitutive_integration_method - Unknown constitutive_integration_method\n";
        return false;
    }
}