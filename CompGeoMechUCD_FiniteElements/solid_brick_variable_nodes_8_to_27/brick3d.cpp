///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              fe.cc
// CLASS:             Finite_Element
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
//
// DATE:              October 14 '94.
// UPDATE HISTORY:
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BRICK3D_CC
#define BRICK3D_CC

#include "brick3d.hh"
#include "nDarray/BJtensor.hh"
#include "nDarray/BJmatrix.hh"

// CONSTRUCTOR
// default constructor
// this one takes only one material model and forwards it to all
// Gauss points.
//#############################################################################
Brick3D::Brick3D(unsigned int element_number,
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
                 stresstensor*    INstress,
                 stresstensor*    INiterative_stress,
                 double*          IN_q_ast_iterative,
                 straintensor*    INstrain)
{
    elem_numb = element_number;
    determinant_of_Jacobian = 0.0;
    r_integration_order = r_int_order; // Gauss-Legendre integration order in r direction
    s_integration_order = s_int_order; // Gauss-Legendre integration order in s direction
    t_integration_order = t_int_order; // Gauss-Legendre integration order in t direction
    //    mmodel = Globalmmodel;
    int total_number_of_Gauss_points = r_integration_order *
                                       s_integration_order *
                                       t_integration_order;
    // according to ARM pp.61 default constructor will be called!!
    //    GaussPoint * GaussP = new GaussPoint[total_number_of_Gauss_points];
    //prebaci sve u jednodimenzioni niz jer samo prvi stepen pointera moze da se pokriva
    //sa onim stosom derived * ->> base * !!

    //    tensor Constitutive( 4, def_dim_4, 0.0);
    if ( total_number_of_Gauss_points != 0 )
    {
        GPmmodel = Globalmmodel->new_mm(total_number_of_Gauss_points);
        ////    GPmmodel = new Material_Model[total_number_of_Gauss_points];
        GPstress = new stresstensor[total_number_of_Gauss_points];

        GPiterative_stress = new stresstensor[total_number_of_Gauss_points];
        GPq_ast_iterative  = new double[total_number_of_Gauss_points];

        GPstrain = new straintensor[total_number_of_Gauss_points];

        GPtangent_E = new tensor[total_number_of_Gauss_points];//default constructor called

        GaussP  = new GaussPoint[total_number_of_Gauss_points];
    }
    else
    {
        GPmmodel = 0;
        GPstress = 0;

        GPiterative_stress = 0;
        GPq_ast_iterative  = 0;

        GPstrain = 0;
        GPtangent_E = 0;

        GaussP  = 0;
    }

    ////////////////////////////////////////////////////////////////////
    //dakle posto:
    //// according to ARM pp.61 default constructor will be called!!
    //onda oni vec postoje u memoriji i samo im treba dodeliti prave
    //vrednosti iz onog modela koji je prenesen unutra. Znaci onInitializxe funkcija
    //sa modelom kao argumentom Initialize(taj_Model).
    //za stresstensor i straintensor isto to napravi
    //tu Initialize funkciju pa da uzima argument i da samo
    //koristi njegove vrednosti pa stavi u vec postojece mesto
    //u memoriji ove vrednsoti. Takodje unutar brick3d napravi ih
    //( te stresstensor , straintensor i mmodel ) static da ostanu tu
    //da se ne pozove destructor . . .

    short where = 0;

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        double r = get_Gauss_p_c( r_integration_order, GP_c_r );
        double rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            double s = get_Gauss_p_c( s_integration_order, GP_c_s );
            double sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                double t = get_Gauss_p_c( t_integration_order, GP_c_t );
                double tw = get_Gauss_p_w( t_integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;

                //DB::printf("\n\nBefore Initialization **************** where = %d \n",where);
                //DB::printf("GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //DB            GP_c_r,GP_c_s,GP_c_t);
                //DB
                //DBGPstress[where].reportshort("stress within before Initialization");
                //DBGPstrain[where].reportshort("strain within before Initialization");
                //DB
                //DB// I suspect that [] should be overloaded so that compiler knows which
                //DB// material model is returning a pointer and fot the purpose
                //DB//GPmmodel[where].report("mmodel within before Initialization");
                //DB//GPmmodel[where].report("mmodel within before Initialization"); // operator[] overloaded
                //DB(GPmmodel)->operator[](where).report("mmodel within before Initialization"); // operator[] overloaded
                //DB                                                               // for Material_Model and
                //DB                                                               // derived types!


                GPtangent_E[where].Initialize_all(*IN_tangent_E);
                GPstress[where].Initialize(*INstress);
                GPiterative_stress[where].Initialize(*INiterative_stress);
                GPq_ast_iterative[where] = IN_q_ast_iterative[where];
                GPstrain[where].Initialize(*INstrain);
                (GPmmodel)->operator[](where).Initialize(*Globalmmodel);
                GaussP[where].Initialize(GP_c_r,
                                         GP_c_s,
                                         GP_c_t,
                                         r, s, t,
                                         rw, sw, tw,
                                         &( GPstress[where] ),
                                         &( GPiterative_stress[where] ),
                                         IN_q_ast_iterative[where] ,
                                         &( GPstrain[where] ),
                                         &( GPtangent_E[where] ),
                                         &( (GPmmodel)->operator[](where) )
                                        );      // ugly syntax but it works!
                // still don't know what's wrong
            }                 // with the old style GPmmodel[where]
        }
    }

    G_N_numbs[0] = node_numb_1;
    G_N_numbs[1] = node_numb_2;
    G_N_numbs[2] = node_numb_3;
    G_N_numbs[3] = node_numb_4;
    G_N_numbs[4] = node_numb_5;
    G_N_numbs[5] = node_numb_6;
    G_N_numbs[6] = node_numb_7;
    G_N_numbs[7] = node_numb_8;
    G_N_numbs[8] = node_numb_9;
    G_N_numbs[9] = node_numb_10;
    G_N_numbs[10] = node_numb_11;
    G_N_numbs[11] = node_numb_12;
    G_N_numbs[12] = node_numb_13;
    G_N_numbs[13] = node_numb_14;
    G_N_numbs[14] = node_numb_15;
    G_N_numbs[15] = node_numb_16;
    G_N_numbs[16] = node_numb_17;
    G_N_numbs[17] = node_numb_18;
    G_N_numbs[18] = node_numb_19;
    G_N_numbs[19] = node_numb_20;

    nodes = GlobalNodes;

    // loop nodes 9-20 and :
    //  1) put the right coordinates on place,
    //  2) calculate the total number of nodes
    nodes_in_brick = 8;

    for ( int i = 8 ; i < 20 ; i++ )
    {
        if (G_N_numbs[i] == 0 )
        {
            node_existance[i - 8] = 0;
        }
        else
        {
            nodes_in_brick++;
            node_existance[i - 8] = 1;
        }
    }

    //    Node * N = new Node[nodes_in_brick];

}

// Initialize
// default constructor
// this one takes only one material model and forwards it to all
// Gauss points.
//#############################################################################
void Brick3D::Initialize(unsigned int element_number,
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
                         stresstensor*    INstress,
                         stresstensor*    INiterative_stress,
                         double*          IN_q_ast_iterative,
                         straintensor*    INstrain)
{
    elem_numb = element_number;

    r_integration_order = r_int_order; // Gauss-Legendre integration order in r direction
    s_integration_order = s_int_order; // Gauss-Legendre integration order in s direction
    t_integration_order = t_int_order; // Gauss-Legendre integration order in t direction

    int total_number_of_Gauss_points = r_integration_order *
                                       s_integration_order *
                                       t_integration_order;

    GPmmodel = Globalmmodel->new_mm(total_number_of_Gauss_points);
    ////GPmmodel = new Material_Model[total_number_of_Gauss_points];
    GPstress = new stresstensor[total_number_of_Gauss_points];
    GPiterative_stress = new stresstensor[total_number_of_Gauss_points];
    GPq_ast_iterative  = new double[total_number_of_Gauss_points];

    GPstrain = new straintensor[total_number_of_Gauss_points];

    GPtangent_E = new tensor[total_number_of_Gauss_points];

    GaussP  = new GaussPoint[total_number_of_Gauss_points];

    ////////////////////////////////////////////////////////////////////
    //dakle posto:
    //// according to ARM pp.61 default constructor will be called!!
    //onda oni vec postoje u memoriji i samo im treba dodeliti prave
    //vrednosti iz onog modela koji je prenesen unutra. Znaci onInitializxe funkcija
    //sa modelom kao argumentom Initialize(taj_Model).
    //za stresstensor i straintensor isto to napravi
    //tu Initialize funkciju pa da uzima argument i da samo
    //koristi njegove vrednosti pa stavi u vec postojece mesto
    //u memoriji ove vrednsoti. Takodje unutar brick3d napravi ih
    //( te stresstensor , straintensor i mmodel ) static da ostanu tu
    //da se ne pozove destructor . . .

    short where = 0;

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        double r = get_Gauss_p_c( r_integration_order, GP_c_r );
        double rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            double s = get_Gauss_p_c( s_integration_order, GP_c_s );
            double sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                double t = get_Gauss_p_c( t_integration_order, GP_c_t );
                double tw = get_Gauss_p_w( t_integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;

                //DB::printf("\n\nBefore Initialization **************** where = %d \n",where);
                //DB::printf("GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //DB            GP_c_r,GP_c_s,GP_c_t);
                //DB
                //DBGPstress[where].reportshort("stress within before Initialization");
                //DBGPstrain[where].reportshort("strain within before Initialization");
                //DB
                //DB// I suspect that [] should be overloaded so that compiler knows which
                //DB// material model is returning a pointer and fot the purpose
                //DB//GPmmodel[where].report("mmodel within before Initialization");
                //DB//GPmmodel[where].report("mmodel within before Initialization"); // operator[] overloaded
                //DB(GPmmodel)->operator[](where).report("mmodel within before Initialization"); // operator[] overloaded
                //DB                                                               // for Material_Model and
                //DB                                                               // derived types!

                //.GaussP[where].Initialize(GP_c_r,
                //.                         GP_c_s,
                //.                         GP_c_t,
                //.                         r, s, t,
                //.                         rw, sw, tw,
                //.                         &( GPstress[where] ),
                //.                         &( GPstrain[where] ),
                //.                         &( (GPmmodel)->operator[](where) )
                //.                        );      // ugly syntax but it works!
                //.                                // still don't know what's wrong
                //.                                // with the old style GPmmodel[where]
                // Initialize it with the elastic stiffness tensor at the begining
                //````double Ey = (GPmmodel)->operator[](where).E();
                //````double nu = (GPmmodel)->operator[](where).nu();
                //````tensor Constitutive = (GPmmodel)->operator[](where).StiffnessTensorE(Ey,nu);
                //````
                //````GPtangent_E[where].Initialize_all(Constitutive);
                GPtangent_E[where].Initialize_all(*IN_tangent_E);

                GPstress[where].Initialize(*INstress);

                GPiterative_stress[where].Initialize(*INiterative_stress);
                GPq_ast_iterative[where] = IN_q_ast_iterative[where];

                GPstrain[where].Initialize(*INstrain);
                (GPmmodel)->operator[](where).Initialize(*Globalmmodel);
                GaussP[where].Initialize(GP_c_r,
                                         GP_c_s,
                                         GP_c_t,
                                         r, s, t,
                                         rw, sw, tw,
                                         &( GPstress[where] ),
                                         &( GPiterative_stress[where] ),
                                         IN_q_ast_iterative[where],
                                         &( GPstrain[where] ),
                                         &( GPtangent_E[where] ),
                                         &( (GPmmodel)->operator[](where) )
                                        );      // ugly syntax but it works!
                // still don't know what's wrong
            }                 // with the old style GPmmodel[where]
        }
    }

    G_N_numbs[0] = node_numb_1;
    G_N_numbs[1] = node_numb_2;
    G_N_numbs[2] = node_numb_3;
    G_N_numbs[3] = node_numb_4;
    G_N_numbs[4] = node_numb_5;
    G_N_numbs[5] = node_numb_6;
    G_N_numbs[6] = node_numb_7;
    G_N_numbs[7] = node_numb_8;
    G_N_numbs[8] = node_numb_9;
    G_N_numbs[9] = node_numb_10;
    G_N_numbs[10] = node_numb_11;
    G_N_numbs[11] = node_numb_12;
    G_N_numbs[12] = node_numb_13;
    G_N_numbs[13] = node_numb_14;
    G_N_numbs[14] = node_numb_15;
    G_N_numbs[15] = node_numb_16;
    G_N_numbs[16] = node_numb_17;
    G_N_numbs[17] = node_numb_18;
    G_N_numbs[18] = node_numb_19;
    G_N_numbs[19] = node_numb_20;

    nodes = GlobalNodes;

    // loop nodes 9-20 and :
    //  1) put the right coordinates on place,
    //  2) calculate the total number of nodes
    // loop nodes 9-20 and :
    //  1) put the right coordinates on place,
    //  2) calculate the total number of nodes
    nodes_in_brick = 8;

    for ( int i = 8 ; i < 20 ; i++ )
    {
        if (G_N_numbs[i] == 0 )
        {
            node_existance[i - 8] = 0;
        }
        else
        {
            nodes_in_brick++;
            node_existance[i - 8] = 1;
        }
    }

    //    Node * N = new Node[nodes_in_brick];

}

