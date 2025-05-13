#ifndef RPS_H
#define RPS_H

#include <vector>
#include <string>
#include <random>

class RPSTrainer
{
private:
    int actions = 3;
    std::vector<double> posActions = { 1, 2, 3 };

    // Payoff matrix for Rock-Paper-Scissors
    // actionReward[i][j] = result of i vs j (1 if win, -1 if lose, 0 if tie)
    std::vector<std::vector<int>> actionReward = {
        {0, -1, 1},     // Rock
        {1,  0, -1},    // Paper
        {-1, 1, 0}      // Scissors
    };

public:
    // Cumulative regret for each action 
    std::vector<double> regretSum = { 0, 0, 0 };
    std::vector<double> oppRegretSum = { 0, 0, 0 };

    // Sum of strategies chosen over time (used to calculate average strategy)
    std::vector<double> strategySum = { 0, 0, 0 };
    std::vector<double> oppStrategySum = { 0, 0, 0 };

    std::default_random_engine gen;

public:
    std::vector<double> GetStrategy(std::vector<double> regret);
    std::vector<double> GetAvgStrategy(const std::vector<double>& strategy_sum);
    int GetAction(const std::vector<double>& strategy);
    int GetReward(int my_action, int opponent_action);
    void print_strategy(const std::string& name, const std::vector<double>& strategy);

public:
    RPSTrainer();
    void train(int iterations);
    void print_final_strategies();
};

#endif 
