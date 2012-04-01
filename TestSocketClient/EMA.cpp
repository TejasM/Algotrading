#include "EMA.h"


/* EMA */
EMA::EMA(int numPeriods) :  curEMA(0), validCount(numPeriods)
{
	alpha = 2/(numPeriods+1); 
}

EMA::~EMA()
{
}

bool EMA::isValid() {
	return (validCount <= 0);
}

double EMA::calculateEMA(double curVal) {
	curEMA = curVal * alpha + curEMA * (1 - alpha); //calculate EMA, store it
	if (validCount > 0) {
		validCount--;
	}
	return curEMA;
}

double EMA::getEMA() {
	return curEMA;
}

//MACD constructor
MACD::MACD(int slowPeriod, int fastPeriod, int signalPeriod) : slow(slowPeriod), fast(fastPeriod), signal(signalPeriod), curMACD(0), curHistogram(0), curSignal(0)
{
}

MACD::~MACD() 
{
}

double MACD::calculateMACD(double curVal)
{
	curMACD = fast.calculateEMA(curVal) - slow.calculateEMA(curVal);
	curSignal = signal.calculateEMA(curMACD);
	curHistogram = curMACD - curSignal;
	return curMACD;
}

double MACD::getMACD() {
	return curMACD;
}

double MACD::getHistogram() {
	return curHistogram;
}

double MACD::getSignal() {
	return curSignal;
}

bool MACD::isValid()
{
	return (signal.isValid() && slow.isValid()); //both signal and slow must be valid
}

double MACD::getFast() {
	return fast.getEMA();
}

double MACD::getSlow() {
	return slow.getEMA();
}