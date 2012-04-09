/*
 * Algorithmic Pairs Trading class
 */

#ifndef PAIRSTRADING_H
#define PAIRSTRADING_H

#include "Stock.h"
#include<fstream>
//#include "EClientSocket.h" 
#include "utils.h"



// See Design Document for threshold terminology
#define T1 0.0001
#define T2 0.0003

// The state the pair is currently in
#define UNCORRELATED 1
#define CORRELATED 2
#define DIVERGED 3
#define FAILURE 4

// time steps we need to be correlated
#define correlatedThreshold 3

// PairsTrading class

class PairsTrading {
	public:
		PairsTrading(Stock * _s1, int EMA_id1, Stock * _s2, int EMA_id2, int i);
		~PairsTrading();
		int getState(); // not used
		void doPairsTrading(double current_money, void *m_pclient);

	private:
		// s1 and s2 form an algorithmic trading pair
		Stock * s1, * s2;
		// store info for each stock

		struct StockInfo {
			int id; // EMA id (for getEMA(id))
			// Also make an ID for the 
			double initialEMA; // at start of state 1
			double currentEMA;
			double percentChange;
			double EMAatDivergence;
			double amountInvested;
			int idListBase;
			int idListTop;

			double AmountBought; // IN SHARES. -ve means shorted
			std::map<int, std::string> OrderType; 
			std::map<int, double> OrderAmount; 
		};

	

		StockInfo s1Data, s2Data;

		int state;
		int correlatedCount; // seconds we've been correlated
		int divergedCount; // seconds we've been diverged

		double EMAdifference; // current EMA difference (%)

		bool calculateDiff();
		void State1();
		void State2();
		void State3(double current_money, void *m_pclient);
		void State4(void *m_pclient);
		
		double getInvestmentAmount (std::string order, 
				double diff, int count, double current_money);

		// For debugging
		std::ofstream fTest;
};

#endif
