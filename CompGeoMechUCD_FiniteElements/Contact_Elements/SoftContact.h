///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell, Boris Jeremic
// DATE:               Thu 21 May 2015 10:00:12 PM PDT
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

#ifndef SoftContact_h
#define SoftContact_h


#include <Element.h>
#include <Matrix.h>
#include <Vector.h>
#include <Domain.h>
#include <Node.h>

#include <ID.h>
#include <Channel.h>

class Node;

class SoftContact: public Element
{

public:

    // Constructor
    SoftContact(int tag,  int node1, int node2, double kn, double sr, double kt,  double cn_,  double ct_, double mu, double e1_x_, double e1_y_, double e1_z_);

    // Empty constructor
    SoftContact();

    //Destructor
    ~SoftContact();

    // Functions to obtain information about dof & connectivity
    int getNumExternalNodes(void) const;
    const ID &getExternalNodes(void);
    Node **getNodePtrs(void);
    int getNumDOF(void);
    void setDomain(Domain *theDomain);


    // Functions to set the state of the element
    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);
    int update(void);
    void zeroLoad(void);
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);


    // Functions to obtain stiffness, mass, damping and residual information
    const Matrix &getTangentStiff(void);
    const Matrix &getInitialStiff(void);
    const Matrix &getDamp(void);
    const Matrix &getMass(void);
    const Vector &getResistingForce(void);
    const Vector &getResistingForceIncInertia(void);


    // Implement for parallel processing
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    // Print out element info
    void Print(ostream &s, int flag = 0);

    // Check element correctness
    int CheckMesh(ofstream &);

    // Give the element a name
    std::string getElementName() const
    {
        return "SoftContact";
    }

    /********************************************************************************************************************
    * Sumeet August, 2016
    * This element has no gauss points and thus no outputs at gauss points 
    * so no needto have the "getGaussOutput()" function
    *********************************************************************************************************************/
    const vector<float> &getElementOutput() ;



protected:
    //Implementation-specific member functions...
    void computeGap();
    void initialize();



private:

    void cross_product(double vect1[],double vect2[],double vect3[]); // Finds cross product -> vect3[] = vect1[] X vect2[] 
    void norm(double vect[],double* norm); // Finds the norm of the vector ->  norm = sqrt(vect[0]*vect[0]+vect[1]*vect[1]+vect[2]*vect[2])

    double kn;     // Intial contact normal stiffness 
    double kn_m;   // contact_normal_stiffness
    double sr;     // stiffneing rate
    double kt;     // Tangential penalty stiffness
    double cn;     // Normal penalty damping stiffness
    double ct;     // Tangential penalty daping stiffness 
    double mu;     // Coeficient of friction
    Matrix B;      // Global to local (incremental gap) matrix
    ID external_nodes;
    Node* nodes[2];

    // current state variables
    bool is_in_contact;      // whether slave is in contact with master
    bool is_in_contact_prev; // whetehr slave was in contact with master
    Vector *tC;     // Current trial local forces  t = [t_T1, t_T2, t_N]
    Vector *tC_pred;// Current trial local forces  t = [t_T1, t_T2, t_N] outside the yield surface 
    Vector *R;      // Current resisting forces
    Vector *g;      // Current gap
    Vector *g_prev; // Previous gap
    Matrix *C;      // Current local stiffness

    // commit state variables
    bool is_in_contact_commit;      // whether slave is in contact with master
    Vector *tA;                     // Current committed local forces  t = [t_T1, t_T2, t_N]
    Vector *g_commit;               // Current gap
    Vector *g_elastic;              // Elastic Displacement
    Vector *tC_pred_commit;         // Commited predicter

    static vector<float> Element_Output_Vector;

};

#endif











