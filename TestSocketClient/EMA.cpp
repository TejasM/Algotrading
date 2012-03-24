/* EMA */

#include <iostream>

using namespace std;

/* The EMA Class. Used to calculate an exponential moving average. The class is for computation only, and has no notion of what stock it is being used for */

class EMA {
	double prevEMA;
	int validCount; //used to calculate whether or not EMA is valid because not enough computations have been done. Counts down from numPeriods
	double alpha; //ema constant
public:
	EMA(int numPeriods);
	~EMA();
	double getEMA(double curVal); //compute the next sequence in EMA based on stored previous value and passed in paramter
	bool isValid(); //is the EMA valid
};

//EMA constructor
EMA::EMA(int numPeriods) :  prevEMA(0), validCount(numPeriods)
{
	alpha = 2/(numPeriods+1); 
}

EMA::~EMA()
{
}

bool EMA::isValid() {
	return (validCount <= 0);
}

double EMA::getEMA(double curVal) {
	prevEMA = curVal * alpha + prevEMA * (1 - alpha); //calculate EMA, store it
	if (validCount > 0) {
		validCount--;
	}
}

/* MACD computation class. Contains 3 EMA's, outputs histogram and macd.
 * defaults to 12/26/9 
 * MACD is EMAfast - EMAslow
 * Signal is an EMA of MACD
 * Histogram is MACD - signal
 */

class MACD {
	EMA slow; //a slow EMA, traditionally 26
	EMA fast; //a fast EMA, traditionally 12
	EMA signal; //EMA of EMAfast – EMAslow, traditionally 9. 
public:
	MACD(int slowPeriod = 12, int fastPeriod = 26, int signalPeriod = 9); //default args just call MACD()
	//Call this function with slowValue, fastValue. Pass in references to assign macd/signal/histogram. IF invalid, NULL
	void getMACD(double slowValue, double fastValue, double *macd, double *signal, double *histogram);
	~MACD();
	bool isValid();
};

//MACD constructor
MACD::MACD(int slowPeriod, int fastPeriod, int signalPeriod) : slow(slowPeriod), fast(fastPeriod), signal(signalPeriod)
{
}

MACD::~MACD() 
{
}

void MACD::getMACD()
{
	unsigned int i = 0;
	//macd[i] = fast->data[i] - slow->data[i];
	//histogram[i] = macd[i] - signal->data[i];
}


bool MACD::isValid()
{
	return (signal.isValid() && slow.isValid()); //both signal and slow must be valid
}