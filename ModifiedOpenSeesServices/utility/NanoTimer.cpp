


// File: ~/utility/NanoTimer.C
//
// Written: Babak Kamrani
// Created: September 2012
// Revision: A




//
// Description: This file contains the class definition for NanoTimer.
// NanoTimer is a high resolution stopwatch . It can measure the time by the precision of Nano Secounds.
//


#include "NanoTimer.h"





NanoTimer::NanoTimer()

{

}

NanoTimer::~NanoTimer()
{

}

void
NanoTimer::start(void)
{
    t_start = Clock::now();
}

void
NanoTimer::pause(void)

{
    t_end = Clock::now();
}


double
NanoTimer::getReal(void)


{
    elapsedtime = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();
    return (double)elapsedtime;
}




