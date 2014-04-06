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
//
// PURPOSE:           Penalty Element
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              May 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#ifndef PenaltyElementApplyDisplacement_h
#define PenaltyElementApplyDisplacement_h


#include <Element.h>
#include <Matrix.h>
#include <Vector.h>

class Node;
class Channel;
class UniaxialMaterial;

class PenaltyElementApplyDisplacement : public Element
{
    public:
        PenaltyElementApplyDisplacement(int tag,
                                        int Existed_Node,
                                        int Element_Direction_Dof,
                                        double Penalty_Stiffness);

        PenaltyElementApplyDisplacement();
        ~PenaltyElementApplyDisplacement();

        // public methods to obtain inforrmation about dof & connectivity
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

        // public methods to obtain stiffness, mass, damping and residual information
        const Matrix &getKi(void);
        const Matrix &getTangentStiff(void);
        const Matrix &getInitialStiff(void);
        const Matrix &getDamp(void);
        const Matrix &getMass(void);

        double getPenaltyStiffness(void);


        void zeroLoad(void);
        int addLoad(ElementalLoad *theLoad, double loadFactor);
        int addInertiaLoadToUnbalance(const Vector &accel);

        const Vector &getResistingForce(void);
        const Vector &getResistingForceIncInertia(void);

        // public methods for element output
        int sendSelf(int commitTag, Channel &theChannel);
        int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
        void Print(ostream &s, int flag = 0);

        // Response* setResponse(const char** argv, int argc, Information& eleInfo);
        // int getResponse(int responseID, Information& eleInformation);

        //    Matrix returnMass(void);

    protected:

    private:


        // private attributes - a copy for each object of the class
        ID  connectedExternalNodes;     // contains the tags of the end nodes
        int numDOF;                     // number of dof for PenaltyElementApplyDisplacement

        Vector *theLoad;     // pointer to the load vector P
        Matrix *theMatrix; // pointer to objects matrix (a class wide Matrix)
        Vector *theVector; // pointer to objects vector (a class wide Vector)
        Matrix *tempMatrix; // pointer to objects matrix (a class wide Matrix)
        Vector *tempVector; // pointer to objects vector (a class wide Vector)


        int direction;
        Node *theNodes[1];
        double penaltystiffness;



        // static data - single copy for all objects of the class

        static Matrix PenaltyElementApplyDisplacementM3;   // class wide matrix for 6*6
        static Matrix PenaltyElementApplyDisplacementM6;  // class wide matrix for 12*12
        static Vector PenaltyElementApplyDisplacementV3;   // class wide Vector for size 6
        static Vector PenaltyElementApplyDisplacementV6;  // class wide Vector for size 12
};

#endif




