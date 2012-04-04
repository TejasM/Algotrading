#ifndef EMACROSSOVER
#define EMACROSSOVER

/*
 * EMA Crossover class
 */

#include "Stock.h"
#include<fstream>

typedef enum {
	WAITING_FOR_STOP,
	FAST_ABOVE_SLOW,
	FAST_BELOW_SLOW,
	INVALID
} state;

class EMACrossover {
    public:
        EMACrossover(Stock *_s, int _macdID, int _orderSize, double _d1, double _d2);
		EMACrossover(Stock *_s, int _macdID, int _orderSize);
        ~EMACrossover();
        state getState(); 
        void doEMACrossover();
		void doEMACrossoverWithStop();
    private:
		void initCommon();
        Stock *s;
		long orderSize;
		MACD *macd;
        state curState;
		double curMACD;
		double stopWin(double fast, double d1);
		double stopLoss(double slow, double d2);
		double d1, d2;
		double riskManagement();
		std::ofstream emacFile;
};

#endif