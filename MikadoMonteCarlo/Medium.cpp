//
// Created by tovermodus on 12/2/20.
//

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
	int rodnr= rods.size();
	for (int i = 0; i < rodnr; ++i) {
		for (int j = 0; j < rodnr; ++j) {
			if (rods[i].check_collision(rods[j]) && i!=j) {
				return 1e100;
			}
		}
	}
	return 0;
}
TrialMedium Medium::get_trial_medium()
{
	return TrialMedium(parameters, rods);
}
void Medium::update(TrialMedium trial_medium)
{
	rods[trial_medium.get_changed_rod_index()] = trial_medium.get_changed_rod();
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
	int rodnr= rods.size();
	for (int i = 0; i < rodnr; ++i) {
		for (int j = 0; j < rodnr; ++j) {
			if (rods[i].get().check_collision(rods[j].get()) && i!=j) {
				return 1e100;
			}
		}
	}
	return 0;
}
Rod TrialMedium::move_random(Rod r, const double &time_step, std::mt19937 rng)
{
	throw "Not Implemented yet";
}
//This function generates the output which is then written to a file
std::ostream &operator<<(std::ostream &os, Medium const &m) {
	throw "Not Implemented yet";
}
