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
// PURPOSE:           Ricker_2nd Wavelet
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
// Harold Ryan. Ricker, Ricker_2nd, Klander, Butterworth - a choice of wavelets. Canadian Society of Exploration
// Geophysicists Recorder, 19(7):8–9, September 1994


#include <Ricker_2nd.h>
#include <Vector.h>
#include <Channel.h>
#include <classTags.h>

#include <math.h>

Ricker_2nd::Ricker_2nd(double amplitude, double TimeMaximum,
                       double startTime, double finishTime,
                       double Frequency)
    : TimeSeries(TSERIES_TAG_Ricker_2nd),
      tStart(startTime), tFinish(finishTime), Timemax(TimeMaximum),
      f(Frequency), cFactor(amplitude)
{

}

Ricker_2nd::Ricker_2nd()
    : TimeSeries(TSERIES_TAG_Ricker_2nd),
      tStart(0.0), tFinish(0.0), Timemax(0.0),
      f(0.0), cFactor(1.0)
{
    // does nothing
}


Ricker_2nd::~Ricker_2nd()
{
    // does nothing
}

double
Ricker_2nd::getFactor(double pseudoTime)
{
    static double Pi = 2 * asin(1.0);
    double a1 = Pi * Pi * f * f;

    if (pseudoTime >= tStart && pseudoTime <= tFinish)
    {
        return (cFactor * (2 * a1 * (pseudoTime - tStart - Timemax) * (pseudoTime - tStart - Timemax) - 1) * exp(-a1 * (pseudoTime - tStart - Timemax) * (pseudoTime - tStart - Timemax)));
    }

    else
    {
        return 0.0;
    }
}

int
Ricker_2nd::sendSelf(int commitTag, Channel& theChannel)
{
    int dbTag = this->getDbTag();
    Vector data(5);
    data(0) = cFactor;
    data(1) = tStart;
    data(2) = tFinish;
    data(3) = Timemax;
    data(4) = f;
    int result = theChannel.sendVector(dbTag, commitTag, data);

    if (result < 0)
    {
        std::cerr << "Ricker_2nd::sendSelf() - channel failed to send data\n";
        return result;
    }

    return 0;
}


int
Ricker_2nd::receiveSelf(int commitTag, Channel& theChannel,
                     FEM_ObjectBroker& theBroker)
{
    int dbTag = this->getDbTag();
    Vector data(5);
    int result = theChannel.receiveVector(dbTag, commitTag, data);

    if (result < 0)
    {
        std::cerr << "Ricker_2nd::sendSelf() - channel failed to receive data\n";
        cFactor = 1.0;
        tStart = 0.0;
        tFinish = 0.0;
        Timemax = 0.0;
        f = 0.0;
        return result;
    }

    cFactor = data(0);
    tStart  = data(1);
    tFinish = data(2);
    Timemax = data(3);
    f = data(4);

    return 0;
}


void
Ricker_2nd::Print(OPS_Stream& s, int flag)
{
    s << "Ricker_2nd Wavelet" << endln;
    s << "\tFactor: " << cFactor << endln;
    s << "\ttStart: " << tStart << endln;
    s << "\ttFinish: " << tFinish << endln;
    s << "\tTimemax: " << Timemax << endln;
    s << "\tFrequency: " << f << endln;


}





