#include "MonteCarloLoop.h"

int main ()
{
	MediumParameters params(0.1,0.01,1,80,500,0,100,true,1,1,10,300);
	std::cout <<params.diffusion_coefficient_parallel << "\n";
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 rng = std::mt19937(rd());
	MonteCarloLoop loop(params,rng, 1e18);
	int plotn = 0;
	system("mkdir PlotFiles");
	for(int i = 0; i < 10000; ++i) {
		std::cout << i<<"\n";
		if(i%20 == 0)
			loop.printToFile("PlotFiles/"+std::to_string(plotn++)+".txt");
		loop.monte_carlo_step();
	}
	system("python3 ../Plot/visualization.py");
	return 0;
}
