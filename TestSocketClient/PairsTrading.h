/*
 * Algorithmic Pairs Trading class
 */

#ifndef PAIRSTRADING_H
#define PAIRSTRADING_H

#include "Stock.h"
#include "utils.h"

// See Design Document for threshold terminology
#define T1 1
#define T2 2

// The state the pair is currently in
#define UNCORRELATED 1
#define CORRELATED 2
#define DIVERGED 3
#define FAILED 4

// timesteps we need to be correlated
#define correlatedThreshold 10

// PairsTrading class

class PairsTrading {
    public:
        PairsTrading(Stock * _s1, Stock * _s2);
        ~PairsTrading();
        int getState(); // not used
        void doPairsTrading();

    private:
        // s1 and s2 form an algorithmic trading pair
        Stock * s1, * s2;

        // store info for each stock
        struct StockInfo {
            double initialEMA; // at start of state 1
            double currentEMA;
            double percentChange;
            double EMAatDivergence;
        };

        StockInfo s1Data, s2Data;

        int state;
        int correlatedCount; // timesteps we've been correlated
        int divergedCount; // timesteps we've been diverged

        double EMAdifference; // current EMA difference (%)

        void calculateDiff();
        void State1();
        void State2();
        void State3();
        void State4();
};

#endif
