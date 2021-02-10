
//
// Created by tovermodus on 1/19/21.
//

#include <gtest/gtest.h>
#include "../Rod.h"
#include "../Medium.h"
#include "../MonteCarloLoop.h"

TEST(OverlapTest, fewInitial)
{
	double scale = 5e-5;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, IRON_DENSITY, 1, 100 / scale / scale, 5000, WATER_DENSITY,
				WATER_VISCOSITY, true, 00 * std::pow(scale, 6), 1 * scale, 1 * scale, EARTH_GRAVITY, ROOM_TEMPERATURE);
	Medium m(params, rng);
	std::cout << (int)(m.calculate_energy() / 1e200) << " overlaps \n";
	ASSERT_TRUE(m.calculate_energy() < 1e100);
}
TEST(OverlapTest, mediumInitial)
{
	double scale = 5e-5;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, IRON_DENSITY, 5, 500 / scale / scale, 5000, WATER_DENSITY,
				WATER_VISCOSITY, true, 00 * std::pow(scale, 6),1 * scale, 1 * scale, EARTH_GRAVITY, ROOM_TEMPERATURE);
	Medium m(params, rng);
	std::cout << (int)(m.calculate_energy() / 1e200) << " overlaps \n";
	ASSERT_TRUE(m.calculate_energy() < 1e100);
}
TEST(OverlapTest, manyInitial)
{
	double scale = 5e-5;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, IRON_DENSITY, 15, 1000 / scale / scale, 5000,
				WATER_DENSITY, WATER_VISCOSITY, true, 00 * std::pow(scale, 6),1 * scale, 1 * scale, EARTH_GRAVITY,
				ROOM_TEMPERATURE);
	Medium m(params, rng);
	std::cout << (int)(m.calculate_energy() / 1e200) << " overlaps \n";
	ASSERT_TRUE(m.calculate_energy() < 1e100);
}
TEST(OverlapTest, fewWaterIron)
{
	double scale = 5e-5;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, IRON_DENSITY, 15, 100 / scale / scale, 5000, WATER_DENSITY,
				WATER_VISCOSITY, true, 00 * std::pow(scale, 6),1 * scale, 1 * scale, EARTH_GRAVITY, ROOM_TEMPERATURE);
	MonteCarloLoop loop(params, rng, params.estimate_time_step());

	for (double i = 0; i < 2e5; ++i) {
		loop.monte_carlo_step();
	}
	ASSERT_TRUE(loop.calculate_energy() < 1e100);
}
TEST(OverlapTest, mediumWaterIron)
{
	double scale = 5e-3;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, IRON_DENSITY, 15, 300 / scale / scale, 5000, WATER_DENSITY,
				WATER_VISCOSITY, true, 00 * std::pow(scale, 6),1 * scale, 1 * scale, EARTH_GRAVITY, ROOM_TEMPERATURE);
	MonteCarloLoop loop(params, rng, params.estimate_time_step());

	for (double i = 0; i < 1e4; ++i) {
		loop.monte_carlo_step();
	}
	ASSERT_TRUE(loop.calculate_energy() < 1e100);
}
TEST(OverlapTest, manyWaterIron)
{
	double scale = 5e-6;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, IRON_DENSITY, 15, 1000 / scale / scale, 5000,
				WATER_DENSITY, WATER_VISCOSITY, true,00 * std::pow(scale, 6), 1 * scale, 1 * scale, EARTH_GRAVITY,
				ROOM_TEMPERATURE);
	MonteCarloLoop loop(params, rng, params.estimate_time_step());

	for (double i = 0; i < 1e4; ++i) {
		loop.monte_carlo_step();
	}
	ASSERT_TRUE(loop.calculate_energy() < 1e100);
}
TEST(OverlapTest, fewWaterLithium)
{
	double scale = 5e-5;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, LITHIUM_DENSITY, 15, 100 / scale / scale, 5000,
				WATER_DENSITY, WATER_VISCOSITY, true,00 * std::pow(scale, 6), 1 * scale, 1 * scale, EARTH_GRAVITY,
				ROOM_TEMPERATURE);
	MonteCarloLoop loop(params, rng, params.estimate_time_step());

	for (double i = 0; i < 2e5; ++i) {
		loop.monte_carlo_step();
	}
	ASSERT_TRUE(loop.calculate_energy() < 1e100);
}
TEST(OverlapTest, mediumWaterLithium)
{
	double scale = 5e-3;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, LITHIUM_DENSITY, 15, 300 / scale / scale, 5000,
				WATER_DENSITY, WATER_VISCOSITY, true,00 * std::pow(scale, 6), 1 * scale, 1 * scale, EARTH_GRAVITY,
				ROOM_TEMPERATURE);
	MonteCarloLoop loop(params, rng, params.estimate_time_step());

	for (double i = 0; i < 1e4; ++i) {
		loop.monte_carlo_step();
	}
	ASSERT_TRUE(loop.calculate_energy() < 1e100);
}
TEST(OverlapTest, manyWaterLithium)
{
	double scale = 5e-6;
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	MediumParameters params(0.05 * scale, 0.001 * scale, LITHIUM_DENSITY, 15, 1000 / scale / scale, 5000,
				WATER_DENSITY, WATER_VISCOSITY, true,00 * std::pow(scale, 6), 1 * scale, 1 * scale, EARTH_GRAVITY,
				ROOM_TEMPERATURE);
	MonteCarloLoop loop(params, rng, params.estimate_time_step());

	for (double i = 0; i < 1e4; ++i) {
		loop.monte_carlo_step();
	}
	ASSERT_TRUE(loop.calculate_energy() < 1e100);
}