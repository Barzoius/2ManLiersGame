#include "pch.h"

#include "2ManLier'sGame\RPS.cpp"

#include "2ManLier'sGame\KhunPoker.cpp"

#include "2ManLier'sGame\2MLG.cpp"


#include "2ManLier'sGame\FA_RPS.h"


///===========================================================[RPS]============================================================///

TEST(RPSTrainerTest, GetRewardTest) {


    RPSTrainer trainer;

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
    RPSTrainer trainer;
    std::vector<double> regrets = { 2, 3, 5 };
    auto strategy = trainer.GetStrategy(regrets);

    EXPECT_NEAR(strategy[0], 0.2, 1e-6) << "Strategy[0] should be ~0.2 for regrets {2,3,5}"; // 2 / (2+3+5)
    EXPECT_NEAR(strategy[1], 0.3, 1e-6) << "Strategy[1] should be ~0.3 for regrets {2,3,5}";
    EXPECT_NEAR(strategy[2], 0.5, 1e-6) << "Strategy[2] should be ~0.5 for regrets {2,3,5}";
}



// Test GetStrategy with all non-positive regrets (uniform distribution)
TEST(RPSTrainerTest, StrategyFromZeroRegret) {
     RPSTrainer trainer;
     std::vector<double> regrets = { -1, 0, -5 };
     auto strategy = trainer.GetStrategy(regrets);
 
     EXPECT_NEAR(strategy[0], 1.0 / 3.0, 1e-6) << "Strategy[0] should be ~1/3 when all regrets are non-positive";
     EXPECT_NEAR(strategy[1], 1.0 / 3.0, 1e-6) << "Strategy[1] should be ~1/3 when all regrets are non-positive";
     EXPECT_NEAR(strategy[2], 1.0 / 3.0, 1e-6) << "Strategy[2] should be ~1/3 when all regrets are non-positive";
 }
 
 // Test GetAvgStrategy with valid strategy sum
 TEST(RPSTrainerTest, AvgStrategyCalculation) {
     RPSTrainer trainer;
     std::vector<double> sum = { 10, 20, 30 };
     auto avg = trainer.GetAvgStrategy(sum);
 
     EXPECT_NEAR(avg[0], 0.166666, 1e-5) << "AvgStrategy[0] should be ~0.1667 for sum {10,20,30}";
     EXPECT_NEAR(avg[1], 0.333333, 1e-5) << "AvgStrategy[1] should be ~0.3333 for sum {10,20,30}";
     EXPECT_NEAR(avg[2], 0.5, 1e-5) << "AvgStrategy[2] should be ~0.5 for sum {10,20,30}";
 }



