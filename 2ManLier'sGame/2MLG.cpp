#include "2MLG.h"

//const int NUM_CARDS = 4;
//const int NUM_PLAYERS = 2;
//const int NUM_ACTIONS = 2;  // {0: play card, 1: call bluff}
//
//enum Card { KING = 0, QUEEN, ACE, JOKER };  
//const std::vector<Card> DECK = {KING, QUEEN, ACE, JOKER};
//
//class Node
//{
//    std::string key;
//    int actions;
//
//
//    std::vector<double> regretSum;
//    std::vector<double> strategySum;
//
//    std::vector<double> strategy;
//
//
//
//    std::vector<std::string> actionsMap;
//
//
//    float reachPB;
//    float reachPBSum;
//
//public:
//
//    Node(std::string key, std::vector<std::string> actions)
//        : key(key), actionsMap(actions),
//          regretSum(NUM_ACTIONS, 0.0),
//          strategySum(NUM_ACTIONS, 0.0),
//          strategy(NUM_ACTIONS, 1.0 / NUM_ACTIONS),
//          reachPB(0.0)
//    {}
//
//
//    void UpdateStartegy()
//    {
//        double normalizedSum = 0;
//
//        for (double r : regretSum) {
//            if (r > 0) normalizedSum += r;
//        }
//        for (int i = 0; i < NUM_ACTIONS; i++) 
//        {
//            if (normalizedSum > 0) 
//            {
//                strategy[i] = regretSum[i] / normalizedSum;
//            }
//            else {
//                strategy[i] = 1.0 / NUM_ACTIONS;
//            }
//            strategySum[i] += reachPB * strategy[i];
//        }
//        reachPB = 0.0;
//    }
//
//    std::vector<double> GetAvgStrategy()
//    {
//        double total_reach = 0;
//        for (double pr : strategySum) total_reach += pr;
//
//        std::vector<double> average_strategy(NUM_ACTIONS);
//
//        for (int i = 0; i < NUM_ACTIONS; i++) 
//        {
//            average_strategy[i] = strategySum[i] / total_reach;
//        }
//        return average_strategy;
//    }
//};
//
//class MLGTrainer
//{
//    std::unordered_map<std::string, Node*> nodeMap;
//
//    std::vector<Card> deck;
//
//    Card tablesCard;
//
//    int current_player;
//    int n_actions = NUM_ACTIONS;
//
//    double expected_game_value = 0;
//
//    std::vector<std::vector<Card>> playerHands;
//
//    std::default_random_engine gen;
//
//public:
//
//    MLGTrainer()
//    {
//        deck = DECK;
//        shuffle(deck.begin(), deck.end(), std::mt19937{ std::random_device{}() });
//
//        current_player = 0;
//        tablesCard = static_cast<Card>(rand() % 3); // out of 3 as joker cant be active card
//
//        playerHands[0].push_back(deck[0]);
//        playerHands[1].push_back(deck[1]);
//        playerHands[0].push_back(deck[2]);
//        playerHands[1].push_back(deck[3]);
//
//
//        std::random_device rd;
//        gen.seed(rd());
//    }
//
//    void display_player_hands() {
//        std::cout << "Player 1 hand: ";
//        for (Card card : playerHands[0]) {
//            std::cout << card << " ";
//        }
//        std::cout << std::endl;
//
//        std::cout << "Player 2 hand: ";
//        for (Card card : playerHands[1]) {
//            std::cout << card << " ";
//        }
//        std::cout << std::endl;
//    }
//
//    double CFR(std::string history, double p1PB, double p2PB)
//    {
//
//        int n = history.size();
//        bool is_player_1 = n % 2 == 0;
//
//        std::vector<Card>& player_hand = playerHands[is_player_1 ? 0 : 1];
//        if (is_terminal(history)) {
//            return GetReward(history, player_hand);
//        }
//
//
//        float P1_PB;
//        float P2_PB;
//
//        bool whichTurn;
//
//        int playerCards;
//
//        int act;
//
//        std::string nextHistory;
//
//        std::vector<double> regrets;
//    }
//
//
//    Node* GetNode(std::string key)
//    {
//        if (nodeMap.find(key) == nodeMap.end()) {
//            std::vector<std::string> actions = {"p1", "cb"};
//            nodeMap[key] = new Node(key, actions);
//        }
//        return nodeMap[key];
//    }
//
//    int GetReward(std::string history, std::vector<Card>&player_hand)
//    {
//        if (history.back() == 'p') {
//            return (player_hand[0] == tablesCard) ? 1.0 : -1.0;
//        }
//        return 0.0;
//    }
//
//    bool is_terminal(std::string history) {
//        return history.find("cb") != std::string::npos;
//    }
//
//    void train()
//    {
//
//    }
//
//    void DisplayRssults()
//    {
//        std::cout << "Expected game value: " << expected_game_value << std::endl;
//        for (auto& entry : nodeMap)
//        {
//            std::vector<double> avg_strategy = entry.second->GetAvgStrategy();
//            std::cout << "Node: " << entry.first << " Strategy: ";
//
//            for (double s : avg_strategy)
//            {
//                std::cout << s << " ";
//            }
//            std::cout << std::endl;
//        }
//    }
//
//    
//};

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

std::vector<double> Node::GetStrategy(double realizationWeight)
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

    auto strategy = node.GetStrategy(currPlayer == 0 ? pr1 : pr2);

    std::vector<double> util(NUM_ACTIONS);
    double nodeUtil = 0;

    for (int a = 0; a < NUM_ACTIONS; a++)
    {
        std::string nextHist = history + std::to_string(a);
        if (currPlayer == 0)
            util[a] = -cfr(nextHist, p, pr1 * strategy[a], pr2);
        else
            util[a] = -cfr(nextHist, p, pr1, pr2 * strategy[a]);

        nodeUtil += strategy[a] * util[a];
    }

    for (int a = 0; a < NUM_ACTIONS; a++)
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
