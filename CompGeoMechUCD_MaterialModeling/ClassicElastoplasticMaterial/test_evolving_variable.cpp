#include <iostream>
#include "../../ltensor/LTensor.h"
#include "EvolvingVariable.h"

int main(void)
{
	double nu = 4;
	double p0 = 0.0;
	DTensor2 alpha0(3, 3, 0.0);

	//Current stresses and strains
	DTensor2 strain(3, 3, 0.0);
	DTensor2 plastic_strain(3, 3, 0.0);
	DTensor2 stress(3, 3, 0.0);

	strain(0, 0) = 1;
	strain(1, 1) = 1;
	strain(2, 2) = 1;


	// Test with scalar double
	//======================================================================


	auto ev_p =
	    [ nu ](const DTensor2 & strain,
	           const DTensor2 & plastic_strain,
	           const DTensor2 & stress)
	    -> double
	{
		std::cout << nu << std::endl;
		return nu ;
	};


	// struct
	// {
	// 	double nu = nu;

	// 	double operator()(strain, plastic_strain, stress)

	// } ev_p;


	std::cout << "ev_p() = " << ev_p(strain, plastic_strain, stress) << std::endl;


	EvolvingVariable < double,  decltype(ev_p)> p(p0, &ev_p);




	double h_p = p.getDerivative(strain, plastic_strain, stress);

	std::cout << "h_p = " << h_p << std::endl;

	std::cout << "p   = " << p.getVariable() << std::endl;

	p.evolve(1, strain, plastic_strain, stress);
	std::cout << "p   = " << p.getVariable() << std::endl;

	p.evolve(1, strain, plastic_strain, stress);
	std::cout << "p   = " << p.getVariable() << std::endl;







	//Test with LTensor
	//======================================================================

	auto ev_alpha =
	    [](const DTensor2 & strain,
	       const DTensor2 & plastic_strain,
	       const DTensor2 & stress)
	    -> DTensor2
	{
		return strain;
	};


	EvolvingVariable < DTensor2,  decltype(ev_alpha)> alpha(alpha0, &ev_alpha);


	DTensor2 h_alpha = alpha.getDerivative(strain, plastic_strain, stress);
	std::cout << "h_alpha = " << h_alpha << std::endl;

	alpha.evolve(1, strain, plastic_strain, stress);
	std::cout << "  alpha = " << alpha.getVariable() << std::endl;

	alpha.evolve(1, strain, plastic_strain, stress);
	std::cout << "  alpha = " << alpha.getVariable() << std::endl;




	return 0;
}