#include "ESSITimer.h"
// #include <stdlib.h>

int main(void)
{

	std::cout << "Resolution: " << std::chrono::high_resolution_clock::period::den << std::endl;


	ESSITimer timers("report.txt");

	timers.start("timer1");
	timers.start("timer2");
	timers.start("timer3");


	std::cin.ignore();

	timers.stop("timer1");

	std::cin.ignore();

	// for (int i = 0; i < 1000000; i++);
	timers.stop("timer2");

	std::cin.ignore();
	// for (int i = 0; i < 10000000; i++);
	timers.stop("timer3");

	timers.report();

	return 0;
}