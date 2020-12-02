//
// Created by tovermodus on 12/2/20.
//

#include "MonteCarloLoop.h"
MonteCarloLoop::MonteCarloLoop(const MediumParameters & parameters, const double & time_step) : time_step(time_step), m(Medium(parameters))
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	rng = std::mt19937(rd());
	uniform_distribution = std::uniform_real_distribution<double>(0.0,1.0);
	time = 0.;

}
void MonteCarloLoop::monte_carlo_step()
{
	TrialMedium tm = m.get_trial_medium();
	tm.random_movement(time_step, rng);
	if (uniform_distribution(rng) < acceptance_probability(tm)) {
		m.update(tm);
		time += time_step;
	}
}
double MonteCarloLoop::acceptance_probability(TrialMedium tm) const
{
	double delta_energy = tm.calculate_energy() - m.calculate_energy();
	return 	std::min(1., std::exp(-delta_energy / (m.parameters.boltz * m.parameters.temperature)));
}
