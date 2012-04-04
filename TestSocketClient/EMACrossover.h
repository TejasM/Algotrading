#ifndef EMACROSSOVER
#define EMACROSSOVER

/*
 * EMA Crossover class
 */

#include "Stock.h"

typedef enum {
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

    private:
		bool withStop;
        Stock *s;
		int macdID, orderSize;
        state curState;
		double curMACD;
		double stopWin(double fast, double d1);
		double stopLoss(double slow, double d2);
		double d1, d2;
};

#endif