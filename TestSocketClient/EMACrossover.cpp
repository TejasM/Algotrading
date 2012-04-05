/*
 * Algorithmic Pairs Trading class
 */

#include<iostream>
#include<cmath>
#include<string>
#include "Stock.h"
#include "EMACrossover.h"

using namespace std;

//print frequency of states

//getInvestment amount and placeOrder assumed. I don't see any way for me to get valid checks. 
EMACrossover::EMACrossover(Stock *_s, int _macdID, int _orderSize) : s(_s), macd(s->getMACD(_macdID)), curState(INVALID), curMACD(0), orderSize(_orderSize), withStop(false) {
	initCommon();
}

EMACrossover::EMACrossover(Stock *_s, int _macdID, int _orderSize, double _d1, double _d2)
	: s(_s), macd(s->getMACD(_macdID)), curState(INVALID), curMACD(0), orderSize(_orderSize), d1(_d1), d2(_d2), withStop(true) {
	initCommon();
}

void EMACrossover::initCommon() {
	// Create and write to files
	string filename;

	// Price, ofstream name fPrice, filename RIM_Price.txt
	filename = s->getTick() + "_Crossover.txt";
	emacFile.open(filename.c_str());
	emacFile << "Current State is " << curState << endl;

	inEntry = false;
}

EMACrossover::~EMACrossover() {
}

// return current state
state EMACrossover::getState() {
    return curState; 
}

void EMACrossover::doEMACrossover() {
	//check if values are valid
	if (!macd->isValid()) {
		return;
	}

	double curPrice = s->getPrice();
	double fast = macd->getFast();
	double slow = macd->getSlow();
	double degrees = riskManagement();

	//init. could probably get rid of invalid and move this to constructor
	if (curState == INVALID) {
		curState = macd->getMACD() > 0 ? FAST_ABOVE_SLOW : FAST_BELOW_SLOW;
		return;
	}

	if (withStop) {
		if (curPrice > stopWin(fast, d1) && inEntry ) {
			emacFile << "STOP WIN: Selling order " << orderSize << " at price " << s->getPrice() << endl;
			//placeOrder("buy", orderSize);
			return;
		} else if (curPrice < stopLoss(slow, d2) && inEntry) {
			emacFile << "STOP LOSS: Selling order " << orderSize << " at price " << s->getPrice() << endl;
			/*placeOrder("sell", orderSize);*/
			return;
		}
	}

	//recall macd is fast - slow
	if (macd->getMACD() > 0) {
		if (curState == FAST_ABOVE_SLOW) {
			return; //no news 
		} else {
			curState = FAST_ABOVE_SLOW;
			inEntry = !inEntry;
			emacFile << "Strength of crossover is " << degrees << endl;
			//time to buy!
			//placeOrder("buy", orderAmount);
		}
	} else {
		if (curState == FAST_BELOW_SLOW) {
			return; //no news
		} else {
			curState = FAST_BELOW_SLOW;
			inEntry = !inEntry;
			emacFile << "Strength of crossover is " << degrees << endl;
			//time to sell or short if we can!
			if (s->isShortable()) {
				//placeOrder("short", orderSize * 2);
				emacFile << "Shorting order " << orderSize *2 << " at price " << s->getPrice() << endl;
			} else {
				//placeorder("sell", orderSize);
				emacFile << "Selling order " << orderSize  << " at price " << s->getPrice() << endl;
				//I'm assuming a sell order that you don't won't do anything so its harmless to call
			}
		}
	}

	return;
}

//returns the crossover strength in degres. You can specify what a "strong enough" crossover is.
double EMACrossover::riskManagement() { 
	double fast = macd->getFast();
	double slow = macd->getSlow();
	double angle = macd->getMACD() > 0 ? atan(fast/slow) : atan(slow/fast);
	double pi = atan((double) 1) * 4;
	double degrees = angle * pi / 180;
	return degrees;
}

double EMACrossover::stopWin(double fast, double  d1) {
	return fast + d1;
}

double EMACrossover::stopLoss(double slow, double d2)  {
	return slow - d2;
}