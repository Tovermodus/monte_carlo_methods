#include "MonteCarloLoop.h"

int main ()
{
	double scale = 5e-4;

	MediumParameters params(0.05*scale,
				0.001*scale,
				IRON_DENSITY,
				15,
				300/scale/scale,
				5000,
				WATER_DENSITY,
				WATER_VISCOSITY,
				true,
				1*scale,
				1*scale,
				EARTH_GRAVITY,
				ROOM_TEMPERATURE);

	std::cout <<params.estimate_time_step() << "\n";
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 rng = std::mt19937(rd());
	MonteCarloLoop loop(params,rng, params.estimate_time_step());
	int plotn = 0;
	double iterations = 1e5;
	int plot_interval = 5000;
    system(("mkdir PlotFiles/iterations:" + std::to_string((int)(iterations/plot_interval))+ params.to_string()).c_str());
    for(double i = 0; i < iterations; ++i) {
		if((int)i%plot_interval == 0) {
			loop.printToFile("PlotFiles/iterations:" + std::to_string((int)(iterations/plot_interval))+ params.to_string() + std::to_string(plotn++) + ".txt");
			std::cout << std::scientific << i << "\n";
		}
		loop.monte_carlo_step();
	}
	std::string command ="python3 ../Plot/visualization.py ../cmake-build-debug/PlotFiles/iterations:" + std::to_string((int)(iterations/plot_interval))+ params.to_string();
	system(command.c_str());
	return 0;
}
