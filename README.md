# 2ManLier'sGame

## Contents

- The 2 Man Lier's Game
- Counterfactual Regret Minimization (CFR)
- On X & 0
- On the 2 Man Lier's Game

## The 2 Man Lier's Game

This Game is a simplified version of the Roblox game ["Lier's Club"](https://www.roblox.com/games/81080628786688/Liars-Club).
It consists of **2 players**, each dealt **4 cards** from a deck containing: 2 **KINGS**, 2 **QUEENS**, 2 **ACES**, and 2 **JOKERS** (8 cards in total).

Each round, a card type (KING, QUEEN, or ACE) is randomly chosen to be the active type for that round (JOKER cannot be chosen as the active type).

Players take turns placing up to 3 cards face down. The cards they play may either match the active type or not. Cards that match the active type are played truthfully, while any others represent a bluff. JOKERs can be played as wildcards, and always count as matching the active type (i.e., they are never considered a bluff).

After a player places their cards, the other player has two options:

1.Call Bluff – If they believe the previous player lied (i.e., played cards that did not match the active type), they can call bluff.

 - If the bluff is correctly called (i.e., at least one of the played cards was not the active type, excluding Jokers), the challenger wins the round.

 - If the bluff call was incorrect (i.e., all cards were of the active type or Jokers), the challenger loses the round.

2.Play Cards – The player may instead choose to continue the round by placing their own cards (up to 3), following the same rules.

If a player plays all their cards, the other player is forced to call bluff, since they can no longer respond with cards.