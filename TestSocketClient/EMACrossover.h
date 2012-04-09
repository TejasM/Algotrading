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

#define MIN_CROSSOVER_STRENGTH 5 //this is how strong a crossover needs to be for us to respond to it

class EMACrossover {
    public:
        EMACrossover(Stock *_s, int _macdID, int _orderSize, double _d1, double _d2); //constructor for stop
		EMACrossover(Stock *_s, int _macdID, int _orderSize); //constructor for no stop
        ~EMACrossover();
        state getState(); 
        void doEMACrossover(double current_money, void *m_pclient);
		void doEMACrossoverWithStop(double current_money, void *m_pclient);
		void emacontractDefine( void * newCon, int id, char * stock, char *exchange, char *primaryExchange, char *currency, double strike, bool includeExpired, char *secType );
    private:
		void initCommon(); //for parts of constructor common to both stop / no stop
        Stock *s;
		long orderSize;
		MACD *macd; 
		long amountBought;
        state curState;
		double stopWin(double fast, double d1);
		double stopLoss(double slow, double d2);
		double d1, d2;
		double prevFast, prevSlow;
		double riskManagement();
		bool placeOrder(Stock *stock, std::string order, std::string tick, double amount, void *m_pClient, int nextid);
		std::ofstream emacFile;
		double amountInvested;
		std::map<int, std::string> OrderType; 
		std::map<int, double> OrderAmount; 
};

#endif