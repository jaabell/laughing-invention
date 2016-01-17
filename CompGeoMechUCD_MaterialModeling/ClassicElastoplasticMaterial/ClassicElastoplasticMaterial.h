///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:
// CLASS:
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:
// DATE:
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////



#ifndef ClassicElastoplasticMaterial_H
#define ClassicElastoplasticMaterial_H


#include "NDMaterialLT.h"
#include <G3Globals.h>
#include <iostream>
#include "../../ltensor/LTensor.h"
#include <Channel.h>
#include <limits>

#include "ClassicElastoplasticityGlobals.h"


#define ClassicElastoplasticMaterial_MAXITER_BRENT 20
#define TOLERANCE1 1e-6
/*

YieldFunctionType, ElasticityType, PlasticFlowType, HardeningLawType are the user-defined classes
that provide the different components of this elastoplastic model. Called Base Elastoplastic Template
Classes (BET Classes)

* All BET Classes  must provide:

C++ "Rule of 5"
 + An empty constructor (default constructor)
 + A copy constructor
 + A copy assignment operator
 + A move constructor
 + A move assign operator
...and
 + A name member function that returns a std::string. For example.
    SomeYieldFunctionType yf(...parameters...);
    std::string name = yf.name();
 + A getObjectSize() function. Returnig size in bytes of the object in memory.


* YieldFunctionType must additionally provide



* ElasticityType must additionally provide



* PlasticFlowType must additionally provide





*/



// This anonymous namespace avoids multiple-definition name clashes.
namespace
{

void printTensor(string name, const DTensor2 &v)
{


    std::cout << name << " = [ " ;
    std::cout << v(0, 0) << " "
              << v(1, 1) << " "
              << v(2, 2) << " "
              << v(0, 1) << " "
              << v(0, 2) << " "
              << v(1, 2) << " "
              << v(1, 0) << " "
              << v(2, 0) << " "
              << v(2, 1) << " ]" << std::endl;
}

void printTensor4(string name, const DTensor4 &v)
{


    std::cout << name << " = [ " ;
    for (int ii = 0; ii < 3; ii++)
        for (int jj = 0; jj < 3; jj++)
            for (int kk = 0; kk < 3; kk++)
                for (int ll = 0; ll < 3; ll++)
                {
                    std::cout << v(ii, jj, kk, ll ) << " ";
                }
    std::cout << "]" << std::endl;
}

}


template < class ElasticityType, class YieldFunctionType, class PlasticFlowType, class MaterialInternalVariablesType, int thisClassTag, class T >
class ClassicElastoplasticMaterial : public NDMaterialLT
{

public:



//==================================================================================================
//  Void constructor
//==================================================================================================
//Empty constructor is needed for parallel, sets all the pointers to zero. Resources will be
//    allocated later
// ClassicElastoplasticMaterial( )
//     : NDMaterialLT(0, thisClassTag),
//       NDMaterialLT(tag, thisClassTag),
//       TrialStress(3, 3, 0.0),
//       TrialStrain(3, 3, 0.0),
//       TrialPlastic_Strain(3, 3, 0.0),
//       CommitStress(3, 3, 0.0),
//       CommitStrain(3, 3, 0.0),
//       CommitPlastic_Strain(3, 3, 0.0),
//       Stiffness(3, 3, 3, 3, 0.0),
//       yf(),
//       et(),
//       pf(),
//       vars()
// {

// }



//==================================================================================================
//  Main constructor
//==================================================================================================
// Constructor... invokes copy constructor for base elastoplastic template parameters
    ClassicElastoplasticMaterial(int tag,
                                 double rho_,
                                 double p0,
                                 const YieldFunctionType& yf_in,
                                 const ElasticityType& et_in,
                                 const PlasticFlowType& pf_in,
                                 const MaterialInternalVariablesType& vars_in
                                )
        : NDMaterialLT(tag, thisClassTag),
          rho(rho_),
          TrialStrain(3, 3, 0.0),
          TrialStress(3, 3, 0.0),
          TrialPlastic_Strain(3, 3, 0.0),
          CommitStress(3, 3, 0.0),
          CommitStrain(3, 3, 0.0),
          CommitPlastic_Strain(3, 3, 0.0),
          yf(yf_in),
          et(et_in),
          pf(pf_in),
          vars(vars_in)
    {
        // cout << "Copy ctor in CEP" << endl;
        //Set initial stress to some value. Needed for models
        // like Drucker-Prager which blow up at sigma_ii = 0 :)
        TrialStress(0, 0) = -p0;
        TrialStress(1, 1) = -p0;
        TrialStress(2, 2) = -p0;

        CommitStress(0, 0) = -p0;
        CommitStress(1, 1) = -p0;
        CommitStress(2, 2) = -p0;
    }



//==================================================================================================
//  Destructor
//==================================================================================================
    ~ClassicElastoplasticMaterial(void)
    {

    }




//==================================================================================================
//  Class type function
//==================================================================================================
// Returns a null-terminated character string with the name of the class
    const char *getClassType(void) const
    {
        std::string name("ClassicElastoplasticMaterial");

        return name.c_str();
    };

