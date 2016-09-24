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
// DSL PROGRAMMER:    Jose Abell
// DESIGNER:          Jose Abell
// DATE:
// UPDATE HISTORY:    September 2016
//
//
///////////////////////////////////////////////////////////////////////////////



int simulate_constitutive_testing_DirectStrain_path(int MaterialNumber, double scale_factor, std::string filein, double sxx0, double syy0, double szz0, double sxy0, double sxz0, double syz0, int verbose_output)
{


    NDMaterialLT* material = theDomain.getNDMaterialLT(MaterialNumber);
    if (material == NULL)
    {
        cerr << "Error: (simulate_constitutive_testing_BardetLT_path) material allocation problem!" << endl;
        return -1;
    }

    DTensor2 s0(3, 3, 0);
    s0(0, 0) = sxx0;
    s0(1, 1) = syy0;
    s0(2, 2) = szz0;
    s0(0, 1) = sxy0;
    s0(1, 0) = sxz0;
    s0(0, 2) = syz0;
    s0(2, 0) = sxy0;
    s0(1, 2) = sxz0;
    s0(2, 1) = syz0;

    material->setStressTensor(s0);

    DirectStrainConstitutiveDriver driver(material);


    ofstream outStress("Stress.feioutput");
    ofstream outStrain("Strain.feioutput");
    ofstream outMaterial("Material_Output.feioutput");
    ifstream infile(filein.c_str());


    int step = 0;
    DTensor2 strain_increment(3, 3, 0);
    double exx;
    double eyy;
    double ezz;
    double exy;
    double exz;
    double eyz;
    while (infile >> exx
            >> eyy
            >> ezz
            >> exy
            >> exz
            >> eyz )
    {
        cout << "Step = " << step << " eps = [" << exx << ", " << eyy << ", " << ezz << ", " << exy << ", " << exz << ", " << eyz << "]" << endl;
        strain_increment(0, 0) = exx;
        strain_increment(1, 1) = eyy;
        strain_increment(2, 2) = ezz;
        strain_increment(0, 1) = exy;
        strain_increment(0, 2) = exz;
        strain_increment(1, 2) = eyz;
        strain_increment(1, 0) = exy;
        strain_increment(2, 0) = exz;
        strain_increment(2, 1) = eyz;

        const DTensor2 & stress = material->getStressTensor();
        const DTensor2 & strain = material->getStrainTensor();
        outStress << stress(0, 0)  << " " << stress(1, 1)  << " " << stress(2, 2)  << " " << stress(0, 1)  << " " << stress(0, 2)  << " " << stress(1, 2) << endl;
        outStrain << strain(0, 0)  << " " << strain(1, 1)  << " " << strain(2, 2)  << " " << strain(0, 1)  << " " << strain(0, 2)  << " " << strain(1, 2) << endl;
        if (verbose_output > 0 && (step % verbose_output) == 0)
        {
            outMaterial << "\nStep = " << step << endl;
            outMaterial << "-----------------------------" << endl;
            material->Print(outMaterial);
        }
        driver.applyIncrement( strain_increment);
        step++;
    }
    outStress.close();
    outStrain.close();
    infile.close();

    return 0;
};


















