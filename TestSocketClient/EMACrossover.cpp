/*
 * Algorithmic Pairs Trading class
 */

#include<iostream>
#include<cmath>
#include<string>
#include "Stock.h"
#include "EMACrossover.h"


using namespace std;

//getInvestment amount and placeOrder assumed. I don't see any way for me to get valid checks. 
EMACrossover::EMACrossover(Stock *_s, int _macdID, int _orderSize) : s(_s), macdID(_macdID), curState(INVALID), curMACD(0), orderSize(_orderSize), withStop(false) {
}

EMACrossover::EMACrossover(Stock *_s, int _macdID, int _orderSize, double _d1, double _d2)
	: s(_s), macdID(_macdID), curState(INVALID), curMACD(0), orderSize(_orderSize), d1(_d1), d2(_d2), withStop(true) {
}

EMACrossover::~EMACrossover() {
}

// return current state
state EMACrossover::getState() {
    return curState; 
}

void EMACrossover::doEMACrossover() {
	MACD *macd = s->getMACD(macdID);
	double pi = atan((double) 1) * 4;

	//check if values are valid
	if (!macd->isValid()) {
		return;
	}

	double curPrice = s->getPrice();
	double fast = macd->getFast();
	double slow = macd->getSlow();
	double degrees = atan(fast/slow) * pi * 180;

	if (withStop) {
		if (curPrice > stopWin(fast, d1)) {
			//placeOrder("buy", orderSize);
			return;
		} else if (curPrice < stopLoss(slow, d2)) {
			/*placeOrder("sell", orderSize);*/
			return;
		}
	}

	if (s->getMACD(macdID) > 0) {
		if (curState == FAST_ABOVE_SLOW) {
			return; //no news 
		} else {
			curState = FAST_ABOVE_SLOW;
			//time to buy!
			//placeOrder("buy", orderAmount);
		}
	} else {
		if (curState == FAST_BELOW_SLOW) {
			return; //no news
		} else {
			curState = FAST_BELOW_SLOW;
			//time to sell!
			//placeorder("sell", orderamount);
		}
	}

	return;
}

double riskManagement(double degrees) {
	double money = 2;
	return (degrees/180 * money * 0.01);
}

double EMACrossover::stopWin(double fast, double  d1) {
	return fast+d1;
}

double EMACrossover::stopLoss(double slow, double d2)  {
	return slow-d2;
}