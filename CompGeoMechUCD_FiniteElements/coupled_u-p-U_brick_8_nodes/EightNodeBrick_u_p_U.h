///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Zhaohui Yang, Xiaoyan Wu, Zhao Cheng
// DATE:               Aug. 2001
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




//////////////////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              EightNodeBrick_u_p_U.h
// CLASS:             EightNodeBrick_u_p_U
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class for coupled system
//  "Coupled system": Solid and fluid coexist.
//                    u-- Solid displacement
//                    p-- Pore pressure
//                    U-- Absolute fluid displacement
// RETURN:
// VERSION:
// LANGUAGE:          C++.ver >= 3.0
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic, Xiaoyan Wu, Zhao Cheng (main for last revision)
// PROGRAMMER:        Boris Jeremic, Xiaoyan Wu, Zhaohui Yang, Zhao Cheng (main for last revision)
// DATE:              Sept. 2001
// UPDATE HISTORY:    Modified from EightNodeBrick.h.  Reorganized a lot by Xiaoyan
//                    01/24/2002    Xiaoyan
//                    Add the permeability tensor and ks, kf  to the constructor  Xiaoyan
//
//                    Clean-up and re-write by Zhao Cheng, 10/20/2004
//
//                    Fixed a bug, and some small modification, ZC 05/2006
//
//                    Nima Tafazzoli, removing pressure from the constructor, July 2012
//////////////////////////////////////////////////////////////////////////////////////////


#ifndef EIGHTNODEBRICK_U_P_U_H
#define EIGHTNODEBRICK_U_P_U_H



// Output is 6 components of strain, 6 components of plastic strain, and 6 of stress per gauss point,
// and one of pore pressure at each gauss
#define EightNodeBrick_U_P_U_NUMBER_OF_GAUSSPOINTS 8
#define EightNodeBrick_U_P_U_OUTPUT_SIZE EightNodeBrick_U_P_U_NUMBER_OF_GAUSSPOINTS*(6*3 + 1)



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


class EightNodeBrick_u_p_U: public Element
{
public:
    EightNodeBrick_u_p_U(int element_number,
                         int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                         int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                         NDMaterial *Globalmmodel,
                         double nn, double alf, double rs, double rf,
                         double permb_x, double permb_y, double permb_z,
                         double kks, double kkf);
    EightNodeBrick_u_p_U ();
    ~EightNodeBrick_u_p_U();

    const char *getClassType(void) const
    {
        return "EightNodeBrick_u_P_U";
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

    // Response* setResponse(const char** argv, int argc, Information& eleInfo);
    // int getResponse(int responseID, Information& eleInformation);

    //int setParameter (const char **argv, int argc, Information &info);
    //int updateParameter (int parameterID, Information &info);

    //alisa
    const Matrix &getTangent( int gaussNum );

    // Nima Tafazzoli (Feb. 2013)
    Vector *getStress(void);

    // Nima Tafazzoli added to return the Mass matrix (Feb. 2010)
    //     Matrix returnMass(void);

    std::string getElementName() const
    {
        return "EightNodeBrick_u_p_U";
    }
private:
    tensor shapeFunction(double, double, double);
    tensor shapeFunctionDerivative(double, double, double);
    tensor getGaussPts(void);
    tensor getNodesCrds(void);
    tensor getNodesDisp(void);
    tensor Jacobian_3D(double, double, double);
    tensor Jacobian_3Dinv(double, double, double);
    tensor dh_Global(double, double, double);

    tensor getStiffnessTensorKep();
    tensor getStiffnessTensorG12();
    tensor getMassTensorMsf();
    tensor getDampTensorC123();
    const Matrix &getStiff00(void);
    const Matrix &getStiff(int Ki_flag);
    double getPorePressure(double, double, double);
    const Vector &getInternalForce();
    const Vector &getBodyForce(const Vector &data);


    //Jose Added for output
    Matrix &getGaussCoordinates(void);
    virtual int getOutputSize() const;
    virtual const Vector &getOutput() ;

private:
    ID  connectedExternalNodes;    // tags of nodes
    Node *theNodes[8] ;            // pointers to eight nodes
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

    Vector perm;                   // Permeability = k/(rho_f*g)
    double poro;                   // Porosity
    double alpha;                  // Coefficient for soil (approximate equal 1)
    double rho_s;                  // Solid density
    double rho_f;                  // Fluid density
    double ks;                     // Bulk modulus of solid
    double kf;                     // Bulk modulus of fluid
    //     double pressure;               // Normal surface traction (pressure) over entire element  //?

    Vector *Q;
    Matrix *Ki;


    Matrix gauss_points;
    Vector outputVector;
};


#endif

