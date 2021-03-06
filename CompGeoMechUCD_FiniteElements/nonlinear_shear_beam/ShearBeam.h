///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic  & Jose Abell
// DATE:               September 2014
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



#ifndef ShearBeam_H
#define ShearBeam_H


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


// #include <ElementResponse.h>
#include <ElementalLoad.h>

class Node;

class ShearBeam: public Element
{

public:
    ShearBeam( int element_number,
                 int node_numb_1, int node_numb_2,
                 double area,
                 NDMaterialLT *Globalmmodel);

    ShearBeam ();
    ~ShearBeam();

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
    int describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel);
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
    double SurfaceShapeFunctionValues( double Xi , double Eta, int whichcomponent );
    double SurfaceLoadValues( double Xi , double Eta, Vector Pressure );
    const Vector &getBodyForce( double loadFactor, const Vector &data );
    const Vector &getSurfaceForce( double loadFactor, const Vector &data );
    // double returnPressure(void);


    // ===================================================================================================================
    // Internal member functions
    // ===================================================================================================================

    void formOutput();
    void ComputeVolume();
    void computeGaussPoint( void );


    Matrix &getGaussCoordinates(void);
    /********************************************************************************************************************
    * Sumeet August, 2016
    * This element has only outputs at gauss points so no needto have the "getElementOutput()" function
    *********************************************************************************************************************/
    virtual const vector<float> &getGaussOutput() ;

    std::string getElementName() const
    {
        return "ShearBeam";
    }

    Vector *getStress( void );

    virtual int startNewStage();

    // ===================================================================================================================
    // Data members
    // ===================================================================================================================
private:
    // bool initialized;
    bool is_mass_computed;  //Mass will be computed just once... its plain silly to compute the same mass infinite times
    double Area;
    double length;
    double rho;

    ID  connectedExternalNodes;

    DTensor2 stress[1];
    Matrix *Ki;

    Node *theNodes[2];
    NDMaterialLT *mmodel;
    NDMaterialLT *material_array[1];



    Vector Q;
    Vector bf;

    Matrix K;
    Matrix M;
    Vector P;

    static DTensor2 gp_coords; //Coordinates of 1D Gaussian quadrature rule
    static DTensor1 gp_weight; //Weights of 1D Gaussian quadrature rule

    Matrix gauss_points;
    static vector<float> Gauss_Output_Vector;

    Index < 'i' > i;
    Index < 'j' > j;
    Index < 'k' > k;
    Index < 'l' > l;
};


#endif
