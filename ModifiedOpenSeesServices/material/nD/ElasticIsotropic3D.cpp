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
//# PURPOSE:           Elastic Isotropic Material implementation:
//# CLASS:             ElasticIsotropic3D
//#
//# VERSION:           0.61803398874989 (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Zhaohui Yang, Boris Jeremic (jeremic@ucdavis.edu)
//# PROGRAMMER(S):     Zhaohui Yang, Boris Jeremic
//#
//#
//# DATE:              10Oct2000
//# UPDATE HISTORY:    22Nov2002 small fixes
//#
//#
//===============================================================================

#include <ElasticIsotropic3D.h>

#include <iostream>
using namespace std;

Matrix ElasticIsotropic3D::D(6, 6);   // global for ElasticIsotropic3D only
Vector ElasticIsotropic3D::sigma(6);     // global for ElasticIsotropic3D only


//===============================================================================
ElasticIsotropic3D::ElasticIsotropic3D
(int tag, double E, double nu, double rho):
    ElasticIsotropicMaterial (tag, ND_TAG_ElasticIsotropic3D, E, nu, rho),
    epsilon(6)
{
    // Set up the elastic constant matrix for 3D elastic isotropic
    D.Zero();
    Dt = tensor( 4, def_dim_4, 0.0 );
    setInitElasticStiffness();
}

//===============================================================================
ElasticIsotropic3D::ElasticIsotropic3D():
    ElasticIsotropicMaterial (0, ND_TAG_ElasticIsotropic3D, 0.0, 0.0, 0.0),
    epsilon(6)
{
    Dt = tensor( 4, def_dim_4, 0.0 );
}

ElasticIsotropic3D::~ElasticIsotropic3D ()
{

}

//===============================================================================
int
ElasticIsotropic3D::setTrialStrain (const Vector& v)
{
    epsilon = v;

    return 0;
}

//===============================================================================
int
ElasticIsotropic3D::setTrialStrain (const Vector& v, const Vector& r)
{
    epsilon = v;

    return 0;
}

//===============================================================================
int
ElasticIsotropic3D::setTrialStrainIncr (const Vector& v)
{
    epsilon += v;

    return 0;
}

//===============================================================================
int
ElasticIsotropic3D::setTrialStrainIncr (const Vector& v, const Vector& r)
{
    epsilon += v;

    return 0;
}

//===============================================================================
const Matrix&
ElasticIsotropic3D::getTangent (void)
{
    double mu2 = E / (1.0 + v);
    double lam = v * mu2 / (1.0 - 2.0 * v);
    double mu  = 0.50 * mu2;

    mu2 += lam;

    D(0, 0) = D(1, 1) = D(2, 2) = mu2;
    D(0, 1) = D(1, 0) = lam;
    D(0, 2) = D(2, 0) = lam;
    D(1, 2) = D(2, 1) = lam;
    D(3, 3) = mu;
    D(4, 4) = mu;
    D(5, 5) = mu;

    return D;
}

//===============================================================================
const Matrix&
ElasticIsotropic3D::getInitialTangent (void)
{
    double mu2 = E / (1.0 + v);
    double lam = v * mu2 / (1.0 - 2.0 * v);
    double mu  = 0.50 * mu2;

    mu2 += lam;

    D(0, 0) = D(1, 1) = D(2, 2) = mu2;
    D(0, 1) = D(1, 0) = lam;
    D(0, 2) = D(2, 0) = lam;
    D(1, 2) = D(2, 1) = lam;
    D(3, 3) = mu;
    D(4, 4) = mu;
    D(5, 5) = mu;

    return D;
}

