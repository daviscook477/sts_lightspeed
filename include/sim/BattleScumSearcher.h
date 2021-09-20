//
// Created by gamerpuppy on 9/6/2021.
//

#ifndef STS_LIGHTSPEED_BATTLESCUMSEARCHER_H
#define STS_LIGHTSPEED_BATTLESCUMSEARCHER_H

#include "combat/BattleContext.h"
#include "sim/StateHandler.h"

#include <iostream>

namespace sts {

    struct StateValue {
        double score = 0;
        int playerHp;
        int combinedHp;

        bool operator<(const StateValue &rhs) const;
        bool operator>(const StateValue &rhs) const;
        bool operator<=(const StateValue &rhs) const;
        bool operator>=(const StateValue &rhs) const;

        friend std::ostream &operator<<(std::ostream &os, const StateValue &value);
    };

    struct SearchInfo {
        BattleContext bc;
        int optionIdx = -1;
        int stateSize = 0;
        RandomBattleStateHandler handler;
        StateValue value; // equal to the cur value or the best state resulting from itself
    };

    struct BattleScumSearcher {
        std::vector<SearchInfo> bestInfos;
        std::int64_t nodesEvaluated = 0;

        std::default_random_engine randEngine;
        // above this sample from the state instead of searching every branch
        // mainly for gamblers brew / elixir potion where a set of cards are selected
        static constexpr int stateSizeSampleThreshold = 11;
        static constexpr int stateSampleCount = 10;

        int maxMoveDepth = 5;
        int minTurnLookahead = 1;

        BattleScumSearcher(int maxMoveDepth, int minTurnLookahead);

        void search(const BattleContext &bc);
        // ************

        static StateValue evaluateState(const BattleContext &bc);
    };

    struct ScumSearcherAgent {
        bool printLogs = false;
        int searchDepth = 3;
        int minTurnLookahead;

        int bossSearchMultiplier = 3;

        std::default_random_engine rng;
        int choiceCount = 0;

        std::int64_t nodesEvaluated = 0;

        explicit ScumSearcherAgent(const std::default_random_engine &rng, int searchDepth);

        void playout(GameContext &gc);
        void playoutBattle(BattleContext &gc, bool print=false);


        fixed_list<MonsterEncounter, 10> getEncountersToTest(const GameContext &gc);
        double evaluateFights(const GameContext &gc, const fixed_list<MonsterEncounter, 10> &encounters);
        void chooseRandom(GameContext &gc);
        void chooseRewardAction(GameContext &gc);
        void pickGoodEventOutcome(GameContext &gc);

    };


    std::ostream& operator<<(std::ostream &os, const SearchInfo &s);
}

#endif //STS_LIGHTSPEED_BATTLESCUMSEARCHER_H
