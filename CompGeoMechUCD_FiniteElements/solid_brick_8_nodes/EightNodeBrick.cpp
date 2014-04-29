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


#ifndef EIGHTNODEBRICK_CPP
#define EIGHTNODEBRICK_CPP

#include <EightNodeBrick.h>
#include <Vector.h>

#define FixedOrder 2

Matrix EightNodeBrick::K(24, 24);
//Guanzhou out Matrix EightNodeBrick::C(24, 24);
Matrix EightNodeBrick::M(24, 24);
Vector EightNodeBrick::P(24);
Vector InfoP(FixedOrder *FixedOrder *FixedOrder * 4 + 1); //Plastic info(coor+pls) 32+1 2X2X2
Vector InfoP1(FixedOrder *FixedOrder * FixedOrder + 1); //Plastic info, no Gauss point coordinates
Vector InfoS(FixedOrder *FixedOrder *FixedOrder * 6 + 1); //Stress 8*6+1  2X2X2
Vector InfoSpq(2); //p and q of count/2
Vector InfoSpq_all(2 * FixedOrder *FixedOrder * FixedOrder + 4); //p and q of all GS points + Sig11-33 +Strain_plastic_vv +psi
Vector Gsc8(FixedOrder *FixedOrder *FixedOrder * 3 + 1); //Gauss point coordinates
Vector InfoStr(49); //Nima added for recording the Strain (10-13-2008)
Vector EightNodeBrick::ShapeFunctionValues_in_function(4); // Nima added for surface load (July 2012)
Vector EightNodeBrick::J_vector_in_function(3); // Nima added for surface load (July 2012)
double EightNodeBrick::SurfaceLoadValues_in_function; // Nima added for surface load (July 2012)


//====================================================================
//Reorganized constructor ____ Zhaohui 02-10-2000
//====================================================================

EightNodeBrick::EightNodeBrick(int element_number,
                               int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                               int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                               NDMaterial *Globalmmodel)

    : Element(element_number, ELE_TAG_EightNodeBrick),
      connectedExternalNodes(8), Ki(0), Q(24),
      rho(0.0), epEnergy(0.0)
{
    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in EightNodeBrick::EightNodeBrick" <<endln; this->Print(cerr);
    //BJ//BJ
    //elem_numb = element_number;


    //================================================================
    // added by Nima Tafazzoli (October 2010)
    // replacing the input density with the one defined in material
    rho = Globalmmodel->getRho();
    //================================================================
    // added by Nima Tafazzoli (October 2010)
    // getting the acceleration field from domain instead of input argument
    // for element
    //     accfield = accelerationfield->getAccelerationFieldVector();

    //     damptag = theDamping->return_tag();
    //     damp_a0 = theDamping->return_a0();
    //     damp_a1 = theDamping->return_a1();
    //     damp_a2 = theDamping->return_a2();
    //     damp_a3 = theDamping->return_a3();
    //     stiffness_type = theDamping->return_stiffnesstype();
    //     damping_type = theDamping->return_dampingtype();


    //================================================================

    determinant_of_Jacobian = 0.0;

    //integration_order = r_int_order;
    //integration_order = s_int_order;
    //integration_order = t_int_order;
    integration_order = FixedOrder; // Gauss-Legendre integration order in r direction
    //Not needed. Right now we have one NDMaterial for each material point
    //mmodel = Globalmmodel->getCopy( type ); // One global mat model

    //Guanzhou moved out to populate()    int total_number_of_Gauss_points = integration_order*integration_order*integration_order;
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()    if ( total_number_of_Gauss_points != 0 )
    //Guanzhou moved out to populate()      {
    //Guanzhou moved out to populate()         matpoint  = new MatPoint3D * [total_number_of_Gauss_points];
    //Guanzhou moved out to populate()      }
    //Guanzhou moved out to populate()    else
    //Guanzhou moved out to populate()      {
    //Guanzhou moved out to populate()  //GPstress = 0;//GPiterative_stress = 0;//GPq_ast_iterative  = 0; //GPstrain = 0;//GPtangent_E = 0;
    //Guanzhou moved out to populate()        matpoint  = 0;
    //Guanzhou moved out to populate()    }
    //Guanzhou moved out to populate()    ////////////////////////////////////////////////////////////////////
    //Guanzhou moved out to populate()    //dakle posto:
    //Guanzhou moved out to populate()    //// according to ARM pp.61 default constructor will be called!!
    //Guanzhou moved out to populate()    //onda oni vec postoje u memoriji i samo im treba dodeliti prave
    //Guanzhou moved out to populate()    //vrednosti iz onog modela koji je prenesen unutra. Znaci onInitializxe funkcija
    //Guanzhou moved out to populate()    //sa modelom kao argumentom Initialize(taj_Model).
    //Guanzhou moved out to populate()    //za stresstensor i straintensor isto to napravi
    //Guanzhou moved out to populate()    //tu Initialize funkciju pa da uzima argument i da samo
    //Guanzhou moved out to populate()    //koristi njegove vrednosti pa stavi u vec postojece mesto
    //Guanzhou moved out to populate()    //u memoriji ove vrednsoti. Takodje unutar brick3d napravi ih
    //Guanzhou moved out to populate()    //( te stresstensor , straintensor i mmodel ) static da ostanu tu
    //Guanzhou moved out to populate()    //da se ne pozove destructor . . .
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()    short where = 0;
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()    for( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    //Guanzhou moved out to populate()      {
    //Guanzhou moved out to populate()        double r = get_Gauss_p_c( integration_order, GP_c_r );
    //Guanzhou moved out to populate()        double rw = get_Gauss_p_w( integration_order, GP_c_r );
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()        for( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
    //Guanzhou moved out to populate()          {
    //Guanzhou moved out to populate()            double s = get_Gauss_p_c( integration_order, GP_c_s );
    //Guanzhou moved out to populate()            double sw = get_Gauss_p_w( integration_order, GP_c_s );
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()            for( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
    //Guanzhou moved out to populate()              {
    //Guanzhou moved out to populate()                double t = get_Gauss_p_c( integration_order, GP_c_t );
    //Guanzhou moved out to populate()                double tw = get_Gauss_p_w( integration_order, GP_c_t );
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()                // this short routine is supposed to calculate position of
    //Guanzhou moved out to populate()                // Gauss point from 3D array of short's
    //Guanzhou moved out to populate()                where =
    //Guanzhou moved out to populate()                ((GP_c_r-1)*integration_order+GP_c_s-1)*integration_order+GP_c_t-1;
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()                //DB::printf("\n\nBefore Initialization **************** where = %d \n",where);
    //Guanzhou moved out to populate()                //DB::printf("GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
    //Guanzhou moved out to populate()                //DB            GP_c_r,GP_c_s,GP_c_t);
    //Guanzhou moved out to populate()                //DB
    //Guanzhou moved out to populate()                //DBGPstress[where].reportshort("stress within before Initialization");
    //Guanzhou moved out to populate()                //DBGPstrain[where].reportshort("strain within before Initialization");
    //Guanzhou moved out to populate()                //DB
    //Guanzhou moved out to populate()                //DB// I suspect that [] should be overloaded so that compiler knows which
    //Guanzhou moved out to populate()                //DB// material model is returning a pointer and fot the purpose
    //Guanzhou moved out to populate()                //DB//matpoint[where].report("mmodel within before Initialization");
    //Guanzhou moved out to populate()                //DB//matpoint[where].report("mmodel within before Initialization"); // operator[] overloaded
    //Guanzhou moved out to populate()                //DB(matpoint)->operator[](where).report("mmodel within before Initialization"); // operator[] overloaded
    //Guanzhou moved out to populate()                //DB                                                               // for NDMaterial and
    //Guanzhou moved out to populate()                //DB                                                               // derived types!
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()    // Zhaohui  09-30-2000
    //Guanzhou moved out to populate()                //GPtangent_E[where].Initialize_all(*IN_tangent_E);
    //Guanzhou moved out to populate()                //GPstress[where].Initialize(*INstress);
    //Guanzhou moved out to populate()                //GPiterative_stress[where].Initialize(*INiterative_stress);
    //Guanzhou moved out to populate()                //GPq_ast_iterative[where] = IN_q_ast_iterative[where];
    //Guanzhou moved out to populate()                //GPstrain[where].Initialize(*INstrain);
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()                // Already assigned the copy of Globalmmodel to each element of matpoint
    //Guanzhou moved out to populate()    //(matpoint)->operator[](where).Initialize(*Globalmmodel);
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()                //NDMaterial *NMD = Globalmmodel->getCopy();
    //Guanzhou moved out to populate()    //if (strcmp( Globalmmodel->getType(), "Template3Dep" ) == 0)
    //Guanzhou moved out to populate()     //   NMD = 0;
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()                //cerr << "where = " << where << endlnn;
    //Guanzhou moved out to populate()    matpoint[where] = new MatPoint3D(GP_c_r,
    //Guanzhou moved out to populate()                                         GP_c_s,
    //Guanzhou moved out to populate()                                         GP_c_t,
    //Guanzhou moved out to populate()                                         r, s, t,
    //Guanzhou moved out to populate()                                         rw, sw, tw,
    //Guanzhou moved out to populate()                                         //InitEPS,
    //Guanzhou moved out to populate()           Globalmmodel);
    //Guanzhou moved out to populate()           //NMD);
    //Guanzhou moved out to populate()           //&( GPstress[where] ), //&( GPiterative_stress[where] ), //IN_q_ast_iterative[where] ,//&( GPstrain[where] ),  //&( GPtangent_E[where] ),
    //Guanzhou moved out to populate()                                         //&( (matpoint)->operator[](where) )
    //Guanzhou moved out to populate()                                         // ugly syntax but it works! Still don't know what's wrong   // with the old style matpoint[where]
    //Guanzhou moved out to populate()              }
    //Guanzhou moved out to populate()          }
    //Guanzhou moved out to populate()      }

    // Set connected external node IDs
    mmodel = Globalmmodel; //Guanzhou added for later populating
    initialized = false; //Guanzhou added!

# ifndef _PARALLEL_PROCESSING
    populate();
# endif

    connectedExternalNodes(0) = node_numb_1;
    connectedExternalNodes(1) = node_numb_2;
    connectedExternalNodes(2) = node_numb_3;
    connectedExternalNodes(3) = node_numb_4;

    connectedExternalNodes(4) = node_numb_5;
    connectedExternalNodes(5) = node_numb_6;
    connectedExternalNodes(6) = node_numb_7;
    connectedExternalNodes(7) = node_numb_8;

    nodes_in_brick = 8;

    //Guanzhou added
    //ComputeVolume();

    // zero node pointers
    for (int i = 0; i < 8; i++)
    {
        theNodes[i] = 0;
    }
}

//Guanzhou added
void EightNodeBrick::populate()
{
    int total_number_of_Gauss_points = integration_order * integration_order * integration_order;


    if ( total_number_of_Gauss_points != 0 )
    {
        matpoint  = new MatPoint3D * [total_number_of_Gauss_points];
    }
    else
    {
        //GPstress = 0;//GPiterative_stress = 0;//GPq_ast_iterative  = 0; //GPstrain = 0;//GPtangent_E = 0;
        matpoint  = 0;
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

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        double r = get_Gauss_p_c( integration_order, GP_c_r );
        double rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            double s = get_Gauss_p_c( integration_order, GP_c_s );
            double sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                double t = get_Gauss_p_c( integration_order, GP_c_t );
                double tw = get_Gauss_p_w( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                //DB::printf("\n\nBefore Initialization **************** where = %d \n",where);
                //DB::printf("GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //DB            GP_c_r,GP_c_s,GP_c_t);
                //DB
                //DBGPstress[where].reportshort("stress within before Initialization");
                //DBGPstrain[where].reportshort("strain within before Initialization");
                //DB
                //DB// I suspect that [] should be overloaded so that compiler knows which
                //DB// material model is returning a pointer and fot the purpose
                //DB//matpoint[where].report("mmodel within before Initialization");
                //DB//matpoint[where].report("mmodel within before Initialization"); // operator[] overloaded
                //DB(matpoint)->operator[](where).report("mmodel within before Initialization"); // operator[] overloaded
                //DB                                                               // for NDMaterial and
                //DB                                                               // derived types!

                // Zhaohui  09-30-2000
                //GPtangent_E[where].Initialize_all(*IN_tangent_E);
                //GPstress[where].Initialize(*INstress);
                //GPiterative_stress[where].Initialize(*INiterative_stress);
                //GPq_ast_iterative[where] = IN_q_ast_iterative[where];
                //GPstrain[where].Initialize(*INstrain);

                // Already assigned the copy of Globalmmodel to each element of matpoint
                //(matpoint)->operator[](where).Initialize(*Globalmmodel);

                //NDMaterial *NMD = Globalmmodel->getCopy();
                //if (strcmp( Globalmmodel->getType(), "Template3Dep" ) == 0)
                //   NMD = 0;

                //cerr << "where = " << where << endlnn;
                matpoint[where] = new MatPoint3D(GP_c_r,
                                                 GP_c_s,
                                                 GP_c_t,
                                                 r, s, t,
                                                 rw, sw, tw,
                                                 //InitEPS,
                                                 mmodel); //Guanzhou changed to point to global material model!
                //NMD);
                //&( GPstress[where] ), //&( GPiterative_stress[where] ), //IN_q_ast_iterative[where] ,//&( GPstrain[where] ),  //&( GPtangent_E[where] ),
                //&( (matpoint)->operator[](where) )
                // ugly syntax but it works! Still don't know what's wrong   // with the old style matpoint[where]
            }
        }
    }

    initialized = true; //Guanzhou added!
}




//====================================================================
EightNodeBrick::EightNodeBrick(): Element(0, ELE_TAG_EightNodeBrick ),
    connectedExternalNodes(8), Ki(0), Q(24), rho(0.0), epEnergy(0.0), mmodel(0)
{
    integration_order = FixedOrder; // Gauss-Legendre integration order in r direction
    integration_order = FixedOrder; // Gauss-Legendre integration order in s direction
    integration_order = FixedOrder; // Gauss-Legendre integration order in t direction
    //Not needed. Right now we have one NDMaterial for each material point
    //mmodel = Globalmmodel->getCopy( type ); // One global mat model

    //Guanzhou moved out to populate()    int total_number_of_Gauss_points = integration_order*integration_order*integration_order;
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()    if ( total_number_of_Gauss_points != 0 )
    //Guanzhou moved out to populate()      {
    //Guanzhou moved out to populate()         matpoint  = new MatPoint3D * [total_number_of_Gauss_points];
    //Guanzhou moved out to populate()      }
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()    short where = 0;
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()    for( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    //Guanzhou moved out to populate()      {
    //Guanzhou moved out to populate()        double r = get_Gauss_p_c( integration_order, GP_c_r );
    //Guanzhou moved out to populate()        double rw = get_Gauss_p_w( integration_order, GP_c_r );
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()        for( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
    //Guanzhou moved out to populate()          {
    //Guanzhou moved out to populate()            double s = get_Gauss_p_c( integration_order, GP_c_s );
    //Guanzhou moved out to populate()            double sw = get_Gauss_p_w( integration_order, GP_c_s );
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()            for( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
    //Guanzhou moved out to populate()              {
    //Guanzhou moved out to populate()                double t = get_Gauss_p_c( integration_order, GP_c_t );
    //Guanzhou moved out to populate()                double tw = get_Gauss_p_w( integration_order, GP_c_t );
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()                // this short routine is supposed to calculate position of
    //Guanzhou moved out to populate()                // Gauss point from 3D array of short's
    //Guanzhou moved out to populate()                where =
    //Guanzhou moved out to populate()                ((GP_c_r-1)*integration_order+GP_c_s-1)*integration_order+GP_c_t-1;
    //Guanzhou moved out to populate()
    //Guanzhou moved out to populate()          matpoint[where] = new MatPoint3D(GP_c_r,
    //Guanzhou moved out to populate()                                          GP_c_s,
    //Guanzhou moved out to populate()                                          GP_c_t,
    //Guanzhou moved out to populate()                                          r, s, t,
    //Guanzhou moved out to populate()                                          rw, sw, tw, 0);
    //Guanzhou moved out to populate()              }
    //Guanzhou moved out to populate()          }
    //Guanzhou moved out to populate()      }

    initialized = false; //Guanzhou added!
# ifndef _PARALLEL_PROCESSING //Guanzhou added!
    populate();
# endif

    nodes_in_brick = 8;

    //Guanzhou added
    //ComputeVolume();


    // zero node pointers
    for (int i = 0; i < 8; i++)
    {
        theNodes[i] = 0;
    }
}

// Initialize
// default constructor
// this one takes only one material model and forwards it to all
// Gauss points.
//#############################################################################
//
//int EightNodeBrick::Initialize(int element_number,
//                                int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
//                                int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
//                                NDMaterial * Globalmmodel,  double b1, double b2, double b3,
//                                double p, double r
//        //, EPState * InitEPS  const char * type,
//              //tensor       * IN_tangent_E,//stresstensor * INstress, //stresstensor * INiterative_stress, //double  * IN_q_ast_iterative, //straintensor * INstrain
//              )
//  {
//    //elem_numb = element_number;
//    bf(0) = b1;
//    bf(1) = b2;
//    bf(2) = b3;
//    rho = r;
//    pressure = p;
//    // integration_order = r_int_order;
//    // integration_order = s_int_order;
//    // integration_order = t_int_order;
//    integration_order = FixedOrder; // Gauss-Legendre integration order in r direction
//    integration_order = FixedOrder; // Gauss-Legendre integration order in s direction
//    integration_order = FixedOrder; // Gauss-Legendre integration order in t direction
//
//    //mmodel = Globalmmodel->getCopy(); // One global mat model
//
//    int total_number_of_Gauss_points = integration_order*
//                                       integration_order*
//                                       integration_order;
//
//    //matpoint = Globalmmodel->new_mm(total_number_of_Gauss_points);
//    //matpoint = Globalmmodel->getCopy(total_number_of_Gauss_points);
//
//    //GPstress = new stresstensor[total_number_of_Gauss_points];
//    //GPiterative_stress = new stresstensor[total_number_of_Gauss_points];
//    //GPq_ast_iterative  = new double[total_number_of_Gauss_points];
//    //GPstrain = new straintensor[total_number_of_Gauss_points];
//    //GPtangent_E = new tensor[total_number_of_Gauss_points];
//    //matpoint  = new MatPoint3D[total_number_of_Gauss_points];
//
//    matpoint = new MatPoint3D * [total_number_of_Gauss_points];
//
//    ////////////////////////////////////////////////////////////////////
//    //dakle posto:
//    //// according to ARM pp.61 default constructor will be called!!
//    //onda oni vec postoje u memoriji i samo im treba dodeliti prave
//    //vrednosti iz onog modela koji je prenesen unutra. Znaci onInitializxe funkcija
//    //sa modelom kao argumentom Initialize(taj_Model).
//    //za stresstensor i straintensor isto to napravi
//    //tu Initialize funkciju pa da uzima argument i da samo
//    //koristi njegove vrednosti pa stavi u vec postojece mesto
//    //u memoriji ove vrednsoti. Takodje unutar brick3d napravi ih
//    //( te stresstensor , straintensor i mmodel ) static da ostanu tu
//    //da se ne pozove destructor . . .
//
//    short where = 0;
//
//    for( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//      {
//        double r = get_Gauss_p_c( integration_order, GP_c_r );
//        double rw = get_Gauss_p_w( integration_order, GP_c_r );
//
//        for( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//          {
//            double s = get_Gauss_p_c( integration_order, GP_c_s );
//            double sw = get_Gauss_p_w( integration_order, GP_c_s );
//
//            for( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//              {
//                double t = get_Gauss_p_c( integration_order, GP_c_t );
//                double tw = get_Gauss_p_w( integration_order, GP_c_t );
//
//                // this short routine is supposed to calculate position of
//                // Gauss point from 3D array of short's
//                where =
//                ((GP_c_r-1)*integration_order+GP_c_s-1)*integration_order+GP_c_t-1;
//
//                //DB::printf("\n\nBefore Initialization **************** where = %d \n",where);
//                //DB::printf("GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
//                //DB            GP_c_r,GP_c_s,GP_c_t);
//                //DB
//                //DBGPstress[where].reportshort("stress within before Initialization");
//                //DBGPstrain[where].reportshort("strain within before Initialization");
//                //DB
//                //DB// I suspect that [] should be overloaded so that compiler knows which
//                //DB// material model is returning a pointer and fot the purpose
//                //DB//matpoint[where].report("mmodel within before Initialization");
//                //DB//matpoint[where].report("mmodel within before Initialization"); // operator[] overloaded
//                //DB(matpoint)->operator[](where).report("mmodel within before Initialization"); // operator[] overloaded
//                //DB                                                               // for NDMaterial and
//                //DB                                                               // derived types!
//
//                //.matpoint[where].Initialize(GP_c_r,
//                //.                         GP_c_s,
//                //.                         GP_c_t,
//                //.                         r, s, t,
//                //.                         rw, sw, tw,
//                //.                         &( GPstress[where] ),
//                //.                         &( GPstrain[where] ),
//                //.                         &( (matpoint)->operator[](where) )
//                //.                        );      // ugly syntax but it works!
//                //.                                // still don't know what's wrong
//                //.                                // with the old style matpoint[where]
//                // Initialize it with the elastic stiffness tensor at the begining
//                //````double Ey = (matpoint)->operator[](where).E();
//                //````double nu = (matpoint)->operator[](where).nu();
//                //````tensor Constitutive = (matpoint)->operator[](where).StiffnessTensorE(Ey,nu);
//                //````
//                //````GPtangent_E[where].Initialize_all(Constitutive);
//
//    //GPtangent_E[where].Initialize_all(*IN_tangent_E);
//                //GPstress[where].Initialize(*INstress);
//                //GPiterative_stress[where].Initialize(*INiterative_stress);
//                //GPq_ast_iterative[where] = IN_q_ast_iterative[where];
//                //GPstrain[where].Initialize(*INstrain);
//
//    //(matpoint)->operator[](where).Initialize(*Globalmmodel);
//    // Already initialized using Globalmmodel in getCopy() --Zhaohui 09-29-2000
//
//                //NDMaterial *NMD = Globalmmodel->getCopy();
//    //if (strcmp( Globalmmodel->getType(), "Template3Dep" ) == 0)
//     //   NMD = 0;
//
//                //matpoint[where].Initialize(GP_c_r,
//                matpoint[where] = new MatPoint3D(GP_c_r,
//                                           GP_c_s,
//                                           GP_c_t,
//                                           r, s, t,
//                                           rw, sw, tw,
//                                           //InitEPS,
//             Globalmmodel);
//             //NMD );
//                                           // &( GPstress[where] ), //&( GPiterative_stress[where] ), // IN_q_ast_iterative[where],
//                                           // &( GPstrain[where] ),  //&( GPtangent_E[where] ),  // ZHaohui 09-29-2000
//                                           // &( (matpoint)->operator[](where) )
//                                           // ugly syntax but it works! // still don't know what's wrong   // with the old style matpoint[where]
//              }
//          }
//      }
//
//      // Set connected external node IDs
//      connectedExternalNodes(0) = node_numb_1;
//      connectedExternalNodes(1) = node_numb_2;
//      connectedExternalNodes(2) = node_numb_3;
//      connectedExternalNodes(3) = node_numb_4;
//      connectedExternalNodes(4) = node_numb_5;
//      connectedExternalNodes(5) = node_numb_6;
//      connectedExternalNodes(6) = node_numb_7;
//      connectedExternalNodes(7) = node_numb_8;
//
//      //G_N_numbs[0] = node_numb_1;
//      //G_N_numbs[1] = node_numb_2;
//      //G_N_numbs[2] = node_numb_3;
//      //G_N_numbs[3] = node_numb_4;
//      //G_N_numbs[4] = node_numb_5;
//      //G_N_numbs[5] = node_numb_6;
//      //G_N_numbs[6] = node_numb_7;
//      //G_N_numbs[7] = node_numb_8;
//
//      //nodes = GlobalNodes;
//
//      // loop nodes 9-20 and :
//      //  1) put the right coordinates on place,
//      //  2) calculate the total number of nodes
//      // loop nodes 9-20 and :
//      //  1) put the right coordinates on place,
//      //  2) calculate the total number of nodes
//      nodes_in_brick = 8;
//      //    for ( int i=8 ; i<20 ; i++ )
//      //      {
//      //        if (G_N_numbs[i] == 0 )
//      //          {
//      //                   node_existance[i-8] = 0;
//      //          }
//      //        else
//      //          {
//      //            nodes_in_brick++;
//      //            node_existance[i-8] = 1;
//      //          }
//      //      }
//      //    Node * N = new Node[nodes_in_brick];
//      //  // Xiaoyan comment for only 8 nodes  07/11/00
//      return 0;
//  }

//#############################################################################



EightNodeBrick::~EightNodeBrick ()
{

    int total_number_of_Gauss_points = integration_order * integration_order * integration_order;

    for (int i = 0; i < total_number_of_Gauss_points; i++)
    {
        // Delete the NDMaterials at each integration point
        if (matpoint[i])
        {
            delete matpoint[i];
        }
    }

    // Delete the array of pointers to NDMaterial pointer arrays
    if (matpoint)
    {
        delete [] matpoint;
    }

    if (Ki != 0)
    {
        delete Ki;
    }

    //if (mmodel)
    //  delete [] mmodel;

}


void EightNodeBrick::incremental_Update()
{

    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in EightNodeBrick::incremental_Update()" <<endln; this->Print(cerr);
    //BJ//BJ

    double r  = 0.0;
    // double rw = 0.0;
    double s  = 0.0;
    // double sw = 0.0;
    double t  = 0.0;
    // double tw = 0.0;

    short where = 0;
    //,,,,,    double weight = 0.0;

    //double this_one_PP = (matpoint)->operator[](where).IS_Perfect_Plastic();

    int dh_dim[] = {8, 3};  //Xiaoyan changed from {20,3} to {8,3}  07/12/00
    // Optimize BJ
    // make static!
    tensor dh(2, dh_dim, 0.0);

    //    tensor Constitutive( 4, def_dim_4, 0.0);

    //    double det_of_Jacobian = 0.0;

    static int disp_dim[] = {8, 3}; //Xiaoyan changed from {20,3} to {8,3}  07/12/00

    // Optimize BJ
    // make static!
    tensor incremental_displacements(2, disp_dim, 0.0);

    // Optimize BJ
    straintensor incremental_strain;
    //    straintensor total_strain_at_GP;

    // Optimize BJ
    // all of these to be created with proper dimensions and made into static
    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    //....    int number_of_subincrements = 1;
    //....    double this_one_PP = 1.0; // if set to 0.0 -> perfectly plastic
    //....                              // if set to 1.0 -> elasto plastic

    //    stresstensor final_stress_after_integration;

    ///    stresstensor incremental_stress;
    // tensor of incremental displacements taken from node objects
    incremental_displacements = incr_disp();

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        //--        rw = get_Gauss_p_w( integration_order, GP_c_r );
        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            //--            sw = get_Gauss_p_w( integration_order, GP_c_s );
            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                //--                tw = get_Gauss_p_w( integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;
                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");
                // determinant of Jacobian tensor ( matrix )
                //--                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                //dhGlobal = dh("ij") * JacobianINV("jk"); //Zhaohui Yang 09-02-2001
                dhGlobal = dh("ij") * JacobianINV("kj");
                //....                dhGlobal.print("dh","dhGlobal");
                //weight
                //                weight = rw * sw * tw * det_of_Jacobian;
                //::::::   ::printf("\n\nIN THE STIFFNESS TENSOR INTEGRATOR ----**************** where = %d \n", where);
                //::::::   ::printf(" void EightNodeBrick::incremental_Update()\n");
                //::::::   ::printf(" GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d    --->>>  where = %d \n",
                //::::::                      GP_c_r,GP_c_s,GP_c_t,where);
                //::::::   ::printf("WEIGHT = %f", weight);
                //::::::   ::printf("determinant of Jacobian = %f", determinant_of_Jacobian);
                //::::::   matpoint[where].report("Gauss Point\n");
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

                // Getting final_stress_after_integration is  Done inside CDriver on EPState____ZHaohui
                //final_stress_after_integration = GPiterative_stress[where];
                //(matpoint)->operator[](where).kappa_set(final_stress_after_integration,
                //                                 GPq_ast_iterative[where]);

                //....         final_stress_after_integration =
                //....           (matpoint)->operator[](where).FinalStress(*(GPstress+where),
                //....                                                     incremental_strain,
                //....                                                     (matpoint)->operator[](where),
                //....                                                     number_of_subincrements,
                //....                                                     this_one_PP);
                //....//final_stress_after_integration.reportshortpqtheta("\n final_stress_after_integration GAUSS \n");
                // calculate the constitutive tensor

                // We do not need: final_stress_after_integration


                //Constitutive =
                //  (matpoint)->operator[](where).ConstitutiveTensor(final_stress_after_integration,
                //                                                   *(GPstress+where),
                //                                                   incremental_strain,
                //                                                   (matpoint)->operator[](where),
                //                                                   this_one_PP);

                // ZHaohui modified __09-29-2000

                // Now no EPState  but NDMaterial for each MatPoint
                //EPState *tmp_eps = (matpoint[where]).getEPS();
                //NDMaterial *tmp_ndm = (matpoint[where]).getNDMat();

                //if ( tmp_eps ) { //if there is an EPState for the MatPoint3D
                //  mmodel->setEPS( *tmp_eps );

                if ( ( (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)) )
                {
                    cerr << "EightNodeBrick::incremental_Update (tag: " << this->getTag() << "), not converged\n";
                }

                //matpoint[where].setEPS( mmodel->getEPS() );
                //}

                //else if ( tmp_ndm )
                //  (matpoint[where].p_matmodel)->setTrialStrainIncr( incremental_strain );
                //else {
                //   g3ErrorHandler->fatal("EightNodeBrick::incremental_Update (tag: %d), no strain or stress state vars", this->getTag());
                //   exit(1);
                //}

                //Constitutive = trialEPS.getEep();

                //::::::                   Constitutive.print("C","\n\n C tensor \n");
                // this is update of constitutive tensor at this Gauss point

                // All done in EPState when calling setTrialStrainIncr and converged
                //GPtangent_E[where].Initialize(Constitutive);
                //GPtangent_E[where].print("\n tangent E at GAUSS point \n");

                //total_strain_at_GP.Initialize(*(GPstrain+where));
                //total_strain_at_GP.reportshort("\n total_strain tensor at GAUSS point \n");
                //total_strain_at_GP = total_strain_at_GP + incremental_strain;
                //total_strain_at_GP.reportshort("\n total_strain tensor at GAUSS point AFTER\n");
                //GPstress[where].Initialize(final_stress_after_integration);
                //GPstress[where].reportshortpqtheta("\n stress at GAUSS point \n");

                //GPstrain[where].Initialize(total_strain_at_GP);

                //GPstrain[where].reportshort("\n strain at GAUSS point \n");
            }
        }
    }
}

//#############################################################################
//#############################################################################
//***************************************************************
tensor EightNodeBrick::H_3D(double r1, double r2, double r3)
{

    int dimension[] = {24, 3}; // Xiaoyan changed from {60,3} to {24,3} for 8 nodes
    // 3*8=24  07/12/00
    tensor H(2, dimension, 0.0);

    // influence of the node number 8
    H.val(22, 1) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) * 0.125; // - (H.val(43,1)+H.val(48,3)+H.val(60,3))/2.0;
    H.val(23, 2) = H.val(22, 1); //(1.0+r1)*(1.0-r2)*(1.0-r3)*0.125;// - (H.val(43,1)+H.val(48,3)+H.val(60,3))/2.0;
    H.val(24, 3) = H.val(22, 1); //(1.0+r1)*(1.0-r2)*(1.0-r3)*0.125;// - (H.val(43,1)+H.val(48,3)+H.val(60,3))/2.0;
    // influence of the node number 7
    H.val(19, 1) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) * 0.125; // - (H.val(42,3)+H.val(43,1)+H.val(57,3))/2.0;
    H.val(20, 2) = H.val(19, 1); //(1.0-r1)*(1.0-r2)*(1.0-r3)*0.125;// - (H.val(42,3)+H.val(43,1)+H.val(57,3))/2.0;
    H.val(21, 3) = H.val(19, 1); //(1.0-r1)*(1.0-r2)*(1.0-r3)*0.125;// - (H.val(42,3)+H.val(43,1)+H.val(57,3))/2.0;
    // influence of the node number 6
    H.val(16, 1) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) * 0.125; //- (H.val(39,3)+H.val(42,3)+H.val(54,3))/2.0;
    H.val(17, 2) = H.val(16, 1); //(1.0-r1)*(1.0+r2)*(1.0-r3)*0.125;//- (H.val(39,3)+H.val(42,3)+H.val(54,3))/2.0;
    H.val(18, 3) = H.val(16, 1); //(1.0-r1)*(1.0+r2)*(1.0-r3)*0.125;//- (H.val(39,3)+H.val(42,3)+H.val(54,3))/2.0;
    // influence of the node number 5
    H.val(13, 1) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) * 0.125; //- (H.val(39,3)+H.val(48,3)+H.val(51,3))/2.0;
    H.val(14, 2) = H.val(13, 1); //(1.0+r1)*(1.0+r2)*(1.0-r3)*0.125;//- (H.val(39,3)+H.val(48,3)+H.val(51,3))/2.0;
    H.val(15, 3) = H.val(13, 1); //(1.0+r1)*(1.0+r2)*(1.0-r3)*0.125;//- (H.val(39,3)+H.val(48,3)+H.val(51,3))/2.0;

    // influence of the node number 4
    H.val(10, 1) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) * 0.125; //- (H.val(33,3)+H.val(36,3)+H.val(60,3))/2.0;
    H.val(11, 2) = H.val(10, 1); //(1.0+r1)*(1.0-r2)*(1.0+r3)*0.125;//- (H.val(33,3)+H.val(36,3)+H.val(60,3))/2.0;
    H.val(12, 3) = H.val(10, 1); //(1.0+r1)*(1.0-r2)*(1.0+r3)*0.125;//- (H.val(33,3)+H.val(36,3)+H.val(60,3))/2.0;
    // influence of the node number 3
    H.val(7, 1) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) * 0.125; //- (H.val(30,3)+H.val(33,3)+H.val(57,3))/2.0;
    H.val(8, 2) = H.val(7, 1); //(1.0-r1)*(1.0-r2)*(1.0+r3)*0.125;//- (H.val(30,3)+H.val(33,3)+H.val(57,3))/2.0;
    H.val(9, 3) = H.val(7, 1); //(1.0-r1)*(1.0-r2)*(1.0+r3)*0.125;//- (H.val(30,3)+H.val(33,3)+H.val(57,3))/2.0;
    // influence of the node number 2
    H.val(4, 1) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) * 0.125; //- (H.val(30,3)+H.val(54,3)+H.val(27,3))/2.0;
    H.val(5, 2) = H.val(4, 1); //(1.0-r1)*(1.0+r2)*(1.0+r3)*0.125;//- (H.val(30,3)+H.val(54,3)+H.val(27,3))/2.0;
    H.val(6, 3) = H.val(4, 1); //(1.0-r1)*(1.0+r2)*(1.0+r3)*0.125;//- (H.val(30,3)+H.val(54,3)+H.val(27,3))/2.0;
    // influence of the node number 1
    H.val(1, 1) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) * 0.125; //- (H.val(36,3)+H.val(51,3)+H.val(27,3))/2.0;
    H.val(2, 2) = H.val(1, 1); //(1.0+r1)*(1.0+r2)*(1.0+r3)*0.125;//- (H.val(36,3)+H.val(51,3)+H.val(27,3))/2.0;
    H.val(3, 3) = H.val(1, 1); //(1.0+r1)*(1.0+r2)*(1.0+r3)*0.125;//- (H.val(36,3)+H.val(51,3)+H.val(27,3))/2.0;

    // The second part were commented by Xiaoyan
    //         double sum = 0;
    //
    //   for (int i=1; i<=60 ; i++)
    //           {
    // //        sum+=H.cval(i,1);
    //       for (int j=1; j<= 1; j++)
    //          {
    //                    sum+=H.cval(i,1);
    //             ::printf( "  %+9.2e", H.cval(i,j) );
    //           }
    //            // ::printf( "  %d \n", i);
    //      }
    //       ::printf( " \n sum= %+6.2e\n", sum );


    //    printf("r1 = %lf, r2 = %lf, r3 = %lf\n", r1, r2, r3);
    //    H.print("h");

    return H;
}

