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
//# PURPOSE:           Finite Deformation Hyper-Elastic classes
//# CLASS:
//#
//# VERSION:           0.6_(1803398874989) (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Zhao Cheng, Boris Jeremic (jeremic@ucdavis.edu)
//# PROGRAMMER(S):     Zhao Cheng, Boris Jeremic
//#
//#
//# DATE:              July 2004
//# UPDATE HISTORY:
//#
//===============================================================================

// the traditional neo-Hookean hyperelasticity:
// w = 0.5*lambda*(lnJ)^2 - G*(lnJ) + 0.5*G*(trace(C)-3)

#include <NeoHookeanCompressible3D.h>

// Boris Jeremic 17Nov2008
#include <iostream>
using namespace std;


stresstensor NeoHookeanCompressible3D::static_NHC_stress;

//---------------------------------------------------------------------------------------------------------------
NeoHookeanCompressible3D::NeoHookeanCompressible3D(int tag,
        int classTag,
        double K_in,
        double G_in,
        double rho_in = 0.0)
    : FiniteDeformationElastic3D(tag, classTag, rho_in), K(K_in), G(G_in)
{

}

//---------------------------------------------------------------------------------------------------------------
NeoHookeanCompressible3D::NeoHookeanCompressible3D(int tag,
        double K_in,
        double G_in,
        double rho_in = 0.0)
    : FiniteDeformationElastic3D(tag, ND_TAG_NeoHookeanCompressible3D, rho_in), K(K_in), G(G_in)
{

}

//---------------------------------------------------------------------------------------------------------------
NeoHookeanCompressible3D::NeoHookeanCompressible3D( )
    : FiniteDeformationElastic3D(0, 0, 0.0), K(0.0), G(0.0)
{

}

//----------------------------------------------------------------------------------------------------------------
NeoHookeanCompressible3D::~NeoHookeanCompressible3D()
{

}

//----------------------------------------------------------------------------------------------------------------
double NeoHookeanCompressible3D::getRho(void)
{
    return rho;
}

//----------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::setTrialF(const straintensor& f)
{
    FromForC = 0;
    F.Initialize(f);
    C = F("ki") * F("kj");
    C.null_indices();

    return this->ComputeTrials();
}

//----------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::setTrialFIncr(const straintensor& df)
{
    return this->setTrialF(this->getF() + df);
}

//----------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::setTrialC(const straintensor& c)
{
    FromForC = 1;
    C.Initialize(c);

    return this->ComputeTrials();
}

//----------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::setTrialCIncr(const straintensor& dc)
{
    return this->setTrialC(this->getC() + dc);
}

//----------------------------------------------------------------------------------------------------------------
const straintensor& NeoHookeanCompressible3D::getF(void)
{
    return F;
}

//----------------------------------------------------------------------------------------------------------------
const straintensor& NeoHookeanCompressible3D::getC(void)
{
    return C;
}

//----------------------------------------------------------------------------------------------------------------
const Tensor& NeoHookeanCompressible3D::getTangentTensor(void)
{
    return Stiffness;
}

//-----------------------------------------------------------------------------------------------------------------
const Tensor
& NeoHookeanCompressible3D::getInitialTangentTensor(void)
{
    return this->getTangentTensor();
}

//-----------------------------------------------------------------------------------------------------------------
const straintensor& NeoHookeanCompressible3D::getStrainTensor(void)
{
    return thisGreenStrain;
}

//----------------------------------------------------------------------------------------------------------------
const stresstensor& NeoHookeanCompressible3D::getStressTensor(void)
{
    return thisPK2Stress;
}

//----------------------------------------------------------------------------------------------------------------
const stresstensor& NeoHookeanCompressible3D::getPK1StressTensor(void)
{
    if ( FromForC == 0 )
    {
        stresstensor SPKStress = this->getStressTensor();
        NeoHookeanCompressible3D::static_NHC_stress = F("kJ") * SPKStress("IJ");
    }

    if ( FromForC == 1 )
    {
std:
        cerr << "NeoHookeanCompressible3D: unknown deformation gradient - cannot compute PK1 stress" << "\n";
        exit (-1);
    }

    return NeoHookeanCompressible3D::static_NHC_stress;
}

