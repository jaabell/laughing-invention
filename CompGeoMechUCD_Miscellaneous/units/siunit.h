#ifndef SIUNIT_H
#define SIUNIT_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class SIUnit
{
    public:
        // Constructors
        SIUnit();
        SIUnit(double s, short int m, short int l, short int t);
        SIUnit(double s, short int m, short int l, short int t, const string& sym);
        SIUnit(double s, const SIUnit& other);
        SIUnit(double s, const SIUnit& other, const string& sym);

        // Arithmetic overloads
        SIUnit operator * (const SIUnit b) const;
        SIUnit operator / (const SIUnit b) const;
        friend SIUnit operator * (const SIUnit& a, double b);
        friend SIUnit operator * (double a, const SIUnit& b);
        friend SIUnit operator / (const SIUnit& a, double b);
        friend SIUnit operator / (double a, const SIUnit& b);

        //Logicals
        bool operator == (const SIUnit b) const;
        bool isConvertible(const SIUnit& other) const;

        //Setters and getters
        double Getscale() const
        {
            return scale;
        }
        short int Getdim_m() const
        {
            return dim_m;
        }
        short int Getdim_l() const
        {
            return dim_l;
        }
        short int Getdim_t() const
        {
            return dim_t;
        }
        void SetSymbol(string n);
        string GetSymbol() const;
        string GetSISymbol() const;

        //Friends
        friend ostream& operator<<(ostream& out, const SIUnit& unit);

    protected:
        void UpdateSymbol();

    private:
        double scale;
        short int dim_m;   // kilogram (kg)
        short int dim_l;   // meters   (m)
        short int dim_t;   // seconds  (s)
        string symbol;
};


// ============================================================================================================
// Auxiliary helper functions (tests)
// ============================================================================================================


bool isAdimensional(const SIUnit& u);
bool isMass     (const SIUnit& u);
bool isLength   (const SIUnit& u);
bool isTime     (const SIUnit& u);
bool isFrequency(const SIUnit& u);
bool isArea     (const SIUnit& u);
bool isVolume   (const SIUnit& u);
bool isForce    (const SIUnit& u);
bool isEnergy   (const SIUnit& u);
bool isTorque   (const SIUnit& u);
bool isPressure (const SIUnit& u);
bool isBodyForce(const SIUnit& u);
bool isDensity  (const SIUnit& u);
bool isVelocity (const SIUnit& u);
bool isAcceleration(const SIUnit& u);
bool isAreaMomentOfInertia(const SIUnit& u);
bool isMassMomentOfInertia(const SIUnit& u);

template<short int m, short int l, short int t>
bool isThisUnit(const SIUnit& u )
{
    return (( u.Getdim_m() == m ) &
            ( u.Getdim_l() == l ) &
            ( u.Getdim_t() == t ));
}
#endif // SIUNIT_H