//#############################################################################
//***************************************************************
tensor EightNodeBrick::interp_poli_at(double r1, double r2, double r3)
{

    int dimension[] = {8};  // Xiaoyan changed from {20} to {8} for 8 nodes 07/12
    tensor h(1, dimension, 0.0);


    // Commented by Xiaoyan

    // influence of the node number 8
    h.val(8) = (1.0 + r1) * (1.0 - r2) * (1.0 - r3) / 8.0; // - (h.val(15)+h.val(16)+h.val(20))/2.0;
    // influence of the node number 7
    h.val(7) = (1.0 - r1) * (1.0 - r2) * (1.0 - r3) / 8.0; // - (h.val(14)+h.val(15)+h.val(19))/2.0;
    // influence of the node number 6
    h.val(6) = (1.0 - r1) * (1.0 + r2) * (1.0 - r3) / 8.0; // - (h.val(13)+h.val(14)+h.val(18))/2.0;
    // influence of the node number 5
    h.val(5) = (1.0 + r1) * (1.0 + r2) * (1.0 - r3) / 8.0; // - (h.val(13)+h.val(16)+h.val(17))/2.0;

    // influence of the node number 4
    h.val(4) = (1.0 + r1) * (1.0 - r2) * (1.0 + r3) / 8.0; // - (h.val(11)+h.val(12)+h.val(20))/2.0;
    // influence of the node number 3
    h.val(3) = (1.0 - r1) * (1.0 - r2) * (1.0 + r3) / 8.0; // - (h.val(10)+h.val(11)+h.val(19))/2.0;
    // influence of the node number 2
    h.val(2) = (1.0 - r1) * (1.0 + r2) * (1.0 + r3) / 8.0; // - (h.val(10)+h.val(18)+h.val(9))/2.0;
    // influence of the node number 1
    h.val(1) = (1.0 + r1) * (1.0 + r2) * (1.0 + r3) / 8.0; // - (h.val(12)+h.val(17)+h.val(9))/2.0;

    return h;
}



