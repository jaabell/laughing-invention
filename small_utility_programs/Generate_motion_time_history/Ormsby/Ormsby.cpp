///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The RegenTimemax of the University
//  of California, for an indefinite period, and anybody caught using it without our
//  permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//  Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//  all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Damping.h
// CLASS:             Damping
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Ormsby Wavelet
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Nima Tafazzoli, Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              Nov. 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


// Reference:
// Harold Ryan. Ricker, Ormsby, Klander, Butterworth - a choice of wavelets. Canadian Society of Exploration
// Geophysicists Recorder, 19(7):8–9, September 1994


#include <Ormsby.h>
#include <Vector.h>
#include <Channel.h>
#include <classTags.h>

#include <math.h>

#define tolerance 1e-10


Ormsby::Ormsby(double amplitude, double TimeMaximum,
               double startTime, double finishTime,
               double Frequency1, double Frequency2,
               double Frequency3, double Frequency4)
    : TimeSeries(TSERIES_TAG_Ormsby),
      tStart(startTime), tFinish(finishTime), Timemax(TimeMaximum),
      f1(Frequency1), f2(Frequency2),
      f3(Frequency3), f4(Frequency4), amplitude_max(amplitude)
{

}

Ormsby::Ormsby()
    : TimeSeries(TSERIES_TAG_Ormsby),
      tStart(0.0), tFinish(0.0), Timemax(0.0),
      f1(0.0), f2(0.0),
      f3(0.0), f4(0.0), amplitude_max(1.0)
{
    // does nothing
}


Ormsby::~Ormsby()
{
    // does nothing
}

double
Ormsby::getFactor(double pseudoTime)
{
    static double Pi = 2 * asin(1.0);
    double a1 = f4 - f3;
    double a2 = f2 - f1;
    double b4 = Pi * f4;
    double b3 = Pi * f3;
    double b2 = Pi * f2;
    double b1 = Pi * f1;

    double cFactor = Pi * (f4 + f3 - f2 - f1);



    if (pseudoTime >= tStart && pseudoTime <= tFinish)
    {
        if (fabs(pseudoTime - tStart - Timemax) > tolerance)
        {
            return (amplitude_max / cFactor) * ( b4 * f4 * sin(b4 * (pseudoTime - tStart - Timemax)) * sin(b4 * (pseudoTime - tStart - Timemax)) / (a1 * b4 * (pseudoTime - tStart - Timemax) * b4 * (pseudoTime - tStart - Timemax)) -
                                                 b3 * f3 * sin(b3 * (pseudoTime - tStart - Timemax)) * sin(b3 * (pseudoTime - tStart - Timemax)) / (a1 * b3 * (pseudoTime - tStart - Timemax) * b3 * (pseudoTime - tStart - Timemax)) -
                                                 b2 * f2 * sin(b2 * (pseudoTime - tStart - Timemax)) * sin(b2 * (pseudoTime - tStart - Timemax)) / (a2 * b2 * (pseudoTime - tStart - Timemax) * b2 * (pseudoTime - tStart - Timemax)) +
                                                 b1 * f1 * sin(b1 * (pseudoTime - tStart - Timemax)) * sin(b1 * (pseudoTime - tStart - Timemax)) / (a2 * b1 * (pseudoTime - tStart - Timemax) * b1 * (pseudoTime - tStart - Timemax)) );
        }
        else
        {
            return (amplitude_max / cFactor) * Pi * (f4 + f3 - f2 - f1);
        }
    }
    else
    {
        return 0.0;
    }
}


int
Ormsby::sendSelf(int commitTag, Channel& theChannel)
{
    int dbTag = this->getDbTag();
    Vector data(8);
    data(0) = amplitude_max;
    data(1) = tStart;
    data(2) = tFinish;
    data(3) = Timemax;
    data(4) = f1;
    data(5) = f2;
    data(6) = f3;
    data(7) = f4;
    int result = theChannel.sendVector(dbTag, commitTag, data);

    if (result < 0)
    {
        std::cerr << "Ormsby::sendSelf() - channel failed to send data\n";
        return result;
    }

    return 0;
}


int
Ormsby::receiveSelf(int commitTag, Channel& theChannel,
                 FEM_ObjectBroker& theBroker)
{
    int dbTag = this->getDbTag();
    Vector data(8);
    int result = theChannel.receiveVector(dbTag, commitTag, data);

    if (result < 0)
    {
        std::cerr << "Ormsby::sendSelf() - channel failed to receive data\n";
        amplitude_max = 1.0;
        tStart = 0.0;
        tFinish = 0.0;
        Timemax = 0.0;
        f1 = 0.0;
        f2 = 0.0;
        f3 = 0.0;
        f4 = 0.0;
        return result;
    }

    amplitude_max = data(0);
    tStart = data(1);
    tFinish = data(2);
    Timemax = data(3);
    f3 = data(4);
    f3 = data(5);
    f3 = data(6);
    f4 = data(7);

    return 0;
}


void
Ormsby::Print(OPS_Stream& s, int flag)
{
    s << "Ormsby Wavelet" << endln;
    s << "\tFactor: " << amplitude_max << endln;
    s << "\ttStart: " << tStart << endln;
    s << "\ttFinish: " << tFinish << endln;
    s << "\tTimemax: " << Timemax << endln;
    s << "\tFrequency1: " << f3 << endln;
    s << "\tFrequency2: " << f3 << endln;
    s << "\tFrequency3: " << f3 << endln;
    s << "\tFrequency4: " << f4 << endln;

}