    double getRho(void)
    {
        return rho;
    }

    double getPressure(void)
    {
        using namespace ClassicElastoplasticityGlobals;
        return -CommitStress(i, i) / 3;
    }

//==================================================================================================
//  Setters for components (invoke assign copy on BET Classes)
//==================================================================================================

    // void setYieldFunction(YieldFunctionType& yf_)
    // {
    //     yf = std::move(yf_);
    // }

    // void setElasticity(ElasticityType& et_)
    // {
    //     et = std::move(et_);
    // }

    // void setPlasticFlow(PlasticFlowType& pf_)
    // {
    //     pf = std::move(pf_);
    // }


//==================================================================================================
//  Set Trial strain and trial strain increment
//==================================================================================================
    // For total strain-based elements.
    // Receives the current total strain at a GP.
    // This function then computes the incremental strain (subtracting from the committed one)
    // and sets the increment.
    // Returns a success flag from the call to setTrialStrainIncr
    int setTrialStrain( const DTensor2 &v )
    {
        using namespace ClassicElastoplasticityGlobals;
        DTensor2 result( 3, 3, 0.0 );
        TrialStrain(i, j) = v(i, j);
        result( i, j ) = v( i, j ) - CommitStrain( i, j );

        return setTrialStrainIncr( result );
    }

    // Directly sets the trial strain increment and does an explicit or implicit step.
    // Returns a flag depending on the result of the step.
    int setTrialStrainIncr( const DTensor2 &v )
    {
        TrialStrain += v;
        return this->explicitStep(v);
        //
        // or
        //
        // return this->implicitStep(strain_increment);
    }

//==================================================================================================
//  Getters
//==================================================================================================

    const DTensor4 &getTangentTensor( void )
    {
        using namespace ClassicElastoplasticityGlobals;

        double yf_val = yf(TrialStress);

        DTensor4& Eelastic = et(TrialStress);


        if (yf_val <= 0.0)
        {
            Stiffness(i, j, k, l) = Eelastic(i, j, k, l);
        }
        else
        {
            const DTensor2& n = yf.df_dsigma_ij(intersection_stress);
            const DTensor2& m = pf(depsilon_elpl, intersection_stress);

            double xi_star_h_star = yf.xi_star_h_star( depsilon_elpl, depsilon_elpl,  intersection_stress);
            double den = n(p, q) * Eelastic(p, q, r, s) * m(r, s) - xi_star_h_star;

            //Compute tangent stiffness
            Stiffness(i, j, k, l) = Eelastic(i, j, k, l) - (Eelastic(i, j, p, q) * m(p, q)) * (n(r, s) * Eelastic(r, s, k, l) ) / den;
        }

        return Stiffness;
    }

    const DTensor2 &getStressTensor( void )
    {
        return TrialStress;
    }

    const DTensor2 &getStrainTensor( void )
    {
        return TrialStrain;
    }

    const DTensor2 &getPlasticStrainTensor( void )
    {
        return TrialPlastic_Strain;
    }

    const DTensor2  &getCommittedStressTensor(void)
    {
        return CommitStress;
    }

    const DTensor2 &getCommittedStrainTensor(void)
    {
        return CommitStrain;
    }

    const DTensor2 &getCommittedPlasticStrainTensor(void)
    {
        return CommitPlastic_Strain;
    }



//==================================================================================================
//  State commiting and reversion
//==================================================================================================

