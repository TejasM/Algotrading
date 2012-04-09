/*
 * Stock class
 */

#include "Stock.h"
//#include "winbase.h"

using namespace std;

// constructor
Stock::Stock(string _tick) {

	// initialize tick and times for EMA and MACD
	tick = _tick;
	curPrice = -1;
	shortable = false;
	// Create and write to files
	string filename;

	// Price, ofstream name fPrice, filename RIM_Price.txt
	filename = this->tick + "_Price.txt";
	fPrice.open(filename.c_str());
	fPrice << "Price" << endl;
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
	string filename = this->tick + "_EMA_Time";
	char temp [10];
	itoa(num_periods, temp, 10);
	filename += temp;
	filename += "s.txt";

	ofstream * out = new ofstream(filename.c_str());
//	fEMA.insert( make_pair(id, out) ); // don't need this
	fEMA[id] = out;
	*fEMA[id] << "EMA" << endl;
}


void Stock::newMACD(int id, int slowPeriods, int fastPeriods, int signalPeriods) {

	// Update the MACD map
	MACD * newMACD= new MACD (slowPeriods, fastPeriods, signalPeriods);
	MACDs[id] = newMACD;

	// Update current MACD map
	curMACD[id] = -1;
	
	// Update file map
	string filename = this->tick + "_MACD.txt";

	ofstream * out = new ofstream(filename.c_str());
	fMACD[id] = out;
	*fMACD[id] << "MACD\tHistogram\tSignal" << endl;
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

string Stock::getTick() {
	return tick;
}

void Stock::update(int id, double price) {

	curPrice = price;
	char text[100];
	/*sprintf(text, "I am here updating");
	MessageBox(text);*/
	// write to file
	if(fPrice.is_open()){
		fPrice << curPrice << endl;
	}else{
		
	}

	// Look for the EMA corresponding to this id
	if (EMAs.find(id) != EMAs.end()) {
		curEMA[id] = -1;
		curEMA[id] = EMAs[id]->calculateEMA(price);
		if (EMAs[id]->isValid()) {
			// write to file
			*fEMA[id] << curEMA[id] << endl;
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
			*fMACD[id] << curMACD[id] << "\t" << histogram << "\t" << signal << endl;
		}
		else curMACD[id]=-1;
	}
}
