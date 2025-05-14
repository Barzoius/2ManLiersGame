#ifndef MLG_TRAINER_H
#define MLG_TRAINER_H

#include <vector>
#include <string>
#include <random>
#include <map>
#include <numeric>
#include <algorithm>
#include <iostream>

const std::vector<std::string> CARDS = { "KING", "ACE", "JOKER", "QUEEN" };
const int NUM_ACTIONS = 2; // 0 = Continue, 1 = Call Bluff

bool matchesActive(const std::string& card, const std::string& active);

int sampleAction(const std::vector<double>& strategy);

struct Node
{
    std::string infoSet;
    std::vector<double> regretSum = { 0, 0 };
    std::vector<double> strategy = { 0.5, 0.5 };
    std::vector<double> strategySum = { 0, 0 };

    std::vector<double> GetStrategy(double realizationWeight, bool isFirstMove);
    std::vector<double> GetAvgStrategy() const;
};

class MLGTrainer
{
private:
    std::map<std::string, Node> nodeMap;

    std::vector<std::string> deck;
    std::string activeCard;
    std::vector<std::string> playerHands[2];

    double cfr(std::string history, int p, double pr1, double pr2);
    void printStrategies();

public:
    void train(int iterations);
};

#endif 
