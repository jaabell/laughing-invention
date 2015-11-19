#ifndef _FEIVECTOR_H
#define _FEIVECTOR_H



#include "feiast.h"
#include <vector>

// Numbers are the most basic of expressions. They just hold a Quantity.
class FeiVector : public Expression
{
    std::vector<Quantity> qtys;
    int evalualte_at_index;
public:

    FeiVector(void) : evalualte_at_index(0) {}

    // FeiVector (double val): qty(val, ESSIunits::unitless), evalualte_at_index(0)
    // {

    // }
    // FeiVector (double val, SIUnit u): qty(val, u), evalualte_at_index(0) {}
    // FeiVector (Quantity q): qty(q), evalualte_at_index(0)
    // {
    //     qtys.push_back(q);
    // }

    // Copy constructor
    FeiVector (const FeiVector& other) :  evalualte_at_index(0)
    {
        qtys = other.qtys;
    }

    FeiVector& operator = (const FeiVector& other)
    {
        if (&other != this)
        {
            qtys = other.qtys;
        }

        return *this;
    }

    virtual Expression* clone ()
    {
        return new FeiVector (*this);
    }

    virtual Quantity value ()
    {
        return qtys[evalualte_at_index];
    }

    void select_component(int i)
    {
        evalualte_at_index = i;
    }

    int size()
    {
        return (int) qtys.size();
    }

    void to_std_vector_double(std::vector<double> &other)
    {
        other.clear();
        for (auto &value : qtys)
        {
            other.push_back(value.Getvalue());
        }
    }

    void add_component(const Quantity &q)
    {
        cout << "Adding " << q << endl;
        qtys.push_back(q);
    }

    void print_vector()
    {
        for (auto & val : qtys)
        {
            cout << val << " ";
        }
    }

};



#endif