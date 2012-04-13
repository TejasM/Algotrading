#ifndef EMACROSSOVER
#define EMACROSSOVER

/*
 * EMA Crossover class
 * This class implements the EMA crossover strategy, both with and without stop. 
 * The sign of a MACD is used to determine whether or not fast is above slow, since MACD is just fast - slow
 * When a switch occurs, a buy or sell signal is triggered. If a stop is specified, the order will be sold at the stop.
 * If a stop is unspecified, the order is reversed at the next crossover. A crossover will only be responded to if its slope is steep enough, as measured in 
 * degrees. 
 */

#include "Stock.h"
#include<fstream>

#define	WAITING_FOR_STOP 0
#define FAST_ABOVE_SLOW 1
#define FAST_BELOW_SLOW 2
#define	INVALID			3

#define MIN_CROSSOVER_STRENGTH 30 //this is how strong a crossover needs to be for us to respond to it

class EMACrossover {
    public:
        EMACrossover(Stock *_s, int _macdID, int _orderSize, double _d1, double _d2); //constructor for stop
		EMACrossover(Stock *_s, int _macdID, int _orderSize); //constructor for no stop
        ~EMACrossover();
        int getState(); 
        void doEMACrossover(double current_money, void *m_pclient);
		void doEMACrossoverWithStop(double current_money, void *m_pclient);
    private:
		void initCommon(); //for parts of constructor common to both stop / no stop
        Stock *s;
		long orderSize;
		MACD *macd; 
		double amountBought; //amount of stock you currently hold
        int curState;
		double stopWin(double fast, double d1);
		double stopLoss(double slow, double d2);
		double d1, d2;
		double prevFast, prevSlow;
		double riskManagement();
		std::ofstream emacFile;
};

#endif