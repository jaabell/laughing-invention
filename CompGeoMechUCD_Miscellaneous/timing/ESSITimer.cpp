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
//                    components.
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

ESSITimer globalESSITimer;

ESSITimer::ESSITimer():
	reportfilename(""),
	header("")
{
	activated = false;
}

ESSITimer::ESSITimer(std::string reportfilename_, std::string header_):
	reportfilename(reportfilename_),
	header(header_)
{
	activated = false;
}

void ESSITimer::setReportFileName(std::string reportfilename_, std::string header_)
{
	reportfilename = reportfilename_;
	header = header_;
	activated = true;
}

void ESSITimer::start(std::string timername)
{
	if (activated)
	{
		auto &timepoint = timepoints[timername];
		timepoint = ESSIClock::now();
	}
}

void ESSITimer::stop(std::string timername)
{
	if (activated)
	{
		auto &timer = timers[timername];
		auto &timepoint = timepoints[timername];
		timer += (ESSIClock::now() - timepoint);
	}
}

void ESSITimer::report()
{

	if (activated)
	{
		std::ofstream fid(reportfilename, std::ios::out);

		fid << header << std::endl;
		for (auto const &timer : timers)
		{
			fid << timer.first << " : " << std::chrono::duration_cast<ESSIDuration>(timer.second).count() << std::endl;
		}

		fid.close();
	}
}

void ESSITimer::print()
{

	if (activated)
	{
		std::cout << header << std::endl;
		for (auto const &timer : timers)
		{
			std::cout << timer.first << " : " << std::chrono::duration_cast<ESSIDuration>(timer.second).count() << std::endl;
		}
	}
	else
	{
		std::cout << "ESSITimer inactive.\n";
	}
}



void ESSITimer::resetAll()
{
	if (activated)
	{
		for (auto const &timer : timers)
		{
			timer.second.zero();
		}
	}
}
void ESSITimer::reset(std::string timername)
{
	if (activated)
	{
		timers[timername].zero();
	}
}
void ESSITimer::clearAll()
{
	if (activated)
	{
		timers.clear();
	}
}
void ESSITimer::remove(std::string timername)
{
	if (activated)
	{
		timers.erase(timername);
	}
}



