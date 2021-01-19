//
// Created by tovermodus on 12/4/20.
//

#include <gtest/gtest.h>
#include "../Rod.h"
TEST(RodTests, collisiontest1)
{
	std::shared_ptr<Rod> r1 = std::make_shared<Rod>(0.5,0.5,0,0.05,0.3, nullptr);
	std::shared_ptr<Rod> r2 = std::make_shared<Rod>(0.4,0.4,0.3,0.05,0.3,nullptr);
	std::shared_ptr<Rod> r3 = std::make_shared<Rod>(0.4,0.4,0.4,0.05,0.3,nullptr);
	ASSERT_FALSE(r1->check_collision(r2));
	ASSERT_TRUE(r2->check_collision(r3));
	ASSERT_TRUE(r1->check_collision(r3));
}

TEST(RodTests, collisiontest2)
{
	std::shared_ptr<Rod> r1 = std::make_shared<Rod>(0.5,0.5,1.4,0.05,0.3,nullptr);
	std::shared_ptr<Rod> r2 = std::make_shared<Rod>(0.5, 0.75, 1.6, 0.05, 0.3, nullptr);
	std::shared_ptr<Rod> r3 = std::make_shared<Rod>(0.6, 0.75, 1.6, 0.05, 0.3, nullptr);
	ASSERT_TRUE(r1->check_collision(r2));
	ASSERT_FALSE(r2->check_collision(r3));
	ASSERT_FALSE(r1->check_collision(r3));
}
TEST(RodTests, onTopTest)
{
	std::shared_ptr<Rod> r1 = std::make_shared<Rod>(0.5,0.5,1.4,0.05,0.3,nullptr);
	std::shared_ptr<Rod> r2 = std::make_shared<Rod>(0.5,0.5,1.4,0.05,0.3,nullptr);
	ASSERT_TRUE(r1->check_collision(r2));
}
