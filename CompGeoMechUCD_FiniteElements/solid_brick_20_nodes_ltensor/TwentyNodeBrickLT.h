///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Zhaohui Yang  and Xiaoyan Wu
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


#ifndef TwentyNodeBrickLTLT_H
#define TwentyNodeBrickLTLT_H


#include <Element.h>
#include <Node.h>
#include <Matrix.h>
#include <Vector.h>
#include <NDMaterialLT.h>
#include <Damping.h>
#include <MatPoint3D.h>

#include <ID.h>
#include <OPS_Globals.h>
#include <Domain.h>
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>

#include <memory>


// #include <ElementResponse.h>
#include <ElementalLoad.h>



// Output is 6 components of strain, 6 components of plastic strain, and 6 of stress per gauss point
#define TwentyNodeBrickLT_NUMBER_OF_GAUSSPOINTS 27
#define TwentyNodeBrickLT_OUTPUT_SIZE TwentyNodeBrickLT_NUMBER_OF_GAUSSPOINTS*(6*3)




class Node;

class TwentyNodeBrickLT: public Element
{

public:
    TwentyNodeBrickLT(int element_number,
                    int node_numb_1,  int node_numb_2,  int node_numb_3,  int node_numb_4,
                    int node_numb_5,  int node_numb_6,  int node_numb_7,  int node_numb_8,
                    int node_numb_9,  int node_numb_10, int node_numb_11, int node_numb_12,
                    int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
                    int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
                    NDMaterialLT *Globalmmodel);

    TwentyNodeBrickLT ();
    ~TwentyNodeBrickLT();


    // ===================================================================================================================
    // Implements interface to Element
    // ===================================================================================================================
   
    int update(void);
    int getNumExternalNodes () const;
    const ID &getExternalNodes ();
    Node **getNodePtrs();

    int getNumDOF ();
    void setDomain(Domain *theDomain);
    // public methods to set the state of the element
    int commitState ();
    int revertToLastCommit ();
    int revertToStart ();
    // public methods to obtain stiffness, mass, damping and residual information
    // We haven't build the following functions.
    // All the value of K M Dmp and F are nothing.

    const Matrix &getTangentStiff ();
    const Matrix &getInitialStiff();
    const Matrix &getMass ();


    void zeroLoad ();
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    //int addLoad(const Vector &addP);
    int addInertiaLoadToUnbalance(const Vector &accel);

    const Vector  FormEquiBodyForce(const Vector &data);
    const Vector &getResistingForce ();
    const Vector &getResistingForceIncInertia ();
    int getObjectSize();
    // public methods for element output
    int sendSelf (int commitTag, Channel &theChannel);
    int receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker
                     &theBroker);

    void Print(ostream &s, int flag = 0);

    int CheckMesh(ofstream &);


    // Nima Tafazzoli added for surface load (July 2012)
    // ===================================================================================================================
    // For Body Forces and Surface forces
    // ===================================================================================================================
  
    Vector &Direction_Weight(double Xi , double Eta,
                             Vector coord1, Vector coord2, Vector coord3, Vector coord4,
                             Vector coord5, Vector coord6, Vector coord7, Vector coord8);
    double SurfaceShapeFunctionValues(double Xi , double Eta, int whichcomponent);
    double SurfaceLoadValues(double Xi , double Eta, Vector Pressure);
    const  Vector &getBodyForce(double loadFactor, const Vector &data);
    const  Vector &getSurfaceForce(double loadFactor, const Vector &data);



    // ===================================================================================================================
    // Internal member functions
    // ===================================================================================================================

    void formOutput();
    void ComputeVolume();
    void computeGaussPoint( void );

    int  get_global_number_of_node( int local_node_number );
    int  get_Brick_Number( void );
    int *get_LM( void );

    const DTensor2 &H_3D( double r1, double r2, double r3 ) const;
    const DTensor1 &interp_poli_at( double r, double s, double t ) const;
    const DTensor2 &dh_drst_at( double r, double s, double t ) const;
    const DTensor4 &getStiffnessTensor( void ) const;
    const DTensor2 &Jacobian_3D( const DTensor2 &dh ) const;
    const DTensor2 &Jacobian_3Dinv( const DTensor2 &dh ) const;
    const DTensor2 &Nodal_Coordinates( void ) const ;
    const DTensor2 &incr_disp( void ) const;
    const DTensor2 &total_disp( void ) const ;
    const DTensor2 &nodal_forces( void ) const;



    Matrix &getGaussCoordinates(void);
    virtual int getOutputSize() const;
    virtual const Vector &getOutput() ;


    std::string getElementName() const
    {
        return "TwentyNodeBrickLT";
    }

    Vector *getStress( void );
    // std::shared_ptr<Vector> getStress( void );

protected:

private:

    bool is_mass_computed;

    int numDOF;             // Number of element DOF
    int nodes_in_brick;
    int order;

    double Volume;
    double e_p;
    double determinant_of_Jacobian;
    double rho;

    ID  connectedExternalNodes; // Tags of quad nodes
    

    DTensor2 stress[20];
    Matrix *Ki;

    Node *theNodes[20];
    NDMaterialLT *mmodel;
    NDMaterialLT *material_array[27];  //27 Gauss Points for 20NodeBrick



    Vector Q;     // Applied nodal loads
    Vector bf;
    // ELE_TAG_TwentyNodeBrickLT

    Matrix M;    // Element mass matrix
    Vector P;    // Element resisting force vector

    static Vector ShapeFunctionValues_in_function;
    static Vector J_vector_in_function;
    static double SurfaceLoadValues_in_function;

    
    
    static DTensor2 gp_coords; //Coordinates of 1D Gaussian quadrature rule
    static DTensor2 gp_weight; //Weights of 1D Gaussian quadrature rule

    static Matrix gauss_points;
    static Vector outputVector;

    static Matrix K;    // Element stiffness Matrix

    Index < 'i' > i;
    Index < 'j' > j;
    Index < 'k' > k;
    Index < 'l' > l;
    

};


#endif