//#############################################################################
void Brick3D::incremental_Update()
{
    double r  = 0.0;
    //    double rw = 0.0;
    double s  = 0.0;
    //    double sw = 0.0;
    double t  = 0.0;
    //    double tw = 0.0;

    short where = 0;
    //,,,,,    double weight = 0.0;
    double this_one_PP = (GPmmodel)->operator[](where).IS_Perfect_Plastic();

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    tensor Constitutive( 4, def_dim_4, 0.0);

    //    double det_of_Jacobian = 0.0;

    static int disp_dim[] = {20, 3};
    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;
    straintensor total_strain_at_GP;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    //....    int number_of_subincrements = 1;
    //....    double this_one_PP = 1.0; // if set to 0.0 -> perfectly plastic
    //....                              // if set to 1.0 -> elasto plastic
    stresstensor final_stress_after_integration;
    ///    stresstensor incremental_stress;
    // tensor of incremental displacements taken from node objects
    incremental_displacements = incr_disp();

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );

        //--        rw = get_Gauss_p_w( r_integration_order, GP_c_r );
        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );

            //--            sw = get_Gauss_p_w( s_integration_order, GP_c_s );
            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                //--                tw = get_Gauss_p_w( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                JacobianINV = Jacobian.inverse();
                //....                JacobianINV.print("JINV");
                // determinant of Jacobian tensor ( matrix )
                //--                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");
                //....                dhGlobal.print("dh","dhGlobal");
                //weight
                //                weight = rw * sw * tw * det_of_Jacobian;
                //::::::   ::printf("\n\nIN THE STIFFNESS TENSOR INTEGRATOR ----**************** where = %d \n", where);
                //::::::   ::printf(" void Brick3D::incremental_Update()\n");
                //::::::   ::printf(" GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d    --->>>  where = %d \n",
                //::::::                      GP_c_r,GP_c_s,GP_c_t,where);
                //::::::   ::printf("WEIGHT = %f", weight);
                //::::::   ::printf("determinant of Jacobian = %f", determinant_of_Jacobian);
                //::::::   GaussP[where].report("Gauss Point\n");
                // incremental straines at this Gauss point
                // now in Update we know the incremental displacements so let's find
                // the incremental strain
                incremental_strain =
                    (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();
                //incremental_strain.reportshort("\n incremental_strain tensor at GAUSS point\n");

                // here comes the final_stress calculation actually on only needs to copy stresses
                // from the iterative data . . .
                //(GPstress+where)->reportshortpqtheta("\n stress START GAUSS \n");
                final_stress_after_integration = GPiterative_stress[where];
                (GPmmodel)->operator[](where).kappa_set(final_stress_after_integration,
                                                        GPq_ast_iterative[where]);
                //....         final_stress_after_integration =
                //....           (GPmmodel)->operator[](where).FinalStress(*(GPstress+where),
                //....                                                     incremental_strain,
                //....                                                     (GPmmodel)->operator[](where),
                //....                                                     number_of_subincrements,
                //....                                                     this_one_PP);
                //....//final_stress_after_integration.reportshortpqtheta("\n final_stress_after_integration GAUSS \n");
                // calculate the constitutive tensor
                Constitutive =
                    (GPmmodel)->operator[](where).ConstitutiveTensor(final_stress_after_integration,
                            *(GPstress + where),
                            incremental_strain,
                            (GPmmodel)->operator[](where),
                            this_one_PP);
                //::::::                   Constitutive.print("C","\n\n C tensor \n");
                // this is update of constitutive tensor at this Gauss point
                GPtangent_E[where].Initialize(Constitutive);
                //GPtangent_E[where].print("\n tangent E at GAUSS point \n");

                total_strain_at_GP.Initialize(*(GPstrain + where));
                //total_strain_at_GP.reportshort("\n total_strain tensor at GAUSS point \n");
                total_strain_at_GP = total_strain_at_GP + incremental_strain;
                //total_strain_at_GP.reportshort("\n total_strain tensor at GAUSS point AFTER\n");
                GPstress[where].Initialize(final_stress_after_integration);
                //GPstress[where].reportshortpqtheta("\n stress at GAUSS point \n");
                GPstrain[where].Initialize(total_strain_at_GP);
                //GPstrain[where].reportshort("\n strain at GAUSS point \n");
            }
        }
    }
}

//#############################################################################
// update iterative_stresses during iterative process
// so that function iterative_nodal_forces can return right internal forces
// (iterative in this case ) !!!!
//#############################################################################
void Brick3D::iterative_Update()
{
    double r  = 0.0;
    //    double rw = 0.0;
    double s  = 0.0;
    //    double sw = 0.0;
    double t  = 0.0;
    //    double tw = 0.0;

    short where = 0;
    //,,,,,    double weight = 0.0;

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    //    tensor Constitutive( 4, def_dim_4, 0.0);

    //    double det_of_Jacobian = 0.0;

    static int disp_dim[] = {20, 3};
    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;
    //    straintensor total_strain_at_GP;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    int number_of_subincrements = 1;
    double this_one_PP = (GPmmodel)->operator[](where).IS_Perfect_Plastic();

    stresstensor final_stress_after_integration;
    //    stresstensor incremental_stress;
    // tensor of incremental displacements taken from node objects for this element !
    incremental_displacements = incr_disp();
    //incremental_displacements.print("disp","\n incremental_displacements tensor at GAUSS point in iterative_Update\n");

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );

        //--        rw = get_Gauss_p_w( r_integration_order, GP_c_r );
        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );

            //--            sw = get_Gauss_p_w( s_integration_order, GP_c_s );
            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                //--                tw = get_Gauss_p_w( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //Jacobian.print("J");
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //JacobianINV.print("JINV");
                // determinant of Jacobian tensor ( matrix )
                //--                det_of_Jacobian  = Jacobian.determinant();
                //::printf("determinant of Jacobian is %f\n",Jacobian_determinant );
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");
                //....                dhGlobal.print("dh","dhGlobal");
                //weight
                //                weight = rw * sw * tw * det_of_Jacobian;
                //::::::   ::printf(" void Brick3D::iterative_Update() \n ");
                //::::::   ::printf(" GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d    --->>>  where = %d \n",
                //::::::                      GP_c_r,GP_c_s,GP_c_t,where);
                //::::::   ::printf("WEIGHT = %f", weight);
                //::::::   ::printf("determinant of Jacobian = %f", determinant_of_Jacobian);
                //::::::   GaussP[where].report("Gauss Point\n");
                incremental_strain =
                    (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();
                incremental_strain.reportshort("incremental_strain in void Brick3D::iterative_Update()");

                //..   dakle ovde posalji strain_increment jer se stari stress cuva u okviru svake
                //..   Gauss tacke a samo saljes strain_increment koji ce da se prenese
                //..   u integracionu rutinu pa ce ta da vrati krajnji napon i onda moze da
                //..   se pravi ConstitutiveStiffnessTensor.
                // here comes the final_stress calculation
                //(GPstress+where)->reportshortpqtheta("\n stress START GAUSS  in iterative_Update\n");
                final_stress_after_integration =
                    (GPmmodel)->operator[](where).FinalStress(*(GPstress + where),
                            incremental_strain,
                            (GPmmodel)->operator[](where),
                            number_of_subincrements,
                            this_one_PP);
                final_stress_after_integration.reportshort("\n final_stress_after_integration in void Brick3D::iterative_Update()\n");
                //----// intialize total strain with the strain at this Gauss point before
                //----// adding this increments strains!
                //----                total_strain_at_GP.Initialize(*(GPstrain+where));
                //----//total_strain_at_GP.reportshort("\n total_strain tensor at GAUSS point \n");
                //----// this is the addition of incremental strains to the previous strain state at
                //----// this Gauss point
                //----                total_strain_at_GP = total_strain_at_GP + incremental_strain;
                //----//total_strain_at_GP.reportshort("\n total_strain tensor at GAUSS point AFTER\n");
                // calculate the constitutive tensor
                // this is update of iterative_stress state at this Gauss point
                GPiterative_stress[where].Initialize(final_stress_after_integration);
                GPq_ast_iterative[where] =
                    (GPmmodel)->operator[](where).kappa_get(final_stress_after_integration);
                //             GPstress[where].Initialize(final_stress_after_integration);
                //GPiterative_stress[where].reportshortpqtheta("\n iterative_stress at GAUSS point in iterative_Update\n");
                GPiterative_stress[where].reportshort("\n iterative_stress at GAUSS point in iterative_Update\n");
                //----    // this is update of strain state at this Gauss point
                //----                GPstrain[where].Initialize(total_strain_at_GP);
                //GPstrain[where].reportshort("\n strain at GAUSS point \n");
            }
        }
    }
}




