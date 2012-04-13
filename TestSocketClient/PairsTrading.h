/*
* Algorithmic Pairs Trading class
*
* See design document for more information. This file implements the
* Finite State Machine described in that document and also calculates
* order amounts and places orders.
*
* State 1 - uncorrelated, wait for correlation
* State 2 - correlated, wait for divergence
* State 3 - diverged. Begin Buying/Selling until failure or reconvergence
* State 4 - Failure or Step before reconvergence: Undo all orders from step 3
*
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
		PairsTrading(Stock * _s1, int EMA_id1, Stock * _s2, int EMA_id2);
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

			// EMA at the start of state 1, needed to calculate %differences
			// (see below) and select next state
			double initialEMA;

			double currentEMA; // at each time step
	
			// Since initialEMA. This is used to calculate EMAdifference.
			double percentChange;
			
			// Used to calculate % change since divergence
			double EMAatDivergence; // NO LONGER USED

			// Used to determine whether to buy or sell
			double percentChangeSinceDiv; // NO LONGER USED
			
			// for placing order
			
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
