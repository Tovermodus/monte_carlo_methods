#include "MonteCarloLoop.h"

int main ()
{
	MediumParameters params(0.1,0.01,1,200,500,0,1,true,1,1,10,300);

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 rng = std::mt19937(rd());
	MonteCarloLoop loop(params,rng, 1);
	loop.printToFile("start.txt");
	for(int i = 0; i < 10; ++i)
		loop.monte_carlo_step();
	loop.printToFile("end.txt");
	return 0;
}
