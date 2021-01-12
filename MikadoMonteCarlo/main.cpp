#include "MonteCarloLoop.h"

int main ()
{
	double scale = 5e-5;
	MediumParameters params(0.05*scale,
				0.001*scale,IRON_DENSITY,
				15,
				1000/scale/scale,
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
	system("mkdir PlotFiles");
	double iterations = 1e7;
	int plot_interval = 5000;
	for(double i = 0; i < iterations; ++i) {
		if((int)i%plot_interval == 0) {
			loop.printToFile("PlotFiles/" + std::to_string(plotn++) + ".txt");
			std::cout << std::scientific << i << "\n";
		}
		loop.monte_carlo_step();
	}
	std::string command ="python3 ../Plot/visualization.py " + std::to_string((int)(iterations/plot_interval));
	system(command.c_str());
	return 0;
}
