//
// Created by tovermodus on 12/2/20.
//

#include <sstream>
#include <iomanip>
#include "Medium.h"
Medium::Medium (const MediumParameters& params):parameters(params)

{
	throw "Not Implemented yet";
}
void Medium::initialize_rods()
{
	throw "Not Implemented yet";
}
double Medium::calculate_energy() const
{
	throw "Not Implemented yet";
}
TrialMedium Medium::get_trial_medium()
{
	return TrialMedium(parameters, rods);
}
void Medium::update(TrialMedium trial_medium)
{
	rods[trial_medium.get_changed_rod_index()] = trial_medium.get_changed_rod();
}
std::string Medium::to_string() const
{
	std::ostringstream ret;
	for(Rod r: rods){
		ret << std::scientific << r.get_x() << " ";
		ret << std::scientific << r.get_y() << " ";
		ret << std::scientific << r.get_angle() << "\n";
	}
	return ret.str();
}
TrialMedium::TrialMedium(const MediumParameters &params, std::vector<Rod> &previous_rods) : parameters(&params)
{
	rods = std::vector<std::reference_wrapper<Rod>>(previous_rods.begin(),previous_rods.end());
}
void TrialMedium::random_movement(const double &time_step, std::mt19937 rng)
{
	std::uniform_int_distribution<> distrib(0, rods.size()-1);
	changed_rod_index = distrib(rng);
	Rod new_rod = move_random(rods[changed_rod_index],time_step,rng);
	rods[changed_rod_index] = std::ref(new_rod);
}
double TrialMedium::calculate_energy() const
{
	throw "Not Implemented yet";
}
Rod TrialMedium::move_random(Rod r, const double &time_step, std::mt19937 rng)
{
	throw "Not Implemented yet";
}
//This function generates the output which is then written to a file
std::ostream &operator<<(std::ostream &os, Medium const &m) {
	os << m.to_string();
	return os;
}
