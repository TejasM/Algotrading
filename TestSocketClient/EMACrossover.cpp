/*
 * Algorithmic Pairs Trading class
 */

#include<iostream>
#include<cmath>
#include<string>
#include "EMACrossover.h"
#include "Stock.h"

using namespace std;

//getInvestment amount and placeOrder assumed. I don't see any way for me to get valid checks. 

EMACrossover::EMACrossover(Stock *_s, int _macdID, int _orderSize) : s(_s), macdID(_macdID), curState(INVALID), curMACD(0), orderSize(_orderSize) {
}

EMACrossover::~EMACrossover() {
}

// return current state
state EMACrossover::getState() {
    return curState; 
}

void EMACrossover::doEMACrossover() {
	double macd = s->getMACD(macdID);

	//check if values are valid, I'm not sure how the interface for this works
	if (macd == INVALID)
		return;

	double curPrice = s->getPrice();
	//how the hell do I get fast and slow based on the currrent interface??? I also need them to calculate degrees!
	if (curPrice > stopWin(fast,d1) {
		placeOrder("buy", orderAmount);
		terminate();
	else if (price < stoLoss(slow, d2) {
		placeOrder("sell", orderAmount);
		terminate();
	}

	if (s->getMACD(macdID) > 0) {
		if (curState == FAST_ABOVE_SLOW) {
			return; //no news 
		} else {
			curState = FAST_ABOVE_SLOW;
			//time to buy!
			double degree = 0; //I'd need so much more information to get this...
			placeOrder("buy", orderAmount);
			terminate();
		}
	} else {
		if (curState == FAST_BELOW_SLOW) {
			return; //no news
		} else {
			curState = FAST_BELOW_SLOW;
			//time to sell!
			placeOrder("sell", orderAmount);
			terminate();
		}
	}

	return;
}

EMACrossover::stopWin(fast, d1) {
	return fast+d1;
}

EMACrossover::stopLoss(slow, d2) 
	return slow-d2;
}

void EMACrossover::terminate() {
	return;
}