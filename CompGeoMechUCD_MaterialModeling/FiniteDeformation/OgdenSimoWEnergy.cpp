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
//# UPDATE HISTORY:    28May2004
//#
//#
//===============================================================================


#ifndef OgdenSimoWEnergy_CPP
#define OgdenSimoWEnergy_CPP

#include <OgdenSimoWEnergy.h>

//================================================================================
// Normal constructor
//================================================================================
OgdenSimoWEnergy::OgdenSimoWEnergy(int N_in, double*  cr_in, double* mur_in, double K_in)
{
    N_Ogden = N_in;
    cr_Ogden = cr_in;
    mur_Ogden = mur_in;
    K = K_in;
}

OgdenSimoWEnergy::OgdenSimoWEnergy( )
{
    N_Ogden = 0;
    cr_Ogden = 0;
    mur_Ogden = 0;
    K = 0.0;
}

//================================================================================
// Normal destructor
//================================================================================
OgdenSimoWEnergy::~OgdenSimoWEnergy( )
{

}

//================================================================================
//create a clone of itself
//================================================================================
WEnergy* OgdenSimoWEnergy::newObj( )
{
    WEnergy*  new_WEnergy = new OgdenSimoWEnergy(N_Ogden,  cr_Ogden,  mur_Ogden, K);
    return new_WEnergy;
}


//================================================================================
// w
//================================================================================
const double  OgdenSimoWEnergy::wE(const double& J_in, const Vector& lambda_wave_in )
{
    double wEnergy = 0.0;

    for (int i = 0; i < N_Ogden; i++)
    {
        wEnergy += *(cr_Ogden + i) / *(mur_Ogden + i) *
                   (pow ( lambda_wave_in(0),  *(mur_Ogden + i) ) +
                    pow ( lambda_wave_in(1),  *(mur_Ogden + i) ) +
                    pow ( lambda_wave_in(2),  *(mur_Ogden + i) ) - 3.0 ) ;
    }

    return wEnergy;
}

//================================================================================
// d(iso)w / d(lambda)
//================================================================================
const Vector  OgdenSimoWEnergy::disowOdlambda( const Vector& lambda_wave_in)
{
    Vector disowOverdlambda(3);

    for (int i = 0; i < N_Ogden; i++)
    {
        disowOverdlambda(0) += *(cr_Ogden + i) * pow( lambda_wave_in(0),  (*(mur_Ogden + i) - 1.0) );
        disowOverdlambda(1) += *(cr_Ogden + i) * pow( lambda_wave_in(1),  (*(mur_Ogden + i) - 1.0) );
        disowOverdlambda(2) += *(cr_Ogden + i) * pow( lambda_wave_in(2),  (*(mur_Ogden + i) - 1.0) );
    }

    return disowOverdlambda;
}

//================================================================================
// d2(iso)w / d(lambda)2
//================================================================================
const Vector  OgdenSimoWEnergy::d2isowOdlambda2(const Vector& lambda_wave_in )
{
    Vector d2isowOverdlambda2(3);

    for (int i = 0; i < N_Ogden; i++)
    {
        d2isowOverdlambda2(0) +=
            *(cr_Ogden + i) * (*(mur_Ogden + i) - 1.0) * pow( lambda_wave_in(0),  (*(mur_Ogden + i) - 2.0) );
        d2isowOverdlambda2(1) +=
            *(cr_Ogden + i) * (*(mur_Ogden + i) - 1.0) * pow( lambda_wave_in(1),  (*(mur_Ogden + i) - 2.0) );
        d2isowOverdlambda2(2) +=
            *(cr_Ogden + i) * (*(mur_Ogden + i) - 1.0) * pow( lambda_wave_in(2),  (*(mur_Ogden + i) - 2.0) );
    }

    return d2isowOverdlambda2;
}

//================================================================================
// d(vol)w / dJ
//================================================================================
const double  OgdenSimoWEnergy::dvolwOdJ(const double& J_in )
{
    double dcolwOverdJ = K * (-2.0 / J_in + 2.0 * J_in) * 0.25;
    return dcolwOverdJ;
}

//================================================================================
// d2(vol)w / dJ2
//================================================================================
const double  OgdenSimoWEnergy::d2volwOdJ2(const double& J_in )
{
    double d2colwOverdJ2 = K * (2.0 / J_in / J_in + 2.0) * 0.25;
    return d2colwOverdJ2;
}



#endif
