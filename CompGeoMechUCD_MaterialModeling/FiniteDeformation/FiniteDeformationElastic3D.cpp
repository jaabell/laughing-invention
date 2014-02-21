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
//# DATE:              19AUg2003
//# UPDATE HISTORY:    Sept 2003
//#                    May28, 2004
//#
//===============================================================================

#include <FiniteDeformationElastic3D.h>
//-------------------------------------------------------------------------------------------------------------------------------------------
FiniteDeformationElastic3D::FiniteDeformationElastic3D(int tag,
        int classTag,
        double rho_in = 0.0)
    : NDMaterial(tag, classTag), rho(rho_in)
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------
FiniteDeformationElastic3D::FiniteDeformationElastic3D( )
    : NDMaterial(0, 0), rho(0.0)
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------
FiniteDeformationElastic3D::~FiniteDeformationElastic3D()
{

}

//---------------------------------------------------------------------------------------------------------------------------------------------
double FiniteDeformationElastic3D::getRho(void)
{
    return rho;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::setTrialF(const straintensor& f)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::setTrialFIncr(const straintensor& df)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::setTrialC(const straintensor& c)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::setTrialCIncr(const straintensor& dc)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const straintensor& FiniteDeformationElastic3D::getF(void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    // Just to make it compile
    straintensor* ret = new straintensor;

    return *ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const straintensor& FiniteDeformationElastic3D::getC(void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    // Just to make it compile
    straintensor* ret = new straintensor;

    return *ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const Tensor& FiniteDeformationElastic3D::getTangentTensor(void)
{
    exit (-1);

    // Just to make it compile
    Tensor* ret = new Tensor;

    return *ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const Tensor
& FiniteDeformationElastic3D::getInitialTangentTensor(void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    // Just to make it compile
    Tensor* ret = new Tensor;

    return *ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const straintensor& FiniteDeformationElastic3D::getStrainTensor(void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    // Just to make it compile
    straintensor* ret = new straintensor;

    return *ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const stresstensor& FiniteDeformationElastic3D::getStressTensor(void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    stresstensor* ret = new stresstensor;

    return *ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const stresstensor& FiniteDeformationElastic3D::getPK1StressTensor(void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    stresstensor* ret = new stresstensor;

    return *ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const stresstensor& FiniteDeformationElastic3D::getCauchyStressTensor(void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    stresstensor* ret = new stresstensor;

    return *ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::commitState (void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return -1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::revertToLastCommit (void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return -1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::revertToStart (void)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return -1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
NDMaterial* FiniteDeformationElastic3D::getCopy (void)

{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
NDMaterial* FiniteDeformationElastic3D::getCopy (const char* type)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const char* FiniteDeformationElastic3D::getType (void) const
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::sendSelf (int commitTag, Channel& theChannel)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------
int FiniteDeformationElastic3D::recvSelf (int commitTag,
        Channel& theChannel,
        FEM_ObjectBroker& theBroker)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
void FiniteDeformationElastic3D::Print (ostream& s, int flag)
{
    cerr << "FiniteDeformationElastic3D-- subclass responsibility\n";
    exit (-1);

    return;
}
