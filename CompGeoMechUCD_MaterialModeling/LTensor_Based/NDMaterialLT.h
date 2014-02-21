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
class Response;

/*

Note on LTensor library usage:

DTensor1 is a Rank-1 tensor of doubles (D)
DTensor2 is a Rank-2 tensor of doubles (D)
DTensor3 is a Rank-3 tensor of doubles (D)
DTensor4 is a Rank-4 tensor of doubles (D)

There are equivalents for int tensors (I) and float tensors (F).

Just replace D for I or F.

*/

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
        virtual const DTensor2 &getTangent(void);
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
        virtual const DTensor4 &getTangentTensor(void);
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

        int sendSelf(int commitTag, Channel &theChannel);
        int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

        virtual Response *setResponse (const char **argv, int argc, Information &matInformation);
        virtual int getResponse (int responseID, Information &matInformation);
        virtual int getObjectSize()
        {
            return 1;
        };
        virtual int CheckMesh(ofstream &);
        virtual void setVolume(double vol);

        virtual void Print(ostream &s, int flag = 0);


    protected:

    private:
        static DTensor2 errMatrix;
        static DTensor1 errVector;
        static DTensor2 errTensor;
        static DTensor4 errTensor4;
        static DTensor2 errTensor2;
        static DTensor2 errstresstensor;
        static DTensor2 errstraintensor;
        double volume;
};


#endif
