//
// Created by tovermodus on 12/2/20.
//

#include <sstream>
#include <iomanip>
#include <iostream>
#include "Medium.h"
Medium::Medium (const MediumParameters& params,std::mt19937 & rng):parameters(params)

{
	initialize_rods(rng);
}
void Medium::initialize_rods(std::mt19937 & rng)
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
void Medium::update(const TrialMedium& trial_medium)
{
	rods[trial_medium.get_changed_rod_index()] = trial_medium.get_changed_rod();
}
bool Medium::rod_is_acceptable(const Rod & rod)
{
	for(const Rod& r:rods)
		if(r.check_collision(rod))
			return false;
	return true;
}
Rod Medium::create_random_rod(std::mt19937 & rng) const
{
	std::uniform_real_distribution<> x_distrib(0, parameters.width);
	std::uniform_real_distribution<> y_distrib(0, parameters.height);
	std::uniform_real_distribution<> angle_distrib(0, M_PI);
	return Rod(x_distrib(rng), y_distrib(rng), angle_distrib(rng), parameters.rod_width, parameters.rod_length);
}
std::string Medium::to_string() const
{
	std::ostringstream ret;
	ret << std::scientific << parameters.width << " ";
	ret << std::scientific << parameters.height << "\n";
	ret << std::scientific << parameters.rod_width << " ";
	ret << std::scientific << parameters.rod_length << "\n";
	for(const Rod& r: rods){
		ret << std::scientific << r.get_x() << " ";
		ret << std::scientific << r.get_y() << " ";
		ret << std::scientific << r.get_angle() << "\n";
	}
	return ret.str();
}
TrialMedium::TrialMedium(const MediumParameters &params, std::vector<Rod> &previous_rods) : parameters(&params)
{
	rods = std::vector<std::reference_wrapper<Rod>>(previous_rods.begin(),previous_rods.end());
	changed_rod_index = -1;
}
void TrialMedium::random_movement(const double &time_step, std::mt19937 & rng)
{
	std::uniform_int_distribution<> distrib(0, rods.size()-1);
	changed_rod_index = distrib(rng);
	Rod new_rod = move_random(rods[changed_rod_index],time_step,rng);
	std::cout <<new_rod.get_x() << " " << new_rod.get_y() << " " << new_rod.get_angle() <<"\n";
	rods[changed_rod_index] = std::ref(new_rod);
	std::cout <<get_changed_rod().get_x() << " " << get_changed_rod().get_y() << " " << get_changed_rod().get_angle() <<"\n";
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
Rod TrialMedium::move_random(const Rod & r, const double &time_step, std::mt19937 & rng) const
{
	double amplitude_parallel = std::sqrt(2*parameters->diffusion_coefficient_parallel*time_step);
	double amplitude_perpendicular = std::sqrt(2*parameters->diffusion_coefficient_perpendicular*time_step);
	double amplitude_rotation = std::sqrt(2*parameters->diffusion_coefficient_rotation*time_step);
	std::uniform_real_distribution<> parallel_distrib(-amplitude_parallel, amplitude_parallel);
	std::uniform_real_distribution<> perpendicular_distrib(-amplitude_perpendicular, amplitude_perpendicular);
	std::uniform_real_distribution<> rotation_distrib(-amplitude_rotation, amplitude_rotation);
	double parallel_movement = parallel_distrib(rng);
	double perpendicular_movement = perpendicular_distrib(rng);
	double rotation_movement = rotation_distrib(rng);
	std::cout <<parallel_movement << " " << perpendicular_movement << " " << rotation_movement <<"\n";
	return r.generate_moved_rod(parallel_movement,perpendicular_movement,rotation_movement);


}
