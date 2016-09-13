
//! Inputs:
//! - gamma, beta: Newmark parameters

int define_dynamic_solution_advancement_integrator_newmark(double gamma, double beta)
{
	theTransientIntegrator = new Newmark(gamma, beta);

	if (theTransientIntegrator == NULL)
	{
		cerr << "Error: (define_dynamic_solution_advancement_integrator_newmark) memory allocating theTransientIntegrator!" << endl;
		return -1;
	}

	return 0;
};




