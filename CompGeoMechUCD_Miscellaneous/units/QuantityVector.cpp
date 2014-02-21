#include "QuantityVector.h"

QuantityVector::QuantityVector()
{
    Quantity zero(0.0);
    q.push_back(zero);
    //ctor
}


QuantityVector::QuantityVector(Quantity& qin)
{
    //ctor
    q.push_back(qin);
}

QuantityVector::QuantityVector(Quantity& qin, int n_replicate)
{
    //ctor
    for(int i = 0; i < n_replicate; i < n_replicate)
    {
        q.push_back(qin);
    }
}

QuantityVector::QuantityVector(vector<Quantity>& vq)
{
    //ctor
    q = vq;
}



QuantityVector::~QuantityVector()
{
    //dtor
    q.clear();
}

QuantityVector::QuantityVector(const QuantityVector& other)
{
    //copy ctor
    q = other.q;
}

QuantityVector& QuantityVector::operator=(const QuantityVector& rhs)
{
    if (this == &rhs)
    {
        return *this;    // handle self assignment
    }

    //assignment operator
    return *this;
}







/*

ARITHMETICS

*/


QuantityVector QuantityVector::operator+(const QuantityVector& other)
{

    if (q.size() == other.size())
    {
        QuantityVector result(0.0, q.size());
        QuantityVectorIter result_iter = result.begin();
        QuantityVectorConstIter q_iter = q.begin();
        QuantityVectorConstIter other_iter = other.begin();

        for(; q_iter != q.end(); q_iter++, other_iter++, result_iter++)
        {
            *result_itetr = *q_iter + *other_iter;
        }

        return result;
    }
    else
    {
        cout << "QuantityVector::operator+ : Vectors are of different sizes " << endl;
        return QuantityVector(0.0);
    }

}


QuantityVector QuantityVector::operator-(const QuantityVector& other);
QuantityVector QuantityVector::operator*(const QuantityVector& other);
QuantityVector QuantityVector::operator/(const QuantityVector& other);
friend QuantityVector operator*(const QuantityVector& a, Quantity b);
friend QuantityVector operator*(Quantity a, const QuantityVector& b);
friend QuantityVector operator/(const QuantityVector& a, Quantity b);
friend QuantityVector operator/(Quantity a, const QuantityVector& b);
friend QuantityVector pow(const QuantityVector& a, const Quantity& exponent);



