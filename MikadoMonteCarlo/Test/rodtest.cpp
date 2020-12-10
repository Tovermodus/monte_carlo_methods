//
// Created by tovermodus on 12/4/20.
//

#include <gtest/gtest.h>
#include "../Rod.h"
TEST(RodTests, collisiontest1)
{
	Rod r1(0.5,0.5,0,0.05,0.3);
	Rod r2(0.4,0.4,0.3,0.05,0.3);
	Rod r3(0.4,0.4,0.4,0.05,0.3);
	ASSERT_FALSE(r1.check_collision(r2));
	ASSERT_TRUE(r2.check_collision(r3));
	ASSERT_TRUE(r1.check_collision(r3));
}

TEST(RodTests, collisiontest2)
{
	Rod r1(0.5,0.5,1.4,0.05,0.3);
	Rod r2(0.5,0.75,1.6,0.05,0.3);
	Rod r3(0.6,0.75,1.6,0.05,0.3);
	ASSERT_TRUE(r1.check_collision(r2));
	ASSERT_FALSE(r2.check_collision(r3));
	ASSERT_FALSE(r1.check_collision(r3));
}
TEST(RodTests, onTopTest)
{
	Rod r1(0.5,0.5,1.4,0.05,0.3);
	Rod r2(0.5,0.5,1.4,0.05,0.3);
	ASSERT_TRUE(r1.check_collision(r2));
}
