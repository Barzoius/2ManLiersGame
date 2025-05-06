#include "pch.h"

#include "2ManLier'sGame\RPS.cpp"

TEST(RPSTrainerTest, GetRewardTest) {


    Trainer trainer;

    // Rock vs Scissors: Rock wins
    EXPECT_EQ(trainer.GetReward(0, 2), 1) << "Expected Rock to beat Scissors";

    // Paper vs Rock: Paper wins
    EXPECT_EQ(trainer.GetReward(1, 0), 1) << "Expected Paper to beat Rock";

    // Scissors vs Paper: Scissors wins
    EXPECT_EQ(trainer.GetReward(2, 1), 1) << "Expected Scissors to beat Paper";

    // Rock vs Rock: tie
    EXPECT_EQ(trainer.GetReward(0, 0), 0) << "Rock vs Rock, expected Tie";

    // Scissors vs Rock: Scissors loses
    EXPECT_EQ(trainer.GetReward(2, 0), -1) << "Expected Scissors to lose to Rock ";

    EXPECT_TRUE(true);


}

// Test GetStrategy with positive regrets
TEST(RPSTrainerTest, StrategyFromPositiveRegret) {
    Trainer trainer;
    std::vector<double> regrets = { 2, 3, 5 };
    auto strategy = trainer.GetStrategy(regrets);

    EXPECT_NEAR(strategy[0], 0.2, 1e-6) << "Strategy[0] should be ~0.2 for regrets {2,3,5}"; // 2 / (2+3+5)
    EXPECT_NEAR(strategy[1], 0.3, 1e-6) << "Strategy[1] should be ~0.3 for regrets {2,3,5}";
    EXPECT_NEAR(strategy[2], 0.5, 1e-6) << "Strategy[2] should be ~0.5 for regrets {2,3,5}";
}



// Test GetStrategy with all non-positive regrets (uniform distribution)
TEST(RPSTrainerTest, StrategyFromZeroRegret) {
     Trainer trainer;
     std::vector<double> regrets = { -1, 0, -5 };
     auto strategy = trainer.GetStrategy(regrets);
 
     EXPECT_NEAR(strategy[0], 1.0 / 3.0, 1e-6) << "Strategy[0] should be ~1/3 when all regrets are non-positive";
     EXPECT_NEAR(strategy[1], 1.0 / 3.0, 1e-6) << "Strategy[1] should be ~1/3 when all regrets are non-positive";
     EXPECT_NEAR(strategy[2], 1.0 / 3.0, 1e-6) << "Strategy[2] should be ~1/3 when all regrets are non-positive";
 }
 
 // Test GetAvgStrategy with valid strategy sum
 TEST(RPSTrainerTest, AvgStrategyCalculation) {
     Trainer trainer;
     std::vector<double> sum = { 10, 20, 30 };
     auto avg = trainer.GetAvgStrategy(sum);
 
     EXPECT_NEAR(avg[0], 0.166666, 1e-5) << "AvgStrategy[0] should be ~0.1667 for sum {10,20,30}";
     EXPECT_NEAR(avg[1], 0.333333, 1e-5) << "AvgStrategy[1] should be ~0.3333 for sum {10,20,30}";
     EXPECT_NEAR(avg[2], 0.5, 1e-5) << "AvgStrategy[2] should be ~0.5 for sum {10,20,30}";
 }



// Test Strategy Convergence Toward Nash Equilibrium
 TEST(RPSTrainerTest, StrategyConvergesToNashEquilibrium) {
     Trainer trainer;
     int iterations = 100000;  // large enough for convergence
     trainer.train(iterations);

     auto p1_avg = trainer.GetAvgStrategy({ trainer.strategySum[0], trainer.strategySum[1], trainer.strategySum[2] });
     auto p2_avg = trainer.GetAvgStrategy({ trainer.oppStrategySum[0], trainer.oppStrategySum[1], trainer.oppStrategySum[2] });

     double expected = 1.0 / 3.0;
     double tolerance = 0.05; // acceptable margin of error for convergence

     SCOPED_TRACE("Checking if Player 1 strategy converges toward uniform distribution.");
     EXPECT_NEAR(p1_avg[0], expected, tolerance) << "Player 1's strategy[0] should approach 1/3 after training";
     EXPECT_NEAR(p1_avg[1], expected, tolerance) << "Player 1's strategy[1] should approach 1/3 after training";
     EXPECT_NEAR(p1_avg[2], expected, tolerance) << "Player 1's strategy[2] should approach 1/3 after training";

     SCOPED_TRACE("Checking if Player 2 strategy converges toward uniform distribution.");
     EXPECT_NEAR(p2_avg[0], expected, tolerance) << "Player 2's strategy[0] should approach 1/3 after training";
     EXPECT_NEAR(p2_avg[1], expected, tolerance) << "Player 2's strategy[1] should approach 1/3 after training";
     EXPECT_NEAR(p2_avg[2], expected, tolerance) << "Player 2's strategy[2] should approach 1/3 after training";
 }
