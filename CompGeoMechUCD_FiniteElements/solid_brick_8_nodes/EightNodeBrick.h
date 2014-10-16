///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              EightNodeBrick.cpp
// CLASS:             EightNodeBrick
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic, Zhaohui Yang and Xiaoyan Wu
// PROGRAMMER:        Boris Jeremic, Zhaohui Yang  and Xiaoyan Wu
// DATE:              Aug. 2000
// UPDATE HISTORY:    Modified from Brick3D and FourNodeQuad.hh  07/06/00
//                    Sept. - Oct 2000 connected to OpenSees by Zhaohui
//                    Sept 2001 optimized to some extent (static tensors...)
//                    May 2004 Guanzhou added update()
//                    September 2010 Nima removed density as an input argument
//                    October 2010 Nima removed pressure as an input argument
//                    July 2012 Nima added the surface load
//
///////////////////////////////////////////////////////////////////////////////


#ifndef EIGHTNODEBRICK_H
#define EIGHTNODEBRICK_H

// Output is 6 components of strain, 6 components of plastic strain, and 6 of stress per gauss point
#define EightNodeBrick_NUMBER_OF_GAUSSPOINTS 8
#define EightNodeBrick_OUTPUT_SIZE EightNodeBrick_NUMBER_OF_GAUSSPOINTS*(6*3)

#ifndef _bool_h
#include "bool.h"
#endif

#include <Element.h>
#include <Node.h>

// Commented by Xiaoyan. Use  ~/fem/node.hh  08/04/00`
// Released Node.h now. Wu use Opensees's Node.09/27/00


#include <ID.h>
//#include <Renderer.h>
#include <Domain.h>
#include <string.h>

//#include <GaussQuadRule1d.h>

#include <OPS_Globals.h>

#include <Matrix.h>
#include <Vector.h>

// Xiaoyan added from brick3d.hh   07/11/00
#include <basics.h>
#include <BJtensor.h>
#include <nDarray.h>
#include <stresst.h>
#include <straint.h>

//#include <node.h>
//#include <mmodel.h>
//#include <fe.h>
#include <MatPoint3D.h>

#include <NDMaterial.h>
#include <Template3Dep.h>

//#include <NDMaterial.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>
//#include <Renderer.h>
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
//class NDMaterial;
//class QuadRule1d;

class EightNodeBrick: public Element
{

    public:
        EightNodeBrick(int element_number,
                       int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                       int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                       NDMaterial *Globalmmodel);
        // int dir, double surflevel);
        //, EPState *InitEPS);   const char * type,

        EightNodeBrick ();
        ~EightNodeBrick();

        //Not needed Zhaohui
        //int Initialize(int element_number,
        //               int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
        //               int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
        //               NDMaterial * Globalmmodel, double b1, double b2, double b3,
        //               double p, double r);
        //             //, EPState * InitEPS);const char * type,

        // update, Guanzhou added Apr. 2004 to update incremental strain in the domain
        int update(void);

        int getNumExternalNodes () const;
        const ID &getExternalNodes ();
        Node **getNodePtrs(void);

        int getNumDOF ();
        void setDomain(Domain *theDomain);

        // public methods to set the state of the element
        int commitState ();
        int revertToLastCommit ();
        int revertToStart ();

        // public methods to obtain stiffness, mass, damping and residual information
        // We haven't build the following functions.
        // All the value of K M Dmp and F are nothing. just
        // want to test the program.  Xiaoyan 08/16/00
        const Matrix &getTangentStiff ();
        const Matrix &getInitialStiff();
        //    const Matrix &getlumpedMass ();

        const Matrix &getMass ();

        void zeroLoad ();
        int addLoad(ElementalLoad *theLoad, double loadFactor);
        int addInertiaLoadToUnbalance(const Vector &accel);

        const Vector  FormEquiBodyForce(const Vector &data);
        const Vector &getResistingForce ();
        const Vector &getResistingForceIncInertia ();

