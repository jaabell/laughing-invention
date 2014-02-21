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
//# PURPOSE:
//# CLASS:             TwoStageEquivalentElasticIsotropic3D
//#
//# VERSION:           0.61803398874989 (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            J. Abell
//# PROGRAMMER(S):     J. Abell
//#
//#
//# DATE:              July 2013
//# UPDATE HISTORY:
//#
//#
//===============================================================================

#include <TwoStageEquivalentElasticIsotropic3D.h>
#include <math.h>

Matrix TwoStageEquivalentElasticIsotropic3D::D(6, 6);     // global for TwoStageEquivalentElasticIsotropic3D only
Vector TwoStageEquivalentElasticIsotropic3D::sigma(6);   // global for TwoStageEquivalentElasticIsotropic3D only


//===============================================================================
TwoStageEquivalentElasticIsotropic3D::TwoStageEquivalentElasticIsotropic3D
(int tag, double nu, double rho,
 double K_, double Kur_, double n_, double c_,
 double phi0_, double dphi_, double Rf_, double K0_,
 double Kb_, double m_, double pa_):
    ElasticIsotropicMaterial(tag, ND_TAG_TwoStageEquivalentElasticIsotropic3D, E, nu, rho),
    epsilon(6),
    K(K_), Kur(Kur_), n(n_), c(c_),
    phi0(phi0_), dphi(dphi_), Rf(Rf_), K0(K0_),
    Kb(Kb_), m(m_), pa(pa_)
{

    // Set up the elastic constant matrix for 3D elastic isotropic
    D.Zero();
    Dt = tensor( 4, def_dim_4, 0.0 );

    stage = -1;
    updateModulus();
    stage = 0;

    setInitElasticStiffness();
}

//===============================================================================
TwoStageEquivalentElasticIsotropic3D::TwoStageEquivalentElasticIsotropic3D():
    ElasticIsotropicMaterial (0, ND_TAG_TwoStageEquivalentElasticIsotropic3D, 0.0, 0.0, 0.0),
    epsilon(6)
{
    Dt = tensor( 4, def_dim_4, 0.0 );
}