//===============================================================================
const Vector&
ElasticIsotropic3D::getStress (void)
{
    double mu2 = E / (1.0 + v);
    double lam = v * mu2 / (1.0 - 2.0 * v);
    double mu  = 0.50 * mu2;
This dataset is made of data points positioned on arbitrary grid:

    each point is connected to its nearest neighbors on the data array. The position of the data points are fully described by 1 coordinate arrays, specifying x, y and z for each point. This is the dataset created by the mlab.pipeline.scalar_field and mlab.pipeline.vector_field factory functions, if the x, y and z arrays are explicitely specified.


        mu2 += lam;

double eps0 = epsilon(0);
    double eps1 = epsilon(1);
    double eps2 = epsilon(2);

    sigma(0) = mu2 * eps0 + lam * (eps1 + eps2);
    sigma(1) = mu2 * eps1 + lam * (eps2 + eps0);
    sigma(2) = mu2 * eps2 + lam * (eps0 + eps1);

    sigma(3) = mu * epsilon(3);
    sigma(4) = mu * epsilon(4);
    sigma(5) = mu * epsilon(5);

    return sigma;
}

//===============================================================================
const Vector&
ElasticIsotropic3D::getStrain (void)
{
    return epsilon;
}

//===============================================================================
int
ElasticIsotropic3D::setTrialStrain (const Tensor& v)
{
    Strain = v;
    return 0;
}

//===============================================================================
int
ElasticIsotropic3D::setTrialStrain (const Tensor& v, const Tensor& r)
{
    Strain = v;
    return 0;
}

//===============================================================================
int
ElasticIsotropic3D::setTrialStrainIncr (const Tensor& v)
{
    //cerr << " before set Tri St Incr " << Strain;
    //cerr << " Strain Incr " << v << endlnn;
    Strain = Strain + v;
    //cerr << " after setTrialStrainIncr  " << Strain << endlnn;
    return 0;
}

//===============================================================================
int
ElasticIsotropic3D::setTrialStrainIncr (const Tensor& v, const Tensor& r)
{
    Strain = Strain + v;
    return 0;
}

//===============================================================================
const Tensor&
ElasticIsotropic3D::getTangentTensor (void)
{
    return Dt;
}

//===============================================================================
const stresstensor&
ElasticIsotropic3D::getStressTensor (void)
{
    Stress = Dt("ijkl") * Strain("kl");
    return Stress;
}

//===============================================================================
const straintensor&
ElasticIsotropic3D::getStrainTensor (void)
{
    return Strain;
}

//===============================================================================
const straintensor&
ElasticIsotropic3D::getPlasticStrainTensor (void)
{
    //Return zero straintensor
    static straintensor t;//Guanzhou changed!
    return t;
}

//===============================================================================
int
ElasticIsotropic3D::commitState (void)
{
    return 0;
}

//===============================================================================
int
ElasticIsotropic3D::revertToLastCommit (void)
{
    return 0;
}

int
ElasticIsotropic3D::revertToStart (void)
{
    return 0;
}

NDMaterial*
ElasticIsotropic3D::getCopy (void)
{
    ElasticIsotropic3D* theCopy =
        new ElasticIsotropic3D (this->getTag(), E, v, rho);
    //cerr << "In Get copy" <<  *theCopy << endln;
    theCopy->epsilon = this->epsilon;
    //  theCopy->sigma = this->sigma;
    theCopy->Strain = this->Strain;
    theCopy->Stress = this->Stress;
    // D and Dt are created in the constructor call

    return theCopy;
}

const char*
ElasticIsotropic3D::getType (void) const
{
    return "ThreeDimensional";
}

int
ElasticIsotropic3D::getOrder (void) const
{
    return 6;
}

//================================================================================
//================================================================================
//================================================================================
void
ElasticIsotropic3D::Print(ostream& s, int flag)
{
    s << "ElasticIsotropic3D" << endln;
    s << "\ttag: " << this->getTag() << endln;
    s << "\tE: " << E << endln;
    s << "\tv: " << v << endln;
    s << "\trho: " << rho << endln;
    //s << "\tD: " << D << endln;
}


//================================================================================
void ElasticIsotropic3D::setInitElasticStiffness(void)
{
    tensor ret( 4, def_dim_4, 0.0 );

    // Kronecker delta tensor
    tensor I2("I", 2, def_dim_2);

    tensor I_ijkl = I2("ij") * I2("kl");


    //I_ijkl.null_indices();
    tensor I_ikjl = I_ijkl.transpose0110();
    tensor I_iljk = I_ijkl.transpose0111();
    tensor I4s = (I_ikjl + I_iljk) * 0.5;

    // Building elasticity tensor
    ret = I_ijkl * ( E * v / ( (1.0 + v) * (1.0 - 2.0 * v) ) ) + I4s * ( E / (1.0 + v) );

    //ret.print();
    Dt = ret;

    return;
}


