/*
 * Algorithmic Pairs Trading class
 */

#include<iostream>
#include<cmath>
#include<string>
#include "PairsTrading.h"
#include "Stock.h"
#include "utils.h"

using namespace std;

// dummy risk management and order placement modules (not part of this class?)
// E.g. they could be functions in this class

float getInvestmentAmount (string order, float diff, int count) {

    /*
        Consider:
          - Amount of money we currently have
          - How long we've been diverged
          - Difference between amount at divergence and now? (e.g. * diff/T2)

        Example function:

        Amount to invest = current$ * MIN(1, 0.001 * count);
    */

    return 0;
}

// requires order management module
void placeOrder(string order, string tick, float amount) {
}

// PairsTrading Class

PairsTrading::PairsTrading(Stock * _s1, Stock * _s2) {
    s1 = _s1;    
    s2 = _s2;

	// Gather initial EMAs
    s1Data.initialEMA = s1->getEMA();
    s2Data.initialEMA = s2->getEMA();

    // stocks are uncorrelated
    state = UNCORRELATED;
    int correlatedCount = 0; // been correlated for 0 seconds

}

PairsTrading::~PairsTrading() {
    // delete heap memory
}

// return current state
int PairsTrading::getState() {
    return state; //this->state
}

// Use absolute EMA difference to find percent change
void PairsTrading::calculateDiff() {
    s1Data.currentEMA = s1->getEMA(0);
    s2Data.currentEMA = s2->getEMA(0);
    
    // calculate percent change of each
    s1Data.percentChange = (s1Data.currentEMA - s1Data.initialEMA) / 100;
    s2Data.percentChange = (s2Data.currentEMA - s2Data.initialEMA) / 100;

    // calculate overall difference
    EMAdifference = ABS(s1Data.percentChange - s2Data.percentChange);
}

// handle state 1, UNCORRELATED
void PairsTrading::State1() {
    // need to be correlated for correlatedThreshold seconds to go to state 2
    if (EMAdifference < T1) {
        correlatedCount++;
    }
    else {
        correlatedCount = 0;
        s1Data.initialEMA = s1->getEMA(0);
        s2Data.initialEMA = s2->getEMA(0);
    }
    
    if (correlatedCount >= correlatedThreshold) {
        state = CORRELATED;

        // in case we diverge right away
        s1Data.EMAatDivergence = s1->getEMA(0);
        s1Data.EMAatDivergence = s2->getEMA(0);
    }
}

// handle state 2, CORRELATED
void PairsTrading::State2() {

    // Wait until state change
    if (EMAdifference > T2) {
        state = FAILED;
    }
    else if (EMAdifference > T1) {
        state = DIVERGED;
    }
    else {
        // update so it's always the value right before we diverge
        s1Data.EMAatDivergence = s1->getEMA(0);
        s1Data.EMAatDivergence = s2->getEMA(0);
    }
}

// handle state 3, DIVERGED (we exploit this)
void PairsTrading::State3() {

    // algorithm failure
    if (EMAdifference > T2) {
        divergedCount = 0;
        state = FAILED;
        return;
    }

    divergedCount++;

    // start buying/selling

    // if stock 1 is falling
    if (s1Data.currentEMA < s1Data.EMAatDivergence) {
        // part of Risk management module (separate class? inputs?)
        // or we could make one risk management module for just this algorithm
        float buyAmount = getInvestmentAmount ("buy",
                              s1Data.currentEMA - s1Data.EMAatDivergence,
                              divergedCount
                              );

        // part of order management module
        placeOrder("buy", s1->getTick(), buyAmount);
    }
    else { // stock 1 is rising
        float sellAmount = getInvestmentAmount ("sell",
                              s1Data.currentEMA - s1Data.EMAatDivergence,
                              divergedCount
                              );
        placeOrder("sell", s1->getTick(), sellAmount);
    }

    // if stock 2 is falling
    if (s2Data.currentEMA < s2Data.EMAatDivergence) {
        // part of Risk management module (separate class? inputs?)
        // or we could make one risk management module for just this algorithm
        float buyAmount = getInvestmentAmount ("buy",
                              s2Data.currentEMA - s2Data.EMAatDivergence,
                              divergedCount
                              );
        placeOrder("buy", s2->getTick(), buyAmount);
    }
    else { // stock 2 is rising
        float sellAmount = getInvestmentAmount ("sell",
                              s2Data.currentEMA - s2Data.EMAatDivergence,
                              divergedCount
                              );
        placeOrder("sell", s2->getTick(), sellAmount);
    }
}

// handle state 4, FAILED
void PairsTrading::State4() {
    state = 1;
    // sell everything, call methods in order management module
}

// Begin algorithmic trading
void PairsTrading::doPairsTrading() { // Called Every Timestep

    // update EMAdifference (see design documentation) and find state
    calculateDiff();

    // handle current state
    if (state == 1) State1();
    else if (state == 2) State2();
    else if (state == 3) State3();
    else State4();
}