tensor EightNodeBrick::dh_drst_at(double r1, double r2, double r3)
{

    int dimensions[] = {8, 3}; // Changed from{20,3} to {8,3} Xiaoyan 07/12
    tensor dh(2, dimensions, 0.0);


    // influence of the node number 8
    dh.val(8, 1) = (1.0 - r2) * (1.0 - r3) * 0.125; ///8.0;// - (dh.val(15,1)+dh.val(16,1)+dh.val(20,1))/2.0;
    dh.val(8, 2) = -(1.0 + r1) * (1.0 - r3) * 0.125; ///8.0;// - (dh.val(15,2)+dh.val(16,2)+dh.val(20,2))/2.0;
    dh.val(8, 3) = -(1.0 + r1) * (1.0 - r2) * 0.125; ///8.0;// - (dh.val(15,3)+dh.val(16,3)+dh.val(20,3))/2.0;
    // influence of the node number 7
    dh.val(7, 1) = -(1.0 - r2) * (1.0 - r3) * 0.125; ///8.0;// - (dh.val(14,1)+dh.val(15,1)+dh.val(19,1))/2.0;
    dh.val(7, 2) = -(1.0 - r1) * (1.0 - r3) * 0.125; ///8.0;// - (dh.val(14,2)+dh.val(15,2)+dh.val(19,2))/2.0;
    dh.val(7, 3) = -(1.0 - r1) * (1.0 - r2) * 0.125; ///8.0;// - (dh.val(14,3)+dh.val(15,3)+dh.val(19,3))/2.0;
    // influence of the node number 6
    dh.val(6, 1) = -(1.0 + r2) * (1.0 - r3) * 0.125; ///8.0;// - (dh.val(13,1)+dh.val(14,1)+dh.val(18,1))/2.0;
    dh.val(6, 2) = (1.0 - r1) * (1.0 - r3) * 0.125; ///8.0;// - (dh.val(13,2)+dh.val(14,2)+dh.val(18,2))/2.0;
    dh.val(6, 3) = -(1.0 - r1) * (1.0 + r2) * 0.125; ///8.0;//- (dh.val(13,3)+dh.val(14,3)+dh.val(18,3))/2.0;
    // influence of the node number 5
    dh.val(5, 1) = (1.0 + r2) * (1.0 - r3) * 0.125; ///8.0;// - (dh.val(13,1)+dh.val(16,1)+dh.val(17,1))/2.0;
    dh.val(5, 2) = (1.0 + r1) * (1.0 - r3) * 0.125; ///8.0;// - (dh.val(13,2)+dh.val(16,2)+dh.val(17,2))/2.0;
    dh.val(5, 3) = -(1.0 + r1) * (1.0 + r2) * 0.125; ///8.0;// - (dh.val(13,3)+dh.val(16,3)+dh.val(17,3))/2.0;

    // influence of the node number 4
    dh.val(4, 1) = (1.0 - r2) * (1.0 + r3) * 0.125; ///8.0;// - (dh.val(11,1)+dh.val(12,1)+dh.val(20,1))/2.0;
    dh.val(4, 2) = -(1.0 + r1) * (1.0 + r3) * 0.125; ///8.0;// - (dh.val(11,2)+dh.val(12,2)+dh.val(20,2))/2.0;
    dh.val(4, 3) = (1.0 + r1) * (1.0 - r2) * 0.125; ///8.0;// - (dh.val(11,3)+dh.val(12,3)+dh.val(20,3))/2.0;
    // influence of the node number 3
    dh.val(3, 1) = -(1.0 - r2) * (1.0 + r3) * 0.125; ///8.0;// - (dh.val(10,1)+dh.val(11,1)+dh.val(19,1))/2.0;
    dh.val(3, 2) = -(1.0 - r1) * (1.0 + r3) * 0.125; ///8.0;// - (dh.val(10,2)+dh.val(11,2)+dh.val(19,2))/2.0;
    dh.val(3, 3) = (1.0 - r1) * (1.0 - r2) * 0.125; ///8.0;// - (dh.val(10,3)+dh.val(11,3)+dh.val(19,3))/2.0;
    // influence of the node number 2
    dh.val(2, 1) = -(1.0 + r2) * (1.0 + r3) * 0.125; ///8.0;// - (dh.val(10,1)+dh.val(18,1)+dh.val(9,1))/2.0;
    dh.val(2, 2) = (1.0 - r1) * (1.0 + r3) * 0.125; ///8.0;// - (dh.val(10,2)+dh.val(18,2)+dh.val(9,2))/2.0;
    dh.val(2, 3) = (1.0 - r1) * (1.0 + r2) * 0.125; ///8.0;// - (dh.val(10,3)+dh.val(18,3)+dh.val(9,3))/2.0;
    // influence of the node number 1
    dh.val(1, 1) = (1.0 + r2) * (1.0 + r3) * 0.125; ///8.0;// - (dh.val(12,1)+dh.val(17,1)+dh.val(9,1))/2.0;
    dh.val(1, 2) = (1.0 + r1) * (1.0 + r3) * 0.125; ///8.0;// - (dh.val(12,2)+dh.val(17,2)+dh.val(9,2))/2.0;
    dh.val(1, 3) = (1.0 + r1) * (1.0 + r2) * 0.125; ///8.0;//- (dh.val(12,3)+dh.val(17,3)+dh.val(9,3))/2.0;
    // Commented by Xiaoyan
    return dh;
}

////#############################################################################
EightNodeBrick &EightNodeBrick::operator[](int subscript)
{
    return ( *(this + subscript) );
}

//Finite_Element & EightNodeBrick::operator[](short subscript)
//  {
//    return ( *(this+subscript) );
//  }

//Finite_Element & EightNodeBrick::operator[](unsigned subscript)
//  {
//    return ( *(this+subscript) );
//  }


////#############################################################################
tensor EightNodeBrick::getStiffnessTensor(void)
{


    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in tensor EightNodeBrick::getStiffnessTensor(void) " <<endln; this->Print(cerr);
    //BJ//BJ

    int K_dim[] = {8, 3, 3, 8}; // Xiaoyan changed from {20,3,3,20} to {8,3,3,8}
    // for 8 nodes      07/12
    tensor Kk(4, K_dim, 0.0);  // Optimize BJ
    tensor Kkt(4, K_dim, 0.0); // all of these to be created with proper dimensions and made into static

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

    int dh_dim[] = {8, 3}; // Xiaoyan changed from {20,3} to {8,3}
    tensor dh(2, dh_dim, 0.0);

    //    tensor Constitutive( 4, def_dim_4, 0.0);
    tensor Constitutive;

    double det_of_Jacobian = 0.0;

    //     static int disp_dim[] = {8,3};   // Xiaoyan changed from {20,3} to {8,3}
    //     tensor incremental_displacements(2,disp_dim,0.0); // \Delta u

    straintensor incremental_strain;
    //    straintensor total_strain_at_GP;

    tensor Jacobian;    // Optimize BJ
    tensor JacobianINV; // all of these to be created with proper dimensions and made into static
    //     tensor JacobianINVtemp;
    tensor dhGlobal;

    //double tmp= 0;


    // tensor of incremental displacements taken from node objects
    //incremental_displacements = incr_disp();
    //cerr << "\n=======" << getTag() << "===========\n";
    //incremental_displacements.print("incr_disp");

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                //dh.print("dh");
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //Jacobian.print("J");

                // Inverse of Jacobian tensor ( matrix )
                //JacobianINV = Jacobian_3Dinv(dh);
                JacobianINV = Jacobian.inverse();
                //JacobianINVtemp = JacobianINVtemp - JacobianINV;
                //JacobianINV.print();
                //JacobianINVtemp.print();

                //JacobianINV = Jacobian.inverse(); //Zhaohui 08-31-2001


                // determinant of Jacobian tensor ( matrix )
                det_of_Jacobian  = Jacobian.determinant();

                //      cout << "Det of Jacobian is: " << det_of_Jacobian << endl;

                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("kj");
                //        ::fprintf(stdout," # %d \n\n\n\n\n\n\n\n", El_count);
                //dhGlobal.print("dhGlobal");
                //weight
                weight = rw * sw * tw * det_of_Jacobian;

                //      cout << "weight is: " << weight << endl;
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
                //matpoint[where].report("Gauss Point\n");

                // incremental straines at this Gauss point
                //GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor1\n");

                //incremental_strain =
                //     (dhGlobal("ib")*incremental_displacements("ia")).symmetrize11();

                //incremental_strain.null_indices();
                //incremental_strain.report("\n incremental_strain tensor at GAUSS point\n");

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

                //final_stress_after_integration =
                //    (matpoint)->operator[](where).FinalStress(*(GPstress+where),
                //                                 incremental_strain,
                //                                 (matpoint)->operator[](where),
                //                                 number_of_subincrements,
                //                                 this_one_PP);
                //final_stress_after_integration.reportshortpqtheta("\n final_stress_after_integration in stiffness_tensor5\n");

                //----   GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor3\n");
                //final_stress_after_integration.reportshortpqtheta("\n final_stress_after_integration GAUSS \n");
                //----   GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor4\n");

                // this final stress after integration is used ONLY to obtain Constitutive tensor
                // at this Gauss point AND to set up the iterative_stress that is used in calculting
                // internal forces during iterations!!

                //GPiterative_stress[where].Initialize(final_stress_after_integration);
                //GPiterative_stress[where].reportshortpqtheta("\n iterative_stress at GAUSS point in stiffness_tensor5\n");


                // Stress state at Gauss point will be updated ( in the
                // sense of updating stresses ( integrating them ) ) in function Update (...) ! ! ! ! !
                // calculate the constitutive tensor
                //......         Constitutive =  GPtangent_E[where];

                //Constitutive =  (matpoint)->operator[](where).ConstitutiveTensor(final_stress_after_integration,
                //                                         *(GPstress+where),
                //                                          incremental_strain,
                //                                          (matpoint)->operator[](where),
                //                                          this_one_PP);
                //Constitutive.print("C","\n\n C tensor \n");

                //EPState *tmp_eps = (matpoint[where]).getEPS();
                //NDMaterial *tmp_ndm = (matpoint[where]).getNDMat();

                //if ( tmp_eps ) {     //Elasto-plastic case
                //    mmodel->setEPS( *tmp_eps );

                //int err = ( matpoint[where]->matmodel )->setTrialStrainIncr( incremental_strain);
                //if ( err)
                //   cerr << "EightNodeBrick::getStiffnessTensor (tag: %d), not converged",
                //         this->getTag());

                Constitutive = (matpoint[where]->matmodel)->getTangentTensor();
                //Constitutive.print("C","\n\n C tensor \n");

                //    matpoint[where].setEPS( mmodel->getEPS() );
                //}
                //else if ( tmp_ndm ) { //Elastic case
                //    (matpoint[where].p_matmodel)->setTrialStrainIncr( incremental_strain );
                //    Constitutive = (matpoint[where].p_matmodel)->getTangentTensor();
                //}
                //else {
                //   g3ErrorHandler->fatal("EightNodeBrick::incremental_Update (tag: %d), could not getTangentTensor", this->getTag());
                //   exit(1);
                //}

                //printf("Constitutive.trace = %12.6e\n", Constitutive.trace());
                //Kmat = this->stiffness_matrix(Constitutive);
                //printf("Constitutive tensor max:= %10.3e\n", Kmat.mmax());

                //----   GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor5\n");
                // this is update of constitutive tensor at this Gauss point
                //GPtangent_E[where].Initialize(Constitutive);
                //....GPtangent_E[where].print(" tangent E at GAUSS point");

                //GPstress[where].reportshortpqtheta("\n stress at GAUSS point in stiffness_tensor6\n");

                //K = K + temp2;

                //Constitutive.print();

                Kkt = dhGlobal("ib") * Constitutive("abcd");
                Kkt = Kkt("aicd") * dhGlobal("jd") * weight;
                Kk = Kk + Kkt;

                //Kk = Kk + dhGlobal("ib")*Constitutive("abcd")*dhGlobal("jd")*weight;
                //....K.print("K","\n\n K tensor \n");

                //Kmat = this->stiffness_matrix(Kkt);
                //Kmat.print("Kmat");

                //printf("K tensor max= %10.3e\n", Kmat.mmax());

                //convert constitutive and K to matrix and find min and max and print!



            }
        }
    }

    // Kk.print("K","\n\n K tensor \n");
    //K = Kk;
    return Kk;
}


//#############################################################################

void EightNodeBrick::set_strain_stress_tensor(FILE *fp, double *u)
{

    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in void EightNodeBrick::set_strain_stress_tensor" <<endln; this->Print(cerr);
    //BJ//BJ

    int dh_dim[] = {8, 3};  // Xiaoyan changed from {20,3} to {8,3}
    tensor dh(2, dh_dim, 0.0);

    //    tensor Constitutive( 4, def_dim_4, 0.0);
    tensor Constitutive;
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;
    int where = 0;

    //     double det_of_Jacobian;

    straintensor strain;
    stresstensor stress;

    //     tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;


    static int disp_dim[] = {8, 3};   // Xiaoyan changed from {20,3} to {8,3}
    tensor total_displacements(2, disp_dim, 0.0); //

    total_displacements = total_disp(fp, u);

    ::printf("\n  displacement(x-y-z) at GAUSS pt %d \n\n", where + 1);

    for (int ii = 1; ii <= 8; ii++)
    {
        ::printf("Global# %d Local#%d  %+0.5e %+0.5e %+0.5e\n",
                 //G_N_numbs[ii-1],
                 connectedExternalNodes(ii - 1),
                 ii, total_displacements.val(ii, 1),
                 total_displacements.val(ii, 2),
                 total_displacements.val(ii, 3));
    }

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                //                 Jacobian = Jacobian_3D(dh);
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                // determinant of Jacobian tensor ( matrix )
                //                 det_of_Jacobian  = Jacobian.determinant();
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("kj");
                //weight
                // straines at this Gauss point from displacement
                strain = (dhGlobal("ib") * total_displacements("ia")).symmetrize11();
                strain.null_indices();
                // here comes the final_stress calculation
                // at this Gauss point.

                //Constitutive =  GPtangent_E[where];
                //Constitutive =  (matpoint->getEPS() )->getEep();
                // if set total displ, then it should be elstic material
                Constitutive =  ( matpoint[where]->matmodel)->getTangentTensor();

                stress = Constitutive("ijkl") * strain("kl");   //<<<<<<<<<<<<<<<
                stress.null_indices();

                ::printf("\n  strain tensor at GAUSS point %d \n", where + 1);
                strain.reportshort("");
                ::printf("\n  stress tensor at GAUSS point %d \n", where + 1);
                stress.reportshort("");


            }
        }
    }
}



