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

#ifndef FrictionalPenaltyContact_h
#define FrictionalPenaltyContact_h


#include <Element.h>
#include <Matrix.h>
#include <Vector.h>
#include <Domain.h>
#include <Node.h>


class Node;
class Channel;


class FrictionalPenaltyContact: public Element
{

public:

    // Constructor
    FrictionalPenaltyContact(int tag, int node1, int node2, double kn,  double kt,  double cn_,  double ct_, double mu, double e1_x_, double e1_y_, double e1_z_);

    // Empty constructor
    FrictionalPenaltyContact();

    //Destructor
    ~FrictionalPenaltyContact();

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
        return "FrictionalPenaltyContact";
    }

    // Output interface functions
    int getOutputSize() const;
    const Vector &getOutput() const;
    Matrix &getGaussCoordinates(void);



protected:
    //Implementation-specific member functions...
    void computeGap();
    void initialize();



private:
    double kn;     // Normal penalty stiffness
    double kt;     // Tangential penalty stiffness
    double cn;
    double ct;
    double mu;     // Coeficient of friction
    bool is_in_contact;
    bool is_in_contact_prev;
    Matrix B;      // Global to local (incremental gap) matrix
    // Vector *d_ij0; // Distance from node i to j at last contact. If the pointer is NULL, it means there was no contact in previous step!
    // Vector *d_ij0_prev; // Distance from node i to j at last contact. If the pointer is NULL, it means there was no contact in previous step!
    Vector *tA;     // Current commitred local forces  t = [t_T1, t_T2, t_N]
    Vector *tC;     // Current trial local forces  t = [t_T1, t_T2, t_N]
    Vector *R;     // Current resisting forces
    Vector *g;     // Current gap
    Vector *g_prev;// Previous gap
    Matrix *C;     // Current local stiffness

    ID external_nodes;
    Node* nodes[2];

};

#endif











