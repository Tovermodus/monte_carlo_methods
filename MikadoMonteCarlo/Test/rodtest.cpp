//
// Created by tovermodus on 12/4/20.
//

#include <gtest/gtest.h>
#include <random>
#include "../Cell.h"
#include "../Rod.h"
TEST(RodTests, collisiontest1)
{
	std::shared_ptr<Rod> r1 = std::make_shared<Rod>(0.5, 0.5, 0, 0.05, 0.3, nullptr);
	std::shared_ptr<Rod> r2 = std::make_shared<Rod>(0.4, 0.4, 0.3, 0.05, 0.3, nullptr);
	std::shared_ptr<Rod> r3 = std::make_shared<Rod>(0.4, 0.4, 0.4, 0.05, 0.3, nullptr);
	ASSERT_FALSE(r1->check_collision(r2));
	ASSERT_TRUE(r2->check_collision(r3));
	ASSERT_TRUE(r1->check_collision(r3));
}

TEST(RodTests, collisiontest2)
{
	std::shared_ptr<Rod> r1 = std::make_shared<Rod>(0.5, 0.5, 1.4, 0.05, 0.3, nullptr);
	std::shared_ptr<Rod> r2 = std::make_shared<Rod>(0.5, 0.75, 1.6, 0.05, 0.3, nullptr);
	std::shared_ptr<Rod> r3 = std::make_shared<Rod>(0.6, 0.75, 1.6, 0.05, 0.3, nullptr);
	ASSERT_TRUE(r1->check_collision(r2));
	ASSERT_FALSE(r2->check_collision(r3));
	ASSERT_FALSE(r1->check_collision(r3));
}
TEST(RodTests, onTopTest)
{
	std::shared_ptr<Rod> r1 = std::make_shared<Rod>(0.5, 0.5, 1.4, 0.05, 0.3, nullptr);
	std::shared_ptr<Rod> r2 = std::make_shared<Rod>(0.5, 0.5, 1.4, 0.05, 0.3, nullptr);
	ASSERT_TRUE(r1->check_collision(r2));
}

TEST(RodTests, reverseMovementTest1)
{
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	std::uniform_real_distribution<> pos_distrib(0, 1);
	std::uniform_real_distribution<> ang_distrib(0, M_PI);
	std::uniform_real_distribution<> mov_distrib(-0.2, 0.2);
	std::shared_ptr<Cell> c = std::make_shared<Cell>(0.5, 0.5, 1, 1);
	for (int i = 0; i < 1000; i++) {
		double x = pos_distrib(rng);
		double y = pos_distrib(rng);
		std::cout << x << " " << y << "\n";
		std::shared_ptr<Rod> r1 = std::make_shared<Rod>(x, y, ang_distrib(rng), 0.05, 0.3, c);
		double mpar = mov_distrib(rng);
		double mper = mov_distrib(rng);
		double mrot = ang_distrib(rng);
		r1->move_rod(mpar, mper, mrot);
		std::cout << "hhj\n";
		std::pair<double, double> boundary_movement = r1->apply_periodic_boundary_conditions(1, 1);
		r1->reverse_boundary_movement(boundary_movement);
		r1->reverse_move_rod(mpar, mper, mrot);

		ASSERT_TRUE(std::abs(r1->get_x() - x) <= 1e-14);
		ASSERT_TRUE(std::abs(r1->get_y() - y) <= 1e-14);
	}
}