//#############################################################################
//#############################################################################
//***************************************************************
tensor Brick3D::H_3D(double r1, double r2, double r3)
{

    int dimension[] = {60, 3};
    tensor H(2, dimension, 0.0);

    // influence of the node number 20
    H.val(58, 1) = node_existance[20 - 1 - 8] * (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    H.val(59, 2) = node_existance[20 - 1 - 8] * (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    H.val(60, 3) = node_existance[20 - 1 - 8] * (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    // influence of the node number 19
    H.val(55, 1) = node_existance[19 - 1 - 8] * (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    H.val(56, 2) = node_existance[19 - 1 - 8] * (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    H.val(57, 3) = node_existance[19 - 1 - 8] * (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    // influence of the node number 18
    H.val(52, 1) = node_existance[18 - 1 - 8] * (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;
    H.val(53, 2) = node_existance[18 - 1 - 8] * (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;
    H.val(54, 3) = node_existance[18 - 1 - 8] * (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;
    // influence of the node number 17
    H.val(49, 1) = node_existance[17 - 1 - 8] * (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;
    H.val(50, 2) = node_existance[17 - 1 - 8] * (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;
    H.val(51, 3) = node_existance[17 - 1 - 8] * (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;

    // influence of the node number 16
    H.val(46, 1) = node_existance[16 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    H.val(47, 2) = node_existance[16 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    H.val(48, 3) = node_existance[16 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    // influence of the node number 15
    H.val(43, 1) = node_existance[15 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) / 4.0;
    H.val(44, 2) = node_existance[15 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) / 4.0;
    H.val(45, 3) = node_existance[15 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) / 4.0;
    // influence of the node number 14
    H.val(40, 1) = node_existance[14 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    H.val(41, 2) = node_existance[14 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    H.val(42, 3) = node_existance[14 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    // influence of the node number 13
    H.val(37, 1) = node_existance[13 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) / 4.0;
    H.val(38, 2) = node_existance[13 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) / 4.0;
    H.val(39, 3) = node_existance[13 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) / 4.0;

    // influence of the node number 12
    H.val(34, 1) = node_existance[12 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    H.val(35, 2) = node_existance[12 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    H.val(36, 3) = node_existance[12 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    // influence of the node number 11
    H.val(31, 1) = node_existance[11 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) / 4.0;
    H.val(32, 2) = node_existance[11 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) / 4.0;
    H.val(33, 3) = node_existance[11 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) / 4.0;
    // influence of the node number 10
    H.val(28, 1) = node_existance[10 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    H.val(29, 2) = node_existance[10 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    H.val(30, 3) = node_existance[10 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    // influence of the node number 9
    H.val(25, 1) = node_existance[9 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) / 4.0;
    H.val(26, 2) = node_existance[9 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) / 4.0;
    H.val(27, 3) = node_existance[9 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) / 4.0;

    // influence of the node number 8
    //    H.val(22,1)=(1.0+r1)*(1.0-r2)*(1.0-r3)/8.0 - (H.val(15)+H.val(16)+H.val(20))/2.0;
    //    H.val(23,2)=(1.0+r1)*(1.0-r2)*(1.0-r3)/8.0 - (H.val(15)+H.val(16)+H.val(20))/2.0;
    //    H.val(24,3)=(1.0+r1)*(1.0-r2)*(1.0-r3)/8.0 - (H.val(15)+H.val(16)+H.val(20))/2.0;
    H.val(22, 1) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) / 8.0 - (H.val(43, 1) + H.val(48, 3) + H.val(60, 3)) / 2.0;
    H.val(23, 2) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) / 8.0 - (H.val(43, 1) + H.val(48, 3) + H.val(60, 3)) / 2.0;
    H.val(24, 3) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) / 8.0 - (H.val(43, 1) + H.val(48, 3) + H.val(60, 3)) / 2.0;
    // influence of the node number 7
    H.val(19, 1) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) / 8.0 - (H.val(42, 3) + H.val(43, 1) + H.val(57, 3)) / 2.0;
    H.val(20, 2) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) / 8.0 - (H.val(42, 3) + H.val(43, 1) + H.val(57, 3)) / 2.0;
    H.val(21, 3) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) / 8.0 - (H.val(42, 3) + H.val(43, 1) + H.val(57, 3)) / 2.0;
    // influence of the node number 6
    H.val(16, 1) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) / 8.0 - (H.val(39, 3) + H.val(42, 3) + H.val(54, 3)) / 2.0;
    H.val(17, 2) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) / 8.0 - (H.val(39, 3) + H.val(42, 3) + H.val(54, 3)) / 2.0;
    H.val(18, 3) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) / 8.0 - (H.val(39, 3) + H.val(42, 3) + H.val(54, 3)) / 2.0;
    // influence of the node number 5
    H.val(13, 1) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) / 8.0 - (H.val(39, 3) + H.val(48, 3) + H.val(51, 3)) / 2.0;
    H.val(14, 2) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) / 8.0 - (H.val(39, 3) + H.val(48, 3) + H.val(51, 3)) / 2.0;
    H.val(15, 3) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) / 8.0 - (H.val(39, 3) + H.val(48, 3) + H.val(51, 3)) / 2.0;

    // influence of the node number 4
    H.val(10, 1) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) / 8.0 - (H.val(33, 3) + H.val(36, 3) + H.val(60, 3)) / 2.0;
    H.val(11, 2) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) / 8.0 - (H.val(33, 3) + H.val(36, 3) + H.val(60, 3)) / 2.0;
    H.val(12, 3) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) / 8.0 - (H.val(33, 3) + H.val(36, 3) + H.val(60, 3)) / 2.0;
    // influence of the node number 3
    H.val(7, 1) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) / 8.0 - (H.val(30, 3) + H.val(33, 3) + H.val(57, 3)) / 2.0;
    H.val(8, 2) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) / 8.0 - (H.val(30, 3) + H.val(33, 3) + H.val(57, 3)) / 2.0;
    H.val(9, 3) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) / 8.0 - (H.val(30, 3) + H.val(33, 3) + H.val(57, 3)) / 2.0;
    // influence of the node number 2
    H.val(4, 1) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) / 8.0 - (H.val(30, 3) + H.val(54, 3) + H.val(27, 3)) / 2.0;
    H.val(5, 2) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) / 8.0 - (H.val(30, 3) + H.val(54, 3) + H.val(27, 3)) / 2.0;
    H.val(6, 3) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) / 8.0 - (H.val(30, 3) + H.val(54, 3) + H.val(27, 3)) / 2.0;
    // influence of the node number 1
    H.val(1, 1) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) / 8.0 - (H.val(36, 3) + H.val(51, 3) + H.val(27, 3)) / 2.0;
    H.val(2, 2) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) / 8.0 - (H.val(36, 3) + H.val(51, 3) + H.val(27, 3)) / 2.0;
    H.val(3, 3) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) / 8.0 - (H.val(36, 3) + H.val(51, 3) + H.val(27, 3)) / 2.0;

    //         double sum = 0;
    //
    //  for (int i=1; i<=60 ; i++)
    //           {
    // //       sum+=H.cval(i,1);
    //      for (int j=1; j<= 1; j++)
    //         {
    //                    sum+=H.cval(i,1);
    //            ::printf( "  %+9.2e", H.cval(i,j) );
    //          }
    //            // ::printf( "  %d \n", i);
    //     }
    //      ::printf( " \n sum= %+6.2e\n", sum );


    //    printf("r1 = %lf, r2 = %lf, r3 = %lf\n", r1, r2, r3);
    //    H.print("h");

    return H;
}

//#############################################################################
//***************************************************************
tensor Brick3D::interp_poli_at(double r1, double r2, double r3)
{

    int dimension[] = {20};
    tensor h(1, dimension, 0.0);

    // influence of the node number 20
    h.val(20) = node_existance[20 - 1 - 8] * (1.0 + r1) * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    // influence of the node number 19
    h.val(19) = node_existance[19 - 1 - 8] * (1.0 - r1) * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    // influence of the node number 18
    h.val(18) = node_existance[18 - 1 - 8] * (1.0 - r1) * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;
    // influence of the node number 17
    h.val(17) = node_existance[17 - 1 - 8] * (1.0 + r1) * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;

    // influence of the node number 16
    h.val(16) = node_existance[16 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    // influence of the node number 15
    h.val(15) = node_existance[15 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 - r3) / 4.0;
    // influence of the node number 14
    h.val(14) = node_existance[14 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    // influence of the node number 13
    h.val(13) = node_existance[13 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 - r3) / 4.0;

    // influence of the node number 12
    h.val(12) = node_existance[12 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    // influence of the node number 11
    h.val(11) = node_existance[11 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) * (1.0 + r3) / 4.0;
    // influence of the node number 10
    h.val(10) = node_existance[10 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    // influence of the node number 9
    h.val(9) = node_existance[9 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) * (1.0 + r3) / 4.0;

    // influence of the node number 8
    h.val(8) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) / 8.0 - (h.val(15) + h.val(16) + h.val(20)) / 2.0;
    // influence of the node number 7
    h.val(7) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) / 8.0 - (h.val(14) + h.val(15) + h.val(19)) / 2.0;
    // influence of the node number 6
    h.val(6) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) / 8.0 - (h.val(13) + h.val(14) + h.val(18)) / 2.0;
    // influence of the node number 5
    h.val(5) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) / 8.0 - (h.val(13) + h.val(16) + h.val(17)) / 2.0;

    // influence of the node number 4
    h.val(4) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) / 8.0 - (h.val(11) + h.val(12) + h.val(20)) / 2.0;
    // influence of the node number 3
    h.val(3) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) / 8.0 - (h.val(10) + h.val(11) + h.val(19)) / 2.0;
    // influence of the node number 2
    h.val(2) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) / 8.0 - (h.val(10) + h.val(18) + h.val(9)) / 2.0;
    // influence of the node number 1
    h.val(1) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) / 8.0 - (h.val(12) + h.val(17) + h.val(9)) / 2.0;


    //    printf("r1 = %lf, r2 = %lf, r3 = %lf\n", r1, r2, r3);
    //    h.print("h");

    return h;
}



