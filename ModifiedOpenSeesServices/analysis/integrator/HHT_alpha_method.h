// Based on HHT_alpha_method implementation from Frank McKenna
// Nima Tafazzoli and Boris Jeremic, CompGeoMech UCD, May 2012

#ifndef HHT_alpha_method_h
#define HHT_alpha_method_h



#include <TransientIntegrator.h>

class DOF_Group;
class FE_Element;
class Vector;

class HHT_alpha_method : public TransientIntegrator
{
    public:
        HHT_alpha_method();
        HHT_alpha_method(double alpha);
        HHT_alpha_method(double alpha, double beta, double gamma);

        ~HHT_alpha_method();

        // methods which define what the FE_Element and DOF_Groups add
        // to the system of equation object.
        int formEleTangent(FE_Element* theEle);
        int formNodTangent(DOF_Group* theDof);

        int domainChanged(void);
        int initialize(void);
        int newStep(double deltaT);
        int update(const Vector& deltaU);

        int commit(void);

        virtual int sendSelf(int commitTag, Channel& theChannel);
        virtual int receiveSelf(int commitTag, Channel& theChannel,
                             FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

    protected:

    private:
        double alpha;
        double gamma;
        double beta;

        double alphaM, betaK, betaKi, betaKc;

        double c1, c2, c3, c4;  // some constants we need to keep
        double a1, a2, a3, a4;  // some constants we need to keep
        Vector* Ut, *Utdot, *Utdotdot; // response quantities at time t
        Vector* U, *Udot, *Udotdot; // response quantities at time t + delta t
        Vector* Ualpha, *Udotalpha; // response quantities at time t+alpha delta t
};

#endif

