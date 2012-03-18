/* EMA */

#include <iostream>
#include <vector>

using namespace std;

class EMA {
	vector <double> *data; //buffer for the past EMA values
	bool isEMAValid; //really shouldn't trust data until numPeriods have passed
	int numPeriods;
	int barSize; //how long to wait before updating
	double alpha; //ema constant
	void calculateEMA(); //updates data with ema values
public:
	EMA(int barSize, int numPeriods);
	void getEMA(); //in a file? a copy of my vector?
	bool isValid();
};

//EMA constructor
EMA::EMA(int barSize, int numPeriods) : barSize(barSize), numPeriods(numPeriods), isEMAValid(false) 
{
		alpha = 2/(numPeriods+1); 
		data->resize(100); //arbitrary default size
		//request real time bar data?
}

bool EMA::isValid() {
	return isEMAValid;
}

void EMA::calculateEMA() {
	double priceToday = 3; //how do I get this?, 
	data->push_back(priceToday * alpha + data->back() * (1 - alpha)); //calculate EMA, store it
	if (data->size() == numPeriods) {
		isEMAValid = true;
	}
}

void EMA::getEMA() {
	return; //not sure yet
}

class MACD {
	EMA *slow;
	EMA *fast;
	double signal;
	double histogram;
	bool isMACDValid;
	vector <double> *data;
	int slowBars;
	int fastBars;
	int slowPeriod;
	int fastPeriod;
public:
	MACD(int slowBars, int fastBars, int slowPeriod, int fastPeriod);
	double getHistogram();
	double getMACD();
	double getSignal();
};

//MACD constructor
MACD::MACD(int slowBars, int fastBars, int slowPeriod, int fastPeriod) : slowBars(slowBars), fastBars(fastBars), slowPeriod(slowPeriod),
			fastPeriod(fastPeriod), isMACDValid(false)
{
		data->resize(100); //arbitrary default size
		this->slow = new EMA(slowBars, slowPeriod);
		this->fast = new EMA(fastBars, fastPeriod);
}

