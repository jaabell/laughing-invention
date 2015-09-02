///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli and Boris Jeremic
// DATE:               May 2013
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




#ifndef VariableNodeBrick_H
#define VariableNodeBrick_H

#include "bool.h"
#include <Element.h>
#include <Node.h>
#include <ID.h>
#include <Domain.h>
#include <string.h>
#include <OPS_Globals.h>
#include <Matrix.h>
#include <Vector.h>
#include <basics.h>
#include <BJtensor.h>
#include <nDarray.h>
#include <stresst.h>
#include <straint.h>
#include <MatPoint3D.h>
#include <NDMaterial.h>
#include <Template3Dep.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>
#include <Domain.h>
#include <string.h>
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
// #include <ElementResponse.h>
#include <ElementalLoad.h>
#include <Damping.h>

typedef BJmatrix matrix;


class Node;


class VariableNodeBrick: public Element
{

public:
    VariableNodeBrick(int element_number, int int_order,
                      int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
                      int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
                      int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
                      int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
                      int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
                      int node_numb_21, int node_numb_22, int node_numb_23, int node_numb_24,
                      int node_numb_25, int node_numb_26, int node_numb_27,
                      NDMaterial *Globalmmodel);


    VariableNodeBrick();
    ~VariableNodeBrick();


    int update(void);

    int getNumExternalNodes() const;
    const ID &getExternalNodes();
    Node **getNodePtrs(void);

    int getNumDOF();
    void setDomain(Domain *theDomain);

    // public methods to set the state of the element
    int commitState();
    int revertToLastCommit();
    int revertToStart();

    const Matrix &getTangentStiff();
    const Matrix &getInitialStiff();


    const Matrix &getMass();
    const Matrix &getMassFullSize();

    void zeroLoad();
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);

    const Vector FormEquiBodyForce(const Vector &data);
    const Vector &getResistingForce();
    const Vector &getResistingForceIncInertia();

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    void Print(ostream &s, int flag = 0);

    // Response* setResponse(const char** argv, int argc, Information& eleInformation);
    // int getResponse(int responseID, Information& eleInformation);


    int CheckMesh(ofstream &);
    Vector &Direction_Weight(double Xi , double Eta, Vector coord1, Vector coord2, Vector coord3, Vector coord4);
    double SurfaceShapeFunctionValues(double Xi , double Eta, int whichcomponent);
    double SurfaceLoadValues(double Xi , double Eta, Vector Pressure);
    const Vector &getBodyForce(double loadFactor, const Vector &data);
    const Vector &getSurfaceForce(double loadFactor, const Vector &data);


    void incremental_Update(void);


    tensor H_3D(double r1, double r2, double r3);
    tensor interp_poli_at(double r, double s, double t);
    tensor dh_drst_at(double r, double s, double t);


    VariableNodeBrick &operator[](int subscript);


    tensor getStiffnessTensor(void);

    void set_strain_stress_tensor(FILE *fp, double *u);


    tensor Jacobian_3D(tensor dh);
    tensor Jacobian_3Dinv(tensor dh);
    tensor Nodal_Coordinates(void);

    tensor incr_disp(void);
    tensor total_disp(void);

    tensor total_disp(FILE *fp, double *u);

    tensor stiffness_matrix(const tensor &K);
    tensor mass_matrix(const tensor &M);


    int  get_global_number_of_node(int local_node_number);
    int  get_Brick_Number(void);


    double get_Gauss_p_c(short order, short point_numb);
    double get_Gauss_p_w(short order, short point_numb);

    // returns nodal forces for given stress field in an element
    tensor nodal_forces(void);
    // returns nodal forces for ITERATIVE stress field in an element
    tensor iterative_nodal_forces(void);
    // returns nodal forces for given constant stress field in the element
    tensor nodal_forces_from_stress(stresstensor &);
    // returns nodal forces for given incremental strain field in an element
    // by using the linearized constitutive tensor from the begining of the step !
    tensor linearized_nodal_forces(void);
    // updates Material point stresses and strains from given displacements
    tensor update_stress_strain(tensor &disp);

    void reportshort(char *);
    void reportPAK(char *);
    void reportpqtheta(int);
    void computeGaussPoint(void);
    void reportCIPIC(char *);
    void reportTensorF(FILE *);

    Vector *getStress(void);

    std::string getElementName() const
    {
        return "VariableNodeBrick";
    }

private:

    int numDOF;
    ID  connectedExternalNodes;
    ID  node_existance;
    ID  *existing_node_numbers;

    Node **theNodes;

    Matrix *K;      // Element stiffness Matrix
    Matrix *M;      // Element mass matrix
    Vector *P;      // Element resisting force vector
    Vector *Q;      // Applied nodal loads

    Vector *GaussCoord;


    Vector *tempZero;

    double rho;     // Mass per unit volume
    int    order;   // Order of the quadrature rule

    double determinant_of_Jacobian;
    int nodes_in_brick;      // number of nodes ( from 8-20 //8 now Zhaohui)


    NDMaterial *mmodel;      // pointer to GLOBAL material models

    bool initialized; //Guanzhou added for populate function
    void populate(); //Guanzhou added for parallel processing

    int integration_order; // Gauss-Legendre integration order

    int Num_TotalGaussPts;

    // Now I want 3D array of Material points!
    // MatPoint3D[r_integration_order][s_integration_order][t_integration_order]
    // 3D array of Material points
    MatPoint3D **matpoint;   // pointer to array of Material Points


};


#endif

