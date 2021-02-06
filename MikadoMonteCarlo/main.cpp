#include "MonteCarloLoop.h"

int main()
{
	double scale = 5e-4;

	MediumParameters params(0.1*0.6 * scale, 0.002*0.6 * scale, LITHIUM_DENSITY, 15, 300 / scale / scale, 5000, HONEY_DENSITY,
				HONEY_VISCOSITY, true, 0 * std::pow(scale, 6), 1 * scale, 1 * scale, EARTH_GRAVITY,
				ROOM_TEMPERATURE);

	std::cout << params.estimate_time_step() << "\n";
	std::random_device rd; //Will be used to obtain a seed for the random number engine
	std::mt19937 rng = std::mt19937(rd());
	MonteCarloLoop loop(params, rng, params.estimate_time_step());
	int plotn = 0;
	double iterations = 5e8;
	int plot_interval = 1000;
	system("mkdir PlotFiles");
	system(("mkdir PlotFiles/iterations:" + std::to_string((int)(iterations / plot_interval)) + params.to_string())
		       .c_str());
	for (double i = 0; i < iterations; ++i) {
		if ((int)i % plot_interval == 0) {
			loop.printToFile("PlotFiles/iterations:" + std::to_string((int)(iterations / plot_interval)) +
					 params.to_string() + std::to_string(plotn++) + ".txt");
			std::cout << std::scientific << i << "\n";
		}
		loop.monte_carlo_step();
	}
	std::string command = "python3 ../Plot/Order_plot.py ../cmake-build-debug/PlotFiles/iterations:" +
			      std::to_string((int)(iterations / plot_interval)) + params.to_string() + "&";
	system(command.c_str());
	command = "python3 ../Plot/endframe.py ../cmake-build-debug/PlotFiles/iterations:" +
		  std::to_string((int)(iterations / plot_interval)) + params.to_string() + "&";
	system(command.c_str());
	command = "python3 ../Plot/visualization.py ../cmake-build-debug/PlotFiles/iterations:" +
		  std::to_string((int)(iterations / plot_interval)) + params.to_string();
	system(command.c_str());
	return 0;
}
