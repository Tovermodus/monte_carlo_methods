//
// Created by tovermodus on 12/2/20.
//

#include "MonteCarloLoop.h"
#include <iostream>
#include <fstream>

MonteCarloLoop::MonteCarloLoop(const MediumParameters & parameters,std::mt19937 rng, const double & time_step) : time_step(time_step), m(parameters,rng)
{
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
double MonteCarloLoop::acceptance_probability(const TrialMedium& tm) const
{
	double delta_energy = tm.calculate_energy() - m.calculate_energy();
	return 	std::min(1., std::exp(-delta_energy / (m.parameters.boltz * m.parameters.temperature)));
}
std::ostream &operator<<(std::ostream &os, Medium const &m) {
	os << m.to_string();
	return os;
}
void MonteCarloLoop::printToFile(const std::string& filename)
{
	std::ofstream out_file;
	out_file.open(filename);
	out_file << m;
	out_file.close();
}
