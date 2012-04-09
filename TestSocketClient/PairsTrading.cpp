/*
* Algorithmic Pairs Trading class
*
* See design document for more information. This file implements the
* Finite State Machine described in that document and also calculates
* order amounts and places orders.
*
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

PairsTrading::PairsTrading(Stock * _s1, int EMA_id1, Stock * _s2, int EMA_id2, int i) {
	s1 = _s1;    
	s2 = _s2;

	// Gather initial EMAs
	s1Data.id = EMA_id1;
	s2Data.id = EMA_id2;
	s1Data.initialEMA = s1->getCurEMA(s1Data.id);
	s2Data.initialEMA = s2->getCurEMA(s2Data.id);
	s1Data.idListBase = 10000*(2*i + 1);
	s1Data.idListTop = s1Data.idListBase;
	s2Data.idListBase = 20000*i + 20000;
	s2Data.idListTop = s2Data.idListBase;
	
	s1Data.AmountBought = 0;
	s2Data.AmountBought = 0;
	
	// stocks are uncorrelated
	state = UNCORRELATED;
	correlatedCount = 0; // been correlated for 0 seconds
	divergedCount = 0;
	
	// Output test file
	fTest.open("debug.txt");
}

PairsTrading::~PairsTrading() {
	// delete heap memory
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

		double Amount_to_invest = current_money * MIN(0.01, 0.001 * count);
		return Amount_to_invest;
}

// return current state
int PairsTrading::getState() {
	return state; //this->state
}

// Use absolute EMA difference to find percent change
// Returns false if not yet valid
bool PairsTrading::calculateDiff() {
	s1Data.currentEMA = s1->getCurEMA(s1Data.id);
	s2Data.currentEMA = s2->getCurEMA(s2Data.id);

	if ( (s1Data.currentEMA < 0) || (s2Data.currentEMA < 0) ) {
		return false;
	}

	// calculate percent change of each
	s1Data.percentChange = (s1Data.currentEMA - s1Data.initialEMA) / 100;
	s2Data.percentChange = (s2Data.currentEMA - s2Data.initialEMA) / 100;

	// calculate overall difference
	EMAdifference = std::abs(s1Data.percentChange - s2Data.percentChange);
}

// handle state 1, UNCORRELATED
// Wait for correlation then move to state 2
void PairsTrading::State1() {

	// Just starting, so update initial EMAs
	if (correlatedCount == 0) {
		s1Data.initialEMA = s1->getCurEMA(s1Data.id);
		s2Data.initialEMA = s2->getCurEMA(s2Data.id);
	}

	// need to be correlated for correlatedThreshold timesteps to go to state 2
	if (EMAdifference < T1) {
		correlatedCount++;
	}
	else {
		correlatedCount = 0;
	}

	if (correlatedCount >= correlatedThreshold) {
		state = CORRELATED;

		// debugging only
		fTest << std::endl << "CORRELATED. stock 1 EMA@Div:" << s1Data.initialEMA << ", s2 EMA@Div: " << s2Data.initialEMA << std::endl;
	}
}

// handle state 2, CORRELATED
void PairsTrading::State2() {

	// Wait until state change
	if (EMAdifference > T2) {
		state = FAILURE;
	}
	else if (EMAdifference > T1) {
		// update so it's always the value right before we diverge
		s1Data.EMAatDivergence = s1->getCurEMA(s1Data.id);
		s2Data.EMAatDivergence = s2->getCurEMA(s2Data.id);
		state = DIVERGED;
	}
}

// handle state 3, DIVERGED (we exploit this)
// Based on how long we have been diverged (i.e. EMAdifference between
// thresholds T1 and T2), place an order on each stock. The type of order
// depends on if the stock is rising or falling.
void PairsTrading::State3(double current_money,void *m_pclient) {

	// if the algorithm has failed or is back to the uncorrelated region
	if ( (EMAdifference > T2) || (EMAdifference < T1) || (divergedCount >= 100) ) {

		// undo all trades
		State4(m_pclient);

		// reset counts
		divergedCount = 0;

		// if we return from divergence, we are still correlated
		if (EMAdifference < T1) {
			state = CORRELATED;
		}
		// else failed because we are uncorrelated
		else {
			correlatedCount = 0;
			state = UNCORRELATED;
		}

		return;
	}

	// Increment how long we have been diverged so that we know to increase
	// the amount to invest this time step
	divergedCount++;

	s1Data.percentChangeSinceDiv = (s1Data.currentEMA - s1Data.EMAatDivergence) / 100;
	s2Data.percentChangeSinceDiv = (s2Data.currentEMA - s2Data.EMAatDivergence) / 100;

	// start buying/selling

	// if stock 1 is falling, buy it
	if (s1Data.percentChange < s2Data.percentChange) {

		// BUY STOCK 1, since we expect it to rise
		double buyAmount = getInvestmentAmount ("buy",
			s1Data.currentEMA - s1Data.EMAatDivergence,
			divergedCount, current_money);

		s1->placeOrder("BUY", buyAmount, m_pclient, s1Data.idListTop, s1Data.AmountBought);

		s1Data.OrderType[s1Data.idListTop] = "SELL";
		s1Data.OrderAmount[s1Data.idListTop] = buyAmount;
		s1Data.idListTop++;

		// SELL STOCK 2
		double sellAmount = getInvestmentAmount ("sell",
			s2Data.currentEMA - s2Data.EMAatDivergence,
			divergedCount, current_money
			);
		
		// placeOrder returns false if the stock is not shortable
		if (s2->placeOrder("SELL", sellAmount, m_pclient, s2Data.idListTop, s2Data.AmountBought)) {
			s2Data.OrderType[s2Data.idListTop] = "BUY";
			s2Data.OrderAmount[s2Data.idListTop] = sellAmount;
			s2Data.idListTop++;
		}
		else {
			s2Data.OrderType[s2Data.idListTop] = "NONE";
			s2Data.OrderAmount[s2Data.idListTop] = 0;
		}
		
	}
	else {

		// BUY STOCK 2
		double buyAmount = getInvestmentAmount ("buy",
			s2Data.currentEMA - s2Data.EMAatDivergence,
			divergedCount, current_money
			);
		s2->placeOrder("BUY", buyAmount, m_pclient, s2Data.idListTop, s2Data.AmountBought);

		s2Data.OrderType[s2Data.idListTop] = "SELL";
		s2Data.OrderAmount[s2Data.idListTop] = buyAmount;
		s2Data.idListTop++;

		// SELL STOCK 1
		double sellAmount = getInvestmentAmount ("sell",
			s1Data.currentEMA - s1Data.EMAatDivergence,
			divergedCount, current_money
			);

		// placeOrder returns false if the stock is not shortable
		if (s1->placeOrder("SELL", sellAmount, m_pclient, s1Data.idListTop, s1Data.AmountBought)) {
			s1Data.OrderType[s1Data.idListTop] = "BUY";
			s1Data.OrderAmount[s1Data.idListTop] = sellAmount;
		}
		else {
			s1Data.OrderType[s1Data.idListTop] = "NONE";
			s1Data.OrderAmount[s1Data.idListTop] = 0;
		}
		s1Data.idListTop++;
	}
}


// State 4 - undoes all orders (i.e. buy back everything shorted or sell
// everything bought) while in the diverged region
//
// This state is entered for two reasons:
//
//	1. Upon algorithm failure (if EMAdifference increases above T2 or we
//	   are diverged for too long)
//  2. Upon reconvergence (i.e. profit will now be made)
//
void PairsTrading::State4(void *m_pclient) {
	// undo all trades by looking up the trade type in the OrderType and
	// OrderAmount maps for each stock and making an opposite order

	// Undo every order on stock 1 since divergence
/*
	int temp = s1Data.idListTop;
	for(int i = s1Data.idListBase; i <= temp; i++){
		placeOrder(s1, s1Data.OrderType[i], s1->getTick(), s1Data.OrderAmount[i], m_pclient, s1Data.idListTop++);
	}
	// Undo every order on stock 2 since divergence
	temp = s2Data.idListTop;
	for(int i = s2Data.idListBase; i <= temp ; i++){
		placeOrder(s2, s2Data.OrderType[i], s2->getTick(), s2Data.OrderAmount[i], m_pclient, s2Data.idListTop++);
	}
*/
	std::string order;
	if (s1Data.AmountBought > 0) order = "SELL";
	else order = "BUY";
	s1Data.idListTop++;
	s1->placeOrder(order, std::abs(s1Data.AmountBought), m_pclient, s1Data.idListTop, s1Data.AmountBought);

	if (s2Data.AmountBought > 0) order = "SELL";
	else order = "BUY";
	s2Data.idListTop++;
	s2->placeOrder(order, std::abs(s2Data.AmountBought), m_pclient, s2Data.idListTop, s2Data.AmountBought);

//	s1Data.AmountBought = 0;
//	s2Data.AmountBought = 0;

	// Clear the maps for the next time we diverge
	s1Data.OrderType.empty();
	s1Data.OrderAmount.empty();
	s2Data.OrderType.empty();
	s2Data.OrderAmount.empty();

	// And increment to new order numbers
	s1Data.idListBase = s1Data.idListTop+1;
	s1Data.idListTop = s1Data.idListBase;
	s2Data.idListBase = s2Data.idListTop+1;
	s2Data.idListTop = s2Data.idListBase;
}

// Update the current EMAdifference (see design documentation), perform
// required steps at the current state and then find the next FSM state
void PairsTrading::doPairsTrading(double current_money, void *m_pclient) {

	// Calculate EMAdifference and return if EMAs are not yet valid
	if (calculateDiff() == false) return;

	// Debugging only
	fTest << "Entering state " << state << ", EMA diff: " << EMAdifference << ", ccount: " <<  correlatedCount; 

	// handle current state
	if (state == 1) State1();
	else if (state == 2) State2();
	else if (state == 3) State3(current_money, m_pclient);
	else State4(m_pclient);

	// Debugging only
	fTest << ". Leaving state " << state << std::endl; 
}
