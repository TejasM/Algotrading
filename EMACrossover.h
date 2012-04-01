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
        EMACrossover(Stock *_s, int macdID, int orderSize);
        ~EMACrossover();
        state getState(); 
        void doEMACrossover();

    private:
        Stock *s;
		int macdID, orderSize;
        state curState;
		double curMACD;
		void terminate();
		stopWin(double fast, double d1);
		stopLoss(double slow, double d2);
};

#endif