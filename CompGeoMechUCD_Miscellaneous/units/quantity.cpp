
#include <iostream>
#include <string>
#include <limits>

#include <cmath>
#include "siunit.h"
#include "quantity.h"

using namespace std;



/*------------------------------------------
 Constructors
------------------------------------------*/
Quantity :: Quantity()
{
    value = 0.;
    unit = SIUnit(1.0, 0, 0, 0, "adim");
}

Quantity::Quantity(double v, const SIUnit& u) : value(v), unit(u)
{

}

Quantity::Quantity(double v) : value(v), unit(SIUnit(1.0, 0, 0, 0))
{

}

Quantity::Quantity(int v) : value(v), unit(SIUnit(1.0, 0, 0, 0))
{

}


/* ------------------------------------
Arithmetical operations
--------------------------------------*/
Quantity Quantity :: operator + (const Quantity& other)
{
    Quantity newqty(0., unit);

    if (unit.isConvertible(other.unit))
    {
        newqty.Setvalue(value + other.convert(unit).Getvalue());
    }
    else
    {
        cout << "error: Quantity :: operator + : non compatible units." <<  endl;
        cout << "                                Value set to NaN!" << endl;
        newqty.Setvalue(numeric_limits<double>::quiet_NaN());
    }

    newqty.toSIUnits();
    return newqty;
}

Quantity Quantity :: operator - (const Quantity& other)
{
    Quantity newqty(0., unit);

    if (unit.isConvertible(other.unit))
    {
        newqty.Setvalue(value - other.convert(unit).Getvalue());
    }
    else
    {
        cout << "error: Quantity :: operator - : non compatible units." <<  endl;
        cout << "                                Value set to NaN!" << endl;
        newqty.Setvalue(numeric_limits<double>::quiet_NaN());
    }

    newqty.toSIUnits();
    return newqty;
}

Quantity Quantity :: operator * (const Quantity& other)
{
    Quantity newqty(value * other.value, unit * other.unit);
    newqty.toSIUnits();
    return newqty;
}

Quantity Quantity :: operator / (const Quantity& other)
{
    double quotient;

    // Handle division by zero
    if ((value == 0) & (other.value == 0))
    {
        quotient = numeric_limits<double>::quiet_NaN();
    }

    if ((value > 0) & (other.value == 0))
    {
        quotient = numeric_limits<double>::infinity();
    }

    if ((value < 0) & (other.value == 0))
    {
        quotient = -numeric_limits<double>::infinity();
    }
    else
    {
        quotient = value / other.value;
    }

    Quantity newqty(quotient, unit / other.unit);
    newqty.toSIUnits();
    return newqty;
}

Quantity operator * (const Quantity& a, double b)
{
    Quantity newqty(a.value * b, a.unit);
    newqty.toSIUnits();
    return newqty;
}

Quantity operator * (double a, const Quantity& b)
{
    return b * a;
}

Quantity operator / (const Quantity& a, double b)
{
    double quotient;

    // Handle division by zero
    if ((a.value == 0) & (b == 0))
    {
        quotient = numeric_limits<double>::quiet_NaN();
    }
    else if ((a.value > 0) & (b == 0))
    {
        quotient = numeric_limits<double>::infinity();
    }
    else if ((a.value < 0) & (b == 0))
    {
        quotient = -numeric_limits<double>::infinity();
    }
    else
    {
        quotient = a.value / b;
    }

    Quantity newqty(quotient, a.unit);
    newqty.toSIUnits();
    return newqty;
}

Quantity operator / (double a, const Quantity& b)
{
    double quotient;

    // Handle division by zero
    if ((b.value == 0) & (a == 0))
    {
        quotient = numeric_limits<double>::quiet_NaN();
    }

    if ((b.value == 0) & (a > 0))
    {
        quotient = numeric_limits<double>::infinity();
    }

    if ((b.value == 0) & (a < 0))
    {
        quotient = -numeric_limits<double>::infinity();
    }
    else
    {
        quotient = a / b.value;
    }

    Quantity newqty(quotient, 1.0 / b.unit);

    newqty.toSIUnits();
    return newqty;

}

