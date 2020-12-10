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
	Rod move_random(const Rod & r, const double & time_step, std::mt19937 & rng) const;
    public:
	const MediumParameters * const  parameters;
	TrialMedium(const MediumParameters &params, std::vector<Rod> & previous_rods);
	double calculate_energy() const;
	Rod get_changed_rod() const {return rods[changed_rod_index].get();}
	int get_changed_rod_index() const {return changed_rod_index;}
	void random_movement(const double & time_step, std::mt19937 & rng);
};
class Medium {
    private:
	std::vector<Rod> rods;
	Rod create_random_rod(std::mt19937 & rng) const;
	bool rod_is_acceptable(const Rod & rod);
    public:
	const MediumParameters parameters;
	//rod_density stands for rods per square meter, all units in meters
	explicit Medium(const MediumParameters &params, std::mt19937 & rng);
	void initialize_rods(std::mt19937 & rng);
	double calculate_energy() const;
	TrialMedium get_trial_medium();
	std::string to_string() const;
	void update(const TrialMedium& trial_medium);
};
//This function generates the output which is then written to a file



#endif //_MEDIUM_H_