    // Forwards the trial variables to commited variables and calls commit state on the
    // BET classes
    int commitState(void)
    {
        using namespace ClassicElastoplasticityGlobals;
        int errorcode = 0;
        CommitStress(i, j) = TrialStress(i, j);
        CommitStrain(i, j) = TrialStrain(i, j);
        CommitPlastic_Strain(i, j) = TrialPlastic_Strain(i, j);

        return errorcode;
    }

    //Reverts the commited variables to the trials and calls revert on BET Classes.
    int revertToLastCommit(void)
    {
        using namespace ClassicElastoplasticityGlobals;
        int errorcode = 0;
        TrialStress(i, j) = CommitStress(i, j);
        TrialStrain(i, j) = CommitStrain(i, j);
        TrialPlastic_Strain(i, j) = CommitPlastic_Strain(i, j);

        return errorcode;
    }

    int revertToStart(void)
    {
        return 0;
    }

    NDMaterialLT *getCopy(void)
    {
        ClassicElastoplasticMaterial< ElasticityType,
                                      YieldFunctionType,
                                      PlasticFlowType,
                                      MaterialInternalVariablesType,
                                      thisClassTag,
                                      T > * newmaterial = new T( this->getTag(), rho, this->getPressure(), yf, et, pf, vars);
        newmaterial->vars.setVars(this->vars);
        newmaterial->TrialStrain = (this->TrialStrain);
        newmaterial->TrialStress = (this->TrialStress);
        newmaterial->TrialPlastic_Strain = (this->TrialPlastic_Strain);
        newmaterial->CommitStress = (this->CommitStress);
        newmaterial->CommitStrain = (this->CommitStrain);
        newmaterial->CommitPlastic_Strain = (this->CommitPlastic_Strain);
        // newmaterial->Stiffness = (this->Stiffness);
        return newmaterial;
    }

    // NDMaterialLT *getCopy(const char *code);
    // const char *getType(void) const;

    int sendSelf(int commitTag, Channel &theChannel)
    {
        int pos = 0;
        static Vector data(1 + 9 * 6); // rho and all the DTensors2 get packed into one vector

        // double rho;
        data(pos++) = rho;
        // DTensor2 TrialStrain;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                data(pos++) = TrialStrain(i, j);
            }
        // DTensor2 TrialStress;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                data(pos++) = TrialStress(i, j);
            }
        // DTensor2 TrialPlastic_Strain;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                data(pos++) = TrialPlastic_Strain(i, j);
            }
        // DTensor2 CommitStress;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                data(pos++) = CommitStress(i, j);
            }
        // DTensor2 CommitStrain;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                data(pos++) = CommitStrain(i, j);
            }
        // DTensor2 CommitPlastic_Strain;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                data(pos++) = CommitPlastic_Strain(i, j);
            }

        // cout << "Sending data" << endl;
        if (theChannel.sendVector(0, commitTag, data) != 0)
        {
            cerr << "ClassicElastoplasticMaterial::sendSelf() - Failed sending data" << endl;
            return -1;
        }

        // cout << "Sending elasticity" << endl;
        // ElasticityType    et;
        if (et.sendSelf(commitTag, theChannel) != 0)
        {
            cerr << "ClassicElastoplasticMaterial::sendSelf() - Failed sending elasticity data" << endl;
            return -1;
        }

        // cout << "Sending variables" << endl;
        // MaterialInternalVariablesType vars;
        if (vars.sendSelf(commitTag, theChannel) != 0)
        {
            cerr << "ClassicElastoplasticMaterial::sendSelf() - Failed sending internal variables data" << endl;
            return -1;
        }

        return 0;
    }
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
    {
        // cout << "Receiving data" << endl;
        static Vector data(1 + 9 * 6); // rho and all the DTensors2 get packed into one vector
        if (theChannel.receiveVector(0, commitTag, data) != 0)
        {
            cerr << "ClassicElastoplasticMaterial::receiveSelf() - Failed receiving data" << endl;
            return -1;
        }

        int pos = 0;
        // double rho;
        rho = data(pos++);
        // DTensor2 TrialStrain;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                TrialStrain(i, j) = data(pos++);
            }
        // DTensor2 TrialStress;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                TrialStress(i, j) = data(pos++);
            }
        // DTensor2 TrialPlastic_Strain;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                TrialPlastic_Strain(i, j) = data(pos++);
            }
        // DTensor2 CommitStress;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                CommitStress(i, j) = data(pos++);
            }
        // DTensor2 CommitStrain;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                CommitStrain(i, j) = data(pos++);
            }
        // DTensor2 CommitPlastic_Strain;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                CommitPlastic_Strain(i, j) = data(pos++);
            }

        // cout << "Receiving elasticity" << endl;
        // ElasticityType    et;
        if (et.receiveSelf(commitTag, theChannel, theBroker) != 0)
        {
            cerr << "ClassicElastoplasticMaterial::receiveSelf() - Failed receiving elasticity data" << endl;
            return -1;
        }
        // cout << "Receiving variables" << endl;
        // MaterialInternalVariablesType vars;
        if (vars.receiveSelf(commitTag, theChannel, theBroker) != 0)
        {
            cerr << "ClassicElastoplasticMaterial::receiveSelf() - Failed receiving internal variables data" << endl;
            return -1;
        }

        return 0;
    }
    void Print(ostream &s, int flag = 0)
    {
        //nothin
    }

    int getObjectSize()
    {
        int size = 0;

        // 6 3x3 DTensor2s and 1 DTensor4 (3x3x3x3)
        size += (3 * 3 * 6 + 3 * 3 * 3 * 3) * sizeof(double);

        //Four pointers
        size += 4 * sizeof(YieldFunctionType*);

        //Whatever the base components size is
        size += sizeof(yf);//yf->getObjectSize();
        size += sizeof(et);//et->getObjectSize();
        size += sizeof(pf);//pf->getObjectSize();
        size += sizeof(vars);//vars->getObjectSize();
        size += sizeof(NDMaterialLT);

        // size += static_cast<T*>(this)->getObjectSize();

        return size;
    }