tensor EightNodeBrick::stiffness_matrix(const tensor &K)
{
    //BJ/BJ
    //BJ    cerr << "\n\n\n\n Print in tensor EightNodeBrick::stiffness_matrix" <<endln; this->Print(cerr);
    //BJ//BJ
    //    int K_dim[] = {20,3,3,20};
    //    tensor K(4,K_dim,0.0);
    matrix Kmatrix(24, 24, 0.0); // Xiaoyan changed from (60,60,0,0) to (24,24,0,0)

    int Ki = 0;
    int Kj = 0;

    for ( int i = 1 ; i <= 8 ; i++ ) // Xiaoyan changed from i<=20 to i<=8 for 8 nodes
    {
        for ( int j = 1 ; j <= 8 ; j++ ) // Xiaoyan changed from i<=20 to i<=8 for 8 nodes
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
tensor EightNodeBrick::mass_matrix(const tensor &M)
{
    //    int K_dim[] = {20,3,3,20};
    //    tensor K(4,K_dim,0.0);
    matrix Mmatrix(24, 24, 0.0); // Xiaoyan changed from (60,60,0,0) to (24,24,0,0) for 8 nodes

    for ( int i = 1 ; i <= 24 ; i++ ) // Xiaoyan changed from i<=60 to i<=24 for 8 nodes
    {
        for ( int j = 1 ; j <= 24 ; j++ ) // Xiaoyan changed from i<=60 to i<=24 for 8 nodes
        {
            Mmatrix.val( i , j ) = M.cval(i, j);
            //  ::printf("Mi Mj %d %d %+6.2e ",Mi,Mj,Mmatrix.val( Mi , Mj ) );
        }
    }

    return Mmatrix;
}
////#############################################################################

////#############################################################################
tensor EightNodeBrick::Jacobian_3D(tensor dh)
{
    //       dh ( 20*3)  // dh(8*3) Xiaoyan
    tensor N_C = Nodal_Coordinates(); // 20*3    // 8*3 Xiaoyan
    tensor Jacobian_3D = dh("ij") * N_C("ik");
    return Jacobian_3D;
}

//#############################################################################
tensor EightNodeBrick::Jacobian_3Dinv(tensor dh)
{
    //       dh ( 20*3)    // dh(8*3) Xiaoyan
    tensor N_C = Nodal_Coordinates(); // 20*3        // 8*3 Xiaoyan
    tensor Jacobian_3Dinv = (dh("ij") * N_C("ik")).inverse();
    return Jacobian_3Dinv;
}


////#############################################################################
tensor EightNodeBrick::Nodal_Coordinates(void)
{
    const int dimensions[] = {8, 3}; // Xiaoyan changed from {20,3} to {8,3} for 8 nodes
    tensor N_coord(2, dimensions, 0.0);

    //for ( int i=0 ; i<8 ; i++ )    // Xiaoyan changed from 20 to 8 for 8 nodes
    //  {
    //    //        N_coord.val(i+1,1) = nodes[ G_N_numbs[i] ].x_coordinate();
    //    //        N_coord.val(i+1,2) = nodes[ G_N_numbs[i] ].y_coordinate();
    //    //        N_coord.val(i+1,3) = nodes[ G_N_numbs[i] ].z_coordinate();
    //    // Xiaoyan changed to the following:  09/27/00
    //    /// LOOK WITH DDD
    //  Vector Coordinates = nodes[ G_N_numbs[i] ].getCrds();
    //    N_coord.val(i+1,1) = Coordinates(0);
    //    N_coord.val(i+1,2) = Coordinates(1);
    //    N_coord.val(i+1,3) = Coordinates(2);
    //  }

    //Zhaohui using node pointers, which come from the Domain
    const Vector &nd1Crds = theNodes[0]->getCrds();
    const Vector &nd2Crds = theNodes[1]->getCrds();
    const Vector &nd3Crds = theNodes[2]->getCrds();
    const Vector &nd4Crds = theNodes[3]->getCrds();
    const Vector &nd5Crds = theNodes[4]->getCrds();
    const Vector &nd6Crds = theNodes[5]->getCrds();
    const Vector &nd7Crds = theNodes[6]->getCrds();
    const Vector &nd8Crds = theNodes[7]->getCrds();

    N_coord.val(1, 1) = nd1Crds(0);
    N_coord.val(1, 2) = nd1Crds(1);
    N_coord.val(1, 3) = nd1Crds(2);
    N_coord.val(2, 1) = nd2Crds(0);
    N_coord.val(2, 2) = nd2Crds(1);
    N_coord.val(2, 3) = nd2Crds(2);
    N_coord.val(3, 1) = nd3Crds(0);
    N_coord.val(3, 2) = nd3Crds(1);
    N_coord.val(3, 3) = nd3Crds(2);
    N_coord.val(4, 1) = nd4Crds(0);
    N_coord.val(4, 2) = nd4Crds(1);
    N_coord.val(4, 3) = nd4Crds(2);
    N_coord.val(5, 1) = nd5Crds(0);
    N_coord.val(5, 2) = nd5Crds(1);
    N_coord.val(5, 3) = nd5Crds(2);
    N_coord.val(6, 1) = nd6Crds(0);
    N_coord.val(6, 2) = nd6Crds(1);
    N_coord.val(6, 3) = nd6Crds(2);
    N_coord.val(7, 1) = nd7Crds(0);
    N_coord.val(7, 2) = nd7Crds(1);
    N_coord.val(7, 3) = nd7Crds(2);
    N_coord.val(8, 1) = nd8Crds(0);
    N_coord.val(8, 2) = nd8Crds(1);
    N_coord.val(8, 3) = nd8Crds(2);

    //N_coord.print();

    return N_coord;
}

////#############################################################################
tensor EightNodeBrick::incr_disp(void)
{
    const int dimensions[] = {8, 3}; // Xiaoyan changed from {20,3} to {8,3} for 8 nodes
    tensor increment_disp(2, dimensions, 0.0);

    //for ( int i=0 ; i<8 ; i++ )   // Xiaoyan changed from 20 to 8 for 8 nodes
    //
    //  {
    //    // increment_disp.val(i+1,1) = nodes[ G_N_numbs[i] ].incremental_translation_x();
    //    // increment_disp.val(i+1,2) = nodes[ G_N_numbs[i] ].incremental_translation_y();
    //    // increment_disp.val(i+1,3) = nodes[ G_N_numbs[i] ].incremental_translation_z();
    //    // Xiaoyan changed to the following 09/27/00
    //    Vector IncremenDis = nodes[ G_N_numbs[i] ].getIncrDisp();
    //
    //    increment_disp.val(i+1,1) = IncremenDis(0);
    //    increment_disp.val(i+1,2) = IncremenDis(1);
    //    increment_disp.val(i+1,3) = IncremenDis(2);
    //
    //  }

    //Zhaohui using node pointers, which come from the Domain
    //const Vector &TotDis1 = theNodes[0]->getTrialDisp();
    //const Vector &incrdelDis1 = theNodes[0]->getIncrDisp();
    //Have to get IncrDeltaDisp, not IncrDisp for cumulation of incr_disp
    const Vector &IncrDis1 = theNodes[0]->getIncrDeltaDisp();
    const Vector &IncrDis2 = theNodes[1]->getIncrDeltaDisp();
    const Vector &IncrDis3 = theNodes[2]->getIncrDeltaDisp();
    const Vector &IncrDis4 = theNodes[3]->getIncrDeltaDisp();
    const Vector &IncrDis5 = theNodes[4]->getIncrDeltaDisp();
    const Vector &IncrDis6 = theNodes[5]->getIncrDeltaDisp();
    const Vector &IncrDis7 = theNodes[6]->getIncrDeltaDisp();
    const Vector &IncrDis8 = theNodes[7]->getIncrDeltaDisp();


    //if ( getTag() == 486 || getTag() == 566 || getTag() == 956)
    //{
    //cerr <<" \n\n element " << getTag() << endlnn;
    //cerr<<"\n tot node " << theNodes[0]->getTag() <<" x "<< TotDis1(0) <<" y "<< TotDis1(1) << " z "<< TotDis1(2);
    //
    //cerr<<"\n incr node " << theNodes[0]->getTag() <<" x "<< incrdelDis1(0) <<" y "<< incrdelDis1(1) << " z "<< incrdelDis1(2);
    //
    //cerr << "\nincr del node " << theNodes[0]->getTag() << " x " << IncrDis1(0) <<" y "<< IncrDis1(1) << " z "<< IncrDis1(2) << endlnn;
    //
    //cerr << " incr del node " << theNodes[1]->getTag() << " x " << IncrDis2(0) <<" y "<< IncrDis2(1) << " z "<< IncrDis2(2) << endlnn;
    //
    //cerr << " incr del node " << theNodes[2]->getTag() << " x " << IncrDis3(0) <<" y "<< IncrDis3(1) << " z "<< IncrDis3(2) << endlnn;
    //
    //cerr << " incr del node " << theNodes[3]->getTag() << " x " << IncrDis4(0) <<" y "<< IncrDis4(1) << " z "<< IncrDis4(2) << endlnn;
    //
    //cerr << " incr del node " << theNodes[4]->getTag() << " x " << IncrDis5(0) <<" y "<< IncrDis5(1) << " z "<< IncrDis5(2) << endlnn;
    //cerr << " incr del node " << theNodes[5]->getTag() << " x " << IncrDis6(0) <<" y "<< IncrDis6(1) << " z "<< IncrDis6(2) << endlnn;
    //cerr << " incr del node " << theNodes[6]->getTag() << " x " << IncrDis7(0) <<" y "<< IncrDis7(1) << " z "<< IncrDis7(2) << endlnn;
    //cerr << " incr del node " << theNodes[7]->getTag() << " x " << IncrDis8(0) <<" y "<< IncrDis8(1) << " z "<< IncrDis8(2) << endlnn;
    //}

    increment_disp.val(1, 1) = IncrDis1(0);
    increment_disp.val(1, 2) = IncrDis1(1);
    increment_disp.val(1, 3) = IncrDis1(2);
    increment_disp.val(2, 1) = IncrDis2(0);
    increment_disp.val(2, 2) = IncrDis2(1);
    increment_disp.val(2, 3) = IncrDis2(2);
    increment_disp.val(3, 1) = IncrDis3(0);
    increment_disp.val(3, 2) = IncrDis3(1);
    increment_disp.val(3, 3) = IncrDis3(2);
    increment_disp.val(4, 1) = IncrDis4(0);
    increment_disp.val(4, 2) = IncrDis4(1);
    increment_disp.val(4, 3) = IncrDis4(2);
    increment_disp.val(5, 1) = IncrDis5(0);
    increment_disp.val(5, 2) = IncrDis5(1);
    increment_disp.val(5, 3) = IncrDis5(2);
    increment_disp.val(6, 1) = IncrDis6(0);
    increment_disp.val(6, 2) = IncrDis6(1);
    increment_disp.val(6, 3) = IncrDis6(2);
    increment_disp.val(7, 1) = IncrDis7(0);
    increment_disp.val(7, 2) = IncrDis7(1);
    increment_disp.val(7, 3) = IncrDis7(2);
    increment_disp.val(8, 1) = IncrDis8(0);
    increment_disp.val(8, 2) = IncrDis8(1);
    increment_disp.val(8, 3) = IncrDis8(2);

    return increment_disp;
}

////#############################################################################
tensor EightNodeBrick::total_disp(void)
{
    const int dimensions[] = {8, 3}; // Xiaoyan changed from {20,3} to {8,3} for 8 nodes
    tensor total_disp(2, dimensions, 0.0);

    //Zhaohui using node pointers, which come from the Domain
    const Vector &TotDis1 = theNodes[0]->getTrialDisp();
    //cerr << "Element: " << this->getTag() << endln;
    //cerr<<"\ntot node " << theNodes[0]->getTag() <<" x "<< TotDis1(0) <<" y "<< TotDis1(1) << " z "<< TotDis1(2) << endln;
    const Vector &TotDis2 = theNodes[1]->getTrialDisp();
    //cerr << "tot node " << theNodes[1]->getTag() << " x " << TotDis2(0) <<" y "<< TotDis2(1) << " z "<< TotDis2(2) << endln;
    const Vector &TotDis3 = theNodes[2]->getTrialDisp();
    //cerr << "tot node " << theNodes[2]->getTag() << " x " << TotDis3(0) <<" y "<< TotDis3(1) << " z "<< TotDis3(2) << endln;
    const Vector &TotDis4 = theNodes[3]->getTrialDisp();
    //cerr << "tot node " << theNodes[3]->getTag() << " x " << TotDis4(0) <<" y "<< TotDis4(1) << " z "<< TotDis4(2) << endln;
    const Vector &TotDis5 = theNodes[4]->getTrialDisp();
    //cerr << "tot node " << theNodes[4]->getTag() << " x " << TotDis5(0) <<" y "<< TotDis5(1) << " z "<< TotDis5(2) << endln;
    const Vector &TotDis6 = theNodes[5]->getTrialDisp();
    //cerr << "tot node " << theNodes[5]->getTag() << " x " << TotDis6(0) <<" y "<< TotDis6(1) << " z "<< TotDis6(2) << endln;
    const Vector &TotDis7 = theNodes[6]->getTrialDisp();
    //cerr << "tot node " << theNodes[6]->getTag() << " x " << TotDis7(0) <<" y "<< TotDis7(1) << " z "<< TotDis7(2) << endln;
    const Vector &TotDis8 = theNodes[7]->getTrialDisp();
    //cerr << "tot node " << theNodes[7]->getTag() << " x " << TotDis8(0) <<" y "<< TotDis8(1) << " z "<< TotDis8(2) << endln;

    total_disp.val(1, 1) = TotDis1(0);
    total_disp.val(1, 2) = TotDis1(1);
    total_disp.val(1, 3) = TotDis1(2);
    total_disp.val(2, 1) = TotDis2(0);
    total_disp.val(2, 2) = TotDis2(1);
    total_disp.val(2, 3) = TotDis2(2);
    total_disp.val(3, 1) = TotDis3(0);
    total_disp.val(3, 2) = TotDis3(1);
    total_disp.val(3, 3) = TotDis3(2);
    total_disp.val(4, 1) = TotDis4(0);
    total_disp.val(4, 2) = TotDis4(1);
    total_disp.val(4, 3) = TotDis4(2);
    total_disp.val(5, 1) = TotDis5(0);
    total_disp.val(5, 2) = TotDis5(1);
    total_disp.val(5, 3) = TotDis5(2);
    total_disp.val(6, 1) = TotDis6(0);
    total_disp.val(6, 2) = TotDis6(1);
    total_disp.val(6, 3) = TotDis6(2);
    total_disp.val(7, 1) = TotDis7(0);
    total_disp.val(7, 2) = TotDis7(1);
    total_disp.val(7, 3) = TotDis7(2);
    total_disp.val(8, 1) = TotDis8(0);
    total_disp.val(8, 2) = TotDis8(1);
    total_disp.val(8, 3) = TotDis8(2);

    return total_disp;
}


////#############################################################################
tensor EightNodeBrick::total_disp(FILE *fp, double *u)
{
    const int dimensions[] = {8, 3}; // Xiaoyan changed from {20,3} to {8,3} for 8 nodes
    tensor total_disp(2, dimensions, 0.0);
    //    double totalx, totaly, totalz;
    //    totalx=0;
    //    totaly=0;
    //    totalz=0;

    //for ( int i=0 ; i<8 ; i++ )  // Xiaoyan changed from 20 to 8 for 8 nodes
    //
    //  {
    //    // total_disp.val(i+1,1) = nodes[ G_N_numbs[i] ].total_translation_x(u);
    //    // total_disp.val(i+1,2) = nodes[ G_N_numbs[i] ].total_translation_y(u);
    //    // total_disp.val(i+1,3) = nodes[ G_N_numbs[i] ].total_translation_z(u);
    //    // Xiaoyan changed to the following 09/27/00
    //    Vector TotalTranDis = nodes[ G_N_numbs[i] ].getDisp();
    //
    //    total_disp.val(i+1,1) = TotalTranDis(0);
    //  total_disp.val(i+1,2) = TotalTranDis(1);
    //    total_disp.val(i+1,3) = TotalTranDis(2);
    //
    //  }

    //Zhaohui using node pointers, which come from the Domain
    const Vector &TotDis1 = theNodes[0]->getTrialDisp();
    const Vector &TotDis2 = theNodes[1]->getTrialDisp();
    const Vector &TotDis3 = theNodes[2]->getTrialDisp();
    const Vector &TotDis4 = theNodes[3]->getTrialDisp();
    const Vector &TotDis5 = theNodes[4]->getTrialDisp();
    const Vector &TotDis6 = theNodes[5]->getTrialDisp();
    const Vector &TotDis7 = theNodes[6]->getTrialDisp();
    const Vector &TotDis8 = theNodes[7]->getTrialDisp();

    total_disp.val(1, 1) = TotDis1(0);
    total_disp.val(1, 2) = TotDis1(1);
    total_disp.val(1, 3) = TotDis1(2);
    total_disp.val(2, 1) = TotDis2(0);
    total_disp.val(2, 2) = TotDis2(1);
    total_disp.val(2, 3) = TotDis2(2);
    total_disp.val(3, 1) = TotDis3(0);
    total_disp.val(3, 2) = TotDis3(1);
    total_disp.val(3, 3) = TotDis3(2);
    total_disp.val(4, 1) = TotDis4(0);
    total_disp.val(4, 2) = TotDis4(1);
    total_disp.val(4, 3) = TotDis4(2);
    total_disp.val(5, 1) = TotDis5(0);
    total_disp.val(5, 2) = TotDis5(1);
    total_disp.val(5, 3) = TotDis5(2);
    total_disp.val(6, 1) = TotDis6(0);
    total_disp.val(6, 2) = TotDis6(1);
    total_disp.val(6, 3) = TotDis6(2);
    total_disp.val(7, 1) = TotDis7(0);
    total_disp.val(7, 2) = TotDis7(1);
    total_disp.val(7, 3) = TotDis7(2);
    total_disp.val(8, 1) = TotDis8(0);
    total_disp.val(8, 2) = TotDis8(1);
    total_disp.val(8, 3) = TotDis8(2);

    return total_disp;
}


////#############################################################################
int EightNodeBrick::get_global_number_of_node(int local_node_number)
{
    //return G_N_numbs[local_node_number];
    return connectedExternalNodes(local_node_number);
}

////#############################################################################
int  EightNodeBrick::get_Brick_Number(void)
{
    //return elem_numb;
    return this->getTag();
}

////#############################################################################
int *EightNodeBrick::get_LM(void)
{
    return LM;
}

////#############################################################################
// returns nodal forces for given stress field in an element
tensor EightNodeBrick::nodal_forces(void)
{

    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in tensor EightNodeBrick::nodal_forces(" <<endln; this->Print(cerr);
    //BJ//BJ

    int force_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor dh(2, dh_dim, 0.0);

    stresstensor stress_at_GP(0.0);

    double det_of_Jacobian = 0.0;

    //straintensor incremental_strain;

    //static int disp_dim[] = {8,3};   // Xiaoyan changed from {20,3} to {8,3}
    //tensor incremental_displacements(2,disp_dim,0.0); // \Delta u

    //incremental_displacements = incr_disp();

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

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
                dhGlobal = dh("ij") * JacobianINV("kj");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;
                //cerr << " UCD det_of_Jacobian "<< det_of_Jacobian << endln;
                //..::printf("\n\nIN THE nodal forces ----**************** where = %d \n", where);
                //..::printf("                    GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                //..                           GP_c_r,GP_c_s,GP_c_t);
                //..::printf("WEIGHT = %f", weight);
                //..::printf("determinant of Jacobian = %f", det_of_Jacobian);
                //..matpoint[where].report("Gauss Point\n");

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
                ////                integr_type = (matpoint)->operator[](where).integration_type();
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
                //stress_at_GP = GPstress[where];

                //EPState *tmp_eps = (matpoint[where]->matmodel)->getEPS();
                //stress_at_GP = tmp_eps->getStress();
                //cerr << "tmp_eps" << (*tmp_eps);

                //NDMaterial *tmp_ndm = (matpoint[where]).getNDMat();

                //if ( tmp_eps ) {     //Elasto-plastic case

                //stress_at_GP = (matpoint[where].matmodel->getEPS())->getStress();

                //   EPState *tmp_eps = (matpoint[where]->matmodel)->getEPS();
                //   stress_at_GP = tmp_eps->getStress();



                //Guanzhou out 5-6-2004    incremental_strain =
                //Guanzhou out 5-6-2004                     (dhGlobal("ib")*incremental_displacements("ia")).symmetrize11();
                //Guanzhou out 5-6-2004//    if (where == 0)
                //Guanzhou out 5-6-2004//       //cerr << " In nodal_force delta_incremental_strain tag "<< getTag() <<"  " <<incremental_strain << endln;
                //Guanzhou out 5-6-2004////    cerr << " el tag = "<< getTag();
                //Guanzhou out 5-6-2004//
                //Guanzhou out 5-6-2004    int err = ( matpoint[where]->matmodel )->setTrialStrainIncr( incremental_strain);
                //Guanzhou out 5-6-2004    if ( err)
                //Guanzhou out 5-6-2004      cerr << "EightNodeBrick::getStiffnessTensor (tag: " << this->getTag() << "), not converged\n";


                //char *test = matpoint[where]->matmodel->getType();
                // fmk - changing if so if into else block must be Template3Dep
                //    if (strcmp(matpoint[where]->matmodel->getType(),"Template3Dep") != 0)
                stress_at_GP = matpoint[where]->getStressTensor();

                //         stress_at_GP.report("PROBLEM");
                //         getchar();

                //    else
                //    {
                //             //Some thing funny happened when getting stress directly from matpoint[where], i have to do it this way!
                //       EPState *tmp_eps = ((Template3Dep *)(matpoint[where]->matmodel))->getEPS();
                //       stress_at_GP = tmp_eps->getStress();
                //       //delete tmp_eps;
                //           }

                //double  p = stress_at_GP.p_hydrostatic();
                //if ( p < 0.0 )
                //{
                //  cerr << getTag();
                //  cerr << " ***p  =    " << p << endln;
                //}

                //cerr << " nodal_force ::: stress_at_GP " << stress_at_GP << endln;

                //}
                //else if ( tmp_ndm ) { //Elastic case
                //    stress_at_GP = (matpoint[where].getNDMat())->getStressTensor();
                //}
                //else {
                //   g3ErrorHandler->fatal("EightNodeBrick::nodal_forces (tag: %d), could not getStress", this->getTag());
                //   exit(1);
                //}

                //stress_at_GP.report("\n stress_at_GPtensor at GAUSS point for nodal forces \n");

                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;
                //nodal_forces.print("nf","\n\n Nodal Forces \n");

            }
        }
    }

    //cerr << "\n element no. " << getTag() << endln;
    //nodal_forces.print("nf","\n Nodal Forces \n");
    return nodal_forces;

}

////#############################################################################
// returns nodal forces for given ITERATIVE stress field in an element
tensor EightNodeBrick::iterative_nodal_forces(void)
{

    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in tensor EightNodeBrick::iterative_nodal_forces(void)" <<endln; this->Print(cerr);
    //BJ//BJ

    int force_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {8, 3};  // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor dh(2, dh_dim, 0.0);

    stresstensor stress_at_GP(0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;
                //.....
                //.....::printf("EightNodeBrick::iterative_nodal_forces(void)  ----**************** where = %d \n", where);
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
                dhGlobal = dh("ij") * JacobianINV("kj");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;

                //                   stress_at_GP = (GPstress)->operator[](where);
                //stress_at_GP = GPiterative_stress[where];

                //stress_at_GP = ( matpoint[where].getTrialEPS() )->getStress();
                stress_at_GP = matpoint[where]->getStressTensor();
                stress_at_GP.reportshortpqtheta("\n iterative_stress at GAUSS point in iterative_nodal_force\n");

                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress_at_GP("ab") * weight;
                //nodal_forces.print("nf","\n EightNodeBrick::iterative_nodal_forces Nodal Forces ~~~~\n");

            }
        }
    }


    return nodal_forces;

}

////#############################################################################
// returns nodal forces for given constant stress field in the element
tensor EightNodeBrick::nodal_forces_from_stress(stresstensor &stress)
{


    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in tensor EightNodeBrick::nodal_forces_from_stress(stresstensor & stress)" <<endln; this->Print(cerr);
    //BJ//BJ

    int force_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    double weight = 0.0;

    int dh_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor dh(2, dh_dim, 0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                //--                where =
                //--                ((GP_c_r-1)*integration_order+GP_c_s-1)*integration_order+GP_c_t-1;
                //.....
                //.....::printf("EightNodeBrick::iterative_nodal_forces(void)  ----**************** where = %d \n", where);
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
                dhGlobal = dh("ij") * JacobianINV("kj");

                //weight
                weight = rw * sw * tw * det_of_Jacobian;

                //                   stress_at_GP = (GPstress)->operator[](where);
                //                stress_at_GP = GPiterative_stress[where];
                //GPiterative_stress[where].reportshortpqtheta("\n iterative_stress at GAUSS point in iterative_nodal_force\n");

                // nodal forces See Zienkievicz part 1 pp 108
                nodal_forces =
                    nodal_forces + dhGlobal("ib") * stress("ab") * weight;
                //nodal_forces.print("nf","\n EightNodeBrick::iterative_nodal_forces Nodal Forces ~~~~\n");

            }
        }
    }

    return nodal_forces;

}


////#############################################################################
// returns nodal forces for given incremental strain field in an element
// by using the linearized constitutive tensor from the begining of the step !
tensor EightNodeBrick::linearized_nodal_forces(void)
{
    int force_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor linearized_nodal_forces(2, force_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor dh(2, dh_dim, 0.0);

    tensor Constitutive( 4, def_dim_4, 0.0);

    double det_of_Jacobian = 0.0;

    static int disp_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor incremental_displacements(2, disp_dim, 0.0);

    straintensor incremental_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    stresstensor final_linearized_stress;
    //    stresstensor incremental_stress;
    // tensor of incremental displacements taken from node objects for this element !
    incremental_displacements = incr_disp();
    //incremental_displacements.print("disp","\n incremental_displacements tensor at GAUSS point in iterative_Update\n");

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

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
                dhGlobal = dh("ij") * JacobianINV("kj");

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

                //Constitutive = GPtangent_E[where];

                //EPState *tmp_eps = (matpoint[where]).getEPS();
                //NDMaterial *tmp_ndm = (matpoint[where]).getNDMat();

                //if ( tmp_eps ) {     //Elasto-plastic case
                //    mmodel->setEPS( *tmp_eps );
                if ( ! (matpoint[where]->matmodel)->setTrialStrainIncr( incremental_strain)  )
                {
                    cerr << "EightNodeBrick::linearized_nodal_forces (tag: " << this->getTag() << "), not converged\n";
                }

                Constitutive = (matpoint[where]->matmodel)->getTangentTensor();
                //    matpoint[where].setEPS( mmodel->getEPS() ); //Set the new EPState back
                //}
                //else if ( tmp_ndm ) { //Elastic case
                //    (matpoint[where].p_matmodel)->setTrialStrainIncr( incremental_strain );
                //    Constitutive = (matpoint[where].p_matmodel)->getTangentTensor();
                //}
                //else {
                //   g3ErrorHandler->fatal("EightNodeBrick::incremental_Update (tag: %d), could not getTangentTensor", this->getTag());
                //   exit(1);
                //}

                //Constitutive = ( matpoint[where].getEPS() )->getEep();
                //..//GPtangent_E[where].print("\n tangent E at GAUSS point \n");

                final_linearized_stress =
                    Constitutive("ijkl") * incremental_strain("kl");

                // nodal forces See Zienkievicz part 1 pp 108
                linearized_nodal_forces = linearized_nodal_forces +
                                          dhGlobal("ib") * final_linearized_stress("ab") * weight;
                //::::::                   nodal_forces.print("nf","\n\n Nodal Forces \n");

            }
        }
    }


    return linearized_nodal_forces;

}

