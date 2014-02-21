
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
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 0 ) &
            ( u.Getdim_t() == 0 ));
}
bool isMass(const SIUnit& u)
{
    return (( u.Getdim_m() == 1 ) &
            ( u.Getdim_l() == 0 ) &
            ( u.Getdim_t() == 0 ));
}

bool isLength(const SIUnit& u)
{
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 1 ) &
            ( u.Getdim_t() == 0 ));
}

bool isTime(const SIUnit& u)
{
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 0 ) &
            ( u.Getdim_t() == 1 ) );
}

bool isFrequency(const SIUnit& u)
{
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 0 ) &
            ( u.Getdim_t() == -1 ) );
}


bool isArea(const SIUnit& u)
{
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 2 ) &
            ( u.Getdim_t() == 0 ) );
}

bool isVolume(const SIUnit& u)
{
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 3 ) &
            ( u.Getdim_t() == 0 ) );
}

bool isForce(const SIUnit& u)
{
    return (( u.Getdim_m() == 1 ) &
            ( u.Getdim_l() == 1 ) &
            ( u.Getdim_t() == -2 ) );
}

bool isEnergy(const SIUnit& u)
{
    return (( u.Getdim_m() == 1 ) &
            ( u.Getdim_l() == 2 ) &
            ( u.Getdim_t() == -2 ) );
}

bool isTorque(const SIUnit& u)
{
    return (( u.Getdim_m() == 1 ) &
            ( u.Getdim_l() == 2 ) &
            ( u.Getdim_t() == -2 ) );
}

bool isPressure(const SIUnit& u)
{
    return (( u.Getdim_m() == 1 ) &
            ( u.Getdim_l() == -1 ) &
            ( u.Getdim_t() == -2 ) );
}

bool isBodyForce(const SIUnit& u)
{
    return (( u.Getdim_m() == 1 ) &
            ( u.Getdim_l() == -2 ) &
            ( u.Getdim_t() == -2 ) );
}

bool isDensity(const SIUnit& u)
{
    return (( u.Getdim_m() == 1 ) &
            ( u.Getdim_l() == -3 ) &
            ( u.Getdim_t() == 0 ) );
}

bool isVelocity(const SIUnit& u)
{
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 1 ) &
            ( u.Getdim_t() == -1 ) );
}

bool isAcceleration(const SIUnit& u)
{
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 1 ) &
            ( u.Getdim_t() == -2 ));
}

bool isAreaMomentOfInertia(const SIUnit& u)
{
    return (( u.Getdim_m() == 0 ) &
            ( u.Getdim_l() == 4 ) &
            ( u.Getdim_t() == 0 ));
}

bool isMassMomentOfInertia(const SIUnit& u)
{
    return (( u.Getdim_m() == 1 ) &
            ( u.Getdim_l() == 2 ) &
            ( u.Getdim_t() == 0 ));
}
