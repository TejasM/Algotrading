#include "EMA.h"
/*  
 * EMA Class - see EMA.h for more information
 */
using namespace std;

EMA::EMA(int numPeriods) 
	:  curEMA(-1), //start invalid
	validCount(numPeriods)
{
	alpha = ((double)2)/((double)numPeriods+(double)1); 
}

EMA::~EMA()
{
}

bool EMA::isValid() {
	return (validCount <= 0);
}

double EMA::calculateEMA(double curVal) {
	if (curEMA < 0 ) {
		curEMA = curVal; //set initial EMA to the price at time of init
	} else {
		curEMA = curVal * alpha + curEMA * (1 - alpha); //calculate EMA recursively
	}
	if (validCount > 0) {
		validCount--;
	}
	return curEMA;
}

double EMA::getEMA() {
	return curEMA;
}

//MACD constructor
MACD::MACD(int slowPeriod  , int fastPeriod , int signalPeriod) 
	: slow(slowPeriod), 
	fast(fastPeriod), 
	signal(signalPeriod), 
	curMACD(-1), 
	curHistogram(-1), 
	curSignal(-1)
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
	return (signal.isValid() && slow.isValid()); //both signal and slow must be valid for macd to be valid
}

double MACD::getFast() {
	return fast.getEMA();
}

double MACD::getSlow() {
	return slow.getEMA();
}