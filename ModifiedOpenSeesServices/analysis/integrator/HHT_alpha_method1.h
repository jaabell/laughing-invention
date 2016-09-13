// Based on HHT implementation from Frank McKenna and Andreas Schellenberg
// Boris Jeremic, CompGeoMech UCD, 30April2009

#ifndef HHT_alpha_method_h
#define HHT_alpha_method_h

#include <TransientIntegrator.h>

class DOF_Group;
class FE_Element;
class Vector;

class HHT_alpha_method : public TransientIntegrator
{
    public:
        // constructors
        HHT_alpha_method();

        HHT_alpha_method(double alpha,
                         double beta,
                         double gamma);

        //     HHT_alpha_method(double alpha,
        //                      double beta,
        //                      double gamma,
        //                      double alphaM,
        //                      double betaK,
        //                      double betaKi,
        //                      double betaKc);

        // destructor
        ~HHT_alpha_method();

        // methods which define what the FE_Element and DOF_Groups add
        // to the system of equation object.
        int formEleTangent(FE_Element* theEle);
        int formNodTangent(DOF_Group* theDof);

        int domainChanged(void);
        int newStep(double deltaT);
        int revertToLastStep(void);
        int update(const Vector& deltaU);
        int commit(void);

        virtual int sendSelf(int commitTag, Channel& theChannel);
        virtual int receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

    protected:

    private:
        double alpha;
        double beta;
        double gamma;
        double deltaT;

        //     rayleigh damping factors
        //     double alphaM;
        //     double betaK;
        //     double betaKi;
        //     double betaKc;

        double c1, c2, c3;              // some constants we need to keep
        Vector* Ut, *Utdot, *Utdotdot;  // response quantities at time t
        Vector* U, *Udot, *Udotdot;     // response quantities at time t + deltaT
        Vector* Ualpha, *Ualphadot;     // response quantities at time t+alpha*deltaT
};

#endif


