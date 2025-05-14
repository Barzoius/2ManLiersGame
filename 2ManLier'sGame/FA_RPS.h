#pragma once

#include <vector>
#include <string>

#include <random>

class FrequencyBot {
public:
    FrequencyBot() : move_counts({ 0, 0, 0 }), gen(rd()) {}

    int GetAction() {
        int most_common = std::distance(move_counts.begin(),
            std::max_element(move_counts.begin(), move_counts.end()));

        // Play the move that beats the opponent's most common move
        return (most_common + 1) % 3;
    }

    void RecordOpponentMove(int move) {
        move_counts[move]++;
    }

private:
    std::vector<int> move_counts;
    std::random_device rd;
    std::mt19937 gen;
};