Quantity pow(const Quantity& a, const Quantity& exponent)
{
    SIUnit ue = exponent.Getunit();
    SIUnit ua = a.Getunit();

    if (isAdimensional(ue))
    {
        short int expnt =
            static_cast<short int>(exponent.Getvalue() ) ;

        double scale = pow( ua.Getscale(), static_cast<int>(expnt) );

        short int m = ua.Getdim_m() * expnt;
        short int l = ua.Getdim_l() * expnt;
        short int t = ua.Getdim_t() * expnt;

        //cout << "m = " << m << endl;
        //cout << "l = " << l << endl;
        //cout << "t = " << t << endl;

        double value = pow(a.value, static_cast<int>(expnt));

        SIUnit u(scale, m, l, t);
        Quantity newqty(value, u);
        newqty.toSIUnits();
        return newqty;
    }
    else
    {
        cout << " error : '^' Operator : Quantities can only be raised to an adimensional integer power." << endl;
        Quantity newqty(numeric_limits<double>::quiet_NaN(), ua);
        newqty.toSIUnits();
        return newqty;
    }
}


/* ------------------------------------
Comparison
--------------------------------------*/

bool operator == (const Quantity& a, const Quantity& b)
{
    SIUnit ua = a.Getunit();
    SIUnit ub = b.Getunit();

    if (ua.isConvertible(ub)) // If the units are compatible
    {
        double da = a.Getvalue() * ua.Getscale();
        double db = b.Getvalue() * ub.Getscale();
        return da == db;
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator ==: Comparing incompatible units returns false." << endl << endl;
        return 0;
    }
}

bool operator != (const Quantity& a, const Quantity& b)
{
    SIUnit ua = a.Getunit();
    SIUnit ub = b.Getunit();

    if (ua.isConvertible(ub)) // If the units are compatible
    {
        double da = a.Getvalue() * ua.Getscale();
        double db = b.Getvalue() * ub.Getscale();
        return da != db;
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator !=: Comparing incompatible units returns false." << endl << endl;
        return 0;
    }
}

bool operator >= (const Quantity& a, const Quantity& b)
{
    SIUnit ua = a.Getunit();
    SIUnit ub = b.Getunit();

    if (ua.isConvertible(ub)) // If the units are compatible
    {
        double da = a.Getvalue() * ua.Getscale();
        double db = b.Getvalue() * ub.Getscale();
        return da >= db;
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator >=: Comparing incompatible units returns false." << endl << endl;
        return 0;
    }
}

bool operator <= (const Quantity& a, const Quantity& b)
{
    SIUnit ua = a.Getunit();
    SIUnit ub = b.Getunit();

    if (ua.isConvertible(ub)) // If the units are compatible
    {
        double da = a.Getvalue() * ua.Getscale();
        double db = b.Getvalue() * ub.Getscale();
        return da <= db;
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator <=: Comparing incompatible units returns false." << endl << endl;
        return 0;
    }
}

bool operator > (const Quantity& a, const Quantity& b)
{
    SIUnit ua = a.Getunit();
    SIUnit ub = b.Getunit();

    if (ua.isConvertible(ub)) // If the units are compatible
    {
        double da = a.Getvalue() * ua.Getscale();
        double db = b.Getvalue() * ub.Getscale();
        return da > db;
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator >: Comparing incompatible units returns false." << endl << endl;
        return 0;
    }
}

bool operator < (const Quantity& a, const Quantity& b)
{
    SIUnit ua = a.Getunit();
    SIUnit ub = b.Getunit();

    if (ua.isConvertible(ub)) // If the units are compatible
    {
        double da = a.Getvalue() * ua.Getscale();
        double db = b.Getvalue() * ub.Getscale();
        return da < db;
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator <: Comparing incompatible units returns false." << endl << endl;
        return 0;
    }
}

bool operator & (const Quantity& a, const Quantity& b)
{
    SIUnit ua = a.Getunit();
    SIUnit ub = b.Getunit();

    if (isAdimensional(ua) && isAdimensional(ub)  ) // Only Adimensional quantities can be ANDed
    {
        double da = a.Getvalue() * ua.Getscale();
        double db = b.Getvalue() * ub.Getscale();
        return (da == 1.0) & (db == 1.0);
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator &: Only Adimensional quantities can be 'and'ed. " << endl << endl;
        return 0;
    }
}

bool operator | (const Quantity& a, const Quantity& b)
{
    SIUnit ua = a.Getunit();
    SIUnit ub = b.Getunit();

    if (isAdimensional(ua) && isAdimensional(ub)  ) // Only Adimensional quantities can be ORed
    {
        double da = a.Getvalue() * ua.Getscale();
        double db = b.Getvalue() * ub.Getscale();
        return (da == 1.0) | (db == 1.0);
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator |: Only Adimensional quantities can be 'oe'ed. " << endl << endl;
        return 0;
    }
}

