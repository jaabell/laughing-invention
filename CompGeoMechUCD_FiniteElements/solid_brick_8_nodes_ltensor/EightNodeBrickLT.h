///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Zhaohui Yang, Xiaoyan Wu, Nima Tafazzoli, & Jose Abell
// DATE:               NOv. 2013
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

/////////////////////////////////////////////////////////////////////////////////////
// Adapted by Jose Abell, from EightNodeBrick to use the LTensor library for
// tensorial representation.
/////////////////////////////////////////////////////////////////////////////////////


#ifndef EightNodeBrickLTLT_H
#define EightNodeBrickLTLT_H


#include <Element.h>
#include <Node.h>
#include <Matrix.h>
#include <Vector.h>
#include <NDMaterialLT.h>
#include <Damping.h>
#include <MatPoint3D.h>
#include <map>
#include <set>

#include <ID.h>
#include <OPS_Globals.h>
#include <Domain.h>
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>


// #include <ElementResponse.h>
#include <ElementalLoad.h>


// Output is 6 components of strain 6 components of plastic strain and 6 of stress per gauss point + 1 for update time
#define EightNodeBrickLT_NUMBER_OF_GAUSSPOINTS 8
#define EightNodeBrickLT_OUTPUT_SIZE EightNodeBrickLT_NUMBER_OF_GAUSSPOINTS*(6*3) 

class Node;

class EightNodeBrickLT: public Element
{

public:
    EightNodeBrickLT( int element_number,
                      int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                      int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                      NDMaterialLT *Globalmmodel);

    EightNodeBrickLT ();
    ~EightNodeBrickLT();

    // ===================================================================================================================
    // Implements interface to Element
    // ===================================================================================================================
    int update( void );
    int getNumExternalNodes () const;
    const ID &getExternalNodes ();
    Node **getNodePtrs( void );

    int getNumDOF ();
    void setDomain( Domain *theDomain );

    int commitState ();
    int revertToLastCommit ();
    int revertToStart ();

    // These NEED to return fmk Matrices
    const Matrix &getTangentStiff ();
    const Matrix &getInitialStiff();
    const Matrix &getMass ();

    void zeroLoad ();
    int addLoad( ElementalLoad *theLoad, double loadFactor );
    int addInertiaLoadToUnbalance( const Vector &accel );

    // These NEED to return fmk Vector
    const Vector  FormEquiBodyForce( void );
    const Vector &getResistingForce ();
    const Vector &getResistingForceIncInertia ();
    int getObjectSize();

    //Used in parallel and in saving model
    int sendSelf ( int commitTag, Channel &theChannel );
    int receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker
                      &theBroker );

    //General reporting of element status
    void Print( ostream &s, int flag = 0 );

    //Determine if element is ok and calls CheckMesh on children material
    int CheckMesh( ofstream &);

    // ===================================================================================================================
    // For Body Forces and Surface forces
    // ===================================================================================================================
    Vector &Direction_Weight( double Xi , double Eta, Vector coord1, Vector coord2, Vector coord3, Vector coord4 );
    double SurfaceShapeFunctionValues( double Xi , double Eta, int whichcomponent );
    double SurfaceLoadValues( double Xi , double Eta, Vector Pressure );
    const Vector &getBodyForce( double loadFactor, const Vector &data );
    const Vector &getSurfaceForce( double loadFactor, const Vector &data );


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
        return "EightNodeBrickLT";
    }

    Vector *getStress( void );

    // ===================================================================================================================
    // Data members
    // ===================================================================================================================
private:
    bool is_mass_computed;  //Mass will be computed just once... its plain silly to compute the same mass infinite times

    int numDOF;
    int nodes_in_brick;
    int order;

    double Volume;
    double e_p;
    double determinant_of_Jacobian;
    double rho;

    std::map<int,int> Global_to_Local_Node_Mapping; // added by sumeet 

    ID  connectedExternalNodes;

    DTensor2 stress[8];
    Matrix *Ki;

    Node *theNodes[8];
    NDMaterialLT *mmodel;
    NDMaterialLT *material_array[8];

    Vector Q;
    Vector bf;

    Matrix M;
    Vector P;

    static double SurfaceLoadValues_in_function;
    static Vector ShapeFunctionValues_in_function;
    static Vector J_vector_in_function;

    static DTensor2 gp_coords; //Coordinates of 1D Gaussian quadrature rule
    static DTensor1 gp_weight; //Weights of 1D Gaussian quadrature rule

    static Matrix gauss_points;
    static Vector outputVector;

    static Matrix K;

    double update_time_taken;

    Index < 'i' > i;
    Index < 'j' > j;
    Index < 'k' > k;
    Index < 'l' > l;
};


#endif
