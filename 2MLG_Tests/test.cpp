#include "pch.h"

#include "2ManLier'sGame\RPS.cpp"

TEST(RPSTest1, GetActionTest) {

     Trainer trainer;

     // Rock vs Scissors: Rock wins
     EXPECT_EQ(trainer.GetReward(0, 2), 1);

     // Paper vs Rock: Paper wins
     EXPECT_EQ(trainer.GetReward(1, 0), 1);

     // Scissors vs Paper: Scissors wins
     EXPECT_EQ(trainer.GetReward(2, 1), 1);

     // Rock vs Rock: tie
     EXPECT_EQ(trainer.GetReward(0, 0), 0);

     // Scissors vs Rock: Scissors loses
     EXPECT_EQ(trainer.GetReward(2, 0), -1);

     EXPECT_TRUE(true);
     

}