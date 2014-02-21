///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              brick3d.hh
// CLASS:             brick3D
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++.ver >= 3.0
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic
// PROGRAMMER:        Boris Jeremic
// DATE:              October 14 '94.
// UPDATE HISTORY:
//
//
//
//
///////////////////////////////////////////////////////////////////////////////
//
#ifndef BRICK3D_HH
#define BRICK3D_HH

#include "nDarray/basics.hh"
#include "nDarray/BJtensor.hh"
#include "nDarray/ndarray.hh"
#include "stresst.hh"
#include "straint.hh"
#include "node.hh"
#include "3D-el-pl/mmodel.hh"
#include "fe.hh"
#include "gausspnt.hh"

class Brick3D : public Finite_Element
{
    private:
        unsigned int  elem_numb;      // element number
        double determinant_of_Jacobian;
        unsigned int  G_N_numbs[20];  // Global node numbers for this element
        short int node_existance[12]; // node existance (nodes 9-20)(to be bool array!)
        short int nodes_in_brick;     // number of nodes ( from 8-20 )
        Node* nodes;                  // pointer to GLOBAL nodes
        Material_Model* mmodel;       // pointer to GLOBAL material models
        short int r_integration_order; // Gauss-Legendre integration order in r direction
        short int s_integration_order; // Gauss-Legendre integration order in s direction
        short int t_integration_order; // Gauss-Legendre integration order in t direction

        // Now I wand 3D array of Gauss points!
        // GaussPoint[r_integration_order][s_integration_order][t_integration_order]
        // 3D array of Gauss points
        GaussPoint* GaussP;     // pointer to array of Gauss Points
        // 3D array of material models for each Gauss points
        Material_Model* GPmmodel;   // pointer to array of material models for Gauss Points

        // 3D array of tensors models for each Gauss points
        // this tensor is tangent constitutive tensor MS definition.
        // It represents the state at Gauss point before applying strain increment
        // that was produced by incremental displacements
        tensor* GPtangent_E;   // pointer to array of constitutive tensors for Gauss Points

        // 3D array of stresstensors models for each Gauss points
        // this stress tensor is start_stress from my MS definition.
        // It represents the state at Gauss point before applying strain increment
        // that was produced by incremental displacements
        stresstensor* GPstress;   // pointer to array of stresstensors for Gauss Points

        // 3D array of stresstensors models for each Gauss points
        // this stress tensor is iterative stress.
        // It represents the state at Gauss point during iterative procedure on FEM level.
        stresstensor* GPiterative_stress;   // pointer to array of stresstensors for Gauss Points
        double* GPq_ast_iterative;   // pointer to array of iterative values of internal variable
        // 3D array of straintensors models for each Gauss points
        // this strain tensor is strain_increment from my MS definition.
        // It represents the additional strains that are to be
        // integrated. After numerical integration is done, the return value
        // ( from one of the numerical integration procedures )
        // is stresstensor that should then be put in GPstress place!
        straintensor* GPstrain;   // pointer to array of straintensors for Gauss Points

        // this is LM array. This array holds DOFs for this element
        int  LM[24]; // for 8noded x 3 = 24
    public:
        // CONSTRUCTOR
        Brick3D(unsigned int element_numb = 0, // default constructor
                short int r_int_order = 0,
                short int s_int_order = 0,
                short int t_int_order = 0,
                unsigned int node_numb_1  = 0,
                unsigned int node_numb_2  = 0,
                unsigned int node_numb_3  = 0,
                unsigned int node_numb_4  = 0,
                unsigned int node_numb_5  = 0,
                unsigned int node_numb_6  = 0,
                unsigned int node_numb_7  = 0,
                unsigned int node_numb_8  = 0,
                unsigned int node_numb_9  = 0,
                unsigned int node_numb_10  = 0,
                unsigned int node_numb_11  = 0,
                unsigned int node_numb_12  = 0,
                unsigned int node_numb_13  = 0,
                unsigned int node_numb_14  = 0,
                unsigned int node_numb_15  = 0,
                unsigned int node_numb_16  = 0,
                unsigned int node_numb_17  = 0,
                unsigned int node_numb_18  = 0,
                unsigned int node_numb_19  = 0,
                unsigned int node_numb_20  = 0,
                Node* GlobalNodes = 0,    // maybe put NULL
                Material_Model* Globalmmodel = 0,
                tensor*    IN_tangent_E = 0,
                stresstensor* INstress = 0,
                stresstensor* INiterative_stress = 0,
                double*        INq_ast_iterative = 0,
                straintensor* INstrain = 0
               );

