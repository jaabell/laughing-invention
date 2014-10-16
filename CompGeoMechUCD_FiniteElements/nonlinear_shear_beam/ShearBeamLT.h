///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              ShearBeamLT.cpp
// CLASS:             ShearBeamLT
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic  & Jose Abell
// PROGRAMMER:        Boris Jeremic  & Jose Abell
// DATE:              September 2014
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#ifndef ShearBeamLT_H
#define ShearBeamLT_H


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


// Output is 6 components of strain 6 components of plastic strain and 6 of stress per gauss point
#define ShearBeamLT_NUMBER_OF_GAUSSPOINTS 1
#define ShearBeamLT_OUTPUT_SIZE ShearBeamLT_NUMBER_OF_GAUSSPOINTS*(6*3)

class Node;

class ShearBeamLT: public Element
{

    public:
        ShearBeamLT( int element_number,
                     int node_numb_1, int node_numb_2,
                     double area,
                     NDMaterialLT *Globalmmodel);

        ShearBeamLT ();
        ~ShearBeamLT();

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
        virtual int getOutputSize() const;
        virtual const Vector &getOutput() const;

        std::string getElementName() const
        {
            return "ShearBeamLT";
        }

        Vector *getStress( void );


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

        static Matrix K;
        static Matrix M;
        static Vector P;

        static DTensor2 gp_coords; //Coordinates of 1D Gaussian quadrature rule
        static DTensor1 gp_weight; //Weights of 1D Gaussian quadrature rule

        Matrix gauss_points;
        Vector outputVector;

        Index < 'i' > i;
        Index < 'j' > j;
        Index < 'k' > k;
        Index < 'l' > l;
};


#endif
