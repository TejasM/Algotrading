/*
* Algorithmic Pairs Trading class
*/

#include<iostream>
#include<cmath>
//#include<string>
#include "Contract.h"
#include "EClientSocket.h" 
#include "PairsTrading.h"
#include "Order.h"

//using namespace std;


// PairsTrading Class

PairsTrading::PairsTrading(Stock * _s1, int EMA_id1, Stock * _s2, int EMA_id2) {
	s1 = _s1;    
	s2 = _s2;

	// Gather initial EMAs
	s1Data.id = EMA_id1;
	s2Data.id = EMA_id2;
	s1Data.initialEMA = s1->getCurEMA(s1Data.id);
	s2Data.initialEMA = s2->getCurEMA(s2Data.id);
	idListBase = 10000;
	idListTop = idListBase;
	// stocks are uncorrelated
	state = UNCORRELATED;
	int correlatedCount = 0; // been correlated for 0 seconds

}

PairsTrading::~PairsTrading() {
	// delete heap memory
}

void contractDefine( Contract * newContract, int id, char * stock, char *exchange, char *primaryExchange, char *currency, double strike, bool includeExpired, char *secType ) 
{
	newContract->conId = id;
	newContract->symbol = stock;
	newContract->exchange = exchange;
	newContract->primaryExchange = primaryExchange;
	newContract->currency = currency;
	newContract->strike = strike;
	newContract->includeExpired = includeExpired;
	newContract->secType = secType;
}

// Should be in another function!
void PairsTrading::placeOrder(Stock *stock, std::string order, std::string tick, double amount, void *m_pClient) {
	Order *newOrder = new Order();
	Contract *newContract = new Contract();
	
	
	contractDefine(newContract, idListTop, (char *) tick.c_str(),"SMART", "ISLAND", "USD", 0, false, "STK" );
	

	newOrder->action = (char *) order.c_str();
	newOrder->orderId = idListTop;
	newOrder->totalQuantity =(long) (amount)/(stock->getPrice());
	newOrder->lmtPrice = stock->getPrice();
	newOrder->orderType = "LMT";
	
	

	((EClient*) m_pClient)->placeOrder(idListTop, *newContract, *newOrder);
	idListTop++;
}


double PairsTrading::getInvestmentAmount (std::string order, 
	double diff, int count, double current_money) {

		/*
		Consider:
		- Amount of money we currently have
		- How long we've been diverged
		? Difference between amount at divergence and now? (e.g. * diff/T2)

		Example function:
		*/

		double Amount_to_invest = current_money * MIN(0.01, 0.0001 * count);
		return Amount_to_invest;
}

// return current state
int PairsTrading::getState() {
	return state; //this->state
}

// Use absolute EMA difference to find percent change
void PairsTrading::calculateDiff() {
	s1Data.currentEMA = s1->getCurEMA(s1Data.id);
	s2Data.currentEMA = s2->getCurEMA(s2Data.id);

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
		s1Data.initialEMA = s1->getCurEMA(s1Data.id);
		s2Data.initialEMA = s2->getCurEMA(s2Data.id);
	}

	if (correlatedCount >= correlatedThreshold) {
		state = CORRELATED;

		// in case we diverge right away
		s1Data.EMAatDivergence = s1->getCurEMA(s1Data.id);
		s1Data.EMAatDivergence = s2->getCurEMA(s2Data.id);
	}
}

// handle state 2, CORRELATED
void PairsTrading::State2() {

	// Wait until state change
	if (EMAdifference > T2) {
		state = FAILURE;
	}
	else if (EMAdifference > T1) {
		state = DIVERGED;
	}
	else {
		// update so it's always the value right before we diverge
		s1Data.EMAatDivergence = s1->getCurEMA(s1Data.id);
		s1Data.EMAatDivergence = s2->getCurEMA(s2Data.id);
	}
}

// handle state 3, DIVERGED (we exploit this)
void PairsTrading::State3(double current_money,void *m_pclient) {

	// algorithm failure
	if (EMAdifference > T2) {
		divergedCount = 0;
		state = FAILURE;
		return;
	}

	divergedCount++;

	// start buying/selling

	// if stock 1 is falling
	if (s1Data.currentEMA < s1Data.EMAatDivergence) {
		// part of Risk management module (separate class? inputs?)
		// or we could make one risk management module for just this algorithm
		double buyAmount = getInvestmentAmount ("buy",
			s1Data.currentEMA - s1Data.EMAatDivergence,
			divergedCount, current_money);

		// part of order management module
		placeOrder(s1, "BUY", s1->getTick(), buyAmount, m_pclient);
	}
	else { // stock 1 is rising
		double sellAmount = getInvestmentAmount ("sell",
			s1Data.currentEMA - s1Data.EMAatDivergence,
			divergedCount, current_money
			);
		placeOrder(s1, "SELL", s1->getTick(), sellAmount, m_pclient);
	}

	// if stock 2 is falling
	if (s2Data.currentEMA < s2Data.EMAatDivergence) {
		// part of Risk management module (separate class? inputs?)
		// or we could make one risk management module for just this algorithm
		double buyAmount = getInvestmentAmount ("buy",
			s2Data.currentEMA - s2Data.EMAatDivergence,
			divergedCount, current_money
			);
		placeOrder(s2, "BUY", s2->getTick(), buyAmount, m_pclient);
	}
	else { // stock 2 is rising
		double sellAmount = getInvestmentAmount ("sell",
			s2Data.currentEMA - s2Data.EMAatDivergence,
			divergedCount, current_money
			);
		placeOrder(s2, "SELL", s2->getTick(), sellAmount, m_pclient);
	}
}

// handle state 4, FAILED
void PairsTrading::State4(void *m_pclient) {
	state = 1;
	// sell everything, call methods in order management module
}

// Begin algorithmic trading
void PairsTrading::doPairsTrading(double current_money, void *m_pclient) {

	// update EMAdifference (see design documentation) and find state
	calculateDiff();

	// handle current state
	if (state == 1) State1();
	else if (state == 2) State2();
	else if (state == 3) State3(current_money, m_pclient);
	else State4(m_pclient);
}
