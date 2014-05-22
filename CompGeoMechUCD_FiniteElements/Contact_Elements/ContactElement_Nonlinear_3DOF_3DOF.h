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
// UPDATE HISTORY:    Some changes made by Babak 11/22/13 to work with parallel
//
///////////////////////////////////////////////////////////////////////////////


// This contact element is the modified and generalized form of the
// Gang Wang's ZeroLengthContact3D element in OpenSees.

#ifndef ContactElement_Nonlinear_3DOF_3DOF_h
#define ContactElement_Nonlinear_3DOF_3DOF_h

// Output is 6 components of strain, 6 components of plastic strain, and 6 of stress per gauss point
#define ContactElement_Nonlinear_3DOF_3DOF_OUTPUT_SIZE 7

#include <Element.h>
#include <Matrix.h>
#include <Vector.h>


class Node;
class Channel;
// class Response;


class ContactElement_Nonlinear_3DOF_3DOF: public Element

{

    public:

        // Constructor
        ContactElement_Nonlinear_3DOF_3DOF(int tag,
                                           int Nd1, int Nd2,
                                           double Kn_factor, double Kt, double fRatio,
                                           double maximum_gap,
                                           double x_local_1, double x_local_2, double x_local_3);

        // Constructor for the element with lock option
        ContactElement_Nonlinear_3DOF_3DOF(int tag,
                                           int Nd1, int Nd2,
                                           double Kn_factor, double Kt, double fRatio,
                                           double maximum_gap,
                                           double x_local_1, double x_local_2, double x_local_3
                                           , int lock_flag = 0);

        // Null constructor
        ContactElement_Nonlinear_3DOF_3DOF();
        ~ContactElement_Nonlinear_3DOF_3DOF();

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
        int update(void);

        void zeroLoad(void);
        int addLoad(ElementalLoad *theLoad, double loadFactor);
        int addInertiaLoadToUnbalance(const Vector &accel);


        // public methods to obtain stiffness, mass, damping and residual information
        const Matrix &getTangentStiff(void);
        const Matrix &getInitialStiff(void);
        const Matrix &getDamp(void);
        const Matrix &getMass(void);
        const Vector &getResistingForce(void);
        const Vector &getLocalResistingForce(void);
        const Vector &getResistingForceIncInertia(void);


        // public methods for element output
        int sendSelf(int commitTag, Channel &theChannel);
        int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
        void Print(ostream &s, int flag = 0);

        // Response* setResponse(const char** argv, int argc, Information& eleInformation);
        // int getResponse(int responseID, Information& eleInformation);

        // defining the contact plane, node connectivity, sticking or sliding
        int ContactPlane(const Vector &);
        int if_nodes_are_in_contact_or_not(void);
        int stick_or_slide(void);

        int CheckMesh(ofstream &);

        std::string getElementName() const
        {
            return "ContactElement_Nonlinear_3DOF_3DOF";
        }

        //Jose Added for output
        int getOutputSize() const;
        const Vector &getOutput() const;

        double getNormalTangentStiffness(u)
        {
            double tmp = Gap_max - u;
            return u / (tmp * tmp) + 1 / (-u + Gap_max);
        }
    protected:

    private:
        ID  connectedExternalNodes;     // contains the tags of the end nodes
        Node *nodePointers[2];          // node pointer
        int print_option;
        int numDOF;                     // number of dof of the contact element

        // parameters
        double Kn_factor;               // normal factor for penalty
        double Gap_max;                 // "fictitious" thickness of the contact element
        double fs;                      // friction ratio
        double Kn;                      // Kn penalty
        double Kt;                      // Kt penalty
        int is_locked;                  // Bool, determines if element is locked Added by Babak 04/12/14


        // contact forces
        double normalforce_n;
        double normalforce_np1;
        double normalforce_increment_np1;
        Vector shearforce_n;
        Vector shearforce_np1;
        Vector shearforce_trial_np1;
        Vector shearforce_increment_np1;
        Vector n_trial_np1;

        // relative displacement (total and plastic)
        double total_normal_relative_displ_n;
        double total_normal_relative_displ_np1;
        double incr_normal_relative_displ_np1;
        Vector global_gap_np1;
        Vector local_gap_np1;
        Vector total_shear_relative_displ_n;
        Vector total_shear_relative_displ_np1;
        Vector incr_shear_relative_displ_np1;
        Vector plastic_shear_relative_displ_n;
        Vector plastic_shear_relative_displ_np1;
        Vector incr_plastic_shear_relative_displ_np1;

        double yield_criteria;


        // Normal and Tangental Vectors for Elemental Nodes, (6*1)
        Vector N;
        Vector T1;
        Vector T2;
        int ContactFlag;    // 0: not in contact; 1: in contact
        int SlidingFlag;    //  0: Stick; 1: Slide
        double shear_force_trial_np1_norm;
        double shearforce_np1_norm;

        Vector resid;   // force residual vector
        Vector outputVector;

        // Static data (shared across all contact elements)
        static Matrix stiff;   // stiffness matrix
        static Vector localresid;   // local force residual vector
        static Matrix zeroMatrix;
        static Vector x_local; //Added by Babak 11/22/13
        static Vector y_local;
        static Vector z_local;
        static Vector x_local_normalized;
        static Vector y_local_normalized;
        static Vector z_local_normalized;
        static Matrix transformation;  // transformation matrix for orientation
        static const int  numberNodes  ;

};

#endif











