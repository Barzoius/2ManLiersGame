#include "KuhnPoker.h"

using namespace std;


KPNode::KPNode(string key_) : key(key_)
{
    regret_sum = vector<double>(n_actions, 0);
    strategy_sum = vector<double>(n_actions, 0);
    strategy = vector<double>(n_actions, 1.0 / n_actions);
    action_dict = { {0, 'p'}, {1, 'b'} };
}

void KPNode::update_strategy()
{
    for (int i = 0; i < n_actions; ++i) {
        strategy_sum[i] += reach_pr * strategy[i];
    }
    reach_pr_sum += reach_pr;
    strategy = GetStrategy();
    reach_pr = 0;
}

vector<double> KPNode::GetStrategy()
{
    vector<double> pos_regrets(n_actions, 0);

    for (int i = 0; i < n_actions; ++i) {
        pos_regrets[i] = max(0.0, regret_sum[i]);
    }

    double normalizing_sum = accumulate(pos_regrets.begin(), pos_regrets.end(), 0.0);

    if (normalizing_sum > 0) {
        for (int i = 0; i < n_actions; ++i) {
            pos_regrets[i] /= normalizing_sum;
        }
        return pos_regrets;
    }
    else {
        return vector<double>(n_actions, 1.0 / n_actions);
    }
}

vector<double> KPNode::GetAvgStrategy()
{
    vector<double> avg_strategy(n_actions, 0);

    for (int i = 0; i < n_actions; ++i) {
        avg_strategy[i] = strategy_sum[i] / reach_pr_sum;
    }

    double total = accumulate(avg_strategy.begin(), avg_strategy.end(), 0.0);

    for (int i = 0; i < n_actions; ++i) {
        avg_strategy[i] /= total;
    }

    return avg_strategy;
}

void KPNode::print() const
{
    vector<double> avg_strat = const_cast<KPNode*>(this)->GetAvgStrategy();

    cout << left << setw(6) << key << " ";
    for (double val : avg_strat) {
        cout << fixed << setprecision(2) << val << " ";
    }
    cout << endl;
}



KuhnTrainer::KuhnTrainer()
{
    random_device rd;
    rng = mt19937(rd());
}

void KuhnTrainer::train(int n_iterations)
{
    double expected_game_value = 0;

    for (int i = 0; i < n_iterations; ++i) {
        shuffle(deck.begin(), deck.end(), rng);
        expected_game_value += cfr("", 1, 1);

        for (auto& p : nodeMap) {
            p.second->update_strategy();
        }
    }

    expected_game_value /= n_iterations;
    display_results(expected_game_value);
}

double KuhnTrainer::cfr(string history, double pr_1, double pr_2)
{
    int n = history.length();
    bool is_player_1 = (n % 2 == 0);
    int player_card = is_player_1 ? deck[0] : deck[1];

    if (is_terminal(history))
    {
        int card_player = is_player_1 ? deck[0] : deck[1];
        int card_opponent = is_player_1 ? deck[1] : deck[0];
        return get_reward(history, card_player, card_opponent);
    }

    KPNode* node = get_node(player_card, history);
    vector<double>& strategy = node->strategy;
    vector<double> action_utils(n_actions, 0);

    for (int a = 0; a < n_actions; ++a) {
        string next_history = history + node->action_dict[a];

        if (is_player_1)
            action_utils[a] = -1 * cfr(next_history, pr_1 * strategy[a], pr_2);
        else
            action_utils[a] = -1 * cfr(next_history, pr_1, pr_2 * strategy[a]);
    }

    double util = inner_product(action_utils.begin(), action_utils.end(), strategy.begin(), 0.0);

    for (int a = 0; a < n_actions; a++) {
        double regret = action_utils[a] - util;

        if (is_player_1) {
            node->reach_pr += pr_1;
            node->regret_sum[a] += pr_2 * regret;
        }
        else {
            node->reach_pr += pr_2;
            node->regret_sum[a] += pr_1 * regret;
        }
    }

    return util;
}

bool KuhnTrainer::is_terminal(const string& history)
{
    return history.size() >= 2 &&
        (history.substr(history.size() - 2) == "pp" ||
            history.substr(history.size() - 2) == "bb" ||
            history.substr(history.size() - 2) == "bp");
}

int KuhnTrainer::get_reward(const string& history, int player_card, int opponent_card)
{
    bool terminal_pass = history.back() == 'p';
    bool double_bet = history.size() >= 2 && history.substr(history.size() - 2) == "bb";

    if (terminal_pass) {
        if (history.substr(history.size() - 2) == "pp") {
            return player_card > opponent_card ? 1 : -1;
        }
        else {
            return 1;
        }
    }
    else if (double_bet) {
        return player_card > opponent_card ? 2 : -2;
    }

    return 0;
}

KPNode* KuhnTrainer::get_node(int card, const string& history)
{
    string key = to_string(card) + " " + history;
    if (nodeMap.find(key) == nodeMap.end()) {
        nodeMap[key] = new KPNode(key);
    }
    return nodeMap[key];
}

void KuhnTrainer::display_results(double ev)
{
    cout << "player 1 expected value: " << ev << endl;
    cout << "player 2 expected value: " << -ev << endl << endl;

    cout << "player 1 strategies:" << endl;
    for (const auto& p : nodeMap) {
        if (p.first.length() % 2 == 0) {
            p.second->print();
        }
    }

    cout << "\nplayer 2 strategies:" << endl;
    for (const auto& p : nodeMap) {
        if (p.first.length() % 2 == 1) {
            p.second->print();
        }
    }
}
