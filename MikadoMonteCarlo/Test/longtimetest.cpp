//
// Created by tovermodus on 1/19/21.
//

#include <gtest/gtest.h>
#include "../Rod.h"
#include "../Medium.h"
#include "../MonteCarloLoop.h"
TEST(LongTimeTest, manyWaterIron)
{
	double scale = 5e-6;

	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, IRON_DENSITY, 15, 1000 / scale / scale, 5000,
				WATER_DENSITY, WATER_VISCOSITY, true, 1 * scale, 1 * scale, EARTH_GRAVITY,
				ROOM_TEMPERATURE);
	MonteCarloLoop loop(params, rng, params.estimate_time_step());

	for (double i = 0; i < 1e7; ++i) {
		loop.monte_carlo_step();
	}
	ASSERT_TRUE(loop.calculate_energy() < 1e100);
}