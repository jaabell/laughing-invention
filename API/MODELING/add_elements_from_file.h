///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral LGPL (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     LGPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Domain Specific Language (DSL)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              January 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Constructing a Node</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (December 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of constructing a node.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - inputelementsfile: name of the input file which has the elements information

//! Format of input file:
//! - The first argument should be the type of the elements which could be
//! one of the followings: Truss / EightNodeBrick / EightNodeBrickupU /
//! TwentyNodeBrick / TwentyNodeBrickupU / TwentySevenNodeBrick /
//! DisplacementBeam / ElasticBeamLumpedMass /
//! Contact3DOF3DOF / Contact3DOF6DOF / Contact6DOF6DOF
//! - The rest of the arguments should be the same as inputs corresponding to each type of element
//! Do not put any blank line at the end of the input file.


int add_elements_from_file(string inputelementsfile)
{

    const char* inputfromfile = inputelementsfile.c_str();

    ifstream inputelements;

    if(! inputelements)
    {
        cerr << "Error: (add_elements_from_file) Can not open " << inputelementsfile << "!" << endl;
        return -1;
    }

    inputelements.open(inputfromfile , ios::in);

    string typeofelement;

    while (!inputelements.eof())
    {

        inputelements >> typeofelement;


        if (typeofelement.compare("Truss") == 0)
        {
            int ElementNumber;
            int iNode;
            int jNode;
            int MaterialNumber;
            double A;
            double rho;

            inputelements >> ElementNumber >> iNode >>
                          jNode >> MaterialNumber >> A >> rho;

            add_element_truss(ElementNumber, iNode, jNode, MaterialNumber, A, rho);
        }

        else if (typeofelement.compare("EightNodeBrick") == 0)
        {
            int ElementNumber;
            int node_numb_1;
            int node_numb_2;
            int node_numb_3;
            int node_numb_4;
            int node_numb_5;
            int node_numb_6;
            int node_numb_7;
            int node_numb_8;
            int MaterialNumber;
            int AccelerationFieldNumber;


            inputelements >> ElementNumber >> node_numb_1 >> node_numb_2 >> node_numb_3 >> node_numb_4 >>
                          node_numb_5 >> node_numb_6 >> node_numb_7 >> node_numb_8 >> MaterialNumber >> AccelerationFieldNumber;

            add_element_brick_8node(ElementNumber, node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, MaterialNumber, AccelerationFieldNumber);

        }

        else if (typeofelement.compare("EightNodeBrickupU") == 0)
        {
            int ElementNumber;
            int node_numb_1;
            int node_numb_2;
            int node_numb_3;
            int node_numb_4;
            int node_numb_5;
            int node_numb_6;
            int node_numb_7;
            int node_numb_8;
            int MaterialNumber;
            int AccelerationFieldNumber;
            double porosity;
            double alpha;
            double rho_s;
            double rho_f;
            double k_x;
            double k_y;
            double k_z;
            double K_s;
            double K_f;


            inputelements >> ElementNumber >> node_numb_1 >> node_numb_2 >> node_numb_3 >>
                          node_numb_4 >> node_numb_5 >> node_numb_6 >> node_numb_7 >> node_numb_8 >>
                          MaterialNumber >> AccelerationFieldNumber >> porosity >> alpha >> rho_s >>
                          rho_f >> k_x >> k_y >> k_z >> K_s >> K_f;

            add_element_brick_8node_upU(ElementNumber,
                                        node_numb_1,
                                        node_numb_2,
                                        node_numb_3,
                                        node_numb_4,
                                        node_numb_5,
                                        node_numb_6,
                                        node_numb_7,
                                        node_numb_8,
                                        MaterialNumber,
                                        AccelerationFieldNumber,
                                        porosity,
                                        alpha,
                                        rho_s,
                                        rho_f,
                                        k_x,
                                        k_y,
                                        k_z,
                                        K_s,
                                        K_f);
        }

        else if (typeofelement.compare("TwentyNodeBrick") == 0)
        {
            int ElementNumber;
            int node_numb_1;
            int node_numb_2;
            int node_numb_3;
            int node_numb_4;
            int node_numb_5;
            int node_numb_6;
            int node_numb_7;
            int node_numb_8;
            int node_numb_9;
            int node_numb_10;
            int node_numb_11;
            int node_numb_12;
            int node_numb_13;
            int node_numb_14;
            int node_numb_15;
            int node_numb_16;
            int node_numb_17;
            int node_numb_18;
            int node_numb_19;
            int node_numb_20;
            int MaterialNumber;
            int AccelerationFieldNumber;


            inputelements >> ElementNumber >> node_numb_1 >> node_numb_2 >> node_numb_3 >>
                          node_numb_4 >> node_numb_5 >> node_numb_6 >> node_numb_7 >> node_numb_8 >>
                          node_numb_9 >> node_numb_10 >> node_numb_11 >> node_numb_12 >> node_numb_13 >>
                          node_numb_14 >> node_numb_15 >> node_numb_16 >> node_numb_17 >> node_numb_18 >>
                          node_numb_19 >> node_numb_20 >> MaterialNumber >> AccelerationFieldNumber;

            add_element_brick_20node(ElementNumber,
                                     node_numb_1,
                                     node_numb_2,
                                     node_numb_3,
                                     node_numb_4,
                                     node_numb_5,
                                     node_numb_6,
                                     node_numb_7,
                                     node_numb_8,
                                     node_numb_9,
                                     node_numb_10,
                                     node_numb_11,
                                     node_numb_12,
                                     node_numb_13,
                                     node_numb_14,
                                     node_numb_15,
                                     node_numb_16,
                                     node_numb_17,
                                     node_numb_18,
                                     node_numb_19,
                                     node_numb_20,
                                     MaterialNumber,
                                     AccelerationFieldNumber);
        }

        else if (typeofelement.compare("TwentyNodeBrickupU") == 0)
        {
            int ElementNumber;
            int node_numb_1;
            int node_numb_2;
            int node_numb_3;
            int node_numb_4;
            int node_numb_5;
            int node_numb_6;
            int node_numb_7;
            int node_numb_8;
            int node_numb_9;
            int node_numb_10;
            int node_numb_11;
            int node_numb_12;
            int node_numb_13;
            int node_numb_14;
            int node_numb_15;
            int node_numb_16;
            int node_numb_17;
            int node_numb_18;
            int node_numb_19;
            int node_numb_20;
            int MaterialNumber;
            double AccelerationFieldNumber;
            double porosity;
            double alpha;
            double rho_s;
            double rho_f;
            double k_x;
            double k_y;
            double k_z;
            double K_s;
            double K_f;


            inputelements >> ElementNumber >> node_numb_1 >> node_numb_2 >> node_numb_3 >>
                          node_numb_4 >> node_numb_5 >> node_numb_6 >> node_numb_7 >> node_numb_8 >>
                          node_numb_9 >> node_numb_10 >> node_numb_11 >> node_numb_12 >> node_numb_13 >>
                          node_numb_14 >> node_numb_15 >> node_numb_16 >> node_numb_17 >> node_numb_18 >>
                          node_numb_19 >> node_numb_20 >> MaterialNumber >> AccelerationFieldNumber >> porosity >>
                          alpha >> rho_s >> rho_f >> k_x >> k_y >> k_z >> K_s >> K_f;

            add_element_brick_20node_upU(ElementNumber,
                                         node_numb_1,
                                         node_numb_2,
                                         node_numb_3,
                                         node_numb_4,
                                         node_numb_5,
                                         node_numb_6,
                                         node_numb_7,
                                         node_numb_8,
                                         node_numb_9,
                                         node_numb_10,
                                         node_numb_11,
                                         node_numb_12,
                                         node_numb_13,
                                         node_numb_14,
                                         node_numb_15,
                                         node_numb_16,
                                         node_numb_17,
                                         node_numb_18,
                                         node_numb_19,
                                         node_numb_20,
                                         MaterialNumber,
                                         AccelerationFieldNumber,
                                         porosity,
                                         alpha,
                                         rho_s,
                                         rho_f,
                                         k_x,
                                         k_y,
                                         k_z,
                                         K_s,
                                         K_f);
        }

        else if (typeofelement.compare("TwentySevenNodeBrick") == 0)
        {
            int ElementNumber;
            int node_numb_1;
            int node_numb_2;
            int node_numb_3;
            int node_numb_4;
            int node_numb_5;
            int node_numb_6;
            int node_numb_7;
            int node_numb_8;
            int node_numb_9;
            int node_numb_10;
            int node_numb_11;
            int node_numb_12;
            int node_numb_13;
            int node_numb_14;
            int node_numb_15;
            int node_numb_16;
            int node_numb_17;
            int node_numb_18;
            int node_numb_19;
            int node_numb_20;
            int node_numb_21;
            int node_numb_22;
            int node_numb_23;
            int node_numb_24;
            int node_numb_25;
            int node_numb_26;
            int node_numb_27;
            int MaterialNumber;
            int AccelerationFieldNumber;


            inputelements >> ElementNumber >> node_numb_1 >> node_numb_2 >> node_numb_3 >>
                          node_numb_4 >> node_numb_5 >> node_numb_6 >> node_numb_7 >> node_numb_8 >>
                          node_numb_9 >> node_numb_10 >> node_numb_11 >> node_numb_12 >> node_numb_13 >>
                          node_numb_14 >> node_numb_15 >> node_numb_16 >> node_numb_17 >> node_numb_18 >>
                          node_numb_19 >> node_numb_20 >> node_numb_21 >> node_numb_22 >> node_numb_23 >>
                          node_numb_24 >> node_numb_25 >> node_numb_26 >> node_numb_27 >>
                          MaterialNumber >> AccelerationFieldNumber;

            add_element_brick_27node(ElementNumber,
                                     node_numb_1,
                                     node_numb_2,
                                     node_numb_3,
                                     node_numb_4,
                                     node_numb_5,
                                     node_numb_6,
                                     node_numb_7,
                                     node_numb_8,
                                     node_numb_9,
                                     node_numb_10,
                                     node_numb_11,
                                     node_numb_12,
                                     node_numb_13,
                                     node_numb_14,
                                     node_numb_15,
                                     node_numb_16,
                                     node_numb_17,
                                     node_numb_18,
                                     node_numb_19,
                                     node_numb_20,
                                     node_numb_21,
                                     node_numb_22,
                                     node_numb_23,
                                     node_numb_24,
                                     node_numb_25,
                                     node_numb_26,
                                     node_numb_27,
                                     MaterialNumber,
                                     AccelerationFieldNumber);

        }

        else if (typeofelement.compare("DisplacementBeam") == 0)
        {
            int ElementNumber;
            int iNode;
            int jNode;
            int numberofintegrationpoints;
            int SectionNumber;
            double rho;
            string integrationrule;
            double vecxzPlane_X;
            double vecxzPlane_Y;
            double vecxzPlane_Z;
            double jntOffsetI_X;
            double jntOffsetI_Y;
            double jntOffsetI_Z;
            double jntOffsetJ_X;
            double jntOffsetJ_Y;
            double jntOffsetJ_Z;

            inputelements >> ElementNumber >> iNode >> jNode >>
                          numberofintegrationpoints >> SectionNumber >> rho >> integrationrule >>
                          vecxzPlane_X >> vecxzPlane_Y >> vecxzPlane_Z >>
                          jntOffsetI_X >> jntOffsetI_Y >> jntOffsetI_Z >>
                          jntOffsetJ_X >> jntOffsetJ_Y >> jntOffsetJ_Z;


            const char* integrationrulechar = integrationrule.c_str(); // get const char * representation

            add_element_beam_displacement_based_3d(ElementNumber,
                                                   iNode,
                                                   jNode,
                                                   numberofintegrationpoints,
                                                   SectionNumber,
                                                   rho,
                                                   integrationrulechar,
                                                   vecxzPlane_X, vecxzPlane_Y, vecxzPlane_Z,
                                                   jntOffsetI_X, jntOffsetI_Y, jntOffsetI_Z,
                                                   jntOffsetJ_X, jntOffsetJ_Y, jntOffsetJ_Z);
        }

        else if (typeofelement.compare("ElasticBeamLumpedMass") == 0)
        {
            int ElementNumber;
            double A;
            double E;
            double G;
            double Jx;
            double Iy;
            double Iz;
            int iNode;
            int jNode;
            double rho;
            double vecxzPlane_X;
            double vecxzPlane_Y;
            double vecxzPlane_Z;
            double jntOffsetI_X;
            double jntOffsetI_Y;
            double jntOffsetI_Z;
            double jntOffsetJ_X;
            double jntOffsetJ_Y;
            double jntOffsetJ_Z;

            inputelements >> ElementNumber >> A >> E >> G >> Jx >> Iy >> Iz >>
                          iNode >> jNode >> rho >>
                          vecxzPlane_X >> vecxzPlane_Y >> vecxzPlane_Z >>
                          jntOffsetI_X >> jntOffsetI_Y >> jntOffsetI_Z >>
                          jntOffsetJ_X >> jntOffsetJ_Y >> jntOffsetJ_Z;


            add_element_beam_elastic_lumped_mass(ElementNumber,
                                                 A,
                                                 E,
                                                 G,
                                                 Jx,
                                                 Iy,
                                                 Iz,
                                                 iNode,
                                                 jNode,
                                                 rho,
                                                 vecxzPlane_X, vecxzPlane_Y, vecxzPlane_Z,
                                                 jntOffsetI_X, jntOffsetI_Y, jntOffsetI_Z,
                                                 jntOffsetJ_X, jntOffsetJ_Y, jntOffsetJ_Z);

        }

        else if (typeofelement.compare("Contact3DOF3DOF") == 0)
        {
            int ElementNumber;
            int iNode;
            int jNode;
            double Knormal;
            double Ktangent;
            double frictionRatio;
            double x_local_1;
            double x_local_2;
            double x_local_3;

            inputelements >> ElementNumber >> iNode >> jNode >> Knormal >>
                          Ktangent >> frictionRatio >> x_local_1 >> x_local_2 >> x_local_3;

            add_element_contact_3dof_to_3dof(ElementNumber,
                                             iNode,
                                             jNode,
                                             Knormal,
                                             Ktangent,
                                             frictionRatio,
                                             x_local_1,
                                             x_local_2,
                                             x_local_3);
        }

        else if (typeofelement.compare("Contact3DOF6DOF") == 0)
        {
            int ElementNumber;
            int iNode;
            int jNode;
            double Knormal;
            double Ktangent;
            double frictionRatio;
            double cohession;
            double x_local_1;
            double x_local_2;
            double x_local_3;

            inputelements >> ElementNumber >> iNode >> jNode >> Knormal >>
                          Ktangent >> frictionRatio >> cohession >> x_local_1 >> x_local_2 >> x_local_3;

            add_element_contact_3dof_to_6dof(ElementNumber,
                                             iNode,
                                             jNode,
                                             Knormal,
                                             Ktangent,
                                             frictionRatio,
                                             cohession,
                                             x_local_1,
                                             x_local_2,
                                             x_local_3);
        }

        else if (typeofelement.compare("Contact3DOF6DOF") == 0)
        {
            int ElementNumber;
            int iNode;
            int jNode;
            double Knormal;
            double Ktangent;
            double frictionRatio;
            double cohession;
            double x_local_1;
            double x_local_2;
            double x_local_3;

            inputelements >> ElementNumber >> iNode >> jNode >> Knormal >>
                          Ktangent >> frictionRatio >> cohession >> x_local_1 >> x_local_2 >> x_local_3;

            add_element_contact_6dof_to_6dof(ElementNumber,
                                             iNode,
                                             jNode,
                                             Knormal,
                                             Ktangent,
                                             frictionRatio,
                                             cohession,
                                             x_local_1,
                                             x_local_2,
                                             x_local_3);
        }

    }

    inputelements.close();

    return 0;

};







