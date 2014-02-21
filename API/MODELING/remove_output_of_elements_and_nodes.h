
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
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Jose Abell, Boris Jeremic
//
// DATE:              June 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



void remove_output_of_elements_and_nodes()
{

    theDomain.removeRecorder(RecorderNumber);
    theDomain.removeRecorder(RecorderNumber + 1);
    theDomain.removeRecorder(RecorderNumber + 2);
    theDomain.removeRecorder(RecorderNumber + 3);

    RecorderNumber++;
    RecorderNumber++;
    RecorderNumber++;
    RecorderNumber++;



    //    cout << "Recorder Number is : " << RecorderNumber << endl;




    //  DataOutputHandler *theOutputHandler_node_displacement = 0;
    //  DataOutputHandler *theOutputHandler_element_force = 0;
    //  DataOutputHandler *theOutputHandler_element_stress = 0;


    //    if(theOutputHandler_node_displacement != 0)
    //      delete theOutputHandler_node_displacement;
    //    theOutputHandler_node_displacement = 0;
    //
    //
    //    if(theOutputHandler_element_stress != 0)
    //      delete theOutputHandler_element_stress;
    //    theOutputHandler_element_stress = 0;


    // FIXME: Nima Tafazzoli (April 2013)
    // This pointer needs to be deleted since it is created up there.
    // But, if the following is used and the simulation has more than
    // one stage of analysis, the segmentation fault will happen!

    //    if(theOutputHandler_element_force != 0)
    //      delete theOutputHandler_element_force;
    //    theOutputHandler_element_force = 0;


};















