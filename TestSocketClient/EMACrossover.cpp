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

//getInvestment amount and placeOrder assumed. I don't see any way for me to get valid checks. 
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

void EMACrossover::initCommon() {
	// Create and write to files
	std::string filename;
	filename = s->getTick() + "_Crossover.txt";
	emacFile.open(filename.c_str());
	emacFile << "Current State is " << curState << std::endl;
	prevFast = -1;
	prevSlow = -1;
	idListTop = 100000;
	amountBought = 0;
}

EMACrossover::~EMACrossover() {
}

state EMACrossover::getState() {
    return curState; 
}

void EMACrossover::doEMACrossoverWithStop(double current_money, void *m_pclient) {

	//check if values are valid
	if (!macd->isValid()) {
		return;
	}
	
	double curPrice = s->getPrice();
	double degrees = riskManagement();

	//init. can't do in constructor b/c no macd valid
	if (curState == INVALID) {
		curState = macd->getMACD() > 0 ? FAST_ABOVE_SLOW : FAST_BELOW_SLOW;
		return;
	}

	//I need some way to signal to Tejas that I am finished, don't I?
	if (curState == WAITING_FOR_STOP) {
		if (curPrice > stopWin(macd->getFast(), d1)) {
			emacFile << "STOP WIN: Selling order " << orderSize << " at price " << s->getPrice() <<std::endl;
			s->placeOrder("SELL", orderSize*(s->getPrice()), m_pclient, idListTop, (double &) amountBought);
			return;
		} else if (curPrice < stopLoss(macd->getSlow(), d2)) {
			emacFile << "STOP LOSS: Selling order " << orderSize << " at price " << s->getPrice() <<std::endl;
			s->placeOrder("BUY", orderSize*(s->getPrice()), m_pclient, idListTop, (double &) amountBought);
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
			s->placeOrder("BUY", orderSize*(s->getPrice()), m_pclient, idListTop, (double &) amountBought);
		}
	} else {
		if (curState == FAST_BELOW_SLOW) {
			return; //no news
		} else {
			curState = FAST_BELOW_SLOW;
			emacFile << "Strength of crossover is " << degrees << std::endl;
			//time to sell
			s->placeOrder("SELL", orderSize*(s->getPrice()), m_pclient, idListTop, (double &) amountBought);
		}
	}
}

void EMACrossover::doEMACrossover(double current_money, void *m_pclient) {
	//check if values are valid

	emacFile << "Curstate is " << getState() << std::endl;

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
				s->placeOrder("BUY", orderSize*(s->getPrice()), m_pclient, idListTop, (double &) amountBought);
				emacFile << "Buying order " << orderSize << " at price " << s->getPrice() << std::endl;
				idListTop++;
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
			//Questions: what does the project sheet and the times 2 mean? 
			//What if I want to actually sell, and not short
			if (degrees > MIN_CROSSOVER_STRENGTH) { 
				s->placeOrder("SELL", orderSize*(s->getPrice()), m_pclient, idListTop, (double &) amountBought);
				emacFile << "Selling order " << orderSize << " at price " << s->getPrice() << std::endl;
				idListTop++;
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
	double degrees = angle * pi / 180;
	return degrees;
}

double EMACrossover::stopWin(double fast, double  d1) {
	return fast + d1;
}

double EMACrossover::stopLoss(double slow, double d2)  {
	return slow - d2;
}

void EMACrossover::emacontractDefine( void * newCon, int id, char * stock, char *exchange, char *primaryExchange, char *currency, double strike, bool includeExpired, char *secType ) 
{
	Contract *newContract = (Contract *) newCon;
	newContract->conId = id;
	newContract->symbol = stock;
	newContract->exchange = exchange;
	newContract->primaryExchange = primaryExchange;
	newContract->currency = currency;
	newContract->strike = strike;
	newContract->includeExpired = includeExpired;
	newContract->secType = secType;
}