tensor Brick3D::dh_drst_at(double r1, double r2, double r3)
{

    int dimensions[] = {20, 3};
    tensor dh(2, dimensions, 0.0);

    // influence of the node number 20
    dh.val(20, 1) =   node_existance[20 - 1 - 8] * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    dh.val(20, 2) = - node_existance[20 - 1 - 8] * (1.0 + r1) * (1.0 - r3 * r3) / 4.0;
    dh.val(20, 3) = - node_existance[20 - 1 - 8] * (1.0 + r1) * (1.0 - r2) * r3 / 2.0;
    // influence of the node number 19
    dh.val(19, 1) = - node_existance[19 - 1 - 8] * (1.0 - r2) * (1.0 - r3 * r3) / 4.0;
    dh.val(19, 2) = - node_existance[19 - 1 - 8] * (1.0 - r1) * (1.0 - r3 * r3) / 4.0;
    dh.val(19, 3) = - node_existance[19 - 1 - 8] * (1.0 - r1) * (1.0 - r2) * r3 / 2.0;
    // influence of the node number 18
    dh.val(18, 1) = - node_existance[18 - 1 - 8] * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;
    dh.val(18, 2) =   node_existance[18 - 1 - 8] * (1.0 - r1) * (1.0 - r3 * r3) / 4.0;
    dh.val(18, 3) = - node_existance[18 - 1 - 8] * (1.0 - r1) * (1.0 + r2) * r3 / 2.0;
    // influence of the node number 17
    dh.val(17, 1) =   node_existance[17 - 1 - 8] * (1.0 + r2) * (1.0 - r3 * r3) / 4.0;
    dh.val(17, 2) =   node_existance[17 - 1 - 8] * (1.0 + r1) * (1.0 - r3 * r3) / 4.0;
    dh.val(17, 3) = - node_existance[17 - 1 - 8] * (1.0 + r1) * (1.0 + r2) * r3 / 2.0;

    // influence of the node number 16
    dh.val(16, 1) =   node_existance[16 - 1 - 8] * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    dh.val(16, 2) = - node_existance[16 - 1 - 8] * (1.0 + r1) * r2 * (1.0 - r3) / 2.0;
    dh.val(16, 3) = - node_existance[16 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) / 4.0;
    // influnce of the node number 15
    dh.val(15, 1) = - node_existance[15 - 1 - 8] * r1 * (1.0 - r2) * (1.0 - r3) / 2.0;
    dh.val(15, 2) = - node_existance[15 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r3) / 4.0;
    dh.val(15, 3) = - node_existance[15 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) / 4.0;
    // influence of the node number 14
    dh.val(14, 1) = - node_existance[14 - 1 - 8] * (1.0 - r2 * r2) * (1.0 - r3) / 4.0;
    dh.val(14, 2) = - node_existance[14 - 1 - 8] * (1.0 - r1) * r2 * (1.0 - r3) / 2.0;
    dh.val(14, 3) = - node_existance[14 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) / 4.0;
    // influence of the node number 13
    dh.val(13, 1) = - node_existance[13 - 1 - 8] * r1 * (1.0 + r2) * (1.0 - r3) / 2.0;
    dh.val(13, 2) =   node_existance[13 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r3) / 4.0;
    dh.val(13, 3) = - node_existance[13 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) / 4.0;

    // influence of the node number 12
    dh.val(12, 1) =   node_existance[12 - 1 - 8] * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    dh.val(12, 2) = - node_existance[12 - 1 - 8] * (1.0 + r1) * r2 * (1.0 + r3) / 2.0;
    dh.val(12, 3) =   node_existance[12 - 1 - 8] * (1.0 + r1) * (1.0 - r2 * r2) / 4.0;
    // influence of the node number 11
    dh.val(11, 1) = - node_existance[11 - 1 - 8] * r1 * (1.0 - r2) * (1.0 + r3) / 2.0;
    dh.val(11, 2) = - node_existance[11 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r3) / 4.0; // bug discovered 01 aug '95 2.0 -> 4.0
    dh.val(11, 3) =   node_existance[11 - 1 - 8] * (1.0 - r1 * r1) * (1.0 - r2) / 4.0;
    // influence of the node number 10
    dh.val(10, 1) = - node_existance[10 - 1 - 8] * (1.0 - r2 * r2) * (1.0 + r3) / 4.0;
    dh.val(10, 2) = - node_existance[10 - 1 - 8] * (1.0 - r1) * r2 * (1.0 + r3) / 2.0;
    dh.val(10, 3) =   node_existance[10 - 1 - 8] * (1.0 - r1) * (1.0 - r2 * r2) / 4.0;
    // influence of the node number 9
    dh.val(9, 1) = - node_existance[9 - 1 - 8] * r1 * (1.0 + r2) * (1.0 + r3) / 2.0;
    dh.val(9, 2) =   node_existance[9 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r3) / 4.0;
    dh.val(9, 3) =   node_existance[9 - 1 - 8] * (1.0 - r1 * r1) * (1.0 + r2) / 4.0;


    // influence of the node number 8
    dh.val(8, 1) = (1.0 - r2) * (1.0 - r3) / 8.0 - (dh.val(15, 1) + dh.val(16, 1) + dh.val(20, 1)) / 2.0;
    dh.val(8, 2) = -(1.0 + r1) * (1.0 - r3) / 8.0 - (dh.val(15, 2) + dh.val(16, 2) + dh.val(20, 2)) / 2.0;
    dh.val(8, 3) = -(1.0 + r1) * (1.0 - r2) / 8.0 - (dh.val(15, 3) + dh.val(16, 3) + dh.val(20, 3)) / 2.0;
    // influence of the node number 7
    dh.val(7, 1) = -(1.0 - r2) * (1.0 - r3) / 8.0 - (dh.val(14, 1) + dh.val(15, 1) + dh.val(19, 1)) / 2.0;
    dh.val(7, 2) = -(1.0 - r1) * (1.0 - r3) / 8.0 - (dh.val(14, 2) + dh.val(15, 2) + dh.val(19, 2)) / 2.0;
    dh.val(7, 3) = -(1.0 - r1) * (1.0 - r2) / 8.0 - (dh.val(14, 3) + dh.val(15, 3) + dh.val(19, 3)) / 2.0;
    // influence of the node number 6
    dh.val(6, 1) = -(1.0 + r2) * (1.0 - r3) / 8.0 - (dh.val(13, 1) + dh.val(14, 1) + dh.val(18, 1)) / 2.0;
    dh.val(6, 2) = (1.0 - r1) * (1.0 - r3) / 8.0 - (dh.val(13, 2) + dh.val(14, 2) + dh.val(18, 2)) / 2.0;
    dh.val(6, 3) = -(1.0 - r1) * (1.0 + r2) / 8.0 - (dh.val(13, 3) + dh.val(14, 3) + dh.val(18, 3)) / 2.0;
    // influence of the node number 5
    dh.val(5, 1) = (1.0 + r2) * (1.0 - r3) / 8.0 - (dh.val(13, 1) + dh.val(16, 1) + dh.val(17, 1)) / 2.0;
    dh.val(5, 2) = (1.0 + r1) * (1.0 - r3) / 8.0 - (dh.val(13, 2) + dh.val(16, 2) + dh.val(17, 2)) / 2.0;
    dh.val(5, 3) = -(1.0 + r1) * (1.0 + r2) / 8.0 - (dh.val(13, 3) + dh.val(16, 3) + dh.val(17, 3)) / 2.0;

    // influence of the node number 4
    dh.val(4, 1) = (1.0 - r2) * (1.0 + r3) / 8.0 - (dh.val(11, 1) + dh.val(12, 1) + dh.val(20, 1)) / 2.0;
    dh.val(4, 2) = -(1.0 + r1) * (1.0 + r3) / 8.0 - (dh.val(11, 2) + dh.val(12, 2) + dh.val(20, 2)) / 2.0;
    dh.val(4, 3) = (1.0 + r1) * (1.0 - r2) / 8.0 - (dh.val(11, 3) + dh.val(12, 3) + dh.val(20, 3)) / 2.0;
    // influence of the node number 3
    dh.val(3, 1) = -(1.0 - r2) * (1.0 + r3) / 8.0 - (dh.val(10, 1) + dh.val(11, 1) + dh.val(19, 1)) / 2.0;
    dh.val(3, 2) = -(1.0 - r1) * (1.0 + r3) / 8.0 - (dh.val(10, 2) + dh.val(11, 2) + dh.val(19, 2)) / 2.0;
    dh.val(3, 3) = (1.0 - r1) * (1.0 - r2) / 8.0 - (dh.val(10, 3) + dh.val(11, 3) + dh.val(19, 3)) / 2.0;
    // influence of the node number 2
    dh.val(2, 1) = -(1.0 + r2) * (1.0 + r3) / 8.0 - (dh.val(10, 1) + dh.val(18, 1) + dh.val(9, 1)) / 2.0;
    dh.val(2, 2) = (1.0 - r1) * (1.0 + r3) / 8.0 - (dh.val(10, 2) + dh.val(18, 2) + dh.val(9, 2)) / 2.0;
    dh.val(2, 3) = (1.0 - r1) * (1.0 + r2) / 8.0 - (dh.val(10, 3) + dh.val(18, 3) + dh.val(9, 3)) / 2.0;
    // influence of the node number 1
    dh.val(1, 1) = (1.0 + r2) * (1.0 + r3) / 8.0 - (dh.val(12, 1) + dh.val(17, 1) + dh.val(9, 1)) / 2.0;
    dh.val(1, 2) = (1.0 + r1) * (1.0 + r3) / 8.0 - (dh.val(12, 2) + dh.val(17, 2) + dh.val(9, 2)) / 2.0;
    dh.val(1, 3) = (1.0 + r1) * (1.0 + r2) / 8.0 - (dh.val(12, 3) + dh.val(17, 3) + dh.val(9, 3)) / 2.0;

    return dh;
}



//CE Dynamic Allocation for brick3d
////#############################################################################
Finite_Element* Brick3D::new_el(unsigned int total)
{
    Brick3D* el_p;
    el_p = ::new Brick3D[total];
    //DB//-------------------------------------------
    //DB    for ( int i=0 ; i<total ; i++ )
    //DB      {
    //DB        el_p[i].report("derived Brick3D\n");
    //DB      }
    //DB//-------------------------------------------
    return el_p;
}

