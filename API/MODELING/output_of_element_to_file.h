
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
// DATE:              May 2009
// UPDATE HISTORY:    August 2009
//
///////////////////////////////////////////////////////////////////////////////



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Recording an Element Response</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of recording an element response.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - ElementNumber: integer tag of an element to be recorded
//! - filename: filename to be used for recorder
//! - dataToStore: type of response to be recorded (which is different depends on type of the element)
//! - echoTime: this argument will place the pseudo time of the as the first entry in the line (true/false)
//! - dT: time interval of recorded response
//!
//! - dataToStore for different elements:
//! - EightNodeBrick: stress / strain / gausspoint
//! - EightNodeBrick_upU: stress / strain / gausspoint / pq
//! - EightNodeBrick_up: stress / strain / gausspoint
//! - TwentyNodeBrick: force / stiffness / plastified / PileM / stress / pq / gausspoint
//! - TwentyNodeBrick_upU: stress / pq / gausspoint
//! - TwentySevenNodeBrick: force / stiffness / plastified / PileM / stress / pq / gausspoint





//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



void output_of_element_to_file(int ElementNumber,
                               const char* filename,
                               const char* dataToStore,
                               bool echoTime,
                               double dT)
{
    echoMode theEMode = NONE;
    openMode theOMode = OVERWRITE;

    int numEle = 0;
    int NB = 1 ;
    ID eleIDs(numEle, numEle + 1);
    eleIDs[numEle++] = ElementNumber;

    const char* fileName = filename;

    DataOutputHandler* theOutputHandler = 0;
    theOutputHandler = new DataOutputFileHandler(fileName, theEMode, theOMode);

    if (theOutputHandler == NULL)
    {
        cerr << "Error: (define_output_of_element_to_file) memory allocation problem for theOutputHandler!" << endl;
        abort();
    }


    Recorder* theelementRecorder = 0;
    theelementRecorder = new ElementRecorder(eleIDs, &dataToStore, NB, echoTime, theDomain, *theOutputHandler, dT);

    if (theelementRecorder == NULL)
    {
        cerr << "Error: (define_output_of_element_to_file) memory allocation problem for theelementRecorder!" << endl;
        abort();
    }

    if( theDomain.addRecorder(*theelementRecorder) != 0 )
    {
        cerr << "Error: (define_output_of_element_to_file) Recorder could not be added to the domain " << endl;
        abort();
    }
};















