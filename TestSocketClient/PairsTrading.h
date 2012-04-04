/*
 * Algorithmic Pairs Trading class
 */

#ifndef PAIRSTRADING_H
#define PAIRSTRADING_H

#include "Stock.h"
#include "EClientSocket.h" 
#include "utils.h"
#include "Contract.h"


// See Design Document for threshold terminology
#define T1 1
#define T2 2

// The state the pair is currently in
#define UNCORRELATED 1
#define CORRELATED 2
#define DIVERGED 3
#define FAILURE 4

// time steps we need to be correlated
#define correlatedThreshold 10

// PairsTrading class

class PairsTrading {
	public:
		PairsTrading(Stock * _s1, int EMA_id1, Stock * _s2, int EMA_id2);
		~PairsTrading();
		int getState(); // not used
		void doPairsTrading(double current_money, EClient *m_pclient);

	private:
		// s1 and s2 form an algorithmic trading pair
		Stock * s1, * s2;
		int idListBase;
		int idListTop;
		// store info for each stock
		struct StockInfo {
			int id; // EMA id (for getEMA(id))
			// Also make an ID for the 
			double initialEMA; // at start of state 1
			double currentEMA;
			double percentChange;
			double EMAatDivergence;
			double amountInvested;
		};

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

		StockInfo s1Data, s2Data;

		int state;
		int correlatedCount; // seconds we've been correlated
		int divergedCount; // seconds we've been diverged

		double EMAdifference; // current EMA difference (%)

		void calculateDiff();
		void State1();
		void State2();
		void State3(double current_money, EClient *m_pclient);
		void State4(EClient *m_pclient);
		void placeOrder(Stock *stock,std::string order, std::string tick, double amount, EClient *m_pClient);

		double getInvestmentAmount (std::string order, 
				double diff, int count, double current_money);
};

#endif