//#############################################################################
void EightNodeBrick::report(char *msg)
{
    if ( msg )
    {
        ::printf("** %s", msg);
    }

    ::printf("\n Element Number = %d\n", this->getTag() );
    ::printf("\n Number of nodes in a EightNodebrick = %d\n",
             nodes_in_brick);
    ::printf("\n Determinant of Jacobian (! ==0 before comp.) = %f\n",
             determinant_of_Jacobian);

    ::printf("Node numbers \n");
    ::printf(
        ".....1.....2.....3.....4.....5.....6.....7.....8.....9.....0.....1.....2\n");

    for ( int i = 0 ; i < 8 ; i++ )
        //::printf("%6d",G_N_numbs[i]);
    {
        ::printf("%6d", connectedExternalNodes(i));
    }

    ::printf("\n");
    //           for ( int j=8 ; j<20 ; j++ )
    //             ::printf("%6d",G_N_numbs[j]);     // Commented by Xiaoyan
    ::printf("\n\n");

    //    ::printf("Node existance array \n");
    //           for ( int k=0 ; k<12 ; k++ )
    //             ::printf("%6d",node_existance[k]);
    //           ::printf("\n\n");          // Commented by Xiaoyan


    int total_number_of_Gauss_points = integration_order *
                                       integration_order *
                                       integration_order;

    if ( total_number_of_Gauss_points != 0 )
    {
        // report from Node class
        //for ( int in=0 ; in<8 ; in++ )
        //             (nodes[G_N_numbs[in]]).report("nodes from within element (first 8)\n");
        //Xiaoyan changed .report to . Print in above line 09/27/00
        //  (nodes[G_N_numbs[in]]).Print(cerr);

        //         theNodes[0]->Print(cerr);
        //         theNodes[1]->Print(cerr);
        //         theNodes[2]->Print(cerr);
        //         theNodes[3]->Print(cerr);
        //         theNodes[4]->Print(cerr);
        //         theNodes[5]->Print(cerr);
        //         theNodes[6]->Print(cerr);
        //         theNodes[7]->Print(cerr);

        //           for ( int jn=8 ; jn<20 ; jn++ )
        //             (nodes[G_N_numbs[jn]]).report("nodes from within element (last 12)\n");
        // Commented by Xiaoyan
    }

    ::printf("\n\nGauss-Legendre integration order\n");
    ::printf("Integration order in r direction = %d\n", integration_order);
    ::printf("Integration order in s direction = %d\n", integration_order);
    ::printf("Integration order in t direction = %d\n\n", integration_order);



    for ( int GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        for ( int GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            for ( int GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                short where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                ::printf("\n\n----------------**************** where = %d \n", where);
                ::printf("                    GP_c_r = %d,  GP_c_s = %d,  GP_c_t = %d\n",
                         GP_c_r, GP_c_s, GP_c_t);
                matpoint[where]->report("Material Point\n");
                //GPstress[where].reportshort("stress at Gauss Point");
                //GPstrain[where].reportshort("strain at Gauss Point");
                //matpoint[where].report("Material model  at Gauss Point");
            }
        }
    }

}


//#############################################################################
void EightNodeBrick::reportshort(char *msg)
{
    if ( msg )
    {
        ::printf("** %s", msg);
    }

    ::printf("\n Element Number = %d\n", this->getTag() );
    ::printf("\n Number of nodes in a EightNodeBrick = %d\n",
             nodes_in_brick);
    ::printf("\n Determinant of Jacobian (! ==0 before comp.) = %f\n",
             determinant_of_Jacobian);

    ::printf("Node numbers \n");
    ::printf(
        ".....1.....2.....3.....4.....5.....6.....7.....8.....9.....0.....1.....2\n");

    for ( int i = 0 ; i < 8 ; i++ )
        //::printf("%6d",G_N_numbs[i]);
    {
        ::printf( "%6d", connectedExternalNodes(i) );
    }

    ::printf("\n");
    //           for ( int j=8 ; j<20 ; j++ )
    //             ::printf("%6d",G_N_numbs[j]);   //// Commented by Xiaoyan
    ::printf("\n\n");

    //    ::printf("Node existance array \n");
    //           for ( int k=0 ; k<12 ; k++ )
    //             ::printf("%6d",node_existance[k]);     // Commented by Xiaoyan
    ::printf("\n\n");

}




//#############################################################################
void EightNodeBrick::reportPAK(char *msg)
{
    if ( msg )
    {
        ::printf("%s", msg);
    }

    ::printf("%10d   ",  this->getTag());

    for ( int i = 0 ; i < 8 ; i++ )
    {
        ::printf( "%6d", connectedExternalNodes(i) );
    }

    //::printf("%6d",G_N_numbs[i]);

    printf("\n");
}


//#############################################################################
void EightNodeBrick::reportpqtheta(int GP_numb)
{
    short where = GP_numb - 1;
    matpoint[where]->reportpqtheta("");
}

////#############################################################################
//Vector EightNodeBrick::reportLM(char * msg)
//  {
//    if ( msg ) ::printf("%s",msg);
//    ::printf("Element # %d, LM->", this->get_Brick_Number());
//    for (int count = 0 ; count < 24 ; count++)
//      {
//        ::printf(" %d", LM[count]);
//      }
//    ::printf("\n");
//
//  }

//#############################################################################
//Compute gauss point coordinates
void EightNodeBrick::computeGaussPoint()
{
    //    if ( msg ) ::printf("** %s\n",msg);

    // special case for 8 nodes only
    // special case for 8 nodes only
    int count;
    count = FixedOrder * FixedOrder * FixedOrder;
    //Vector Gsc(count*3+1); //+1: number of Gauss point in element
    Gsc8(0) = count;

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    // special case for 8 nodes only
    static const int dim[] = {3, 8}; // static-> see ARM pp289-290
    static const int dimM[] = {3,  FixedOrder *FixedOrder * FixedOrder}; // found a bug: dimM depends on Order of Gauss Points Joey Yang March 02, 02
    tensor NodalCoord(2, dim, 0.0);
    tensor matpointCoord(2, dimM, 0.0);
    int h_dim[] = {24, 3};  // Xiaoyan changed from {60,3} to {24,3} for 8 nodes
    tensor H(2, h_dim, 0.0);

    //for (int ncount = 1 ; ncount <= 8 ; ncount++ )
    ////  for (int ncount = 0 ; ncount <= 7 ; ncount++ )
    //  {
    //  //int global_node_number = get_global_number_of_node(ncount-1);
    //  // printf("global node num %d",global_node_number);
    //
    //    //   NodalCoord.val(1,ncount) = nodes[global_node_number].x_coordinate();
    //    //   NodalCoord.val(2,ncount) = nodes[global_node_number].y_coordinate();
    //    //   NodalCoord.val(3,ncount) = nodes[global_node_number].z_coordinate();
    //    // Xiaoyan changed to the following:  09/27/00
    //  Vector Coordinates = nodes[global_node_number].getCrds();
    //
    //    NodalCoord.val(1,ncount) = Coordinates(0);
    //    NodalCoord.val(2,ncount) = Coordinates(1);
    //    NodalCoord.val(3,ncount) = Coordinates(2);
    //printf("global point %d     x=%+.6e   y=%+.6e   z=%+.6e \n ", global_node_number,
    //                                                      NodalCoord.val(1,ncount),
    //                  NodalCoord.val(2,ncount),
    //                  NodalCoord.val(3,ncount));
    //}

    //Zhaohui using node pointers, which come from the Domain
    const Vector &nd1Crds = theNodes[0]->getCrds();
    const Vector &nd2Crds = theNodes[1]->getCrds();
    const Vector &nd3Crds = theNodes[2]->getCrds();
    const Vector &nd4Crds = theNodes[3]->getCrds();
    const Vector &nd5Crds = theNodes[4]->getCrds();
    const Vector &nd6Crds = theNodes[5]->getCrds();
    const Vector &nd7Crds = theNodes[6]->getCrds();
    const Vector &nd8Crds = theNodes[7]->getCrds();

    NodalCoord.val(1, 1) = nd1Crds(0);
    NodalCoord.val(2, 1) = nd1Crds(1);
    NodalCoord.val(3, 1) = nd1Crds(2);
    NodalCoord.val(1, 2) = nd2Crds(0);
    NodalCoord.val(2, 2) = nd2Crds(1);
    NodalCoord.val(3, 2) = nd2Crds(2);
    NodalCoord.val(1, 3) = nd3Crds(0);
    NodalCoord.val(2, 3) = nd3Crds(1);
    NodalCoord.val(3, 3) = nd3Crds(2);
    NodalCoord.val(1, 4) = nd4Crds(0);
    NodalCoord.val(2, 4) = nd4Crds(1);
    NodalCoord.val(3, 4) = nd4Crds(2);
    NodalCoord.val(1, 5) = nd5Crds(0);
    NodalCoord.val(2, 5) = nd5Crds(1);
    NodalCoord.val(3, 5) = nd5Crds(2);
    NodalCoord.val(1, 6) = nd6Crds(0);
    NodalCoord.val(2, 6) = nd6Crds(1);
    NodalCoord.val(3, 6) = nd6Crds(2);
    NodalCoord.val(1, 7) = nd7Crds(0);
    NodalCoord.val(2, 7) = nd7Crds(1);
    NodalCoord.val(3, 7) = nd7Crds(2);
    NodalCoord.val(1, 8) = nd8Crds(0);
    NodalCoord.val(2, 8) = nd8Crds(1);
    NodalCoord.val(3, 8) = nd8Crds(2);


    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates

                H = H_3D(r, s, t);

                for (int encount = 1 ; encount <= 8 ; encount++ )
                    //         for (int encount=0 ; encount <= 7 ; encount++ )
                {
                    //  matpointCoord.val(1,where+1) =+NodalCoord.val(1,where+1) * H.val(encount*3-2,1);
                    //  matpointCoord.val(2,where+1) =+NodalCoord.val(2,where+1) * H.val(encount*3-1,2);
                    //  matpointCoord.val(3,where+1) =+NodalCoord.val(3,where+1) * H.val(encount*3-0,3);
                    matpointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
                    //::printf("-- NO nodal, H_val :%d %+.2e %+.2e %+.5e\n", encount,NodalCoord.val(1,encount),H.val(encount*3-2,1),matpointCoord.val(1,where+1) );
                    matpointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
                    matpointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);

                }

                //::printf("gauss point# %d   %+.6e %+.6e %+.6e \n", where+1,
                //                                        matpointCoord.val(1,where+1),
                //                                        matpointCoord.val(2,where+1),
                //                                        matpointCoord.val(3,where+1));

                Gsc8(where * 3 + 1) = matpointCoord.val(1, where + 1);
                Gsc8(where * 3 + 2) = matpointCoord.val(2, where + 1);
                Gsc8(where * 3 + 3) = matpointCoord.val(3, where + 1);

                //matpoint[where].reportTensor("");


            }
        }
    }

    //return Gsc8;
}


////#############################################################################

//#############################################################################
//void EightNodeBrick::reportTensor(char * msg)
// ZHaohui added to print gauss point coord. to file fp

void EightNodeBrick::reportTensorF(FILE *fp)
{
    //if ( msg ) ::printf("** %s\n",msg);

    // special case for 8 nodes only
    // special case for 8 nodes only
    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    // special case for 8 nodes only
    static const int dim[] = {3, 8}; // static-> see ARM pp289-290
    tensor NodalCoord(2, dim, 0.0);
    tensor matpointCoord(2, dim, 0.0);
    int h_dim[] = {24, 3}; // Xiaoyan changed from {60,3} to {24,3} for 8 nodes

    tensor H(2, h_dim, 0.0);

    //for (int ncount = 1 ; ncount <= 8 ; ncount++ )
    //  // for (int ncount = 0 ; ncount <= 7 ; ncount++ )
    //  {
    //  int global_node_number = get_global_number_of_node(ncount-1);
    //  // printf("global node num %d",global_node_number);
    //
    //    //        NodalCoord.val(1,ncount) = nodes[global_node_number].x_coordinate();
    //    //        NodalCoord.val(2,ncount) = nodes[global_node_number].y_coordinate();
    //    //        NodalCoord.val(3,ncount) = nodes[global_node_number].z_coordinate();
    //    // Xiaoyan changed to the following:  09/27/00
    //  Vector Coordinates = nodes[global_node_number].getCrds();
    //    NodalCoord.val(1,ncount) = Coordinates(0);
    //    NodalCoord.val(2,ncount) = Coordinates(1);
    //    NodalCoord.val(3,ncount) = Coordinates(2);
    //printf("global point %d     x=%+.6e   y=%+.6e   z=%+.6e \n ", global_node_number,
    //                                                      NodalCoord.val(1,ncount),
    //                  NodalCoord.val(2,ncount),
    //                  NodalCoord.val(3,ncount));
    //  }

    //Zhaohui using node pointers, which come from the Domain
    const Vector &nd1Crds = theNodes[0]->getCrds();
    const Vector &nd2Crds = theNodes[1]->getCrds();
    const Vector &nd3Crds = theNodes[2]->getCrds();
    const Vector &nd4Crds = theNodes[3]->getCrds();
    const Vector &nd5Crds = theNodes[4]->getCrds();
    const Vector &nd6Crds = theNodes[5]->getCrds();
    const Vector &nd7Crds = theNodes[6]->getCrds();
    const Vector &nd8Crds = theNodes[7]->getCrds();

    NodalCoord.val(1, 1) = nd1Crds(0);
    NodalCoord.val(2, 1) = nd1Crds(1);
    NodalCoord.val(3, 1) = nd1Crds(2);
    NodalCoord.val(1, 2) = nd2Crds(0);
    NodalCoord.val(2, 2) = nd2Crds(1);
    NodalCoord.val(3, 2) = nd2Crds(2);
    NodalCoord.val(1, 3) = nd3Crds(0);
    NodalCoord.val(2, 3) = nd3Crds(1);
    NodalCoord.val(3, 3) = nd3Crds(2);
    NodalCoord.val(1, 4) = nd4Crds(0);
    NodalCoord.val(2, 4) = nd4Crds(1);
    NodalCoord.val(3, 4) = nd4Crds(2);
    NodalCoord.val(1, 5) = nd5Crds(0);
    NodalCoord.val(2, 5) = nd5Crds(1);
    NodalCoord.val(3, 5) = nd5Crds(2);
    NodalCoord.val(1, 6) = nd6Crds(0);
    NodalCoord.val(2, 6) = nd6Crds(1);
    NodalCoord.val(3, 6) = nd6Crds(2);
    NodalCoord.val(1, 7) = nd7Crds(0);
    NodalCoord.val(2, 7) = nd7Crds(1);
    NodalCoord.val(3, 7) = nd7Crds(2);
    NodalCoord.val(1, 8) = nd8Crds(0);
    NodalCoord.val(2, 8) = nd8Crds(1);
    NodalCoord.val(3, 8) = nd8Crds(2);

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                // this short routine is supposed to calculate position of
                // Gauss point from 3D array of short's
                where =
                    ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;
                // derivatives of local coordinates with respect to local coordinates

                H = H_3D(r, s, t);

                for (int encount = 1 ; encount <= 8 ; encount++ )
                    //         for (int encount=0 ; encount <= 7 ; encount++ )
                {
                    //  matpointCoord.val(1,where+1) =+NodalCoord.val(1,where+1) * H.val(encount*3-2,1);
                    //  matpointCoord.val(2,where+1) =+NodalCoord.val(2,where+1) * H.val(encount*3-1,2);
                    //  matpointCoord.val(3,where+1) =+NodalCoord.val(3,where+1) * H.val(encount*3-0,3);
                    matpointCoord.val(1, where + 1) += NodalCoord.val(1, encount) * H.val(encount * 3 - 2, 1);
                    //::printf("-- NO nodal, H_val :%d %+.2e %+.2e %+.5e\n", encount,NodalCoord.val(1,encount),H.val(encount*3-2,1),matpointCoord.val(1,where+1) );
                    matpointCoord.val(2, where + 1) += NodalCoord.val(2, encount) * H.val(encount * 3 - 1, 2);
                    matpointCoord.val(3, where + 1) += NodalCoord.val(3, encount) * H.val(encount * 3 - 0, 3);

                }

                fprintf(fp, "gauss point# %d   %+.6e %+.6e %+.6e \n", where + 1,
                        matpointCoord.val(1, where + 1),
                        matpointCoord.val(2, where + 1),
                        matpointCoord.val(3, where + 1));

                //matpoint[where].reportTensor("");


            }
        }
    }

}



//=============================================================================
int EightNodeBrick::getNumExternalNodes () const
{
    return 8;  //changed from 4 to 8 Xiaoyan 07/06/00
}


//=============================================================================
const ID &EightNodeBrick::getExternalNodes ()
{
    return connectedExternalNodes;
}

Node **
EightNodeBrick::getNodePtrs(void)
{
    return theNodes;
}

//=============================================================================
int EightNodeBrick::getNumDOF ()
{
    return 24;       //Changed from 2*4=8 to 3*8=24 Xiaoyan 07/06/00
}

//=============================================================================
void EightNodeBrick::setDomain (Domain *theDomain)
{
    // Check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
        theNodes[2] = 0;
        theNodes[3] = 0;
        theNodes[4] = 0;
        theNodes[5] = 0;
        theNodes[6] = 0;
        theNodes[7] = 0;
    }
    else
    {
        int Nd1 = connectedExternalNodes(0);
        int Nd2 = connectedExternalNodes(1);
        int Nd3 = connectedExternalNodes(2);
        int Nd4 = connectedExternalNodes(3);
        int Nd5 = connectedExternalNodes(4);
        int Nd6 = connectedExternalNodes(5);
        int Nd7 = connectedExternalNodes(6);
        int Nd8 = connectedExternalNodes(7);

        theNodes[0] = theDomain->getNode(Nd1);
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[0] == 0)
        {
            theNodes[0] = theDomain->getOutsideNode(Nd1);
        }

# endif
        theNodes[1] = theDomain->getNode(Nd2);
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[1] == 0)
        {
            theNodes[1] = theDomain->getOutsideNode(Nd2);
        }

# endif
        theNodes[2] = theDomain->getNode(Nd3);
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[2] == 0)
        {
            theNodes[2] = theDomain->getOutsideNode(Nd3);
        }

# endif
        theNodes[3] = theDomain->getNode(Nd4);
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[3] == 0)
        {
            theNodes[3] = theDomain->getOutsideNode(Nd4);
        }

# endif

        //Xiaoyan added 5-8  07/06/00
        theNodes[4] = theDomain->getNode(Nd5);
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[4] == 0)
        {
            theNodes[4] = theDomain->getOutsideNode(Nd5);
        }

# endif
        theNodes[5] = theDomain->getNode(Nd6);
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[5] == 0)
        {
            theNodes[5] = theDomain->getOutsideNode(Nd6);
        }

# endif
        theNodes[6] = theDomain->getNode(Nd7);
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[6] == 0)
        {
            theNodes[6] = theDomain->getOutsideNode(Nd7);
        }

# endif
        theNodes[7] = theDomain->getNode(Nd8);
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[7] == 0)
        {
            theNodes[7] = theDomain->getOutsideNode(Nd8);
        }

# endif

        if (theNodes[0] == 0 || theNodes[1] == 0 || theNodes[2] == 0 || theNodes[3] == 0 ||
                theNodes[4] == 0 || theNodes[5] == 0 || theNodes[6] == 0 || theNodes[7] == 0 )
        {
            //Xiaoyan added 5-8  07/06/00

            cerr << "FATAL ERROR EightNodeBrick (tag: " << this->getTag() << "), node not found in domain\n";
            exit(-1);
        }

        int dofNd1 = theNodes[0]->getNumberDOF();
        int dofNd2 = theNodes[1]->getNumberDOF();
        int dofNd3 = theNodes[2]->getNumberDOF();
        int dofNd4 = theNodes[3]->getNumberDOF();

        //Xiaoyan added 5-8  07/06/00
        int dofNd5 = theNodes[4]->getNumberDOF();
        int dofNd6 = theNodes[5]->getNumberDOF();
        int dofNd7 = theNodes[6]->getNumberDOF();
        int dofNd8 = theNodes[7]->getNumberDOF();

        if (dofNd1 != 3 || dofNd2 != 3 || dofNd3 != 3 || dofNd4 != 3 ||  // Changed 2 to 3 Xiaoyan
                dofNd5 != 3 || dofNd6 != 3 || dofNd7 != 3 || dofNd8 != 3 )
        {
            cerr << "FATAL ERROR EightNodeBrick (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit(-1);
        }

        this->DomainComponent::setDomain(theDomain);
    }

    //Guanzhou added
    ComputeVolume();

}

