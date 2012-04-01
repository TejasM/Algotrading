/*
 * EMA class
 */

#ifndef EMA_H
#define EMA_H

#include <iostream>
using namespace std;

/* The EMA Class. Used to calculate an exponential moving average. The class is for computation only, and has no notion of what stock it is being used for */

class EMA {
	double curEMA;
	int validCount; //used to calculate whether or not EMA is valid because not enough computations have been done. Counts down from numPeriods
	double alpha; //ema constant
public:
	EMA(int numPeriods);
	~EMA();
	double calculateEMA(double curVal); //compute the next sequence in EMA based on stored previous value and passed in paramter
	double getEMA();
	bool isValid(); //is the EMA valid
};

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
	double curMACD;
	double curSignal;
	double curHistogram;
public:
	MACD(int slowPeriod = 12, int fastPeriod = 26, int signalPeriod = 9); //default args just call MACD()
	//Call this function with slowValue, fastValue. Pass in references to assign macd/signal/histogram. 
	double calculateMACD(double curVal);
	double getMACD();
	double getHistogram();
	double getSignal();
	double getFast(); 
	double getSlow();
	~MACD();
	bool isValid();
};

#endif