////#############################################################################
Finite_Element& Brick3D::operator[](int subscript)
{
    return ( *(this + subscript) );
}
Finite_Element& Brick3D::operator[](short subscript)
{
    return ( *(this + subscript) );
}
Finite_Element& Brick3D::operator[](unsigned subscript)
{
    return ( *(this + subscript) );
}


////#############################################################################
tensor Brick3D::stiffness_tensor(void)
{
    int K_dim[] = {20, 3, 3, 20};
    tensor K(4, K_dim, 0.0);

    //debugging
    matrix Kmat;

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    //    tensor Constitutive( 4, def_dim_4, 0.0);
    tensor Constitutive;

    double det_of_Jacobian = 0.0;

    static int disp_dim[] = {20, 3};
    tensor incremental_displacements(2, disp_dim, 0.0); // \Delta u

    straintensor incremental_strain;
    straintensor total_strain_at_GP;

    tensor Jacobian;
    tensor JacobianINV;
    tensor JacobianINVtemp;
    tensor dhGlobal;

    int number_of_subincrements = 1;
    double this_one_PP = 1.0; // if set to 0.0 -> perfectly plastic
    // if set to 1.0 -> elasto plastic
    //    tensor Ktemp(4,K_dim,0.0);
    //    char * integr_type = 0;

    stresstensor final_stress_after_integration;
    stresstensor incremental_stress;
    // tensor of incremental displacements taken from node objects
    incremental_displacements = incr_disp();
    //..    incremental_displacements.print("incr_disp");

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );
        rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );
            sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                tw = get_Gauss_p_w( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                //dh.print("dh");
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                JacobianINVtemp = Jacobian.inverse();
                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");
                //        ::fprintf(stdout," # %d \n\n\n\n\n\n\n\n", El_count);
                //dhGlobal.print("dhGlobal");
                //weight
                weight = rw * sw * tw * det_of_Jacobian;
                //::::::
                //printf("\n\nIN THE STIFFNESS TENSOR INTEGRATOR ----**************** where = %d \n", where);
                //printf("  Stifness_Tensor \n");
                //printf("                    GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //                            GP_c_r,GP_c_s,GP_c_t);
                //printf("WEIGHT = %f", weight);
                //Jacobian.print("J");
                //JacobianINV.print("JINV");
                //JacobianINVtemp.print("JINVtemp");
                //tensor I = JacobianINV("ij")*Jacobian("jk");
                //I.print("I");

                //printf("determinant of Jacobian = %.6e", det_of_Jacobian);
                //GaussP[where].report("Gauss Point\n");

                // incremental straines at this Gauss point
                GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor1\n");
                incremental_strain =
                    (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();
                incremental_strain.report("\n incremental_strain tensor at GAUSS point\n");
                // incremental_strain.reportshort("\n incremental_strain tensor at GAUSS point\n");
                //----   GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor2\n");
                // intialize total strain with the strain at this Gauss point before
                // adding this increments strains!
                //                total_strain_at_GP.Initialize(*(GPstrain+where));
                //total_strain_at_GP.reportshort("\n total_strain tensor at GAUSS point BEFORE\n");
                // this is the addition of incremental strains to the previous strain state at
                // this Gauss point
                //                total_strain_at_GP = total_strain_at_GP + incremental_strain;
                //total_strain_at_GP.reportshort("\n total_strain tensor at GAUSS point AFTER\n");
                //..   dakle ovde posalji strain_increment jer se stari stress cuva u okviru svake
                //..   Gauss tacke a samo saljes strain_increment koji ce da se prenese
                //..   u integracionu rutinu pa ce ta da vrati krajnji napon i onda moze da
                //..   se pravi ConstitutiveStiffnessTensor.
                // here comes the final_stress calculation
                // this final stress after integration is used ONLY to obtain Constitutive tensor
                // at this Gauss point.
                final_stress_after_integration =
                    (GPmmodel)->operator[](where).FinalStress(*(GPstress + where),
                            incremental_strain,
                            (GPmmodel)->operator[](where),
                            number_of_subincrements,
                            this_one_PP);
                final_stress_after_integration.reportshortpqtheta("\n final_stress_after_integration in stiffness_tensor5\n");

                //----   GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor3\n");
                //final_stress_after_integration.reportshortpqtheta("\n final_stress_after_integration GAUSS \n");
                //----   GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor4\n");

                // this final stress after integration is used ONLY to obtain Constitutive tensor
                // at this Gauss point AND to set up the iterative_stress that is used in calculting
                // internal forces during iterations!!

                GPiterative_stress[where].Initialize(final_stress_after_integration);
                GPiterative_stress[where].reportshortpqtheta("\n iterative_stress at GAUSS point in stiffness_tensor5\n");


                // Stress state at Gauss point will be updated ( in the
                // sense of updating stresses ( integrating them ) ) in function Update (...) ! ! ! ! !
                // calculate the constitutive tensor
                //......         Constitutive =  GPtangent_E[where];
                Constitutive =  (GPmmodel)->operator[](where).ConstitutiveTensor(final_stress_after_integration,
                                *(GPstress + where),
                                incremental_strain,
                                (GPmmodel)->operator[](where),
                                this_one_PP);
                //Constitutive.print("C","\n\n C tensor \n");
                printf("Constitutive.trace = %12.6e\n", Constitutive.trace());
                Kmat = this->stiffness_matrix(Constitutive);
                printf("Constitutive tensor max:= %10.3e\n", Kmat.mmax());

                //----   GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor5\n");
                // this is update of constitutive tensor at this Gauss point
                GPtangent_E[where].Initialize(Constitutive);
                //....GPtangent_E[where].print(" tangent E at GAUSS point");

                //GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor6\n");

                //K = K + temp2;
                K = K + dhGlobal("ib") * Constitutive("abcd") * dhGlobal("jd") * weight;
                //....K.print("K","\n\n K tensor \n");
                Kmat = this->stiffness_matrix(K);
                printf("K tensor max= %10.3e\n", Kmat.mmax());

                //convert constitutive and K to matrix and find min and max and print!



            }
        }
    }

    return K;
}



////#############################################################################
void Brick3D::set_strain_stress_tensor(FILE* fp, double* u)
{
    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    //    tensor Constitutive( 4, def_dim_4, 0.0);
    tensor Constitutive;
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    int where = 0;

    double det_of_Jacobian = 0.0;

    straintensor strain;
    stresstensor stress;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;


    static int disp_dim[] = {20, 3};
    tensor total_displacements(2, disp_dim, 0.0); //

    total_displacements = total_disp(fp, u);

    ::printf("\n  displacement(x-y-z) at GAUSS pt %d \n\n", where + 1);

    for (int ii = 1; ii <= 8; ii++)
    {
        ::printf("Global# %d Local#%d  %+0.5e %+0.5e %+0.5e\n", G_N_numbs[ii - 1], ii, total_displacements.val(ii, 1),
                 total_displacements.val(ii, 2),
                 total_displacements.val(ii, 3));
    }

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");
                //weight
                // straines at this Gauss point from displacement
                strain = (dhGlobal("ib") * total_displacements("ia")).symmetrize11();
                strain.null_indices();
                // here comes the final_stress calculation
                // at this Gauss point.
                Constitutive =  GPtangent_E[where];
                stress = Constitutive("ijkl") * strain("kl");
                stress.null_indices();

                ::printf("\n  strain tensor at GAUSS point %d \n", where + 1);
                strain.reportshort("");
                ::printf("\n  stress tensor at GAUSS point %d \n", where + 1);
                stress.reportshort("");


            }
        }
    }
}



////#############################################################################
tensor Brick3D::mass_tensor(void)
//  tensor Brick3D::mass_tensor(Elastic  mmodel)
{
    int M_dim[] = {60, 60};
    tensor M(2, M_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    int h_dim[] = {60, 3};
    //    int h_dim[] = {20,3};
    tensor H(2, h_dim, 0.0);

    //    tensor Constitutive( 4, def_dim_4, 0.0);
    //    tensor Constitutive;

    double det_of_Jacobian = 0.0;

    //    static int disp_dim[] = {20,3};
    //    tensor incremental_displacements(2,disp_dim,0.0); // \Delta u

    //    straintensor incremental_strain;
    //    straintensor total_strain_at_GP;

    tensor Jacobian;
    //    tensor JacobianINV;
    //    tensor dhGlobal;

    //....    int number_of_subincrements = 1;
    //....    double this_one_PP = 1.0; // if set to 0.0 -> perfectly plastic
    //....                              // if set to 1.0 -> elasto plastic
    //    tensor Ktemp(4,K_dim,0.0);
    //    char * integr_type = 0;

    //    stresstensor final_stress_after_integration;
    //    stresstensor incremental_stress;
    // tensor of incremental displacements taken from node objects
    //    incremental_displacements = incr_disp();
    //    double RHO = 2.25;

    //   GaussPoint GaPo = GP();


    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );
        rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );
            sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                tw = get_Gauss_p_w( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //      Jacobian.print("J","Jacobian");
                // Inverse of Jacobian tensor ( matrix )
                //                JacobianINV = Jacobian_3Dinv(dh);
                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();
                //      printf("det_of_Jacobian = %6.2e \n",det_of_Jacobian);
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                //                dhGlobal = dh("ij") * JacobianINV("jk");
                // derivatives of local coordinates with respect to local coordinates


                // printf("\n\nIN THE MASS TENSOR INTEGRATOR ----**************** where = %d \n", where);
                // printf("  Mass_Tensor \n");
                // printf("                    GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //                             GP_c_r,GP_c_s,GP_c_t);
                //
                H = H_3D(r, s, t);

                //  double sum = 0.0;
                //  for (int i=1; i<=60 ; i++)
                //           {
                // //       sum+=H.cval(i,1);
                //      for (int j=1; j<= 3; j++)
                //         {
                //                    sum+=H.cval(i,j);
                //            ::printf( "  %+9.2e", H.cval(i,j) );
                //          }
                //             ::printf( "  %d \n", i);
                //     }
                //      ::printf( " \n sum= %+6.2e\n", sum );




                //                GaussPoint GaPo = GaussPoint::GP()+where;
                double RHO = GPmmodel->rho();
                printf("RHO = %10.3e", RHO);
                //      getchar();

                //weight
                weight = rw * sw * tw * RHO * det_of_Jacobian;
                //   printf("weight = %6.2e \n",weight);



                //M.print("M","BEFORE");

                //   tensor temp = H("ib")*H("kb");
                //temp.print("t","temporary tensor H(\"ib\")*H(\"kb\") \n\n" );

                M = M + H("ib") * H("kb") * weight;
                //   printf("\n +++++++++++++++++++++++++ \n\n");
                //    M.print("M","AFTER");
            }
        }
    }

    return M;
}


