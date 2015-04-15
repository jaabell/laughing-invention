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
// PURPOSE:           Provides a consistent interface to time different ESSI
//                    components
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Jose Abell
//
// DATE:              Nov. 22 2014
// UPDATE HISTORY:
//
//
///////////////////////////////////////////////////////////////////////////////


#ifndef ESSITIMER_H
#define ESSITIMER_H

#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <map>

typedef std::chrono::high_resolution_clock ESSIClock;
typedef std::chrono::duration<double, std::ratio<1> > ESSIDuration;
typedef std::map<std::string, ESSIDuration> ESSITimerList;
typedef std::chrono::time_point<ESSIClock> ESSITimePoint;
typedef std::map<std::string, ESSITimePoint> ESSITimePointList;

class ESSITimer
{
public:
    ESSITimer();
    ESSITimer(std::string reportfilename_, std::string header = "");
    void setReportFileName(std::string reportfilename_, std::string header = "");
    void start(std::string timername);
    void stop(std::string timername);
    void report();
    void print();
    void resetAll();
    void reset(std::string timername);
    void clearAll();
    void remove(std::string timername);


private:
    std::string reportfilename;
    std::string header;

    ESSITimePointList timepoints;
    ESSITimerList timers;

    bool activated;
};

extern ESSITimer globalESSITimer;

#endif