private:


    int explicitStep(const DTensor2 &strain_incr)
    {
        using namespace ClassicElastoplasticityGlobals;
        int errorcode = 0;

        static DTensor2 depsilon(3, 3, 0);
        depsilon *= 0;
        depsilon(i, j) = strain_incr(i, j);
        const DTensor2& sigma = CommitStress;
        const DTensor2& epsilon = CommitStrain;
        // const DTensor2& epsilon_pl = CommitPlastic_Strain;


        dsigma *= 0;//Zero-out the stress increment tensor
        intersection_stress *= 0; //Zero-out these intersection stress and strain tensors
        intersection_strain *= 0;

        DTensor4& Eelastic = et(sigma);

        for (int ii = 0; ii < 3; ii++)
        {
            for (int jj = 0; jj < 3; jj++)
            {
                for (int kk = 0; kk < 3; kk++)
                {
                    for (int ll = 0; ll < 3; ll++)
                    {
                        dsigma(ii, jj) = Eelastic(ii, jj, kk, ll) * depsilon(kk, ll);
                    }
                }
                TrialStress(ii, jj) = sigma(ii, jj) + dsigma(ii, jj);
                TrialStrain(ii, jj) = CommitStrain(ii, jj) + depsilon(ii, jj);
                TrialPlastic_Strain(ii, jj) = CommitPlastic_Strain(ii, jj);
            }
        }

        double yf_val_start = yf(sigma);
        double yf_val_end = yf(TrialStress);

        printTensor("   * depsilon", depsilon);
        printTensor("   * sigma", sigma);
        printTensor4("   * Eelastic", Eelastic);
        printTensor("   * dsigma", dsigma);
        std::cout << "   * yf start = " << yf_val_start << std::endl;
        std::cout << "   * yf end   = " << yf_val_end << std::endl;

        DTensor2& start_stress = CommitStress;
        DTensor2& end_stress = TrialStress;

        intersection_stress(i, j) = start_stress(i, j);

        if ((yf_val_start <= 0.0 && yf_val_end <= 0.0) || yf_val_start > yf_val_end) //Elasticity
        {
            Stiffness(i, j, k, l) = Eelastic(i, j, k, l);
        }
        else  //Plasticity
        {
            depsilon_elpl(i, j) = depsilon(i, j);
            if (yf_val_start < 0)
            {
                double intersection_factor = zbrentstress( start_stress, end_stress, 0.0, 1.0, TOLERANCE1 );

                intersection_stress(i, j) = start_stress(i, j) * (1 - intersection_factor) + end_stress(i, j) * intersection_factor;
                intersection_strain(i, j) = epsilon(i, j)  + depsilon(i, j) * intersection_factor;
                depsilon_elpl(i, j) = (1 - intersection_factor) * depsilon(i, j);
                TrialStress(i, j) = intersection_stress(i, j);

                Eelastic = et(intersection_stress);
                TrialStress(i, j)  += Eelastic(i, j, k, l) * depsilon_elpl(k, l);
            }

            //Compute normal to YF (n) and Plastic Flow direction (m)
            const DTensor2& n = yf.df_dsigma_ij(intersection_stress);
            const DTensor2& m = pf(depsilon_elpl, intersection_stress);

            // printTensor("intersection_stress", intersection_stress);


            double xi_star_h_star = yf.xi_star_h_star( depsilon_elpl, depsilon_elpl,  intersection_stress);
            double den = n(p, q) * Eelastic(p, q, r, s) * m(r, s) - xi_star_h_star;

            //Compute the plastic multiplier
            if (den == 0)
            {
                cerr << "CEP - den = 0\n";
                return -1;
            }
            double dLambda =  n(i, j) * Eelastic(i, j, k, l) * depsilon_elpl(k, l);
            dLambda /= den;

            //Update the trial plastic strain.
            TrialPlastic_Strain(i, j) += dLambda * m(i, j);

            //Correct the trial stress
            TrialStress(i, j) = TrialStress(i, j) - dLambda * Eelastic(i, j, k, l) * m(k, l);


            std::cout << "   * dLambda = " << dLambda << std::endl;
            std::cout << "   * xi_star_h_star = " << xi_star_h_star << std::endl;
            std::cout << "   * den = " << den << std::endl;

            vars.evolve(dLambda, depsilon_elpl, m, intersection_stress);
        }



        return errorcode;
    }



    int implicitStep(const DTensor2 &strain_incr)
    {
        using namespace ClassicElastoplasticityGlobals;
        int errorcode = 0;

        //Magic happens here!

        return errorcode;
    }

