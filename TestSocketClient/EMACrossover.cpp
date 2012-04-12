/*
 * Algorithmic Pairs Trading class
 */

#include<iostream>
#include<cmath>
#include<string>
#include "Stock.h"
#include "EMACrossover.h"
#include "PairsTrading.h"
#include "Contract.h"
#include "EClientSocket.h" 
#include "Order.h"


EMACrossover::EMACrossover(Stock *_s, int _macdID, int _orderSize) 
	: s(_s), 
	macd(s->getMACD(_macdID)), 
	curState(INVALID), 
	orderSize(_orderSize) 
{
	initCommon();
}

EMACrossover::EMACrossover(Stock *_s, int _macdID, int _orderSize, double _d1, double _d2)
	: s(_s), 
	macd(s->getMACD(_macdID)),
	curState(INVALID), 
	orderSize(_orderSize),
	d1(_d1),
	d2(_d2)
{
	initCommon();
}

const char* states[4] = {"WAITING_FOR_STOP",
				  "FAST_ABOVE_SLOW",
				  "FAST_BELOW_SLOW",
				  "INVALID"};
void EMACrossover::initCommon() {
	// Create and write to files
	std::string filename;
	filename = s->getTick() + "_Crossover.txt";
	emacFile.open(filename.c_str());
	emacFile << "Current State is " << states[curState] << std::endl;
	prevFast = -1;
	prevSlow = -1;
	amountBought = 0;
}

EMACrossover::~EMACrossover() {
}

state EMACrossover::getState() {
    return curState; 
}

void EMACrossover::doEMACrossoverWithStop(double current_money, void *m_pclient) {
	
	emacFile << "Curstate is " << states[getState()] << std::endl;

	//check if values are valid
	if (!macd->isValid()) {
		return;
	}
	
	double curPrice = s->getPrice();
	double degrees = riskManagement();

	//init. can't do in constructor b/c no macd valid
	if (curState == INVALID) {
		curState = macd->getMACD() > 0 ? FAST_ABOVE_SLOW : FAST_BELOW_SLOW;
		prevFast = macd->getFast();
		prevSlow = macd->getSlow();
		return;
	}

	if (curState == WAITING_FOR_STOP) {
		if (curPrice > stopWin(macd->getFast(), d1)) {
			emacFile << "STOP WIN: Selling order " << orderSize << " at price " << s->getPrice() <<std::endl;
			s->placeOrder("SELL", orderSize*(s->getPrice()), m_pclient, (double &) amountBought);
			return;
		} else if (curPrice < stopLoss(macd->getSlow(), d2)) {
			emacFile << "STOP LOSS: Selling order " << orderSize << " at price " << s->getPrice() <<std::endl;
			s->placeOrder("SELL", orderSize*(s->getPrice()), m_pclient, (double &) amountBought);
			return;
		}
		return;
	}

	//recall macd is fast - slow
	if (macd->getMACD() > 0) {
		if (curState == FAST_ABOVE_SLOW) {
			return; //no news 
		} else {
			curState = WAITING_FOR_STOP;
			emacFile << "Strength of crossover is " << degrees << std::endl;
			//time to buy!
			s->placeOrder("BUY", orderSize*(s->getPrice()), m_pclient, (double &) amountBought);
		}
	} else {
		if (curState == FAST_BELOW_SLOW) {
			return; //no news
		} else {
			curState = FAST_BELOW_SLOW;
			emacFile << "Strength of crossover is " << degrees << std::endl;
			//time to sell
			s->placeOrder("SELL", orderSize*(s->getPrice()), m_pclient, (double &) amountBought);
		}
	}
}

void EMACrossover::doEMACrossover(double current_money, void *m_pclient) {
	//check if values are valid

	emacFile << "Curstate is " << states[getState()] << std::endl;

	if (!macd->isValid()) {
		return;
	}

	double curPrice = s->getPrice();
	double degrees = riskManagement();
	
	//init.
	if (curState == INVALID) {
		curState = macd->getMACD() > 0 ? FAST_ABOVE_SLOW : FAST_BELOW_SLOW;
		prevFast = macd->getFast();
		prevSlow = macd->getSlow();
		return;
	}

	if (macd->getMACD() > 0) {
		if (curState == FAST_ABOVE_SLOW) {
			prevFast = macd->getFast();
			prevSlow = macd->getSlow();
			return; //no news 
		} else {
			curState = FAST_ABOVE_SLOW;
			emacFile << "Strength of crossover is " << degrees << std::endl;
			//time to buy!
			if (degrees > MIN_CROSSOVER_STRENGTH) {
				s->placeOrder("BUY", orderSize*(s->getPrice()), m_pclient, (double &) amountBought);
				emacFile << "Buying order " << orderSize << " at price " << s->getPrice() << std::endl;
			}
		}
	} else {
		if (curState == FAST_BELOW_SLOW) {
			prevFast = macd->getFast();
			prevSlow = macd->getSlow();
			return; //no news
		} else {
			curState = FAST_BELOW_SLOW;
			emacFile << "Strength of crossover is " << degrees << std::endl;
			//time to sell or short if we can!
			if (degrees > MIN_CROSSOVER_STRENGTH) { 
				s->placeOrder("SELL", orderSize*(s->getPrice()), m_pclient, (double &) amountBought);
				emacFile << "Selling order " << orderSize << " at price " << s->getPrice() << std::endl;
			}
		}
	}
	prevFast = macd->getFast();
	prevSlow = macd->getSlow();

	return;
}

//returns the crossover strength in degres. You can specify what a "strong enough" crossover by taking arccos
double EMACrossover::riskManagement() { 
	double fast = macd->getFast();
	double slow = macd->getSlow();
	double dotProduct = 4.25 + (fast-prevFast)*(slow - prevSlow)/4;
	double magFast = sqrt(pow(2.5, 2) + pow((fast-prevFast)/2, 2));
	double magSlow = sqrt(pow(2.5, 2) + pow((slow-prevSlow)/2, 2));
	double angle = std::abs(dotProduct/(magFast*magSlow));
	double pi = atan((double) 1) * 4;
	double degrees = angle * 180/ pi;
	return degrees;
}

double EMACrossover::stopWin(double fast, double  d1) {
	return fast + d1;
}

double EMACrossover::stopLoss(double slow, double d2)  {
	return slow - d2;
}

