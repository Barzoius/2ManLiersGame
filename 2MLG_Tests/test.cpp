#include "pch.h"

#include "2ManLier'sGame\RPS.cpp"

TEST(RPSTest1, GetActionTest) {

     Trainer trainer;

     // Rock vs Scissors: Rock wins
     EXPECT_EQ(trainer.GetReward(0, 2), 1);

     // Paper vs Rock: Paper wins
     EXPECT_EQ(trainer.GetReward(1, 0), 1);

     // Scissors vs Paper: Scissors wins
     EXPECT_EQ(trainer.GetReward(2, 1), 1);

     // Rock vs Rock: tie
     EXPECT_EQ(trainer.GetReward(0, 0), 0);

     // Scissors vs Rock: Scissors loses
     EXPECT_EQ(trainer.GetReward(2, 0), -1);

     EXPECT_TRUE(true);
}

// Test GetStrategy with all non-positive regrets (uniform distribution)
TEST(RPSTrainerTest, StrategyFromZeroRegret) {
     Trainer trainer;
     std::vector<double> regrets = { -1, 0, -5 };
     auto strategy = trainer.GetStrategy(regrets);
 
     EXPECT_NEAR(strategy[0], 1.0 / 3.0, 1e-6);
     EXPECT_NEAR(strategy[1], 1.0 / 3.0, 1e-6);
     EXPECT_NEAR(strategy[2], 1.0 / 3.0, 1e-6);
 }
 
 // Test GetAvgStrategy with valid strategy sum
 TEST(RPSTrainerTest, AvgStrategyCalculation) {
     Trainer trainer;
     std::vector<double> sum = { 10, 20, 30 };
     auto avg = trainer.GetAvgStrategy(sum);
 
     EXPECT_NEAR(avg[0], 0.166666, 1e-5);
     EXPECT_NEAR(avg[1], 0.333333, 1e-5);
     EXPECT_NEAR(avg[2], 0.5, 1e-5);
 }