        void Initialize(unsigned int element_numb, // Initialize function
                        short int r_int_order,
                        short int s_int_order,
                        short int t_int_order,
                        unsigned int node_numb_1,
                        unsigned int node_numb_2,
                        unsigned int node_numb_3,
                        unsigned int node_numb_4,
                        unsigned int node_numb_5,
                        unsigned int node_numb_6,
                        unsigned int node_numb_7,
                        unsigned int node_numb_8,
                        unsigned int node_numb_9,
                        unsigned int node_numb_10,
                        unsigned int node_numb_11,
                        unsigned int node_numb_12,
                        unsigned int node_numb_13,
                        unsigned int node_numb_14,
                        unsigned int node_numb_15,
                        unsigned int node_numb_16,
                        unsigned int node_numb_17,
                        unsigned int node_numb_18,
                        unsigned int node_numb_19,
                        unsigned int node_numb_20,
                        Node* GlobalNodes,
                        Material_Model* Globalmmodel,
                        tensor*    IN_tangent_E,
                        stresstensor* INstress,
                        stresstensor* INiterative_stress,
                        double*        INq_ast_iterative,
                        straintensor* INstrain
                       );

        void incremental_Update(void);
        void iterative_Update(void);

        tensor H_3D(double r1, double r2, double r3);
        tensor interp_poli_at(double r, double s, double t);
        tensor dh_drst_at(double r, double s, double t);


        //CE Dynamic Allocation for for brick3d s.
        Finite_Element* new_el( unsigned int total );
        Finite_Element& operator[](int subscript);
        Finite_Element& operator[](short subscript);
        Finite_Element& operator[](unsigned subscript);

        tensor stiffness_tensor(void);
        //matrix stiffness_tensor(void); //Zhaohui question why declared return typr is tensor while return matrix?

        void set_strain_stress_tensor(FILE* fp, double* u);
        tensor mass_tensor(void);
        //    tensor mass_tensor(Elastic  mmodel);
        double Potential_Energy(void);

        //    tensor mass_tensor(void);


        tensor Jacobian_3D(tensor dh);
        tensor Jacobian_3Dinv(tensor dh);
        tensor Nodal_Coordinates(void);

        tensor incr_disp(void);
        tensor total_disp(FILE* fp, double* u);

        tensor stiffness_matrix(const tensor& K);
        tensor mass_matrix(const tensor& M);


        unsigned int  get_global_number_of_node(unsigned int local_node_number);
        unsigned int  get_Brick_Number(void);


        int* get_LM(void);
        void set_LM(Node* node);



        // returns nodal forces for given stress field in an element
        tensor nodal_forces(void);
        // returns nodal forces for ITERATIVE stress field in an element
        tensor iterative_nodal_forces(void);
        // returns nodal forces for given constant stress field in the element
        tensor nodal_forces_from_stress(stresstensor&);
        // returns nodal forces for given incremental strain field in an element
        // by using the linearized constitutive tensor from the begining of the step !
        tensor linearized_nodal_forces(void);
        // updates Gauss point stresses and strains from given displacements
        tensor update_stress_strain(tensor& disp);

        double get_first_q_ast();
        double get_first_etacone();

        void report(char*);
        void reportshort(char*);
        void reportPAK(char*);
        void reportpqtheta(int);
        void reportLM(char*);
        void reportTensor(char*);
        void reportCIPIC(char*);
        void reportTensorF(FILE*);


};

#endif
