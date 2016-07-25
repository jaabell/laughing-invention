
#include <iostream>
#include <string>
#include <sstream>

#include "siunit.h"

using namespace std;


/*------------------------------------------
 Constructors
------------------------------------------*/
SIUnit :: SIUnit()
{
    // Adimensional
    scale = 1;
    dim_m = 0;
    dim_l = 0;
    dim_t = 0;
    UpdateSymbol();
}

SIUnit :: SIUnit(double s, short int m, short int l, short int t)  :
    scale(s), dim_m(m), dim_l(l), dim_t(t)
{
    UpdateSymbol();
}

SIUnit :: SIUnit(double s, short int m, short int l, short int t, const string& sym) :
    scale(s), dim_m(m), dim_l(l), dim_t(t), symbol(sym)
{
}

SIUnit :: SIUnit(double s, const SIUnit& other)
{
    scale = other.scale * s;
    dim_m = other.dim_m;
    dim_l = other.dim_l;
    dim_t = other.dim_t;
    symbol = other.symbol;
}

SIUnit :: SIUnit(double s, const SIUnit& other, const string& sym)
{
    scale = other.scale * s;
    dim_m = other.dim_m;
    dim_l = other.dim_l;
    dim_t = other.dim_t;
    symbol = sym;
}

/* ------------------------------------
Arithmetical operations
--------------------------------------*/
SIUnit SIUnit :: operator * (const SIUnit b) const
{

    SIUnit newunit(scale * b.scale, dim_m + b.dim_m, dim_l + b.dim_l, dim_t + b.dim_t);
    /*if (isAdimensional(*this))
    {
        newunit.SetSymbol(b.symbol);
    } else {
        newunit.SetSymbol(this->symbol + string("*") + b.symbol);
    }*/
    newunit.UpdateSymbol();
    return newunit;
}

SIUnit SIUnit :: operator / (const SIUnit b) const
{
    SIUnit newunit(scale / b.scale, dim_m - b.dim_m, dim_l - b.dim_l, dim_t - b.dim_t);
    /*if (isAdimensional(*this))
    {
        newunit.SetSymbol("1" + string("/") + b.symbol);
    } else {
        newunit.SetSymbol(this->symbol + string("/") + b.symbol);
    }*/
    newunit.UpdateSymbol();
    return newunit;
}

SIUnit operator * (const SIUnit& a, double b)
{
    SIUnit newunit(a.scale * b, a);
    return newunit;
}

SIUnit operator * (double a, const SIUnit& b)
{
    return b * a;
}

SIUnit operator / (const SIUnit& a, double b)
{
    SIUnit newunit(a.scale / b, a);
    return newunit;
}

SIUnit operator / (double a, const SIUnit& b)
{
    SIUnit newunit( a / b.scale, -b.dim_m, -b.dim_l, -b.dim_t);
    return newunit;
}



/* ------------------------------------
Logical operations
--------------------------------------*/
bool SIUnit :: operator == (const SIUnit other) const
{
    return scale == other.scale && isConvertible(other);
}

bool SIUnit :: isConvertible(const SIUnit& other) const
{
    return dim_m == other.dim_m && dim_l == other.dim_l && dim_t == other.dim_t;
}


/* ------------------------------------
Setters and getters
--------------------------------------*/


void SIUnit::SetSymbol(string n)
{
    symbol = n;
}
string SIUnit::GetSymbol() const
{
    return symbol;
}

void SIUnit::UpdateSymbol()
{
    symbol = GetSISymbol();
}


string SIUnit::GetSISymbol() const
{
    stringstream ss;

    if ((dim_m == 0) & (dim_l == 0) & (dim_t == 0) & (scale != 0.))
    {
        //ss << "adim";
        ss << "";
    }
    else
    {
        ss << "";

        if (dim_m != 0)
        {
            ss <<  "kg";

            if (dim_m > 1 || dim_m < 0)
            {
                ss << "^";
                ss << dim_m;
            }
        }

        if (dim_l != 0)
        {
            if (dim_m != 0)
            {
                ss << "*";
            }

            ss <<  "m";

            if (dim_l > 1 || dim_l < 0)
            {
                ss <<  "^";
                ss <<  dim_l;
            }
        }

        if (dim_t != 0)
        {
            if (dim_l != 0)
            {
                ss << "*";
            }

            ss <<  "s";

            if (dim_t > 1 || dim_t < 0)
            {
                ss <<  "^";
                ss <<  dim_t;
            }
        }
    }

    return ss.str();
}



