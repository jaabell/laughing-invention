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
//! - BodyForceNumber: body force tag number to be used in elements
//! - accelerationfield_x, accelerationfield_y, accelerationfield_z: acceleration field in x, y, and z direction respectively




int add_acceleration_field(int GravityFieldNumber,
                           double accelerationfield_x,
                           double accelerationfield_y,
                           double accelerationfield_z)
{
    AccelerationField* theaccelerationfield = 0;
    theaccelerationfield = new AccelerationField(GravityFieldNumber, accelerationfield_x, accelerationfield_y, accelerationfield_z);

    if (theaccelerationfield == NULL)
    {
        cerr << "Error: (add_acceleration_field) memory allocating for theaccelerationfield!" << endl;
        return -1;
    }


    if ( theDomain.addAccelerationField(theaccelerationfield) != 0 )
    {
        cerr << "Error: (add_acceleration_field) theaccelerationfield could not be added to the domain " << endl;
        return -1;
    }

    //     free(theaccelerationfield);
    return 0;
};





