/* ****************************************************************** **
**                                                                    **
**                                                                    **
** ****************************************************************** */


#ifndef NDMaterialLT_h
#define NDMaterialLT_h


#include <Material.h>
#include "../../ltensor/LTensor.h"

class ID;
class Information;
// class Response;
class Channel;
class HDF5_Channel;

/*

Note on LTensor library usage:

DTensor1 is a Rank-1 tensor of doubles (D)
DTensor2 is a Rank-2 tensor of doubles (D)
DTensor3 is a Rank-3 tensor of doubles (D)
DTensor4 is a Rank-4 tensor of doubles (D)

There are equivalents for int tensors (I) and float tensors (F).

Just replace D for I or F.

*/
enum struct NDMaterialLT_Constitutive_Integration_Method : int
{
    Not_Set,
    Forward_Euler,
    Forward_Euler_Crisfield,
    Multistep_Forward_Euler,
    Multistep_Forward_Euler_Crisfield,
    Modified_Euler_Error_Control,
    Runge_Kutta_45_Error_Control,
    Backward_Euler
};

class NDMaterialLT : public Material
{
public:
    NDMaterialLT(int tag, int classTag);
    NDMaterialLT();
    virtual ~NDMaterialLT();

    virtual double getRho(void);
    virtual double getE(void);
    virtual double getnu(void);
    virtual double getpsi(void);


    // methods to set and retrieve state using the Tensor class

    virtual int setTrialStrain(const DTensor1 &v);
    virtual int setTrialStrain(const DTensor1 &v, const DTensor1 &r);
    virtual int setTrialStrainIncr(const DTensor1 &v);
    virtual int setTrialStrainIncr(const DTensor1 &v, const DTensor1 &r);
    virtual const DTensor2 &getTangent(void);// = 0;
    virtual const DTensor2 &getInitialTangent(void)
    {
        return this->getTangent();
    };

    virtual const DTensor1 &getStress(void);
    virtual const DTensor1 &getStrain(void);
    virtual const DTensor1 &getCommittedStress(void);
    virtual const DTensor1 &getCommittedStrain(void);

    virtual int setTrialStrain(const DTensor2 &v);
    virtual int setTrialStrain(const DTensor2 &v, const DTensor2 &r);
    virtual int setTrialStrainIncr(const DTensor2 &v);
    virtual int setTrialStrainIncr(const DTensor2 &v, const DTensor2 &r);

    virtual const DTensor4 &getTangentTensor(void) = 0;
    virtual const DTensor2 &getStressTensor(void);
    virtual const DTensor2 &getStrainTensor(void);
    virtual const DTensor2 &getPlasticStrainTensor(void);

    virtual int commitState(void);
    virtual int revertToLastCommit(void);
    virtual int revertToStart(void);

    virtual NDMaterialLT *getCopy(void);
    //virtual NDMaterialLT *getCopy(const char *code);

    virtual const char *getType(void) const;
    virtual int getOrder(void) const
    {
        return 1;
    };


    virtual int sendSelf(int commitTag, Channel &theChannel);
    virtual int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    // virtual Response *setResponse (const char **argv, int argc, Information &matInformation);
    // virtual int getResponse (int responseID, Information &matInformation);
    virtual int getObjectSize()
    {
        return sizeof(*this);
    };
    virtual int CheckMesh(ofstream &);
    virtual void setVolume(double vol);

    virtual void Print(ostream &s, int flag = 0);

    virtual int getOutputSize()
    {
        return 0; // Can be overloaded for material point output
    }

    const Vector & getOutput()
    {
        return errVectorVector;
    }

    virtual void zeroStrain();

    static bool set_constitutive_integration_method(int method, double f_relative_tol, double stress_relative_tol, int n_max_iterations);


protected:
    static NDMaterialLT_Constitutive_Integration_Method constitutive_integration_method;     //
    static double f_relative_tol;
    static double stress_relative_tol;
    static int n_max_iterations;

private:
    static DTensor2 errMatrix;
    static DTensor1 errVector;
    static DTensor2 errTensor;
    static DTensor4 errTensor4;
    static DTensor2 errTensor2;
    static DTensor2 errstresstensor;
    static DTensor2 errstraintensor;
    static Vector errVectorVector;
    double volume;

};


#endif
