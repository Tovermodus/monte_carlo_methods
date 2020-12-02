//
// Created by tovermodus on 12/2/20.
//

#ifndef _MEDIUMPARAMETERS_H_
#define _MEDIUMPARAMETERS_H_
#include <cmath>

class MediumParameters {
public:
	const double rod_length;
	const double rod_width;
	const double rod_density;
	const double aspect_ratio;

	const double rods_per_volume;

	const double density;
	const double viscosity;
	const bool periodic_boundary_conditions;
	const double width;
	const double height;

	const double gravity;
	const double temperature;
	const double boltz = 1.38064852e-23; //m^2 kg s^-2 K^-1
	const double end_correction_rotation = -0.662;
	const double end_correction_parallel = -0.207;
	const double end_correction_perpendicular = 0.839;
	const double friction_rotation;
	const double friction_parallel;
	const double friction_perpendicular;

	const double diffusion_coefficient_rotation;
	const double diffusion_coefficient_parallel;
	const double diffusion_coefficient_perpendicular;

	MediumParameters(const double rod_length,
			 const double rod_width,
			 const double rod_density,
			 const double rods_per_volume,
			 const double density,
			 const double viscosity,
			 const bool periodic_boundary_conditions,
			 const double width,
			 const double height,
			 const double gravity,
			 const double temperature,
			 const double end_correction_rotation,
			 const double end_correction_parallel,
			 const double end_correction_perpendicular)
		: rod_length(rod_length),
		  rod_width(rod_width),
		  rod_density(rod_density),
		  rods_per_volume(rods_per_volume),
		  density(density),
		  viscosity(viscosity),
		  periodic_boundary_conditions(periodic_boundary_conditions),
		  width(width),
		  height(height),
		  gravity(gravity),
		  temperature(temperature),
		  boltz(boltz),
		  end_correction_rotation(end_correction_rotation),
		  end_correction_parallel(end_correction_parallel),
		  end_correction_perpendicular(end_correction_perpendicular),
		  aspect_ratio(rod_length/rod_width),
		  friction_rotation(
			  M_PI*viscosity*std::pow(rod_length, 3)/3*(std::log(aspect_ratio) + end_correction_rotation)),
		  friction_parallel(2*M_PI*viscosity*rod_length/(std::log(aspect_ratio) + end_correction_parallel)),
		  friction_perpendicular(4*M_PI*viscosity*rod_length/(std::log(aspect_ratio) + end_correction_perpendicular)),
		  diffusion_coefficient_rotation(boltz*temperature/friction_rotation),
		  diffusion_coefficient_parallel(boltz*temperature/friction_parallel),
		  diffusion_coefficient_perpendicular(boltz*temperature/friction_perpendicular)
	{}

	MediumParameters(MediumParameters &params)
		: rod_length(params.rod_length),
		  rod_width(params.rod_width),
		  rod_density(params.rod_density),
		  rods_per_volume(params.rods_per_volume),
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
			  M_PI*viscosity*std::pow(rod_length, 3)/3*(std::log(aspect_ratio) + end_correction_rotation)),
		  friction_parallel(2*M_PI*viscosity*rod_length/(std::log(aspect_ratio) + end_correction_parallel)),
		  friction_perpendicular(4*M_PI*viscosity*rod_length/(std::log(aspect_ratio) + end_correction_perpendicular)),
		  diffusion_coefficient_rotation(boltz*temperature/friction_rotation),
		  diffusion_coefficient_parallel(boltz*temperature/friction_parallel),
		  diffusion_coefficient_perpendicular(boltz*temperature/friction_perpendicular)
	{}

};
#endif //_MEDIUMPARAMETERS_H_