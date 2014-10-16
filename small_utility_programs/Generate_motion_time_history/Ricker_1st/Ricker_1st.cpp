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
// PURPOSE:           Ricker_1st Wavelet
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Nima Tafazzoli, Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              March 2011
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////




#include <Ricker_1st.h>
#include <Vector.h>
#include <Channel.h>
#include <classTags.h>

#include <math.h>

Ricker_1st::Ricker_1st(double amplitude, double TimeMaximum,
                       double startTime, double finishTime,
                       double Frequency)
    : TimeSeries(TSERIES_TAG_Ricker_1st),
      tStart(startTime), tFinish(finishTime), Timemax(TimeMaximum),
      f(Frequency), cFactor(amplitude)
{

}

Ricker_1st::Ricker_1st()
    : TimeSeries(TSERIES_TAG_Ricker_1st),
      tStart(0.0), tFinish(0.0), Timemax(0.0),
      f(0.0), cFactor(1.0)
{
    // does nothing
}


Ricker_1st::~Ricker_1st()
{
    // does nothing
}

double
Ricker_1st::getFactor(double pseudoTime)
{
    static double Pi = 2 * asin(1.0);


    if (pseudoTime >= tStart && pseudoTime <= tFinish)
    {
        return (cFactor * (Pi * f * (pseudoTime - tStart - Timemax) * exp(-Pi * Pi * f * f * (pseudoTime - tStart - Timemax) * (pseudoTime - tStart - Timemax))));
    }

    else
    {
        return 0.0;
    }
}

int
Ricker_1st::sendSelf(int commitTag, Channel& theChannel)
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
        std::cerr << "Ricker_1st::sendSelf() - channel failed to send data\n";
        return result;
    }

    return 0;
}


int
Ricker_1st::receiveSelf(int commitTag, Channel& theChannel,
                     FEM_ObjectBroker& theBroker)
{
    int dbTag = this->getDbTag();
    Vector data(5);
    int result = theChannel.receiveVector(dbTag, commitTag, data);

    if (result < 0)
    {
        std::cerr << "Ricker_1st::sendSelf() - channel failed to receive data\n";
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
Ricker_1st::Print(OPS_Stream& s, int flag)
{
    s << "Ricker_1st Wavelet" << endln;
    s << "Amplitude: " << cFactor << endln;
    s << "tStart: " << tStart << endln;
    s << "tFinish: " << tFinish << endln;
    s << "Time of Maximum: " << Timemax << endln;
    s << "Frequency: " << f << endln;


}





