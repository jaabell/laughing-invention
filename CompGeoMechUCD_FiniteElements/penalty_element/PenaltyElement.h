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
// DATE:              May 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#ifndef PenaltyElement_h
#define PenaltyElement_h


#include <Element.h>
#include <Matrix.h>
#include <Vector.h>
#include <Information.h>

#include <Domain.h>
#include <Node.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <UniaxialMaterial.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

// #include <ElementResponse.h>


class Node;
class Channel;
class UniaxialMaterial;

class PenaltyElement : public Element
{
    public:
        PenaltyElement(int tag, int node1, int node2, double Penalty_Stiffness, int dof1);


        PenaltyElement();
        ~PenaltyElement();

        // public methods to obtain inforrmation about dof & connectivity
        int getNumExternalNodes(void) const;
        const ID &getExternalNodes(void);
        Node **getNodePtrs(void);

        int getNumDOF(void);
        void setDomain(Domain *theDomain);

        // public methods to set the state of the element
        int commitState(void);
        int revertToLastCommit(void)
        {
            return 0;
        };
        int revertToStart(void)
        {
            return 0;
        };
        int update(void)
        {
            return 0;
        };

        const Matrix &getTangentStiff(void);
        const Matrix &getInitialStiff(void);
        const Matrix &getMass(void);

        double getPenaltyStiffness(void);


        void zeroLoad(void);
        int addLoad(ElementalLoad *theLoad, double loadFactor)
        {
            return 0;
        };
        int addInertiaLoadToUnbalance(const Vector &accel)
        {
            return 0;
        };

        const Vector &getResistingForce(void);
        const Vector &getResistingForceIncInertia(void);

        // public methods for element output
        int sendSelf(int commitTag, Channel &theChannel);
        int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
        void Print(ostream &s, int flag = 0);

        // Response* setResponse(const char** argv, int argc, Information& eleInfo);
        // int getResponse(int responseID, Information& eleInformation);



    private:


        ID  connectedExternalNodes;
        int numDOF;
        int dof;

        Matrix *Stiffness;



        int direction;
        Node *theNodes[2];
        double penaltystiffness;

        Vector tempZeroVector;
        Matrix tempZeroMatrix;

};

#endif




