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
// PURPOSE:           API
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Jose Abell, Boris Jeremic
//
// DATE:              Feb 2014
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



/*
int write_domain_state_for_this_step_static()
{
    string nodal_displacements_filename = ModelName + "_" + StageName + "_Nodes_Generalized_Displacements.feioutput";
    string element_stresses_filename    = ModelName + "_" + StageName + "_Element_Stresses.feioutput";
    string element_strains_filename     = ModelName + "_" + StageName + "_Element_Strains.feioutput";
    string element_end_forces_filename  = ModelName + "_" + StageName + "_Element_End_Forces.feioutput";

    nodal_displacements_file (nodal_displacements_filename, ios::out | ios::app);// | ios::binary);
    element_stresses_file    (nodal_displacements_filename, ios::out | ios::app);// | ios::binary);
    element_strains_file     (nodal_displacements_filename, ios::out | ios::app);// | ios::binary);
    element_end_forces_file  (nodal_displacements_filename, ios::out | ios::app);// | ios::binary);
    if (not nodal_displacements_file.is_open())
    {
        cerr << "write_domain_state_for_this_step :: ERROR Opening file. " << nodal_displacements_filename << endl;
    }
    if (not nodal_displacements_file.is_open())
    {
        cerr << "write_domain_state_for_this_step :: ERROR Opening file. " << nodal_displacements_filename << endl;
    }
    if (not nodal_displacements_file.is_open())
    {
        cerr << "write_domain_state_for_this_step :: ERROR Opening file. " << nodal_displacements_filename << endl;
    }
    if (not nodal_displacements_file.is_open())
    {
        cerr << "write_domain_state_for_this_step :: ERROR Opening file. " << nodal_displacements_filename << endl;
    }

    int Nelems         = theDomain.getNumElements();
    int Nnodes         = theDomain.getNumNodes();
    double currentTime = theDomain.getCurrentTime();



    //cout << "Time = " << currentTime << " Nelems = " << Nelems << " Nnodes = " << Nnodes << endl;
    NodeIter &theNodes = theDomain.getNodes();
    Node *node;
    while ((node = theNodes()) != 0 )
    {
        int num_dof = node->getNumberDOF();
        int tag = node->getTag();
        const Vector &disp = node->getDisp();
        const Vector &vel = node->getVel();
        const Vector &accel =  node->getAccel();
    }

    nodal_displacements_file.close();
    element_stresses_file.close();
    element_strains_file.close();
    element_end_forces_file.close();


    return 0;

};


*/






