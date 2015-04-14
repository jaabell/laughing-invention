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


// #include <ESSITimer.h>
#include "ESSITimer.h"



ESSITimer::ESSITimer(std::string reportfilename_):
	reportfilename(reportfilename_)
{
	//
}

void ESSITimer::start(std::string timername)
{
	// auto timer = timers[timername];
	auto &timepoint = timepoints[timername];
	timepoint = ESSIClock::now();

}

void ESSITimer::stop(std::string timername)
{
	auto &timer = timers[timername];
	auto &timepoint = timepoints[timername];
	timer += (ESSIClock::now() - timepoint);
}

void ESSITimer::report()
{
	std::ofstream fid(reportfilename, std::ios::out);

	for (auto const &timer : timers)
	{
		fid << timer.first << " : " << std::chrono::duration_cast<ESSIDuration>(timer.second).count() << std::endl;
	}

	fid.close();
	// return std::chrono::duration_cast<second_>(end - beginning).count();
}

