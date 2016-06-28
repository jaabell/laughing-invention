#ifndef SIUNIT_H
#define SIUNIT_H

#include <string>
#include <sstream>

// using namespace std;

class SIUnit
{
public:
    // Constructors
    SIUnit();
    SIUnit(double s, short int m, short int l, short int t);
    SIUnit(double s, short int m, short int l, short int t, const std::string& sym);
    SIUnit(double s, const SIUnit& other);
    SIUnit(double s, const SIUnit& other, const std::string& sym);

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
    void SetSymbol(std::string n);
    std::string GetSymbol() const;
    std::string GetSISymbol() const;

    //Friends
    friend std::ostream& operator<<(std::ostream& out, const SIUnit& unit);

protected:
    void UpdateSymbol();

private:
    double scale;
    short int dim_m;   // kilogram (kg)
    short int dim_l;   // meters   (m)
    short int dim_t;   // seconds  (s)
    std::string symbol;
};


// ============================================================================================================
// Auxiliary helper functions (tests)
// ============================================================================================================

bool isAdimensional(const SIUnit& u);
bool isAdimensional(const std::string& var_name, const SIUnit& u);
bool isMass     (const std::string& var_name, const SIUnit& u);
bool isLength   (const std::string& var_name, const SIUnit& u);
bool isTime     (const std::string& var_name, const SIUnit& u);
bool isFrequency(const std::string& var_name, const SIUnit& u);
bool isArea     (const std::string& var_name, const SIUnit& u);
bool isVolume   (const std::string& var_name, const SIUnit& u);
bool isForce    (const std::string& var_name, const SIUnit& u);
bool isEnergy   (const std::string& var_name, const SIUnit& u);
bool isTorque   (const std::string& var_name, const SIUnit& u);
bool isPressure (const std::string& var_name, const SIUnit& u);
bool isBodyForce(const std::string& var_name, const SIUnit& u);
bool isDensity  (const std::string& var_name, const SIUnit& u);
bool isVelocity (const std::string& var_name, const SIUnit& u);
bool isAcceleration(const std::string& var_name, const SIUnit& u);
bool isAreaMomentOfInertia(const std::string& var_name, const SIUnit& u);
bool isMassMomentOfInertia(const std::string& var_name, const SIUnit& u);

template<short int m, short int l, short int t>
bool isThisUnit(const std::string& var_name, const SIUnit& u )
{
    bool verified = (( u.Getdim_m() == m ) &
                   ( u.Getdim_l() == l ) &
                   ( u.Getdim_t() == t ));

    if (!verified)
         std::cout << std::endl << std::endl << "     Error: " << var_name << " should have unit as " << "[kg^("<<m<<")*m^(" << l << ")*s^(" << t << ")] " << std::endl;

    return verified;
}
#endif // SIUNIT_H