//=============================================================================
int EightNodeBrick::commitState ()
{

    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in int EightNodeBrick::commitState ()" <<endln; this->Print(cerr);
    //BJ//BJ

    int retVal = 0;


    // call element commitState to do any base class stuff
    if ((retVal = this->Element::commitState()) != 0)
    {
        cerr << "EightNodeBrick::commitState () - failed in base class";
    }

    // int order = theQuadRule->getOrder();     // Commented by Xiaoyan

    int i;
    //     int plastify = 0;
    //int j, k;      // Xiaoyan added k for three dimension


    // Loop over the integration points and commit the material states
    int count  = integration_order * integration_order * integration_order;
    //for (i = 0; i < integration_order; i++)        // Xiaoyan chaneged order to
    //  for (j = 0; j < integration_order; j++)      // integration_order,
    //                  // integration_order, and
    //      for (k = 0; k < integration_order; k++)      // added integration_order,
    //         retVal += (GaussPtheMaterial[i][j][k]).commitState();

    Vector pp = getResistingForce();

    //if ( this->getTag() == 1 || this->getTag() == 700)
    //{
    for (i = 0; i < count; i++)
        //for (i = 0; i < 0; i++)
    {
        retVal += matpoint[i]->commitState();
        //if (i == 4 && strcmp(matpoint[i]->matmodel->getType(),"Template3Dep") == 0)
        //          stresstensor st;
        //            stresstensor prin;
        //            straintensor stn;
        //            straintensor pl_stn;
        //            straintensor stnprin;
        //
        //           //checking if element plastified
        //           pl_stn = matpoint[i]->getPlasticStrainTensor();
        //
        //           double  p_plastc = pl_stn.p_hydrostatic();
        //           if (  fabs(p_plastc) > 0 )
        //             {
        //               plastify = 1;
        //               break;
        //             }
        //
        //           st = matpoint[i]->getStressTensor();
        //           prin = st.principal();
        //           stn = matpoint[i]->getStrainTensor();
        //           stnprin = stn.principal();
        //
        //  //   cerr << "\nGauss Point: " << i << endln;
        //  //   cerr << "sigma11: "<< st.cval(1, 1) << " "<< st.cval(1, 2) << " " << st.cval(1, 3) << endln;
        //  //   cerr << "sigma21: "<< st.cval(2, 1) << " "<< st.cval(2, 2) << " " << st.cval(2, 3) << endln;
        //  //    cerr << "sigma31: "<< st.cval(3, 1) << " "<< st.cval(3, 2) << " " << st.cval(3, 3) << endln << endln;
        //
        //     //cerr << "strain11: "<< stn.cval(1, 1) << " "<< stn.cval(1, 2) << " " << stn.cval(1, 3) << endln;
        //     //cerr << "strain21: "<< stn.cval(2, 1) << " "<< stn.cval(2, 2) << " " << stn.cval(2, 3) << endln;
        //      //cerr << "strain31: "<< stn.cval(3, 1) << " "<< stn.cval(3, 2) << " " << stn.cval(3, 3) << endln;
        //
        //     //double  p = -1*( prin.cval(1, 1)+ prin.cval(2, 2) +prin.cval(3, 3) )/3.0;
        //     //double  ev = -1*( stnprin.cval(1, 1)+ stnprin.cval(2, 2) + stnprin.cval(3, 3) )/3.0;
        //     //cerr << "   " << p;
        //
        //     //if (p < 0)
        //     //  cerr  << "gs pnt:" << i << "  p="<< p;
        //
        //
        //     double q;
        //     //if ( fabs(prin.cval(1, 1) - prin.cval(2, 2) ) <=  0.0001 )
        //           if ( fabs(prin.cval(1, 1) - prin.cval(2, 2) ) <=  0.001 )
        //           {
        //               q = prin.cval(1, 1) - prin.cval(3, 3);
        //               //cerr << "1 = 2";
        //           }
        //           else
        //               q = prin.cval(3, 3) - prin.cval(1, 1);
        //
        //     //Triaxial compr.  fabs
        //           //cerr << "     " << st.cval(2, 3); //tau_yz
        //     //cerr << "     " << q;
        //     ////----cerr << "     " << fabs(q);
        //
        //           //cerr << "     " << ev << endln;
        //
        //  //out22Jan2001   if (strcmp(matpoint[i]->matmodel->getType(),"Template3Dep") == 0)
        //  //out22Jan2001          {
        //  //out22Jan2001           st = ( ((Template3Dep *)(matpoint[i]->matmodel))->getEPS())->getStress();
        //  //out22Jan2001           prin = st.principal();
        //  //out22Jan2001    }
        //  //out22Jan2001    else
        //  //out22Jan2001    {
        //  //out22Jan2001            st = matpoint[i]->getStressTensor();
        //  //out22Jan2001           prin = st.principal();
        //  //out22Jan2001
        //  //out22Jan2001    }
        //
        //      //double  p = st.p_hydrostatic();
        //      //double  p = -1*( prin.cval(1, 1)+ prin.cval(2, 2) +prin.cval(3, 3) )/3.0;
        //            //cerr << "\n " << prin.cval(1, 1) << "   " << prin.cval(2, 2) << "  " <<  prin.cval(3, 3) << endln;
        //            //if ( getTag() == 960)
        //            //cerr << " El= " << getTag() << " , p    " << p << endln;
        //
        //      //printf(stderr, " Gauss Point i = %d ", (i+1));
        //      //printf(stderr, " Gauss Point i = %d ", (i+1));
        //
        //
        //            //if ( p < 0 )
        //      //{
        //      //  cerr << getTag();
        //      //  cerr << " ***p  =    " << p << endln;
        //      //}
        //            //J2D
        //            //cerr << "        " << st.q_deviatoric();
        //
        //            //double q;
        //            //if ( fabs(prin.cval(1, 1) - prin.cval(2, 2) ) <=  0.0001 )
        //            //{
        //            //    q = prin.cval(1, 1) - prin.cval(3, 3);
        //            //    //cerr << "1 = 2";
        //            //}
        //            //else
        //            //    q = prin.cval(3, 3) - prin.cval(1, 1);
        //
        //            //Triaxial compr.
        //            //cerr << "        " << q;
    }

    //}

    //cerr << this->getTag() << " " << plastify << endln;


    //cerr << " at elements " << this->getTag() << endln;


    //output nodal force
    //cerr << "    " << pp(2) << endln;
    //}
    return retVal;
}

//=============================================================================
int EightNodeBrick::revertToLastCommit ()
{
    //  int order = theQuadRule->getOrder();  // Commented by Xiaoyan
    int i;
    //int j, k;     // Xiaoyan added k for three dimension
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states
    int count  = integration_order * integration_order * integration_order;
    //for (i = 0; i < integration_order; i++)       // Xiaoyan chaneged order to
    //  for (j = 0; j < integration_order; j++)     // integration_order,
    //      for (k = 0; k < integration_order; k++)     // integration_order, and
    // added integration_order,
    //retVal += (theMaterial[i][j][k]).revertToLastCommit();

    for (i = 0; i < count; i++)
    {
        retVal += matpoint[i]->revertToLastCommit();
    }


    return retVal;
}

//=============================================================================
int EightNodeBrick::revertToStart ()
{
    int i;     // Xiaoyan added k for three dimension
    int retVal = 0;

    // Loop over the integration points and revert to last committed material states
    //for (i = 0; i < integration_order; i++)       // Xiaoyan chaneged order to
    //  for (j = 0; j < integration_order; j++)     // integration_order,
    //      for (k = 0; k < integration_order; k++)     // integration_order, and
    // added integration_order,
    //      retVal += (theMaterial[i][j][k]).revertToLastCommit();

    int count  = integration_order * integration_order * integration_order;

    for (i = 0; i < count; i++)
    {
        retVal += matpoint[i]->revertToStart();
    }


    return retVal;

    // Loop over the integration points and revert to initial material states
}


//=============================================================================
const Matrix &EightNodeBrick::getTangentStiff()
{
    //BJ/BJ
    //BJ    cerr << "\n\n\n\n Print in const Matrix &EightNodeBrick::getTangentStiff ()" <<endln; this->Print(cerr);
    //BJ//BJ



    tensor stifftensor = getStiffnessTensor();
    int Ki = 0;
    int Kj = 0;


    //BJ UNROLL this loop!!!!!!
    // Optimize BJ

    for ( int i = 1 ; i <= 8 ; i++ )
    {
        for ( int j = 1 ; j <= 8 ; j++ )
        {
            for ( int k = 1 ; k <= 3 ; k++ )
            {
                for ( int l = 1 ; l <= 3 ; l++ )
                {
                    Ki = k + 3 * (i - 1);
                    Kj = l + 3 * (j - 1);
                    K( Ki - 1 , Kj - 1 ) = stifftensor.cval(i, k, l, j);
                    //if (Ki == Kj && K( Ki - 1 , Kj - 1 ) <= 0 )
                    // {
                    //     cout <<
                    //     "K( " <<
                    //     i - 1 << "," <<
                    //     k - 1 << "," <<
                    //     l - 1 << "," <<
                    //     j - 1 << "," <<
                    //     ") = " <<
                    //     "K( " <<
                    //     Ki - 1 << "," <<
                    //     Kj - 1 <<
                    //     ") = " <<
                    //     K( Ki - 1 , Kj - 1 ) << endl;
                    // }
                }
            }
        }
    }

    //      cerr << " K " << K << endln;
    //      K.Output(cerr);
    //      if (this->getTag() == 6 || this->getTag() == 70)
    //      cerr << "EightNodeBrick::getTangentStiff: " << this->getTag() << "\nTANGENT: " << K << endln;


    return K;
}

//=============================================================================
const Matrix &EightNodeBrick::getInitialStiff ()
{
    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in const Matrix &EightNodeBrick::getInitialStiff ()" <<endln; this->Print(cerr);
    //BJ//BJ


    if (Ki == 0)
    {
        Ki = new Matrix(this->getTangentStiff());
    }

    if (Ki == 0)
    {
        cerr << "FATAL fElement::getInitialStiff() -";
        cerr << "ran out of memory\n";
        exit(-1);
    }

    return *Ki;
}


//Get consistent mass
const Matrix &EightNodeBrick::getMass ()
{
    int M_dim[] = {24, 24};
    tensor Mm(2, M_dim, 0.0);

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    //    short where = 0;
    double weight = 0.0;

    int dh_dim[] = {8, 3};   // Xiaoyan changed from {20,3} to {8,3}

    tensor dh(2, dh_dim, 0.0);

    int h_dim[] = {24, 3}; // Xiaoyan changed from {60,3} to {24,3}
    tensor H(2, h_dim, 0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;

    double RHO;
    RHO = rho;   //global

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                dh = dh_drst_at(r, s, t);
                Jacobian = Jacobian_3D(dh);

                det_of_Jacobian  = Jacobian.determinant();

                H = H_3D(r, s, t);

                //weight
                weight = rw * sw * tw * RHO * det_of_Jacobian;


                Mm = Mm + H("ib") * H("kb") * weight;

            }
        }
    }

    //Mm.print("Mm","Mm");

    for ( int ii = 1 ; ii <= 24 ; ii++ )
    {
        for ( int jj = 1 ; jj <= 24 ; jj++ )
        {
            M( ii - 1 , jj - 1 ) = Mm.cval(ii, jj);
        }
    }

    return M;
}

//=============================================================================
void EightNodeBrick::zeroLoad(void)
{
    Q.Zero();
}



//======================================================================
const Vector &EightNodeBrick::getBodyForce(double loadFactor, const Vector &data)
{

    static Vector bforce(24);
    bforce.Zero();


    // Check for a quick return
    //cerr << "rho " << rho << endln;
    if (rho == 0.0)
    {
        return bforce;
    }


    Vector bf(3);
    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);



    Vector ba(24), bfx(3);
    bfx(0) = bf(0) * loadFactor;
    bfx(1) = bf(1) * loadFactor;
    bfx(2) = bf(2) * loadFactor;

    ba(0) =  bfx(0);
    ba(1) =  bfx(1);
    ba(2) =  bfx(2);
    ba(3) =  bfx(0);
    ba(4) =  bfx(1);
    ba(5) =  bfx(2);
    ba(6) =  bfx(0);
    ba(7) =  bfx(1);
    ba(8) =  bfx(2);
    ba(9) =  bfx(0);
    ba(10) = bfx(1);
    ba(11) = bfx(2);
    ba(12) = bfx(0);
    ba(13) = bfx(1);
    ba(14) = bfx(2);
    ba(15) = bfx(0);
    ba(16) = bfx(1);
    ba(17) = bfx(2);
    ba(18) = bfx(0);
    ba(19) = bfx(1);
    ba(20) = bfx(2);
    ba(21) = bfx(0);
    ba(22) = bfx(1);
    ba(23) = bfx(2);

    //Form equivalent body force
    this->getMass();

    bforce.addMatrixVector(0.0, M, ba, 1.0);


    return bforce;
}


//======================================================================
const Vector &EightNodeBrick::getSurfaceForce(double loadFactor, const Vector &data)
{

    int node_exist = 0;
    Vector node_local(4);

    // check if the nodes of the surface belong to the element
    for (int i = 0; i < 4; i++)
    {

        for (int j = 0; j < 8; j++)
        {
            if (data(i) == connectedExternalNodes(j))
            {
                node_exist = 1;
                node_local(i) = j;
                break;
            }
        }

        if (node_exist != 1)
        {
            cerr << "\nERROR: Node " << data(i) << " defined for the BrickSurfaceLoad does not belong to element " << this->getTag() << endl;
            exit(1);
        }
    }


    int node1_local = node_local(0);
    int node2_local = node_local(1);
    int node3_local = node_local(2);
    int node4_local = node_local(3);


    // get the surface nodal coordinates
    const Vector &coordnode1 = theNodes[node1_local]->getCrds();
    const Vector &coordnode2 = theNodes[node2_local]->getCrds();
    const Vector &coordnode3 = theNodes[node3_local]->getCrds();
    const Vector &coordnode4 = theNodes[node4_local]->getCrds();



    double ShapeFunctionValues;
    double LoadValue;
    Vector J_vector(3);
    Vector Pressure(4);



    Pressure(0) = data(4) * loadFactor;
    Pressure(1) = data(5) * loadFactor;
    Pressure(2) = data(6) * loadFactor;
    Pressure(3) = data(7) * loadFactor;



    static Vector NodalForces(24);

    for (int m = 0; m < 24; m++)
    {
        NodalForces(m) = 0;
    }



    double oneOverSquareRoot3 = 1.0 / sqrt(3.0);
    Matrix GsPts(4, 2);

    GsPts(0, 0) =  oneOverSquareRoot3;
    GsPts(0, 1) =  oneOverSquareRoot3;

    GsPts(1, 0) = -oneOverSquareRoot3;
    GsPts(1, 1) =  oneOverSquareRoot3;

    GsPts(2, 0) = -oneOverSquareRoot3;
    GsPts(2, 1) = -oneOverSquareRoot3;

    GsPts(3, 0) =  oneOverSquareRoot3;
    GsPts(3, 1) = -oneOverSquareRoot3;



    int r = 0;


    // loop over dof
    for (int k = 0; k < 3; k++)
    {
        // loop over nodes
        for (int j = 0; j < 4; j++)
        {

            for (int v = 0; v < 8; v++)
            {
                if (data(j) == connectedExternalNodes(v))
                {
                    r = v;
                    break;
                }
            }

            // loop over Gauss points
            for (int i = 0; i < 4; i++)
            {

                ShapeFunctionValues = SurfaceShapeFunctionValues(GsPts(i, 0) , GsPts(i, 1), j);
                J_vector = Direction_Weight(GsPts(i, 0) , GsPts(i, 1), coordnode1, coordnode2, coordnode3, coordnode4);
                LoadValue = SurfaceLoadValues(GsPts(i, 0) , GsPts(i, 1), Pressure);


                NodalForces(r * 3 + k) = NodalForces(r * 3 + k) + LoadValue * J_vector(k) * ShapeFunctionValues;
            }
        }
    }



    ///////////////////////////////////////////////////////////////////////
    //   for (int i=0; i<24; i++)
    //   {
    //    cout << "\nNodalForce(" << i << "): " << NodalForces(i);
    //   }
    //
    //
    //
    //     double sum_NodalForces;
    //
    //     for (int m=0; m<24; m++)
    //       {
    //          sum_NodalForces += NodalForces(m);
    //       }
    //
    //          cout << "\nSum Force: " << sum_NodalForces << endl;
    ///////////////////////////////////////////////////////////////////////


    return NodalForces;


}



//=============================================================================
int EightNodeBrick::addLoad(ElementalLoad *theLoad, double loadFactor)
{


    int type;
    const Vector &data = theLoad->getData(type, loadFactor);

    if (type == LOAD_TAG_ElementSelfWeight)
    {

        Vector Fbody = this->getBodyForce(loadFactor, data);

        Q.addVector(1.0, Fbody, 1.0);

    }
    else if (type == LOAD_TAG_BrickSurfaceLoad)
    {
        Vector Fsurface = this->getSurfaceForce(loadFactor, data);

        Q.addVector(1.0, Fsurface, 1.0);

    }
    else if (type == LOAD_TAG_BrickSurfaceLoad_from_Contact_Pore_Pressure)
    {
        Vector FsurfaceCoupled = this->getSurfaceForce(1.0, data);

        Q.addVector(1.0, FsurfaceCoupled, 1.0);

    }
    else
    {
        cerr << "EightNodeBrick::addLoad() - 8NodeBrick " << this->getTag() << ",load type " << type << "unknown\n";
        return -1;
    }


    ///////////////////////////////////////////////////////////////////////
    //   for (int i=0; i<8; i++)
    //          {
    //             cout << "\nQ(" << i+1 << "): " << Q(3*i) << "      " << Q(3*i+1) << "      " << Q(3*i+2);
    //          }
    //
    //
    //
    //     double sum_NodalForces;
    //
    //     for (int m=0; m<24; m++)
    //       {
    //  sum_NodalForces += Q(m);
    //       }
    //
    //          cout << "\nQ Total Force: " << sum_NodalForces << endl;
    ///////////////////////////////////////////////////////////////////////


    return 0;
}



