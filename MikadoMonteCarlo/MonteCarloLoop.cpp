//
// Created by tovermodus on 12/2/20.
//

#include "MonteCarloLoop.h"
#include <iostream>
#include <fstream>

MonteCarloLoop::MonteCarloLoop(const MediumParameters &parameters, std::mt19937 rng, const double &time_step)
	: time_step(time_step)
{
	m = std::make_shared<Medium>(parameters, rng);
	uniform_distribution = std::uniform_real_distribution<double>(0.0, 1.0);
	time = 0.;
}
double MonteCarloLoop::calculate_energy()
{
	return m->calculate_energy();
}
void MonteCarloLoop::monte_carlo_step()
{
	Medium::Movement mov(m, time_step, rng);
	double ran = uniform_distribution(rng);
	if (ran < acceptance_probability_single_movement(mov)) {
		mov.execute_movement();
		time += time_step;
	}
}
double MonteCarloLoop::acceptance_probability(Medium::Movement mov) const
{
	double delta_energy = mov.calculate_energy_after_movement() - m->calculate_energy();
	//std::cout << mov.calculate_energy_after_movement() << " " << m->calculate_energy() <<" " <<  delta_energy <<"  "<<std::min(1., std::exp(-delta_energy / (m->parameters.boltz * m->parameters.temperature)))<<"\n";
	return std::min(1., std::exp(-delta_energy / (m->parameters.boltz * m->parameters.temperature)));
}
double MonteCarloLoop::acceptance_probability_single_movement(Medium::Movement mov) const
{
	double delta_energy =
		mov.calculate_energy_after_movement_for_rod() - mov.calculate_energy_before_movement_for_rod();
	//std::cout << mov.calculate_energy_after_movement_for_rod()  -  mov.calculate_energy_before_movement_for_rod() << " "
	//	  <<  (mov.calculate_energy_after_movement() - m->calculate_energy()) <<"\n";
	return std::min(1., std::exp(-delta_energy / (m->parameters.boltz * m->parameters.temperature)));
}
std::ostream &operator<<(std::ostream &os, const std::shared_ptr<Medium> &m)
{
	os << m->to_string();
	return os;
}
void MonteCarloLoop::printToFile(const std::string &filename)
{
	std::ofstream out_file;
	out_file.open(filename);
	out_file << m;
	out_file.close();
}
