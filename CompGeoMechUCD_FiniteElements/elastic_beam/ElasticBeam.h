///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               December 2012
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




#ifndef ElasticBeam_h
#define ElasticBeam_h

#include <Element.h>
#include <Node.h>
#include <Matrix.h>
#include <Vector.h>

class Channel;
class Information;
class Response;
class SectionForceDeformation;

class ElasticBeam : public Element
{
public:
    ElasticBeam();
    ElasticBeam(int tag, double A, double E, double G,
                double Jx, double Iy, double Iz, int Nd1, int Nd2,
                double rho, int sectionTag,
                double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z);

    ElasticBeam(int tag, int Nd1, int Nd2, SectionForceDeformation *section,
                double rho,
                double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z);

    ~ElasticBeam();

    const char *getClassType(void) const
    {
        return "ElasticBeam";
    };

    int getNumExternalNodes(void) const;
    const ID &getExternalNodes(void);
    Node **getNodePtrs(void);

    int getNumDOF(void);
    void setDomain(Domain *theDomain);

    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);

    int update(void);
    const Matrix &getTangentStiff(void);
    const Matrix &getInitialStiff(void);
    const Matrix &getMass(void);

    void zeroLoad(void);
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);

    const Vector &getResistingForce(void);
    const Vector &getResistingForceIncInertia(void);

    /********************************************************************************************************************
    * Sumeet August, 2016
    * This element has no gauss points and thus no outputs at gauss points 
    * so no needto have the "getGaussOutput()" function
    *********************************************************************************************************************/
    const vector<float> &getElementOutput() ;


    // int describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel);
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    void Print(ostream &s, int flag = 0);

    Response *setResponse (const char **argv, int argc, Information &info);
    int getResponse (int responseID, Information &info);

    int getLocalAxes(Vector &XAxis, Vector &YAxis, Vector &ZAxis);
    int computeElemtLengthAndOrient();
    int initialize();
    const Matrix &getGlobalStiffnessMatrix (const Matrix &KB);
    const Matrix &getGlobalConsistentMassMatrix (const Matrix &KB);
    Vector *getForce(void);

    std::string getElementName() const
    {
        return "ElasticBeam";
    }


private:

    void Form_Element_Output_Vector();

    double A, E, G, Jx, Iy, Iz;
    double rho;
    double L;       // undeformed element length

    Vector *nodeIOffset; // rigid joint offsets
    Vector *nodeJOffset; // rigid joint offsets

    Matrix Mass;
    Matrix Stiffness;
    int builtK;
    int builtM;

    Matrix R;       // Transformation matrix
    Vector P;
    Vector Q;
    // Vector LocalDisplacements;
    // Vector LocalEndForces;
    static vector<float> Element_Output_Vector;

    ID  connectedExternalNodes;

    Vector *nodeIInitialDisp;
    Vector *nodeJInitialDisp;
    bool initialDispChecked;

    Node *theNodes[2];
};

#endif


