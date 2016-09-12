///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Zhaohui Yang, Xiaoyan Wu, Zhao Cheng & Sumeet Kumar Sinha
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
// Adapted by Sumeet Kumar Sinha, from EightNodeBrick to use the LTensor library for
// tensorial representation.
/////////////////////////////////////////////////////////////////////////////////////


#ifndef TwentyNodeBrick_upU_LTLT_H
#define TwentyNodeBrick_upU_LTLT_H


#include <Element.h>
#include <Node.h>
#include <Matrix.h>
#include <Vector.h>
#include <NDMaterialLT.h>
#include <Damping.h>
#include <MatPoint3D.h>
#include <map>
#include <set>
#include <vector>

#include <ID.h>
#include <OPS_Globals.h>
#include <Domain.h>
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>


#include <ElementalLoad.h>

class Node;

class TwentyNodeBrick_upU_LT: public Element
{

public:
    TwentyNodeBrick_upU_LT(int element_number,
                          int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                          int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                          int node_numb_9, int node_numb_10, int node_numb_11, int node_numb_12,
                          int node_numb_13, int node_numb_14, int node_numb_15, int node_numb_16,
                          int node_numb_17, int node_numb_18, int node_numb_19, int node_numb_20,
                          NDMaterialLT *Globalmmodel,
                          double nn, double alf, double rs, double rf,
                          double permb_x, double permb_y, double permb_z,
                          double kks, double kkf);

    TwentyNodeBrick_upU_LT ();
    ~TwentyNodeBrick_upU_LT();

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
    const Matrix &getConstStiff();    // Returns the matrix K [Sumeet September, 2016]
    const Matrix &getMass();
    const Matrix &getDamp();

    void zeroLoad ();
    int addLoad( ElementalLoad *theLoad, double loadFactor );
    int addInertiaLoadToUnbalance( const Vector &accel );

    // These NEED to return fmk Vector
    const Vector  FormEquiBodyForce( void );
    const Vector &getResistingForce ();
    const Vector &getResistingForceIncInertia ();
    // int getObjectSize();

    //Used in parallel and in saving model
    // int sendSelf ( int commitTag, Channel &theChannel );
    // int receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker
    //                   &theBroker );

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
    // const Vector &getSurfaceForce( double loadFactor, const Vector &data );


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
    void getStiffnessTensorKep();
    void getStiffnessTensorG12();
    void getDampTensorC123();
    void getMassTensorMsf();


    Matrix &getGaussCoordinates(void);
    /********************************************************************************************************************
    * Sumeet August, 2016
    * This element has only outputs at gauss points so no needto have the "getElementOutput()" function
    *********************************************************************************************************************/
    virtual const vector<float> &getGaussOutput();

    std::string getElementName() const
    {
        return "TwentyNodeBrick_upU_LT";
    }

    Vector *getStress( void );

    virtual void zeroStrain();

    // ===================================================================================================================
    // Data members
    // ===================================================================================================================
private:

    bool is_mass_computed;  //Mass will be computed just once... its plain silly to compute the same mass infinite times
    bool is_damping_computed;  //Mass will be computed just once... its plain silly to compute the same mass infinite times

    double Volume;

    ID  connectedExternalNodes;

    Node *theNodes[20];
    NDMaterialLT *mmodel;
    NDMaterialLT *material_array[27];

    Vector perm;                   // Permeability = k/(rho_f*g)
    double poro;                   // Porosity
    double alpha;                  // Coefficient for soil (approximate equal 1)
    double rho_s;                  // Solid density
    double rho_f;                  // Fluid density
    double ks;                     // Bulk modulus of solid
    double kf;                     // Bulk modulus of fluid

    Vector Q;

    Matrix M;                      // Mass
    Matrix C;                      // Damping
    Matrix *Ki;                    // Initial Stiffness
    static Matrix K;               // Tangentialia Stiffness

    Vector P;                      // Resisting Force

    // static double SurfaceLoadValues_in_function;
    // static Vector ShapeFunctionValues_in_function;
    // static Vector J_vector_in_function;
    std::map<int,int> Global_to_Local_Node_Mapping; // added by sumeet 

    static DTensor2 gp_coords;  // Coordinates of 1D Gaussian quadrature rule
    static DTensor2 gp_weight;  // Weights of 1D Gaussian quadrature rule
    static DTensor4 TensorKep;  // Stiffness Tensor [see lecture notes]
    static DTensor3 TensorG12;  // Stiffness Tensor [see lecture notes]
    static DTensor4 TensorC123; // Damping Stiffness tensor [see lecture notes]
    static DTensor2 TensorMsf;  // Mass Tensor [see lecture notes]

    static Matrix gauss_points;
    static vector<float> Gauss_Output_Vector;  // Sumeet August, 2016

    double update_time_taken;

    Index < 'i' > i;
    Index < 'j' > j;
    Index < 'k' > k;
    Index < 'l' > l;
};


#endif
