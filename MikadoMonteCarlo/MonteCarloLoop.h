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
	Medium m;
	double time;
	std::mt19937 rng;
	std::uniform_real_distribution<double> uniform_distribution;
	double acceptance_probability(const TrialMedium& tm) const;


    public:
	MonteCarloLoop(const MediumParameters & parameters, std::mt19937 rng, const double & time_step);
	void monte_carlo_step();
	void printToFile(const std::string& filename);
};

#endif //MIKADOMONTECARLO__MONTECARLOLOOP_H_
