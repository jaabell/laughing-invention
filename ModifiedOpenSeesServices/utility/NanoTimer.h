


// File: ~/utility/NanoTimer.C
//
// Written: Babak Kamrani
// Created: September 2012
// Revision: A




//
// Description: This file contains the class definition for NanoTimer.
// NanoTimer is a high resolution stopwatch . It can measure the time by the precision of Nano Secounds.
//

#ifndef NanoTimer_h
#define NanoTimer_h
#endif


#include <iostream>
#include <chrono>



class NanoTimer
{
    public:
        NanoTimer();
        virtual ~NanoTimer();

        void start(void);
        void pause(void);
        double getReal(void) ;

        typedef std::chrono::high_resolution_clock Clock;
        std::chrono::system_clock::time_point t_start;
        std::chrono::system_clock::time_point t_end;
        long int elapsedtime;



    protected:

    private:

};
