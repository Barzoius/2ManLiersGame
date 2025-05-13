#ifndef KUHN_TRAINER_H
#define KUHN_TRAINER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <numeric>
#include <random>
#include <array>
#include <iomanip>
#include <algorithm>

class KPNode
{
public:
    std::string key;
    int n_actions = 2;

    std::vector<double> regret_sum;
    std::vector<double> strategy_sum;
    std::vector<double> strategy;

    std::unordered_map<int, char> action_dict;

    double reach_pr = 0;
    double reach_pr_sum = 0;

    KPNode(std::string key_);


    void update_strategy();
    std::vector<double> GetStrategy();
    std::vector<double> GetAvgStrategy();
    void print() const;
};

class KuhnTrainer
{
public:
    KuhnTrainer();

    void train(int n_iterations = 50000);

private:
    std::unordered_map<std::string, KPNode*> nodeMap;
    std::array<int, 3> deck = { 0, 1, 2 };
    int n_actions = 2;
    std::mt19937 rng;

public:
    double cfr(std::string history, double pr_1, double pr_2);
    bool is_terminal(const std::string& history);
    int get_reward(const std::string& history, int player_card, int opponent_card);
    KPNode* get_node(int card, const std::string& history);
    void display_results(double ev);
};

#endif 