#include "2MLG.h"


std::random_device rd;
std::mt19937 gen(rd());

bool matchesActive(const std::string& card, const std::string& active) {
    return card == active || card == "JOKER";
}

int sampleAction(const std::vector<double>& strategy) {
    std::discrete_distribution<> dist(strategy.begin(), strategy.end());
    return dist(gen);
}

std::vector<double> Node::GetStrategy(double realizationWeight, bool isFirstMove)
{
    double normalizingSum = 0.0;

    for (int a = 0; a < NUM_ACTIONS; a++)
    {
        strategy[a] = regretSum[a] > 0 ? regretSum[a] : 0;
        normalizingSum += strategy[a];
    }

    for (int a = 0; a < NUM_ACTIONS; a++)
    {
        if (normalizingSum > 0)
            strategy[a] /= normalizingSum;
        else
            strategy[a] = 1.0 / NUM_ACTIONS;

        if (isFirstMove && a == 1)  // Force bluff = 0
            strategy[a] = 0.0;

        strategySum[a] += realizationWeight * strategy[a];
    }
    return strategy;
}

std::vector<double> Node::GetAvgStrategy() const
{
    std::vector<double> avgStrategy(NUM_ACTIONS);

    double total = std::accumulate(strategySum.begin(), strategySum.end(), 0.0);
    

    for (int a = 0; a < NUM_ACTIONS; a++)
    {
        avgStrategy[a] = total > 0 ? strategySum[a] / total : 1.0 / NUM_ACTIONS;
    }

    return avgStrategy;
}

double MLGTrainer::cfr(std::string history, int p, double pr1, double pr2)
{
    int plays = history.size();
    int currPlayer = plays % 2;

    if (plays >= 3) { // auto-call at end

        std::string playedCard = history.substr(history.size() - 1, 1);
        std::string realCard = playerHands[currPlayer][(plays - 1) / 2];

        bool isMatch = matchesActive(realCard, activeCard);
        return (isMatch ? 1 : -1) * (currPlayer == p ? 1 : -1);
    }

    std::string infoSet = playerHands[currPlayer][0] + playerHands[currPlayer][1] + history;
    Node& node = nodeMap[infoSet];

    bool isFirstMove = history.empty();

    auto strategy = node.GetStrategy(currPlayer == 0 ? pr1 : pr2, isFirstMove);

    std::vector<double> util(NUM_ACTIONS);
    double nodeUtil = 0;

    int validActions = (history.empty()) ? 1 : NUM_ACTIONS;

    for (int a = 0; a < validActions; a++)
    {
        std::string nextHist = history + std::to_string(a);
        if (currPlayer == 0)
            util[a] = -cfr(nextHist, p, pr1 * strategy[a], pr2);
        else
            util[a] = -cfr(nextHist, p, pr1, pr2 * strategy[a]);

        nodeUtil += strategy[a] * util[a];
    }

    for (int a = 0; a < validActions; a++)
    {
        double regret = util[a] - nodeUtil;
        node.regretSum[a] += (currPlayer == 0 ? pr2 : pr1) * regret;
    }

    return nodeUtil;
}

void MLGTrainer::train(int iterations)
{
    double util = 0;

    for (int i = 0; i < iterations; i++)
    {
        deck = CARDS;
        std::shuffle(deck.begin(), deck.end(), gen);
        activeCard = CARDS[gen() % 4];

        // Deal 2 cards to each player
        playerHands[0] = { deck[0], deck[2] };
        playerHands[1] = { deck[1], deck[3] };

        util += cfr("", 0, 1, 1);
    }

    std::cout << "Average game value for player 1: " << util / iterations << "\n\n";
    std::cout << "Average game value for player 2: " << -(util / iterations) << "\n\n";


    printStrategies();
}

void MLGTrainer::printStrategies()
{
    std::cout << "\n--- Player 1 Strategies ---\n";
    for (const auto& [key, node] : nodeMap) {
        int historyLength = key.size() - 4; // 4 characters for the 2 cards
        int currPlayer = historyLength % 2;

        if (currPlayer == 0) { // Player 1's turn
            auto strat = node.GetAvgStrategy();
            std::cout << key << " : [";
            for (int i = 0; i < NUM_ACTIONS; ++i) {
                std::cout << strat[i];
                if (i < NUM_ACTIONS - 1) std::cout << ", ";
            }
            std::cout << "]\n";
        }
    }

    std::cout << "\n--- Player 2 Strategies ---\n";
    for (const auto& [key, node] : nodeMap) {
        int historyLength = key.size() - 4;
        int currPlayer = historyLength % 2;

        if (currPlayer == 1) { // Player 2's turn
            auto strat = node.GetAvgStrategy();
            std::cout << key << " : [";
            for (int i = 0; i < NUM_ACTIONS; ++i) {
                std::cout << strat[i];
                if (i < NUM_ACTIONS - 1) std::cout << ", ";
            }
            std::cout << "]\n";
        }
    }
}
