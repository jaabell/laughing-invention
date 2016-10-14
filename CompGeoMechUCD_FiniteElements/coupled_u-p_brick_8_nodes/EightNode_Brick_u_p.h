///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Zhao Cheng, Boris Jeremic
// DATE:               Aug. 2006
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



#ifndef EIGHTNODE_BRICK_U_P_H
#define EIGHTNODE_BRICK_U_P_H

#ifndef _bool_h
#include "bool.h"
#endif

#include <Information.h>
// #include <ElementResponse.h>
#include <ElementalLoad.h>
#include <Domain.h>
#include <Node.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
//#include <Renderer.h>
#include <OPS_Globals.h>

#include <Element.h>
#include <NDMaterial.h>

#include <Vector.h>
#include <Matrix.h>
#include <BJtensor.h>
#include <stresst.h>
#include <straint.h>


class EightNode_Brick_u_p: public Element
{
public:
    EightNode_Brick_u_p(int element_number,
                        int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                        int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                        NDMaterial *Globalmmodel,
                        double nf, double alphaf, double rs, double rf,
                        double permb_x, double permb_y, double permb_z,
                        double kks, double kkf);
    EightNode_Brick_u_p ();
    ~EightNode_Brick_u_p();

    const char *getClassType(void) const
    {
        return "EightNode_Brick_u_p";
    };

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

    // public methods to obtain stiffness, mass, damping and residual information
    const Matrix &getTangentStiff(void);
    const Matrix &getInitialStiff(void);
    const Matrix &getDamp(void);
    const Matrix &getMass(void);

    void zeroLoad(void);
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);
    const Vector &getResistingForce(void);
    const Vector &getResistingForceIncInertia(void);

    // public methods for element output
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
    int displaySelf(Renderer &theViewer, int displayMode, float fact);
    void Print(ostream &s, int flag = 0);

    Matrix &getGaussCoordinates(void);
    /********************************************************************************************************************
    * Sumeet August, 2016
    * This element has only outputs at gauss points so no needto have the "getElementOutput()" function
    *********************************************************************************************************************/
    virtual const vector<float> &getGaussOutput();

    // Nima Tafazzoli (Feb. 2013)
    Vector *getStress(void);

    //int setParameter (const char **argv, int argc, Information &info);
    //int updateParameter (int parameterID, Information &info);

    // Nima Tafazzoli added to return the Mass matrix (Feb. 2010)
    //     Matrix returnMass(void);

    std::string getElementName() const
    {
        return "EightNode_Brick_u_p";
    }

private:
    tensor shapeFunction(double, double, double);
    tensor shapeFunctionDerivative(double, double, double);
    tensor getNodesCrds();
    tensor getNodesDisp();
    tensor Jacobian_3D(double, double, double);
    tensor Jacobian_3Dinv(double, double, double);
    tensor dh_Global(double, double, double);

    tensor getGaussPts(void);
    double getPorePressure(double, double, double);

    tensor getStiffnessTensorQ();
    tensor getStiffnessTensorH();
    tensor getStiffnessTensorKep();
    tensor getMassTensorM1();
    tensor getDampingTensorS();
    const Vector &getInternalForce();
    const Vector &getForceU(const Vector &data);
    const Vector &getForceP(const Vector &data);

    const Matrix &getStiff(int Ki_flag);
    const Matrix &getStiff00(void);

private:
    ID  connectedExternalNodes;    // tags of nodes
    Node *theNodes[8] ;            // pointers to nodes
    NDMaterial **theMaterial;      // pointers to the ND material objects

    static Matrix MCK;             // Mass, Damping, Stiffness
    static Vector P;


    static const int  Num_IntegrationPts;
    static const int  Num_TotalGaussPts;
    static const int  Num_Nodes;
    static const int  Num_Dim;
    static const int  Num_Dof;
    static const int  Num_ElemDof;
    static const double pts[2];    // Stores quadrature points
    static const double wts[2];    // Stores quadrature weights
    static tensor perm;        // Permeability = k/(rho_f*g)

    static Matrix gauss_points;
    static vector<float> Gauss_Output_Vector;  // For output [Sumeet September, 2016]

    double nf;                     // Porosity
    double alpha;              // simga = sigma' + alpha*p
    double rho_s;              // Solid density
    double rho_f;              // Fluid density
    double ks;                     // Bulk modulus of solid
    double kf;                     // Bulk modulus of fluid

    Vector *Q;
    Matrix *Ki;
};


#endif

