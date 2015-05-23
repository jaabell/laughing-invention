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
// PROGRAMMER:      Jose Abell, Boris Jeremic
// DATE:            Thu 21 May 2015 10:00:08 PM PDT
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


// Style guide. ---
// * Use CamelCase for member functions, constructors, destructors and class names have intial upper case
//   letter and other functions have lower-case initial letter.
//      Exception:  when variables are named consistently with theory (e.g. K for striffness matrix, etc.)
// * Use underscores for variable names (e.g yield_function_value)
// * i,j,k,l,m,n,p,q are reserved for indexes into arrays (iterating over components)
//     -Note1: the type of these can be integer or other fancy types, ie. when using LTensor class
//             to store tensors.
// * Use "Allman" code styling. Use a code formatter like AStyle (http://astyle.sourceforge.net/)
//   there are many plugins which interface this into editors (SublimeAStyleFormatter for sublime text editor)
// * Use the most recent C++ standard supported by your compiler. Code should compile without
//   warnings using the -Wall flag or equivalent. In case this cannot be done, comment on why.
// * Delay allocating resources as much as possible (at least until setDomain function gets called).
// * Clean up resources as soon as possible.
// * Think parallel. Have to implement sendSelf() and receiveSelf() functions.
//
// 2015 Jose Abell - UCD CompGeoMech


// (!) More instructions and details on the implementation file FrictionalPenaltyContact.cpp

// Rename the header guard
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
    FrictionalPenaltyContact(int tag, int node1, int node2, double kn,  double kt, double mu, double e1_x_, double e1_y_, double e1_z_);

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
    double mu;     // Coeficient of friction
    bool is_in_contact;
    bool is_in_contact_prev;
    Matrix B;      // Global to local (incremental gap) matrix
    Vector *d_ij0; // Distance from node i to j at last contact. If the pointer is NULL, it means there was no contact in previous step!
    Vector *d_ij0_prev; // Distance from node i to j at last contact. If the pointer is NULL, it means there was no contact in previous step!
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











