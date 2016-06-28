#ifndef QUANTITY_H
#define QUANTITY_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Quantity
{
    public:
        Quantity();
        Quantity(double v, const SIUnit& u);
        Quantity(double v);
        Quantity(int v);

        // Setter and getters
        double Getvalue() const;
        void Setvalue(double val);
        SIUnit Getunit() const;

        //Conversion
        Quantity convert(const SIUnit& otherUnit) const;
        Quantity convert(const Quantity& q) const;
        Quantity inSIUnits() const;
        void toSIUnits();

        //Arithmetics
        Quantity operator+(const Quantity& other);
        Quantity operator-(const Quantity& other);
        Quantity operator*(const Quantity& other);
        Quantity operator/(const Quantity& other);
        friend Quantity operator*(const Quantity& a, double b);
        friend Quantity operator*(double a, const Quantity& b);
        friend Quantity operator/(const Quantity& a, double b);
        friend Quantity operator/(double a, const Quantity& b);
        friend Quantity pow(const Quantity& a, const Quantity& exponent);

        // Comparison
        friend bool operator==(const Quantity& a, const Quantity& b);
        friend bool operator!=(const Quantity& a, const Quantity& b);
        friend bool operator>=(const Quantity& a, const Quantity& b);
        friend bool operator<=(const Quantity& a, const Quantity& b);
        friend bool operator>(const Quantity& a, const Quantity& b);
        friend bool operator<(const Quantity& a, const Quantity& b);

        //Boolean
        friend bool operator&(const Quantity& a, const Quantity& b);
        friend bool operator|(const Quantity& a, const Quantity& b);
        friend bool operator~(const Quantity& a);

        //Dispayage
        void display();
        void getString();

        //Streaming
        friend ostream& operator<<(ostream& out, const Quantity& qt);

    protected:
    private:
        double value;
        SIUnit unit;
};



// ============================================================================================================
// Auxiliary helper functions (tests)
// ============================================================================================================

bool isAdimensional(const Quantity& q);
bool isAdimensional(const string& var_name, const Quantity& q);
bool isMass(const string& var_name, const Quantity& q);
bool isLength(const string& var_name, const Quantity& q);
bool isTime(const string& var_name, const Quantity& q);
bool isFrequency(const string& var_name, const Quantity& q);
bool isArea(const string& var_name, const Quantity& q);
bool isVolume(const string& var_name, const Quantity& q);
bool isForce(const string& var_name, const Quantity& q);
bool isEnergy(const string& var_name, const Quantity& q);
bool isTorque(const string& var_name, const Quantity& q);
bool isPressure(const string& var_name, const Quantity& q);
bool isBodyForce(const string& var_name, const Quantity& q);
bool isDensity(const string& var_name, const Quantity& q);
bool isVelocity(const string& var_name, const Quantity& q);
bool isAcceleration(const string& var_name, const Quantity& q);
bool isAreaMomentOfInertia(const string& var_name, const Quantity& q);
bool isMassMomentOfInertia(const string& var_name, const Quantity& q);
bool isMassMomentOfInertia(const string& var_name, const Quantity& q);

// For any other cases not defined herein
template<short int m, short int l, short int t>
bool isThisUnit(const string& var_name, const Quantity& q)
{
    return isThisUnit<m, l, t>(var_name,q.Getunit());
}
#endif // QUANTITY_H

