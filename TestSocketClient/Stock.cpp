/*
 * Stock class
 */

#include "Stock.h"
#include "Order.h"
#include "Contract.h"
#include "EClientSocket.h" 
//#include "winbase.h"

//extern int idListTop =100000;
// constructor
int idListTop = 100000;
Stock::Stock(std::string _tick) {

	// initialize tick and times for EMA and MACD
	tick = _tick;
	curPrice = -1;
	shortable = false;
	// Create and write to files
	std::string filename;

	// Price, ofstream name fPrice, filename RIM_Price.txt
	filename = this->tick + "_Price.txt";
	fPrice.open(filename.c_str());
	fPrice << "Price" << std::endl;
}

Stock::~Stock() {
	// Need to delete everything we new
	// (MACD, EMA and ofstream objects)
	// and also close files

	fPrice.close();
}

void Stock::updateShortable(double value) {
	 if (value > 2.5) { // 3.0
       shortable = true;
  }
  else if (value > 1.5) { // 2.0
       shortable = false;
  }
  else {
	  shortable = false;
  }
}

void Stock::newEMA(int num_periods, int id) {

	// Update the EMAs map
	EMA * newEMA = new EMA (num_periods);
	EMAs[id] = newEMA;
	
	// Update current EMA map
	curEMA[id] = -1;
	
	// Update file map
	std::string filename = this->tick + "_EMA_Time";
	char temp [10];
	itoa(num_periods, temp, 10);
	filename += temp;
	filename += "s.txt";

	std::ofstream * out = new std::ofstream(filename.c_str());
//	fEMA.insert( make_pair(id, out) ); // don't need this
	fEMA[id] = out;
	*fEMA[id] << "EMA" << std::endl;
}


void contractDefine( Contract * newContract, int id, const char * stock, char *exchange, char *primaryExchange, char *currency, double strike, bool includeExpired, char *secType ) 
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

bool Stock::placeOrder(std::string order, double amount, 
		void *m_pClient, double & AmountBought) {


	Order *newOrder = new Order();
	Contract *newContract = new Contract();
	
	contractDefine(newContract,idListTop, this->tick.c_str(), "SMART", "ISLAND", "USD", 0, false, "STK" );

	newOrder->action = (char *) order.c_str();
	newOrder->orderId = idListTop;
	newOrder->totalQuantity =(long) (amount)/curPrice;
	newOrder->lmtPrice = curPrice;
	newOrder->orderType = "LMT";
	if(order == "BUY") {
		((EClient*) m_pClient)->placeOrder(idListTop, *newContract, *newOrder);
		AmountBought += newOrder->totalQuantity;
		idListTop++;
	}
	else if (order == "SELL") {
		if(shortable || (AmountBought*curPrice >= amount)) {
			((EClient*) m_pClient)->placeOrder(idListTop, *newContract, *newOrder);
			AmountBought -= newOrder->totalQuantity;
			idListTop++;
		}
		else return false;
	}
	return true;
}
	

void Stock::newMACD(int id, int slowPeriods, int fastPeriods, int signalPeriods) {

	// Update the MACD map
	MACD * newMACD= new MACD (slowPeriods, fastPeriods, signalPeriods);
	MACDs[id] = newMACD;

	// Update current MACD map
	curMACD[id] = -1;
	
	// Update file map
	std::string filename = this->tick + "_MACD.txt";

	std::ofstream * out = new std::ofstream(filename.c_str());
	fMACD[id] = out;
	*fMACD[id] << "MACD\tHistogram\tSignal" << std::endl;
}


double Stock::getCurEMA(int id) {
	if (curEMA.find(id) != curEMA.end()) {
		return curEMA[id];
	}
	return -1;
}

EMA * Stock::getEMA(int id) {
	if (EMAs.find(id) != EMAs.end()) {
		return EMAs[id];
	}
	return NULL;
}

double Stock::getCurMACD(int id) {
	if (curMACD.find(id) != curMACD.end()) {
		return curMACD[id];
	}
	return -1;
}

MACD * Stock::getMACD(int id) {
	if (MACDs.find(id) != MACDs.end()) {
		return MACDs[id];
	}
	return NULL;
}

double Stock::getPrice() {
	return curPrice;
}

std::string Stock::getTick() {
	return tick;
}

void Stock::update(int id, double price) {

	curPrice = price;
	char text[100];
	/*sprintf(text, "I am here updating");
	MessageBox(text);*/
	// write to file
	if(fPrice.is_open()){
		fPrice << curPrice << std::endl;
	}else{
		
	}

	// Look for the EMA corresponding to this id
	if (EMAs.find(id) != EMAs.end()) {
		curEMA[id] = -1;
		curEMA[id] = EMAs[id]->calculateEMA(price);
		if (EMAs[id]->isValid()) {
			// write to file
			*fEMA[id] << curEMA[id] << std::endl;
		}
		else curEMA[id]=-1;
	}

	// Look for the MACD corresponding to this id
	if (MACDs.find(id) != MACDs.end()) {
		curMACD[id] = -1;
		curMACD[id] = MACDs[id]->calculateMACD(price);
		if (MACDs[id]->isValid()) {
			double histogram = MACDs[id]->getHistogram();
			double signal = MACDs[id]->getSignal();
			// write to file
			*fMACD[id] << curMACD[id] << "\t" << histogram << "\t" << signal << std::endl;
		}
		else curMACD[id]=-1;
	}
}
