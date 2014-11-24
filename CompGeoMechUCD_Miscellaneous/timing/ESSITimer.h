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



#include <iostream>
#include <string>
#include <chrono>
#include <fstream>


class ESSITimer
{
    public:
        ESSITimer(std::string name);
        void start(std::string filename, int linestart);
        void stop(int linestop);
        double report();


    private:
        std::string name;
        std::string filename;
        int linestart;
        int linestop;
        typedef std::chrono::high_resolution_clock clock_;
        typedef std::chrono::duration<double, std::ratio<1> > second_;
        std::chrono::time_point<clock_> beginning;
        std::chrono::time_point<clock_> end;
};