/*------------------------------------------
 Friend functions
------------------------------------------*/
ostream& operator<<(ostream& out, const SIUnit& unit)
{
    out << unit.GetSymbol();
    return out;
}


// ============================================================================================================
// Auxiliary helper functions (tests)
// ============================================================================================================

bool isAdimensional(const SIUnit& u)
{
    return  (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 0 ) &
            ( u.Getdim_t() == 0 ));
}

bool isAdimensional(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 0 ) &
                   ( u.Getdim_t() == 0 ));
    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should be dimensionless" << endl;

    return verified;
}

bool isMass(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 1 ) &
                   ( u.Getdim_l() == 0 ) &
                   ( u.Getdim_t() == 0 ));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Mass [kg]" << endl;

    return verified;
}


bool isLength(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 1 ) &
                   ( u.Getdim_t() == 0 ));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Length i.e. [m]" << endl;

    return verified;
}


bool isTime(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 0 ) &
                   ( u.Getdim_t() == 1 ));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Time i.e. [s]" << endl;

    return verified;
}

bool isFrequency(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 0 ) &
                   ( u.Getdim_t() == -1));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Frequency i.e. [s^(-1)] " << endl;

    return verified;
}


bool isArea(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 2 ) &
                   ( u.Getdim_t() == 0 ));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Area i.e. [m^(2)]" << endl;

    return verified;

}

bool isVolume(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 3 ) &
                   ( u.Getdim_t() == 0 ));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Volume i.e. [m^(3)] " << endl;

    return verified;
}

bool isForce(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 1 ) &
                   ( u.Getdim_l() == 1 ) &
                   ( u.Getdim_t() == -2));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Force i.e. [kg*m/s^(2)] " << endl;

    return verified;
}

bool isEnergy(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 1 ) &
                   ( u.Getdim_l() == 2 ) &
                   ( u.Getdim_t() == -2));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Energy i.e. [kg*m^(2)/s^(2)] " << endl;

    return verified;
}

bool isTorque(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 1 ) &
                   ( u.Getdim_l() == 2 ) &
                   ( u.Getdim_t() == -2));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Torque i.e. [kg*m^(2)/s^(2)] " << endl;

    return verified;
}

bool isPressure(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 1 ) &
                   ( u.Getdim_l() == -1) &
                   ( u.Getdim_t() == -2));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Pressure i.e. [kg/m/s^(2)] " << endl;

    return verified;
}

bool isBodyForce(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 1 ) &
                   ( u.Getdim_l() == -2 )&
                   ( u.Getdim_t() == -2));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Body Force i.e. [kg/m^(2)/s^(2)] " << endl;

    return verified;

}

bool isDensity(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 1 ) &
                   ( u.Getdim_l() == -3) &
                   ( u.Getdim_t() == 0 ));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Density i.e. [kg/m^(3)] " << endl;

    return verified;

}

bool isVelocity(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 1 ) &
                   ( u.Getdim_t() == -1));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Force i.e. [kg*m/s^(2)] " << endl;

    return verified;
}

bool isAcceleration(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 1 ) &
                   ( u.Getdim_t() == -2));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of acceleration i.e. [m/s^(2)] " << endl;

    return verified;
}

bool isAreaMomentOfInertia(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 0 ) &
                   ( u.Getdim_l() == 4 ) &
                   ( u.Getdim_t() == 0 ));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Area Moment of Interia i.e. [m^(4)] " << endl;

    return verified;
}


bool isMassMomentOfInertia(const string& var_name, const SIUnit& u)
{
    bool verified = (( u.Getdim_m() == 1 ) &
                   ( u.Getdim_l() == 2 ) &
                   ( u.Getdim_t() == 0 ));

    if (!verified)
         cout << endl << endl << "     Error: " << var_name << " should have unit of Mass Moment of Intertia i.e. [kg*m^(2)] " << endl;

    return verified;

}