TwoStageEquivalentElasticIsotropic3D::~TwoStageEquivalentElasticIsotropic3D ()
{

}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::setTrialStrain (const Vector& v)
{
    epsilon = v;

    return 0;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::setTrialStrain (const Vector& v, const Vector& r)
{
    epsilon = v;

    return 0;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::setTrialStrainIncr (const Vector& v)
{
    epsilon += v;

    return 0;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::setTrialStrainIncr (const Vector& v, const Vector& r)
{
    epsilon += v;

    return 0;
}

//===============================================================================
const Matrix&
TwoStageEquivalentElasticIsotropic3D::getTangent (void)
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

    //cerr << "TwoStageEquivalentElasticIsotropic3D::getTangent (void)\n" << D << "\n";

    return D;
}

//===============================================================================
const Matrix&
TwoStageEquivalentElasticIsotropic3D::getInitialTangent (void)
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

    //cerr << "TwoStageEquivalentElasticIsotropic3D::getInitialTangent (void)\n" << D << "\n";

    return D;
}

//===============================================================================
const Vector&
TwoStageEquivalentElasticIsotropic3D::getStress (void)
{
    double mu2 = E / (1.0 + v);
    double lam = v * mu2 / (1.0 - 2.0 * v);
    double mu  = 0.50 * mu2;

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

    //cerr  << "TwoStageEquivalentElasticIsotropic3D::getStress()\n" << sigma << "\n";


    return sigma;
}

//===============================================================================
const Vector&
TwoStageEquivalentElasticIsotropic3D::getStrain (void)
{
    return epsilon;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::setTrialStrain (const Tensor& v)
{
    Strain = v;
    updateModulus();
    return 0;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::setTrialStrain (const Tensor& v, const Tensor& r)
{
    Strain = v;
    updateModulus();
    return 0;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::setTrialStrainIncr (const Tensor& v)
{
    //cerr << " before set Tri St Incr " << Strain;
    //cerr << " Strain Incr " << v << endlnn;
    Strain = Strain + v;
    updateModulus();
    //cerr << " after setTrialStrainIncr  " << Strain << endlnn;
    return 0;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::setTrialStrainIncr (const Tensor& v, const Tensor& r)
{
    Strain = Strain + v;
    updateModulus();
    return 0;
}

//===============================================================================
const Tensor&
TwoStageEquivalentElasticIsotropic3D::getTangentTensor (void)
{
    //updateModulus();
    //cout << "TwoStageEquivalentElasticIsotropic3D::getTangentTensor (void)\n" << "\n";
    //Dt.print();
    return Dt;
}

//===============================================================================
const stresstensor&
TwoStageEquivalentElasticIsotropic3D::getStressTensor (void)
{
    //updateModulus();
    Stress = Dt("ijkl") * Strain("kl");
    return Stress;
}

//===============================================================================
const straintensor&
TwoStageEquivalentElasticIsotropic3D::getStrainTensor (void)
{
    return Strain;
}

//===============================================================================
const straintensor&
TwoStageEquivalentElasticIsotropic3D::getPlasticStrainTensor (void)
{
    //Return zero straintensor
    static straintensor t;//Guanzhou changed!
    return t;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::commitState (void)
{
    //cout << "======================================= Commit state!" << endl;
    sig3p = sig3;
    sig3p = sig1;
    return 0;
}

//===============================================================================
int
TwoStageEquivalentElasticIsotropic3D::revertToLastCommit (void)
{
    return 0;
}

int
TwoStageEquivalentElasticIsotropic3D::revertToStart (void)
{
    return 0;
}

NDMaterial*
TwoStageEquivalentElasticIsotropic3D::getCopy (void)
{
    TwoStageEquivalentElasticIsotropic3D* theCopy =
        new TwoStageEquivalentElasticIsotropic3D (this->getTag(),  v,  rho,
                K,  Kur,  n,  c,
                phi0,  dphi,  Rf,  K0,
                Kb,  m,  pa);
    //cerr << "In Get copy" <<  *theCopy << endln;
    theCopy->epsilon = this->epsilon;
    //  theCopy->sigma = this->sigma;
    theCopy->Strain = this->Strain;
    theCopy->Stress = this->Stress;
    // D and Dt are created in the constructor call

    return theCopy;
}

const char*
TwoStageEquivalentElasticIsotropic3D::getType (void) const
{
    return "ThreeDimensional";
}

int
TwoStageEquivalentElasticIsotropic3D::getOrder (void) const
{
    return 6;
}

//================================================================================
//================================================================================
//================================================================================
void
TwoStageEquivalentElasticIsotropic3D::Print(ostream& s, int flag)
{
    s << "TwoStageEquivalentElasticIsotropic3D" << endln;
    s << "    tag: " << this->getTag() << endln;
    s << "    stage   = " << stage << endln;
    s << "    nu      = " << v << endln;
    s << "    rho     = " << rho << endln;
    s << "    K       = " << K << endln;
    s << "    Kur     = " << Kur << endln;
    s << "    n       = " << n << endln;
    s << "    c       = " << c << endln;
    s << "    phi0    = " << phi0 << endln;
    s << "    dphi    = " << dphi << endln;
    s << "    Rf      = " << Rf << endln;
    s << "    K0      = " << K0 << endln;
    s << "    Kb      = " << Kb << endln;
    s << "    m       = " << m << endln;
    s << "    pa      = " << pa << endln;
    // ======================================
    //s << "\tD: " << D << endln;
}


//================================================================================
void TwoStageEquivalentElasticIsotropic3D::setInitElasticStiffness(void)
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

    //cout << "InitElasticStiffness = " << endl;
    //ret.print();
    Dt = ret;

    return;
}


//Guanzhou added for PDD
int
TwoStageEquivalentElasticIsotropic3D::sendSelf (int commitTag, Channel& theChannel)
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
TwoStageEquivalentElasticIsotropic3D::recvSelf (int commitTag, Channel& theChannel,
        FEM_ObjectBroker& theBroker)
{
    int dataTag = this->getDbTag();
    static Vector data(4);

    if (theChannel.recvVector(dataTag, commitTag, data) < 0)
    {
        cerr << "ElasticIsotropicMaterial::recvSelf -- could not recv Vector\n";
        E = 0;
        this->setTag(0);
        return -1;
    }

    this->setTag((int)data(0));
    E = data(1);
    v = data(2);
    rho = data(3);

    if (theChannel.recvVector(dataTag, commitTag, sigma) < 0)
    {
        cerr << "ElasticIsotropicMaterial::recvSelf -- could not recv Stress Vector\n";
        return -1;
    }

    if (theChannel.recvMatrix(dataTag, commitTag, D) < 0)
    {
        cerr << "ElasticIsotropicMaterial::recvSelf -- could not recv Elastic Constant Vector\n";
        return -1;
    }


    if (theChannel.recvVector(dataTag, commitTag, epsilon) < 0)
    {
        cerr << "ElasticIsotropicMaterial::recvSelf -- could not recv strain Vector\n";
        return -1;
    }

    if (theChannel.recvnDarray(dataTag, commitTag, Stress) < 0)
    {
        cerr << "ElasticIsotropicMaterial::recvSelf -- could not recv Stress Tensor\n";
        return -1;
    }

    if (theChannel.recvnDarray(dataTag, commitTag, Dt) < 0)
    {
        cerr << "ElasticIsotropicMaterial::recvSelf -- could not recv Elastic Constant Tensor\n";
        return -1;
    }

    if (theChannel.recvnDarray(dataTag, commitTag, Strain) < 0)
    {
        cerr << "ElasticIsotropicMaterial::recvSelf -- could not recv Strain Tensor\n";
        return -1;
    }

    return 0;

}

int  //Guanzhou added
TwoStageEquivalentElasticIsotropic3D::getObjectSize()
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



inline void TwoStageEquivalentElasticIsotropic3D::nextStage()
{
    stage++;
}



inline void TwoStageEquivalentElasticIsotropic3D::updateModulus()
{
    #ifdef _TSEL_Debug_Mode
    cout << "=====================================" << endl;
    cout << "Update Modulus call" << endl;
    #endif

    /*cout << "    stage   = " << stage << endl;
    cout << "    nu      = " << v << endl;
    cout << "    rho     = " << rho << endl;
    cout << "    K       = " << K << endl;
    cout << "    Kur     = " << Kur << endl;
    cout << "    n       = " << n << endl;
    cout << "    c       = " << c << endl;
    cout << "    phi0    = " << phi0 << endl;
    cout << "    dphi    = " << dphi << endl;
    cout << "    Rf      = " << Rf << endl;
    cout << "    K0      = " << K0 << endl;
    cout << "    Kb      = " << Kb << endl;
    cout << "    m       = " << m << endl;
    cout << "    pa      = " << pa << endl;*/
    // ============================================================================================
    // Static analysis
    // ============================================================================================
    if ( stage <= 0)
    {
        #ifdef _TSEL_Debug_Mode
        cout << "--->  Static analysis" << endl;
        #endif
        double s1, s3;  // Average stress between previous step and current... (results in a secant stiffness :P)
        double desv, devfh, strlev, phi, Bmin, G_mod, Ei_el;

        if (stage == 0)
        {
            stresstensor st = getStressTensor();
            nDarray ss = st.eigenvalues();
            #ifdef _TSEL_Debug_Mode
            cout << "Stress tensor:" << endln;
            st.print();
            cout << endln;
            cout << "Eigenvalues  :" << endln;
            ss.print();
            cout << endln;
            #endif
            sig1 = -ss.cval(3);
            sig3 = -ss.cval(1);
            #ifdef _TSEL_Debug_Mode

            for (int i = 0; i <= 3; i++)
            {
                cout << "ss.cval(" << i << ") = " << ss.cval(i) << endl;
            }

            #endif
        }
        else // For very first step (initial tangent) set confinement to be atmospheric pressures
        {
            #ifdef _TSEL_Debug_Mode
            cout << "---> Initial step" << endl;
            #endif
            sig1 = pa;
            sig3 = pa;
            sig1p = pa;
            sig3p = pa;
        }

        s1 = sig1;//0.5*(sig1 + sig1p);
        s3 = sig3;//0.5*(sig3 + sig3p);

        #ifdef _TSEL_Debug_Mode
        cout << "s1 = " << s1 << endl;
        cout << "s3 = " << s3 << endl;
        #endif
        /// TCL

        desv = s1 - s3;

        #ifdef _TSEL_Debug_Mode
        cout << "desv = " << desv << endl;
        #endif

        // Calculate Stress-Level
        if (c > 0.)
        {
            #ifdef _TSEL_Debug_Mode
            cout << "--->  With cohesion (c > 0)" << endl;
            #endif
            devfh = (2 * c * cos(phi0) + 2 * s3 * sin(phi0)) / (1. - sin(phi0));

        }
        else
        {
            #ifdef _TSEL_Debug_Mode
            cout << "--->  No cohesion (c < 0)" << endl;
            #endif

            if (s3 <= 0.25 * pa )
            {
                phi = phi_calc (phi0, dphi, pa, ( 0.25 * pa));
            }
            else
            {
                phi = phi_calc (phi0, dphi, pa, s3);
            }

            devfh = ( 2 * s3 * sin(phi) / (1. - sin(phi)));

            if (devfh < 0.)
            {
                devfh = -devfh;
            }
        }

        #ifdef _TSEL_Debug_Mode
        cout << "phi0 = " << phi0 << endl;
        cout << "dphi = " << dphi << endl;
        cout << "phi = " << phi << endl;
        cout << "sin(phi) = " << sin(phi) << endl;
        cout << "desvfh = " << devfh << endl;
        #endif

        if ( devfh == 0 )
        {
            if (desv == 0)
            {
                strlev  = 0.;
            }
            else
            {
                strlev = ( 0.25 * pa);
            }
        }
        else
        {
            strlev = ( desv / devfh);
        }

        if (strlev > 10)
        {
            strlev = 10;
        }

        double B_min;
        Bmin = B_calc ( Kb, pa, n, ( 0.25 * pa) );

        double B_el, Et_el, nu_el;

        //Tension Failure
        if (s3 < -0.1 * pa)
        {
            #ifdef _TSEL_Debug_Mode
            cout << "--->  Tension failure (s3 < -0.1*pa.)" << endl;
            #endif
            B_el = B_calc  (Kb, pa, n, ( 0.25 * pa) );

            if (stage == 0)       // NOTE: If this produces problems think about it better and compare with TCL stuff.
            {
                B_el = ( B_el / 100.);
            }

            G_mod = ( B_el / 10.);
            Et_el = ( ( 9.*B_el * G_mod ) / ( 3 * B_el + G_mod ));
            nu_el  = ( ( 3.*B_el - 2.*G_mod ) / ( 6.*B_el + 2.*G_mod ));

            //If not tension failure check other stages
        }
        else
        {

            if (strlev > 0.95 )
            {
                #ifdef _TSEL_Debug_Mode
                cout << "--->  Shear failure (strlev = " << strlev << " > 0.95)" << endl;
                #endif
                // Shear failure
                Ei_el = Ei_calc(K, pa, n, s3);
                B_el  =  B_calc(Kb, pa, n, s3);
                Et_el = ( Ei_el * pow(1 - Rf * 0.95, 2));
                nu_el = ( (3 * B_el - Et_el) / (6 * B_el));
                //puts -nonewline fid_log "Shear   ,  " ;//Et = (format %8.3f Et_el) nu = (format %5.3f nu_el)"

            }
            else // If not shear failure
            {

                //Elastic Unloading
                if (strlev < ( 0.95 * strlev_max))
                {
                    #ifdef _TSEL_Debug_Mode
                    cout << "--->  Elastic unloading   (strlev = " << strlev << " < ( 0.95*strlev_max) = " << 0.95 * strlev_max <<  ") " << endl;
                    #endif
                    Ei_el = Eur_calc(Kur, pa, n, s3);
                    B_el = B_calc(Kb, pa, n, s3);

                    if (B_el < B_min)
                    {
                        B_el = B_min;
                    }

                    Et_el = Ei_el;
                    nu_el = ( (3 * B_el - Et_el) / (6 * B_el));
                    //puts -nonewline fid_log  "Unload  ,  " ;//Et = (format %8.3f Et_el) nu = (format %5.3f nu_el)"

                }
                else
                {
                    #ifdef _TSEL_Debug_Mode
                    cout << "--->  Primary loading (strlev = " << strlev <<  ")" << endl;
                    #endif

                    //Primary loading
                    if ( s3 > 0. )
                    {
                        if ( (c <= 0.) & (s3 < ( 0.25 * pa)))
                        {
                            Ei_el = Ei_calc(K, pa, n, 0.25 * pa);
                            B_el  = B_calc(Kb, pa, n, 0.25 * pa);
                        }
                        else
                        {
                            Ei_el = Ei_calc (K, pa, n, s3);
                            B_el = B_calc  (Kb, pa, n, s3);
                        }

                        Et_el = ( Ei_el * pow(1. - Rf * strlev, 2));
                        nu_el = ( (3 * B_el - Et_el) / (6 * B_el));
                    }
                } ;// Not shear failure
            } ;// Not tension failure
        }

        E = Et_el;
        //v = nu_el;

        if (strlev > strlev_max)
        {
            strlev_max = strlev;
        }

        #ifdef _TSEL_Debug_Mode
        cout << "   E          = " << E << endl;
        cout << "   nu         = " << v << endl;
        #endif
        //cout << "   strlev     = " << strlev << endl;


        ////  END TCL

    }
    // ============================================================================================
    // Dynamic analysis
    // ============================================================================================
    else if (stage == 1 )
    {
    }
    else
    {
        cerr << "TwoStageEquivalentElasticIsotropic3D -> Unknown stage # " << stage << endl;
    }
}
















//Definir funciones para calcular rigideces y amortiguamientos
// ---------------------------------------------------------------

// Elasticidad inicial
inline double Ei_calc (double K, double pa, double n, double sig3)
{
    return K * pa * pow(sig3 / pa, n);
}

//Elasticidad tangente
inline double Et_calc (double K, double pa, double n, double Rf, double c, double phi, double desv, double sig3)
{
    double Ei = K * pa * pow(sig3 / pa, n);
    double corr = Rf * (1 - sin(phi)) * desv / (2 * c * cos(phi) + 2 * sig3 * sin(phi));
    return Ei * pow(1 - corr, 2);
}

// Elasticidad de recarga
inline double Eur_calc (double Kur, double pa, double n, double sig3)
{
    return Kur * pa * pow(sig3 / pa, n);
}

// Angulo de friccion
inline double phi_calc (double phi0, double Dphi, double pa, double sig3)
{
    return phi0 - Dphi * log10(sig3 / pa);
}

// Modulo volumetrico
inline double B_calc (double Kb, double pa, double n, double sig3)
{
    return Kb * pa * pow(sig3 / pa, n);
}

// Modulo Poisson inicial
inline double nui_calc (double G, double F, double pa, double sig3)
{
    return G - F * log10(sig3 / pa);
}






