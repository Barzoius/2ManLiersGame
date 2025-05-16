#include <iostream>

#include "RPS.h"
#include "FA_RPS.h"

#include "KuhnPoker.h"
#include "2MLG.h"

#include <chrono>


void TrainRPS()
{
    RPSTrainer trainer;
    for (int i = 10; i < 100001; i *= 10)
    {
        std::cout << "-----------------------------------------------------------------------\n";
        std::cout << "Iterations: " << i << "\n";
        auto start = std::chrono::high_resolution_clock::now();
        trainer.train(i);
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
        std::cout << "Training execution time: " << duration.count() << " ms\n";
        trainer.print_final_strategies();

    
    }
    std::cout << "-----------------------------------------------------------------------\n";
    
}


void TrainKhun()
{
    auto start = std::chrono::high_resolution_clock::now();

    KuhnTrainer trainer;
    trainer.train(25000);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Training execution time: " << duration.count() << " ms" << std::endl;

}


void Train2MLG()
{
    auto start = std::chrono::high_resolution_clock::now();

    MLGTrainer trainer;
    trainer.train(10000);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Training execution time: " << duration.count() << " ms" << std::endl;
}

void Simulate_FA_VS_CFR(int iterations)
{
    int ii = iterations;
    int cnt = 0;
    for (int iter = 10; iter <= iterations; iter *= 10)
        cnt++;

    std::vector<std::vector<float>> results(cnt, std::vector<float>(3));

    for (int j = 0; j < 100; j++)
    {
        int jj = 0;

        for (int iter = 10; iter < iterations + 1; iter *= 10, jj++)
        {
            
            RPSTrainer cfrBot;
            FrequencyBot freqBot;

            std::vector<int> win_counts = { 0, 0, 0 }; // [CFR wins, FreqAn wins, Draws]

            for (int i = 0; i < iter; i++)
            {
                auto strategy = cfrBot.GetStrategy(cfrBot.regretSum); // CFR strategy
                int cfr_action = cfrBot.GetAction(strategy);          // CFR move
                int freq_action = freqBot.GetAction();                // FreqAn move

                int result = cfrBot.GetReward(cfr_action, freq_action);
                // result: +1 (CFR win), 0 (draw), -1 (loss)

                // Update outcome stats
                if (result == 1) win_counts[0]++;
                else if (result == -1) win_counts[1]++;
                else win_counts[2]++;


                freqBot.RecordOpponentMove(cfr_action); // Frequency bot learns from CFR's move

                // CFR updates regret based on the single game
                for (int k = 0; k < 3; k++) {
                    double regret = cfrBot.GetReward(k, freq_action) - cfrBot.GetReward(cfr_action, freq_action);
                    cfrBot.regretSum[k] += regret;
                    cfrBot.strategySum[k] += strategy[k];
                }
            }


            //std::cout << "+---------------------------(" << "RESULTS" << ")---------------------------+\n";
            //std::cout << "\nGame Results after " << iter << " iterations:\n";
            //std::cout << "CFR Wins: " << win_counts[0] << "\n";
            //std::cout << "FrequencyBot Wins: " << win_counts[1] << "\n";
            //std::cout << "Draws: " << win_counts[2] << "\n\n";


            // --- Print CFR Strategy ---
            //double normalizingSum = std::accumulate(cfrBot.strategySum.begin(), cfrBot.strategySum.end(), 0.0);
            //std::cout << "\nFinal CFR Strategy:\n";
            //for (int i = 0; i < 3; ++i) {
            //    double avgStrategy = normalizingSum > 0 ? cfrBot.strategySum[i] / normalizingSum : 1.0 / 3;
            //    std::cout << "Action " << i << ": " << avgStrategy << "\n";
            //}

            //// --- Print FrequencyBot Strategy ---
            //std::vector<int> freqCounts = freqBot.GetObservedCounts(); 
            //int total = std::accumulate(freqCounts.begin(), freqCounts.end(), 0);
            //std::cout << "\nFinal FrequencyBot Strategy (based on CFR move frequencies):\n";
            //for (int i = 0; i < 3; ++i) {
            //    double freq = total > 0 ? static_cast<double>(freqCounts[i]) / total : 1.0 / 3;
            //    std::cout << "Action " << i << ": " << freq << "\n";
            //}

            results[jj][0] += win_counts[0];
            results[jj][1] += win_counts[1];
            results[jj][2] += win_counts[2];



            //cfrBot.print_final_strategies();
        }



        //std::cout << "+---------------------------------------------------------------+\n";
    }

    std::cout << "+---------------- CFR vs FrequencyBot Summary ----------------+\n";
    std::cout << "| Iterations |  CFR Wins  |  FreqBot Wins  | Draws           |\n";
    std::cout << "+-------------------------------------------------------------+\n";

    int iter = 10;
    for (int i = 0; i < cnt; i++, iter *= 10)
    {
        results[i][0] /= 100;
        results[i][1] /= 100;
        results[i][2] /= 100;

        std::cout << "| " << std::setw(10) << iter
            << " | " << std::setw(8) << results[i][0]
            << " | " << std::setw(13) << results[i][1]
            << " | " << std::setw(17) << results[i][2]
            << " |\n";
    }
}