bool operator ~ (const Quantity& a)
{
    SIUnit ua = a.Getunit();

    if (isAdimensional(ua)) // Only Adimensional quantities can be NOTed
    {
        double da = a.Getvalue() * ua.Getscale();
        return ~(da == 1.0);
    }
    else
    {
        cout << endl << "Warning! Quantity :: operator ~: Only Adimensional quantities can be 'not'ed. " << endl << endl;
        return 0;
    }
}


/* ------------------------------------
Setters and getters
--------------------------------------*/

void Quantity :: Setvalue(double v)
{
    value = v;
}

double Quantity::Getvalue() const
{
    return value;
}

SIUnit Quantity::Getunit() const
{
    return unit;
}

/*------------------------------------------
 Conversion
------------------------------------------*/
Quantity Quantity::convert(const SIUnit& otherUnit) const
{
    return Quantity(value * unit.Getscale() / otherUnit.Getscale(), otherUnit);
}

Quantity Quantity::convert(const Quantity& q) const
{
    return Quantity(value * unit.Getscale() / (q.unit.Getscale() * q.value), q.unit);
}

void Quantity::toSIUnits()
{
    value = value * unit.Getscale();
    unit = SIUnit(1.0, unit.Getdim_m(),  unit.Getdim_l(),  unit.Getdim_t(), unit.GetSISymbol() );
}

Quantity Quantity::inSIUnits() const
{
    return Quantity(value * unit.Getscale(),
                    SIUnit(1.0, unit, unit.GetSISymbol()
                          ));
}

/*------------------------------------------
 Friend functions
------------------------------------------*/

ostream& operator<<(ostream& out, const Quantity& qt)
{
    out << qt.Getvalue() << " [" << qt.Getunit().GetSymbol() << "]";
    return out;
}



// ============================================================================================================
// Auxiliary helper functions (tests)
// ============================================================================================================

bool isAdimensional(const Quantity& q)
{
    //cout << q << " is" <<
    //(isAdimensional(q.Getunit()) ? " " : " not") <<
    //"an adimensional quantity." << endl;
    return isAdimensional(q.Getunit());
}

bool isAdimensional(const string& var_name, const Quantity& q)
{
    return isAdimensional(var_name,q.Getunit());
}

bool isMass(const string& var_name, const Quantity& q)
{
    return isMass(var_name,q.Getunit());
}

bool isLength(const string& var_name, const Quantity& q)
{
    return isLength(var_name,q.Getunit());
}

bool isTime(const string& var_name, const Quantity& q)
{
    return isTime(var_name,q.Getunit());
}

bool isFrequency(const string& var_name, const Quantity& q)
{
    return isFrequency(var_name,q.Getunit());
}

bool isArea(const string& var_name, const Quantity& q)
{
    return isArea(var_name,q.Getunit());
}

bool isVolume(const string& var_name, const Quantity& q)
{
    return isVolume(var_name,q.Getunit());
}

bool isForce(const string& var_name, const Quantity& q)
{
    return isForce(var_name,q.Getunit());
}

bool isEnergy(const string& var_name, const Quantity& q)
{
    return isEnergy(var_name,q.Getunit());
}

bool isTorque(const string& var_name, const Quantity& q)
{
    return isTorque(var_name,q.Getunit());
}

bool isPressure(const string& var_name, const Quantity& q)
{
    return isPressure(var_name,q.Getunit());
}

bool isBodyForce(const string& var_name, const Quantity& q)
{
    return isBodyForce(var_name,q.Getunit());
}

bool isDensity(const string& var_name, const Quantity& q)
{
    return isDensity(var_name,q.Getunit());
}

bool isVelocity(const string& var_name, const Quantity& q)
{
    return isVelocity(var_name,q.Getunit());
}

bool isAcceleration(const string& var_name, const Quantity& q)
{
    return isAcceleration(var_name,q.Getunit());
}

bool isAreaMomentOfInertia(const string& var_name, const Quantity& q)
{
    return isAreaMomentOfInertia(var_name,q.Getunit());
}

bool isMassMomentOfInertia(const string& var_name, const Quantity& q)
{
    return isMassMomentOfInertia(var_name,q.Getunit());
}

