///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Guanzhou Jie
// DATE:               Oct. 2003
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



#ifndef TWENTYSEVENNODEBRICK_H
#define TWENTYSEVENNODEBRICK_H


#define TwentySevenNodeBrick_NUMBER_OF_GAUSSPOINTS 27
#define TwentySevenNodeBrick_IntegrationOrder 3
#define TwentySevenNodeBrick_NumberOfNodes 27
#define TwentySevenNodeBrick_NumberOfGaussPoints 27
#define TwentySevenNodeBrick_NumberOfDOFS 81

// Output is 6 components of strain, 6 comps of plastic strain, and 6 of stress per gauss point
#define TwentySevenNodeBrick_OUTPUT_SIZE TwentySevenNodeBrick_NUMBER_OF_GAUSSPOINTS*(6*3)

#ifndef _bool_h
#include "bool.h"
#endif

#include <Element.h>
#include <Node.h>


#include <ID.h>
//#include <Renderer.h>
#include <Domain.h>
#include <string.h>

//#include <GaussQuadRule1d.h>

#include <OPS_Globals.h>

#include <Matrix.h>
#include <Vector.h>
#include <ID.h>

#include <basics.h>
#include <BJtensor.h>
#include <nDarray.h>
#include <stresst.h>
#include <straint.h>

#include <MatPoint3D.h>

#include <Template3Dep.h>

#include <NDMaterial.h>
#include <ID.h>
#include <Domain.h>
#include <string.h>
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <ElementalLoad.h>
#include <Damping.h>

typedef BJmatrix matrix;

class Node;

class TwentySevenNodeBrick: public Element
{

public:
    TwentySevenNodeBrick(int element_number,
                         int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
                         int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
                         int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
                         int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
                         int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
                         int node_numb_21,  int node_numb_22,  int node_numb_23,  int node_numb_24,
                         int node_numb_25,  int node_numb_26,  int node_numb_27,
                         NDMaterial *Globalmmodel);

    TwentySevenNodeBrick ();
    ~TwentySevenNodeBrick();

    int getNumExternalNodes () const;
    const ID &getExternalNodes ();
    Node **getNodePtrs();

    int getNumDOF ();
    void setDomain(Domain *theDomain);

    int commitState ();
    int revertToLastCommit ();
    int revertToStart ();
    int update(void);

    const Matrix &getTangentStiff ();
    const Matrix &getInitialStiff();
    const Matrix &getMass ();

    const Matrix &getConsMass ();
    const Matrix &getLumpedMass ();

    void zeroLoad ();
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);
    const Vector  FormEquiBodyForce(const Vector &data);
    const Vector &getResistingForce ();
    const Vector &getResistingForceIncInertia ();

    int sendSelf (int commitTag, Channel &theChannel);
    int receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker
                     &theBroker);
    void Print(ostream &s, int flag = 0);

    Vector &Direction_Weight(double Xi , double Eta,
                             Vector coord1, Vector coord2, Vector coord3, Vector coord4,
                             Vector coord5, Vector coord6, Vector coord7, Vector coord8,
                             Vector coord9);
    Vector &SurfaceShapeFunctionValues(double Xi , double Eta);
    double &SurfaceLoadValues(double Xi , double Eta, Vector Pressure);
    const  Vector &getBodyForce(double loadFactor, const Vector &data);
    const  Vector &getSurfaceForce(double loadFactor, const Vector &data);

    int getObjectSize();
    int CheckMesh(ofstream &);

    Matrix &getGaussCoordinates(void);
    virtual int getOutputSize() const;
    virtual const Vector &getOutput() ;

    std::string getElementName() const
    {
        return "TwentySevenNodeBrick";
    }


    void incremental_Update(void);

    tensor H_3D(double r1, double r2, double r3);
    tensor interp_poli_at(double r, double s, double t);
    tensor dh_drst_at(double r, double s, double t);


    TwentySevenNodeBrick &operator[](int subscript);

    tensor getStiffnessTensor(void);

    void set_strain_stress_tensor(FILE *fp, double *u);
    tensor getMassTensor(void);

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

    void computeGaussPoint(void);
    Vector getWeightofGP(void);

    Vector *getStress(void);


private:

    int numDOF;          // Number of element DOF
    ID  connectedExternalNodes; // Tags of quad nodes

    Matrix *Ki;
    Node *theNodes[27];

    static Matrix K;    // Element stiffness Matrix
    static Matrix C;    // Element damping matrix
    static Matrix M;    // Element mass matrix
    static Vector P;    // Element resisting force vector
    Vector Q;           // Applied nodal loads
    Vector bf;

    double rho;    // Mass per unit volume DO WE GET THIS ONE OUT!!!
    int    order;    // Order of the quadrature rule

    static Vector ShapeFunctionValues_in_function;
    static Vector J_vector_in_function;
    static double SurfaceLoadValues_in_function;

    double determinant_of_Jacobian;

    NDMaterial *mmodel;      // pointer to GLOBAL material models


    MatPoint3D **matpoint;   // pointer to array of Material Points

    Matrix gauss_points;
    Vector outputVector;
};


#endif

