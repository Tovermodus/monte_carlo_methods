//
// Created by tovermodus on 12/2/20.
//

#ifndef _MEDIUMPARAMETERS_H_
#define _MEDIUMPARAMETERS_H_
#include <cmath>

class MediumParameters {
public:
	double rod_length;
	double rod_width;
	double rod_density;
	double aspect_ratio;

	int number_of_cells_per_direction;

	double rods_per_volume;
	int rod_placing_max_iterations;

	double density;
	double viscosity;
	bool periodic_boundary_conditions;
	double width;
	double height;

	double gravity;
	double temperature;
	double boltz = 1.38064852e-23; //m^2 kg s^-2 K^-1
	double end_correction_rotation = -0.662;
	double end_correction_parallel = -0.207;
	double end_correction_perpendicular = 0.839;
	double friction_rotation;
	double friction_parallel;
	double friction_perpendicular;

	double diffusion_coefficient_rotation;
	double diffusion_coefficient_parallel;
	double diffusion_coefficient_perpendicular;

	MediumParameters(double rod_length,
			 double rod_width,
			 double rod_density,
			 int number_of_cells_per_direction,
			 double rods_per_volume,
			 int rod_placing_max_iterations,
			 double density,
			 double viscosity,
			 bool periodic_boundary_conditions,
			 double width,
			 double height,
			 double gravity,
			 double temperature)
		: rod_length(rod_length),
		  rod_width(rod_width),
		  rod_density(rod_density),
		  number_of_cells_per_direction(number_of_cells_per_direction),
		  rods_per_volume(rods_per_volume),
		  rod_placing_max_iterations(rod_placing_max_iterations),
		  density(density),
		  viscosity(viscosity),
		  periodic_boundary_conditions(periodic_boundary_conditions),
		  width(width),
		  height(height),
		  gravity(gravity),
		  temperature(temperature),
		  aspect_ratio(rod_length/rod_width),
		  friction_rotation(
			  M_PI*viscosity*std::pow(rod_length, 3)/(3*(std::log(aspect_ratio) + end_correction_rotation))),
		  friction_parallel(2*M_PI*viscosity*rod_length/(std::log(aspect_ratio) + end_correction_parallel)),
		  friction_perpendicular(4*M_PI*viscosity*rod_length/(std::log(aspect_ratio) + end_correction_perpendicular)),
		  diffusion_coefficient_rotation(boltz*temperature/friction_rotation),
		  diffusion_coefficient_parallel(boltz*temperature/friction_parallel),
		  diffusion_coefficient_perpendicular(boltz*temperature/friction_perpendicular)
	{}

	MediumParameters(const MediumParameters& params)
		: rod_length(params.rod_length),
		  rod_width(params.rod_width),
		  rod_density(params.rod_density),
		  number_of_cells_per_direction(params.number_of_cells_per_direction),
		  rods_per_volume(params.rods_per_volume),
		  rod_placing_max_iterations(params.rod_placing_max_iterations),
		  density(params.density),
		  viscosity(params.viscosity),
		  periodic_boundary_conditions(params.periodic_boundary_conditions),
		  width(params.width),
		  height(params.height),
		  gravity(params.gravity),
		  temperature(params.temperature),
		  boltz(params.boltz),
		  end_correction_rotation(params.end_correction_rotation),
		  end_correction_parallel(params.end_correction_parallel),
		  end_correction_perpendicular(params.end_correction_perpendicular),
		  aspect_ratio(rod_length/rod_width),
		  friction_rotation(
			  M_PI*viscosity*std::pow(rod_length, 3)/(3*(std::log(aspect_ratio) + end_correction_rotation))),
		  friction_parallel(2*M_PI*viscosity*rod_length/(std::log(aspect_ratio) + end_correction_parallel)),
		  friction_perpendicular(4*M_PI*viscosity*rod_length/(std::log(aspect_ratio) + end_correction_perpendicular)),
		  diffusion_coefficient_rotation(boltz*temperature/friction_rotation),
		  diffusion_coefficient_parallel(boltz*temperature/friction_parallel),
		  diffusion_coefficient_perpendicular(boltz*temperature/friction_perpendicular)
	{}

};
#endif //_MEDIUMPARAMETERS_H_