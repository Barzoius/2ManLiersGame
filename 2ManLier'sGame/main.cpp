#include <iostream>
#include "TicTacToe.cpp"

int main()
{
    Trainer trainer;
    trainer.train(10000);
    trainer.print_final_strategies();
    return 0;
}