//-----------------------------------------------------------------------------------------------------------------
const stresstensor& NeoHookeanCompressible3D::getCauchyStressTensor(void)
{
    if ( FromForC == 0 )
    {
        stresstensor SPKStress = this->getStressTensor();
        stresstensor tensor1 = F("iJ") * SPKStress("JK");
        stresstensor tensor2 = tensor1("iK") * F("mK");
        double J = fabs(F.determinant());
        NeoHookeanCompressible3D::static_NHC_stress = tensor2 * (1.0 / J);
    }

    if ( FromForC == 1 )
    {
std:
        cerr << "NeoHookeanCompressible3D: unknown deformation gradient - cannot compute Cauchy stress" << "\n";
        exit (-1);
    }

    return NeoHookeanCompressible3D::static_NHC_stress;
}

//------------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::commitState (void)
{
    return 0;
}

//------------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::revertToLastCommit (void)
{
    return 0;
}

//------------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::revertToStart (void)
{
    tensor F0("I", 2, def_dim_2);
    F.Initialize(F0);
    C.Initialize(F0);

    tensor ss_zero(2, def_dim_2, 0.0);
    thisPK2Stress.Initialize(ss_zero);
    thisGreenStrain.Initialize(ss_zero);

    Stiffness = getInitialTangentTensor();

    return 0;
}

//------------------------------------------------------------------------------------------------------------------
NDMaterial* NeoHookeanCompressible3D::getCopy (void)
{
    NDMaterial* theCopy =
        new NeoHookeanCompressible3D (this->getTag(), this->K, this->G, this->rho);

    return theCopy;
}

//-------------------------------------------------------------------------------------------------------------------
NDMaterial* NeoHookeanCompressible3D::getCopy (const char* type)
{
std:
    cerr << "NeoHookeanCompressible3D::getCopy(const char *) - not yet implemented\n";
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
const char* NeoHookeanCompressible3D::getType (void) const
{
    return "ThreeDimentionalFD";
}

//-------------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::sendSelf (int commitTag, Channel& theChannel)
{
    int res = 0;
    // not yet implemented
    return res;
}

//-------------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::receiveSelf (int commitTag,
                                        Channel& theChannel,
                                        FEM_ObjectBroker& theBroker)
{
    int res = 0;
    // not yet implemented
    return res;
}

//-------------------------------------------------------------------------------------------------------------------
//int NeoHookeanCompressible3D::setParameter(char **argv, int argc, Information &info)
//{
//   return -1;
//}


//------------------------------------------------------------------------------------------------------------------
//int NeoHookeanCompressible3D::updateParameter(int parameterID, Information &info)
//{
//   return -1;
//}


//------------------------------------------------------------------------------------------------------------------
int NeoHookeanCompressible3D::ComputeTrials()
{
    tensor tensorI2("I", 2, def_dim_2);
    tensor tsr1;
    tensor tsr2;

    // Cinv:
    straintensor Cinv = C.inverse();
    Cinv.symmetrize11();

    // J:
    double J = sqrt(C.determinant());

    // lame constants:
    double lambda = K - 2.0 * G / 3.0;
    double mu = G - lambda * log(J);

    // Pk2Stress:
    thisPK2Stress = (tensorI2 - Cinv) * G + Cinv * (lambda * log(J));

    // Green Strain:
    thisGreenStrain = (C - tensorI2) * 0.5;

    // Lagrangian Tangent Stiffness:
    tsr1 = Cinv("ij") * Cinv("kl");
    tsr1.null_indices();
    tsr2 = tsr1.transpose0110() + tsr1.transpose0111();

    Stiffness = tsr1 * lambda + tsr2 * mu;

    return 0;
}
