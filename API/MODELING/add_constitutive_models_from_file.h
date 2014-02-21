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
//! - inputmaterialsfile: name of the input file which has the elements information

//! Format of input file:
//! - The first argument should be the type of the elements which could be
//! one of the followings: UniaxialMaterial / Elastic / Steel01 /
//! Steel02 / Concrete02 / nDMaterial
//! In case the type of the material is nDMaterial, the second argument should be
//! the type of the nDMaterial to be used:
//! vonMisesPP / vonMisesIH / vonMisesKH
//! DruckerPragerPP / DruckerPragerIH / DruckerPragerKH
//! CamClay / DafaliasManzari / SANISAND
//! ElasticIsotropic3D / CrossAnisotropic

//! - The rest of the arguments should be the same as inputs corresponding to each type of element


int add_constitutive_models_from_file(string inputmaterialsfile)
{

    const char* inputfromfile = inputmaterialsfile.c_str();

    ifstream inputmaterials;

    if(! inputmaterials)
    {
        cerr << "Error: (add_constitutive_models_from_file) Can not open " << inputmaterialsfile << "!" << endl;
        return -1;
    }

    inputmaterials.open(inputfromfile , ios::in);

    string typeofmaterial;

    while (!inputmaterials.eof())
    {

        inputmaterials >> typeofmaterial;


        if (typeofmaterial.compare("UniaxialMaterial") == 0)
        {
            string materialname;

            inputmaterials >> materialname;

            if (materialname.compare("Elastic") == 0)
            {

                int MaterialNumber;
                double elasticmodulus;
                double eta;

                inputmaterials >> MaterialNumber >> elasticmodulus >> eta;

                add_constitutive_model_uniaxial_elastic(MaterialNumber, elasticmodulus, eta);
            }

            else if (materialname.compare("Steel01") == 0)
            {

                int MaterialNumber;
                double fy;
                double Ep;
                double Hd;
                double a1;
                double a2;
                double a3;
                double a4;

                inputmaterials >> MaterialNumber >> fy >> Ep >> Hd >> a1 >> a2 >> a3 >> a4;

                add_constitutive_model_uniaxial_steel01(MaterialNumber, fy, Ep, Hd, a1, a2, a3, a4);
            }

            else if (materialname.compare("Steel02") == 0)
            {

                int MaterialNumber;
                double fy;
                double E0;
                double b;
                double R0;
                double cR1;
                double cR2;
                double a1;
                double a2;
                double a3;
                double a4;

                inputmaterials >> MaterialNumber >> fy >> E0 >> b >> R0 >> cR1 >> cR2 >> a1 >> a2 >> a3 >> a4;

                add_constitutive_model_uniaxial_steel02(MaterialNumber, fy, E0, b, R0, cR1, cR2, a1, a2, a3, a4);
            }

            else if (materialname.compare("Concrete02") == 0)
            {

                int MaterialNumber;
                double fpc;
                double epsc0;
                double fpcu;
                double epscu;
                double rat;
                double ft;
                double Ets;

                inputmaterials >> MaterialNumber >> fpc >> epsc0 >> fpcu >> epscu >> rat >> ft >> Ets;

                add_constitutive_model_uniaxial_concrete02( MaterialNumber, fpc, epsc0, fpcu, epscu, rat, ft, Ets);
            }
        }


        else if (typeofmaterial.compare("nDMaterial") == 0)
        {

            string materialname;

            inputmaterials >> materialname;

            if (materialname.compare("CamClay") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double e0;
                double M;
                double lambda;
                double kappa;
                double v;
                double Kc;
                double initialconfiningstress;


                inputmaterials >> MaterialNumber >> Algorithm >> rho >> e0 >> M >>
                               lambda >> kappa >> v >> Kc >> initialconfiningstress;

                add_constitutive_model_NDMaterial_camclay(MaterialNumber, Algorithm, rho, e0, M, lambda, kappa, v, Kc, initialconfiningstress);
            }

            else if (materialname.compare("vonMisesPP") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double E;
                double v;
                double k;
                double initialconfiningstress;


                inputmaterials >> MaterialNumber >> Algorithm >> rho >> E >> v >> k >> initialconfiningstress;

                add_constitutive_model_NDMaterial_vonmises_perfectly_plastic(MaterialNumber, Algorithm, rho, E, v, k, initialconfiningstress);
            }

            else if (materialname.compare("vonMisesIH") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double E;
                double v;
                double k;
                double H;
                double initialconfiningstress;

                inputmaterials >> MaterialNumber >> Algorithm >> rho >> E >> v >> k >> H >> initialconfiningstress;

                add_constitutive_model_NDMaterial_vonmises_isotropic_hardening(MaterialNumber, Algorithm, rho, E, v, k, H, initialconfiningstress);
            }

            else if (materialname.compare("vonMisesKH") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double E;
                double v;
                double k;
                double ha;
                double Cr;
                double initialconfiningstress;

                inputmaterials >> MaterialNumber >> Algorithm >> rho >> E >> v >> k >> ha >> Cr >> initialconfiningstress;

                add_constitutive_model_NDMaterial_vonmises_kinematic_hardening(MaterialNumber, Algorithm, rho, E, v, k, ha, Cr, initialconfiningstress);
            }

            else if (materialname.compare("DruckerPragerPP") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double E;
                double v;
                double k;
                double initialconfiningstress;

                inputmaterials >> MaterialNumber >> Algorithm >> rho >> E >> v >> k >> initialconfiningstress;

                add_constitutive_model_NDMaterial_druckerprager_perfectly_plastic(MaterialNumber, Algorithm, rho, E, v, k, initialconfiningstress);
            }

            else if (materialname.compare("DruckerPragerIH") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double E;
                double v;
                double k;
                double H;
                double initialconfiningstress;

                inputmaterials >> MaterialNumber >> Algorithm >> rho >> E >> v >> k >> H >> initialconfiningstress;

                add_constitutive_model_NDMaterial_druckerprager_isotropic_hardening(MaterialNumber, Algorithm, rho, E, v, k, H, initialconfiningstress);
            }

            else if (materialname.compare("DruckerPragerKH") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double E;
                double v;
                double k;
                double ha;
                double Cr;
                double initialconfiningstress;

                inputmaterials >> MaterialNumber >> Algorithm >> rho >> E >> v >> k >> ha >> Cr >> initialconfiningstress;

                add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening(MaterialNumber, Algorithm, rho, E, v, k, ha, Cr, initialconfiningstress);
            }

            else if (materialname.compare("DafaliasManzari") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double e0;
                double G0;
                double nu;
                double Pat;
                double p_cut;
                double Mc;
                double c;
                double lambda_c;
                double xi;
                double ec_ref;
                double m;
                double h0;
                double ch;
                double nb;
                double A0;
                double nd;
                double z_max;
                double cz;
                double initialconfiningstress;

                inputmaterials >> MaterialNumber >> Algorithm >> rho >> e0 >> G0 >> nu >> Pat >> p_cut >> Mc >> c >>
                               lambda_c >> xi >> ec_ref >> m >> h0 >> ch >> nb >> A0 >> nd >> z_max >> cz >> initialconfiningstress;

                add_constitutive_model_NDMaterial_dafalias_manzari( MaterialNumber, Algorithm, rho, e0, G0, nu, Pat, p_cut,
                        Mc, c, lambda_c, xi, ec_ref, m, h0, ch, nb, A0, nd, z_max, cz, initialconfiningstress);
            }

            else if (materialname.compare("SANISAND") == 0)
            {
                int MaterialNumber;
                int Algorithm;
                double rho;
                double e0;
                double G0;
                double K0;
                double Pat;
                double k_c;
                double alpha_cc;
                double c;
                double lambda;
                double xi;
                double ec_ref;
                double m;
                double h0;
                double ch;
                double nb;
                double A0;
                double nd;
                double p_r;
                double rho_c;
                double theta_c;
                double X;
                double z_max;
                double cz;
                double p0;
                double p_in;

                inputmaterials >> MaterialNumber >> Algorithm >> rho >> e0 >> G0 >> K0 >> Pat >> k_c >> alpha_cc >> c >> lambda >> xi >> ec_ref >> m >> h0 >> ch >> nb >> A0 >> nd >> p_r >> rho_c >> theta_c >> X >> z_max >> cz >> p0 >> p_in;

                add_constitutive_model_NDMaterial_sanisand( MaterialNumber, Algorithm, rho, e0, G0, K0, Pat, k_c, alpha_cc, c, lambda, xi, ec_ref, m, h0, ch, nb, A0, nd, p_r, rho_c, theta_c, X, z_max, cz, p0, p_in);
            }

            else if (materialname.compare("ElasticIsotropic3D") == 0)
            {
                int MaterialNumber;
                double ElasticModulus;
                double nu;
                double rho;

                inputmaterials >> MaterialNumber >> ElasticModulus >> nu >> rho;

                add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d(MaterialNumber, ElasticModulus, nu, rho);
            }

            else if (materialname.compare("CrossAnisotropic") == 0)
            {
                int MaterialNumber;
                double Ehp;
                double Evp;
                double nuhvp;
                double nuhhp;
                double Ghvp;
                double rhop;

                inputmaterials >> MaterialNumber >> Ehp >> Evp >> nuhvp >> nuhhp >> Ghvp >> rhop;

                add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic(MaterialNumber, Ehp, Evp, nuhvp, nuhhp, Ghvp, rhop);
            }
        }
    }

    inputmaterials.close();

    return 0;

};





