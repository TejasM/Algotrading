/*
 * Stock class
 */

#include "Stock.h"
#include<fstream>
#include<ctime>
#define timestep 1

using namespace std;

// constructor
Stock::Stock(string _tick) {

	// initialize tick and times for EMA and MACD
	_tick = tick;
	curPrice = -1;

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

Stock::newEMA(int num_periods, int id) {

	// Update the EMAs map
	EMA * newEMA = new EMA (num_periods);
	EMAs[id] = newEMA;
	
	// Update current EMA map
	curEMA[id] = -1;
	
	// Update file map
	string filename = this->tick + "_EMA_Time";
	char temp [10];
	itoa(timestep, temp, 10);
	filename += temp;
	filename += "s.txt";
	fEMA.at(i).open(filename.c_str());

	ofsream * out = new ofstream(filename.c_str());
//	fEMA.insert( make_pair(id, out) ); // don't need this
	fEMA[id] = out;
	fEMA[id] << "EMA" << endl;
}


Stock::newMACD(int id) {

	// Update the MACD map
	MACD * newMACD= new MACD ();
	MACDs[id] = newMACD;

	// Update current MACD map
	curMACD[id] = -1;
	
	// Update file map
	string filename = this->tick + "_MACD_Time";
	char temp [10];
	itoa(timestep, temp, 10);
	filename += temp;
	filename += "s.txt";
	fMACD.at(i).open(filename.c_str());

	ofsream * out = new ofstream(filename.c_str());
	fMACD[id] = out;
	fMACD[id] << "MACD\tHistogram\tSignal" << endl;
}


double Stock::getEMA(int index) {
	if (curEMA.find(index) != curEMA.end()) {
		return curEMA[index];
	}
	return -1;
}

double Stock::getMACD() {
	if (curMACD.find(index) != curMACD.end()) {
		return curMACD[index];
	}
	return -1;
}

double Stock::getPrice() {
	return curPrice;
}

string Stock::getTick() {
	return tick;
}

void Stock::update(int id, double price) {

	curPrice = price;

	// write to file
	fPrice << curPrice << endl;

	// Look for the EMA corresponding to this id
	if (EMAs.find(id) != EMAs.end()) {
		curEMA[id] = -1;
		curEMA[i] = EMAs[i]->calculateEMA(price);
		if (EMAs[i]->isValid()) {
			// write to file
			fEMA[i] << curEMA[i] << endl;
		}
	}

	// Look for the MACD corresponding to this id
	if (MACDs.find(id) != MACDs.end()) {
		curMACD[id] = -1;
		curMACD[i] = MACDs[i]->calculateMACD(price);
		if (MACDs[i]->isValid()) {
			double histogram = MACDs[i]->getHistogram();
			double signal = MACDs[i]->getSignal();
			// write to file
			fMACD[i] << curMACD[i] << "\t" << histogram << "\t" << signal << endl;
		}
	}
}
