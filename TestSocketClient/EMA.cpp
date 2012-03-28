#include "EMA.h"


/* EMA */
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
	return prevEMA;
}

//MACD constructor
MACD::MACD(int slowPeriod, int fastPeriod, int signalPeriod) : slow(slowPeriod), fast(fastPeriod), signal(signalPeriod)
{
}

MACD::~MACD() 
{
}

bool MACD::getMACD(double slowValue, double fastValue, double *macd, double *sig, double *histogram)
{
	*macd = fast.getEMA(fastValue) - slow.getEMA(slowValue);
	*sig = signal.getEMA(*macd);
	*histogram = *macd - *sig;
	return isValid();
}


bool MACD::isValid()
{
	return (signal.isValid() && slow.isValid()); //both signal and slow must be valid
}