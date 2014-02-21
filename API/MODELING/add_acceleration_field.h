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
// DATE:
// UPDATE HISTORY:    October 2010
//
//
///////////////////////////////////////////////////////////////////////////////



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


    if( theDomain.addAccelerationField(theaccelerationfield) != 0 )
    {
        cerr << "Error: (add_acceleration_field) theaccelerationfield could not be added to the domain " << endl;
        return -1;
    }

    //     free(theaccelerationfield);
    return 0;
};





