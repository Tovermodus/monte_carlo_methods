//
// Created by tovermodus on 12/2/20.
//

#include "Medium.h"
Medium::Medium (const MediumParameters& params,const std::mt19937 & rng):parameters(params)

{
	throw "Not Implemented yet";
}
void Medium::initialize_rods(const std::mt19937 & rng)
{
	rods = std::vector<Rod>();
	int iterations = 0;
	int total_number_of_rods = (int)(parameters.rods_per_volume*parameters.width*parameters.height);
	while(rods.size() < total_number_of_rods && iterations < parameters.rod_placing_max_iterations)
	{
		Rod new_rod = create_random_rod(rng);
		if(rod_is_acceptable(new_rod)) {
			rods.push_back(new_rod);
		}
		iterations ++;
	}
}
double Medium::calculate_energy() const
{
	throw "Not Implemented yet";
}
TrialMedium Medium::get_trial_medium()
{
	return TrialMedium(parameters, rods);
}
void Medium::update(const TrialMedium& trial_medium)
{
	rods[trial_medium.get_changed_rod_index()] = trial_medium.get_changed_rod();
}
bool Medium::rod_is_acceptable(const Rod & rod)
{
	for(Rod r:rods)
		if(r.check_collision(rod))
			return false;
	return true;
}
Rod Medium::create_random_rod(const std::mt19937 & rng) const
{
	std::uniform_int_distribution<> x_distrib(0, parameters.width);
	std::uniform_int_distribution<> y_distrib(0, parameters.height);
	std::uniform_int_distribution<> angle_distrib(0, M_PI);
	return Rod(x_distrib(rng), y_distrib(rng), angle_distrib(rng), parameters.rod_width, parameters.rod_length);
}
TrialMedium::TrialMedium(const MediumParameters &params, std::vector<Rod> &previous_rods) : parameters(&params)
{
	rods = std::vector<std::reference_wrapper<Rod>>(previous_rods.begin(),previous_rods.end());
}
void TrialMedium::random_movement(const double &time_step, const std::mt19937 & rng)
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
Rod TrialMedium::move_random(Rod r, const double &time_step, const std::mt19937 & rng)
{
	throw "Not Implemented yet";
}
//This function generates the output which is then written to a file
std::ostream &operator<<(std::ostream &os, Medium const &m) {
	throw "Not Implemented yet";
}
