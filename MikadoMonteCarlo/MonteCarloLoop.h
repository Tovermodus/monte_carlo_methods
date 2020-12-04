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
	double acceptance_probability(TrialMedium tm) const;


    public:
	MonteCarloLoop(const MediumParameters & parameters, const double & time_step);
	void monte_carlo_step();
};

#endif //MIKADOMONTECARLO__MONTECARLOLOOP_H_
