//
// Created by tovermodus on 12/2/20.
//

#ifndef _MEDIUM_H_
#define _MEDIUM_H_

#include <vector>
#include <memory>
#include <random>
#include "Rod.h"
#include "MediumParameters.h"

class TrialMedium {
    private:
	std::vector<std::reference_wrapper<Rod>> rods;
	int changed_rod_index;
	Rod move_random(Rod r, const double & time_step, std::mt19937 rng);
    public:
	const MediumParameters * const  parameters;
	TrialMedium(const MediumParameters &params, const std::vector<Rod> & previous_rods);
	double calculate_energy() const;
	Rod get_changed_rod() const {return rods[changed_rod_index].get();}
	int get_changed_rod_index() const {return changed_rod_index;}
	void random_movement(const double & time_step, std::mt19937 rng);
};
class Medium {
    private:
	std::vector<Rod> rods;
    public:
	const MediumParameters parameters;
	//rod_density stands for rods per square meter, all units in meters
	explicit Medium(const MediumParameters &params);
	void initialize_rods();
	double calculate_energy() const;
	TrialMedium get_trial_medium();
	void update(TrialMedium trial_medium);
};



#endif //_MEDIUM_H_
