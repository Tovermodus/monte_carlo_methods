//
// Created by tovermodus on 12/2/20.
//

#ifndef _MEDIUMPARAMETERS_H_
#define _MEDIUMPARAMETERS_H_
#include <cmath>
#include <stdexcept>
#include <iostream>

const double WATER_DENSITY = 1000;
const double IRON_DENSITY = 7874;
const double LITHIUM_DENSITY = 534;
const double HONEY_DENSITY = 1450;
const double WATER_VISCOSITY = 0.001;
const double HONEY_VISCOSITY = 10;
const double EARTH_GRAVITY = 9.81;
const double ROOM_TEMPERATURE = 293;

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
	double ellipsoidal_potential;
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

	MediumParameters(double rod_length, double rod_width, double rod_density, int number_of_cells_per_direction,
			 double rods_per_volume, int rod_placing_max_iterations, double density, double viscosity,
			 bool periodic_boundary_conditions, double ellipsoidal_potential, double width, double height,
			 double gravity, double temperature)
		: rod_length(rod_length), rod_width(rod_width), rod_density(rod_density),
		  number_of_cells_per_direction(number_of_cells_per_direction), rods_per_volume(rods_per_volume),
		  rod_placing_max_iterations(rod_placing_max_iterations), density(density), viscosity(viscosity),
		  periodic_boundary_conditions(periodic_boundary_conditions),
		  ellipsoidal_potential(ellipsoidal_potential), width(width), height(height), gravity(gravity),
		  temperature(temperature), aspect_ratio(rod_length / rod_width),
		  friction_rotation(M_PI * viscosity * std::pow(rod_length, 3) /
				    (3 * (std::log(aspect_ratio) + end_correction_rotation))),
		  friction_parallel(2 * M_PI * viscosity * rod_length /
				    (std::log(aspect_ratio) + end_correction_parallel)),
		  friction_perpendicular(4 * M_PI * viscosity * rod_length /
					 (std::log(aspect_ratio) + end_correction_perpendicular)),
		  diffusion_coefficient_rotation(boltz * temperature / friction_rotation),
		  diffusion_coefficient_parallel(boltz * temperature / friction_parallel),
		  diffusion_coefficient_perpendicular(boltz * temperature / friction_perpendicular)
	{
		check_state();
	}

	MediumParameters(const MediumParameters &params)
		: rod_length(params.rod_length), rod_width(params.rod_width), rod_density(params.rod_density),
		  number_of_cells_per_direction(params.number_of_cells_per_direction),
		  rods_per_volume(params.rods_per_volume),
		  rod_placing_max_iterations(params.rod_placing_max_iterations), density(params.density),
		  viscosity(params.viscosity), periodic_boundary_conditions(params.periodic_boundary_conditions),
		  ellipsoidal_potential(params.ellipsoidal_potential), width(params.width), height(params.height),
		  gravity(params.gravity), temperature(params.temperature), boltz(params.boltz),
		  end_correction_rotation(params.end_correction_rotation),
		  end_correction_parallel(params.end_correction_parallel),
		  end_correction_perpendicular(params.end_correction_perpendicular),
		  aspect_ratio(rod_length / rod_width),
		  friction_rotation(M_PI * viscosity * std::pow(rod_length, 3) /
				    (3 * (std::log(aspect_ratio) + end_correction_rotation))),
		  friction_parallel(2 * M_PI * viscosity * rod_length /
				    (std::log(aspect_ratio) + end_correction_parallel)),
		  friction_perpendicular(4 * M_PI * viscosity * rod_length /
					 (std::log(aspect_ratio) + end_correction_perpendicular)),
		  diffusion_coefficient_rotation(boltz * temperature / friction_rotation),
		  diffusion_coefficient_parallel(boltz * temperature / friction_parallel),
		  diffusion_coefficient_perpendicular(boltz * temperature / friction_perpendicular)
	{
		check_state();
	}
	void check_state()
	{
		if (rod_length < rod_width)
			throw std::domain_error("rod width is larger than rod length");
		if (rod_length > width || rod_length > height)
			throw std::domain_error("rod is larger than medium" + std::to_string(rod_length) + " " +
						std::to_string(width) + " " + std::to_string(height));
	}
	double estimate_time_step()
	{
		double cell_size = width / number_of_cells_per_direction;
		double safety_coefficient = 0.1;
		return safety_coefficient * cell_size * cell_size / 2 *
		       std::min(1. / diffusion_coefficient_parallel, 1. / diffusion_coefficient_perpendicular);
	}

	std::string to_string()
	{
		std::string rod_medium, liquid_medium;
		switch ((int)rod_density) {
		case 1000:
			rod_medium = "water";
			break;

		case 7874:
			rod_medium = "iron";
			break;

		case 534:
			rod_medium = "lithium";
			break;

		case 1450:
			rod_medium = "honey";
			break;
		}
		switch ((int)density) {
		case 1000:
			liquid_medium = "water";
			break;

		case 7874:
			liquid_medium = "iron";
			break;

		case 534:
			liquid_medium = "lithium";
			break;

		case 1450:
			liquid_medium = "honey";
			break;
		}
		return "-rod_med:" + rod_medium + "-liquid_med:" + liquid_medium +
		       "-rod_len*1000:" + std::to_string(rod_length * 1000) +
		       "-rod_width*1000:" + std::to_string(rod_width * 1000) +
		       "-rods_per_volume:" + std::to_string(rods_per_volume) +
		       "-ellipsoidal_potential:" + std::to_string(ellipsoidal_potential / std::pow(width, 6)) +
		       "-periodic_boundary:" + std::to_string(periodic_boundary_conditions) + "/";
	}
};
#endif //_MEDIUMPARAMETERS_H_