#include <iostream>
#include <vector>

#include <random>
#include <numeric>

class Trainer {
private:
    int actions = 3;
    std::vector<double> posActions = { 1,2,3 };

    std::vector<std::vector<int>> actionReward = { {0, -1, 1},
                                                   {1, 0, -1},
                                                   {-1, 1, 0} };



    std::vector<double> regretSum = { 0, 0, 0 };

    std::vector<double> oppRegretSum = { 0, 0, 0 };




    std::vector<double> strategySum = { 0, 0, 0 };

    std::vector<double> oppStrategySum = { 0, 0, 0 };

    std::default_random_engine gen;

public:

    Trainer() {
        std::random_device rd;
        gen.seed(rd());
    };

    std::vector<double> GetStrategy(std::vector<double> regret) {
        std::vector<double> strategy = { 0, 0, 0 };
        double normalizing_sum = 0;


        
        for (int i = 0; i < actions; i++) {
            strategy[i] = regret[i] > 0 ? regret[i] : 0;
            normalizing_sum += strategy[i];
        }

        if (normalizing_sum > 0) {
            for (int i = 0; i < actions; ++i)
                strategy[i] /= normalizing_sum;
        }
        else {
            for (int i = 0; i < actions; ++i)
                strategy[i] = 1.0 / actions;
        }

        return strategy;
    }


    std::vector<double> GetAvgStrategy(const std::vector<double>& strategy_sum) {
        std::vector<double> avgStrategy = { 0, 0, 0 };
        double normalizing_sum = std::accumulate(strategy_sum.begin(), strategy_sum.end(), 0.0);

        for (int i = 0; i < actions; i++) 
        {
            if (normalizing_sum > 0)
                avgStrategy[i] = strategy_sum[i] / normalizing_sum;
            else
                avgStrategy[i] = 1.0 / actions;
        }

        return avgStrategy;
    }



    int GetAction(const std::vector<double>& strategy) {
        std::discrete_distribution<int> dist(strategy.begin(), strategy.end());
        return dist(gen);
    }

    int GetReward(int my_action, int opponent_action) {
        return actionReward[my_action][opponent_action];
    }

    void train(int iterations) {
        for (int i = 0; i < iterations; i++) {
            auto strategy = GetStrategy(regretSum);
            auto opp_strategy = GetStrategy(oppRegretSum);

            for (int j = 0; j < actions; j++) {
                strategySum[j] += strategy[j];
                oppStrategySum[j] += opp_strategy[j];
            }

            int my_action = GetAction(strategy);
            int opp_action = GetAction(opp_strategy);

            int my_reward = GetReward(my_action, opp_action);
            int opp_reward = GetReward(opp_action, my_action);

            for (int k = 0; k < actions; k++) {
                double my_regret = GetReward(k, opp_action) - my_reward;
                double opp_regret = GetReward(k, my_action) - opp_reward;
                regretSum[k] += my_regret;
                oppRegretSum[k] += opp_regret;
            }
        }
    }

    void print_strategy(const std::string& name, const std::vector < double >& strategy) {
        std::cout << name << " policy: [";
        for (int i = 0; i < actions; ++i) 
        {
            std::cout << strategy[i];
            if (i < actions - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    void print_final_strategies() {
        print_strategy("Player 1", GetAvgStrategy(strategySum));
        print_strategy("Player 2", GetAvgStrategy(oppStrategySum));
    }

};