#include "MonteCarloLoop.h"

int main ()
{
	double scale = 2e-6;
	MediumParameters params(0.06*scale,0.003*scale,7874,25,300/scale/scale,5000,1000,1000,true,1*scale,1*scale,1000000,300);
	std::cout <<params.diffusion_coefficient_parallel << "\n";
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 rng = std::mt19937(rd());
	MonteCarloLoop loop(params,rng, 1e-1);
	int plotn = 0;
	system("mkdir PlotFiles");
	int iterations = 100000000;
	int plot_interval = 5000;
	for(int i = 0; i < iterations; ++i) {
		if(i%plot_interval == 0) {
			loop.printToFile("PlotFiles/" + std::to_string(plotn++) + ".txt");
			std::cout << i << "\n";
		}
		loop.monte_carlo_step();
	}
	std::string command ="python3 ../Plot/visualization.py " + std::to_string(iterations/plot_interval);
	system(command.c_str());
	return 0;
}
