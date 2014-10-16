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
// PURPOSE:           Contact Element
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              November 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


// This contact element is the modified and generalized form of the
// Gang Wang's ZeroLengthContact3D element in OpenSees.

#ifndef ContactElement_6DOF_6DOF_h
#define ContactElement_6DOF_6DOF_h

#include <Element.h>
#include <Matrix.h>
#include <Vector.h>


class Node;
class Channel;
// class Response;


class ContactElement_6DOF_6DOF: public Element

{

    public:

        // Constructor
        ContactElement_6DOF_6DOF(int tag,
                                 int Nd1, int Nd2,
                                 double Kn, double Kt, double fRatio, double c,
                                 double x_local_1, double x_local_2, double x_local_3);


        // Null constructor
        ContactElement_6DOF_6DOF();
        ~ContactElement_6DOF_6DOF();

        // public methods to obtain information about dof & connectivity
        int getNumExternalNodes(void) const;
        const ID &getExternalNodes(void);
        Node **getNodePtrs(void);
        int getNumDOF(void);
        void setDomain(Domain *theDomain);


        // public methods to set the state of the element
        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);
        void zeroLoad(void);
        int addLoad(ElementalLoad *theLoad, double loadFactor);
        int addInertiaLoadToUnbalance(const Vector &accel);


        // public methods to obtain stiffness, mass, damping and residual information
        const Matrix &getTangentStiff(void);
        const Matrix &getInitialStiff(void);
        const Matrix &getDamp(void);
        const Matrix &getMass(void);
        const Vector &getResistingForce(void);
        const Vector &getResistingForceIncInertia(void);


        // public methods for element output
        int sendSelf(int commitTag, Channel &theChannel);
        int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
        void Print(ostream &s, int flag = 0);

        // Response* setResponse(const char** argv, int argc, Information& eleInformation);
        // int getResponse(int responseID, Information& eleInformation);

        // defining the contact plane, node connectivity, sticking or sliding
        int ContactPlane(const Vector &);
        int if_nodes_are_in_contact_or_not(void);
        int stick_or_slide(void);

        // Nima Tafazzoli added to return the Mass matrix (Feb. 2010)
        //   Matrix returnMass(void);
        std::string getElementName() const
        {
            return "ContactElement_6DOF_6DOF";
        };

    protected:

    private:

        ID  connectedExternalNodes;   // contains the tags of the end nodes
        static const int  numberNodes  ;
        Node *nodePointers[2];   // node pointer


        // parameters
        double Kn;          // normal penalty
        double Kt;          // tangential penalty
        double fs;          // friction ratio
        double cohesion;    // cohension

        static Vector y_local;
        static Vector z_local;

        // creating the normalized local vectors
        static Vector x_local_normalized;
        static Vector y_local_normalized;
        static Vector z_local_normalized;
        static Matrix transformation;  // transformation matrix for orientation


        // contact forces
        static double normalforce;
        static Vector shearforce;
        double yield_criteria;
        double shear_slide_1;  // friction in local dir1
        double shear_slide_2;  // friction in local dir2

        static Vector global_gap;
        static Vector local_gap;
        static Vector shear_relative_displacement;

        // Normal and Tangental Vectors for Elemental Nodes, (6*1)
        Vector N;
        Vector T1;
        Vector T2;

        int ContactFlag;    // 0: not in contact; 1: in contact
        int SlidingFlag;    //  0: Stick; 1: Slide
        int numDOF;         // number of dof of the contact element
        int definecontact;
        double shear_force_norm;

        static Matrix stiff;   // stiffness matrix
        static Vector resid;   // force residual vector
        static Matrix zeroMatrix;
};

#endif