//Guanzhou added for PDD
int
ElasticIsotropic3D::sendSelf (int commitTag, Channel& theChannel)
{
    int dataTag = this->getDbTag();
    static Vector data(4);

    data(0) = this->getTag();
    data(1) = E;
    data(2) = v;
    data(3) = rho;

    if (theChannel.sendVector(dataTag, commitTag, data) < 0)
    {
        cerr << "ElasticIsotropicMaterial::sendSelf -- could not send Vector\n";
        return -1;
    }

    if (theChannel.sendVector(dataTag, commitTag, sigma) < 0)
    {
        cerr << "ElasticIsotropicMaterial::sendSelf -- could not send Stress Vector\n";
        return -1;
    }

    if (theChannel.sendMatrix(dataTag, commitTag, D) < 0)
    {
        cerr << "ElasticIsotropicMaterial::sendSelf -- could not send Elastic Constant Vector\n";
        return -1;
    }


    if (theChannel.sendVector(dataTag, commitTag, epsilon) < 0)
    {
        cerr << "ElasticIsotropicMaterial::sendSelf -- could not send strain Vector\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Stress) < 0)
    {
        cerr << "ElasticIsotropicMaterial::sendSelf -- could not send Stress Tensor\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Dt) < 0)
    {
        cerr << "ElasticIsotropicMaterial::sendSelf -- could not send Elastic Constant Tensor\n";
        return -1;
    }

    if (theChannel.sendnDarray(dataTag, commitTag, Strain) < 0)
    {
        cerr << "ElasticIsotropicMaterial::sendSelf -- could not send Strain Tensor\n";
        return -1;
    }

    return 0;
}

int
ElasticIsotropic3D::receiveSelf (int commitTag, Channel& theChannel,
                              FEM_ObjectBroker& theBroker)
{
    int dataTag = this->getDbTag();
    static Vector data(4);

    if (theChannel.receiveVector(dataTag, commitTag, data) < 0)
    {
        cerr << "ElasticIsotropicMaterial::receiveSelf -- could not recv Vector\n";
        E = 0;
        this->setTag(0);
        return -1;
    }

    this->setTag((int)data(0));
    E = data(1);
    v = data(2);
    rho = data(3);

    if (theChannel.receiveVector(dataTag, commitTag, sigma) < 0)
    {
        cerr << "ElasticIsotropicMaterial::receiveSelf -- could not recv Stress Vector\n";
        return -1;
    }

    if (theChannel.receiveMatrix(dataTag, commitTag, D) < 0)
    {
        cerr << "ElasticIsotropicMaterial::receiveSelf -- could not recv Elastic Constant Vector\n";
        return -1;
    }


    if (theChannel.receiveVector(dataTag, commitTag, epsilon) < 0)
    {
        cerr << "ElasticIsotropicMaterial::receiveSelf -- could not recv strain Vector\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Stress) < 0)
    {
        cerr << "ElasticIsotropicMaterial::receiveSelf -- could not recv Stress Tensor\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Dt) < 0)
    {
        cerr << "ElasticIsotropicMaterial::receiveSelf -- could not recv Elastic Constant Tensor\n";
        return -1;
    }

    if (theChannel.receivenDarray(dataTag, commitTag, Strain) < 0)
    {
        cerr << "ElasticIsotropicMaterial::receiveSelf -- could not recv Strain Tensor\n";
        return -1;
    }

    return 0;

}

int  //Guanzhou added
ElasticIsotropic3D::getObjectSize()
{
    int size = 0;
    size += 4 * sizeof(double);
    size += sigma.Size() * sizeof(double);
    size += D.dataSize * sizeof(double);
    size += epsilon.Size() * sizeof(double);
    size += (Stress.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Dt.pc_nDarray_rep)->total_numb * sizeof(double);
    size += (Strain.pc_nDarray_rep)->total_numb * sizeof(double);
    return size;

}
