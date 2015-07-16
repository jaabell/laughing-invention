#include <iostream>
#include "../../ltensor/LTensor.h"
#include "EvolvingVariable.h"

int main(void)
{
	double nu = 2;
	double p0 = 0.0;

	auto evolaw = [nu](const DTensor2 & strain, const DTensor2 & plastic_strain, const DTensor2 & stress)->double
	{
		return nu ;
	};

	EvolvingVariable < double,  decltype(evolaw)> p;

	DTensor2 strain(3, 3, 0.0);
	DTensor2 plastic_strain(3, 3, 0.0);
	DTensor2 stress(3, 3, 0.0);

	strain(0, 0) = 1;

	double h = p.getDerivative(strain, plastic_strain, stress);

	std::cout << "h = " << h << std::endl;

	return 0;
}