////#############################################################################
double Brick3D::Potential_Energy(void)
{
    //    double Potential_Energy_Estimate = 0.0;
    double Delta_Potential_Energy_Estimate = 0.0;

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    static int disp_dim[] = {20, 3};
    tensor incremental_displacements(2, disp_dim, 0.0); // \Delta u

    double det_of_Jacobian = 0.0;

    straintensor incremental_strain;
    //    straintensor total_strain_at_GP;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    stresstensor stress_sum;
    // tensor of incremental displacements taken from node objects
    incremental_displacements = incr_disp();

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );
        rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );
            sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                tw = get_Gauss_p_w( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");
                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");
                //weight
                weight = rw * sw * tw * det_of_Jacobian;
                //::::::   ::printf("\n\nIN THE STIFFNESS TENSOR INTEGRATOR ----**************** where = %d \n", where);
                //::fprintf(stdout," Potential_Energy\n");
                //::fprintf(stdout,"                    GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //                                      GP_c_r,GP_c_s,GP_c_t);
                //::fprintf(stdout,"WEIGHT = %f", weight);
                //::fprintf(stdout,"determinant of Jacobian = %f", determinant_of_Jacobian);
                //::::::   GaussP[where].report("Gauss Point\n");
                //::::::
                // incremental straines at this Gauss point
                //----   GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor1\n");
                incremental_strain =
                    (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();
                //GPstress[where].reportshortpqtheta("\n stress at GAUSS point in Potential_Energy\n");
                //GPiterative_stress[where].reportshortpqtheta("\n ITERATIVE stress at GAUSS point in Potential_Energy\n");

                stress_sum = GPstress[where] + GPiterative_stress[where];
                //stress_sum.reportshortpqtheta("\n ITERATIVE stress_SUM at GAUSS point in Potential_Energy\n");

                Delta_Potential_Energy_Estimate += weight * (stress_sum("ij") * incremental_strain("ij")).trace();
                //         Potential_Energy_Estimate =

            }
        }
    }

    //::fprintf(stdout,"Delta_Potential_Energy_Estimate = %.20e \n",Delta_Potential_Energy_Estimate);
    return Delta_Potential_Energy_Estimate;
}


////#############################################################################
tensor Brick3D::stiffness_matrix(const tensor& K)
{
    //    int K_dim[] = {20,3,3,20};
    //    tensor K(4,K_dim,0.0);
    matrix Kmatrix(60, 60, 0.0);

    int Ki = 0;
    int Kj = 0;

    for ( int i = 1 ; i <= 20 ; i++ )
    {
        for ( int j = 1 ; j <= 20 ; j++ )
        {
            for ( int k = 1 ; k <= 3 ; k++ )
            {
                for ( int l = 1 ; l <= 3 ; l++ )
                {
                    Ki = k + 3 * (i - 1);
                    Kj = l + 3 * (j - 1);
                    //::printf("i=%d k=%d  Ki=%d       j=%d l=%d  Kj=%d\n",i,k,Ki,j,l,Kj);

                    Kmatrix.val( Ki , Kj ) = K.cval(i, k, l, j);
                }
            }
        }
    }

    return Kmatrix;
}

//#############################################################################

////#############################################################################
// Constructing mass matrix from mass tensor ___Zhaohui 07-05-99
tensor Brick3D::mass_matrix(const tensor& M)
{
    //    int K_dim[] = {20,3,3,20};
    //    tensor K(4,K_dim,0.0);
    matrix Mmatrix(60, 60, 0.0);

    for ( int i = 1 ; i <= 60 ; i++ )
    {
        for ( int j = 1 ; j <= 60 ; j++ )
        {
            Mmatrix.val( i , j ) = M.cval(i, j);
            //  ::printf("Mi Mj %d %d %+6.2e ",Mi,Mj,Mmatrix.val( Mi , Mj ) );
        }
    }

    return Mmatrix;
}
////#############################################################################

////#############################################################################
tensor Brick3D::Jacobian_3D(tensor dh)
{
    //       dh ( 20*3)
    tensor N_C = Nodal_Coordinates(); // 20*3
    tensor Jacobian_3D = dh("ij") * N_C("ik");
    return Jacobian_3D;
}

//#############################################################################
tensor Brick3D::Jacobian_3Dinv(tensor dh)
{
    //       dh ( 20*3)
    tensor N_C = Nodal_Coordinates(); // 20*3
    tensor Jacobian_3Dinv = (dh("ij") * N_C("ik")).inverse();
    return Jacobian_3Dinv;
}


////#############################################################################
tensor Brick3D::Nodal_Coordinates(void)
{
    const int dimensions[] = {20, 3};
    tensor N_coord(2, dimensions, 0.0);

    for ( int i = 0 ; i < 20 ; i++ )
    {
        N_coord.val(i + 1, 1) = nodes[ G_N_numbs[i] ].x_coordinate();
        N_coord.val(i + 1, 2) = nodes[ G_N_numbs[i] ].y_coordinate();
        N_coord.val(i + 1, 3) = nodes[ G_N_numbs[i] ].z_coordinate();
    }

    return N_coord;
}

////#############################################################################
tensor Brick3D::incr_disp(void)
{
    const int dimensions[] = {20, 3};
    tensor increment_disp(2, dimensions, 0.0);

    for ( int i = 0 ; i < 20 ; i++ )
    {
        increment_disp.val(i + 1, 1) = nodes[ G_N_numbs[i] ].incremental_translation_x();
        increment_disp.val(i + 1, 2) = nodes[ G_N_numbs[i] ].incremental_translation_y();
        increment_disp.val(i + 1, 3) = nodes[ G_N_numbs[i] ].incremental_translation_z();
    }

    return increment_disp;
}

////#############################################################################
tensor Brick3D::total_disp(FILE* fp, double* u)
{
    const int dimensions[] = {20, 3};
    tensor total_disp(2, dimensions, 0.0);
    //    double totalx, totaly, totalz;
    //    totalx=0;
    //    totaly=0;
    //    totalz=0;

    for ( int i = 0 ; i < 20 ; i++ )
    {
        total_disp.val(i + 1, 1) = nodes[ G_N_numbs[i] ].total_translation_x(u);
        total_disp.val(i + 1, 2) = nodes[ G_N_numbs[i] ].total_translation_y(u);
        total_disp.val(i + 1, 3) = nodes[ G_N_numbs[i] ].total_translation_z(u);
    }

    return total_disp;
}

////#############################################################################
unsigned int Brick3D::get_global_number_of_node(unsigned int local_node_number)
{
    return G_N_numbs[local_node_number];
}

////#############################################################################
unsigned int  Brick3D::get_Brick_Number(void)
{
    return elem_numb;
}

////#############################################################################
int* Brick3D::get_LM(void)
{
    return LM;
}


////#############################################################################
void Brick3D::set_LM(Node* node)
{
    //    unsigned int BrickNumber = this->get_Brick_Number();
    //    this->reportshort("");
    // for element numbered BrickNumber create LM array (see Bathe pp984
    //    for (int LocalNodeNumber = 1 ; LocalNodeNumber<=20 ; LocalNodeNumber++ )
    for (int LocalNodeNumber = 1 ; LocalNodeNumber <= 8 ; LocalNodeNumber++ ) // for 8noded brick
    {
        //        unsigned int global_node_number = b3d[BrickNumber-1].get_global_number_of_node(LocalNodeNumber-1);
        unsigned int global_node_number = this->get_global_number_of_node(LocalNodeNumber - 1);
        LM[3 * LocalNodeNumber - 3] = node[global_node_number].eqn_tx();
        LM[3 * LocalNodeNumber - 2] = node[global_node_number].eqn_ty();
        LM[3 * LocalNodeNumber - 1] = node[global_node_number].eqn_tz();
    }

    // ::printf("\n\n");

    //===   this->reportLM("LM");
    //   for (int count01=1;count01<=8;count01++)
    //     {
    //       ::printf("element %4d localNode %4d Globalnode %4d  LM   %4d   %4d   %4d\n", BrickNumber, count01,this->get_global_number_of_node(count01-1),  LM[count01*3-3], LM[count01*3-2], LM[count01*3-1] );
    //     }

}


////#############################################################################
// returns nodal forces for given stress field in an element
tensor Brick3D::nodal_forces(void)
{
    int force_dim[] = {20, 3};
    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    stresstensor stress_at_GP(0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );
        rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );
            sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                tw = get_Gauss_p_w( t_integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;
                //..::printf("\n\nIN THE nodal forces ----**************** where = %d \n", where);
                //..::printf("                    GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //..                           GP_c_r,GP_c_s,GP_c_t);
                //..::printf("WEIGHT = %f", weight);
                //..::printf("determinant of Jacobian = %f", det_of_Jacobian);
                //..GaussP[where].report("Gauss Point\n");

                //..   // samo jos odredi ovaj tensor E i to za svaku gauss tacku drugaciji !!!!!!!!!!!!
                //..   ovde negde bi trebalo da se na osnovu inkrementalnih pomeranja
                //..   nadje inkrementalna deformacija ( strain_increment ) pa sa njom dalje:
                //..
                //// tensor of incremental displacements taken from node objects
                //                incremental_displacements = incr_disp();
                //
                //// incremental straines at this Gauss point
                //                incremental_strain =
                //                  (dhGlobal("ib")*incremental_displacements("ia")).symmetrize11();
                //
                //                incremental_strain.null_indices();
                ////incremental_strain.reportshort("\n incremental_strain tensor at GAUSS point\n");
                //
                ////                integr_type = (GPmmodel)->operator[](where).integration_type();
                ////                if ( !strcmp(integr_type,"BakcwardEuler")

                //..   dakle ovde posalji strain_increment jer se stari stress cuva u okviru svake
                //..   Gauss tacke a samo saljes strain_increment koji ce da se prenese
                //..   u integracionu rutinu pa ce ta da vrati krajnji napon i onda moze da
                //..   se pravi ConstitutiveStiffnessTensor.
                //.. Ustvari posalji sve sto imas ( incremental_strain, start_stress,
                //.. number_of_subincrements . . . u ovu Constitutive_tensor funkciju
                //.. pa ona nek ide, u zavisnosti od modela koji se koristi i neka
                //.. onda tamo u svakoj posebnoj modelskoj funkciji vrati sta treba
                //.. ( recimo Elastic odmah vraca Eelastic a recimo MRS_Lade prvo
                //.. pita koji nacin integracije da koristi pa onda u zvisnosti od toga
                //.. zove funkcuju koja integrali za taj algoritam ( ForwardEuler, BakcwardEuler,
                //.. SemiBackwardEuler, . . . ) i onda kada funkcija vrati napon onda
                //.. se opet pita koji je tip integracije bio u pitanju pa pravi odgovarajuci
                //.. ConstitutiveTensor i vraca ga nazad!

                //                   stress_at_GP = (GPstress)->operator[](where);
                stress_at_GP = GPstress[where];
                //..stress_at_GP.reportshort("\n stress_at_GPtensor at GAUSS point for nodal forces \n");

                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;
                //::::::                   nodal_forces.print("nf","\n\n Nodal Forces \n");

            }
        }
    }


    return nodal_forces;

}

