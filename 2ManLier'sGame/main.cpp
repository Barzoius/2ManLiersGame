#include <iostream>
#include "RPS.cpp"

#include <chrono>


void TrainRPS()
{
    Trainer trainer;
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


int main()
{
    TrainRPS();

    return 0;
}