private:
// Routine used by yield_surface_cross to find the stresstensor at cross point
//================================================================================
    double zbrentstress(const DTensor2& start_stress,
                        const DTensor2& end_stress,
                        double x1, double x2, double tol) const
    {
        using namespace ClassicElastoplasticityGlobals;
        double EPS = numeric_limits<double>::epsilon();

        int iter;
        double a = x1;
        double b = x2;
        double c = 0.0;
        double d = 0.0;
        double e = 0.0;
        double min1 = 0.0;
        double min2 = 0.0;
        double fc = 0.0;
        double p = 0.0;
        double q = 0.0;
        double r = 0.0;
        double s = 0.0;
        double tol1 = 0.0;
        double xm = 0.0;

        // double fa = func(start_stress, end_stress, *ptr_material_parameter, a);
        // double fb = func(start_stress, end_stress, *ptr_material_parameter, b);

        static DTensor2 sigma_a(3, 3, 0.0);
        static DTensor2 sigma_b(3, 3, 0.0);

        sigma_a(i, j) = start_stress(i, j) * (1 - a)  + end_stress(i, j) * a;
        sigma_b(i, j) = start_stress(i, j) * (1 - b)  + end_stress(i, j) * b;

        double fa = yf(sigma_a);
        double fb = yf(sigma_b);


        if ( (fb * fa) > 0.0)
        {
            std::cout << "\a\n Root must be bracketed in ZBRENTstress " << std::endl;
            exit(1);
        }

        fc = fb;

        for ( iter = 1; iter <= ClassicElastoplasticMaterial_MAXITER_BRENT; iter++ )
        {
            if ( (fb * fc) > 0.0)
            {
                c = a;
                fc = fa;
                e = d = b - a;
            }

            if ( fabs(fc) < fabs(fb) )
            {
                a = b;
                b = c;
                c = a;
                fa = fb;
                fb = fc;
                fc = fa;
            }

            tol1 = 2.0 * EPS * fabs(b) + 0.5 * tol;
            xm = 0.5 * (c - b);

            if ( fabs(xm) <= tol1 || fb == 0.0 )
            {
                return b;
            }

            if ( fabs(e) >= tol1 && fabs(fa) > fabs(fb) )
            {
                s = fb / fa;

                if (a == c)
                {
                    p = 2.0 * xm * s;
                    q = 1.0 - s;
                }
                else
                {
                    q = fa / fc;
                    r = fb / fc;
                    p = s * ( 2.0 * xm * q * (q - r) - (b - a) * (r - 1.0) );
                    q = (q - 1.0) * (r - 1.0) * (s - 1.0);
                }

                if (p > 0.0)
                {
                    q = -q;
                }

                p = fabs(p);
                min1 = 3.0 * xm * q - fabs(tol1 * q);
                min2 = fabs(e * q);

                if (2.0 * p < (min1 < min2 ? min1 : min2))
                {
                    e = d;
                    d = p / q;
                }
                else
                {
                    d = xm;
                    e = d;
                }
            }
            else
            {
                d = xm;
                e = d;
            }

            a = b;
            fa = fb;

            if (fabs(d) > tol1)
            {
                b += d;
            }
            else
            {
                b += (xm > 0.0 ? fabs(tol1) : -fabs(tol1));
            }

            // fb = func(start_stress, end_stress, *ptr_material_parameter, b);
            sigma_b(i, j) = start_stress(i, j) * (1 - b)  + end_stress(i, j) * b;
            fb = yf(sigma_b);
        }

        return 0.0;
    }