// Test Strategy Convergence Toward Nash Equilibrium
 TEST(RPSTrainerTest, StrategyConvergesToNashEquilibrium) {
     RPSTrainer trainer;
     int iterations = 10000;  // large enough for convergence
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


 ///==========================================================[FREQAN]===========================================================///


 TEST(FrequencyBotTest, RespondsToRockSpam) {
     FrequencyBot bot;
     for (int i = 0; i < 10; ++i)
         bot.RecordOpponentMove(0); // Opponent keeps playing Rock

     EXPECT_EQ(bot.GetAction(), 1) << "Expected Paper to beat Rock";
 }

 TEST(FrequencyBotTest, RespondsToPaperSpam) {
     FrequencyBot bot;
     for (int i = 0; i < 5; ++i)
         bot.RecordOpponentMove(1); // Opponent keeps playing Paper

     EXPECT_EQ(bot.GetAction(), 2) << "Expected Scissors to beat Paper";
 }


 ///--------TESTS DONE WITH CHATGPT--------///


 // 1. Mixed Input Frequencies
 //    To ensure the bot picks the move that counters the 
 //    most frequent opponent move, even when inputs are mixed.

 TEST(FrequencyBotTest, RespondsToScissorsSpam) {
     FrequencyBot bot;
     for (int i = 0; i < 7; ++i)
         bot.RecordOpponentMove(2); // Opponent plays Scissors

     EXPECT_EQ(bot.GetAction(), 0) << "Expected Rock to beat Scissors";
 }
 TEST(FrequencyBotTest, RespondsToMixedMoves) {
     FrequencyBot bot;
     bot.RecordOpponentMove(0); // Rock x2
     bot.RecordOpponentMove(0);
     bot.RecordOpponentMove(1); // Paper x3
     bot.RecordOpponentMove(1);
     bot.RecordOpponentMove(1);
     bot.RecordOpponentMove(2); // Scissors x1

     EXPECT_EQ(bot.GetAction(), 2) << "Expected Scissors to beat most common Paper";
 }


 // 2. Tied Frequencies
 //    When multiple moves are tied in frequency, test which one the bot considers 
 //    as “most common”(likely the one with the lowest index).

 TEST(FrequencyBotTest, RespondsToTiedFrequencies) {
     FrequencyBot bot;
     bot.RecordOpponentMove(0); // Rock
     bot.RecordOpponentMove(1); // Paper
     bot.RecordOpponentMove(2); // Scissors

     EXPECT_EQ(bot.GetAction(), 1) << "Expected Paper (beats Rock) when all are tied, assuming Rock is selected by default";
 }


 // 3. No Moves Recorded Yet
 //    If GetAction() is called before any move is recorded, we should test that the bot doesn't 
 //    crash and behaves in a defined way (could return 1, because initial vector is all 
 //    zeros and max_element will return 0).

 TEST(FrequencyBotTest, NoMovesYet) {
     FrequencyBot bot;
     int action = bot.GetAction();
     EXPECT_GE(action, 0);
     EXPECT_LE(action, 2);
 }

 ///===========================================================[2MLG]============================================================///

 //TEST(MLGTest, MatchesActiveTest)
 //{
 //    EXPECT_TRUE(matchesActive("ACE", "ACE")) << "ACE should match ACE";
 //    EXPECT_TRUE(matchesActive("JOKER", "KING")) << "JOKER should match any active card";
 //    EXPECT_FALSE(matchesActive("QUEEN", "KING")) << "QUEEN shouldn't match KING";
 //}

 //TEST(MLGTest, SampleActionTest) {
 //    std::vector<double> strategy = { 0.0, 1.0 };

 //    // With full weight on action 1, sampleAction should almost always return 1
 //    int result = sampleAction(strategy);
 //    EXPECT_EQ(result, 1) << "Expected action 1 due to strategy [0.0, 1.0]";

 //    strategy = { 1.0, 0.0 };
 //    result = sampleAction(strategy);
 //    EXPECT_EQ(result, 0) << "Expected action 0 due to strategy [1.0, 0.0]";
 //}

 //TEST(MLGTest, NodeGetStrategyTest) {
 //    Node node;
 //    node.regretSum = { 10.0, 0.0 };

 //    std::vector<double> strat = node.GetStrategy(1.0, false);

 //    EXPECT_NEAR(strat[0], 1.0, 1e-6) << "Expected full strategy on action 0";
 //    EXPECT_NEAR(strat[1], 0.0, 1e-6) << "Expected zero strategy on action 1";
 //}

 //TEST(MLGTest, NodeGetAvgStrategyTest) {
 //    Node node;
 //    node.strategySum = { 30.0, 10.0 };

 //    std::vector<double> avg = node.GetAvgStrategy();

 //    EXPECT_NEAR(avg[0], 0.75, 1e-6) << "Expected 75% for action 0";
 //    EXPECT_NEAR(avg[1], 0.25, 1e-6) << "Expected 25% for action 1";
 //}



 /////==========================================================[KuhnPoker]===========================================================///


 /////---------THESE TESTS WERE CREATED USIGN CHAT GPT---------///

 //TEST(KuhnPokerTest, GetStrategyTest) {
 //    KPNode node("0");

 //    // Set some custom values for the regret sum to test strategy update
 //    node.regret_sum = { 10.0, 0.0 };

 //    // Call GetStrategy method with a realization weight
 //    std::vector<double> strategy = node.GetStrategy();

 //    // We expect the first action to have a strategy of 1 (due to positive regret)
 //    EXPECT_NEAR(strategy[0], 1.0, 1e-6) << "Expected full strategy on action 0";
 //    // The second action should have a strategy of 0 (due to no regret)
 //    EXPECT_NEAR(strategy[1], 0.0, 1e-6) << "Expected zero strategy on action 1";
 //}

 //TEST(KuhnPokerTest, GetAvgStrategyTest) {
 //    KPNode node("0");

 //    // Set some custom values for the strategy sum
 //    node.strategy_sum = { 30.0, 10.0 };
 //    node.reach_pr_sum = 40.0;

 //    std::vector<double> avgStrategy = node.GetAvgStrategy();

 //    // Calculate the expected values manually
 //    double total = 40.0; // Reach probability sum
 //    double expectedStrategy0 = 30.0 / total; // Action 0
 //    double expectedStrategy1 = 10.0 / total; // Action 1

 //    // Test the expected average strategies
 //    EXPECT_NEAR(avgStrategy[0], expectedStrategy0, 1e-6) << "Expected average strategy for action 0";
 //    EXPECT_NEAR(avgStrategy[1], expectedStrategy1, 1e-6) << "Expected average strategy for action 1";
 //}

 //TEST(KuhnPokerTest, GetRewardTest)
 //{
 //    KuhnTrainer trainer;

 //    // Simulate a "pp" history (Player 1 passes, Player 2 passes)
 //    EXPECT_EQ(trainer.get_reward("pp", 2, 1), 1) << "Expected Player 1 to win with higher card";

 //    // Simulate a "bp" history (Player 1 bets, Player 2 passes)
 //    EXPECT_EQ(trainer.get_reward("bp", 1, 2), 1) << "Expected Player 1 to win because Player 2 passed";

 //    // Simulate a "bb" history (both players bet)
 //    EXPECT_EQ(trainer.get_reward("bb", 2, 1), 2) << "Expected Player 1 to win with double bet and higher card";

 //    // Simulate a "pp" history (Player 1 passes, Player 2 passes) with lower card
 //    EXPECT_EQ(trainer.get_reward("pp", 1, 2), -1) << "Expected Player 1 to lose with lower card";
 //}




