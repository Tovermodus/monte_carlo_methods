//
// Created by tovermodus on 12/2/20.
//

#ifndef MIKADOMONTECARLO__MONTECARLOLOOP_H_
#define MIKADOMONTECARLO__MONTECARLOLOOP_H_
#include <random>
#include "MediumParameters.h"
#include "Medium.h"
class MonteCarloLoop {
    private:
	const double time_step;
	std::shared_ptr<Medium> m;
	double time;
	std::mt19937 rng;
	std::uniform_real_distribution<double> uniform_distribution;
	double acceptance_probability(Medium::Movement tm) const;
	double acceptance_probability_single_movement(Medium::Movement mov) const;

    public:
	MonteCarloLoop(const MediumParameters &parameters, std::mt19937 rng, const double &time_step);
	void monte_carlo_step();
	void printToFile(const std::string &filename);
	double calculate_energy();
};

#endif //MIKADOMONTECARLO__MONTECARLOLOOP_H_