//================================================================================
    // double func(const DTensor2& start_stress,
    //             const DTensor2& end_stress,
    //             double alfa ) const
    // {
    //     DTensor2 alfa_stress(3, 3, 0.0);
    //     alfa_stress(i, j) =  start_stress(i, j) * (1.0 - alfa) ) +  end_stress(i, j) * alfa ;

    //     double f = ;

    //     return yf( alfa_stress );
    // }



private:

    double rho;

    DTensor2 TrialStrain;
    DTensor2 TrialStress;
    DTensor2 TrialPlastic_Strain;

    DTensor2 CommitStress;
    DTensor2 CommitStrain;
    DTensor2 CommitPlastic_Strain;


    YieldFunctionType yf;
    ElasticityType    et;
    PlasticFlowType   pf;
    MaterialInternalVariablesType vars;

    static DTensor2 dsigma;
    static DTensor2 depsilon_elpl;    //Elastoplastic strain increment : For a strain increment that causes first yield, the step is divided into an elastic one (until yield) and an elastoplastic one.
    static DTensor2 intersection_stress;
    static DTensor2 intersection_strain;
    static DTensor4 Stiffness;
    // static DTensor2 m;

};

// int ClassicElastoplasticMaterial< class ElasticityType, class YieldFunctionType, , class PlasticFlowType, class HardeningLawType, int thisClassTag >::constitutive_integration_method = 0;

template < class E, class Y, class P, class M, int tag, class T >
DTensor2 ClassicElastoplasticMaterial< E,  Y,  P,  M,  tag,  T >::dsigma(3, 3, 0.0);
template < class E, class Y, class P, class M, int tag, class T >
DTensor2 ClassicElastoplasticMaterial< E,  Y,  P,  M,  tag,  T >::depsilon_elpl(3, 3, 0.0);  //Used to compute the yield surface intersection.
template < class E, class Y, class P, class M, int tag, class T >
DTensor2 ClassicElastoplasticMaterial< E,  Y,  P,  M,  tag,  T >::intersection_stress(3, 3, 0.0);  //Used to compute the yield surface intersection.
template < class E, class Y, class P, class M, int tag, class T >
DTensor2 ClassicElastoplasticMaterial< E,  Y,  P,  M,  tag,  T >::intersection_strain(3, 3, 0.0);  //Used to compute the yield surface intersection.
template < class E, class Y, class P, class M, int tag, class T >
DTensor4 ClassicElastoplasticMaterial< E,  Y,  P,  M,  tag,  T >::Stiffness(3, 3, 3, 3, 0.0);  //Used to compute the yield surface intersection.


#endif
