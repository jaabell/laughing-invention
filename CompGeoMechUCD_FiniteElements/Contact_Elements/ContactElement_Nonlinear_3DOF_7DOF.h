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
// PROGRAMMER:        Nima Tafazzoli, Mario Martinelli, Boris Jeremic
// DATE:
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#ifndef ContactElement_Nonlinear_3DOF_7DOF_h
#define ContactElement_Nonlinear_3DOF_7DOF_h

#include <Element.h>
#include <Matrix.h>
#include <Vector.h>


class Node;
class Channel;
// class Response;


class ContactElement_Nonlinear_3DOF_7DOF: public Element

{

    public:

        // Constructor
        ContactElement_Nonlinear_3DOF_7DOF(int tag,
                                           int Nd1, int Nd2,
                                           double Kn_factor, double fRatio, double maximum_gap,
                                           double x_local_1, double x_local_2, double x_local_3);


        // Null constructor
        ContactElement_Nonlinear_3DOF_7DOF();
        ~ContactElement_Nonlinear_3DOF_7DOF();

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


        // Nima Tafazzoli added for returning pore pressure for generating forces
        double returnPressure(void);


    private:

        ID  connectedExternalNodes;   // contains the tags of the end nodes
        static const int  numberNodes  ;
        Node *nodePointers[2];   // node pointer


        // parameters
        double Kn_factor;     // normal factor for penalty
        double Gap_max;       // "fictitious" thickness of the contact element
        //double Kt_factor;   // tangential factor for penalty (Kt = Kt_factor*Kn)
        double fs;          // friction ratio
        double Kn;            // Kn penalty
        double Kn_max;            // max value of Kn penalty
        double Kt;            // Kt penalty

        static Vector y_local;
        static Vector z_local;

        // creating the normalized local vectors
        static Vector x_local_normalized;
        static Vector y_local_normalized;
        static Vector z_local_normalized;
        static Matrix transformation;  // transformation matrix for orientation


        // Normal and Tangental Vectors for Elemental Nodes, (6*1)
        Vector N;
        Vector T1;
        Vector T2;

        Vector global_gap_np1;
        Vector local_gap_np1;

        // contact forces
        double normalforce_n;
        double normalforce_np1;
        double normalforce_increment_np1;
        Vector shearforce_n;
        Vector shearforce_np1;
        Vector shearforce_trial_np1;
        Vector shearforce_increment_np1;



        double total_normal_relative_displ_n;
        double total_normal_relative_displ_np1;
        double incr_normal_relative_displ_np1;
        Vector total_shear_relative_displ_n;
        Vector total_shear_relative_displ_np1;
        Vector incr_shear_relative_displ_np1;
        Vector plastic_shear_relative_displ_n;
        Vector plastic_shear_relative_displ_np1;
        Vector incr_plastic_shear_relative_displ_np1;

        // yield_criteria
        double yield_criteria;


        Vector n_trial_np1;


        // print option
        int print_option;
        int jjj;
        int numb_contact_element;



        int ContactFlag;    // 0: not in contact; 1: in contact
        int SlidingFlag;    //  0: Stick; 1: Slide
        int numDOF;         // number of dof of the contact element
        int definecontact;
        double shear_force_trial_np1_norm;
        double shearforce_np1_norm;

        static Matrix stiff;   // stiffness matrix
        static Vector resid;   // force residual vector
        static Vector localresid;   // local force residual vector
        static Matrix zeroMatrix;
};

#endif