//=============================================================================
int EightNodeBrick::addInertiaLoadToUnbalance(const Vector &accel)
{
    // Check for a quick return
    if (rho == 0.0)
    {
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV(accel);
    const Vector &Raccel2 = theNodes[1]->getRV(accel);
    const Vector &Raccel3 = theNodes[2]->getRV(accel);
    const Vector &Raccel4 = theNodes[3]->getRV(accel);
    // Xiaoyan added the following four 09/27/00
    const Vector &Raccel5 = theNodes[4]->getRV(accel);
    const Vector &Raccel6 = theNodes[5]->getRV(accel);
    const Vector &Raccel7 = theNodes[6]->getRV(accel);
    const Vector &Raccel8 = theNodes[7]->getRV(accel);

    if (3 != Raccel1.Size() || 3 != Raccel2.Size() || 3 != Raccel3.Size() ||
            3 != Raccel4.Size() || 3 != Raccel5.Size() || 3 != Raccel6.Size() ||
            3 != Raccel7.Size() || 3 != Raccel8.Size()  )
    {
        // Xiaoyan changed 2 to 3 and added Racce15-18  09/27/00
        cerr << "EightNodeBrick::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }

    static Vector ra(24);  // Changed form 8 to 24(3*8)  Xiaoyan 09/27/00

    ra(0) =  Raccel1(0);
    ra(1) =  Raccel1(1);
    ra(2) =  Raccel1(2);
    ra(3) =  Raccel2(0);
    ra(4) =  Raccel2(1);
    ra(5) =  Raccel2(2);
    ra(6) =  Raccel3(0);
    ra(7) =  Raccel3(1);
    ra(8) =  Raccel3(2);
    ra(9) =  Raccel4(0);
    ra(10) = Raccel4(1);
    ra(11) = Raccel4(2);
    // Xiaoyan added the following 09/27/00
    ra(12) = Raccel5(0);
    ra(13) = Raccel5(1);
    ra(14) = Raccel5(2);
    ra(15) = Raccel6(0);
    ra(16) = Raccel6(1);
    ra(17) = Raccel6(2);
    ra(18) = Raccel7(0);
    ra(19) = Raccel7(1);
    ra(20) = Raccel7(2);
    ra(21) = Raccel8(0);
    ra(22) = Raccel8(1);
    ra(23) = Raccel8(2);


    // Want to add ( - fact * M R * accel ) to unbalance
    // Take advantage of lumped mass matrix
    // Mass matrix is computed in setDomain()

    //double column_mass = 0;
    //for (int i = 0; i < 24; i++)
    //   column_mass += M(1,i);
    //column_mass = column_mass/3.0;

    //cerr << " addInerti... column_mass " << column_mass << endln;

    //for (int i = 0; i < 24; i++)
    //    Q(i) += -M(i,i)*ra(i);

    Q.addMatrixVector(1.0, M, ra, -1.0);
    return 0;
}

//=============================================================================
const Vector EightNodeBrick::FormEquiBodyForce(const Vector &data)
{

    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in const Vector EightNodeBrick::FormEquiBodyForce(void)" <<endln; this->Print(cerr);
    //BJ//BJ


    Vector bforce(24);

    // Check for a quick return
    //cerr << "rho " << rho << endln;
    if (rho == 0.0)
    {
        return bforce;
    }


    Vector bf(3);
    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);


    Vector ba(24);

    ba(0) =  bf(0);
    ba(1) =  bf(1);
    ba(2) =  bf(2);
    ba(3) =  bf(0);
    ba(4) =  bf(1);
    ba(5) =  bf(2);
    ba(6) =  bf(0);
    ba(7) =  bf(1);
    ba(8) =  bf(2);
    ba(9) =  bf(0);
    ba(10) = bf(1);
    ba(11) = bf(2);
    ba(12) = bf(0);
    ba(13) = bf(1);
    ba(14) = bf(2);
    ba(15) = bf(0);
    ba(16) = bf(1);
    ba(17) = bf(2);
    ba(18) = bf(0);
    ba(19) = bf(1);
    ba(20) = bf(2);
    ba(21) = bf(0);
    ba(22) = bf(1);
    ba(23) = bf(2);

    //Form equivalent body force
    bforce.addMatrixVector(0.0, M, ba, 1.0);
    //cerr << " ba " << ba;
    //cerr << " M " << M;
    //if (getTag() == 886)
    //cerr << " @@@@@ FormEquiBodyForce  " << bforce;

    return bforce;


}


//=============================================================================
const Vector &EightNodeBrick::getResistingForce()
{
    //BJ//BJ
    //BJ    cerr << "\n\n\n\n Print in const Vector &EightNodeBrick::getResistingForce ()" <<endln; this->Print(cerr);
    //BJ//BJ

    int force_dim[] = {8, 3};
    tensor nodalforces(2, force_dim, 0.0);

    nodalforces = nodal_forces();

    //converting nodalforce tensor to vector
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 3; j++)
        {
            P(i * 3 + j) = nodalforces.cval(i + 1, j + 1);
        }

    //     cerr << "Element " << this->getTag() << "\n";
    //     cerr << "Nodal force from Stress P :" << P << "\n";
    //     cerr << "External loading Q :" << Q << "\n";


    //     for (int i=0; i<8; i++)
    //     {
    //       for (int j=0; j<3; j++)
    //     {
    //      cout << "brick force before(" << i << "," << j << "): " << P(i*3+j) << "       ";
    //     }
    //     cout << endl;
    //     }




    //P = P - Q;
    P.addVector(1.0, Q, -1.0);

    //cerr << "P-Q" << P;
    //if (this->getTag() == 6 || this->getTag() == 70 || this->getTag() == 71)
    //  cerr << "ELEMENT: " << this->getTag() << " FOrces: " << P;


    return P;
}

//=============================================================================
const Vector &EightNodeBrick::getResistingForceIncInertia ()
{
    // form resisting force
    this->getResistingForce();

    //
    // now add dynamic terms
    // P += M * a + C * v
    //

    if (rho != 0.0)
    {

        // form the mass matrix
        this->getMass();

        const Vector &accel1 = theNodes[0]->getTrialAccel();
        const Vector &accel2 = theNodes[1]->getTrialAccel();
        const Vector &accel3 = theNodes[2]->getTrialAccel();
        const Vector &accel4 = theNodes[3]->getTrialAccel();
        const Vector &accel5 = theNodes[4]->getTrialAccel();
        const Vector &accel6 = theNodes[5]->getTrialAccel();
        const Vector &accel7 = theNodes[6]->getTrialAccel();
        const Vector &accel8 = theNodes[7]->getTrialAccel();

        static Vector a(24);  // originally 8

        a(0) =  accel1(0);
        a(1) =  accel1(1);
        a(2) =  accel1(2);
        a(3) =  accel2(0);
        a(4) =  accel2(1);
        a(5) =  accel2(2);
        a(6) =  accel3(0);
        a(7) =  accel3(1);
        a(8) =  accel3(2);
        a(9) =  accel4(0);
        a(10) = accel4(1);
        a(11) = accel4(2);
        // Xiaoyn added the following 09/27/00
        a(12) = accel5(0);
        a(13) = accel5(1);
        a(14) = accel5(2);
        a(15) = accel6(0);
        a(16) = accel6(1);
        a(17) = accel6(2);
        a(18) = accel7(0);
        a(19) = accel7(1);
        a(20) = accel7(2);
        a(21) = accel8(0);
        a(22) = accel8(1);
        a(23) = accel8(2);

        // P += M * a
        P.addMatrixVector(1.0, M, a, 1.0);

        // add the damping forces if rayleigh damping
        if (a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            P += this->getRayleighDampingForces();
        }

    }
    else
    {

        // add the damping forces if rayleigh damping
        if (a1 != 0.0 || a2 != 0.0 || a3 != 0.0)
        {
            P += this->getRayleighDampingForces();
        }
    }


    //     cerr << P;

    return P;

}

//Guanzhou added for PDD
int EightNodeBrick::sendSelf (int commitTag, Channel &theChannel)
{

    if ( !initialized )
    {
        populate();
    }

    int dataTag = this->getDbTag();
    int matDbTag;
    static ID idData(26);

    //the matDbTag will be the same for all matPoints in a SPECIFIC element
    //=========================================================================
    for (int i = 0; i < 8; i++)
    {
        idData(i) = (matpoint[i]->matmodel)->getClassTag();
        matDbTag  = (matpoint[i]->matmodel)->getDbTag();

        // NOTE: we do have to ensure that the material has a database
        // tag if we are sending it to a database channel.
        if (matDbTag == 0)
        {
            matDbTag = theChannel.getDbTag();

            if (matDbTag != 0)
            {
                (matpoint[i]->matmodel)->setDbTag(matDbTag);
            }
        }

        idData(i + 8) = matDbTag;
    }


    idData(17) = connectedExternalNodes(0);
    idData(18) = connectedExternalNodes(1);
    idData(19) = connectedExternalNodes(2);
    idData(20) = connectedExternalNodes(3);
    idData(21) = connectedExternalNodes(4);
    idData(22) = connectedExternalNodes(5);
    idData(23) = connectedExternalNodes(6);
    idData(24) = connectedExternalNodes(7);

    idData(25) = this->getTag();


    //    theChannel.sendName(string name);
    if (theChannel.sendID(dataTag, commitTag, idData) < 0)
    {
        cerr << "WARNING EightNodeBrick::sendSelf() - " << this->getTag() << " failed to send ID\n";
        return -1;
    }

    // Finally, 8node brick asks its material objects to send themselves
    for (int i = 0; i < 8; i++)
    {
        if ((matpoint[i]->matmodel)->sendSelf(commitTag, theChannel) < 0)
        {
            cerr << "WARNING EightNodeBrick::sendSelf() - " << this->getTag() << " failed to send material models\n";
            return -1;
        }
    }

    //sending resisting force vector
    static Vector matProp(1);
    matProp(0) = rho;

    if (theChannel.sendVector(dataTag, commitTag, matProp) < 0)
    {
        cerr << "WARNING EightNodeBrick::sendSelf() - " << this->getTag() << " failed to send its Material Property\n";
        return -1;
    }

    return 0;

}

//Guanzhou added
int EightNodeBrick::recvSelf (int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{


    if ( !initialized )
    {
        populate();    //Guanzhou added
    }

    int dataTag = this->getDbTag();

    static ID idData(26);


    // 8node brick now receives the tags of its four external nodes
    if (theChannel.recvID(dataTag, commitTag, idData) < 0)
    {
        cerr << "WARNING EightNodeBrick::recvSelf() - failed to receive ID\n";
        return -1;
    }

    this->setTag(idData(25));

    connectedExternalNodes(0) = idData(17);
    connectedExternalNodes(1) = idData(18);
    connectedExternalNodes(2) = idData(19);
    connectedExternalNodes(3) = idData(20);
    connectedExternalNodes(4) = idData(21);
    connectedExternalNodes(5) = idData(22);
    connectedExternalNodes(6) = idData(23);
    connectedExternalNodes(7) = idData(24);

    int matClassTag;
    int matDbTag;


    if (matpoint[0]->matmodel == 0)
    {
        for (int i = 0; i < 8; i++)
        {

            matClassTag = idData(i);
            matDbTag    = idData(i + 8);


            // Allocate new material with the sent class tag
            NDMaterial *ndmat = theBroker.getNewNDMaterial(matClassTag);

            if (ndmat == 0)
            {
                cerr << "EightNodeBrick::recvSelf() - Broker could not create NDMaterial of class type " << matClassTag << endln;
                return -1;
            }

            // Now receive materials into the newly allocated space
            ndmat->setDbTag(matDbTag);

            if ((ndmat)->recvSelf(commitTag, theChannel, theBroker) < 0)
            {
                cerr << "EightNodeBrick::recvSelf() - material " << i << "failed to recv itself\n";
                return -1;
            }

            matpoint[i]->matmodel = ndmat;
        }
    }
    // materials exist , ensure materials of correct type and recvSelf on them
    else
    {
        for (int i = 0; i < 8; i++)
        {

            matClassTag = idData(i);
            matDbTag    = idData(i + 8);

            static Vector matProp(4);

            if ( theChannel.recvVector(dataTag, commitTag, matProp) < 0 )
            {
                cerr << "EightNodeBrick::recvSelf() - failed to recv rho!\n";
                return -1;
            }

            NDMaterial *ndmat = theBroker.getNewNDMaterial(matClassTag);;

            // Check that material is of the right type; if not,
            // delete it and create a new one of the right type
            if ((matpoint[i]->matmodel)->getClassTag() != matClassTag)
            {
                delete matpoint[i]->matmodel;

                //        ndmat = theBroker.getNewNDMaterial(matClassTag);
                if (ndmat ==  0)
                {
                    cerr << "EightNodeBrick::recvSelf() - Broker could not create NDMaterial of class type " << matClassTag << endln;
                    return -1;
                }

                ndmat->setDbTag(matDbTag);
            }

            // Receive the material
            if ((ndmat)->recvSelf(commitTag, theChannel, theBroker) < 0)
            {
                cerr << "EightNodeBrick::recvSelf() - material " << i << "failed to recv itself\n";
                return -1;
            }

            matpoint[i]->matmodel = ndmat;

        }
    }

    static Vector matProp(1);

    if ( theChannel.recvVector(dataTag, commitTag, matProp) < 0 )
    {
        cerr << "EightNodeBrick::recvSelf() - failed to recv rho!\n";
        return -1;
    }

    rho = matProp(0);

    return 0;

}


//Guanzhou added for PDD
int EightNodeBrick::getObjectSize()
{
    int size = 0;
    size += 11 * sizeof(int);
    size += 4 * sizeof(double);

    for (int i = 0; i < 8; i++)
    {
        size += (matpoint[i]->matmodel)->getObjectSize();
    }

    //size for resisting force vector
    //    size += 24*sizeof(double);
    return size;
}

void EightNodeBrick::Print(ostream &s, int flag)
{
    if (flag == 1)
    {

        s << "EightNodeBrick, element id:  " << this->getTag() << endln;
        s << "Connected external nodes:  " << connectedExternalNodes;
        s << this->getResistingForce();

    }
    else
    {

        //report(" EightNodeBrick ");
        s << "EightNodeBrick, element id:  " << this->getTag() << endln;
        s <<  this->getResistingForce();

        //GZ out s << "Connected external nodes:  " << connectedExternalNodes;
        //GZ out
        //GZ out int total_number_of_Gauss_points = integration_order*
        //GZ out                                    integration_order*
        //GZ out                                    integration_order;
        //GZ out if ( total_number_of_Gauss_points != 0 )
        //GZ out   {
        //GZ out  theNodes[0]->Print(s);
        //GZ out  theNodes[1]->Print(s);
        //GZ out  theNodes[2]->Print(s);
        //GZ out  theNodes[3]->Print(s);
        //GZ out  theNodes[4]->Print(s);
        //GZ out  theNodes[5]->Print(s);
        //GZ out        theNodes[6]->Print(s);
        //GZ out  theNodes[7]->Print(s);
        //GZ out }
        //GZ out s << "Element mass density:  " << rho << endln << endln;
        //GZ out s << "Material model:  " << endln;
        //GZ out
        //GZ out for( int GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
        //GZ out {
        //GZ out   for( int GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        //GZ out   {
        //GZ out     for( int GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
        //GZ out     {
        //GZ out        // this short routine is supposed to calculate position of
        //GZ out        // Gauss point from 3D array of short's
        //GZ out        short where =
        //GZ out        ((GP_c_r-1)*integration_order+GP_c_s-1)*integration_order+GP_c_t-1;
        //GZ out
        //GZ out        s << "\n insde Gauss-Legendre loop: where = " << where << endln;
        //GZ out        s << " GP_c_r= " << GP_c_r << "GP_c_s = " << GP_c_s << " GP_c_t = " << GP_c_t << endln;
        //GZ out        matpoint[where]->report("Material Point\n");
        //GZ out     }
        //GZ out   }
        //GZ out}
    }
}

//=============================================================================
// Response *EightNodeBrick::setResponse (const char **argv, int argc, Information &eleInformation)
// {
//     //========================================================
//     //     if (strcmp(argv[0],"force") == 0 || strcmp(argv[0],"forces") == 0)
//     //     return new ElementResponse(this, 1, P);

//     //========================================================
//     if (strcmp(argv[0], "stiff") == 0 || strcmp(argv[0], "stiffness") == 0)
//     {
//         return new ElementResponse(this, 2, K);
//     }

//     //========================================================
//     else if (strcmp(argv[0], "plasticGPC") == 0 || strcmp(argv[0], "plastifiedGPC") == 0)
//     {
//         //checking if element plastified
//         //int count  = integration_order* integration_order * integration_order;
//         //straintensor pl_stn;
//         //int plastify = 0;
//         //
//         //for (int i = 0; i < count; i++) {
//         //  pl_stn = matpoint[i]->getPlasticStrainTensor();
//         //   double  p_plastc = pl_stn.p_hydrostatic();
//         //
//         //   if (  fabs(p_plastc) > 0 ) {
//         //      plastify = 1;
//         //      break;
//         //   }
//         //}

//         return new ElementResponse(this, 3, InfoP);
//     }
//     //========================================================
//     else if (strcmp(argv[0], "plastic") == 0 || strcmp(argv[0], "plastified") == 0)
//     {
//         return new ElementResponse(this, 31, InfoP1);
//     }
//     //========================================================
//     else if (strcmp(argv[0], "stress") == 0 || strcmp(argv[0], "stresses") == 0)
//     {
//         return new ElementResponse(this, 4, InfoS);
//     }
//     //========================================================
//     else if (strcmp(argv[0], "pq") == 0 || strcmp(argv[0], "PQ") == 0)
//     {
//         return new ElementResponse(this, 41, InfoSpq);
//     }
//     //Added 06-27-02 for p-q
//     //========================================================
//     else if (strcmp(argv[0], "stresspq") == 0 || strcmp(argv[0], "stressespq") == 0)
//     {
//         return new ElementResponse(this, 41, InfoSpq);
//     }
//     //Added 07-22-02 for all p-q, e_v_pl, xi
//     //========================================================
//     else if (strcmp(argv[0], "pqall") == 0 )
//     {
//         return new ElementResponse(this, 42, InfoSpq_all);
//     }

//     //========================================================
//     else if (strcmp(argv[0], "gausspoint") == 0 || strcmp(argv[0], "GaussPoint") == 0)
//     {
//         return new ElementResponse(this, 5, Gsc8);
//     }
//     //Guanzhou added
//     else if (strcmp(argv[0], "plasticEnergy") == 0 || strcmp(argv[0], "plasticenergy") == 0)
//     {
//         return new ElementResponse(this, 6, epEnergy);
//     }
//     //========================================================
//     //Nima added for recording the Strain (10-13-2008)

//     else if ( (strcmp(argv[0], "strains") == 0) || (strcmp(argv[0], "strain") == 0) )
//     {
//         return new ElementResponse(this, 7, InfoStr );
//     }

//     //========================================================
//     //Nima added for recording the Strain (April 2010)

//     else if ( (strcmp(argv[0], "vol_strain") == 0) || (strcmp(argv[0], "volumetric_strain") == 0) )
//     {
//         return new ElementResponse(this, 71, e_p );
//     }

//     //========================================================
//     //Nima added for recording the Mass Matrix (Feb 2010)

//     //    else if ( (strcmp(argv[0],"mass") == 0) || (strcmp(argv[0],"Mass") ==0) )
//     //      return new ElementResponse(this, 8, M);

//     //========================================================


//     /*else if (strcmp(argv[0],"material") == 0 || strcmp(argv[0],"integrPoint") == 0) {
//       int pointNum = atoi(argv[1]);
//       if (pointNum > 0 && pointNum <= 4)
//         return theMaterial[pointNum-1]->setResponse(&argv[2], argc-2, eleInfo);
//         else
//         return 0;
//     }*/

//     // otherwise response quantity is unknown for the quad class
//     else
//     {
//         return 0;
//     }
// }

// //=============================================================================
// int EightNodeBrick::getResponse (int responseID, Information &eleInfo)
// {
//     switch (responseID)
//     {

//             //        case 1:
//             //          return eleInfo.setVector(this->getResistingForce());
//             //          break;

//         case 2:
//             return eleInfo.setMatrix(this->getTangentStiff());
//             break;


//         case 3:
//         {
//             //checking if element plastified
//             int count  = integration_order * integration_order * integration_order;
//             //cerr << count << endln;
//             //Vector Gsc(FixedOrder*FixedOrder*FixedOrder*3+1);  // 8*3 + count
//             computeGaussPoint();
//             //cerr << count << endln;

//             //Vector Info(109); // count * 4 +1
//             InfoP(0) = Gsc8(0); //Number of Gauss point

//             straintensor pl_stn;

//             //         int plastify;
//             for (int i = 0; i < count; i++)
//             {
//                 //             plastify = 0;
//                 InfoP(i * 4 + 1) = Gsc8(i * 3 + 1); //x
//                 InfoP(i * 4 + 2) = Gsc8(i * 3 + 2); //y
//                 InfoP(i * 4 + 3) = Gsc8(i * 3 + 3); //z
//                 pl_stn = matpoint[i]->getPlasticStrainTensor();
//                 //double  p_plastc = pl_stn.p_hydrostatic();
//                 double  q_plastc = pl_stn.q_deviatoric();

//                 InfoP(i * 4 + 4) = q_plastc; //plastify; //Plastified?

//             }

//             return eleInfo.setVector( InfoP );
//             break;

//             //return plastify;

//         }

//         case 4:
//         {
//             int count = integration_order * integration_order * integration_order;
//             int i;
//             stresstensor sts;
//             //Vector Gsc(81+1);  // 8*3 + count
//             //Gsc = this->reportTensor("Gauss Point Coor.");

//             //Vector Info(109 + 3 ); //Z values, x-disp. and corresponding avg. moment
//             InfoS(0) = count;

//             for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//             {
//                 //r = get_Gauss_p_c( integration_order, GP_c_r );
//                 for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//                 {
//                     //s = get_Gauss_p_c( integration_order, GP_c_s );
//                     //rs = (GP_c_r-1)*integration_order+GP_c_s-1;

//                     for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//                     {
//                         //for (int i = 0; i < count; i++)
//                         i =
//                             ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                         sts = matpoint[i]->getStressTensor();
//                         InfoS(i * 6 + 1) = sts.cval(1, 1); //sigma_xx
//                         InfoS(i * 6 + 2) = sts.cval(2, 2); //sigma_yy
//                         InfoS(i * 6 + 3) = sts.cval(3, 3); //sigma_zz
//                         InfoS(i * 6 + 4) = sts.cval(1, 2); //Assign sigma_xy
//                         InfoS(i * 6 + 5) = sts.cval(1, 3); //Assign sigma_xz
//                         InfoS(i * 6 + 6) = sts.cval(2, 3); //Assign sigma_yz
//                     }
//                 }
//             }

//             return eleInfo.setVector( InfoS );
//             break;
//         }

//         case 41:
//         {
//             int count = integration_order * integration_order * integration_order;
//             count = count / 2;
//             stresstensor sts;
//             sts = matpoint[count]->getStressTensor();
//             InfoSpq(0) = sts.p_hydrostatic();
//             InfoSpq(1) = sts.q_deviatoric();
//             return eleInfo.setVector( InfoSpq );
//         }

//         case 42:
//         {
//             //         int count = integration_order* integration_order * integration_order;
//             int i;
//             stresstensor sts, principle;

//             //InfoSpq_all(0) = count;

//             for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//             {
//                 //r = get_Gauss_p_c( integration_order, GP_c_r );
//                 for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//                 {
//                     //s = get_Gauss_p_c( integration_order, GP_c_s );
//                     //rs = (GP_c_r-1)*integration_order+GP_c_s-1;

//                     for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//                     {
//                         //for (int i = 0; i < count; i++)
//                         i =
//                             ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                         sts = matpoint[i]->getStressTensor();
//                         InfoSpq_all(i * 2 + 0) = sts.p_hydrostatic();
//                         //deviatoric stress sqrt(J2/3)
//                         InfoSpq_all(i * 2 + 1) = sts.q_deviatoric();
//                         //deviator stress +/-
//                         principle = sts.principal();

//                         //InfoSpq_all(i*2+1) =principle.val(1,1)-principle.val(3,3);

//                         if (i == 7)
//                         {
//                             InfoSpq_all(i * 2 + 2) = principle.val(1, 1);
//                             //InfoSpq_all(i*2+3) = principle.val(2,2);
//                             InfoSpq_all(i * 2 + 3) = principle.val(3, 3);

//                             //Output volumetric strain for the eight Gauss point
//                             straintensor pl_stn;
//                             pl_stn = matpoint[i]->getPlasticStrainTensor();
//                             //pl_stn = matpoint[i]->getStrainTensor();
//                             InfoSpq_all(i * 2 + 4) = pl_stn.Iinvariant1();
//                             double psi = matpoint[i]->getpsi();
//                             InfoSpq_all(i * 2 + 5) = psi;
//                         }
//                     }
//                 }
//             }

//             return eleInfo.setVector( InfoSpq_all );
//             break;
//         }

//         case 5:
//         {
//             this->computeGaussPoint();
//             return eleInfo.setVector(Gsc8);
//             break;
//         }

//         case 31:
//         {
//             // Output element plastic info
//             int count  = integration_order * integration_order * integration_order;

//             InfoP1(0) = count; //Number of Gauss point

//             straintensor pl_stn;

//             for (int i = 0; i < count; i++)
//             {
//                 pl_stn = matpoint[i]->getPlasticStrainTensor();
//                 //double  p_plastc = pl_stn.p_hydrostatic();
//                 double  q_plastc = pl_stn.q_deviatoric();

//                 InfoP1(i + 1) = q_plastc; //plastify; //Plastified?

//             }

//             return eleInfo.setVector( InfoP1 );
//             break;
//         }

//         //Guanzhou added
//         case 6:
//         {

//             NDMaterial *pMat;
//             //static straintensor pl_stn[8];
//             //static stresstensor stress[8];
//             //double tmp = 0.0;
//             //epEnergy = 0.0;
//             straintensor tmp_strain;
//             stresstensor tmp_stress;
//             double r  = 0.0;
//             double rw = 0.0;
//             double s  = 0.0;
//             double sw = 0.0;
//             double t  = 0.0;
//             double tw = 0.0;

//             short where = 0;
//             double weight = 0.0;

//             int dh_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

//             tensor dh(2, dh_dim, 0.0);

//             double det_of_Jacobian = 0.0;

//             tensor Jacobian;

//             for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//             {
//                 r = get_Gauss_p_c( integration_order, GP_c_r );
//                 rw = get_Gauss_p_w( integration_order, GP_c_r );

//                 for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//                 {
//                     s = get_Gauss_p_c( integration_order, GP_c_s );
//                     sw = get_Gauss_p_w( integration_order, GP_c_s );

//                     for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//                     {
//                         t = get_Gauss_p_c( integration_order, GP_c_t );
//                         tw = get_Gauss_p_w( integration_order, GP_c_t );

//                         where =
//                             ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                         // derivatives of local coordiantes with respect to local coordiantes
//                         dh = dh_drst_at(r, s, t);

//                         // Jacobian tensor ( matrix )
//                         Jacobian = Jacobian_3D(dh);
//                         //....                Jacobian.print("J");

//                         det_of_Jacobian  = Jacobian.determinant();
//                         //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );

//                         //weight
//                         weight = rw * sw * tw * det_of_Jacobian;
//                         //nodal_forces.print("nf","\n\n Nodal Forces \n");

//                         pMat = matpoint[where]->getNDMat();
//                         tmp_strain = pMat->getPlasticStrainTensor();
//                         //tmp_strain = pMat->getStrainTensor();
//                         tmp_stress = pMat->getStressTensor();
//                         //cerr << "tmp_strain: \n" << tmp_strain;
//                         //cerr << "tmp_stress: \n" << tmp_stress;
//                         tmp_strain = tmp_strain - pl_stn[where];//delta plastic strain
//                         tmp_stress = tmp_stress + stress[where];
//                         tmp_stress = tmp_stress * 0.5; //average stress
//                         //cerr << "tmp_strain: \n" << tmp_strain;
//                         //cerr << "tmp_stress: \n" << tmp_stress;

//                         //tensor temp1( 2, def_dim_2, 0.0);
//                         stresstensor temp1;


//                         temp1 = tmp_stress("ij") * tmp_strain("ij");
//                         //cerr << "temp1: \n" << temp1;
//                         temp1.null_indices();
//                         //tmp = tmp + temp1.trace();
//                         //cerr << "tmp" << tmp << '\n';
//                         //pl_stn[where] = pMat->getStrainTensor();
//                         pl_stn[where] = pMat->getPlasticStrainTensor();
//                         stress[where] = pMat->getStressTensor();
//                         epEnergy = epEnergy + temp1.trace() * weight;
//                     }
//                 }
//             }

//             return eleInfo.setDouble(epEnergy);
//             break;
//         }


//         //************************************************************************************************

//         //Nima added for recording the Strain (10-13-2008)

//         case 7:

//         {
//             int count = integration_order * integration_order * integration_order;
//             int i;
//             straintensor str;

//             InfoStr(0) = count;

//             for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//             {
//                 for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//                 {
//                     for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//                     {
//                         i = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                         str = matpoint[i]->getStrainTensor();

//                         InfoStr(i * 6 + 1) = str.cval(1, 1); //sigma_xx
//                         InfoStr(i * 6 + 2) = str.cval(2, 2); //sigma_yy
//                         InfoStr(i * 6 + 3) = str.cval(3, 3); //sigma_zz
//                         InfoStr(i * 6 + 4) = str.cval(1, 2); //Assign sigma_xy
//                         InfoStr(i * 6 + 5) = str.cval(1, 3); //Assign sigma_xz
//                         InfoStr(i * 6 + 6) = str.cval(2, 3); //Assign sigma_yz
//                     }
//                 }
//             }

//             return eleInfo.setVector( InfoStr );
//             break;
//         }

//         //************************************************************************************************

//         //Nima added for recording the Volumetric_Strain (April 2010)

//         case 71:

//         {
//             int count = integration_order * integration_order * integration_order;
//             int i;
//             straintensor str;

//             InfoStr(0) = count;

//             for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
//             {
//                 for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
//                 {
//                     for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
//                     {
//                         i = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

//                         str = matpoint[i]->getStrainTensor();


//                         e_p = str.cval(1, 1) + str.cval(2, 2) + str.cval(3, 3);
//                     }
//                 }
//             }

//             return eleInfo.setDouble(e_p);
//             break;
//         }


//         // //************************************************************************************************
//         // //Nima added for recording the Mass matrix (Feb. 2010)
//         //  case 8:
//         //            return eleInfo.setMatrix(this->getMass());
//         //     break;

//         default:
//             return -1;
//             break;
//     }

//     return (-1);

// }

//Guanzhou added to calculate plastic energy
void EightNodeBrick::ComputeVolume()
{
    Volume = 0.0;

    double r  = 0.0;
    double rw = 0.0;
    double s  = 0.0;
    double sw = 0.0;
    double t  = 0.0;
    double tw = 0.0;

    //     short where = 0;
    double weight = 0.0;

    int dh_dim[] = {8, 3}; // Xiaoyan changed from {20,3 to {8,3} for 8 nodes

    tensor dh(2, dh_dim, 0.0);

    double det_of_Jacobian = 0.0;

    tensor Jacobian;

    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );
        rw = get_Gauss_p_w( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );
            sw = get_Gauss_p_w( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );
                tw = get_Gauss_p_w( integration_order, GP_c_t );

                //                 where = ((GP_c_r-1)*integration_order+GP_c_s-1)*integration_order+GP_c_t-1;

                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);

                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");

                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );

                //weight
                weight = rw * sw * tw * det_of_Jacobian;
                Volume = Volume + 8.0 * weight;
                //nodal_forces.print("nf","\n\n Nodal Forces \n");

            }
        }
    }
}


