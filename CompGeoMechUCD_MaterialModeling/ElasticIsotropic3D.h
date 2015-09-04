///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Zhaohui Yang, Boris Jeremic
// DATE:               10Oct2000
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////


#ifndef ElasticIsotropic3D_h
#define ElasticIsotropic3D_h

#include <ElasticIsotropicMaterial.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>

#include <straint.h>
#include <stresst.h>
#include <Tensor.h>

#include <Channel.h>

class ElasticIsotropic3D : public ElasticIsotropicMaterial
{
public:
    ElasticIsotropic3D (int tag, double E, double nu, double rho);
    ElasticIsotropic3D ();
    ~ElasticIsotropic3D ();

    int setTrialStrain (const Vector& v);
    int setTrialStrain (const Vector& v, const Vector& r);
    int setTrialStrainIncr (const Vector& v);
    int setTrialStrainIncr (const Vector& v, const Vector& r);
    const Matrix& getTangent (void);
    const Matrix& getInitialTangent (void);

    const Vector& getStress (void);
    const Vector& getStrain (void);

    int setTrialStrain (const Tensor& v);
    int setTrialStrain (const Tensor& v, const Tensor& r);
    int setTrialStrainIncr (const Tensor& v);
    int setTrialStrainIncr (const Tensor& v, const Tensor& r);
    const Tensor& getTangentTensor (void);
    const stresstensor& getStressTensor (void);
    const straintensor& getStrainTensor (void);
    const straintensor& getPlasticStrainTensor (void);

    int commitState (void);
    int revertToLastCommit (void);
    int revertToStart (void);

    NDMaterial* getCopy (void);
    const char* getType (void) const;
    int getOrder (void) const;

    void Print(ostream& s, int flag = 0);
    void setInitElasticStiffness(void);


    //Guanzhou added for PDD
    virtual int sendSelf(int commitTag, Channel& theChannel);
    virtual int receiveSelf(int commitTag, Channel& theChannel,
                            FEM_ObjectBroker& theBroker);
    int getObjectSize();


protected:

private:
    static Vector sigma;        // Stress vector
    static Matrix D;            // Elastic constantsVector sigma;
    Vector epsilon;     // Strain vector

    stresstensor Stress;    // Stress tensor
    Tensor Dt;          // Elastic constants tensor
    straintensor Strain;    // Strain tensor
};


#endif