void FA_VS_CFR_STRATEGIES(int iterations)
{
    RPSTrainer cfrBot;
    FrequencyBot freqBot;

    std::vector<int> win_counts = { 0, 0, 0 }; // [CFR wins, FrequencyBot wins, Draws]

    for (int i = 0; i < iterations; ++i) {
        auto strategy = cfrBot.GetStrategy(cfrBot.regretSum); // CFR strategy
        int cfr_action = cfrBot.GetAction(strategy);          // CFR move
        int freq_action = freqBot.GetAction();                // FrequencyBot move

        int result = cfrBot.GetReward(cfr_action, freq_action);

        // Update outcome stats
        if (result == 1) win_counts[0]++;
        else if (result == -1) win_counts[1]++;
        else win_counts[2]++;

        // Update both bots
        freqBot.RecordOpponentMove(cfr_action); // Frequency bot learns from CFR's move

        // CFR updates regret
        for (int k = 0; k < 3; k++) {
            double regret = cfrBot.GetReward(k, freq_action) - cfrBot.GetReward(cfr_action, freq_action);
            cfrBot.regretSum[k] += regret;
            cfrBot.strategySum[k] += strategy[k];
        }
    }

    // Print final results
    std::cout << "\nGame Results after " << iterations << " iterations:\n";
    std::cout << "CFR Wins: " << win_counts[0] << "\n";
    std::cout << "FrequencyBot Wins: " << win_counts[1] << "\n";
    std::cout << "Draws: " << win_counts[2] << "\n";

    // --- Print CFR Strategy ---
    double normalizingSum = std::accumulate(cfrBot.strategySum.begin(), cfrBot.strategySum.end(), 0.0);
    std::cout << "\nCFR Strategy:\n";
    for (int i = 0; i < 3; ++i) {
        double avgStrategy = normalizingSum > 0 ? cfrBot.strategySum[i] / normalizingSum : 1.0 / 3;
        std::cout << "Action " << i << ": " << avgStrategy << "\n";
    }

    // --- Print FrequencyBot Strategu ---
    std::vector<int> freqCounts = freqBot.GetObservedCounts(); 
    int total = std::accumulate(freqCounts.begin(), freqCounts.end(), 0);
    std::cout << "\nFrequencyBot Strategy (based on CFR move frequencies):\n";
    for (int i = 0; i < 3; ++i) {
        double freq = total > 0 ? static_cast<double>(freqCounts[i]) / total : 1.0 / 3;
        std::cout << "Action " << i << ": " << freq << "\n";
    }
}


void menu()
{
    int k = 1;

    while (true)
    {
        std::cout << "==========(MENU)==========\n";
        std::cout << "0 - CLOSE\n";
        std::cout << "1 - ROCK PAPER SCISSORS\n";
        std::cout << "2 - KHUN POKER\n";
        std::cout << "3 - 2 MAN LIER'S GAME\n";
        std::cout << "4 - CFR VS FREQA\n";

        std::cout << "=========================="<<std::endl;


        int option;

        try
        {
            std::cin >> option;
        }
        catch (const std::invalid_argument&) {
            std::cerr << "Invalid input. Please enter a number corresponding to a valid option." << std::endl;
            continue;
        }

        switch (option)
        {
            case 1:
            {
                TrainRPS();
                break;
            }
            
            case 2: {

                TrainKhun();
                break;
            }

            case 3:
            {
                Train2MLG();
                break;
            }

            case 4:
            {
                Simulate_FA_VS_CFR(10000);
                break;
            }

            case 0:
            {
                return;
                break;
            }

        }

    }
}

int main()
{
    menu();

    return 0;
}