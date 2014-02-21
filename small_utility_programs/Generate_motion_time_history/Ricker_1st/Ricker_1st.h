///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The Regents of the University
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




#ifndef Ricker_1st_h
#define Ricker_1st_h


#include <TimeSeries.h>

class Ricker_1st : public TimeSeries
{
    public:
        // constructors
        Ricker_1st(double amplitude, double TimeMaximum,
                   double startTime, double finishTime,
                   double Frequency);

        Ricker_1st();

        // destructor
        ~Ricker_1st();

        // method to get load factor
        double getFactor(double pseudoTime);
        double getDuration ()
        {
            return tFinish - tStart;
        }
        double getPeakFactor ()
        {
            return cFactor;
        }
        double getTimeIncr (double pseudoTime)
        {
            return tFinish - tStart;
        }

        // methods for output
        int sendSelf(int commitTag, Channel& theChannel);
        int recvSelf(int commitTag, Channel& theChannel,
                     FEM_ObjectBroker& theBroker);

        void Print(OPS_Stream& s, int flag = 0);

    protected:

    private:
        double tStart;
        double tFinish;
        double Timemax;
        double f;
        double cFactor;

};

#endif
