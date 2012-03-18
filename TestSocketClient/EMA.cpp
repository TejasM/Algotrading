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
	~EMA();
	void getEMA(); //in a file? a copy of my vector?
	bool isValid();
};

//EMA constructor
EMA::EMA(int barSize, int numPeriods) : barSize(barSize), numPeriods(numPeriods), isEMAValid(false) 
{
		alpha = 2/(numPeriods+1); 
		data = new vector <double> (100); //arbitrary default size
		//request real time bar data?
}

EMA::~EMA()
{
	delete data;
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
	EMA *signal;
	bool isMACDValid;
	vector <double> *histogram;
	vector <double> *macd;
	int slowBars;
	int fastBars;
	int slowPeriod;
	int fastPeriod;
	int macdPeriod;
	void calculateMACD();
public:
	MACD(int slowBars, int fastBars, int slowPeriod, int fastPeriod, int macdPeriod);
	~MACD();
	bool isValid();
};

//MACD constructor
MACD::MACD(int slowBars, int fastBars, int slowPeriod, int fastPeriod, int macdPeriod) : slowBars(slowBars), fastBars(fastBars), slowPeriod(slowPeriod), 
			fastPeriod(fastPeriod), macdPeriod(macdPeriod), isMACDValid(false), signal(NULL)
{
		histogram = new vector <double> (100); //arbitrary default size
		macd = new vector <double> (100);
		this->slow = new EMA(slowBars, slowPeriod);
		this->fast = new EMA(fastBars, fastPeriod);
}

void MACD::calculateMACD()
{
	unsigned int i = 0;
	//macd[i] = fast->data[i] - slow->data[i];
	//histogram[i] = macd[i] - signal->data[i];
}

MACD::~MACD() 
{
	delete slow; 
	delete fast;
	delete signal;
	delete histogram;
	delete macd;
}

bool MACD::isValid()
{
	return isMACDValid;
}