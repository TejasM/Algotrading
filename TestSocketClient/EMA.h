#ifndef EMA_H
#define EMA_H

/*
 * EMA class
 */

/* 
 * The EMA Class. Used to calculate an exponential moving average. The class is for computation only, and has no notion of what stock it is being used for.
 * The recursive algorithm is used to compute the average. The class does not use historical data; instead it waits for a number of data points equal to
 * the length of the EMA before declaring it valid.
 */

#include <iostream>

class EMA {
	double curEMA; //stores the most up to date EMA value
	int validCount; //used to calculate whether or not EMA is valid because not enough computations have been done. Counts down from numPeriods
	double alpha; //ema constant
public:
	EMA(int numPeriods);
	~EMA();
	double calculateEMA(double curVal); //compute the next sequence in EMA based on stored previous EMA and current price. Returns new EMA
	double getEMA();
	bool isValid();
};

/* MACD computation class. Contains 3 EMA's, outputs histogram and macd.
 * defaults to 12(fast) 26(slow) 9(signal)
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
	MACD(int slowPeriod = 26, int fastPeriod = 26, int signalPeriod = 9); //default args 
	double calculateMACD(double curVal); //calculates macd, histogram and signal based on their previous values and the current price. Returns new macd
	double getMACD();
	double getHistogram();
	double getSignal();
	double getFast(); //get the fast ema
	double getSlow(); //get the slow ema
	~MACD();
	bool isValid();
};

#endif
