/* EMA */

class EMA {
	double *data; //contains old info used to calculate ema
	double prevEMA; //previous EMA
	double currEMA; //current EMA
	int numPeriods; //how long to wait before getting started
	bool isEMAValid; //does currEMA have garbage?
	double alpha; 
	void calculateEMA();
public:
	EMA(int barSize, int numPeriods);
	double getEMA();
	bool isValid();
};

EMA::EMA(int barSize, int numPeriods) {
		this->data = new double[numPeriods];
		numPeriods = numPeriods;
		barSize = barSize;
		isEMAValid = false;
		alpha = 2/(numPeriods+1); 
}

bool EMA::isValid() {
	return isEMAValid;
}


void EMA::calculateEMA() {
	double priceToday = 3; //how do I get this?
	currEMA = priceToday * alpha + prevEMA * (1 - alpha);
	numPeriods--;
}

double EMA::getEMA() {
	return this->currEMA;
}

class MACD {
	EMA *slow;
	EMA *fast;
	double signal;
public:
	MACD(int slowBars, int fastBars);
};

