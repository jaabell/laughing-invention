///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               October 2009 - Aug 2015
// UPDATE HISTORY:     See git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////

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

    if ( theDomain.addRecorder(*theelementRecorder) != 0 )
    {
        cerr << "Error: (define_output_of_element_to_file) Recorder could not be added to the domain " << endl;
        abort();
    }
};















