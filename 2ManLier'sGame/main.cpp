#include <iostream>

#include "RPS.h"
#include "KuhnPoker.h"
#include "2MLG.h"

#include <chrono>


void TrainRPS()
{
    RPSTrainer trainer;
    for (int i = 10; i < 100001; i *= 10)
    {
        std::cout << "-----------------------------------------------------------------------\n";
        std::cout << "Numar iteratii: " << i << "\n";
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