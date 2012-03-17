/* EMA */

using namespace std;
class CRectangle {
    int x, y;
  public:
    void set_values (int,int);
    int area () {return (x*y);}
};

void CRectangle::set_values (int a, int b) {
  x = a;
  y = b;
}
class EMA {
	double *ema = NULL;
	double currEMA;
	int numPeriods;
	bool isEMAValid = FALSE;
public:
	double getEMA();
}

double EMA::EMA(int barSize, int numPeriods) {
		this->arr = new(sizeof(double)*numPeriods);
		numPeriods = numPeriods;
		barSize = barSize;
}


EMA::calculateEMA() {
		if (num

}

	public double getEMA() {
		return this->currEMA;
	}


}



EMA = Price(Today)* k + EMA(y)* (1 – k),where k=2/(N+1)

class MACD {
	EMA *slow;
	EMA *fast;


}