        // public methods for element output
        int sendSelf (int commitTag, Channel &theChannel);
        int receiveSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker
                      &theBroker);

        int getObjectSize(); //Guanzhou added

        // OUT graphics has nothing to do here!    int displaySelf (Renderer &theViewer, int displayMode, float fact);
        void Print(ostream &s, int flag = 0);
        //    Do nothing with void Print (ostream &s, int flag =0);
        //    use Brick3D report.  08/16/00
        // Response* setResponse (const char** argv, int argc, Information& eleInformation);
        // int getResponse (int responseID, Information& eleInformation);



        // Nima Tafazzoli added for surface load (July 2012)
        Vector &Direction_Weight(double Xi , double Eta, Vector coord1, Vector coord2, Vector coord3, Vector coord4);
        double SurfaceShapeFunctionValues(double Xi , double Eta, int whichcomponent);
        double SurfaceLoadValues(double Xi , double Eta, Vector Pressure);
        const Vector &getBodyForce(double loadFactor, const Vector &data);
        const Vector &getSurfaceForce(double loadFactor, const Vector &data);
        double returnPressure(void);

        // Nima Tafazzoli added (Sep. 2012)
        int CheckMesh(ofstream &);

        std::string getElementName() const
        {
            return "EightNodeBrick";
        }

        //     int getDampingTag(void);
        //     double getDamping_a0(void);
        //     double getDamping_a1(void);
        //     double getDamping_a2(void);
        //     double getDamping_a3(void);
        //     string getDamping_stiffnesstype(void);
        //     string getDamping_type(void);

    protected:

    private:
        // private attributes - a copy for each object of the class

        //NDMaterial * **theMaterial; // pointer to the ND material objects

        int numDOF;             // Number of element DOF
        ID  connectedExternalNodes; // Tags of quad nodes

        Matrix *Ki;
        Node *theNodes[8];

        static Matrix K;        // Element stiffness Matrix
        //Guanzhou out static Matrix C;     // Element damping matrix
        static Matrix M;        // Element mass matrix
        static Vector P;        // Element resisting force vector
        Vector Q;       // Applied nodal loads

        //Vector q0;  // hold element load affects q0 and p0 in one vector

        // double thickness;    // Element thickness
        double rho;     // Mass per unit volume
        //    double pressure;  // Normal surface traction (pressure) over entire element
        int    order;   // Order of the quadrature rule

        //Guanzhou added
        double epEnergy;
        double Volume;
        double e_p;


        // Nima Tafazzoli added for surface load (July 2012)
        static Vector ShapeFunctionValues_in_function;
        static Vector J_vector_in_function;
        static double SurfaceLoadValues_in_function;


        // Nima Tafazzoli (Oct. 2010)
        //     int damptag;
        //     double damp_a0;
        //     double damp_a1;
        //     double damp_a2;
        //     double damp_a3;
        //     string stiffness_type;
        //     string damping_type;

        void ComputeVolume();
        straintensor pl_stn[8]; //Guanzhou
        stresstensor stress[8]; //Guanzhou
        //int dir;            // Direction of vertial coord.
        //double surflevel;   // free surface level above or below this element

        //QuadRule1d *theQuadRule;  // Integration rule

        //    Matrix J;     // Jacobian of transformation
        //    Matrix L;     // Inverse of J
        //    Matrix B;     // Strain interpolation matrix


        //    // static data - single copy for all objects of the class
        //    static G3Matrix N;    // Displacement interpolation matrix

        //    // private member functions - only objects of this class can call these
        //    void setJacobian (double r, double s, double t);         // Xiaoyan changed
        //    double formDetJ (double r, double s, double t);      // xi, eta to r and s
        //    void formBMatrix (double r, double s, double t);     // and added t
        //    static void formNMatrix (double r, double s, double t);  // 07/06/00

        //Jose Added for output
        Matrix &getGaussCoordinates(void);
        virtual int getOutputSize() const;
        virtual const Vector &getOutput() const;

    private:
        // element number (tag)
        //unsigned int  elem_numb;

        double determinant_of_Jacobian;
        //int  G_N_numbs[8];     // Global node numbers for this element  Xiaoyan changed from 20 to 8

        int nodes_in_brick;      // number of nodes ( from 8-20 //8 now Zhaohui)

        //Node * nodes;          // pointer to GLOBAL nodes

        NDMaterial *mmodel;      // pointer to GLOBAL material models

        bool initialized; //Guanzhou added for populate function
        void populate(); //Guanzhou added for parallel processing

        int integration_order; // Gauss-Legendre integration order

        // Now I want 3D array of Material points!
        // MatPoint3D[r_integration_order][s_integration_order][t_integration_order]
        // 3D array of Material points
        MatPoint3D **matpoint;   // pointer to array of Material Points

        // 3D array of material models for each Material points
        // NDMaterial *GPmmodel;  // pointer to array of material models for Material Points
        // Do we need this one?

        //..NDMaterial  *MatPoint;  // Zhaohui  10-01-2000


        int  LM[24]; // for 8noded x 3 = 24

        Matrix gauss_points;
        Vector outputVector;


    public:

        void incremental_Update(void);
        //void iterative_Update(void);


        tensor H_3D(double r1, double r2, double r3);
        tensor interp_poli_at(double r, double s, double t);
        tensor dh_drst_at(double r, double s, double t);


        //CE Dynamic Allocation for for brick3d s.
        //Finite_Element * new_el( int total );
        EightNodeBrick &operator[](int subscript);
        //Finite_Element & operator[](int subscript);
        //Finite_Element & operator[](int subscript);

        tensor getStiffnessTensor(void);
        //matrix stiffness_tensor(void);

        void set_strain_stress_tensor(FILE *fp, double *u);
        //    tensor getMassTensor(void);
        //out19Jan2001     double Potential_Energy(void);

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

        int *get_LM(void);
        //void set_LM(Node * node); // commented out temporarily 09-27-2000 Zhaohui

        //these two files are originally in fe.h
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

        void report(char *);
        void reportshort(char *);
        void reportPAK(char *);
        void reportpqtheta(int);
        //void reportLM(char *);
        void computeGaussPoint(void);
        void reportCIPIC(char *);
        void reportTensorF(FILE *);

        // Setting initial E according to the initial pressure
        //void setInitE(void);
        //void reportStressTensorF(FILE *);


        // Nima Tafazzoli (Jan. 2013)
        Vector *getStress(void);

};


#endif

