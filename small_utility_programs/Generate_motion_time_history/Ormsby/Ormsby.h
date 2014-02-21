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




#ifndef Ormsby_h
#define Ormsby_h


#include <TimeSeries.h>

class Ormsby : public TimeSeries
{
    public:
        // constructors
        Ormsby(double amplitude, double TimeMaximum,
               double startTime, double finishTime,
               double Frequency1, double Frequency2,
               double Frequency3, double Frequency4);

        Ormsby();

        // destructor
        ~Ormsby();

        // method to get load factor
        double getFactor(double pseudoTime);
        double getDuration ()
        {
            return tFinish - tStart;
        }
        double getPeakFactor ()
        {
            return amplitude_max;
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
        double f1;
        double f2;
        double f3;
        double f4;
        double amplitude_max;

};

#endif