double EightNodeBrick::get_Gauss_p_c(short order, short point_numb)
{
    //  Abscissae coefficient of the Gaussian quadrature formula
    // starting from 1 not from 0
    static double Gauss_coordinates[7][7];

    Gauss_coordinates[1][1] = 0.0 ;
    Gauss_coordinates[2][1] = -0.577350269189626;
    Gauss_coordinates[2][2] = -Gauss_coordinates[2][1];
    Gauss_coordinates[3][1] = -0.774596669241483;
    Gauss_coordinates[3][2] = 0.0;
    Gauss_coordinates[3][3] = -Gauss_coordinates[3][1];
    Gauss_coordinates[4][1] = -0.861136311594053;
    Gauss_coordinates[4][2] = -0.339981043584856;
    Gauss_coordinates[4][3] = -Gauss_coordinates[4][2];
    Gauss_coordinates[4][4] = -Gauss_coordinates[4][1];
    Gauss_coordinates[5][1] = -0.906179845938664;
    Gauss_coordinates[5][2] = -0.538469310105683;
    Gauss_coordinates[5][3] = 0.0;
    Gauss_coordinates[5][4] = -Gauss_coordinates[5][2];
    Gauss_coordinates[5][5] = -Gauss_coordinates[5][1];
    Gauss_coordinates[6][1] = -0.932469514203152;
    Gauss_coordinates[6][2] = -0.661209386466265;
    Gauss_coordinates[6][3] = -0.238619186083197;
    Gauss_coordinates[6][4] = -Gauss_coordinates[6][3];
    Gauss_coordinates[6][5] = -Gauss_coordinates[6][2];
    Gauss_coordinates[6][6] = -Gauss_coordinates[6][1];

    return Gauss_coordinates[order][point_numb];
}

double EightNodeBrick::get_Gauss_p_w(short order, short point_numb)
{
    //  Weight coefficient of the Gaussian quadrature formula
    // starting from 1 not from 0
    static double Gauss_weights[7][7]; // static data ??

    Gauss_weights[1][1] = 2.0;
    Gauss_weights[2][1] = 1.0;
    Gauss_weights[2][2] = 1.0;
    Gauss_weights[3][1] = 0.555555555555556;
    Gauss_weights[3][2] = 0.888888888888889;
    Gauss_weights[3][3] = Gauss_weights[3][1];
    Gauss_weights[4][1] = 0.347854845137454;
    Gauss_weights[4][2] = 0.652145154862546;
    Gauss_weights[4][3] = Gauss_weights[4][2];
    Gauss_weights[4][4] = Gauss_weights[4][1];
    Gauss_weights[5][1] = 0.236926885056189;
    Gauss_weights[5][2] = 0.478628670499366;
    Gauss_weights[5][3] = 0.568888888888889;
    Gauss_weights[5][4] = Gauss_weights[5][2];
    Gauss_weights[5][5] = Gauss_weights[5][1];
    Gauss_weights[6][1] = 0.171324492379170;
    Gauss_weights[6][2] = 0.360761573048139;
    Gauss_weights[6][3] = 0.467913934572691;
    Gauss_weights[6][4] = Gauss_weights[6][3];
    Gauss_weights[6][5] = Gauss_weights[6][2];
    Gauss_weights[6][6] = Gauss_weights[6][1];

    return Gauss_weights[order][point_numb];
}



int EightNodeBrick::update(void) //Note: Guanzhou finished the algorithm consistent with global incremental calculation Mar2005
{

    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    short where = 0;

    static int dh_dim[] = {8, 3};
    tensor dh(2, dh_dim, 0.0);

    static int disp_dim[] = {8, 3};
    //GZ out tensor incr_displacements(2,disp_dim,0.0);

    //GZ out straintensor incr_strain;

    tensor Jacobian;
    tensor JacobianINV;
    tensor dhGlobal;

    //Guanzhou out incr_displacements = incr_disp();//Get incrmental disp from domain

    tensor trial_disp(2, disp_dim, 0.0);
    trial_disp = total_disp();//Guanzhou added, get trial disp from domain

    straintensor trial_strain;


    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );

                where = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;
                // derivatives of local coordiantes with respect to local coordiantes
                dh = dh_drst_at(r, s, t);
                // Jacobian tensor ( matrix )
                Jacobian = Jacobian_3D(dh);
                //....                Jacobian.print("J");
                // Inverse of Jacobian tensor ( matrix )
                JacobianINV = Jacobian_3Dinv(dh);
                //....                JacobianINV.print("JINV");
                // determinant of Jacobian tensor ( matrix )
                //--                det_of_Jacobian  = Jacobian.determinant();
                //....  ::printf("determinant of Jacobian is %f\n",Jacobian_determinant );
                // Derivatives of local coordinates multiplied with inverse of Jacobian (see Bathe p-202)
                dhGlobal = dh("ij") * JacobianINV("kj");
                // incrmental straines at this Gauss point
                // now in Update we know the total displacements so let's find
                // the total strain
                trial_strain = (dhGlobal("ib") * trial_disp("ia")).symmetrize11();
                trial_strain.null_indices();

                //Guanzhou out Mar2005 if ( ( (matpoint[where]->matmodel)->setTrialStrainIncr( incr_strain)) )
                if ( ( (matpoint[where]->matmodel)->setTrialStrain(trial_strain)) )
                {
                    cerr << "EightNodeBrick::update (tag: " << this->getTag() << "), Update Failed\n";
                }

            }
        }
    }

    return 0;

}



/////////////////////////////////////////////////////////////////////////
// Nima Tafazzoli added for surface load (July 2012)
double EightNodeBrick::SurfaceShapeFunctionValues(double Xi , double Eta, int whichcomponent)
{
    ShapeFunctionValues_in_function(0) = 0.25 * (1 + Xi) * (1 + Eta);
    ShapeFunctionValues_in_function(1) = 0.25 * (1 - Xi) * (1 + Eta);
    ShapeFunctionValues_in_function(2) = 0.25 * (1 - Xi) * (1 - Eta);
    ShapeFunctionValues_in_function(3) = 0.25 * (1 + Xi) * (1 - Eta);

    return ShapeFunctionValues_in_function(whichcomponent);

}


Vector &EightNodeBrick::Direction_Weight(double Xi , double Eta, Vector coord1, Vector coord2, Vector coord3, Vector coord4)
{
    Vector J1(3);
    Vector J2(3);

    J1(0) = 0.25 * ( (1 + Eta) * coord1(0) - (1 + Eta) * coord2(0) - (1 - Eta) * coord3(0) + (1 - Eta) * coord4(0) );
    J1(1) = 0.25 * ( (1 + Eta) * coord1(1) - (1 + Eta) * coord2(1) - (1 - Eta) * coord3(1) + (1 - Eta) * coord4(1) );
    J1(2) = 0.25 * ( (1 + Eta) * coord1(2) - (1 + Eta) * coord2(2) - (1 - Eta) * coord3(2) + (1 - Eta) * coord4(2) );


    J2(0) = 0.25 * ( (1 + Xi) * coord1(0) + (1 - Xi) * coord2(0) - (1 - Xi) * coord3(0) - (1 + Xi) * coord4(0) );
    J2(1) = 0.25 * ( (1 + Xi) * coord1(1) + (1 - Xi) * coord2(1) - (1 - Xi) * coord3(1) - (1 + Xi) * coord4(1) );
    J2(2) = 0.25 * ( (1 + Xi) * coord1(2) + (1 - Xi) * coord2(2) - (1 - Xi) * coord3(2) - (1 + Xi) * coord4(2) );


    J_vector_in_function(0) = fabs( J1(1) * J2(2) - J1(2) * J2(1) );
    J_vector_in_function(1) = fabs( J1(2) * J2(0) - J1(0) * J2(2) );
    J_vector_in_function(2) = fabs( J1(0) * J2(1) - J1(1) * J2(0) );


    return J_vector_in_function;
}



double EightNodeBrick::SurfaceLoadValues(double Xi , double Eta, Vector Pressure)
{

    SurfaceLoadValues_in_function =  0.25 * (1 + Xi) * (1 + Eta) * Pressure(0)
                                     + 0.25 * (1 - Xi) * (1 + Eta) * Pressure(1)
                                     + 0.25 * (1 - Xi) * (1 - Eta) * Pressure(2)
                                     + 0.25 * (1 + Xi) * (1 - Eta) * Pressure(3);



    return SurfaceLoadValues_in_function;

}


// Nima Tafazzoli (September 2012)
int
EightNodeBrick::CheckMesh(ofstream &checkmesh_file)
{


    double r  = 0.0;
    double s  = 0.0;
    double t  = 0.0;

    int dh_dim[] = {8, 3};
    tensor dh(2, dh_dim, 0.0);


    double det_of_Jacobian = 0.0;
    bool jacobian_flag = false;

    tensor Jacobian;


    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        r = get_Gauss_p_c( integration_order, GP_c_r );

        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            s = get_Gauss_p_c( integration_order, GP_c_s );

            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                t = get_Gauss_p_c( integration_order, GP_c_t );

                // derivatives of local coordinates with respect to local coordinates
                dh = dh_drst_at(r, s, t);


                // Jacobian tensor (matrix)
                Jacobian = Jacobian_3D(dh);


                // determinant of Jacobian tensor (matrix)
                det_of_Jacobian  = Jacobian.determinant();


                if (det_of_Jacobian <= 0)
                {
                    checkmesh_file << "ERROR!!! Element number (" << this->getTag() << "): has negative Jacobian determinant!" << endl;
                    jacobian_flag = true;
                    break;
                }


            }

            if (jacobian_flag == true)
            {
                break;
            }

        }

        if (jacobian_flag == true)
        {
            break;
        }

    }



    return 0;

}


//==================================================================================
Vector *
EightNodeBrick::getStress(void)
{

    int i = 0;
    int j = 0;
    stresstensor sts;
    Vector *stresses = new Vector(48);


    for ( short GP_c_r = 1 ; GP_c_r <= integration_order ; GP_c_r++ )
    {
        for ( short GP_c_s = 1 ; GP_c_s <= integration_order ; GP_c_s++ )
        {
            for ( short GP_c_t = 1 ; GP_c_t <= integration_order ; GP_c_t++ )
            {
                i = ((GP_c_r - 1) * integration_order + GP_c_s - 1) * integration_order + GP_c_t - 1;

                sts = matpoint[i]->getStressTensor();
                (*stresses)(j * 6 + 0) = sts.cval(1, 1); //sigma_xx
                (*stresses)(j * 6 + 1) = sts.cval(2, 2); //sigma_yy
                (*stresses)(j * 6 + 2) = sts.cval(3, 3); //sigma_zz
                (*stresses)(j * 6 + 3) = sts.cval(1, 2); //Assign sigma_xy
                (*stresses)(j * 6 + 4) = sts.cval(1, 3); //Assign sigma_xz
                (*stresses)(j * 6 + 5) = sts.cval(2, 3); //Assign sigma_yz

                j++;
            }
        }
    }

    return stresses;

}


double EightNodeBrick::returnPressure(void)
{
    return 0;
}




#endif