////#############################################################################
// returns nodal forces for given ITERATIVE stress field in an element
tensor Brick3D::iterative_nodal_forces(void)
{
    int force_dim[] = {20, 3};
    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    stresstensor stress_at_GP(0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );
        rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );
            sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                tw = get_Gauss_p_w( t_integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                //.....
                //.....::printf("Brick3D::iterative_nodal_forces(void)  ----**************** where = %d \n", where);
                //.....::printf("UPDATE ");
                //.....::printf("   GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //.....                           GP_c_r,GP_c_s,GP_c_t);
                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;

                //                   stress_at_GP = (GPstress)->operator[](where);
                stress_at_GP = GPiterative_stress[where];
                GPiterative_stress[where].reportshortpqtheta("\n iterative_stress at GAUSS point in iterative_nodal_force\n");

                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;
                //nodal_forces.print("nf","\n Brick3D::iterative_nodal_forces Nodal Forces ~~~~\n");

            }
        }
    }


    return nodal_forces;

}

////#############################################################################
// returns nodal forces for given constant stress field in the element
tensor Brick3D::nodal_forces_from_stress(stresstensor& stress)
{
    int force_dim[] = {20, 3};
    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    //    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    //    stresstensor stress_at_GP(0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );
        rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );
            sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                tw = get_Gauss_p_w( t_integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                //--                where =
                //--                ((GP_c_r-1)*s_integration_order+GP_c_s-1)*t_integration_order+GP_c_t-1;
                //.....
                //.....::printf("Brick3D::iterative_nodal_forces(void)  ----**************** where = %d \n", where);
                //.....::printf("UPDATE ");
                //.....::printf("   GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //.....                           GP_c_r,GP_c_s,GP_c_t);
                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;

                //                   stress_at_GP = (GPstress)->operator[](where);
                //                stress_at_GP = GPiterative_stress[where];
                //GPiterative_stress[where].reportshortpqtheta("\n iterative_stress at GAUSS point in iterative_nodal_force\n");

                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress("ab") * weight;
                //nodal_forces.print("nf","\n Brick3D::iterative_nodal_forces Nodal Forces ~~~~\n");

            }
        }
    }

    return nodal_forces;

}


////#############################################################################
// returns nodal forces for given incremental strain field in an element
// by using the linearized constitutive tensor from the begining of the step !
tensor Brick3D::linearized_nodal_forces(void)
{
    int force_dim[] = {20, 3};
    tensor linearized_nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {20, 3};
    tensor dh(2, dh_dim, 0.0);

    //    stresstensor stress_at_GP(0.0);

    tensor Constitutive( 4, def_dim_4, 0.0);

    double det_of_Jacobian = 0.0;

    static int disp_dim[] = {20, 3};
    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    //    char * integr_type = 0;

    stresstensor final_linearized_stress;
    //    stresstensor incremental_stress;
    // tensor of incremental displacements taken from node objects for this element !
    incremental_displacements = incr_disp();
    //incremental_displacements.print("disp","\n incremental_displacements tensor at GAUSS point in iterative_Update\n");

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );
        rw = get_Gauss_p_w( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );
            sw = get_Gauss_p_w( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                tw = get_Gauss_p_w( t_integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");

                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");

                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("jk");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;
                //..::printf("\n\nIN THE nodal forces ----**************** where = %d \n", where);
                //..::printf("                    GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //..                           GP_c_r,GP_c_s,GP_c_t);
                //..::printf("WEIGHT = %f", weight);
                //..::printf("determinant of Jacobian = %f", det_of_Jacobian);
                // incremental straines at this Gauss point
                // now in Update we know the incremental displacements so let's find
                // the incremental strain
                incremental_strain =
                    (dhGlobal("ib") * incremental_displacements("ia")).symmetrize11();
                incremental_strain.null_indices();
                //incremental_strain.reportshort("\n incremental_strain tensor at GAUSS point in iterative_Update\n");

                Constitutive = GPtangent_E[where];
                //..//GPtangent_E[where].print("\n tangent E at GAUSS point \n");

                final_linearized_stress =
                    Constitutive("ijkl") * incremental_strain("kl");

                // nodal forces See Zienkievicz part 1 pp 108
                linearized_nodal_forces =
                    linearized_nodal_forces +
                    dhGlobal("ib") * final_linearized_stress("ab") * weight;
                //::::::                   nodal_forces.print("nf","\n\n Nodal Forces \n");

            }
        }
    }


    return linearized_nodal_forces;

}

//....////#############################################################################
//....// updates Gauss point stresses and strains from given displacements
//....void Brick3D::update_stress_strain(tensor & displacementsT)
//....  {
//....//    int force_dim[] = {20,3};
//....//    tensor nodal_forces(2,force_dim,0.0);
//....
//....    double r  = 0.0;
//....    double rw = 0.0;
//....    double s  = 0.0;
//....    double sw = 0.0;
//....    double t  = 0.0;
//....    double tw = 0.0;
//....
//....    short where = 0;
//....    double weight = 0.0;
//....
//....    int dh_dim[] = {20,3};
//....    tensor dh(2, dh_dim, 0.0);
//....
//....    stresstensor stress_at_GP(0.0);
//....    straintensor strain_at_GP(0.0);
//....
//....    double det_of_Jacobian = 0.0;
//....
//....    tensor Jacobian;
//....    tensor JacobianINV;
//....    tensor dhGlobal;
//....
//....    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
//....      {
//....        r = get_Gauss_p_c( r_integration_order, GP_c_r );
//....        rw = get_Gauss_p_w( r_integration_order, GP_c_r );
//....
//....        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
//....          {
//....            s = get_Gauss_p_c( s_integration_order, GP_c_s );
//....            sw = get_Gauss_p_w( s_integration_order, GP_c_s );
//....
//....            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
//....              {
//....                t = get_Gauss_p_c( t_integration_order, GP_c_t );
//....                tw = get_Gauss_p_w( t_integration_order, GP_c_t );
//....
//....// this short routine is supposed to calculate position of
//....// Gauss point from 3D array of short's
//....                where =
//....                ((GP_c_r-1)*s_integration_order+GP_c_s-1)*t_integration_order+GP_c_t-1;
//....
//....//........................................................
//....//........................................................
//....// interpolation functions
//....                tensor h = b3darray[0].interp_poli_at(r,s,t);
//....                ::printf("\n\n r = %f, s = %f, t = %f\n", r, s, t);
//....//  h.print("h");
//....
//....// displacements
//....//....   tensor disp_at_rst = h("i")*displacementsT("ia");
//....//....   disp_at_rst.print("disp");
//....
//....// derivatives of interpolation functions
//....                dh = dh_drst_at(r,s,t);
//....//                ::printf("\n\n r = %f, s = %f, t = %f\n", r, s, t);
//....//  dh.print("dh");
//....
//....                Jacobian = b3darray[0].Jacobian_3D(dh);
//....//                Jacobian.print("J");
//....
//....                JacobianINV = b3darray[0].Jacobian_3Dinv(dh);
//....//                JacobianINV.print("JINV");
//....
//....//                det_of_Jacobian = Jacobian.determinant();
//....//                ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );
//....
//....// Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
//....                dhGlobal = dh("ij") * JacobianINV("jk");
//....// straines
//....//  strain = (dh("ib")*displacements("ia")).symmetrize11();
//....                strain = (dhGlobal("ib")*displacementsT("ia")).symmetrize11();
//....//  straintensor strain = dh("ib")*displacements("ia");
//....                strain.reportshort("\n strain tensor\n");
//....                strain.null_indices();
//....
//....//                tensor E = mmElastic.ElasticStiffness();
//....
//....//stresses
//....                stress = E("ijkl") * strain("kl");
//....                stress.reportshort("\n\n stress tensor \n");
//....//...
//....//........................................................
//....//........................................................
//....//........................................................
//....//........................................................
//....//........................................................
//....//........................................................
//....//........................................................
//....
//....
//....              }
//....          }
//....      }
//....
//....  }

//#############################################################################
//#############################################################################
double Brick3D::get_first_q_ast(void)
{
    double ret = GPmmodel[0].kappa_cone_get();

    return ret;

}
//#############################################################################
double Brick3D::get_first_etacone(void)
{
    double ret = GPmmodel[0].etacone();

    return ret;

}


