#include "MonteCarloLoop.h"

int main ()
{
	double scale = 1e-6;
	MediumParameters params(0.03*scale,0.001*scale,1,500/scale/scale,800,0,100,true,1*scale,1*scale,10,300);
	std::cout <<params.diffusion_coefficient_parallel << "\n";
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 rng = std::mt19937(rd());
	MonteCarloLoop loop(params,rng, 1e-1);
	int plotn = 0;
	system("mkdir PlotFiles");
	for(int i = 0; i < 20000; ++i) {
		std::cout << i<<"\n";
		if(i%50 == 0)
			loop.printToFile("PlotFiles/"+std::to_string(plotn++)+".txt");
		loop.monte_carlo_step();
	}
	system("python3 ../Plot/visualization.py");
	return 0;
}