//#############################################################################
void Brick3D::report(char* msg)
{
    if ( msg )
    {
        ::printf("** %s", msg);
    }

    ::printf("\n Element Number = %d\n",
             elem_numb);
    ::printf("\n Number of nodes in a brick3D = %d\n",
             nodes_in_brick);
    ::printf("\n Determinant of Jacobian (! ==0 before comp.) = %f\n",
             determinant_of_Jacobian);

    ::printf("Node numbers \n");
    ::printf(
        ".....1.....2.....3.....4.....5.....6.....7.....8.....9.....0.....1.....2\n");

    for ( int i = 0 ; i < 8 ; i++ )
    {
        ::printf("%6d", G_N_numbs[i]);
    }

    ::printf("\n");

    for ( int j = 8 ; j < 20 ; j++ )
    {
        ::printf("%6d", G_N_numbs[j]);
    }

    ::printf("\n\n");

    ::printf("Node existance array \n");

    for ( int k = 0 ; k < 12 ; k++ )
    {
        ::printf("%6d", node_existance[k]);
    }

    ::printf("\n\n");

    int total_number_of_Gauss_points = r_integration_order *
                                       s_integration_order *
                                       t_integration_order;

    if ( total_number_of_Gauss_points != 0 )
    {
        // report from Node class
        for ( int in = 0 ; in < 8 ; in++ )
        {
            (nodes[G_N_numbs[in]]).report("nodes from within element (first 8)\n");
        }

        for ( int jn = 8 ; jn < 20 ; jn++ )
        {
            (nodes[G_N_numbs[jn]]).report("nodes from within element (last 12)\n");
        }
    }

    ::printf("\n\nGauss-Legendre integration order\n");
    ::printf("Integration order in r direction = %d\n", r_integration_order);
    ::printf("Integration order in s direction = %d\n", s_integration_order);
    ::printf("Integration order in t direction = %d\n\n", t_integration_order);



    for( int GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        for( int GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            for( int GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                short where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;

                ::printf("\n\n----------------**************** where = %d \n", where);
                ::printf("                    GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                         GP_c_r, GP_c_s, GP_c_t);
                GaussP[where].report("Gauss Point\n");
                //GPstress[where].reportshort("stress at Gauss Point");
                //GPstrain[where].reportshort("strain at Gauss Point");
                //GPmmodel[where].report("Material model  at Gauss Point");
            }
        }
    }

}


//#############################################################################
void Brick3D::reportshort(char* msg)
{
    if ( msg )
    {
        ::printf("** %s", msg);
    }

    ::printf("\n Element Number = %d\n",
             elem_numb);
    ::printf("\n Number of nodes in a brick3D = %d\n",
             nodes_in_brick);
    ::printf("\n Determinant of Jacobian (! ==0 before comp.) = %f\n",
             determinant_of_Jacobian);

    ::printf("Node numbers \n");
    ::printf(
        ".....1.....2.....3.....4.....5.....6.....7.....8.....9.....0.....1.....2\n");

    for ( int i = 0 ; i < 8 ; i++ )
    {
        ::printf("%6d", G_N_numbs[i]);
    }

    ::printf("\n");

    for ( int j = 8 ; j < 20 ; j++ )
    {
        ::printf("%6d", G_N_numbs[j]);
    }

    ::printf("\n\n");

    ::printf("Node existance array \n");

    for ( int k = 0 ; k < 12 ; k++ )
    {
        ::printf("%6d", node_existance[k]);
    }

    ::printf("\n\n");

}




//#############################################################################
void Brick3D::reportPAK(char* msg)
{
    if ( msg )
    {
        ::printf("%s", msg);
    }

    ::printf("%10d   ",  elem_numb);

    for ( int i = 0 ; i < 8 ; i++ )
    {
        ::printf("%6d", G_N_numbs[i]);
    }

    printf("\n");
}


//#############################################################################
void Brick3D::reportpqtheta(int GP_numb)
{
    short where = GP_numb - 1;
    GaussP[where].reportpqtheta("");
}

//#############################################################################
void Brick3D::reportLM(char* msg)
{
    if ( msg )
    {
        ::printf("%s", msg);
    }

    ::printf("Element # %d, LM->", this->get_Brick_Number());

    for (int count = 0 ; count < 24 ; count++)
    {
        ::printf(" %d", LM[count]);
    }

    ::printf("\n");

}

//#############################################################################
void Brick3D::reportTensor(char* msg)
{
    //    if ( msg ) ::printf("** %s\n",msg);

    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    // special case for 8 nodes only
    static const int dim[] = {3, 8}; // static-> see ARM pp289-290
    tensor NodalCoord(2, dim, 0.0);
    tensor GaussPointCoord(2, dim, 0.0);
    int h_dim[] = {60, 3};
    tensor H(2, h_dim, 0.0);

    for (int ncount = 1 ; ncount <= 8 ; ncount++ )
        //    for (int ncount = 0 ; ncount <= 7 ; ncount++ )
    {
        int global_node_number = get_global_number_of_node(ncount - 1);
        // printf("global node num %d",global_node_number);

        NodalCoord.val(1, ncount) = nodes[global_node_number].x_coordinate();
        NodalCoord.val(2, ncount) = nodes[global_node_number].y_coordinate();
        NodalCoord.val(3, ncount) = nodes[global_node_number].z_coordinate();
        //printf("global point %d     x=%+.6e   y=%+.6e   z=%+.6e \n ", global_node_number,
        //                                                      NodalCoord.val(1,ncount),
        //                            NodalCoord.val(2,ncount),
        //                            NodalCoord.val(3,ncount));
    }

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates

                H = H_3D(r, s, t);

                for (int encount = 1 ; encount <= 8 ; encount++ )
                    //         for (int encount=0 ; encount <= 7 ; encount++ )
                {
                    //  GaussPointCoord.val(1,where+1) =+NodalCoord.val(1,where+1) * H.val(encount*3-2,1);
                    //  GaussPointCoord.val(2,where+1) =+NodalCoord.val(2,where+1) * H.val(encount*3-1,2);
                    //  GaussPointCoord.val(3,where+1) =+NodalCoord.val(3,where+1) * H.val(encount*3-0,3);
                    GaussPointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
                    //::printf("-- NO nodal, H_val :%d %+.2e %+.2e %+.5e\n", encount,NodalCoord.val(1,encount),H.val(encount*3-2,1),GaussPointCoord.val(1,where+1) );
                    GaussPointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
                    GaussPointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);

                }

                ::printf("gauss point# %d   %+.6e %+.6e %+.6e \n", where + 1,
                         GaussPointCoord.val(1, where + 1),
                         GaussPointCoord.val(2, where + 1),
                         GaussPointCoord.val(3, where + 1));

                //GaussP[where].reportTensor("");


            }
        }
    }

}

//#############################################################################
void Brick3D::reportCIPIC(char* msg)
{
    //    if ( msg ) ::printf("** %s\n",msg);

    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    // special case for 8 nodes only
    static const int dim[] = {3, 8}; // static-> see ARM pp289-290
    tensor NodalCoord(2, dim, 0.0);
    tensor GaussPointCoord(2, dim, 0.0);
    int h_dim[] = {60, 3};
    tensor H(2, h_dim, 0.0);

    for (int ncount = 1 ; ncount <= 8 ; ncount++ )
        //    for (int ncount = 0 ; ncount <= 7 ; ncount++ )
    {
        int global_node_number = get_global_number_of_node(ncount - 1);
        // printf("global node num %d",global_node_number);

        NodalCoord.val(1, ncount) = nodes[global_node_number].x_coordinate();
        NodalCoord.val(2, ncount) = nodes[global_node_number].y_coordinate();
        NodalCoord.val(3, ncount) = nodes[global_node_number].z_coordinate();
        //printf("global point %d     x=%+.6e   y=%+.6e   z=%+.6e \n ", global_node_number,
        //                                                      NodalCoord.val(1,ncount),
        //                            NodalCoord.val(2,ncount),
        //                            NodalCoord.val(3,ncount));
    }

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates

                H = H_3D(r, s, t);

                for (int encount = 1 ; encount <= 8 ; encount++ )
                    //         for (int encount=0 ; encount <= 7 ; encount++ )
                {
                    //  GaussPointCoord.val(1,where+1) =+NodalCoord.val(1,where+1) * H.val(encount*3-2,1);
                    //  GaussPointCoord.val(2,where+1) =+NodalCoord.val(2,where+1) * H.val(encount*3-1,2);
                    //  GaussPointCoord.val(3,where+1) =+NodalCoord.val(3,where+1) * H.val(encount*3-0,3);
                    GaussPointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
                    //::printf("-- NO nodal, H_val :%d %+.2e %+.2e %+.5e\n", encount,NodalCoord.val(1,encount),H.val(encount*3-2,1),GaussPointCoord.val(1,where+1) );
                    GaussPointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
                    GaussPointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);

                }

                ::printf("gauss point# %d   %+.6e %+.6e %+.6e \n", where + 1,
                         GaussPointCoord.val(1, where + 1),
                         GaussPointCoord.val(2, where + 1),
                         GaussPointCoord.val(3, where + 1));

                //GaussP[where].reportTensor("");


            }
        }
    }

}


////#############################################################################

//#############################################################################
//void Brick3D::reportTensor(char * msg)
// ZHaohui added to print gauss point coord. to file gausspoint.dat
void Brick3D::reportTensorF(FILE* fp)
{
    //    if ( msg ) ::printf("** %s\n",msg);

    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    // special case for 8 nodes only
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    // special case for 8 nodes only
    static const int dim[] = {3, 8}; // static-> see ARM pp289-290
    tensor NodalCoord(2, dim, 0.0);
    tensor GaussPointCoord(2, dim, 0.0);
    int h_dim[] = {60, 3};
    tensor H(2, h_dim, 0.0);

    for (int ncount = 1 ; ncount <= 8 ; ncount++ )
        //    for (int ncount = 0 ; ncount <= 7 ; ncount++ )
    {
        int global_node_number = get_global_number_of_node(ncount - 1);
        // printf("global node num %d",global_node_number);

        NodalCoord.val(1, ncount) = nodes[global_node_number].x_coordinate();
        NodalCoord.val(2, ncount) = nodes[global_node_number].y_coordinate();
        NodalCoord.val(3, ncount) = nodes[global_node_number].z_coordinate();
        //printf("global point %d     x=%+.6e   y=%+.6e   z=%+.6e \n ", global_node_number,
        //                                                      NodalCoord.val(1,ncount),
        //                            NodalCoord.val(2,ncount),
        //                            NodalCoord.val(3,ncount));
    }

    for( short GP_c_r = 1 ; GP_c_r <= r_integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( r_integration_order, GP_c_r );

        for( short GP_c_s = 1 ; GP_c_s <= s_integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( s_integration_order, GP_c_s );

            for( short GP_c_t = 1 ; GP_c_t <= t_integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( t_integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * s_integration_order + GP_c_s - 1) * t_integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates

                H = H_3D(r, s, t);

                for (int encount = 1 ; encount <= 8 ; encount++ )
                    //         for (int encount=0 ; encount <= 7 ; encount++ )
                {
                    //  GaussPointCoord.val(1,where+1) =+NodalCoord.val(1,where+1) * H.val(encount*3-2,1);
                    //  GaussPointCoord.val(2,where+1) =+NodalCoord.val(2,where+1) * H.val(encount*3-1,2);
                    //  GaussPointCoord.val(3,where+1) =+NodalCoord.val(3,where+1) * H.val(encount*3-0,3);
                    GaussPointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
                    //::printf("-- NO nodal, H_val :%d %+.2e %+.2e %+.5e\n", encount,NodalCoord.val(1,encount),H.val(encount*3-2,1),GaussPointCoord.val(1,where+1) );
                    GaussPointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
                    GaussPointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);

                }

                fprintf(fp, "gauss point# %d   %+.6e %+.6e %+.6e \n", where + 1,
                        GaussPointCoord.val(1, where + 1),
                        GaussPointCoord.val(2, where + 1),
                        GaussPointCoord.val(3, where + 1));

                //GaussP[where].reportTensor("");


            }
        }
    }

}




#endif

